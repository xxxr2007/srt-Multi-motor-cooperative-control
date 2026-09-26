# 文献日报（电控 + 机械，每日自动推送）

> **用途**：边做边学——每天自动从网上捞 3~4 篇电控 + 3~4 篇机械的最新文献/方向，
> 并在每条批次末尾写「可参考方向」，给本项目（多电机同步 + 双惯量弹性系统）提供先进借鉴。
> **机制**：由每日自动化任务（recurring automation）追加新批次；本文件首条为种子批次（2026-09-23）。
> **姊妹文档**：创新点汇总 `docs/Follow-up/innovation.md`；跨组数据源 `data/shared/mech_deliverables.csv`。

---

## 📅 2026-09-26（每日自动推送）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **Liu J., Yang J., Wang Z., Chen K.** *Ring-Coupled Nonlinear Adaptive PI Coordinated Control Strategy for Multi-PMSMs with Event-Triggered Mechanism*, **Machines**, 2026, 14(8):869. — 环耦多 PMSM + **事件触发非线性自适应 PI**：自适应律估不确定性、事件触发机制实时更新控制量，四 PMSM 环耦台架信号传输较时间触发减少 **92.1%**，精度相当。DOI:10.3390/machines14080869
2. **Wei W., Zhou Z., Liu Y., Wang J.** *Model-free adaptive predictive synchronous control of multiple PMSMs under multi-agent dynamic event triggering*, **Engineering Research Express**, 2025, 7:035363. — **无模型自适应预测同步 + 多智能体动态事件触发**：基于 I/O 数据建动态线性化模型，各 PMSM 为 agent 跟踪 leader，动态事件触发协议省通信，实验验证有效。DOI:10.1088/2631-8695/ae000a
3. **Wang Z., Gao X.** *High-precision synchronous control for multi-motor system: an event-triggered model predictive iterative learning approach*, **Measurement Science and Technology**, 2026. — **事件触发 MPC + 迭代学习（EMPIL）+ 终端积分滑模**：集总不确定性建模 + 差分动态解耦主动补偿，迭代学习 + 终端积分滑模抑扰，事件触发嵌入滚动时域省算力通信，机器人关节多电机角度高精度 + 协同一致 + 节能。DOI:10.1088/1361-6501/aea2b2
4. **Lu W., Lu S., Zheng S., Song B.** *Load-side resonant suppression based on adaptive state feedback and decoupled sliding-mode observer for servo system*, **Trans. Institute of Measurement and Control**, 2025, 48(11):2882–2892. — 双惯量**解耦滑模观测器（DSMO）**同时估负载转矩与惯量（解耦项消除与转速误差的耦合、降抖振）+ 自适应状态反馈调负载侧阻尼，压负载侧振荡、提谐振抑制效率。DOI:10.1177/01423312251361587

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **戴昊, 鲁文其, 鲁玉军 等** *基于自适应陷波滤波器的永磁伺服系统共振抑制*, **电子科技**, 2025, 38(9):58. — 双惯量弹性负载系统，推导电机惯量与机械谐振频率关系；**可调带宽/深度陷波（双线性变换）+ 归一化估计算法在线辨识谐振频率**，辨识精度 **1.87%**，稳态转速误差 6.0%→2.8%，双谐振点 1.28 s 内更新。DOI:10.16180/j.cnki.issn1007-7820.2025.09.008
2. **Wang L.** *Multi-inertia servo transmission system for motor under composite control algorithm considering resonance point changes*, **Int. J. of Dynamics and Control**, 2025, 13(6). — 多惯量耦合模型 + **预测模型 + 三参数陷波**复合控制，陷波随谐振点变化实时估并主动抑制，冲击负载下速度响应 ≤502 r/min，优于对比。DOI:10.1007/s40435-025-01743-1
3. **Zhang J., Zheng C., Qian H., et al.** *Resonance mechanism analysis of flexible shaft transmission system*, **IET Conference Proceedings**, 2024 (Online 2025), 2024(13):1033–1039. — 柔钢轴多直线作动器同步场景，建双惯量伺服模型 → 扩展**三惯量模型**，分析谐振主因，为更鲁棒控制奠基（北航）。DOI:10.1049/icp.2024.3027
4. **（电气传动 2025）** *双惯量系统高阻尼位置控制参数设计（谐振比控制）*, **电气传动**, 2025(1). — 在传统三环控制基础上结合**谐振比控制**提高速度闭环阻尼、优化闭环零点，提出高阻尼位置控制参数设计，不同惯量比下性能一致、显著降到位抖动。链接：https://castjournals.cast.org.cn/joweb/dqcd/CN/PDF/1190325457112367765

