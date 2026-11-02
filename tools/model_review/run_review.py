"""Interactive one-click launcher for local branch review."""
from pathlib import Path
import argparse
import os
import subprocess
import sys
import webbrowser
from review import git


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--theirs')
    p.add_argument('--mine', default='WORKTREE')
    args = p.parse_args()
    repo = Path(__file__).resolve().parents[2]
    print('正在同步远端分支信息（不会合并或覆盖工作区）……', flush=True)
    git(repo, 'fetch', '--prune', 'origin')
    theirs = args.theirs
    if not theirs:
        branches = git(repo, 'branch', '-r', '--format=%(refname:short)').decode().splitlines()
        branches = [b for b in branches if not b.endswith('/HEAD')]
        print('可选队友分支：\n' + '\n'.join(f'{n+1}. {b}' for n,b in enumerate(branches)))
        choice = input('输入编号或分支名（回车比较 origin/main）：').strip()
        theirs = branches[int(choice)-1] if choice.isdecimal() and 1 <= int(choice) <= len(branches) else choice or 'origin/main'
    root = git(repo, 'config', '--get', 'modelreview.matlabRoot').decode().strip()
    print('正在分别生成我和队友的报告，MATLAB 首次启动需要一些时间……', flush=True)
    command = [sys.executable, str(Path(__file__).with_name('review.py')),
               '--mine', args.mine, '--theirs', theirs, '--matlab', str(Path(root) / 'bin' / 'matlab.exe')]
    env = os.environ.copy()
    env['PYTHONIOENCODING'] = 'utf-8'
    env['PYTHONUTF8'] = '1'
    completed = subprocess.run(command, stdout=subprocess.PIPE, stderr=None, text=True, encoding='utf-8', errors='replace', env=env)
    print(completed.stdout)
    # prepare prints the manifest path before MATLAB's output starts.
    manifest = next((Path(line) for line in completed.stdout.splitlines() if line.strip().endswith('manifest.json') and Path(line.strip()).is_file()), None)
    if manifest:
        report = manifest.parent / 'index.html'
        subprocess.run([sys.executable, str(Path(__file__).with_name('review.py')), '--render', str(manifest)], check=True)
        webbrowser.open(report.as_uri())
        print('报告：', report)
    return completed.returncode


if __name__ == '__main__':
    raise SystemExit(main())
