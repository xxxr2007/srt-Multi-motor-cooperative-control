import argparse
import importlib.util
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch
import zipfile

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import review
import git_tools


def command(repo, *args):
    return review.git(repo, *args).decode().strip()


def model(path, blocks):
    path.parent.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(path, 'w') as z:
        z.writestr('simulink/blockdiagram.xml', '<ModelInformation><Model/></ModelInformation>')
        content = '<System>' + ''.join(f'<Block Name="{n}" BlockType="Gain" SID="{sid}"><P Name="Gain">{v}</P></Block>' for n,sid,v in blocks) + '</System>'
        z.writestr('simulink/systems/system_root.xml', content)


class ReviewTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)
        self.repo = self.root / 'repo'; self.repo.mkdir()
        command(self.repo, 'init', '-b', 'main')
        command(self.repo, 'config', 'user.name', 'Model review test')
        command(self.repo, 'config', 'user.email', 'test@example.invalid')
        command(self.repo, 'config', 'core.autocrlf', 'false')
        self.path = self.repo / 'electrical/simulink/车辆/vehicle.slx'
        model(self.path, [('GainA', '1', '1'), ('GainB','2','1')])
        self.text = self.path.with_suffix('.m'); self.text.write_text('x = 1;\n', encoding='utf-8')
        self.commit('baseline'); self.base = command(self.repo, 'rev-parse', 'HEAD')

    def tearDown(self):
        self.temp.cleanup()

    def commit(self, message):
        command(self.repo, 'add', '.'); command(self.repo, 'commit', '-m', message)

    def args(self, **kwargs):
        d = dict(repo=str(self.repo),mine='WORKTREE',theirs='teammate',base=None,
                 output=str(self.root/'report'),scope=None,matlab=None)
        d.update(kwargs)
        return argparse.Namespace(**d)

    def diverge(self):
        command(self.repo, 'checkout', '-b', 'teammate')
        model(self.path, [('GainA','1','3'), ('PeerAdded','3','1')])
        self.commit('peer edits')
        command(self.repo, 'checkout', 'main')
        model(self.path, [('GainA','1','2'), ('GainB','2','1'), ('MyAdded','4','1')])
        self.text.write_text('x = "<script>bad</script>";\n',encoding='utf-8')

    def test_divergent_sides_worktree_and_escaping(self):
        self.diverge()
        manifest = review.prepare(self.args())
        data = json.loads(manifest.read_text(encoding='utf-8'))
        self.assertEqual(data['commits']['base'],self.base)
        mine = next(x for x in data['changes'] if x['side']=='mine' and x['file'].endswith('.slx'))
        peer = next(x for x in data['changes'] if x['side']=='theirs')
        self.assertIn('MyAdded',mine['inventory']['added_paths'])
        self.assertNotIn('PeerAdded',mine['inventory']['added_paths'])
        self.assertIn('PeerAdded',peer['inventory']['added_paths'])
        self.assertIn('GainB',peer['inventory']['removed_paths'])
        self.assertEqual(mine['inventory']['modified_parameters'][0]['changes'][0]['new'],'2')
        self.assertEqual(peer['inventory']['modified_parameters'][0]['changes'][0]['new'],'3')
        page=(manifest.parent/'index.html').read_text(encoding='utf-8')
        self.assertNotIn('<script>bad</script>',page)
        self.assertIn('&lt;script&gt;',page)

    def test_git_history_no_manual_old_files(self):
        self.diverge(); self.commit('my edits')
        manifest=review.prepare(self.args(mine='HEAD'))
        data=json.loads(manifest.read_text(encoding='utf-8'))
        self.assertEqual(data['commits']['base'],self.base)
        self.assertTrue((manifest.parent/'_snapshots/base/electrical/simulink/车辆/vehicle.slx').exists())

    def test_lfs_snapshot_and_failed_automerge_preserve_pointer(self):
        command(self.repo,'lfs','install','--local')
        (self.repo/'.gitattributes').write_text('*.slx filter=lfs diff=lfs merge=lfs -text\n',encoding='utf-8')
        command(self.repo,'add','--renormalize','.')
        self.commit('enable lfs')
        pointer=review.git(self.repo,'show','HEAD:electrical/simulink/车辆/vehicle.slx')
        self.assertTrue(pointer.startswith(review.POINTER))
        files=review.snapshot(self.repo,'HEAD',['electrical/simulink'],self.root/'snapshot')
        self.assertEqual(Path(files['electrical/simulink/车辆/vehicle.slx']['path']).read_bytes(),self.path.read_bytes())
        fake=self.root/'MATLAB/bin/win64';fake.mkdir(parents=True)
        (fake/'mlAutoMergeExecutable.exe').touch()
        command(self.repo,'config','modelreview.matlabRoot',str(fake.parents[1]))
        inputs=[]
        for name in ('base','ours','theirs'):
            p=self.root/name;p.write_bytes(pointer);inputs.append(str(p))
        original_run=subprocess.run
        def fake_run(cmd,*a,**kw):
            if str(cmd[0]).endswith('mlAutoMergeExecutable.exe'):
                return subprocess.CompletedProcess(cmd,1)
            return original_run(cmd,*a,**kw)
        with patch('git_tools.subprocess.run',side_effect=fake_run):
            self.assertEqual(git_tools.run(self.repo,'auto',inputs+['vehicle.slx']),1)
        self.assertEqual(Path(inputs[1]).read_bytes(),pointer)
        def success_run(cmd,*a,**kw):
            if str(cmd[0]).endswith('mlAutoMergeExecutable.exe'):
                for item in cmd[1:4]:
                    self.assertTrue(Path(item).read_bytes().startswith(b'PK'))
                Path(cmd[4]).write_bytes(self.path.read_bytes())
                return subprocess.CompletedProcess(cmd,0)
            return original_run(cmd,*a,**kw)
        with patch('git_tools.subprocess.run',side_effect=success_run):
            self.assertEqual(git_tools.run(self.repo,'auto',inputs+['vehicle.slx']),0)
        result=Path(inputs[1]).read_bytes()
        self.assertTrue(result.startswith(review.POINTER))
        self.assertEqual(review.expand_lfs(self.repo,result,'vehicle.slx'),self.path.read_bytes())

    def test_worktree_crlf_does_not_create_fake_script_change(self):
        command(self.repo,'branch','teammate')
        self.text.write_bytes(b'x = 1;\r\n')
        manifest=review.prepare(self.args())
        data=json.loads(manifest.read_text(encoding='utf-8'))
        self.assertEqual(data['changes'],[])

    def test_possible_moves_are_not_counted_as_additions(self):
        before=self.root/'before.slx';after=self.root/'after.slx'
        model(before,[('A','1','1')]);model(after,[('A','1','2')])
        with zipfile.ZipFile(after,'a') as z:
            # A nested system with the same persistent identity.
            z.writestr('simulink/systems/nested.xml','<System><Block Name="A" BlockType="Gain" SID="1"><P Name="Gain">2</P></Block></System>')
        # Rebuild to avoid duplicate ZIP entry names.
        with zipfile.ZipFile(after,'w') as z:
            z.writestr('simulink/blockdiagram.xml','<ModelInformation><Model/></ModelInformation>')
            z.writestr('simulink/systems/system_root.xml','<System><Block Name="Group" BlockType="SubSystem" SID="2"><System Ref="nested"/></Block></System>')
            z.writestr('simulink/systems/nested.xml','<System><Block Name="A" BlockType="Gain" SID="1"><P Name="Gain">2</P></Block></System>')
        result=review.inventory(before,after)
        self.assertEqual(result['possible_moves'],[{'old':'A','new':'Group/A'}])
        self.assertNotIn('Group/A',result['added_paths'])

    def test_path_escape_rejected(self):
        with self.assertRaises(ValueError):review.safe_path(self.root,'../outside')


if __name__ == '__main__':
    unittest.main()