### 💡 本批「可参考方向」
- **事件触发 + 多智能体同步（低通信开销新基线）**：把 Liu 2026 的环耦非线性自适应 PI + 事件触发（信号传输降 92.1%）与 Wei 2025 的无模型自适应预测 + 动态事件触发结合，作本项目四电机台架的"低通信开销同步"对照基线——特别适合 W13 之后迭代期或分布式/网络化部署，呼应 `innovation.md`「多智能体事件触发」方向。（→ §五 候选点：电控·无模型自适应预测 + 多智能体事件触发）
- **事件触发 MPC + 迭代学习（EMPIL）作快速预测新基线**：Wang 2026 的"事件触发 + MPC + 迭代学习 + 终端积分滑模"在机器人关节多电机上实现角度高精度 + 协同一致 + 节能，可直接对照本项目四策略，作"无权重因子快速预测"对照，呼应创新点三逐轮迭代寻优。（→ §五 候选点：电控·模型预测同步（无权重因子快速预测））
- **解耦滑模观测器（DSMO）估负载惯量/转矩 + 自适应状态反馈压负载侧谐振**：Lu 2025 的双惯量 DSMO 同时估负载转矩与惯量（解耦项消耦合、降抖振）+ 自适应状态反馈调负载侧阻尼，正可接进本项目 DCC+DOB 作"高阶滑模观测"进阶，且直接服务创新点二谐振抑制（负载侧振荡是 `g/ω_n=0.30` 缺口之外的另一短板）。（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））
- **在线自适应陷波（治 `g/ω_n=0.30` 缺口，最优先）**：戴昊 2025（双惯量弹性负载、谐振频率在线辨识精度 1.87%）+ Wang L. 2025（预测模型 + 三参数陷波随谐振点变化自适应）把本项目"经验定 g=100"升级为"据 ω_n 在线辨识 + 自整定陷波"，直接补创新点二 `g/ω_n=0.30` 欠阻尼缺口，优先做（对应 `innovation.md` ④ 最小验证路径）。（→ §五 候选点：机械·谐振频率在线辨识 + 自适应陷波）
- **三惯量扩展 + 谐振比控制（带宽-刚度匹配再升级）**：Zhang 2024 IET 的柔轴双惯量→三惯量建模 + 电气传动 2025 的谐振比控制（按惯量比/谐振比定速度环结构），把本项目线性双惯量扩成三惯量、并按"带宽-刚度匹配"量化选刚度（谐振比落优区），形成创新点二进阶 + 创新点三方法论新证据，也呼应机械组刚度分级选型（→ §五 候选点：机械·柔性联轴器刚度分级 / 可调）。（→ §五 候选点：机械·三惯量扩展）

---

