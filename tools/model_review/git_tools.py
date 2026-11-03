"""Git adapters: expand LFS pointers before invoking MATLAB's native tools."""
from pathlib import Path
import os
import shlex
import subprocess
import sys
import tempfile
from review import expand_lfs, git


def run(repo, action, paths):
    root = git(repo, 'config', '--get', 'modelreview.matlabRoot').decode().strip()
    binary = {'diff': 'mlDiff.exe', 'merge': 'mlMerge.exe', 'auto': 'mlAutoMergeExecutable.exe'}[action]
    exe = Path(root) / 'bin' / 'win64' / binary
    if not exe.is_file():
        raise FileNotFoundError(f'Run setup_local.py first. Missing {exe}')
    relative = paths[3] if action == 'auto' else 'model.slx'
    originals = paths[:3] if action == 'auto' else paths[:3] if action == 'merge' else paths[:2]
    with tempfile.TemporaryDirectory(prefix='matlab-lfs-') as temp:
        folder = Path(temp)
        copies = []
        for i, original in enumerate(originals):
            dest = folder / str(i) / 'model.slx'
            dest.parent.mkdir()
            data = Path(original).read_bytes() if Path(original).exists() else b''
            dest.write_bytes(expand_lfs(repo, data, relative))
            copies.append(dest)
        if action == 'auto':
            # Native ordering is base, mine, theirs, output. Keep Git's input
            # untouched unless MATLAB reports successful semantic merging.
            merged = folder / 'merged.slx'
            merged.write_bytes(copies[1].read_bytes())
            completed = subprocess.run([str(exe), *map(str, copies), str(merged)])
            if completed.returncode != 0 or not merged.is_file():
                return 1
            pointer = git(repo, 'lfs', 'clean', '--', relative, data=merged.read_bytes())
            Path(paths[1]).write_bytes(pointer)
            return 0
        if action == 'merge':
            merged = folder / 'merged.slx'
            merged.write_bytes(copies[1].read_bytes())
            completed = subprocess.run([str(exe), *map(str, copies), str(merged)])
            if completed.returncode or not merged.is_file():
                return 1
            Path(paths[3]).write_bytes(merged.read_bytes())
            return 0
        return subprocess.run([str(exe), *map(str, copies)]).returncode


if __name__ == '__main__':
    try:
        repo = Path(git(Path.cwd(), 'rev-parse', '--show-toplevel').decode().strip())
        if sys.argv[1] not in {'diff', 'merge', 'auto'}:
            raise ValueError('Expected diff, merge or auto')
        expected = {'diff': 2, 'merge': 4, 'auto': 4}[sys.argv[1]]
        if len(sys.argv[2:]) != expected:
            raise ValueError(f'Expected {expected} file arguments')
        sys.exit(run(repo, sys.argv[1], sys.argv[2:]))
    except Exception as error:
        print(f'MATLAB Git tool: {error}', file=sys.stderr)
        sys.exit(1)
