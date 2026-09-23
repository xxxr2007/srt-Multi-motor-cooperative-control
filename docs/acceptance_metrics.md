# 验收指标表

> 本表是 `docs/collaboration.md` §四（**机器判定规则**：写入 `data/acceptance.json`、由 `check_acceptance.py` 自动判定）的**通俗解释版**。四策略对比、迭代每轮都用同一口径，结果才可比。
> 口径对齐：仿真结果命名见 `docs/rules.md` §4；数据留档见同节。**`collaboration.md` §四 才是冻结的唯一权威阈值**，本表数字更宽松，仅作申报书/结题的设计目标参考。

## 一、统一 baseline 场景（对比前提）
- 被控对象：四电机 + 弹性联轴器（双惯量），`Ks` 取当前方案值
- 扰动：转速阶跃（0 → 额定）
- 采样：与 C 内核一致（步长见 `docs/collaboration.md`）
- 指标计算：同步误差 = 各电机转速与平均转速的偏差

## 二、验收指标与达标线（设计目标通俗版）

> 下列为**申报书/结题用的通俗设计目标**，单位更直观（占额定转速百分比、通用单位），数值偏宽松；**真正冻结、自动判定的严格阈值在 `docs/collaboration.md` §四**（RMS ≤ 0.05 rad/s、峰值 ≤ 0.60、恢复 ≤ 0.02 s、跌落 ≤ 0.60，外加相对提升 ≥50% / 不恶化 <5% / 收敛 <5%），写在 `data/acceptance.json`，由 `check_acceptance.py` 判定。

| 指标 | 含义 | 达标线（建议） | 对应脚本字段 |
|---|---|---|---|
| 同步误差 RMS | 稳态同步精度 | < 2% 额定转速（机器严格值 RMS ≤ 0.05 rad/s，见 collaboration §四） | `sync_err_rms` |
| 超调量 | 阶跃超调 | < 5% | `overshoot` |
| 调节时间 ts | 进入稳态带时间 | < 0.5 s | `settle_time` |
| 谐振峰衰减 | 柔带来的谐振是否被压住 | 谐振峰 < 0 dB，或被陷波器压 10 dB+ | `resonance_peak` |
| 稳态误差 | 末态偏差 | ≈ 0 | `steady_err` |

> 达标线为建议值，最终以 `check_acceptance.py` 阈值（collaboration §四）+ 组长判定为准；**迭代期逐轮看「比上轮好」比绝对线更重要**。

## 三、四策略对比口径
- 主从 / CCC / DCC / DCC+DOB 用同一 baseline 跑，各出一张曲线
- 对比图命名 `fig_W{周}_{策略}_同步误差.png`（见 `rules.md` §4）
- 迭代期每轮记录指标到 `data/results/`，逐轮收敛曲线即创新点证据

## 四、交付物证据与数据留存（写论文 / 专利用）

- 以下交付物既是进度证明，也是验收证据，必须留档：
  - 机械组：`ω_n` 计算书（W6）、联轴器选型定案表 + 工程理由（W10）、参数交接单 v1/v2
  - 电控组：四策略对比基准图 `results/baseline/`（W6）、参数冻结清单 `params_freeze_v1.0.json`（W11）、每次仿真 csv+json
- 重要数据存 `data/results/*.csv+json`（参数 / 工况 / 脚本版本），不只留截图；口径见 `rules.md` §4
- 最终验收证据：`check_acceptance.py` 全项 PASS 截图 + 指标 csv