## 📅 2026-09-25（每日自动推送）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **Li M. et al.** *Adaptive NN Observer-Based Synthesize Strategy for Connected Nonlinear Multi-Motor Servo System*, **IEEE Trans. Automation Science and Engineering**, 2025. — 自适应**滑模扰动观测器**（有限时间收敛）+ 自适应 NN 跟踪 + 滑模同步控制，处理联网非线性同构多电机的同步与全状态约束。DOI:10.1109/TASE.2025.3564331
2. **Wang J., Liu Y., Wang B., Cai M.** *Recursive Sliding Mode Control of Dual-Motor Synchronous Drive Servo System Based on Disturbance Observer*, **Trans. Institute of Measurement and Control**, 2026 (OnlineFirst). — **递推非奇异终端滑模（RNFTSM）** + 每机独立**有限时间扰动观测器**前馈 + 同步反馈信号，四滑面有限时间收敛、抑制抖振。DOI:10.1177/01423312261471936
3. **Wang M., He E., Ma D., Zhou P., Wang Z.** *Data-Driven Adaptive Synchronous Control for Multi-PMSM Drive Systems under Severe Asymmetrical Load Disturbances*, **Electrical Engineering (Springer)**, 2026, 108:409. — **超扭曲滑模区间观测器**实时估集总负载扰动前馈 + 自适应 NFTSMC，六电机 TBM 缩比台架：非对称负载下转矩同步 RMSE 暂态降 88.6%、稳态降 93.7%（对比 PI-VACC）。DOI:10.1007/s00202-026-03769-w
4. **Zhang X., Chen J., Sun Z., et al.** *Speed Collaborative Pre-Compensation Control of Dual PMSM Systems*, **Journal of Power Electronics**, 2026, 26(6):1347–1361. — 引入 **MPC 增量预测模型**，最小化含同步/跟踪误差的价值函数得最优补偿量 v1/v2，提前注入 q 轴电压，结构简单、调节时间更短。DOI:10.1007/s43236-025-01149-4

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **Yang J., Pan Z., Cheng G., Yu X.** *Low-Frequency Vibration Suppression Strategy Based on Dual Observers*, **37th Chinese Control and Decision Conference (CCDC)**, 2025. — 针对双惯量系统提出**双扩张状态观测器（双 ESO）**机械谐振抑制，Bode 分析 + Simulink/实验证有效压转速振动。DOI:10.1109/CCDC65474.2025.11090940
2. **Li C.** *Research on Resonance Suppression Methods for Servo Systems*, **LNEE (China Electrotechnical Society Annual Conf.)**, 2026, 1581:381–391. — 双惯量建模解析固有/反谐振频率，对比**陷波 / PI 极点配置 / ADRC** 三法，ADRC 在压谐振、抑超调、动态稳定上综合最优。DOI:10.1007/978-981-95-7652-4_41
3. **Wang B., Pan J., Xu D.** *Logarithmic Chirp Identification and Decoupled Analytical Active Damping for Mid-Low Frequency Resonance in Dual-Inertia Servo Systems*, **IEEE Trans. Power Electronics**, 2026, 41(12):20828–20841. — **对数扫频（log chirp）辨识**反/谐振 + **解析有源阻尼**（q 轴电流注入，极点配到 ζ=0.707 免整定）+ **低频偶极子隔绝 DC 负载**，49/140 Hz 实验速度纹波衰减 >96.99%。DOI:10.1109/TPEL.2026.3715682
4. **Tie Y., Li X., et al.** *Modeling and Pole Placement Control for Vibration Suppression in Motor-Gear Coupled Systems with Friction Torque*, **Proc. Inst. Mech. Eng. Part C (SAGE)**, 2026 (OnlineFirst). — 建含**齿轮摩擦转矩 + 背隙**的二惯量模型，基于极点配置的多环路主动扰动抑制，降超调、增稳定。DOI:10.1177/09544070261454575

### 💡 本批「可参考方向」
- **自适应有源阻尼 + 偶极子隔直（治 g/ω_n=0.30 缺口）**：把 Bo Wang 2026 的「解析有源阻尼（极点配 ζ=0.707）+ 低频偶极子隔绝 DC 负载」与 Yang 2025 的「双 ESO」结合，把本项目「经验定 g=100」升级为「据 ω_n 解析整定有源阻尼 + 偶极子隔直」，直接对应创新点二当前 `g/ω_n=0.30` 欠阻尼缺口，优先做。（→ §五 候选点：电控·自适应陷波 / 有源阻尼系统化设计（替代经验定 g））
- **PLL-ESO / ADRC 谐振观测接进 DCC+DOB**：Li Changzhi 2026 用 ADRC 比 notch/PI 更优地压谐振；连同已有吴春 2024 的 PLL-ESO，可把「机械谐振前馈补偿」嫁接进本项目 DCC+DOB，作创新点一进阶基线。（→ §五 候选点：电控·PLL-ESO 谐振观测）
- **含齿隙/摩擦的非线性双惯量建模**：Tie 2026 的电机-齿轮耦合二惯量（齿轮摩擦 + 背隙）正可替换本项目线性 Ks-Bs 对象，让仿真更贴真实，反哺创新点一/二。（→ §五 候选点：机械·含齿隙/摩擦的非线性双惯量模型）
- **超扭曲滑模区间观测应对非对称负载**：Meng Wang 2026 的「超扭曲滑模区间观测器 + 自适应 NFTSMC」在六电机 TBM 台架把非对称负载下转矩同步 RMSE 降 88.6%（暂态）/93.7%（稳态），可直接对照本项目「参数摄动 + 单机突加负载」场景，作 DCC+DOB 的强扰动对照基线。（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））
- **MPC 速度协同预补偿（四策略之外的新基线）**：Zhang Xiuyun 2026 的 MPC 增量预测 + q 轴电压前补偿思路，可作四策略之外的「快速预测同步」新基线，呼应 W9–W13 迭代寻优，也可与 Wang 2025 动态耦合增益组合。（→ §五 候选点：电控·模型预测同步（无权重因子快速预测））

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
