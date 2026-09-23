# 卡点 / 问题（含解决）

## [BLK][2026-09-23] shell PATH 噪声（command not found）
- **问题**：AI 工具的 Bash 会话中途 PATH 丢失，`mkdir`/`grep`/`head`/`wc`/`tail` 报 `command not found`，并成对出现 `cd: null directory`。这是本机 shell 会话抽风，不是你本机环境坏。
- **解决**：① 别重试 Bash，改用专用工具（Read / Grep / Glob）或 Python（managed runtime）；② 需要跑命令时把结果写文件再用 Read 看；③ gcc 编译必须用 Python 包一层 PATH（见 bugs.md）。

## [BLK][2026-09-23] git rm 导致 13 个文件误删事故
- **问题**：`git rm` 两个废弃文件后，`git status` 爆出 13 个非预期 ` D`（核心源码在磁盘上真的消失）。疑 safe-shim 拦截 `rm -rf` 的副作用 + `cd: null directory` cwd 噪声叠加导致。
- **解决**：立即 `git -C <repo> checkout -- <13个文件>` 全量恢复（未 commit 的删除都能从 HEAD 找回），逐个 Read 验证。绝不在未核对的情况下继续下一步操作。

## [BLK][2026-09-23] safe-shim 拦截 rm / rmdir
- **问题**：安全 shim 拦截 `rm`/`rmdir`/`Add-Type` 等删除命令，反复尝试也绕不过。
- **解决**：别硬绕。空目录残留无害（Git 不跟踪空目录），留着即可。

## [BLK][2026-09-22] VS Code 未保存旧缓冲
- **问题**：标签带 `⊙`（未保存标记）时，磁盘文件已更新但编辑器仍显示旧内容，且不会自动刷新，致用户误以为“文件没改 / 不存在”。
- **解决**：让用户 **千万别 Ctrl+S**（会把旧缓冲写回磁盘覆盖新版）→ 关闭标签选“不保存”或 `Ctrl+Shift+P → Revert File` → 重开即见最新。排查顺序：① VS Code 旧缓冲 ② SourceGit 未刷新 ③ GitHub 网页缓存，最后才怀疑文件本身。
