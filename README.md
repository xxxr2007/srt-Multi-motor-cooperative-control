# SRT 多电机协同控制与车辆仿真

仓库包含整车动力学、车辆控制与多电机协同研究。日常开发从下面的入口开始；原始示例和 CAD 资料集中归档。

| 要做的事 | 入口 |
|---|---|
| 打开正式车辆模型 | [启动车辆仿真.m](启动车辆仿真.m)，或 [整车模型目录](electrical/simulink/整车仿真模型/) |
| 看我和队友分别改了什么 | 首次双击 [配置车辆协作工具.cmd](配置车辆协作工具.cmd)，之后双击 [查看我和队友的改动.cmd](查看我和队友的改动.cmd) |
| 查看/编写比较报告 | [车辆协作与报告操作](docs/车辆模型协作与报告.md) · [实际填写示例](docs/实际模型报告填写示例.md) |
| 找当前任务、周报与会议模板 | [文档导航](docs/README.md) |
| 找机械模型、CAD 与交付参数 | [机械组导航](mechanical/README.md) |
| 找电控代码与正式仿真 | [电控组导航](electrical/README.md) |
| 找原始模型和学习教程 | [参考资料导航](references/README.md) |
| 查看研究方案与已有运行命令 | [完整研究说明](docs/项目研究说明.md) |

## 第一次获取仓库

安装 Git LFS、Python 3.10+ 和 MATLAB / Simulink（当前模型与报告工具按 R2024b 验证）。

```bash
git lfs install
git clone --recurse-submodules https://github.com/xxxr2007/srt-Multi-motor-cooperative-control.git
cd srt-Multi-motor-cooperative-control
git lfs pull
```

已有克隆要补教程内容，可执行 `git submodule update --init --recursive`。教程是独立子模块，主仓库只记录其提交指针。

在 MATLAB 中运行根目录的 `启动车辆仿真.m`，打开正式模型。该入口只设置项目路径并打开模型，不自动运行仿真。

## 目录约定

```text
electrical/          电控代码、MATLAB 脚本、正式车辆模型
mechanical/          机械建模、交付参数、CAD 参考库
data/                机械与电控共享数据、验收口径
docs/                任务、组规、周报、会议、研究和入门资料
references/          原始车辆示例与外部学习教程
tools/               参数同步、检查、模型比较和报告工具
results/             本地生成结果；自动比较报告不入版本库
```

新增开发工作放在对应正式目录，学习原件放 references 或 mechanical/reference。参数交接仍按 [现有协作约定](docs/collaboration.md) 执行。

## 提交与审查

每项任务建立自己的分支，提交 Pull Request。车辆模型相关 PR 会运行 `Vehicle model review`，分别比较双方与共同旧版的差异；报告位于 Actions 的 Artifacts，下载解压后打开 `index.html`。

PR 模板记录“我新增的内容”“队友已有改动”“本次集成修改”和验证结果。自动报告不能替代车辆仿真验证。分支与评审约定见 [组规](docs/rules.md)。
