# 重点知识点

## [KEY][带宽-刚度匹配准则]（创新点二核心）
- 双惯量弹性模型固有频率：`ω_n = √(Ks·(1/J₁ + 1/J₂))`。当前机械组交付 Ks=200、J₁=0.002、J₂=0.02 → `ω_n ≈ 331.7 rad/s`。
- 准则：扰动观测器（DOB）/ 陷波滤波器的带宽需覆盖 **2~5 倍 ω_n** 才能有效抑制弹性谐振。
- 当前缺口（实测）：DOB 带宽 g=100 仅覆盖 ω_n 的 **0.30 倍**，远不足 2~5 倍要求。
- 实测佐证：弹性模式下 `dcc_dob` 同步误差 RMS=**0.223** 反而劣于 `CCC/DCC` 的 **0.149** —— 带宽不够的 DOB 反而帮倒忙。这是申报书 / 答辩的硬证据，比空讲理论硬。

## [KEY][CSV 单一数据源]（跨组协作机制）
- 机械组 **只在** `data/shared/mech_deliverables.csv` 手填参数。
- 跑 `tools/sync_mech_to_elec.py` → ① 生成 `electrical/src/params_from_mech.h`（电控 `#include` 即用）② 写回 `mechanical/params/mech_params.json`（代码真源）③ 自动调 `merge_params.py`。
- 电控组 **不要手填** 机械参数，只读生成的头文件。避免两套参数源打架（之前就因 xlsx / csv 双源不一致踩过坑）。

## [KEY][Git 铁律]（本项目）
- 所有 git 命令用 `git -C "C:/Users/31394/Desktop/srtfangzhen" <子命令>` 绝对路径，禁止 `cd 仓库 && git`（本机 shell 偶发 `cd: null directory` 会把后续命令带偏、误删文件）。
- 任何 `git rm` / 删除类动作后，**必须立刻 `git status --short` 全量核对**；发现非预期删除立即 `git checkout -- <文件>` 恢复。
- 禁止 `git fetch --prune`（本机会误删本地全部远程跟踪引用）。

## [KEY][会话内 push 真相]
- 能否 `git push` 完全取决于 **Clash 代理是否在线**：开了 → 会话内 `git push origin main` 可成功；关了 → `schannel: failed to receive handshake`（exit 128）。
- push 前先探：`curl -s -o /dev/null -w "%{http_code}" -x http://127.0.0.1:7897 https://github.com` → 返 200 直接推，返 000 让用户去 SourceGit 点按钮。别再无条件说“会话内推不了”。
