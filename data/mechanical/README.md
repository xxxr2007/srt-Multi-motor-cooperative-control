# 机械组交付区（只写这个文件夹）

> **约定**：机械组的所有参数只写在 `data/mechanical/` 里，**不要动其他任何文件**。
> 电控组从本文件夹直接读取参数跑仿真，不需要你们碰代码。

---

## 1. 文件说明

| 文件 | 谁维护 | 说明 |
|---|---|---|
| `mech_params.json` | 由 sync 脚本自动生成 | 参数真源：由 `data/shared/mech_deliverables.csv` 经 `tools/sync_mech_to_elec.py` 写回，进 git 可追溯；**禁止手改**（手填请去 csv） |
| `README.md` | 电控组 | 本说明，不用改 |

### 1.1 csv 路线（当前唯一推荐入口，不用碰 JSON 语法）

```
① 打开 data/shared/mech_deliverables.csv（机械组唯一手填表，列：param,value,unit,week,note）
② 只填 value 列（J/B/Ks/Ds 等），单位见 unit 列
③ 仓库根目录运行同步：
     python tools/sync_mech_to_elec.py
   → 自动写回 data/mechanical/mech_params.json + 生成 electrical/src/params_from_mech.h（含 ω_n 与建议带宽 2~5×）
④ 把 csv 和生成的 json/h 一起 git add + commit（电控组 pull 即拿到）
⑤ 群里说一声"机械参数 vN 已更新"
```

> ω_n 由脚本按 √(Ks·(1/J₁+1/J₂)) 自动算，不用手填；`version` 记得每次交付 +1（写在 csv 的 note 或同步维护）。

---

## 2. `mech_params.json` 字段说明（供理解与排错，**手填请去 csv**）

按下面的字段说明填，**只改数值和文字，不要改字段名**：
（JSON 里双引号必须成对、逗号不能多也不能少，写完把文件拖进 VS Code 看有没有红波浪线）

### 2.1 `meta` —— 版本与交付信息（必填）

| 字段 | 含义 | 填写要求 |
|---|---|---|
| `version` | 版本号 | 每次改动 **+1**（v1 → v2 → v3），不许原地改数值 |
| `delivered_by` | 交付人 | 姓名 |
| `date` | 交付日期 | `YYYY-MM-DD` |
| `source` | 数据来源 | 如"CATIA 测量惯量（MKS 单位）"/"手册查表" |

### 2.2 `motor` —— 电机与惯量（必填）

| 字段 | 单位 | 含义 |
|---|---|---|
| `J` | kg·m² | 四台电机转动惯量（**含折算到电机轴的负载**），数组 4 个数 |
| `B` | N·m·s/rad | 四台电机黏性摩擦（手册/经验值，软件算不出） |
| `perturb_pct` | % | 参数摄动幅度（当前 ±10） |
| `perturb_reasons` | — | 摄动来源，如 `["制造公差","温升","磨损"]` |

### 2.3 `coupling` —— 弹性传动链（方案 A 核心，重点）

| 字段 | 单位 | 含义 |
|---|---|---|
| `Ks` | N·m/rad | 联轴器扭转刚度（**查手册**，填不上就先写 `null`） |
| `Ds` | N·m·s/rad | 联轴器阻尼（可取临界阻尼 1%~5%） |
| `scheme` | — | 联轴器选型方案名，如"膜片联轴器 JMⅡ-2" |
| `I_ratio` | — | 减速比（直连填 1.0） |
| `eta` | — | 传动效率（直连填 1.0） |

> **顺手算一个数交给电控组**（很重要，是两组接口）：
>
> ```
> ω_n = √( Ks × (1/J1 + 1/J2) )
> ```
>
> 脚本会自动算并写进 `mech_deliverables_resolved.json` 与 `params_from_mech.h`，无需手填。电控组的观测器带宽必须覆盖它的 2~5 倍。

### 2.4 `load_physics` —— 负载工况的物理来源

负载**幅值**归电控组设定（工况属课题设计），机械组只需说明它们**对应什么真实工况**：

| 字段 | 填写要求 |
|---|---|
| `step` | 突加负载对应什么场景，如"传送带突然上料" |
| `sin` | 周期波动负载对应什么场景，如"旋转刀盘周期切削" |
| `ramp` | 斜坡加载对应什么场景，如"逐渐加压" |

> 这三句话是报告里"负载为什么这么设"的依据，评审必问，**不能空着**。

---

## 3. 交付流程

```
① 按上面填好 mech_params.json（version +1）
② git add data/mechanical/mech_params.json → commit → push
③ 在群里说一声"机械参数 vN 已提交"
        ↓  电控组执行
④ python electrical/c/merge_params.py      （自动校验参数是否合理）
⑤ 重新编译运行仿真 → check_acceptance.py 出达标判定
⑥ 把结论反馈给你们：达标则冻结；不达标则开变更单，回 ①
```

**注意**：不要直接改 `data/params.json`（它是自动生成的合并产物），
也不要改 `data/electrical/ctrl_params.json`（那是电控组的控制参数）。
两组各自的文件合起来，才是完整的一套仿真参数。

---

## 附：本文件夹在两组协同中的位置

```
data/mechanical/mech_params.json   ← 你们只写这里
data/electrical/ctrl_params.json   ← 电控组写这里
                ↓ merge_params.py 合并
        data/params.json  +  build/params_generated.h
                ↓
        仿真运行 → check_acceptance.py 判定达标
```

完整机制见 `docs/collaboration.md`。

## 4. 自检清单

- [ ] `version` 已递增，`delivered_by` / `date` 已填
- [ ] `J`、`B` 都是 4 个数，单位是 SI（kg·m²、N·m·s/rad）
- [ ] 惯量是**绕电机轴**的值（不是各零件自身惯量直接相加）
- [ ] `Ks` 已查手册填入，并算出 `omega_n_rad_s`
- [ ] `load_physics` 写清了每个负载的物理来源
- [ ] 文件无语法错误（VS Code 里没有红波浪线）
