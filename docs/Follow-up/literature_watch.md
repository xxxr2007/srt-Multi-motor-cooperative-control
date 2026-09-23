# 文献日报（电控 + 机械，每日自动推送）

> **用途**：边做边学——每天自动从网上捞 3~4 篇电控 + 3~4 篇机械的最新文献/方向，
> 并在每条批次末尾写「可参考方向」，给本项目（多电机同步 + 双惯量弹性系统）提供先进借鉴。
> **机制**：由每日自动化任务（recurring automation）追加新批次；本文件首条为种子批次（2026-09-23）。
> **姊妹文档**：创新点汇总 `docs/Follow-up/innovation.md`；跨组数据源 `data/shared/mech_deliverables.csv`。

---

## 📅 2026-09-23（种子批次 · 手动建库）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **Gao S. et al.** *A review of multi-motor coordinated control technologies: advanced strategies, intelligent algorithms, and future trends*, **Renewable and Sustainable Energy Reviews**, 2026 (225):116188. — 综述：架构从集中→分布→混合智能演进；未来方向 = 异构电机集成、能效-精度协同优化、**数字孪生实时增强**、边缘计算分布式架构。DOI:10.1016/j.rser.2025.116188
2. **Zhang D. et al.** *Cross-Coupling Synchronous Control of Dual-Motor Based on Improved ADRC–Nonsingular Fast Terminal SMC*, **Electronics**, 2025, 14(3):526. — ADRC-NFTSMC：响应速度 +18.9%、同步精度 +46.7%（对比 NFTSMC）。DOI:10.3390/electronics14030526
3. **Wu K. et al.** *Cross Coupling Method Based on Improved GFTSMC and Disturbance Observer*, **Appl. Sci.**, 2025, 15(4):1915. — 全局快速终端滑模 + DOB 估计**摩擦转矩**实时补偿。DOI:10.3390/app15041915
4. **Wang Z. et al.** *Adaptive Weight Predictive Position Synchronization Control … Dynamic Coupling Gain*, **IEEE ACEEE**, 2025. — 增量连续集 MPC + **动态耦合增益**实时更新权重，提升双 PMSM 位置同步精度。

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **吴春 等** *基于锁相环型扩张状态观测器（PLL-ESO）的双惯量弹性伺服系统机械谐振抑制*, **电工技术学报**, 2024, 39(18):5680–5691. — PLL-ESO 比陷波滤波器/常规 ESO **更快更准**，宽 ω_n 范围有效，对参数摄动鲁棒。DOI:10.19595/j.cnki.1000-6753.tces.231093
2. **Zhang J. et al.** *Resonance mechanism analysis of flexible shaft transmission system*, **IET AUS**, 2024. — 柔轴+多惯量谐振机理，双惯量→三惯量建模，为多电机协同鲁棒控制奠基。
3. **Xu J. et al.** *Dynamic characteristics analysis of gear system … based on motor control strategy*, **Nonlinear Dynamics**, 2024. — 柔性联轴器等效柔性关节，BP-PID 自适应整定。DOI:10.1007/s11071-023-09193-0
4. **（赛事方向非论文）** 第十二届全国大学生机械创新设计大赛主题「灵巧·智能，美好生活」（水产品/叶菜/仿生蝴蝶/扇贝开半壳，2026 决赛）——机构创新 + 实物样机，正是本项目机械组练兵场。

### 💡 本批「可参考方向」
- **谐振抑制 × 复合策略**：把电工技术学报的 **PLL-ESO** 谐振观测，嫁接进本项目「DCC+DOB」复合策略，形成「机械谐振前馈补偿 + 同步误差闭环」双保险，可写成一条新创新点。
- **动态耦合增益**：用 IEEE 那篇的**动态耦合增益**替代本项目固定的偏差耦合权重，做 W9–W13 迭代寻优，呼应 `innovation.md` 创新点三「协同迭代」。（→ §五 候选点：电控·动态耦合增益）
- **(Ks, g) 协同寻优**：机械侧把柔性联轴器刚度做成**可调/分级**，电控侧按「带宽-刚度匹配准则」自动扫 ω_n 的 2~5 倍带宽区间，二者合起来就是申报书里的「机电磁协同参数匹配」亮点。
- **数字孪生 / 边缘计算**：作为 W13 之后迭代期（见 `weekly_tasks_iteration.md`）的进阶方向，先记着，不打当前基线。（→ §五 候选点：电控·数字孪生/边缘计算）

---

<!-- 新批次由自动化任务追加在上方（按日期倒序）。格式参考首条。 -->
