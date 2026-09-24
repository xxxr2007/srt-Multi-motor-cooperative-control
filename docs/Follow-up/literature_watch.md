# 文献日报（电控 + 机械，每日自动推送）

> **用途**：边做边学——每天自动从网上捞 3~4 篇电控 + 3~4 篇机械的最新文献/方向，
> 并在每条批次末尾写「可参考方向」，给本项目（多电机同步 + 双惯量弹性系统）提供先进借鉴。
> **机制**：由每日自动化任务（recurring automation）追加新批次；本文件首条为种子批次（2026-09-23）。
> **姊妹文档**：创新点汇总 `docs/Follow-up/innovation.md`；跨组数据源 `data/shared/mech_deliverables.csv`。

---

## 📅 2026-09-24（每日自动推送）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **Li X. et al.** *A Global Fixed-Time Measurement Control Strategy for Multimotor System With Lumped Disturbance*, **IEEE Trans. Instrumentation and Measurement**, 2026, 75:3002910. — 未知输入双幂**固定时间观测器**估计并前馈补偿集总扰动（外部扰动+参数摄动），收敛时间独立于初值且更节能，dSPACE/RT-LAB 半实物验证。DOI:10.1109/TIM.2026.3701197
2. **Li L., Wang Q., Li Y.** *Dual-Motor Position Control Based on a Synchronous State Observer*, **Machines**, 2026, 14(6):681. — **同步状态观测器**实时重建并前馈补偿「同步扰动」（传动参数失配、轴间力矩不平衡），最大位置同步误差 < 6.3×10⁻⁴°，电流偏差 ±0.25 A。DOI:10.3390/machines14060681
3. **金雪峰 等** *基于ESO的多电机位置同步模型预测控制*, **天津工业大学学报**, 2025, 44(06):74–81+90（网络首发 2026-01）. — ESO 观测位置环扰动作前馈 + MPC（价值函数含轮廓/跟踪/控制增量），3 台 PMSM 螺旋线轨迹跟踪误差 1.78→0.56 mm。链接：https://cjournal.hep.com.cn/1671-024X/CN/1217769014924914991
4. **Hou L., Jin X.** *Leader-Follow Consistency Multi PMSM Speed Collaborative New Control System*, **LNEE (China Electrotechnical Society Annual Conf.)**, 2026, 1564:529–544. — 多智能体固定时间一致性 + **固定时间扩张状态观测器（FTESO）** 前馈补偿，对比偏差耦合显著降低超调与抖振、提升同步精度。DOI:10.1007/978-981-95-7334-9_55

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **Ai W. et al.** *Resonance Suppression of Flexible-Transmission Servo Systems with Fractional-Order Dual-Inertia Modeling and Singular Perturbation*, **IEEE MESA**, 2026 (Beijing). — **分数阶双惯量**模型刻画粘弹/记忆特性 + 奇异摄动分解，分数阶主动阻尼比整数阶更优地衰减负载侧谐振且保跟踪。DOI:10.1109/MESA70578.2026.11684359
2. **张广泽 等** *数控机床双惯量进给传动系统机电耦合建模与谐振抑制*, **制造技术与机床**, 2026（录用 2026-09-08，网络出版 2026-09-15）. — 双惯量等效动力学建模定量算固有频率；单/双频窄带**陷波**定点/同步抑双阶模态（速度 PI 输出端嵌入），五阶**龙伯格速度观测器**全域削弱时变齿槽脉动。链接：https://1951.mtmt.com.cn/article/id/51cc41ea-f709-4266-a0e4-a27b4064e484
3. **焦鹏程** *电机驱动机电耦合位置控制系统设计方法的研究*, **湖北工业大学 硕士**, 2026. — 基于机电耦合二惯量模型分析不同电机-负载**惯量比**下的阻尼特性，给出半闭环位置控制参数整定与增益特征优化策略。链接：https://inds3.cnki.net/kmobile/Master/detail/AKSM/1026355854.nh
4. **莫梓浩** *伺服二惯量系统谐振抑制技术的研究*, **广东工业大学 硕士**, 2025. — 建二惯量模型推传递函数，设计改进模糊 PI + 新型滑模控制器抑制柔性环节引发的转速/转矩振荡，对比传统 PI 抗谐振更优。链接：https://inds3.cnki.net/kmobile/Master/detail/CXPM/1025505133.nh

### 💡 本批「可参考方向」
- **固定时间/扩张状态观测器升级现有 DOB**：把 Li 2026 / Hou 2026 的**固定时间观测器**思路引入本项目「DCC+DOB」，对集总扰动（负载+摩擦+参数摄动）做固定时间估计与前馈，比一阶 DOB 收敛更快、对初值不敏感，可作创新点一「DOB 再进阶」对照基线。（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））
- **同步状态观测器解耦同步与跟踪**：借鉴 Li 2026 的同步状态观测器，把「同步误差」与「跟踪」分开估计再前馈补偿，正好对应本项目可扩展的偏差耦合拓扑升级。（→ §五 候选点：电控·状态均值偏差耦合拓扑）
- **ESO + MPC 多电机位置同步**：把 Jin 2025 的 ESO 前馈 + MPC 价值函数（含轮廓/跟踪/控制增量）作为四策略之外的「无权重因子快速预测」新基线，丰富创新点三对比维度。（→ §五 候选点：电控·模型预测同步（无权重因子快速预测））
- **分数阶双惯量建模 + 主动阻尼**：将 Ai 2026 的分数阶双惯量模型替换本项目线性 Ks-Bs 对象，分数阶主动阻尼更准地压负载侧谐振，反哺创新点二「(Ks, g) 协同」的建模精度。（→ §五 候选点：机械·分数阶双惯量系统建模与参数辨识）
- **双频陷波 + 龙伯格抑振（直接治 g/ω_n=0.30 缺口）**：Zhang 2026 的单/双频陷波定点/同步衰减双阶模态 + 龙伯格估齿槽扰动，正是把本项目「经验定 g=100」升级为「据 ω_n 系统化整定陷波/有源阻尼」的可落地参照，优先做。（→ §五 候选点：电控·自适应陷波 / 有源阻尼系统化设计（替代经验定 g））

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
