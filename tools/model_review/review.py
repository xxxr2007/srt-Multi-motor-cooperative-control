"""Review each branch against its merge base; standard-library Python only."""
from __future__ import annotations

import argparse
import difflib
import hashlib
import html
import json
import os
from pathlib import Path
import subprocess
import sys
import xml.etree.ElementTree as ET
import zipfile
from datetime import datetime

SUFFIXES = {'.slx', '.mdl', '.m', '.sldd'}
POINTER = b'version https://git-lfs.github.com/spec/v1'
VISUAL = {'Position', 'ZOrder', 'ForegroundColor', 'BackgroundColor', 'FontName',
          'FontSize', 'FontWeight', 'FontAngle', 'ShowName', 'NamePlacement',
          'SIDHighWatermark', 'Location', 'Open', 'ZoomFactor'}


def git(repo, *args, data=None):
    run = subprocess.run(['git', '-C', str(repo), *args], input=data,
                         stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if run.returncode:
        raise RuntimeError(run.stderr.decode('utf-8', errors='replace'))
    return run.stdout


def resolve(repo, ref):
    return git(repo, 'rev-parse', '--verify', ref + '^{commit}').decode().strip()


def expand_lfs(repo, data, filename):
    if data.startswith(POINTER):
        data = git(repo, 'lfs', 'smudge', '--', filename, data=data)
        if data.startswith(POINTER):
            raise RuntimeError(f'LFS object was not downloaded: {filename}')
    return data


def safe_path(root, relative):
    target = (root / relative).resolve()
    if not target.is_relative_to(root.resolve()):
        raise ValueError(f'Path escapes snapshot: {relative}')
    return target


def snapshot(repo, ref, scopes, dest):
    if ref == 'WORKTREE':
        names = git(repo, 'ls-files', '-z', '--cached', '--others', '--exclude-standard', '--', *scopes)
    else:
        names = git(repo, 'ls-tree', '-rz', '--name-only', ref, '--', *scopes)
    files = {}
    for raw in sorted(set(names.split(b'\0')) - {b''}):
        name = raw.decode('utf-8')
        if Path(name).suffix.lower() not in SUFFIXES:
            continue
        if ref == 'WORKTREE':
            source = safe_path(repo, name)
            if not source.is_file():
                continue
            data = source.read_bytes()
        else:
            data = git(repo, 'show', f'{ref}:{name}')
        data = expand_lfs(repo, data, name)
        target = safe_path(dest, name)
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(data)
        compare_data = data.replace(b'\r\n', b'\n').replace(b'\r', b'\n') if Path(name).suffix.lower() == '.m' else data
        files[name] = {'path': str(target), 'sha256': hashlib.sha256(data).hexdigest(),
                       'comparison_sha256': hashlib.sha256(compare_data).hexdigest()}
    return files


def model_blocks(filename):
    """Supplemental structural inventory, not a semantic replacement for visdiff."""
    if not filename or Path(filename).suffix.lower() != '.slx':
        return {}
    result = {}
    with zipfile.ZipFile(filename) as archive:
        bd = ET.fromstring(archive.read('simulink/blockdiagram.xml'))
        root = (ET.fromstring(archive.read('simulink/systems/system_root.xml'))
                if 'simulink/systems/system_root.xml' in archive.namelist()
                else bd.find('.//Model/System'))

        def walk(system, parent):
            for block in system.findall('Block'):
                # Escape literal slashes the same way Simulink block paths do.
                name = block.get('Name', '').replace('/', '//')
                path = parent + '/' + name if parent else name
                result[path] = {
                    'sid': block.get('SID'), 'type': block.get('BlockType'),
                    'name': block.get('Name'),
                    'parameters': {p.get('Name'): p.text or '' for p in block.findall('P')
                                   if p.get('Name') not in VISUAL},
                }
                child = block.find('System')
                if child is not None:
                    if child.get('Ref'):
                        child = ET.fromstring(archive.read('simulink/systems/' + child.get('Ref') + '.xml'))
                    walk(child, path)
        if root is not None:
            walk(root, '')
    return result


def inventory(oldfile, newfile):
    old, new = model_blocks(oldfile), model_blocks(newfile)
    same = {p for p in old.keys() & new.keys() if old[p]['type'] == new[p]['type']}
    removed, added = set(old) - same, set(new) - same
    # Only suggest a move when name, type and persistent SID all agree uniquely.
    oldids, newids = {}, {}
    for paths, blocks, mapping in ((removed, old, oldids), (added, new, newids)):
        for p in paths:
            b = blocks[p]
            if b['sid']:
                mapping.setdefault((b['sid'], b['type'], b['name']), []).append(p)
    moved = []
    pairs = [(p, p) for p in sorted(same)]
    for identity in oldids.keys() & newids.keys():
        if len(oldids[identity]) == len(newids[identity]) == 1:
            a, b = oldids[identity][0], newids[identity][0]
            removed.remove(a); added.remove(b)
            moved.append({'old': a, 'new': b}); pairs.append((a, b))
    modified = []
    for a, b in pairs:
        pa, pb = old[a]['parameters'], new[b]['parameters']
        changes = [{'parameter': k, 'old': pa.get(k), 'new': pb.get(k)}
                   for k in sorted(pa.keys() | pb.keys()) if pa.get(k) != pb.get(k)]
        if changes:
            modified.append({'old_path': a, 'new_path': b, 'changes': changes})
    return {'added_paths': sorted(added), 'removed_paths': sorted(removed),
            'possible_moves': moved, 'modified_parameters': modified}


def prepare(args):
    repo = Path(args.repo).resolve()
    mine = 'WORKTREE' if args.mine == 'WORKTREE' else resolve(repo, args.mine)
    theirs = resolve(repo, args.theirs)
    mine_commit = resolve(repo, 'HEAD') if mine == 'WORKTREE' else mine
    base = resolve(repo, args.base) if args.base else git(repo, 'merge-base', mine_commit, theirs).decode().strip()
    stamp = datetime.now().strftime('%Y%m%d-%H%M%S-%f')
    out = Path(args.output).resolve() if args.output else repo / 'results' / 'model-review' / stamp
    out.mkdir(parents=True, exist_ok=True)
    manifest_path = out / 'manifest.json'
    if manifest_path.exists():
        raise ValueError('Output directory already contains a review; choose a new directory.')
    scopes = args.scope or ['electrical/simulink']
    files = {side: snapshot(repo, ref, scopes, out / '_snapshots' / side)
             for side, ref in [('base', base), ('mine', mine), ('theirs', theirs)]}
    manifest = {'schema': 1, 'generated': datetime.now().isoformat(timespec='seconds'),
                'repo': str(repo), 'refs': {'base': base, 'mine': args.mine, 'theirs': args.theirs},
                'commits': {'base': base, 'mine': mine_commit, 'theirs': theirs},
                'scopes': scopes, 'changes': [], 'native_results': []}
    for side in ('mine', 'theirs'):
        before, after = files['base'], files[side]
        for name in sorted(before.keys() | after.keys()):
            a, b = before.get(name), after.get(name)
            if a and b and a['comparison_sha256'] == b['comparison_sha256']:
                continue
            status = 'modified' if a and b else 'added' if b else 'deleted'
            entry = {'side': side, 'file': name, 'status': status,
                     'old': a['path'] if a else '', 'new': b['path'] if b else '',
                     'text_diff': '',
                     'native_name': f'{side}_{len(manifest["changes"]):04d}',
                     'inventory': inventory(a['path'] if a else None, b['path'] if b else None)
                     if Path(name).suffix.lower() == '.slx' else {}}
            if Path(name).suffix.lower() == '.m':
                def read(item):
                    if not item:
                        return []
                    raw = Path(item['path']).read_bytes()
                    try:
                        return raw.decode('utf-8-sig').splitlines()
                    except UnicodeDecodeError:
                        return raw.decode('gb18030', errors='replace').splitlines()
                entry['text_diff'] = '\n'.join(difflib.unified_diff(read(a), read(b), fromfile='共同旧版/' + name, tofile=side + '/' + name, lineterm=''))
            manifest['changes'].append(entry)
    manifest_path.write_text(json.dumps(manifest, ensure_ascii=False, indent=2), encoding='utf-8')
    render(manifest_path)
    print(str(manifest_path), flush=True)
    if args.matlab:
        os.environ['MODEL_REVIEW_MANIFEST'] = str(manifest_path)
        code_dir = str(Path(__file__).resolve().parent).replace('\\', '/').replace("'", "''")
        completed = subprocess.run([args.matlab, '-batch', f"addpath('{code_dir}'); publish_model_review(getenv('MODEL_REVIEW_MANIFEST'))"])
        render(manifest_path)
        if completed.returncode:
            raise RuntimeError('MATLAB report generation failed. Structural report is available; inspect native_results.json.')
    return manifest_path


def render(manifest_path):
    manifest_path = Path(manifest_path)
    out = manifest_path.parent
    m = json.loads(manifest_path.read_text(encoding='utf-8'))
    native_file = out / 'native_results.json'
    native = json.loads(native_file.read_text(encoding='utf-8')) if native_file.exists() else []
    if isinstance(native, dict):
        native = [native]
    reports = {x['name']: x for x in native}
    esc = lambda x: html.escape(str(x) if x is not None else '（未显式设置）')
    statusnames = {'added': '新增文件', 'deleted': '删除文件', 'modified': '修改文件'}
    page = ['<!doctype html><meta charset="utf-8"><title>我与队友的车辆模型改动</title>',
            '<style>body{font:16px/1.7 system-ui;max-width:1200px;margin:32px auto;padding:0 24px;color:#18313b}table{border-collapse:collapse;width:100%;margin:16px 0}td,th{border:1px solid #ccd6db;padding:8px;text-align:left;vertical-align:top}pre{white-space:pre-wrap;overflow-wrap:anywhere;background:#f3f6f7;padding:14px}summary{cursor:pointer;font-weight:600}h2{border-bottom:3px solid #287484;padding-top:20px}.note{background:#fff4d8;padding:12px}.add{color:#087247}.del{color:#a53434}a{color:#075aab}</style>',
            '<h1>我与队友分别改了什么</h1>',
            '<p>两边各自与共同旧版比较。未执行合并或仿真；“我/队友”表示选择的分支，分支中可能包含多名作者。</p>',
            '<table><tr><th>共同旧版</th><th>我的版本</th><th>队友版本</th></tr><tr>',
            *[f'<td>{esc(m["refs"][s])}<br>{esc(m["commits"][s])}</td>' for s in ('base','mine','theirs')], '</tr></table>',
            '<p class="note">结构清单用于快速定位。路径新增/消失可能来自移动或重命名；同 SID、名称、类型的移动仅为候选。参数清单省略布局属性，不包含全部连线、Stateflow 代码和配置；完整内容请打开 MATLAB 原生报告。模型文件外的 .m 修改单独列出。自动报告不推断修改目的或性能改善。</p>']
    md = ['# 车辆模型改动报告', '', '比较方法：共同旧版分别对照我、队友。只读生成，不执行合并或仿真。', '',
          *[f'- {s}: `{m["refs"][s]}` / `{m["commits"][s]}`' for s in ('base','mine','theirs')], '']
    changed = {s: {e['file'] for e in m['changes'] if e['side'] == s} for s in ('mine','theirs')}
    common = sorted(changed['mine'] & changed['theirs'])
    page.append('<h2>双方都动过的文件</h2><p>这只是共同修改文件提示，不等于存在合并冲突。</p><ul>' + ''.join(f'<li>{esc(p)}</li>' for p in common) + '</ul>')
    for side, label in [('mine','我的改动'), ('theirs','队友的改动')]:
        entries = [e for e in m['changes'] if e['side'] == side]
        page.append(f'<h2>{label}</h2>'); md += [f'## {label}', '']
        if not entries:
            page.append('<p>在本次范围内，与共同旧版没有变化。</p>'); md += ['与共同旧版没有变化。', '']
        for e in entries:
            page.append(f'<h3>{statusnames[e["status"]]}：{esc(e["file"])}</h3>')
            md += [f'- {statusnames[e["status"]]}：`{e["file"]}`']
            nr = reports.get(e['native_name'])
            if nr and nr.get('success'):
                link = Path(nr['report']).name
                page.append(f'<p><a href="{esc(link)}">打开 MATLAB 完整比较报告（旧值 / 新值、模块、代码及连线差异）</a></p>')
                if nr.get('warning'):
                    page.append(f'<p class="note">导出警告：{esc(nr["warning"])}</p>')
            elif nr:
                page.append(f'<pre>MATLAB 导出失败：{esc(nr.get("error"))}</pre>')
            elif e['old'] and e['new'] and Path(e['file']).suffix.lower() in {'.slx','.mdl'}:
                page.append('<p>原生报告尚未生成。请运行 MATLAB 导出步骤。</p>')
            elif Path(e['file']).suffix.lower() == '.sldd':
                page.append('<p>R2024b 此入口仅记录数据字典文件变化；字典内部变化请在 MATLAB 比较窗口中查看。</p>')
            inv = e['inventory']
            if inv:
                for key, title in [('added_paths','新增结构路径'),('removed_paths','消失结构路径')]:
                    paths = inv[key]
                    page.append(f'<details><summary>{title}：{len(paths)}</summary><ul>' + ''.join(f'<li>{esc(p)}</li>' for p in paths) + '</ul></details>')
                moves = inv['possible_moves']
                page.append(f'<details><summary>可能移动的模块：{len(moves)}</summary><ul>' + ''.join(f'<li>{esc(p["old"])} → {esc(p["new"])}</li>' for p in moves) + '</ul></details>')
                page.append('<details open><summary>模块属性旧值 → 新值</summary><table><tr><th>模块路径</th><th>属性</th><th>共同旧版</th><th>本分支</th></tr>')
                for b in inv['modified_parameters']:
                    for p in b['changes']:
                        page.append('<tr>' + ''.join(f'<td><pre>{esc(v)}</pre></td>' for v in (b['new_path'],p['parameter'],p['old'],p['new'])) + '</tr>')
                page.append('</table></details>')
            if e.get('text_diff'):
                page.append(f'<details open><summary>M 文件逐行修改</summary><pre>{esc(e["text_diff"])}</pre></details>')
        md += ['', '修改人补充：新增功能、修改原因、接口/单位/采样时间影响、验证工况与结果。未验证请明确写“未验证”。', '']
    (out / 'index.html').write_text('\n'.join(page), encoding='utf-8')
    (out / 'summary.md').write_text('\n'.join(md), encoding='utf-8')


def parser():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--repo', default=str(Path(__file__).resolve().parents[2]))
    p.add_argument('--mine', default='WORKTREE', help='My ref or WORKTREE (includes saved uncommitted edits)')
    p.add_argument('--theirs', default='origin/main', help='Teammate branch/ref')
    p.add_argument('--base', help='Optional explicit baseline; default is merge-base')
    p.add_argument('--scope', action='append', help='Repository-relative folder; repeatable')
    p.add_argument('--output')
    p.add_argument('--matlab', help='MATLAB executable; generates full native reports too')
    p.add_argument('--render', help='Refresh HTML after MATLAB generation using this manifest')
    return p


if __name__ == '__main__':
    args = parser().parse_args()
    try:
        render(args.render) if args.render else prepare(args)
    except Exception as error:
        print(f'ERROR: {error}', file=sys.stderr)
        sys.exit(1)
