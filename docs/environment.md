# 环境锁定（W0 由组长统一确认，所有人一致）

> 目的：4 人协作第一翻车点是「今天能跑明天报错」。W0 就把环境锁死，写在此处。

## Python 环境
- Python：3.11 或 3.12（组长 W0 确认统一版本，填在此处）
- 包：见根目录 `requirements.txt`（numpy / pandas / matplotlib / jupyter）
- 建议用 Anaconda 或 venv，别每人一套乱版本

## C 仿真内核
- 编译器：gcc（MinGW-w64）≥ 13，或仓库指定版本
- 标准：C11（`-std=c11`）
- 不依赖第三方库（纯 C）

## MATLAB / Simulink
- 版本：MATLAB 2024b（或南农校园正版授权版本，W0 确认填此处）
- 用途：四电机建模、四策略仿真、出图

## CATIA
- 版本：V5-6 R20xx（校园授权，W0 确认填此处）
- 用途：机械建模、测惯量 J/B

## 正版授权（外援，组长负责确认）
- MATLAB / CATIA 南农是否有校园授权？W0 前找导师/机房确认，写在此处
- 无授权 → 改用开源替代（Python 出图替代部分 MATLAB；FreeCAD 替代部分 CATIA），并在周报标注
