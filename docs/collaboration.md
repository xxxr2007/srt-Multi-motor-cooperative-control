# 两组协同机制（机械组 × 电控组）

> 本文是 SRT 小组的**协作宪法**：规定谁写哪个文件、参数怎么流动、
> 效果压到什么程度算达标、以及迭代什么时候停。

---

## 一、数据边界：各写各的，谁也别碰对方的文件

| 文件 | 归属 | 内容 | 谁能改 |
|---|---|---|---|
| `data/mechanical/mech_params.json` | 由 sync 脚本自动生成 | 转动惯量 J、黏性摩擦 B、联轴器刚度 Ks、阻尼 Ds、负载物理来源（**真正手填入口是 `data/shared/mech_deliverables.csv`**） | 机械组禁止手改（改 csv 后由 `tools/sync_mech_to_elec.py` 重写） |
| `data/electrical/ctrl_params.json` | **电控组** | 控制增益、DOB 带宽 g、仿真设置、工况时序与幅值 | 只有电控组 |
| `data/params.json` | 脚本生成 | 两组参数合并结果 | **谁都不许手改** |
| `build/params_generated.h` | 脚本生成 | 给 C 内核的宏定义 | **谁都不许手改** |
| `data/acceptance.json` | 组长冻结 | 验收规则（达标线） | 开题冻结后不许改 |
| `mechanical/docs/plant_model.md` | 机械组 | 参数交接单（含物理含义、单位、来源） | 机械组 |

**为什么要分这么细**：避免"两份参数各改一半，最后对不上账"。合并是唯一入口。

---

## 二、参数流向

```
机械组                                   电控组
data/mechanical/mech_params.json         data/electrical/ctrl_params.json
   (J · B · Ks · Ds)                        (Kp · Ki · g · 工况 · dt)
        │                                          │
        └──────────────┬───────────────────────────┘
                       ▼
          python electrical/c/merge_params.py        ← 合并 + 校验
                       │
        ┌──────────────┴──────────────┐
        ▼                             ▼
 data/params.json            build/params_generated.h
 (Python / Simulink 用)      (C 内核编译时自动 include)
                       │
                       ▼
              运行仿真 → results/metrics_c.csv
                       │
                       ▼
          python electrical/c/check_acceptance.py    ← 达标判定
                       │
        达标？──是──► 冻结参数与算法，写报告
              └──否──► 先调电控（内环）；压不住才开机械变更单（外环）
```

---

## 三、每次迭代的命令（都在仓库根目录执行）

```bash
python electrical/c/merge_params.py        # ① 合并参数（改过参数后必跑）
gcc -O2 -o build/multi_motor_sync.exe electrical/c/multi_motor_sync.c -lm   # ② 编译
build\multi_motor_sync.exe            # ③ 跑仿真
python electrical/c/check_acceptance.py --record   # ④ 判定达标情况并记入迭代历史
python electrical/c/plot_results.py        # ⑤ 出图（需要时）
```

> C 内核会自动读取 `build/params_generated.h`；若该文件不存在（没跑第 ① 步），
> 会退回源码里内置的默认参数，不会编译失败。

---

## 四、验收规则：效果压到什么地步算过

规则写在 `data/acceptance.json`，判定脚本是 `electrical/c/check_acceptance.py`。
以当前参数下的实测数据为基准（主从 RMS 0.190 → CCC 0.099 → DCC 0.098 → **DCC+DOB 0.041**）：

| 规则类别 | 要求 | 为什么要有 |
|---|---|---|
| ① 绝对阈值 | RMS ≤ 0.05；峰值 ≤ 0.60；恢复时间 ≤ 0.02 s；跌落 ≤ 0.60 | 给"好"一个数字定义 |
| ② 相对提升 | 目标策略 RMS 比最强经典策略（DCC）再降 ≥ 50% | 证明创新点真的有效，不是换个名字 |
| ③ 不恶化条款 | 任一项指标不得比上一轮差 5% 以上 | 防止"按下葫芦浮起瓢" |
| ④ 迭代停止 | 连续两轮改进 < 5% 即判收敛 | 防止无限打转 |

**重点**：这套规则**开题时冻结，中途不许改**。改了阈值再说"达标"，等于没达标。

> 通俗解释版（设计目标、单位更直观）见 `docs/acceptance_metrics.md` §二；本文档 §四 才是冻结的唯一权威阈值，由 `check_acceptance.py` 自动判定。

未启用的规则：稳定裕度（相位裕度 ≥ 45°、幅值裕度 ≥ 6 dB）——需要先实现频率响应计算，
`acceptance.json` 里 `robustness.enabled` 置 `true` 才会参与判定。

---

## 五、迭代闭环：内环快、外环慢

| | 谁动 | 改动成本 | 一轮多久 |
|---|---|---|---|
| **内环** | 电控组：调 g、调增益、加陷波 | 改代码重跑，分钟级 | 可以一天好几轮 |
| **外环** | 机械组：换联轴器、改刚度、调惯量配比 | 重新建模/选型/查手册，天~周级 | 一轮别超过一次 |

**节奏纪律**：

1. 电控组先在内环榨干（至少调 3~5 轮），**确实压不住**才开机械变更单；
2. 机械组每完成一轮改动，版本号 +1，并在群里通知；
3. 电控组拿到新参数**必须重跑并重新判定**，否则报告里的对照不成立；
4. 组长每轮主持一次短会（10 分钟），只对三件事：本轮指标、是否达标、下一步谁做什么。

**判据分叉**：
- 达标 → 冻结，写报告；
- 差一点且瓶颈在控制（如带宽够但增益没调好）→ 回内环；
- 瓶颈在机械（典型：`ω_n` 高于观测器带宽 `g` 的 2 倍，DOB 压不住谐振）→ 开机械变更单。

---

## 六、机械变更单模板（每次改机械就复制一份）

```
变更单编号：MECH-CR-001
日期：2026-__-__
机械组版本：v_ → v_          （改完必须递增）
改了什么：如"联轴器由刚性改为膜片式"
为什么改：如"ω_n = 141 rad/s 高于观测器带宽 100 rad/s，DOB 压不住谐振"
改前 / 改后：Ks = ____ → ____ N·m/rad；ω_n = ____ → ____ rad/s
预期影响：如"ω_n 降到 70 rad/s，落在 g=100 的 2 倍覆盖范围内"
电控组验证结果：待填（下一轮 check_acceptance.py 的输出）
```

---

## 七、常见问题

**Q：机械组的参数还没算准，仿真能先跑吗？**
能。`mech_deliverables.csv` 里现在是占位值（与现有仿真同口径），跑 `tools/sync_mech_to_elec.py` 后写回 `mech_params.json`，先跑起来，
机械组后续替换并递增版本号即可。不要等"全部算准"才开工。

**Q：改了参数但仿真结果没变？**
多半是漏了第 ① 步（没跑 `tools/sync_mech_to_elec.py` 或没跑 `merge_params.py`），或者 C 内核没重新编译。

**Q：`data/params.json` 我能直接改吗？**
不能。它是合并产物，下次合并会被覆盖。要改就往两个源文件里改。

**Q：判定 FAIL 了但我觉得曲线看起来还行？**
看数字，不看感觉。FAIL 说明有某项没进线，先看是哪一项，再决定内环还是外环。
