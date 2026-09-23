# Bug 记录

## [BUG][2026-09-23] MinGW gcc 静默编译失败（实为环境假象）
- **现象**：`gcc -O2 -o build/multi_motor_sync.exe electrical/c/multi_motor_sync.c -lm` 返回 `exit=1`，stdout/stderr 全空，不生成 exe。连 `gcc --version` 在 AI 工具的 Bash 里也吞输出。
- **根因**：AI 工具本次 Bash 会话的 PATH 被噪声破坏，缺 `mingw64/bin`。gcc 调后端 `as.exe`/`ld.exe`（在 `mingw64/x86_64-w64-mingw32/bin/`）依赖的运行时 DLL（`libwinpthread` 等）全在 `mingw64/bin/` 下，PATH 一缺它 → as/ld 静默崩溃 → gcc 返回 1 且无报错。
- **验证**：连最基础的 hello-world 用同一 gcc 也 `exit=1`、stderr 空 → 证明非代码问题。
- **复现 / 解决**：用 Python（managed runtime，不依赖 shell PATH）调 gcc，并在 `env["PATH"]` 前置 `mingw64/bin`：
  ```python
  import os, subprocess
  base = r'C:/Users/31394/.workbuddy/binaries/mingw64'
  env = os.environ.copy(); env['PATH'] = base + '/bin;' + env.get('PATH', '')
  subprocess.run([base + '/bin/gcc.exe', '-O2', '-o', 'build/multi_motor_sync.exe',
                  'electrical/c/multi_motor_sync.c', '-lm'],
                 cwd=r'C:/Users/31394/Desktop/srtfangzhen', env=env)
  ```
  实测：编译 EXIT 0，生成约 64 KB 的 exe，运行 EXIT 0，四策略仿真 CSV 全生成。
- **组员日常**：自己的 SourceGit 终端 / 系统 cmd 的 PATH 是完整的，`gcc` 直接能用。想一劳永逸就把 `C:\Users\31394\.workbuddy\binaries\mingw64\bin` 加进系统 PATH。可考虑加一个 `tools/build_c.py` 一键编译脚本（用 Python 包一层 PATH）。

## [BUG][2026-09-23] Edit 工具“返回成功但不落盘”的静默失效
- **现象**：对同一文件在同一条消息里发多个 Edit，部分 Edit 工具返回成功，但磁盘文件仍是旧文本（甚至导致以为已 commit 的改动实际未落盘）。
- **根因**：同一条消息对同一个文件堆多个 Edit 的竞态 / 静默丢失。
- **复现**：一条消息内对 `three_month_plan.md` 连发 4 个 Edit，其中 2 个未生效。
- **解决**：① 同文件改动 **逐条单独** 发 Edit，不要一条消息堆多个；② 每次 Edit 后用 Grep 复验新内容是否真在文件里；③ 提交前用 `git status` + `git diff` 双验证。
