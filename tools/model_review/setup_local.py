"""Configure this clone only; preserve Git LFS and other repositories."""
import argparse
import json
from pathlib import Path
import shlex
import shutil
import sys
from review import git


def configure(repo, matlab_root):
    repo, matlab_root = Path(repo).resolve(), Path(matlab_root).resolve()
    for binary in ('matlab.exe',):
        if not (matlab_root / 'bin' / binary).is_file():
            raise FileNotFoundError(matlab_root / 'bin' / binary)
    for binary in ('mlDiff.exe','mlMerge.exe','mlAutoMergeExecutable.exe'):
        if not (matlab_root / 'bin' / 'win64' / binary).is_file():
            raise FileNotFoundError(matlab_root / 'bin' / 'win64' / binary)
    tool = repo / 'tools' / 'model_review' / 'git_tools.py'
    cmd = shlex.quote(Path(sys.executable).as_posix()) + ' ' + shlex.quote(tool.as_posix())
    settings = {
        'modelreview.matlabRoot': matlab_root.as_posix(),
        'diff.tool': 'mlDiff',
        'merge.tool': 'mlMerge',
        'difftool.mlDiff.cmd': cmd + ' diff "$LOCAL" "$REMOTE"',
        'difftool.prompt': 'false',
        'mergetool.mlMerge.cmd': cmd + ' merge "$BASE" "$LOCAL" "$REMOTE" "$MERGED"',
        'mergetool.mlMerge.trustExitCode': 'true',
        'merge.matlab-lfs.name': 'MATLAB semantic merge with Git LFS',
        'merge.matlab-lfs.driver': cmd + ' auto %O %A %B %P',
        'merge.matlab-lfs.recursive': 'binary',
    }
    previous = {}
    for key, value in settings.items():
        try:
            previous[key] = git(repo, 'config', '--local', '--get', key).decode().strip()
        except RuntimeError:
            previous[key] = None
        git(repo, 'config', '--local', key, value)
    backup = repo / '.git' / 'model-review-config-before.json'
    if not backup.exists():
        backup.write_text(json.dumps(previous, ensure_ascii=False, indent=2), encoding='utf-8')
    print('Configured local clone:', repo)
    print('MATLAB:', matlab_root)
    print('Original local settings saved:', backup)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--repo', default=str(Path(__file__).resolve().parents[2]))
    p.add_argument('--matlab-root')
    args = p.parse_args()
    root = args.matlab_root
    if not root:
        executable = shutil.which('matlab')
        candidates = sorted(Path('C:/Program Files/MATLAB').glob('R20*'))
        root = str(Path(executable).parent.parent) if executable else str(candidates[-1]) if candidates else None
    if not root:
        p.error('Pass --matlab-root pointing to your MATLAB installation')
    configure(args.repo, root)
