# 3. 关联 GitHub 私人仓库

Git 保存版本历史，GitHub 保存远程副本。**添加远程地址只是建立关联；成功推送后，文件才到达云端。**

私人仓库可邀请指定队友访问：[GitHub 仓库权限说明](https://docs.github.com/en/repositories/creating-and-managing-repositories/about-repositories)。

## 先准备 Git 和登录

Windows 安装 [Git for Windows](https://git-scm.com/download/win)，允许其他应用使用 Git，并启用 Git Credential Manager。仅安装 GitHub Desktop 不足以保证 Obsidian 插件可以找到 Git。验证：

```powershell
git --version
git config credential.helper
```

按插件安装指南，建议 Git 2.29 或更新版本；凭据助手常见输出是 `manager`。其他助手也可能有效，不要仅凭名字不同就覆盖已有配置。[插件安装说明](https://publish.obsidian.md/git-doc/Installation)

第一次使用 push/pull/clone 可能出现登录窗口，请在系统登录流程中完成；不要把密码或访问令牌写进本包、笔记、聊天或仓库 URL。

## 情况 A：新 GitHub 仓库完全为空

创建仓库时选择 Private，不预置 README、许可证或 .gitignore。打开笔记库根目录的 PowerShell，逐条执行，确认每步成功后再继续：

```powershell
cd 'D:\Obsidian\AI项目知识库'

# 确认当前没有属于其他项目的 Git 仓库
git rev-parse --show-toplevel

# 仅在尚未初始化时执行
git init -b main
```

创建 `.gitignore`，内容为 `.obsidian/`。如果已有文件，只补充该行，不覆盖已有规则。

```powershell
# 用你希望公开给协作者的提交身份；仅配置本仓库
git config user.name '你的提交姓名'
git config user.email '你的提交邮箱'

# 先检查笔记内容和将被纳入的文件
git status --short
git add -- '.gitignore' 'AI 项目笔记'
git diff --cached --stat
git commit -m '添加 AI 项目笔记'

# 替换成自己的远程地址
git remote add origin 'https://github.com/你的账号/你的仓库.git'
git remote -v
git push -u origin main
```

若没有“AI 项目笔记”目录，先用技能生成一份笔记，或先只提交 .gitignore。已有 origin 时先用 `git remote get-url origin` 检查，不重复添加或随意改写。

`-u` 设置上游分支，方便以后直接执行 push/pull。首次成功后确认：

```powershell
git branch -vv
git ls-remote origin refs/heads/main
```

正常时 main 显示跟踪 origin/main，GitHub 页面能看到笔记。[GitHub 首次上传指南](https://docs.github.com/en/migrations/importing-source-code/using-the-command-line-to-import-source-code/adding-locally-hosted-code-to-github)

## 情况 B：远程已经有 README 或其他提交

先克隆，保留远程历史，而不是用全新本地历史强推覆盖：

```powershell
cd 'D:\Obsidian'
git clone 'https://github.com/你的账号/你的仓库.git' 'AI项目知识库-云端版'
```

把原库的笔记和附件复制到这个新目录，检查没有覆盖需要保留的远程文件，使用 Obsidian 打开新目录，更新技能的 vault_path，再提交和推送。原库中的 `.git` 不要复制过去。

如果本地与远程都已有不同的提交历史，让助手先查看两端历史再整合；不要把 `push --force` 或 `reset --hard` 当作常规解决方案。

## 日常手动命令（自动化启用后的备用方式）

`git status` 查看本地状态；`git log -1` 查看最近提交；`git push` 上传已有提交；`git pull` 获取并整合远程变化。

存在冲突时先处理冲突，再继续同步。首次人工推送证明网络与身份可用，**不证明自动同步定时器已正常运行**，下一步还要配置插件并单独测试。
