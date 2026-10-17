# 文献日报（电控 + 机械，每日自动推送）

> **用途**：边做边学——每天自动从网上捞 3~4 篇电控 + 3~4 篇机械的最新文献/方向，
> 并在每条批次末尾写「可参考方向」，给本项目（多电机同步 + 双惯量弹性系统）提供先进借鉴。
> **机制**：由每日自动化任务（recurring automation）追加新批次；本文件首条为种子批次（2026-09-23）。
> **姊妹文档**：创新点汇总 `docs/Follow-up/innovation.md`；跨组数据源 `data/shared/mech_deliverables.csv`。

---

## 📅 2026-10-14（每日自动推送）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **Huynh D.-T., Pham C.-T., Lam H.-Q.-D., Trieu V.-T., Nguyen M.-T.** *Distributed Backstepping Integral Terminal Sliding Mode Control for Consensus Speed Tracking in Networked PMSM Systems*, **Energies**, 2026, 19(18):4438. — 把网络化多 PMSM 化为**有向通信图上的 leader–follower 一致问题**：速度环给 q 轴参考、本地用**积分终端滑模面**调 dq 电流，在显式扰动/估计误差界下证**有限时间到达滑模面**、再用 ISS 给出物理转速误差上界、并以 pinned graph matrix 显式暴露**拓扑依赖**；四智能体仿真较基线峰-峰瞬态转速散布降约 83%、稳态散布降 >90%（两支路存档数据带载侧稳态一致 RMSE 较 PID 低 91.5%）。正好把本项目「偏差耦合」从定权重耦合升为**图论一致 + 终端滑模**的网络化结构，也是「多智能体」候选的现成对照。DOI:10.3390/en19184438 （链接：https://www.mdpi.com/1996-1073/19/18/4438）
2. **Wang Z., Gao X.** *High-precision synchronous control for multi-motor system: an event-triggered model predictive iterative learning approach*, **Measurement Science and Technology**, 2026（2026-09-15 在线）. — 提**事件触发模型预测迭代学习（EMPIL）**：用集总不确定性建模 + 差分动态主动补偿实现**动态解耦**，再用迭代学习 + 终端积分滑模互补抑多源扰动、不强依赖精确量测；**事件触发机制嵌进滚动时域**打破传统周期 MPC 范式，按需匹配能效与跟踪精度，动捕系统实测证角度跟踪/协调一致性/能效均优。把本项目「模型预测同步」做成**无权重因子 + 事件触发**的轻量版，直接呼应「无模型自适应预测 + 多智能体事件触发」候选。DOI:10.1088/1361-6501/aea2b2 （链接：https://doi.org/10.1088/1361-6501/aea2b2）
3. **Chen Y., Liu Y., Huang R., Liu C.**（中国香港 香港城市大学） *Data-Driven Speed Synchronization Predictive Control of a Five-Leg VSI Driving Dual-PMSM System*, **IEEE Trans. on Industrial Electronics**, 2026（在线 2026-02-03）. — 五桥臂逆变器驱动双 PMSM 的**数据驱动同步预测控制（DSSPC）**：基于特征建模建**等效一阶模型**做电流预测（不依赖电机参数）、改进 MPC 加占空比调节并考虑共桥臂开关约束、构**全局代价函数**统一选双电机最优电压矢量；关键是用**电流参考补偿**替代**传统定增益交叉耦合**，快速消除双电机同步误差；实验证同时压同步误差与跟踪误差、抗参数失配/外扰鲁棒更强。既是「模型预测同步（无权重因子快速预测）」候选的顶刊实验基线，也给「定增益交叉耦合 → 参考补偿」的**动态耦合增益**提供可抄机制。DOI:10.1109/TIE.2026.3657020 （链接：https://doi.org/10.1109/TIE.2026.3657020）
4. **Li G., Li P., Peng R., Wu D., Jiang L., Li Q., Li Z.** *Research on Synchronized Control Strategy of Drone Swarm Garage Side-Pulling Platform*, **IEEE Access**, 2025, 13:91795–91807. — 无人机机巢侧拉平台**双 PMSM 同步**：单电机侧把 **BP 神经网络 PID（BPPID）**嵌入速度环增鲁棒，电机间用**自适应模糊 PID 交叉耦合（AFPIDCC）**做速度补偿，较经典 PID 在不同满载/变转速工况改善 31.7%/27.2%/29.2%、单/双机同速负载下改善 37.2%/27.0%。正是本项目「交叉耦合 + 动态耦合增益（模糊 PID 随工况自整定补偿）」的低成本工程落版，本科生可直接借鉴其补偿器结构。DOI:10.1109/ACCESS.2025.3568850 （链接：https://ieeexplore.ieee.org/document/11000288/）

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **鞠锦勇**（安徽工程大学） *Novel non-contact adjustable torsional electromagnetic negative stiffness device: Theoretical analysis and application*, **Mechanical Systems and Signal Processing**, 2026. — 提**非接触式可调扭转电磁负刚度装置**：借轴向电磁耦合产生**旋转负刚度**、通过电流调控实现传动刚度**连续可调**，研样机做刚度特性与振动传递实验，证负刚度输出连续可调、线性度良好、能有效抑制传动系统扭振且鲁棒强。正好给本项目「柔性联轴器刚度分级 / 可调」提供一种**电流调控、可连续切档**的紧凑新结构（比机械档位更细），机械组可按 ω_n 连续调 Ks、电控侧据刚度档实时联动调 g。DOI:10.1016/j.ymssp.2026.114896 （链接：https://doi.org/10.1016/j.ymssp.2026.114896）
2. **张广泽, 王楚婷, 黄祖广, 薛瑞娟, 耿遵敏** *数控机床双惯量进给传动系统机电耦合建模与谐振抑制*, **制造技术与机床**, 2026（网络首发）. — 针对数控进给双惯量传动**弹性谐振 + 齿槽转矩扰动**：建双惯量等效动力学模型、解析谐振规律、按**弹性轴刚度 + 电机/负载惯量定量算多负载工况固有 ω_n**，构**单频/双频窄带陷波**并确定速度 PI 输出端为最佳嵌入位，再引入**五阶龙伯格观测器**；多负载扭转平台 + 滚珠丝杠扫频实验证**双频结构可同步衰减双阶模态**、其余频段动态特性不受扰。正好把本项目「经验定 g / 单陷波」升为「多负载 ω_n 定量 + 双频陷波 + 观测器」系统方案，且是「三惯量扩展」的机电耦合工程模板。链接：https://inds3.cnki.net/kmobile/Journal/detail/YJZJ/ZJYC2026091700L
3. **Yang J., Pan Z.Q., Cheng G.Y., Yu X.X.**（福州大学） *Low-Frequency Vibration Suppression Strategy Based on Dual Observers*, **IEEE Xplore**, 2025, pp. 6331–6336. — 针对弹性联轴器致**双惯量系统机械谐振**，提**双扩张状态观测器（dual-ESO）**抑低频振动：解析谐振成因与传函、用 Bode 图对比 PI / 单 ESO / 所提方法的工作原理，Simulink + 实验证有效抑制转速振荡。给本项目「PLL-ESO 谐振观测」候选一个「双 ESO 互补」的邻近结构：本科生可在 `control_dcc_dob.c` 旁把单 DOB 换成 dual-ESO 观测器、扫 Ks=12/200/2000 看低频谐振峰与同步 RMS。链接：https://ieeexplore.ieee.org/document/11090940
4. **（发明专利 CN120542017A，重庆大学）** *变刚度磁扭转减振缓冲器的隔振率计算、调控方法及系统*. — 把**环形 Halbach 磁阵列负刚度 + 弹簧扭转片正刚度**并联耦合成**准零刚度**，用等效磁荷法建模 + 谐波平衡法求解隔振率，再靠**间隙可调机构**实时调弹簧片刚度和磁环间隙，兼顾高扭矩传递与低频扭振抑制。给本项目「柔性联轴器刚度分级 / 可调」一种**正负刚度并联 + 间隙调档**的可量化结构（直接对应「联轴器选型量化手册」里的 Ks 档位表）。链接：https://patents.google.com/patent/CN120542017A

### 💡 本批「可参考方向」
- **事件触发预测 + 迭代学习把「模型预测同步」做成无权重因子轻量版（最贴本项目四策略外挂新基线）**：Wang & Gao 2026（Meas. Sci. Technol.）的 EMPIL 用集总不确定性建模做动态解耦、迭代学习 + 终端积分滑模补扰、事件触发嵌进滚动时域按需匹配能效与精度——本科生可在外挂轻量 MPC 时同步加事件触发判据（|跟踪误差|>阈值才解一次 QP），对比周期 MPC vs 事件触发 MPC 的同步 RMS 与计算量。（→ §五 候选点：电控·模型预测同步（无权重因子快速预测））；配套（→ §五 候选点：电控·无模型自适应预测 + 多智能体事件触发）
- **数据驱动同步预测 + 电流参考补偿替代定增益交叉耦合（模型预测 + 动态耦合双落地）**：Chen 等 2026（IEEE TIE）五桥臂双 PMSM 的 DSSPC 用等效一阶模型免参数预测、全局代价函数统一选矢量，并用**电流参考补偿替换传统定增益交叉耦合**快速消同步误差、抗参数失配/外扰更强——一条顶刊同时落地「模型预测同步」与「动态耦合增益」两候选，本科生可把 `control_dcc.c` 的固定耦合权重换成「按同步误差实时算的电流参考补偿」、对照四策略突加载 RMS。（→ §五 候选点：电控·模型预测同步（无权重因子快速预测））；配套（→ §五 候选点：电控·动态耦合增益）
- **图论一致 + 模糊 PID 交叉耦合把偏耦升为网络化/自适应两层结构（状态均值偏差耦合 + 动态耦合增益）**：Huynh 等 2026（Energies）把网络化多 PMSM 化为有向图 leader–follower 一致、本地积分终端滑模、拓扑依赖由 pinned graph matrix 显式给出，四智能体峰-峰散布降约 83%；Li 等 2025（IEEE Access）双 PMSM 用自适应模糊 PID 交叉耦合做速度补偿、较 PID 改善 27%–37%——前者给「状态均值偏差耦合拓扑（解耦同步与跟踪）」一个图论可扩展骨架、后者给「动态耦合增益」一个随工况自整定补偿器的低成本实现；本科生可先在 `control_deviation.c` 加模糊 PID 自适应权重、再把 4 电机通信图扩成 leader–follower。（→ §五 候选点：电控·状态均值偏差耦合拓扑（解耦同步与跟踪））；配套（→ §五 候选点：电控·动态耦合增益）
- **谐振观测 + 系统化陷波双升级（治 `g/ω_n=0.30`：dual-ESO 邻近结构 + 双频陷波 + 多负载 ω_n 定量）**：张广泽等 2026（制造技术与机床）按弹性轴刚度与电机/负载惯量定量算多负载 ω_n、构单/双频窄带陷波、定速度 PI 输出端为最佳嵌入位并引五阶龙伯格观测器、扫频证双频结构同步衰减双阶模态；Yang 等 2025（IEEE）用**双 ESO 互补**抑双惯量低频振动（Bode 对比 PI/单 ESO）——两者把本项目「经验定 g」升级为「据 ω_n 系统化双频陷波 + 多观测器」方案，直接治 `g/ω_n=0.30`；本科生可在 `control_dcc_dob.c` 旁把单 DOB 换 dual-ESO、并把固定陷波换双频结构，扫 Ks=12/200/2000 看双峰衰减与同步 RMS。（→ §五 候选点：电控·自适应陷波 / 有源阻尼系统化设计（替代经验定 g））；配套（→ §五 候选点：电控·PLL-ESO 谐振观测）；配套（→ §五 候选点：机械·三惯量扩展）
- **可调负刚度 / 正负刚度并联联轴给「刚度分级」上连续档（机械·刚度分级 + 选型量化手册）**：鞠锦勇 2026（MSSP）用轴向电磁耦合产生旋转负刚度、电流调控连续调传动刚度并样机验证抑扭振；CN120542017A（重庆大学）把 Halbach 磁负刚度与弹簧扭转片正刚度并联成准零刚度、靠间隙可调机构调档——两者给本项目「柔性联轴器刚度分级 / 可调」提供「电磁连续可调」与「正负刚度并联 + 间隙调档」两类可量化结构，机械组可据此把选型手册的 Ks 从 12/200/2000 三档扩成连续可调档、电控侧据档联动调 g。（→ §五 候选点：机械·柔性联轴器刚度分级 / 可调）；配套（→ §五 候选点：机械·联轴器选型量化手册）

---

## 📅 2026-10-12（每日自动推送）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **(Electronics, 2025)** *Study on Cross-Coupling Synchronous Control Strategy of Dual-Motor Based on Improved Active Disturbance Rejection Control–Nonsingular Fast Terminal Sliding Mode Control Strategy*. — 把 **ADRC 与 NFTSMC 融合**嵌进**交叉耦合**框架：提**离散非线性跟踪微分器（DNLTD）+ 离散非线性扩张状态观测器（DNLESO）**估扰并前馈，配优化趋近律的 NFTSMC；双电机伺服突加非均匀负载下较纯 NFTSMC 响应速度 +18.9%、同步精度 +46.7%；正好把本项目「DCC+DOB」的一阶 DOB 升级为「离散 NLP + NFTSMC」滑模观测进阶结构。DOI:10.3390/electronics14030526 （链接：https://www.mdpi.com/2079-9292/14/3/526）
2. **Zhiqiang Wang, Heyu Wang, Baorui Zhao, Xiuyun Zhang** *Predictive Synchronous Control of Dual Permanent Magnet Synchronous Motor Based on Stability Analysis*, **2025 8th Asia Conf. on Energy and Electrical Engineering (ACEEE)**, 2025, pp. 95–99. — 把双电机**位置/速度同步误差、跟踪误差与控制量**统一建模为多目标**预测同步**问题，构动态耦合状态空间做多步预测、时域优化求最优解，并用 Lyapunov 证有界扰动下跟踪/同步误差收敛到与扰动幅值成比例的邻域；仿真证同步同时保单机跟踪——正好把本项目「模型预测同步」候选落到双电机对象并给稳定性证明。DOI:10.1109/ACEEE66257.2025.11148859 （链接：https://doi.org/10.1109/ACEEE66257.2025.11148859）
3. **Lixin Liu, Cheng Liu, Changjin Che, Yunbo Wu, Qing Zhao** *Research on the Coordinated Control of Mining Multi-PMSM Systems Based on an Improved Active Disturbance Rejection Controller*, **Electronics**, 2025, 14(3):477. — 三电机皮带输送**环耦（ring coupling）**同步，提**增强一阶 ADRC（Efal_ADRC，优化非线性函数）** + 用**加权平均（weighted arithmetic mean）**增强环耦补偿器；较 PID 抗扰、控制精度与同步性更优——正好把本项目「状态均值偏差耦合拓扑」做成「环耦 + 加权平均增强」的可抄工程结构。DOI:10.3390/electronics14030477 （链接：https://www.mdpi.com/2079-9292/14/3/477）
4. **（发明专利 CN120675458A）** *Cross coupling cooperative control method for double permanent magnet synchronous motors of aviation airborne suspension lifting system*. — 提**动态拓扑权重 + 一致性协议 + 模糊 ADRC**的分层协同：按负载差/耦合效应**动态分配权重**、基于一致性协议生成协同校正量做负载扰动下误差均衡分配；正好给本项目「动态耦合增益」一种「按工况自整定权重」的飞机悬挂提升双电机落地结构。链接：https://patents.google.com/patent/CN120675458A

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **Wang X., Su Y., Luo Y. 等** (华南理工大学) *Fractional-Order Modeling and Identification for Dual-Inertia Servo Inverter Systems with Lightweight Flexible Shaft or Coupling*, **Fractal Fract**, 2025, 9(4):222. — 把双惯量伺服**扩为分数阶模型**（捕黏弹/记忆效应）捕捉谐振更准，用**输出误差法 + Levenberg–Marquardt（LM）算法**做分数阶参数辨识，PMSM 伺服实验平台验证显著提升双惯量系统表征精度；正好是 innovation.md §五「分数阶双惯量系统建模与参数辨识」点名的 Fractal Fract 2025 专刊锚点，可直接替换本项目整数阶 Ks-Bs 为分数阶。DOI:10.3390/fractalfract9040222 （链接：https://www.mdpi.com/2504-3110/9/4/222）
2. **Hafez I., Dhaouadi R.** *Parameter Identification of a Flexible-Joint Robot Axis using Sinusoidal Position Tracking*, **J. Intell. Robot. Syst.**, 2025. — 用**正弦位置跟踪控制**辨识柔关节**惯量、联轴器刚度、摩擦**三类机械参数：借电机/负载两侧转矩-转速-位置的 90° 相位关系由积分量解算，较标准法惯量/刚度绝对百分比误差 3.55%–14.6%、摩擦 10.76%–19%；正好把本项目「谐振频率在线辨识 + 自适应陷波」支撑的 Ks/J 在线辨识做成无额外传感器方案。DOI:10.1007/s10846-025-02244-1 （链接：https://link.springer.com/article/10.1007/s10846-025-02244-1）
3. **He G., Zhou B., Zheng Y. 等** (华南理工大学) *Unraveling the Vibration Mechanism in Robotic Harmonic Drive: Coupled Dynamics Under Cyclic Inertial Loading and Installation Errors*, **Machines**, 2025, 13(12):1083. — 把**谐波减速器**化为含**齿轮啮合 + 薄壁轴承接触**的两类接触力学模型、建**平移-扭转耦合**动力学，揭示其振动几乎无啮合频率分量、由循环惯量载荷与安装误差激发的机理；正好把本项目「含齿隙/摩擦的非线性双惯量模型」扩到谐波减速器（柔轮弹性变形 → 非线性扭转刚度 + 齿隙死区），是多电机+柔轴场景三惯量化的前哨。DOI:10.3390/machines13121083 （链接：https://www.mdpi.com/2075-1702/13/12/1083）
4. **Qu X., Feng Z., Ju K. 等** *A compact variable stiffness joint for compliant robotics enabled by torsion-spring mean-diameter variation*, **Scientific Reports**, 2026. — 提 **TSDV 变刚度关节**：借**圆柱扭簧中径随转角变化**机制调刚度，含扭簧 + 内开槽套 + 滚珠导套，结构紧凑、被动/主动双模、刚度范围宽，偏转角 1.2 rad 时输出转矩 0–3.8→0–9.2 Nm，开槽套锁死内簧中径即转刚性关节；正好给本项目「柔性联轴器刚度分级/可调」一种「扭簧中径变异」紧凑新结构，机械组可按 ω_n 切档。DOI:10.1038/s41598-026-50967-4 （链接：https://www.nature.com/articles/s41598-026-50967-4）

### 💡 本批「可参考方向」
- **ADRC-NFTSMC 交叉耦合把 DCC+DOB 升为「离散 NLP + NFTSMC」滑模观测进阶（最贴创新点一）**：Daode Zhang 2025（Electronics 14(3):526）把 ADRC 与 NFTSMC 融合嵌进交叉耦合框架，提离散非线性跟踪微分器 + 离散非线性扩张状态观测器（DNLESO）估扰前馈、配优化趋近律 NFTSMC，双电机伺服突加非均匀负载下较纯 NFTSMC 响应 +18.9%、同步精度 +46.7%——正好把本项目「DCC+DOB」的一阶 DOB 升级为离散非线性 ESO + NFTSMC 滑模观测进阶结构，本科生可在 `control_dcc_dob.c` 旁加 DNLESO + NFTSMC 对照四策略看同步 RMS 与突加载恢复。（→ §五 候选点：电控·高阶滑模+扩张状态滑模观测（平均偏差耦合进阶））
- **双电机多目标预测同步 + 分数阶主动阻尼落到双惯量（模型预测同步 + 治 g/ω_n=0.30 双落地）**：Zhiqiang Wang 2025（ACEEE）把双电机位置/速度同步误差、跟踪误差与控制量统一为多目标预测同步、构动态耦合状态空间多步预测 + Lyapunov 稳定性证明；Wei Ai 2026（IEEE MESA，华南理工同组）把双惯量扩为**分数阶模型 + 奇异摄动**，在边界层谐振动力学上设计**分数阶主动阻尼（fractional-order active damping）**，较整数阶主动阻尼更抑负载侧谐振且保跟踪——两者合力把本项目「模型预测同步」落到双惯量对象、并把「经验定 g」升级为「据分数阶谐振特性系统化主动阻尼」直接治 `g/ω_n=0.30` 缺口，本科生可先在四策略外挂轻量 MPC 扭矩前馈、再把固定陷波换 Wei Ai 的分数阶主动阻尼扫 Ks=12/200/2000。（→ §五 候选点：电控·模型预测同步（无权重因子快速预测））；配套（→ §五 候选点：电控·自适应陷波/有源阻尼系统化设计（替代经验定 g））
- **环耦 + 加权平均增强 ADRC + 动态拓扑权重一致模糊 ADRC（状态均值偏差耦合拓扑 + 动态耦合增益双结构）**：Lixin Liu 2025（Electronics 14(3):477）三电机皮带输送用环耦同步、提增强一阶 ADRC + 加权平均增强环耦补偿器，较 PID 抗扰/精度/同步更优；CN120675458A 飞机悬挂提升双电机用**动态拓扑权重 + 一致性协议 + 模糊 ADRC** 按负载差动态分配权重做误差均衡——两者把本项目「状态均值偏差耦合拓扑（解耦同步与跟踪）」做成「环耦/一致性 + 加权/动态权重」可抄结构、并天然落地「动态耦合增益」候选，本科生可在 `control_dcc.c` 把固定耦合权重换动态拓扑权重 + 模糊 ADRC、对比四策略。（→ §五 候选点：电控·状态均值偏差耦合拓扑（解耦同步与跟踪））；配套（→ §五 候选点：电控·动态耦合增益）
- **分数阶双惯量建模与 LM 辨识 + 正弦跟踪 Ks/J/摩擦在线辨识（分数阶建模 + 治 g/ω_n=0.30 系统化整定双支撑）**：Wang X. 2025（Fractal Fract 9(4):222，华南理工）把双惯量扩为分数阶模型捕黏弹/记忆效应、用输出误差法 + LM 算法做分数阶辨识，正是 innovation.md §五「分数阶双惯量系统建模与参数辨识」点名的 Fractal Fract 2025 专刊锚点；Hafez 2025（J Intell Robot Syst）用正弦位置跟踪由电机/负载两侧相位关系解算惯量/联轴器刚度/摩擦、绝对误差 3.55%–19%——两者合力把本项目整数阶 Ks-Bs 升为分数阶、并把「谐振频率在线辨识 + 自适应陷波」的 Ks/J 在线辨识做成无额外传感器方案，直接支撑机械给 Ks/J、电控据 ω_n 自整定陷波治 `g/ω_n=0.30`。（→ §五 候选点：机械·分数阶双惯量系统建模与参数辨识）；配套（→ §五 候选点：机械·谐振频率在线辨识+自适应陷波）
- **谐波减速器非线性双惯量振动 + TSDV 扭簧中径变刚度关节（含齿隙摩擦非线性 + 刚度分级双落地）**：He G. 2025（Machines 13(12):1083，华南理工）把谐波减速器化为含齿轮啮合 + 薄壁轴承接触两类接触力学、建平移-扭转耦合动力学，揭示其振动由循环惯量载荷与安装误差激发、几乎无啮合频率分量，正好把本项目「含齿隙/摩擦的非线性双惯量模型」扩到谐波减速器（柔轮弹性变形 → 非线性扭转刚度 + 齿隙死区），是多电机+柔轴场景三惯量化的前哨；Qu X. 2026（Sci Rep）提 TSDV 变刚度关节、借扭簧中径随转角变异调刚度、被动/主动双模、偏转角 1.2 rad 转矩 0–3.8→0–9.2 Nm、锁死转刚性——给本项目「柔性联轴器刚度分级/可调」一种紧凑扭簧中径变异新结构，机械组可按 ω_n 切档设计分级联轴、电控侧据刚度档位联动调 g。（→ §五 候选点：机械·含齿隙/摩擦的非线性双惯量模型）；配套（→ §五 候选点：机械·柔性联轴器刚度分级/可调）；配套（→ §五 候选点：机械·三惯量扩展）

---

## 📅 2026-10-11（每日自动推送）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **(IEEE ICEPAE, 2026)** *Dynamic Shear Mapping Prescribed Performance Control for Multi-PMSM Speed Synchronization*. — 提**动态剪切映射预设性能控制**：构造复合误差同时表征参考跟踪与电机间同步，规避传统预设性能变换在归一化误差越界时失效；配**积分终端滑模控制器**，Lyapunov 证一致最终有界且瞬态越界后仍能恢复预设性能；三电机平台顺序非对称负载扰下协调速度恢复。正好把本项目四策略的同步误差瞬态包络做成可预设的进阶。DOI:10.1109/ICEPAE71371.2026.11688599 （链接：https://xplorestaging.ieee.org/document/11688599）
2. **王文大** *基于深度强化学习的多电机同步伺服系统自适应协同控制策略*. **信息记录材料**, 2026, 27(6):21–23. — 建多电机协同伺服状态空间、把同步误差与转速/位置作智能体观测，以 **DDPG（深度确定性策略梯度）** 为核心设计自适应协同控制器，实验证降最大同步误差、缩短同步时间。直接对应本项目「动态耦合增益」从定增益升为学习自适应。DOI:10.16009/j.issn.1009-5624.2026.06.007 （链接：https://cjournal.hep.com.cn/1009-5624/CN/1271789989554225752）
3. **Zhang Y., Qi H., Chen J., Du C., Ge S., Xie Y.** *Energy-aware optimization of electric vehicles' dual-motor coupled powertrain based on heterogeneous synchronous RL*, **Control Engineering Practice**, 2026, 168:106745. — 用**异构同步强化学习（HSRL）**同时选双电机耦合动力总成驱动模式与分配系数，跨域学习策略（离散域+连续域）优化能效；是多电机「耦合分配系数」学习自适应的现成工程样例。DOI:10.1016/j.conengprac.2025.106745
4. **Man T., Zhu H., Chen R., Yi C.** *Hierarchical DRL-Based Multi-motor Control with Torque Synchronization in Industrial IoT*, **WASA 2025 (LNCS 15688)**, 2025, pp. 11–21. — 提**分层 DRL（HDRL-NMC-TS）**：把采样周期与电压补偿解耦为两个 MDP 各自求解、联合最小化控制误差与能耗，工业 IoT 下保证多电机转矩同步；是本项目「网络化/多智能体同步 + 无模型学习」可抄基线。DOI:10.1007/978-981-96-8731-2_2

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **Vukojičić S., Ristić L., Kvaščev G.** *Industrial application of neural network-optimized model predictive control for a two mass resonant mechanical system*, **Electrical Engineering**, 2025, 107:14121–14136. — 双惯量谐振系统做**NN 辅助 MPC**：NN 估机械参数、Luenberger 观测器估轴转矩与负载转速，SIL 在 PLC 实时跑、外服跑**数字孪生（DT）**估参/调参；宽机械参数变化下稳速。正好把本项目「模型预测同步」落到双惯量对象并给「数字孪生/边缘增强」工业样板。DOI:10.1007/s00202-025-03261-x
2. **Yang Y., Gao P., Liu H., Xiang C.** *Design and Performance Test of a Compound Torsional Vibration Damper With Parallel Variable Stiffness*, **IEEE Sensors Journal**, 2026, 26(9):12959–12971. — 用**磁流变弹性体（MRE）**做**并联变刚度扭转减振器（PVSTD）**，吸收+隔离理论、半主动、频率可调，实验证全调谐范围减振优于被动；是本项目「柔性联轴器刚度分级/可调」的 MRE 半主动具体结构。DOI:10.1109/JSEN.2026.3673562
3. **Yan L., Gu J., Zheng Z., Liu S., Sun S., Peng F., Xu W.** *H-VSJ: A Hybrid Stiff-Compliant Variable Stiffness Joint With Decoupling Position and Stiffness Modulation*, **IEEE Robotics and Automation Letters**, 2026, 11(7):8944–8951. — 提**混合刚-柔变刚度关节（H-VSJ）**，解耦位姿与刚度调制、刚/柔模式平滑切换，柔态刚度 23.57–260.36 Nm/rad、刚态 8630.25 Nm/rad；是「刚度分级协同」机构解耦类新结构。DOI:10.1109/LRA.2026.3701570
4. **Lu W., Lu S., Zheng S., Song B.** *Load-side resonant suppression based on adaptive state feedback and decoupled sliding-mode observer for servo system*, **Trans. Inst. Meas. Control**, 2026, 48(11). — 双惯量伺服用**双二阶陷波（bi-quad notch）**补偿柔轴谐振，再提**自适应状态反馈**自由调负载侧阻尼抑负载侧振荡，并设计两个**解耦滑模观测器（DSMO）**同步辨负载转矩与负载惯量、解耦项消参数估计抖振；正好把本项目「经验定 g/陷波」升为「自适应陷波 + 在线辨 Ks/J + 解耦 SMO」系统化方案，直治 `g/ω_n=0.30` 缺口。DOI:10.1177/01423312251361587

### 💡 本批「可参考方向」
- **动态剪切映射预设性能 + 积分终端滑模把多PMSM同步误差瞬态包络做成可预设（最贴本项目四策略同步进阶）**：Liu J. 2026（IEEE ICEPAE）提动态剪切映射预设性能控制，构造复合误差同时表征参考跟踪与电机间同步、规避传统预设性能变换在归一化误差越界时失效，配积分终端滑模、Lyapunov 证一致最终有界且瞬态越界后仍能恢复预设性能，三电机平台顺序非对称负载扰下协调速度恢复——正好把本项目「主从/交叉耦合/偏差耦合/虚拟主轴」四策略的同步误差瞬态包络做成可预设的进阶，本科生可在 `control_*.c` 加一个动态剪切映射预设性能层、扫 Ks=12/200/2000 看越界恢复与同步 RMS。（→ §五 候选点：电控·高阶滑模+扩张状态滑模观测（平均偏差耦合进阶））
- **分层 DRL 网络化多电机转矩同步（无模型学习 + 多智能体方向）**：Man 2025（WASA 南航）提分层 DRL（HDRL-NMC-TS），把采样周期与电压补偿解耦为两个 MDP 各自求解、联合最小化控制误差与能耗，工业 IoT 下保证多电机转矩同步——正是本项目「网络化/多智能体同步 + 无模型学习」可抄结构，可作四电机之外分布式同步基线；本科生可把本项目四策略同步误差 csv 喂一个轻量 DRL agent 做「耦合增益/补偿量」在线学习概念验证。（→ §五 候选点：电控·无模型自适应预测+多智能体事件触发）
- **DDPG / 异构同步 RL 把「耦合增益/分配系数」从定增益升为学习自适应（动态耦合增益双落地）**：王文大 2026（信息记录材料）用 DDPG 建多电机协同伺服状态空间、以同步误差与转速/位置为观测、自适应学习协调机制，降最大同步误差与同步时间；Zhang 2026（CEP）用异构同步 RL 同时选双电机耦合动力总成驱动模式与分配系数、跨域学习策略优化能效——两者都把「耦合增益/分配系数」从定增益升为学习自适应，直接呼应创新点「动态耦合增益」，本科生可在 `control_deviation.c` 把固定耦合增益改为 DDPG/查表自适应增益、对比四策略。（→ §五 候选点：电控·动态耦合增益）
- **NN 辅助 MPC 落到双惯量 + 工业 SIL/数字孪生样板（模型预测同步 + 数字孪生同框）**：Vukojičić 2025（Electr Eng）把双惯量谐振系统做 NN 辅助 MPC，NN 估机械参数、Luenberger 观测器估轴转矩与负载转速，SIL 在 PLC 实时跑、外服跑数字孪生（DT）估参/调参，宽机械参数变化下稳速——正好把本项目「模型预测同步」落到双惯量对象、并给「数字孪生/边缘增强」提供工业 SIL+DT 样板，本科生可先在四策略外挂一个轻量 MPC 扭矩前馈、再看 DT 镜像参数漂移补偿。（→ §五 候选点：电控·模型预测同步（无权重因子快速预测））；配套（→ §五 候选点：电控·数字孪生/边缘计算实时增强）
- **变刚度柔性联轴器三连击 + 自适应陷波/解耦 SMO 谐振抑制（机械刚度分级 + 治 g/ω_n=0.30 系统化最贴本项目缺口）**：Yang 2026（IEEE Sensors J）用 MRE 磁流变弹性体做并联变刚度扭转减振器（PVSTD）、半主动频率可调、全调谐范围优于被动；Yan 2026（IEEE RA-L）提 H-VSJ 混合刚-柔变刚度关节、解耦位姿与刚度调制、柔态刚度 23.57–260.36 Nm/rad——两者给本项目「柔性联轴器刚度分级/可调」提供 MRE 半主动与机构解耦两类具体结构，机械组可按 ω_n 切档设计分级联轴；Lu 2026（Trans Inst Meas Control）在双惯量伺服用双二阶陷波补偿柔轴谐振、再提自适应状态反馈自由调负载侧阻尼抑负载侧振荡、并设计两个解耦滑模观测器（DSMO）同步辨负载转矩与负载惯量，正好把本项目「经验定 g（固定 DOB 带宽）/陷波」升为「自适应陷波 + 在线辨 Ks/J + 解耦 SMO」系统化方案，直治 `g/ω_n=0.30` 缺口，本科生可在 `control_dcc_dob.c` 把固定陷波换 Lu 的自适应状态反馈+DSMO、扫 Ks=12/200/2000 看谐振峰衰减与同步 RMS。（→ §五 候选点：机械·柔性联轴器刚度分级/可调）；配套（→ §五 候选点：电控·自适应陷波/有源阻尼系统化设计（替代经验定 g））；配套（→ §五 候选点：机械·谐振频率在线辨识+自适应陷波）

---

## 📅 2026-10-10（每日自动推送）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **(Springer, 2026)** *Data-driven adaptive synchronous control for multi-PMSM drive systems under severe asymmetrical load disturbances.* — 用**超局部模型（ultra-local model）**把机电磁耦合化为紧致数据驱动表示，配**超扭曲滑模区间观测器（STSMO interval observer）**实时估集总非对称负载扰并前馈补偿，再并入**自适应非奇异快速终端滑模（ANFTSMC）**降电磁转矩波动；六电机 TBM 试验台验证：较 PI-VACC 突加载瞬态力矩同步 RMSE 降 88.6%、稳态降 93.7%。直接对应本项目「齿隙 + 刚度退化 → 非对称负载」真实工况与 DCC+DOB 的滑模观测进阶。DOI:10.1007/s00202-026-03769-w
2. **Wang J., Liu Y., Wang B., Cai M.** *Recursive sliding mode control of dual-motor synchronous drive servo system based on disturbance observer*, **Trans. Inst. Meas. Control**, 2026 (OnlineFirst). — 为每台电机设计**独立有限时间扰动观测器（FTDO）**直接并入递归非奇异终端滑模（四滑面），精心设计的到达律抑抖振、并引入**同步反馈**保证两电机精确协调；Lyapunov 证有限时间稳定，双电机伺服仿真验证扰动抑制 / 跟踪 / 同步精度优于现有方法。DOI:10.1177/01423312261471936
3. **（发明专利 CN116560211B）** *基于数字孪生的多电机协同控制方法、装置及存储介质*. — 构建多电机协同场景**数字孪生模型**（虚拟电机 + 扰动观测模型 + 位置耦合模型），以改进**位置耦合模型**平衡虚拟 / 真实输出比重得同步误差并补偿，估内部 / 外部综合误差提升抗扰；把本项目「偏差耦合 / 位置同步」升为虚实映射闭环。链接：https://patents.google.com/patent/CN116560211B/en
4. **Arivamudhan M., Thiyagarajan R., Suganya M., Navaprakash N., Baskaran S.** *Digital Twin-Enabled Edge Intelligence Framework for Sustainable Traction Control and Energy Optimization in Autonomous Mobile Robots*, **2026 7th IEEE ICSSIT**, 2026. — **数字孪生 + 边缘智能**：四轮差速机器人实时数字镜像，轻量边缘神经网络调节差速电机扭矩分配 / 调速维持牵引，平均响应延迟 42 ms、滑移率降 24.6%、轨迹精度 +18.3%、能效 +16.8%；是多电机协同「数字孪生 / 边缘实时增强」可抄工程样例。DOI:10.1109/ICSSIT69151.2026.11656546

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **Hirata M., Nomura R., Suzuki M., Araya H., Igarashi Y.** *Final-state control for a system with load displacement-dependent stiffness in a two-inertia system*, **Mech. Eng. J. (J-STAGE)**, 2025, 12(3):24-00416. — 旋转 + 垂直运动执行器的扭转刚度随垂直位移**时变（位移相关刚度）**，用**时变最终状态控制（TVFSC）**免反复计算、无需试凑频率整形即可抑残余振动；实验证振动抑制媲美频域整形 FFSC 而计算更省。正好把本项目线性 Ks 扩为「随工况变化刚度」的非线性双惯量对象。链接：https://www.jstage.jst.go.jp/article/mej/12/3/12_24-00416/_article
2. **Marsal Vilanova P.** (Universitat Politècnica de Catalunya) *Model predictive joint torque control of mechanical transmissions modeled by Two-Inertia Systems*, **Master Thesis, UPC**, 2026. — 双惯量传动**MPC 关节转矩控制（TTC Predictive）**：双线程 1 ms、OSQP 求解器，内部模型扩为五状态 + **双观测器**估负载扰 / 电机非线性摩擦，事件式非线性前馈预补偿零速穿越静摩擦；实验较 IPIP 级联与 IP+ωs 更快 rise time、无静差、电流严格限幅。是把本项目「模型预测同步」落到双惯量对象的可抄基线。链接：https://www.recercat.cat/handle/2117/472962
3. **Zou J., Zhao K., Fan G., Shen K., Jia L.** (湖南工业大学) *Improved Terminal Sliding Mode Control for PMSM Dual-Inertia System Based on Dual Finite-Time Disturbance Observers*, **Progress In Electromagnetics Research C**, 2026, 173:256–265. — 针对柔轴耦合 + 负载扰致双惯量 PMSM 转速跟踪劣化与扭振，提**高阶非奇异快速积分终端滑模（HONFITSMC）** + **双有限时间扩张滑模扰动观测器（双 FTESMDO）**分别估电机侧 / 负载侧轴转矩与扰并前馈补偿；实验证抗扰与鲁棒显著提升。直接支撑本项目「双惯量 + 轴转矩观测 + 高阶滑模观测进阶」。DOI:10.2528/PIERC26073001
4. **（电气技术）** *基于 PLL 型扩张状态观测器的双惯量弹性伺服系统机械谐振抑制方法*. — 提**PLL-ESO**谐振抑制：相较常规 ESO 同极点配置下**更高带宽、更大相位裕度**，实验证较陷波 / 常规 ESO 谐振抑制更快更准，且**宽无阻尼固有频率范围有效**、输入参数摄动影响快速性与稳态精度；正好把本项目「PLL-ESO 谐振观测」候选落成双惯量直接验证。链接：http://dgjsxb.ces-transaction.com/CN/abstract/abstract9145.shtml

### 💡 本批「可参考方向」
- **数据驱动超局部 + 超扭曲区间观测落到多电机齿隙 / 刚度退化非对称负载（最贴本项目真实工况）**：Springer 2026 的 multi-PMSM 数据驱动框架用超局部模型把机电磁耦合化为紧致数据驱动表示、超扭曲滑模区间观测实时估集总非对称负载扰并前馈、自适应非奇异快速终端滑模（ANFTSMC）降电磁转矩波动，六电机 TBM 试验台突加载瞬态力矩同步 RMSE 降 88.6%、稳态降 93.7%——正好把本项目「齿隙 + 刚度退化 → 非对称负载」真实工况与 DCC+DOB 的滑模观测进阶一次落地，呼应创新点一 DOB 再进阶；本科生可在 `control_dcc_dob.c` 把固定一阶 DOB 换成超扭曲区间观测 + ANFTSMC 对照四策略看同步 RMS 与突加载恢复。（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））
- **递归非奇异终端滑模 + 独立有限时间 DOB + 同步反馈（双电机伺服可抄结构）**：Wang 2026（Trans Inst Meas Control）给每电机独立有限时间扰动观测器直接并入递归非奇异终端滑模（四滑面）、精心到达律抑抖振、引入同步反馈保两电机协调，Lyapunov 证有限时间稳定——正是本项目把一阶 DOB 升级为「有限时间 + 递归终端滑模」双电机同步现成结构，可作 `control_cross*.c` / `control_dcc.c` 的对照进阶；突加载下对比固定 vs 递归终端滑模的 RMS 与恢复时间。（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））
- **数字孪生 / 边缘计算把多电机协同升为虚实闭环（W13 后迭代期进阶）**：CN116560211B 的「数字孪生模型（虚拟电机 + 扰动观测模型 + 位置耦合模型）平衡虚拟 / 真实输出得同步误差并补偿」+ ICSSIT 2026 的「差速机器人实时数字镜像、边缘神经网络 42 ms 调差速扭矩、滑移率降 24.6% / 能效 +16.8%」共同把本项目「偏差耦合 / 位置同步」升为虚实映射闭环——正对应 §五「数字孪生 / 边缘计算实时增强」候选，可作 W13 之后迭代期把仿真内核搬到边缘 / 数字孪生验证的路线参照；本科生可先用本项目四策略同步误差 csv 喂一个数字孪生镜像做在线误差补偿概念验证。（→ §五 候选点：电控·数字孪生 / 边缘计算实时增强）
- **PLL-ESO 双惯量谐振抑制 + 双 FTESMDO 轴转矩观测（谐振观测进阶双落地）**：电气技术 PLL-ESO 文证 PLL-ESO 较常规 ESO 更高带宽 / 更大相位裕度、宽 ω_n 范围有效，正好把本项目「PLL-ESO 谐振观测」候选落成双惯量直接验证；Zou 2026（PIER C）的双 FTESMDO 分别估电机侧 / 负载侧轴转矩与扰前馈补偿，可替换本项目线性 Ks-Bs 为含轴转矩观测的双惯量对象——两者合力补创新点一 / 二，直接治当前 `g/ω_n=0.30` 缺口；本科生可在 `control_dcc_dob.c` 旁加 PLL-ESO 模块、扫 Ks=12/200/2000 三档对比 DOB-only vs DOB+PLL-ESO 的谐振峰衰减与同步 RMS。（→ §五 候选点：电控·PLL-ESO 谐振观测）；配套（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））
- **位移相关刚度 / 时变双惯量 + MPC 关节转矩控制（机械非线性建模 + 预测同步同框）**：Hirata 2025（J-STAGE）把双惯量扭转刚度随负载垂直位移时变、用时变最终状态控制（TVFSC）免试凑抑残余振动，正好把本项目线性 Ks 扩为「随工况变化刚度」的非线性双惯量对象（呼应创新点二「(Ks,g) 协同」里刚度不再固定）；UPC 2026 硕士论文把双惯量传动做实时 MPC 关节转矩控制（双线程 1 ms、五状态 + 双观测器估负载 / 非线性摩擦、事件前馈），直接把本项目「模型预测同步」候选落到双惯量对象——两者合力给机械组「非线性双惯量建模」与电控组「无权重因子快速预测」同框的可抄基线。（→ §五 候选点：机械·含齿隙 / 摩擦的非线性双惯量模型）；配套（→ §五 候选点：电控·模型预测同步（无权重因子快速预测））

---

## 📅 2026-10-09（每日自动推送）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **(IEEE Xplore, 2026)** *Fixed-Time Fractional-Order Sliding Mode Synchronization Control for Dual Motor.* — 在交叉耦合骨架上引入**固定时间扰动观测器（FTDO）** + **分数阶固定时间滑模控制器**：收敛时间独立于初值，分数阶微积分让同步比整数阶更快更准，DOB 固定时间估扰并前馈补偿；正是把本项目一阶 DOB 升级为「分数阶 + 固定时间」滑模观测进阶的现成基线。链接：https://ieeexplore.ieee.org/document/11415567
2. **(电机与控制应用 2026-02-07)** *基于扰动观测器的双电机同步驱动伺服系统滑模控制*. — 设计**有限时间扰动观测器**快速估外部扰（有限时间收敛）+ 构造**非奇异终端滑模（NTSMC）**避奇异 + 引入**同步反馈与优化到达律**抑抖振、保双电机高精度同步；直接对应本项目「偏差耦合拓扑解耦同步与跟踪」+「DOB 前馈」双内核。链接：https://www.motor-abc.cn/djykzyy/article/html/20260207
3. **(电子测量与仪器学报 2026)** *多永磁同步电机有限时间领导-跟随转速协同控制*. — 依赖无向拓扑建分布式**有限时间领导-跟随速度一致协议**，引入**超扭曲扩张状态观测器（STESO）**估不确定扰/未建模动态并前馈补偿出 q 轴电流，Lyapunov 证有限时间收敛且有上界；多 PMSM 实验证优于偏差耦合。链接：https://jemi.etmchina.com/jemi/article/html/B2508174
4. **Wang Y.-W., Zhang D.-F., Wu Q. 等** *A fully-actuated system approach based synchronization control of networked multi-motor motion systems*, **Control Engineering Practice**, 2025. — 把网络化多电机化为**全驱系统（FAS）模型**，将不确定 + 外扰 + **时变延迟**统为集总扰动，设计**等效输入扰动（IVEID）观测器**同时估状态与扰动并前馈补偿，获线性闭环高性同步；指向通信受限/网络化多电机同步方向。DOI:10.1016/j.conengprac.2025.003958

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **(IEEE, early access 2025)** *Design of a Dynamic Compensator for Load-side Speed and Torsional Torque Controls of a Two-inertia System.* — 提**动态速度补偿器（DSC）** + **轴转矩前馈补偿器（TTFC）**：基于扭转变形特征实时估**轴转矩**且**无需负载侧编码器/转矩传感器**，TTFC 抑制扭振引起的速度波动；双惯量实验证负载侧速度跟踪优良，是「轴转矩在线观测 + 双惯量」可抄工程样例。链接：https://ieeexplore.ieee.org/document/10920957
2. **Sheng Y., Wang G., Wang F., Li D., Hu M.** *Adaptive robust control for ball-screw drives with flexible transmission and nonlinear friction via dynamic surface control*, **ISA Transactions**, 2025. — 建含**扭转振动态**的双惯量模型 + 连续可微 **LuGre 摩擦**模型，用**动态面控制（DSC）+ 双观测器**实时观测补偿非线性摩擦，一阶滤波器消反步「复杂性爆炸」；仿真 + 实验证跟踪与抑振提升。链接：https://www.sciencedirect.com/science/article/pii/S0019057825002976
3. **Jiang R., Yin X., Kawata S., She J.** *Adaptive Horizon Model Predictive Control with Equivalent-Input-Disturbance for Vibration Suppression*, **J. Adv. Comput. Intell. Intell. Inform. (J-STAGE)**, 2026, 30(1):176–183. — 把**双惯量机械传动**振动抑制化为**自适应时域 MPC（AHMPC）** + **等效输入扰动（EID）**：自适应时域降算力、QP 解算、EID 前馈补外扰；双惯量仿真证显著抑振，呼应「双惯量 + 模型预测 + DOB」同构。DOI:10.20965/jaciii.2026.p0176
4. **(US Patent US20260009427A1, 2026)** *Torsional Vibration Control Coupling.* — 被动**非线性退化刚度联轴**：正刚度元件 + 负刚度元件组合成**退化（degressive）弹簧特性线**，在静态扭矩传递的同时实现宽频带扭转隔振、无需附加阻尼件；正好给本项目「柔性联轴器刚度分级/可调」一种「正负刚度耦合」的可量化结构。链接：https://patents.google.com/patent/US20260009427A1/

### 💡 本批「可参考方向」
- **固定时间分数阶滑模同步（DOB 再进阶的分数阶 + 固定时间版）**：IEEE 2026 的「交叉耦合 + 固定时间 DOB + 分数阶固定时间滑模」把本项目一阶 DOB 升级为「分数阶 + 固定时间收敛」的滑模观测进阶，收敛快、对初值不敏感，直接呼应创新点一 DOB 再进阶；本科生可在 `control_*` 旁加一个分数阶固定时间观测模块，对比四策略看同步 RMS 与恢复时间。（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））
- **非奇异终端滑模 + 有限时间 DOB + 同步反馈（解耦同步与跟踪落地）**：电机与控制应用 2026 的「NTSMC + 有限时间 DOB + 同步反馈/优化到达律」既把本项目「状态均值偏差耦合拓扑」做成可抄的双电机同步反馈结构，又把固定一阶 DOB 升为有限时间观测——本科生可在 `control_dcc.c` 把跟踪误差与同步误差合成综合误差再设计反馈，对比四策略是否同时改善跟踪与同步。（→ §五 候选点：电控·状态均值偏差耦合拓扑（解耦同步与跟踪））
- **超扭曲 ESO + 有限时间领导-跟随一致（高阶滑模观测 + 多智能体方向）**：电子测量与仪器学报 2026 的「无向拓扑有限时间领导-跟随 + STESO 估扰前馈」可替换本项目一阶 DOB 为超扭曲 ESO 进阶、且天然是多智能体一致框架——直接呼应创新点一观测器进阶，也可作四电机之外「网络化/分布式同步」的扩展基线。（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））
- **网络化多电机 FAS + IVEID 观测器（事件触发/网络化同步新基线）**：CEP 2025 把含时变延迟的网络化多电机化为全驱系统、用等效输入扰动观测器同时估状态与集总扰动前馈补偿——指向本项目「通信受限下多电机同步」方向，本科生可先在四策略外挂一个 EID 前馈层、注入网络延迟对照，反哺创新点一鲁棒性论证。（→ §五 候选点：电控·无模型自适应预测 + 多智能体事件触发）
- **机械侧三篇合力补「轴转矩观测 + 非线性摩擦 + 双惯量 MPC/EID + 正负刚度联轴」闭环（最贴机械组候选）**：IEEE 2025 的 DSC+TTFC 实时估轴转矩（无需负载侧传感器）、ISA Trans 2025 的 LuGre 双观测器摩擦补偿、J-STAGE 2026 的 AHMPC+EID 双惯量抑振三者把「双惯量对象 + 轴转矩/摩擦观测 + 模型预测抑振」落成闭环，直接支撑本项目「含齿隙/摩擦非线性双惯量」建模与「机械谐振在线辨识 + 自适应陷波」（把 TTFC/EID 观测量接入 notch/有源阻尼）；US20260009427A1 的「正 + 负刚度退化特性联轴」则给「柔性联轴器刚度分级/可调」一种免附加阻尼件的宽频隔振结构——机械组可据其退化刚度线设计分级联轴、电控侧按 ω_n 切档扫参。（→ §五 候选点：机械·含齿隙/摩擦的非线性双惯量模型）；配套（→ §五 候选点：机械·柔性联轴器刚度分级 / 可调）；配套（→ §五 候选点：机械·谐振频率在线辨识 + 自适应陷波）

---

## 📅 2026-10-08（每日自动推送）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **(IRJMETS, 2026)** *Adaptive RBF Neural Network-Based Fixed-Time Super-Twisting Sliding Mode Output-Feedback Control for Elastic Multi-Motor Drive Systems.* — 直接以**弹性多电机驱动系统**（含非线性动力学、弹性耦合、参数不确定、非线性摩擦、外扰）为对象，高增益观测器重建不可测状态 + **RBFNN 在线逼近集总不确定性** + **固定时间超扭曲滑模**输出反馈；四工况（标称/突加载/连续外扰/参数+非线性摩擦）较 PID 超调更小、整定更快、鲁棒更强。链接：https://www.irjmets.com/paperdetail.php?paperId=558eb1def3425b91eaa0739b1e2f665e
2. **Chen Y., Wang P., Yu W.** *Research on Synchronous Control of Dual-PMSM Systems Based on Super-Twisting Sliding Mode Control*, **2025 IEEE 15th Int. Conf. on Power Electronics and Drive Systems (PEDS)**, 2025. — 把**幂律增强 NFTSM** 嵌进**交叉耦合**框架 + 改进**无差拍预测电流控制（DPCC）**配**超扭曲滑模观测器（STSMO）**消除一步延迟与抖振；双 PMSM 抗扰/跟踪/同步一致性较 PI 显著提升。DOI:10.1109/PEDS63958.2025.11144881
3. **Zhao Y., Guo L., Zhang J., Zhou Y., Lu W.** (浙江理工大学) *Repetitive Fractional-Order Active Disturbance Rejection Control for Permanent Magnet Synchronous Motor*, **Machines**, 2026, 14(1):70. — 把**重复控制**与**分数阶自抗扰（FO-ADRC）**级联，并用分数阶控制替换 NLSEF、改进平滑函数引入 ESO，抑周期性扰 + 降抖振；1.5 kW 平台突加载恢复 0.22 s、稳态速差 ±2.27 r/min。DOI:10.3390/machines14010070
4. **Liu J., Yang Z., Xu X., Zhou T., Zhou Y.** (江苏大学) *Research on Fault-Tolerant Synchronous Control of Dual Motors for Wire-Controlled Steering Based on Average Deviation Coupled Fuzzy PID*, **Machines**, 2026, 14(5):495. — 用**模糊 PID 均值偏差耦合（mean-deviation coupling）**做双电机转矩同步、超螺旋滑模电流环经自适应差分进化优化；较非同步基线电机间电流失配降 8.1–78.6%、故障后稳定时间较 PID/STSM/H∞ 缩短 50–70%/9–20%/16.7%。DOI:10.3390/machines14050495

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **Görgülü İ., Dede M.İ.C., Kiper G.** *A new safe flexible torsion joint design with softening stiffness characteristics*, **Mechanism and Machine Theory**, 2025, 210:106015. — 被动**变刚度扭关节**：双向挠度 + 弹簧并/串联、按负载分**多刚度区（刚性/硬/软）**、可串接多单元，静态/动态实验吻合理想模型，直接给本项目「柔性联轴器刚度分级/可调」可复现结构。DOI:10.1016/j.mechmachtheory.2025.106015
2. **(Preprints, 2026)** *Variable-Stiffness Targeted Energy Transfer for Wide Range Torsional Vibration Mitigation.* — 用**气动柔性联轴器**实现**变刚度靶向能量传递（TET）**，宽频带抑制扭转振动，正好呼应本项目「变刚度柔性联轴器 + 谐振抑制」双主题。链接：https://www.preprints.org/manuscript/202606.1604
3. **Yang Y., Liu H., Gao P., Li Z., Xiang C.** *An integrated isolator-absorber with quasi-zero stiffness for vibration isolation and semi-actively controlled variable-stiffness absorption*, **Proc. IMechE (Sage, OnlineFirst 2026)**. — **准零刚度隔离（QZSI）** + **半主动变刚度吸收（VSA）**，STFT 频率跟踪做半主动控制，HEV 传动扭振案例验证低频隔振与显著抑振，直接对标车辆传动双惯量扭振。DOI:10.1177/09544070261450713
4. **Wen K., Wu G.** (大连理工大学) *Design and Analysis of a Serial Position-Controlled Variable Stiffness Rotating Mechanism Based on Multi-Stage Torsional Compliant Mechanisms*, **Actuators**, 2025, 14(5):236. — **三级柔顺扭转机构串接**，按角位置调刚度、**最大/最小刚度比约 9 倍**，SolidWorks–Ansys 协同优化 + 实验验证，给「刚度分级/可调联轴器」量化结构方案。DOI:10.3390/act14050236

### 💡 本批「可参考方向」
- **弹性多电机 RBFNN 固定时间超扭曲（把 DCC+DOB 扩到含弹性耦合/摩擦的实物级对象）**：IRJMETS 2026 直接以「弹性多电机驱动系统」为对象，高增益观测器重建不可测状态 + RBFNN 在线逼近集总非线性/摩擦/外扰 + 固定时间超扭曲滑模输出反馈，四工况（含突加载、连续外扰、参数+非线性摩擦）均较 PID 超调更小、整定更快、更强鲁棒——正是本项目把「线性双惯量 Ks-Bs」升级为「含非线性摩擦的弹性多电机」并配套固定时间超扭曲控制的现成基线，呼应创新点一 DOB 再进阶与机械组非线性建模。（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））；配套（→ §五 候选点：机械·含齿隙/摩擦的非线性双惯量模型）
- **交叉耦合 + 超扭曲滑模观测（STSMO）+ DPCC（把 DCC+DOB 的观测器换成超扭曲版）**：Chen 2025（PEDS）把幂律增强 NFTSM 嵌进交叉耦合框架、配改进无差拍预测电流（DPCC）+ 超扭曲滑模观测器（STSMO）消一步延迟与抖振，双 PMSM 抗扰/跟踪/同步一致性较 PI 显著提升——正好把本项目一阶 DOB 升级为「超扭曲滑模观测 + 预测电流」进阶结构，且同属交叉耦合架构、与本项目四策略之一同构，本科生可在 `control_cross*.c` 旁加 STSMO 对照。（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））
- **均值偏差耦合 + 模糊 PID（状态均值偏差耦合拓扑 + 动态耦合增益双落地）**：Liu 2026（Machines 14(5)）用模糊 PID 均值偏差耦合（mean-deviation coupling）做双电机转矩同步、超螺旋滑模电流环经自适应差分进化优化，故障后稳定时间较 PID/STSM/H∞ 缩短约 50–70%/9–20%/16.7%——既把本项目「状态均值偏差耦合拓扑（解耦同步与跟踪）」做成可抄的平均偏差耦合 + 模糊 PID 结构，又把固定耦合增益换成随工况自适应的模糊 PID（动态耦合增益候选）；本科生可在 `control_dcc.c` 把固定权重替换为模糊 PID 均值补偿，对比突加载下固定 vs 模糊动态增益的 RMS 与恢复时间。（→ §五 候选点：电控·状态均值偏差耦合拓扑（解耦同步与跟踪））；配套（→ §五 候选点：电控·动态耦合增益）
- **分数阶重复自抗扰（RFO-ADRC）给 DCC+DOB 的分数阶 ESO 进阶**：Zhao 2026（Machines 14(1)）把重复控制与分数阶 ADRC 级联、用分数阶控制替换 NLSEF 并把改进平滑函数引入 ESO，抑周期性扰 + 降抖振，1.5 kW 平台突加载恢复 0.22 s、稳态速差 ±2.27 r/min——把本项目一阶 DOB/ESO 升级为「分数阶 + 重复补偿」的 ESO 进阶，且分数阶 ESO 与机械组「分数阶双惯量建模」相互呼应，可作四策略外的对照观测器。（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））
- **机械侧四篇合力把「刚度分级/可调柔性联轴器 + 谐振抑制」落成闭环（最贴合机械组候选）**：Görgülü 2025（Mech Mach Theory）的被动变刚度扭关节（刚性/硬/软多区、可串接）、Wen 2025（Actuators）的三级柔顺扭转机构（最大/最小刚度比 9 倍、按角位置调刚度）、Preprints 2026 的气动柔性联轴器变刚度靶向能量传递、Yang 2026（Sage QZS+半主动变刚度吸收、STFT 频率跟踪）四者共同给出「柔性联轴器刚度分级/可调」的可量化结构与控制方案——正好对接本项目机械组「刚度分级协同寻优」候选，本科生可把固定 Ks=200 换成「分级/可调刚度曲线」跑三档对比，看同步 RMS 与谐振峰随刚度档位的变化；其中 QZS+半主动变刚度 + 频率跟踪还能给「据 ω_n 自适应整定陷波」提供机械侧的变刚度搭档。（→ §五 候选点：机械·刚度分级协同）；配套（→ §五 候选点：机械·谐振频率在线辨识 + 自适应陷波）

---

## 📅 2026-10-07（每日自动推送）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **Pan Y., Jiang W.** *Adaptive Fusion Sliding Mode Control in Model Predictive Synchronization Control for Multi-Induction Motor*, **IET Electric Power Applications**, 2026, 20(1). — 把**平均偏差耦合控制（ADCC）**与模型预测转矩控制（MPTC）融合成 MPSC 框架，设计**自适应滑模观测器（ASMO）**实时估负载转矩，再用**自适应融合滑模控制器（AFSMC）**协同优化跟踪与同步，IM 参数失配下鲁棒性优于传统偏差耦合。DOI:10.1049/elp2.70206
2. **Wu J.-M., Huang X., Liu C.-L.** *A new predefined-time fractional-order sliding mode synchronization control scheme for multi-motor systems*, **Communications in Nonlinear Science and Numerical Simulation**, 2026, 152:109307. — 提出**预设时间分数阶滑模**同步：简化预设时间稳定条件 + **分数阶滑模观测器**估扰，分数阶滑面与切换律兼顾灵活性与收敛速度，较整数阶更快更抗扰。DOI:10.1016/j.cnsns.2025.109307
3. **(电工技术学报 2026)** *基于预设性能控制的多永磁同步电机速度一致性控制*. — 把多 PMSM 化为**多智能体一致性**问题，设计**预设性能控制（PPC）**速度一致协议约束暂/稳态过程，引入**超扭曲扩张状态观测器（STESO）**估扰并实时补偿；综合实验平台验证稳速误差约 0.5 r/min（相对耦合控制 RCC 约 5 r/min），同步误差显著下降。DOI:10.19595/j.cnki.1000-6753.tces.242219
4. **Wu Y., Ping Z., Huang Y.** *Distributed internal model control for speed synchronization of multiple PMSMs in urban rail traction systems*, **Control Engineering Practice**, 2026, 172:106923. — 把多 PMSM 速度同步化为**协同鲁棒输出调节（CROR）**问题，构造分布式**内模控制（IMC）**在**有向通信 + 参数不确定**下实现速度一致，仿真与实验验证有效。DOI:10.1016/j.conengprac.2026.106923

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **Wróbel K., Śleszyński K.K., Majdański P.** *Multi-Layer Nonlinear Extended Kalman Filters for Two-Mass Drive System Parameters Identification*, **Energies**, 2026, 19(1):28. — 用**多层非线性扩展卡尔曼滤波（multi-layer EKF）**在线辨识双惯量（两质量）传动系统的负载惯量、轴刚度等参数，较经典 EKF 估计更准、在线辨识更快，dSPACE 1103 实验验证；直接给本项目「据 ω_n 在线辨识机械参数」提供观测器底座。DOI:10.3390/en19010028
2. **Niu F., Chao X., Zhang Y., Chen J., Li S.** *MEKF-Tuned Full-State Feedback for Two-Mass Resonance Suppression*, **2025 IEEE 12th Int. Symp. on Sensorless Control for Electrical Drives (SLED)**, 2025. — 提出**多层 EKF（MEKF）**并行双架构：实时辨负载惯量 + 高精度估负载转速/轴转矩/扰动力矩，按**最优惯量比**动态配置全状态反馈增益保阻尼，克服单 EKF 收敛慢与观测失配，实现动态谐振抑制（河北工业大学）。链接：https://www.aminer.cn/pub/68c4ac2d163c01c8507e96a9/mekf-tuned-full-state-feedback-for-two-mass-resonance-suppression
3. **(学位论文 2025)** *永磁同步伺服系统的机械谐振抑制研究*. — 建双惯量模型后，提出**锁相环型扩张状态观测器（PLL-ESO / PLESO）**谐振抑制，并进一步给出**可变带宽 PLESO（VB-PLESO）**与饱和跟踪微分器（Sat-TD）的位置复合控制，验证 PLESO 在宽 ω_n 范围有效、VB-PLESO 提升变参数下快速性与稳态精度（吴春 2024 期刊的学位论文级扩展）。链接：https://search.napstic.cn/literature/degree/0620250700435138
4. **Kimura I., Padron J., Miyazaki T., Ohishi K. et al.** *Force Control of a Two-Inertia System Using Torsion Torque Observer-Based Quantization Error Compensation with a Low-Resolution Torque Sensor*, **2026 IEEE 19th Int. Conf. on Advanced Motion Control (AMC)**, 2026, pp.1–6. — 针对双惯量系统力控中低分辨率转矩传感器的量化误差，设计**扭转转矩观测器（TTOB）**估轴转矩并补偿量化误差，较未补偿显著提升力控性能，是「轴转矩在线观测 + 双惯量」的可抄工程样例。链接：https://scholar.google.co.uk/citations?citation_for_view=pIVib9IAAAAJ%3AC-Rn0OCouf8C

### 💡 本批「可参考方向」
- **平均偏差耦合 + MPTC + ASMO 融合（状态均值偏差耦合新落地 + 预测同步同框）**：Pan 2026（IET EPA）把 ADCC（平均偏差耦合）与 MPTC 融成 MPSC 框架、ASMO 实时估负载转矩、AFSMC 协同跟踪与同步——既把本项目「状态均值偏差耦合拓扑」做成可抄的平均偏差耦合 + MPC 结构，又把「模型预测同步」候选落进同一篇（预测转矩控制直接做速度同步）；本科生可在 `control_dcc.c` 旁加一个 ADCC+MPTC 协调层，对比四策略看同步与跟踪是否同框改善。（→ §五 候选点：电控·状态均值偏差耦合拓扑（解耦同步与跟踪））
- **预设时间分数阶滑模 + 分数阶滑模观测（DOB 再进阶的分数阶版）**：Wu J.-M. 2026（CNSNS）的预设时间分数阶滑模同步 + 分数阶滑模观测器估扰，把本项目 DCC+DOB 的一阶 DOB 升级为「分数阶 + 预设时间收敛」的滑模观测进阶，收敛更快、对初值/参数更不敏感，直接呼应创新点一 DOB 再进阶。（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））
- **预设性能 + 超扭曲 ESO 的多智能体一致（解耦同步与跟踪 + 观测进阶双重）**：电工技术学报 2026 把多 PMSM 化为多智能体、PPC 约束暂/稳态过程 + STESO 估扰补偿，稳速误差较 RCC 降约 10 倍（0.5 vs 5 r/min）——可作本项目「状态均值偏差耦合拓扑」的多智能体一致版，且 STESO 呼应创新点一观测器进阶；本科生可在四策略外挂一个 STESO 前馈对照。（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））
- **多层 EKF/MEKF 在线辨识 Ks/J → 自适应陷波（最优先治 g/ω_n=0.30）**：Wróbel 2026（Energies）与 Niu 2025（IEEE SLED）都用多层 EKF 在线辨识双惯量系统的负载惯量、轴刚度等机械参数，较单 EKF 更快更准、dSPACE 实验验证——正好给本项目「据 ω_n 自适应整定陷波」提供机械参数在线来源，直接接进 `sync` 脚本算出的 ω_n，把固定 g=100 换成「在线辨识 Ks/J → 实时整定陷波/有源阻尼」，优先治当前 `g/ω_n=0.30` 欠阻尼缺口。（→ §五 候选点：机械·谐振频率在线辨识 + 自适应陷波）
- **VB-PLESO（可变带宽 PLL-ESO）+ TTOB 轴转矩观测（谐振观测进阶 + 双惯量非线性）**：2025 学位论文把吴春 2024 的 PLL-ESO 扩成**可变带宽 PLESO（VB-PLESO）**，在变参数下仍保快速性与精度，正好把本项目「PLL-ESO 谐振观测」升级为随工况自调带宽；Kimura 2026（IEEE AMC）的扭转转矩观测器（TTOB）估轴转矩补偿低分辨率传感器量化误差，可替换本项目线性 Ks-Bs 为含轴转矩观测的双惯量对象，使力/位置控更贴真实。两者合力补创新点一/二。（→ §五 候选点：电控·PLL-ESO 谐振观测）；配套（→ §五 候选点：机械·含齿隙/摩擦的非线性双惯量模型）

---

## 📅 2026-10-06（每日自动推送）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **Xu Y., Liu C., Huang Z., Sun S., Cui Z.** *Research on Coordinated Control of Multi-PMSM for Shaftless Overprinting System*, **Symmetry**, 2025, 17(6):958–979. — 三电机轴less印刷系统，提出**加权平均优化偏差耦合补偿器（NDCS）** + 新型到达律滑模（NSMC）+ 改进超扭曲电流环，动态响应快 38.7%、稳态转速偏差压至 0.2 r/min、峰偏差 ≤8 r/min。DOI:10.3390/sym17060958
2. **Zhu Z.** *MAS-Based Cooperative Control of Multi-PMSM Systems*, **2025 5th Int. Conf. on Mechanical, Electronics and Electrical and Automation Control (METMS 2025)**, 2025, pp.162–165. — 把多 PMSM 化为**多智能体一致性**问题，设计分布式滑模协议（常速到达律 + 线性滑面、邻域同步误差渐近收敛）+ **滑模扰动观测器**前馈补偿负载扰动；相较传统相对耦合（RCC）在同步误差、动态响应与可扩展性上均占优。DOI:10.1109/METMS65303.2025.11047911
3. **(IEEE 2025)** *Synchronous control of multiple motors based on deviation coupling*. — 在偏差耦合骨架上引入**极值（最大/最小）模块动态调节电机间耦合关系**，突加载时实时优化速度补偿、缩短动态响应；三 PMSM 仿真证同步精度与抗扰显著提升。DOI:10.1109/11041975（IEEE Xplore 文档 11041975）
4. **Wu J., Huang X., Liu C.** *Extended state observer based adaptive fuzzy sliding mode control for multi-motor systems*, **Physica Scripta**, 2025, 100(1):015203. — 用多电机**耦合误差**设计非奇异滑模，以**扩张状态观测器（ESO）**估集总扰动、用**自适应模糊**替代不连续切换项抑抖振，固定时间收敛到小邻域；对比实验控制精度与响应速度优于传统控制器。DOI:10.1088/1402-4896/ad94ad

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **Wang B., Ji R., Zhou C., Liu K., Hua W., Ye H.** *Online Identification Method for Mechanical Parameters of Dual-Inertia Servo System*, **Energies**, 2025, 18(1):79. — 基于**遗忘因子递推最小二乘（FFRLS）**在线同时辨识双惯量系统的**转子惯量、负载惯量、轴刚度**（误差 <10%），DSP 28335 平台验证，为"据 ω_n 自适应整定陷波"提供机械参数在线来源。DOI:10.3390/en18010079
2. **He Y., Zhao K., Yi Z., Huang Y.** *Improved Terminal Sliding Mode Control of PMSM Dual-Inertia System with Acceleration Feedback Based on Finite-Time ESO*, **Progress In Electromagnetics Research M**, 2025, 134:21–30. — 速度环引入**加速度反馈**建立双惯量模型，改进到达律使收敛速度自适应调整，设计**有限时间扩张状态观测器（FTESO）**估扰并前馈补偿 INTSMC，仿真与实验证抗扰能力显著提升。DOI:10.2528/PIERM25040405
3. **（制造技术与机床 2026）** *中大型直驱转台双惯量机电耦合建模*. — 建**三惯量运动链**（电机转子 J₁ / 连接套 J₂ / 工作台 J₃）并降阶为双惯量，给出**静态刚度 Ks,stat 与动态刚度 Ks,dyn 两种等效准则**（第一扭振 672.8 Hz vs 降阶 773.6 Hz），是"三惯量扩展 + 刚度分级"的可复现建模。链接：https://1951.mtmt.com.cn/article/id/a4348c05-50c4-4651-a226-3bfa39599970
4. **（发明专利 CN120415192A）** *基于模型预测控制的双惯量伺服系统齿隙振荡抑制方法*. — 建平滑**齿隙死区模型**（轴转矩—角度差关系）+ **位置误差齿隙观测器**估齿隙等级，按不同等级构造 MPC 预测模型与代价函数主动抑振；是"含齿隙非线性双惯量"直接可抄的工程样例。链接：https://patents.google.com/patent/CN120415192A/en

### 💡 本批「可参考方向」
- **加权平均偏差耦合补偿器（状态均值偏差耦合拓扑新实现）**：Xu 2025（Symmetry）的 NDCS 用加权平均法优化偏差耦合补偿器，把"各电机速度 vs 系统均值速度"做成加权补偿、配 NSMC + 改进超扭曲电流环——正是本项目"状态均值偏差耦合拓扑（解耦同步与跟踪）"的可抄结构；本科生可在 `control_*` 里把"跟踪误差 + 偏差耦合项"合成加权均值补偿，对比四策略看是否同时改善跟踪与同步。（→ §五 候选点：电控·状态均值偏差耦合拓扑（解耦同步与跟踪））
- **极值模块动态调耦合（动态耦合增益现成落地）**：IEEE 2025 的"偏差耦合 + 极值(max/min)模块动态调节电机间耦合关系"在突加载时自动切换耦合补偿，正是本项目"动态耦合增益"候选的可抄实现；直接把固定权重换成随偏差极值自适应的开关式增量公式搬进 `control_dcc.c`，对比突加载下固定 vs 极值自适应增益的 RMS 与恢复时间。（→ §五 候选点：电控·动态耦合增益）
- **多智能体滑模一致 + ESO/模糊滑模观测（DOB 再进阶基线）**：Zhu 2025（METMS）把多 PMSM 化为多智能体、分布式滑模一致 + 滑模扰动观测器前馈，Wu 2025（Physica Scripta）用 ESO + 自适应模糊滑模固定时间收敛并抑抖振，He 2025（PIER M）在双惯量对象上用加速度反馈 + FTESO 进阶——三者把本项目一阶 DOB 升级为"多智能体滑模一致 + 扩张状态滑模观测"进阶基线，且 Zhu 的多智能体框架天然是"事件触发低通信同步"的前置结构。（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））
- **FFRLS 在线辨识 ω_n 与机械参数 → 自适应陷波（最优先治 g/ω_n=0.30）**：Wang B. 2025（Energies）用遗忘因子递推最小二乘在线同时辨识转子惯量、负载惯量、轴刚度（误差<10%），正好给本项目"据 ω_n 自适应整定陷波"提供机械参数在线来源——直接接进 `sync` 脚本算出的 ω_n，把固定 g=100 换成"在线辨识 Ks/J → 实时整定陷波/有源阻尼"，优先治当前 `g/ω_n=0.30` 欠阻尼缺口。（→ §五 候选点：电控·自适应陷波 / 有源阻尼系统化设计（替代经验定 g））
- **含齿隙非线性双惯量 + 三惯量降阶（让仿真更贴真实）**：CN120415192A 的"位置误差齿隙观测器 + MPC 分档抑振"给出含齿隙双惯量的可复现建模，制造技术与机床 2026 的"三惯量链降阶 + 静/动刚度准则"给出把双惯量扩成"电机—连接套—工作台"三惯量并量化刚度等效的方法——两者替换本项目线性 Ks-Bs 为含齿隙/多惯量对象，使换向冲击与刚度匹配场景更真，反哺创新点一/二。（→ §五 候选点：机械·含齿隙/摩擦的非线性双惯量模型）；配套（→ §五 候选点：机械·三惯量扩展）

---

## 📅 2026-10-05（每日自动推送）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **Sun H., Zhao W., Wang C. et al.** (南京航空航天大学) *Redundant torque synchronization and steering angle tracking strategy for dual three-phase steer-by-wire system*, **Control Engineering Practice**, 2026. — 双三相 PMSM（DTP-PMSM）线控转向系统，提出**均值偏差耦合（mean deviation coupling）+ 小波神经网络补偿器**动态优化偏差耦合结构增益：常态平滑死区包容相位滞后、强扰/故障瞬时激发高协同刚度，消除三冗余电机间转矩异步；实验证快速转矩同步与转向角跟踪提升。DOI:10.1016/j.conengprac.2026.00286
2. **(Vietnam Aviation Academy)** *Distributed Backstepping Integral Terminal Sliding Mode Control for Consensus Speed Tracking in Networked PMSM Systems*, **Energies**, 2026, 19(18):4438. — 领导-跟随图 + **反向步进积分终端滑模（BITSM）** 一致速度跟踪：q 轴电流环用积分终端滑面、速度环受图加权界约束，有限时间达面、输入-状态稳定（ISS）；四 agent 仿真峰值瞬态转速差降约 83%、稳态 agent 间差降 >90%。DOI:10.3390/en19184438
3. **(机床与液压 2026)** *螺杆空压机双PMSM改进交叉耦合控制*. — 改进 ADRC + **并行扩张状态观测器（并行 ESO）** + **共模/差模信号分离** + 改进交叉耦合：把外部扰动拆成影响速度跟踪的共模扰动与产生位置偏差的差模扰动，专以改进交叉耦合补偿差模减小两螺杆位置偏差（保障位置偏差 <0.01 rad），是「解耦同步与跟踪」的工程样例。链接：http://www.jcyyy.com.cn/jcyyy/article/abstract/202615011
4. **(Scientific Reports, Nature 2026)** *Incremental model predictive control of PMSM based on parameter tuning of multi-layer perceptron neural network and disturbance observer*, **Scientific Reports**, 2026. — 提出 **DOB-增量 MPC（DOB-IMPC）**：增量预测模型 + DOB 实时估集总扰动前馈 + **多层感知机（MLP）神经网络在线整定** 关键控制参数；较人工整定 MPC 速度 RMSE 降 46.2%、突加载最大波动再降 31.2%、参数摄动下 RMSE 降 75.8%。DOI:10.1038/s41598-026-55885-z

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **Zhang G., Chen C., Zhan Y.** (湖北工业大学) *Vibration Suppression and Notch Filter Design for Flexible Drive System*, **西北工业大学学报**, 2024 (online 2024). — 双惯量柔传系统，引入**参数平面法（parameter plane method）** 解析设计 PI + 陷波器参数（突破传统「同实部/同半径代表极点」经验整定），可据惯量比系统化求阻尼、配陷波中心频率；证实传统陷波有效提升系统稳定性。DOI:10.13433/j.cnki.1003-8728.20240028
2. **Xie J., Wang X., Zhang J. et al.** *Research on extended high dimensional Melnikov and singularity based on double inertial coupling vibration system*, **Chaos, Solitons & Fractals**, 2026, 209(P1). — 建含多非线性与**分数阶微分项**的双惯量耦合振动 2-DOF 模型，用**扩展高维 Melnikov 法**导出混沌运动临界判据、结合奇异性理论定稳定/不稳定区边界与多稳态路径依赖；揭示分数阶/非线性参数对动力学演化的调控机理，为双惯量系统混沌抑制与参数优化提供理论支撑。DOI:10.1016/j.chaos.2026.118414
3. **(IEEE 2026)** *Load Position Tracking Control of Two-Inertia Systems via Speed-Predictive Functional Control with Robust Disturbance Compensation*. — 双惯量（TTS）负载位置控制，提出**非线性扰动观测器（NDOB）** 估集总不确定 + **速度预测功能控制（predictive functional control）** 设计速度环，PMSM 实验证负载位置跟踪与抗扰优良；是把「预测控制 + DOB」直接落到双惯量对象的现成基线。链接：https://ieeexplore.ieee.org/document/11317680
4. **(SpecForge 2026, 行业技术综述)** *AC Servo Motor and Drive Pairing: Inertia Matching Reconsidered for 2026*. — 现代伺服驱动器用**自动整定识别双惯量谐振并放置陷波**，惯量比从「1:1 硬指标」转为灵敏度参数；明确指出「谐振频率必须落在驱动器可陷波范围内」的选型逻辑与 Ks/惯量比→带宽天花板关系。链接：https://www.sourcebyspec.com/news/ac-servo-motor-and-drive-pairing-inertia-matching-reconsidered-for-2026.html

### 💡 本批「可参考方向」
- **小波神经网络动态耦合增益（最贴合本项目偏差耦合升级）**：Sun 2026（Control Engineering Practice）的「均值偏差耦合 + 小波神经网络补偿器」动态优化结构增益——常态低协同刚度包容相位滞后、强扰瞬时高协同刚度，正是把本项目固定偏差耦合权重升级为随工况自适应的**动态耦合增益**现成实现；直接把固定权重换成随同步误差自适应的增量公式搬进 `control_dcc.c`，对比突加载下固定 vs 小波NN动态增益的 RMS 与恢复时间。（→ §五 候选点：电控·动态耦合增益）
- **共模/差模分离的偏差耦合（解耦同步与跟踪）**：机床与液压 2026 螺杆空压机文把外部扰动拆成「影响速度跟踪的共模扰动」与「产生位置偏差的差模扰动」、专以改进交叉耦合补偿差模，正好对应 `innovation.md`「状态均值偏差耦合拓扑」的「解耦同步与跟踪」内核——本科生可在 `control_*` 里把跟踪误差与同步误差分别经共模/差模通道处理，对比四策略看是否同时改善跟踪与同步。（→ §五 候选点：电控·状态均值偏差耦合拓扑（解耦同步与跟踪））
- **预测功能控制 + DOB 落到双惯量（无权重因子快速预测可算版）**：IEEE 2026 双惯量负载位置跟踪文用「非线性 DOB 前馈 + 速度预测功能控制」直接在双惯量对象上做高精度负载位置跟踪，配 Scientific Reports 2026 的 DOB-IMPC（MLP 在线整定、突加载波动降 31.2%）——二者把本项目「模型预测同步」候选从「权重整定难」痛点里解放，且都自带 DOB 前馈、与 DCC+DOB 同构，可先在四电机仿真用虚拟均值电机当协调层试跑。（→ §五 候选点：电控·模型预测同步（无权重因子快速预测））
- **参数平面法系统化整定陷波（最优先治 `g/ω_n=0.30`）**：Zhang 2024（西北工业大学学报）用参数平面法解析设计 PI+陷波参数、突破「同实部代表极点」经验整定，可据惯量比系统化配陷波中心频率——正把本项目「经验定 g=100」升级为「据 ω_n 系统化整定陷波与有源阻尼」；连同 SpecForge 2026 指出的「现代驱动器自动识别双惯量谐振并放置陷波、谐振须落在可陷波带宽」工程事实，直接补 `g/ω_n=0.30` 欠阻尼缺口，优先做。（→ §五 候选点：电控·自适应陷波 / 有源阻尼系统化设计（替代经验定 g））
- **分数阶双惯量建模 + 混沌/参数优化（机械侧建模升级）**：Xie 2026（Chaos, Solitons & Fractals）建含分数阶微分项的双惯量耦合振动模型、用扩展 Melnikov 法导出混沌临界与稳定区边界，揭示分数阶/非线性参数对动力学的调控——把本项目线性 Ks-Bs 扩成分数阶并给出可落地参数优化路径，直接支撑「分数阶双惯量系统建模与参数辨识」候选；本科生可先用该文分数阶模型替换整数阶对象，跑三档 Ks 看谐振捕捉是否更贴真实。（→ §五 候选点：机械·分数阶双惯量系统建模与参数辨识）

---

## 📅 2026-10-04（每日自动推送）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **Hu G., Xu D., Jiang B., Pan T., Hua W.** *Fixed-Time Cooperative Model-Free Sliding Mode Control for Fractional-Order Multi-Motor Systems*, **IEEE Trans. Automation Science and Engineering**, 2025, 22:22950–22961. — 面向分数阶多电机系统，用**分数阶超局部模型（model-free）** + **固定时间扰动观测器（FTDO）**构建**固定时间协同无模型滑模（FCM-SMC）**；以电机 #1 为参考，相对现有方法误差指标（ME/MAE/RMSE）最高降 64.8% / 70.2% / 69.7%，收敛时间有上界、与初值无关。DOI:10.1109/TASE.2025.3618245
2. **Hu G., Xu D., Hua W., Jiang B., Shi P., Rudas I.J.** *Fixed-Time Cooperative Sliding Mode Control for Synchronization of Multilinear Motor Systems*, **IEEE/ASME Trans. Mechatronics**, 2025. — 把多直线电机化为多智能体，提出**固定时间协同滑模（FTCSMC）** + **固定时间扰动观测器**实现速度一致；固定通信拓扑下速度一致在固定时间内达成、与初值无关，Lyapunov 证稳定，仿真+实验优于分布式 PI 与有限时间控制。DOI:10.1109/TMECH.2025.3585574
3. **Chen Y., Liu Y., Huang R., Liu C.** *Data-Driven Speed Synchronization Predictive Control of a Five-Leg VSI Driving Dual-PMSM System*, **IEEE Trans. Industrial Electronics**, 2026, 73(6):8392–8404. — **数据驱动 MPC（DSSPC）**基于特征建模建无参数依赖的一阶电流预测模型，改进 MPC 带占空比调节；用**电流参考补偿直接替代固定增益交叉耦合**消除双 PMSM 同步误差，对参数失配/外扰更鲁棒（© 2026 IEEE）。DOI:10.1109/TIE.2026.3657020
4. **Wang L., Zhao X., Jin H.** *Multi-motor synchronization control based on neural network adaptive sliding mode and dynamic gain*, **Electrical Engineering (Springer)**, 2026, 108:304. — 提出**动态增益偏差耦合（DGDCC）** + **神经网络最小参数学习自适应滑模（NN-MPLM-ASMC）**：用模型参考辨识（MRIM）实时估 PMSM 惯量、动态调节速度补偿器耦合增益，根治固定增益偏差耦合的同步误差累积；实验证跟踪与同步精度显著提升。DOI:10.1007/s00202-026-03652-8

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **Zhu X., Li T., Tan X., Liang C.** *Design of Notch Filters Based on LQR Method for Resonance Suppression in Welding Servo Systems*, **2025 13th IEEE ICCMA**, 2025（入 IEEE Xplore 2026-02）. — 针对双质量系统固有谐振，提出**基于线性二次调节器（LQR）的陷波器系统化设计**：高置信度模型辨识 + 数字时延补偿、并行子系统分解分离谐振模态、cheap control 理论导出最优状态反馈并等价转串级陷波；实验证较常规 PI 与经验调谐陷波器谐振抑制更强、相位裕度更高、变载鲁棒。DOI:10.1109/ICCMA67641.2025.11369499
2. **Kawasaki M., Sakamoto N., Nakashima A., Kobayashi Y., Tanaka Y.** *An Observer-Based Vibration Suppression Method for Backlash-Induced Oscillations in EV Powertrains*, **IEEE Access**, 2026, 14:57444–57462. — 建含**轴弹性 + 粘滞阻尼 + 齿隙 + 量化噪声 + 通信延时**的控制导向非线性双惯量传动模型，设计**状态观测器**估相对位移与齿隙演化（接触/非接触切换），据此设计角速度阻尼控制（AVDC）与带冲击抑制的 AVDIMC；车辆实验证传动轴扭转转矩峰值约 2000 → 1000 Nm、振动稳定时间约 1.41 → 0.53–0.57 s。DOI:10.1109/ACCESS.2026.3674637
3. **（谐波齿轮传动柔性双连杆机械臂, 2026）** *谐波齿轮传动的柔性双连杆机械臂定位（解耦定位控制）*, **中文期刊（维普收录, 2026）**. — 建**双连杆三惯量**机械臂物理模型（每关节 = 电机侧 / 谐波齿轮侧 / 连杆侧三惯量经弹簧耦合），用 MIMO 解耦器在 2-DOF 半闭环框架内解耦连杆间耦合转矩并补偿角传动误差；实验证 ±0.1 mm 精度、0.1 s 稳定时间，是「三惯量扩展」的可复现对象。链接：http://dianda.cqvip.com/Qikan/Article/Detail?id=7108354596
4. **（ISA Transactions 2025）** *Analysis and design of oscillation frequency correction for servo resonance suppression*, **ISA Transactions**, 2025. — 针对双惯量伺服**陷波频率随工况漂移**问题，提出基于系统状态与机械参数的**在线振荡频率校正**方法：弃离线定点整定，用滑动模态观测器网络并行辨识刚度/惯量等、实时修正陷波中心频率，化解「参数变化 → 谐振抑制失效」。链接：https://www.sciencedirect.com/science/article/pii/S001905782500309X

### 💡 本批「可参考方向」
- **分数阶无模型固定时间滑模，给四策略之外加一类「免模型」新基线**：Hu 2025（TASE）用分数阶超局部模型 + 固定时间扰动观测器做多电机协同无模型滑模，误差指标较现有法最高降约 70%、收敛有上界；可作本项目四策略（尤其 DCC+DOB）的「无模型对照」，呼应 `innovation.md`「无模型自适应预测」方向——本科生先跑仿真对照即可，不必真上分数阶训练。（→ §五 候选点：电控·无模型自适应预测 + 多智能体事件触发）
- **固定时间滑模一致 + 固定时间 DOB，把 DCC+DOB 收敛「预约化」再进阶**：Hu 2025（TMECH）把多直线电机化为多智能体、FTCSMC + 固定时间 DOB 使速度一致在固定时间内达成且与初值无关，正是把本项目一阶 DOB 升级为「固定时间扰动观测 + 滑模一致」的现成基线，直接呼应创新点一 DOB 再进阶闭环。（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））
- **数据驱动 MPC 替代固定增益交叉耦合，作「无权重因子快速预测」可算版**：Chen 2026（TIE）用特征建模 + 改进 MPC 占空比调节，并以**电流参考补偿直接替代传统定增益交叉耦合**消除双 PMSM 同步误差、对参数失配更鲁棒——正好把本项目「模型预测同步」候选从「权重整定难」痛点里解放，可先在四电机仿真用「虚拟均值电机」当协调层试跑，再决定是否进实物。（→ §五 候选点：电控·模型预测同步（无权重因子快速预测））
- **动态增益偏差耦合（DGDCC）是「动态耦合增益」最贴实现锚点**：Wang 2026（Springer）用模型参考辨识实时估 PMSM 惯量、动态调节速度补偿器耦合增益，根治固定增益偏差耦合的同步误差累积，配 NN 自适应滑模抑抖振——正是本项目 `innovation.md`「动态耦合增益」候选的可抄实现；直接把固定权重换成随惯量/同步误差自适应的增量公式搬进 `control_dcc.c`，对比突加载下固定 vs 动态增益的 RMS 与恢复时间。（→ §五 候选点：电控·动态耦合增益）
- **机械侧三件套（LQR 系统化陷波 + 在线频率校正 + 含齿隙非线性 + 三惯量谐波减速器）合力补 `(g/ω_n=0.30)` 缺口与结构升级**：Zhu 2025 的 LQR 陷波把「经验定 g」升级为「按模型系统化整定」、ISA Trans 2025 的在线振荡频率校正随工况实时修正陷波中心频率——二者直接治本项目当前 `g/ω_n=0.30` 欠阻尼（对应 `innovation.md` ④ 最小验证路径）；Kawasaki 2026 的含齿隙非线性双惯量观测器（EV 传动轴，转矩峰值降约半、稳定时间缩至 0.53 s）可替换本项目线性 Ks-Bs 为含齿隙对象使换向冲击更真；谐波减速器双连杆三惯量解耦（2026）则给「三惯量扩展」提供可复现建模。四者共同把创新点二「带宽-刚度匹配」与机械组刚度分级/三惯量方向落成闭环。（→ §五 候选点：机械·三惯量扩展）

---

## 📅 2026-10-02（每日自动推送）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **Li T., Chen Z., Hou L.** (辽宁工程技术大学) *Speed Synchronization of Multi-PMSMs With Multiagent Prescribed-Time Sliding Mode Consensus Control*, **IEEE Journal of Emerging and Selected Topics in Power Electronics**, 2025, 13(6):7716–7730. — 把多 PMSM 速度协调化为**多智能体一致性问题**，设计**全局预设时间时变滑模（TVSM）一致控制** + **双层自适应预设时间扰动观测器（PTDO）**（无需扰动导数上界），Lyapunov 证预设时间内达成同步；较偏差耦合显著提升精度与鲁棒。DOI:10.1109/JESTPE.2025.3562166
2. **Wang J., Wang Y., Wu K.** (南京理工大学) *Speed synchronization control of multi-PMSM system based on FCS-MPC*, **Electric Power Systems Research**, 2026, 113166. — 把三 PMSM 视为 MIMO **统一模型**，提出**非级联有限控制集 MPC（FCS-MPC）**将速度跟踪与同步误差并入单一价值函数，并加 **Luenberger 负载转矩观测器**在线估扰补偿；较偏差耦合峰值跟踪误差 110.25→53.92 rpm、恢复 1.46→0.82 s、同步误差 38.84→31.32 rpm。DOI:10.1016/J.EPSR.2026.113166
3. **Pham V.T., Le T.-L.** *Adaptive speed synchronization of dual-motor drives using a recurrent Type-2 fuzzy NARX-CMAC network*, **Measurement Science and Technology**, 2026, 37:326203. — 提出**递归二型模糊 NARX-CMAC** 统一非线性控制：二型模糊处理不确定性、NARX 抓时序动态、CMAC 局部快学习、递归结构借历史信息提升暂态同步与抗扰；噪声/时变负载下优于 PID 等常规法。DOI:10.1088/1361-6501/ae92aa
4. **吕刚强, 袁浩, 高泽坤, 戴瑞娇, 王慧慧, 刘洋, 张旭** *基于新型滑模控制与自适应扩展状态观测器的双 PMSM 运动系统性能同步控制*（Prescribed performance synchronization control of dual-PMSM motion system via novel sliding mode control and adaptive extended state observer）, **ISA Transactions**, 2026（网络首发 2026-07）. — 融合**预设性能交叉耦合控制（PPCCC）** + **新型滑模控制（NSMC：指数-幂次混合到达律 + sigmoid 抑抖振）** + **自适应带宽扩展状态观测器（AESO）**估集总扰动前馈；对数壁垒函数约束同步误差收敛率与超调，仿真/实验验证同步误差压在预设界内。链接：https://www.ebiotrade.com/newsf/2026-7/20260709174614063.htm

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **Zou J., Zhao K., Fan G., Shen K., Jia L.** *Improved Terminal Sliding Mode Control for PMSM Dual-Inertia System Based on Dual Finite-Time Disturbance Observers*, **Progress In Electromagnetics Research C**, 2026, 173:256–265. — 针对柔轴联轴 + 负载扰动的 PMSM 双惯量系统，提出**高阶非奇异快速积分终端滑模（HONFITSMC）** + **双有限时间扩张滑模扰动观测器（dual FTESMDO）**分别估**轴转矩（电机侧）**与**负载侧扰动**并前馈补偿；仿真与实验证抗扰与鲁棒性显著提升。链接：https://www.jpier.org/issues/pierc.html?volume=3&page=34
2. **（2025 28th ICEMS, Busan）** *Disturbance Observer based Robust Nonlinear Position Tracking Control for Dual-Motor Servo System with Backlash*, **ICEMS 2025**. — 针对含**齿隙**双电机 PMSM 系统，提出**双电机反步位置控制器（DM-BPC）** + **四阶线性扩张状态观测器（FLESO）**估系统不确定（外部负载 + 内部参数变化），并设计**动态转矩偏置（DTBS）** + **微分二阶滑模速度同步控制器（SSMSSC）**抑制齿隙非线性与同步误差；实验证高精度负载侧运动控制。DOI:10.23919/ICEMS66262.2025.11317641
3. **He C., Lu S., Zheng S., Song B.** *Resonance Suppression Based on Improved BFGS Notch Filter and Simplified Linear Triangular Model for Double-Inertia Servo Control*, **IEEE/ASME Transactions on Mechatronics**, 2024, 29(3):2150–2160. — 针对传统自适应陷波缺陷，提出**改进 BFGS（IBFGS）参数自整定三参数陷波器** + **简化线性三角模型（SLTM）**近似系统梯度，用修正 Hessian + 扩展割线方程加速收敛、在线整定陷波参数抑制双惯量谐振；仿真与实验证有效。DOI:10.1109/TMECH.2023.3335341
4. **Zell C.V., Willeke S., Reinsperger N., Steidl M.** *Numerical and Experimental Analysis of Compliant Zero Stiffness Mechanisms for Torsional Vibration Isolation*, **Lecture Notes in Mechanical Engineering (Springer, ICOVP 2026)**, 2026, pp 25–34. — 设计**准零刚度（QZS）柔性联轴**结构：常规线性正刚度 + 非线性负刚度梁后屈曲柔顺机构组合，使扭矩波动传递段进入近零刚度区；往复发动机案例**扭振幅值降达 93%**，为"刚度分级/可调"提供量化结构方案。链接：https://link.springer.com/chapter/10.1007/978-3-032-11549-2_3

### 💡 本批「可参考方向」
- **预设时间滑模一致 + 双/自适应扰动观测，把 DCC+DOB 收敛"预约化"**：Li 2025 的全局预设时间 TVSM 一致 + 双层自适应 PTDO（不依赖扰动导数上界）把多电机同步锁定在预设收敛时间内、抗扰更强，吕刚强 2026 的 PPCCC + AESO 把同步误差经对数壁垒函数约束在预设界内、自适应带宽 ESO 实时估扰前馈，Zou 2026 更在双惯量系统用**双 FTESMDO** 分别估轴转矩与负载侧扰动——三者共同把本项目一阶 DOB 升级为"预设时间/双有限时间扰动观测 + 滑模一致"进阶基线，直接呼应创新点一 DOB 再进阶闭环（Pham 2026 的递归二型模糊 NARX-CMAC 可作无模型智能对照）。（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））
- **FCS-MPC 统一模型多电机同步作"无权重因子快速预测"可算版**：Wang 2026 把三 PMSM 当 MIMO 统一模型、FCS-MPC 单价值函数并跟踪+同步、加 Luenberger 负载转矩观测器，较偏差耦合峰值误差降约半、恢复时间缩 44%、同步误差降约 20%；可直接对照本项目四策略，作创新点三"无权重因子快速预测"的现成对照组，且给出与 DCC 的量化差距锚点。（→ §五 候选点：电控·模型预测同步（无权重因子快速预测））
- **改进 BFGS 自适应陷波直接治 `g/ω_n=0.30` 缺口（最优先）**：He 2024 用 IBFGS 自整定三参数陷波 + SLTM 在线逼近系统梯度、避免经验定参，正是把本项目"经验定 g=100"升级为"据 ω_n 系统化/优化整定陷波"的直接参照；本科生可把固定 g 换成 IBFGS 自整定陷波模块，重跑三档 Ks 对比固定 vs 自整定，直接验证创新点二协同优选能把 RMS 压回基线。（→ §五 候选点：电控·自适应陷波 / 有源阻尼系统化设计（替代经验定 g））
- **含齿隙/摩擦的非线性双惯量对象，让仿真更贴真实**：ICEMS 2025 的 DM-BPC + 四阶 LESO（FLESO）+ 动态转矩偏置 + 微分二阶滑模同步，把"齿隙非线性 + 双电机位置同步"做成完整可复现框架，吕刚强 2026 的 PPCCC + AESO 亦以交叉耦合结构约束同步误差于预设界内——两者都可替换本项目线性 Ks-Bs 为含齿隙非线性，使参数摄动/换向冲击场景更真，反哺创新点一/二的鲁棒性论证。（→ §五 候选点：机械·含齿隙/摩擦的非线性双惯量模型）
- **准零刚度柔性联轴把"刚度分级协同"落成可量化结构**：Zell 2026 的 QZS 联轴（正刚度 + 负刚度柔顺机构组合、扭振降 93%）给本项目"柔性联轴器刚度分级/可调"候选提供可复现结构方案——机械组可按"软段隔振 + 硬段传扭"双模态设计联轴，电控侧据 ω_n 切档扫参，形成创新点二"带宽-刚度匹配"的实物级交付物。（→ §五 候选点：机械·柔性联轴器刚度分级 / 可调）

---

## 📅 2026-10-01（每日自动推送）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **Zhang X., Zhuang Y., Wu C.** (东北大学) *Collaborative Control Algorithm of Dual PMSMs Based on Improved Sliding Mode Control*, **Journal of Physics: Conference Series**, 2025, 3108:012013. — 提出**交叉耦合 + 超扭曲趋近律 + 线性扩张观测器（LESO）+ 无差拍电流预测（DPCC）**双 PMSM 协同架构：超扭曲到达律平滑抖振、LESO 传电机状态消噪、DPCC 替代电流环 PID；仿真验证有效。DOI:10.1088/1742-6596/3108/1/012013
2. **冯高明, 周庆凯, 谭兴国** *矿用电机车双电机同步控制方法研究*, **河南理工大学学报（自然科学版）**, 2025, 44(2):128–137. — 新型积分滑模控制器 + **改进交叉耦合（耦合系数改为自适应）**缓解稳态/暂态矛盾 + **滑模扰动观测器（SMDO）**估外部扰动前馈补偿；仿真表明对负载扰动转速变化小、对参数差异不敏感、抗扰鲁棒。链接：https://xuebao.hpu.edu.cn/info/11197/96074.htm
3. **Zhang X., Sun Z., Chen J., Zhao Z., Liu T., Hou W.** *Predictive Position Synchronization Control of Dual PMSM System Based on Geometric Constraints*, **IEEE Trans. Power Electronics**, 2026, 41(3):3399–3411. — **增量 MPC + 几何约束可行域**：把电流/电压约束映射到二维平面、用价值函数椭球族与可行域切点解析求解，**算力较 QP 降约 47%、动态响应提速约 57%**，化解多电机同步「精度-算力」权衡。DOI:10.1109/TPEL.2025.3605365
4. **朱昌林, 涂群章, 蒋成明, 汪世骄** (陆军工程大学) *多电机速度同步系统中程偏差耦合控制*, **电气传动**, 2025, 55(1). — 在偏差耦合骨架上用**中程速度**算转速偏差优化结构 + **非奇异 Terminal 滑模（NFTSMC）**设计控制器；多电机实验平台验证同步性能提升。DOI:10.19457/j.1001-2095.dqcd25159

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **梅凯龙, 魏佳丹, 张泽宇, 周波（南京航空航天大学）** *一种双惯量弹性位置伺服系统及其控制方法*, **发明专利 CN121173144A**（公开 2025-12-19）. — 以电机速度为**二阶线性扩张状态观测器（LESO）**输入、引入速度观测误差重构内外扰动，实时观测含传动弹性形变的**时变弹性扰动**并经状态误差反馈补偿，简化结构同时抑制双惯量机械谐振与位置末端抖振。链接：https://patents.google.com/patent/CN121173144A/
2. **（授权发明专利 CN119045392B）** *一种融合模型与数据驱动的双惯量系统位置控制方法*. — **奇异摄动降阶**（慢/快子系统）+ 慢变子系统用**解耦扩张状态观测器（DESO）两自由度 ADRC**、快变子系统用**数据驱动 + 神经网络 PD**；**无需已知刚度参数即可抑振**，跟踪与抗扰性能解耦。链接：https://patents.google.com/patent/CN119045392B/zh
3. **（2026 IEEE DDCLS）** *Vibration suppression of 2- and 3-inertia systems based on disturbance observer compensation*. — 提出**基于扰动观测器补偿**的统一框架同时覆盖**二惯量与三惯量**系统：DOB（含二阶低通）+ 滑模控制器，借双曲函数新收敛律消抖振；数值验证在 2-/3-惯量柔性系统均有效，且观测器与控制器可分别设计、易移植。链接：https://www.mendeley.com/catalogue/ecead238-68c9-39cd-8fbf-c3a8ab973d6a
4. **（伺服联轴器选型技术文档）** *CNC 滚珠丝杠系统伺服联轴器的选型*（工业传动知识库）. — 给出量化选型法则：双惯量系统**固有频率 ω_n=√(K·(1/J₁+1/J₂))**，应选使共振频率**高于 500 Hz（位置环主导轴）/ 1 kHz（高动态轮廓）**的联轴器刚度；并对比波纹管（K≈3000 N·m/rad，共振≈975 Hz，留 5 倍裕量）vs 星形（K≈150，共振≈218 Hz，逼近速度环带宽）联轴器。链接：https://industrialmonitordirect.com/pt/blogs/knowledgebase/servo-coupling-selection-for-cnc-lead-screw-systems

### 💡 本批「可参考方向」
- **双惯量 ESO 化，把「机械谐振观测」直接嫁接进 DCC+DOB**：南航专利 CN121173144A 用二阶线性 ESO 实时观测含时变弹性扰动的双惯量系统、CN119045392B 用 DESO 两自由度 ADRC + 数据驱动 PD 在「未知刚度」下抑振——两者把本项目「线性 Ks-Bs 对象 + 一阶 DOB」升级为「双惯量 ESO/DESO 化」，正好对应 `innovation.md` 最该先做的「PLL-ESO 谐振观测」候选；本科生可先在 `control_dcc_dob.c` 旁加一个 ESO 模块估时变弹性扰动，对比 DOB-only vs DOB+ESO 的谐振峰衰减与同步 RMS。（→ §五 候选点：电控·PLL-ESO 谐振观测）
- **改进交叉耦合 + 自适应耦合系数，是「动态耦合增益」现成落地**：冯高明 2025 把交叉耦合的耦合系数改成自适应（常态低协同刚度包容相位滞后、强扰瞬时高协同），配 SMDO 估扰补偿——正是本项目 `innovation.md`「动态耦合增益」候选的可抄实现；直接把固定权重换成随同步误差自适应的增量公式搬进 `control_dcc.c`，对比突加载下固定 vs 自适应增益的 RMS 与恢复时间。（→ §五 候选点：电控·动态耦合增益）
- **几何约束增量 MPC 作「无权重因子快速预测」可算版**：张秀云 2026（IEEE TPEL）用几何约束把电流/电压约束映射二维平面、以椭球族与可行域切点解析求解，算力较 QP 降约 47%、动态响应提速约 57%——把本项目「模型预测同步」候选从「算力/权重整定难」痛点里解放出来，可先在四电机仿真用「虚拟均值电机」当协调层试跑，再决定是否进实物。（→ §五 候选点：电控·模型预测同步（无权重因子快速预测））
- **2-/3-惯量 DOB 统一框架，把双惯量扩成三惯量有现成脚注**：2026 IEEE DDCLS 的「DOB 补偿 + 滑模」同时覆盖二惯量与三惯量柔性系统、且观测器/控制器可分别设计易移植——正是本项目 `innovation.md`「三惯量扩展」候选的建模与控制铺垫；机械组可先把 Ks-Bs 扩成「电机—柔轴—中间惯量—负载」再接同一套 DOB/滑模对照。（→ §五 候选点：机械·三惯量扩展）
- **柔性联轴器量化选型，把「选型即定控制带宽」落成手册**：选型文档给出 ω_n=√(K·(1/J₁+1/J₂)) 与「共振频率需高于 500 Hz/1 kHz」的硬性阈值、并用波纹管 vs 星形联轴器算例量化刚度—共振关系——可直接转成本项目 `innovation.md`「联轴器选型量化手册」候选的交付物（Ks × ω_n × 阻尼对照表），让机械组按 ω_n 反推所需刚度、电控侧据 2~5 倍 ω_n 定带宽，落地「机电磁协同参数匹配」。（→ §五 候选点：机械·联轴器选型量化手册）

---

## 📅 2026-09-29（每日自动推送）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **Zhang D., Zhao L., Han X. 等** *Robust coordinated fault-tolerant control for aerospace multi-motor synchronous drive systems against inverter fault*, **Measurement and Control** (UK), 2026. — 针对多无刷直流电机**相邻交叉耦合**同步驱动，提出基于协同控制理论（synergetic control）的容错协调律：以扰动观测器 + 自适应技术抵消负载扰动，**逆变器故障下仍优先保证同步精度**；三重电机系统仿真与实验验证有效。DOI:10.1177/00202940261419018
2. **Sun G., Li F., Li C.** *Finite Time Adaptive Sliding Mode Control of Multi-Motor Based on Prescribed Performance*, **LNEE (ICMIC 2026)**, 2026, 1495:225–233. — 把**预设性能控制（PPF）**与**自适应非奇异快速终端滑模（NFTSMC）+ 扰动观测器（DO）**结合：PPF 把角跟踪误差约束在预设界内、NFTSMC 增强鲁棒、DO 估扰补偿，Lyapunov 证有限时间收敛；多电机伺服仿真验证。DOI:10.1007/978-981-95-3312-1_20
3. **Ma P., Li Z., Zhao J., Zhang N., Zhang Z.** *Lateral Stability and Synchronization Control for Dual-Motor Steer-by-Wire Vehicles*, **Symmetry**, 2026, 18(5):828. — 分层控制：上层 **MPC** 跟踪侧滑角/横摆率保横向稳定，下层 **ESO-复合趋近律滑模（ESO-CRLSMC）** 解决双电机参数失配与速度同步；硬件在环验证时变扰动/参数失配下同步与鲁棒性更优。DOI:10.3390/sym18050828
4. **Zhang G., Zhang P., Hua W., Fan Y., Guo X., Xu X.** *Improved deviation coupling control for multi-motor speed synchronization with PSO-based parameter optimization*, **Journal of Power Electronics**, 2025（online 2025-08-04；26(5):1211–1224, 2026）. — 在偏差耦合骨架（含虚拟电机）上引入**同步系数 + 跟踪系数**两个可调耦合增益，独立整定稳态/启动的同步与跟踪性能，并用**粒子群（PSO）优化**两系数；三电机实验验证优于传统偏差耦合。DOI:10.1007/s43236-025-01133-y

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **Zhang Z., Yang M., Lan P., Zhang X., Lv Z.** *Resonance Ratio Control for Vibration and Disturbance Suppression in Force Servoing*, **PCIM Asia Shanghai Conference 2025**, 2025. — 针对双惯量谐振系统提出 **PD + 谐振比控制（RRC）**：由**扰动观测器估反向转矩**定电机-臂谐振频率比、经极点配置抑扭振与扰动；并给出 DOB 最优速度，化解"DOB 需远快于谐振频率"的实现难题。DOI:10.30420/566583058
2. **Luo W., Li H., Zhang R., Zhang J., Vazquez S., Leon J.I., Wang X., Franquelo L.G.** *MPC-Based Sliding Mode Control of Dual-Inertia System Analysis*, **Energies**, 2026, 19(1):226. — 建**双惯量弹性模型**刻画弹性变形 + **背隙非线性**，提出分层架构：速度环 **Luenberger-观测器 MPC** + 电流环**超螺旋滑模（ST-SMC）**，同时实现状态估计增强鲁棒、滑模抑高频振动、MPC 约束传动轴转矩；比 PI/陷波 PI 更能压谐振与转矩纹波。DOI:10.3390/en19010226
3. **Lu S., Lu W., Zheng S., Song B., Li H.** *Double-Feedback Design Using an Estimation Network for Servo Resonance Suppression*, **IEEE Trans. Transportation Electrification**, 2025, 11(6):13203–13212. — 提出**五扩展滑模观测器（ESMO）估计网络**并行估**负载转速、电机惯量、负载惯量、刚度系数、负载转矩**，支撑**差分转速 + 轴转矩双反馈**经零极点配置抑机械谐振，并补偿负载转矩变化；仿真+实验验证。DOI:10.1109/TTE.2025.3600315
4. **Wang K., Huang G., Wang Z., Fan B.** *Adaptive synchronization control based on prescribed performance for dual-motor drive system*, **Proc. Inst. Mech. Eng. Part C (J. Mech. Eng. Sci.)**, 2026（OnlineFirst）. — 针对双电机驱动的**齿轮磨损、背隙、参数不确定**导致的死区非线性与模型失配，用连续函数近似死区 + **改进预设性能函数**约束暂态，反步框架 + 自适应律在线估传动参数、补偿背隙偏置转矩；实验证位置跟踪提升、转矩振荡抑制。DOI:10.1177/09596518261455945

### 💡 本批「可参考方向」
- **交叉耦合容错结构，把 DCC 鲁棒性推向"故障工况"**：Zhang D. 2026 在相邻交叉耦合骨架上加扰动观测器 + 自适应，使三重电机在逆变器故障下仍优先保同步精度——可作本项目 DCC+DOB 的"强扰动/故障"对照基线，本科生先在四策略脚本里给偏差耦合/DCC 加一个故障/扰动注入开关，对比正常 vs 故障的同步误差，反哺创新点一鲁棒性论证。（→ §五 候选点：电控·状态均值偏差耦合拓扑（解耦同步与跟踪））
- **高阶滑模 + 预设性能/ESO 观测，作 DCC+DOB 进阶**：Sun G. 2026 的 PPF+NFTSMC+DO 把"收敛+瞬态约束"打包进有限时间滑模，Ma P. 2026 的 ESO-CRLSMC+MPC 用 ESO 估扰动解决双电机参数失配与同步——两者都可替换本项目一阶 DOB 为"高阶滑模 + ESO 观测"，收敛更快、对初值/参数更不敏感，直接呼应创新点一"DOB 再进阶"。（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））
- **可调耦合增益（同步/跟踪系数）+ PSO 优化，是"动态耦合增益"直接落地参照**：Zhang G. 2025 在偏差耦合上引入同步系数 + 跟踪系数两个可调增益并经 PSO 寻优，比固定权重更灵活、且能独立整定稳态/启动性能——正是本项目 `innovation.md`「动态耦合增益」候选的现成实现，本科生可直接把这两个系数搬进 `control_dcc.c` 替代固定权重，呼应 W9–W13 迭代寻优。（→ §五 候选点：电控·动态耦合增益）
- **在线辨识 ω_n 与机械参数 + 自整定/反馈，治 g/ω_n=0.30（最优先）**：Zhang Z. 2025 的"DOB 估反向转矩定谐振比 + 极点配置"与 Lu S. 2025 的"五 ESMO 并行估刚度/惯量/负载转矩"共同把本项目"经验定 g=100"升级为"据 ω_n 与机械参数在线辨识 + 自整定"，直接补 `g/ω_n=0.30` 欠阻尼缺口；本科生可先用 sync 脚本算出的 ω_n 接一个 Luenberger/ESMO 在线估刚度模块，对比固定 g vs 自适应。（→ §五 候选点：机械·谐振频率在线辨识 + 自适应陷波）
- **含齿隙/死区非线性双惯量对象，让仿真更贴真实**：Energies 2026 把双惯量扩成"弹性变形 + 背隙非线性"并用 Luenberger-MPC/ST-SMC 抑振，Wang K. 2026 在双电机驱动中处理齿轮磨损/背隙/死区并做预设性能自适应同步——两者都可替换本项目线性 Ks-Bs 为含齿隙/死区非线性，使参数摄动/换向冲击场景更真，反哺创新点一/二的鲁棒性论证。（→ §五 候选点：机械·含齿隙/摩擦的非线性双惯量模型）

---

## 📅 2026-09-28（每日自动推送）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **许德智, 牟泮龙, 潘庭龙, 张清越, 叶宇剑, 花为** *基于预设时间滑模的多直线电机系统位置协同控制*, **控制与决策**, 2026（第 5 期）. — 设计**预设时间滑模控制器（PTSMC）**使综合误差在**预设时间内**收敛到零邻域（收敛时间可"预约"，不随初值漂），**非线性干扰观测器（NDO）**估扰并前馈补偿；用控制律切换消奇异性、稳定后控制器不再依赖时间；多直线电机仿真 + 实验验证协同跟踪一致性。链接：http://kzyjc.alljournals.cn/kzyjc/article/abstract/2025-0204
2. **Hou L., Shi C., Zhang P., Ren Y.** *Second-Order Finite-Time Leader-Following Multiagent Systems-Based Integrated Speed Cooperative Control for Multi-PMSMs*, **IEEE J. Emerging and Selected Topics in Power Electronics**, 2026, 14(2):2069–2082. — 把每台 PMSM 视作一个 **agent**，用虚拟控制变量解耦重建**二阶多智能体（MAS）模型**，设**分布式领导-跟随自适应有限时间一致性协议** + **高阶超螺旋滑模观测器**实时估集总扰动并动态补偿，直接在矢量控制框架内生成 q 轴电压；与偏差耦合、一阶一致性对比实验验证同步精度与鲁棒性更优。DOI:10.1109/JESTPE.2025.3603849
3. **Yang C., Ren X., Song J., Zheng D.** *Predefined-Time Control with Prescribed Performance for Multi-Motor Servo System Based on Generalized Coupling Error*, **2026 IEEE DDCLS**, Jishou, China, 2026. — 提出**广义耦合误差（GCE）**：把电机**跟踪误差**与**同步误差统一成单一误差变量**，从而把"耦合跟踪+同步"问题化为 GCE 收敛，**有效避免跟踪环与同步环互相耦合**；构造 GCE 分数型误差转换函数直接调瞬态、规避传统性能转换法的数值奇异；四电机仿真验证负载跟踪与同步均在预设时间内收敛。DOI:10.1109/DDCLS71227.2026.11610012
4. **（Energies 2026）** *Virtual Leader-Guided Cooperative Control of Dual Permanent Magnet Synchronous Motors*, **Energies**, 2026, 19(3):640. — **虚拟领导 + 分层 MPC**：上层虚拟领导跑**模型预测速度控制（MPSC）**做协调跟踪、下层跑模型预测电流控制（MPCC）；理论复杂度分析显示该解耦架构较集中式 MPC **算力降约 75%**；另设负载扰动观测器估补外部转矩，突加载下超调比常规 PI 降约 20%。DOI:10.3390/en19030640

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **（J. Mech. Sci. Technol. 2026）** *Dynamic characteristics analysis and vibration suppression of electric drive transmission system*, **Journal of Mechanical Science and Technology**, 2026. — 用**集中质量法**建电驱动传动系统（EDTS）动力学模型（**把轴承接触载荷写进方程**）并仿真+实验验证；对集中质量模型简化后**引入三惯量建模**技术，提出**RBF 神经网络补偿滑模（NNSMC）**控制三惯量模型，网络在线辨识不确定项、提升跟踪精度，抑制扭振且跨结构参数鲁棒，平均绝对误差比常规 SMC **低 16.67%**。DOI:10.1007/s12206-026-0403-x
2. **（IEEJ J. Industry Applications 2026）** *Low-Shock Joint Torque Control of Two-Inertia Systems with Backlash using a Torque-Sensor-Based Nonlinear Observer*, **IEEJ Journal of Industry Applications**, 2026 (advpub 2026-02-18). — 针对**含齿隙**双惯量系统的转矩换向冲击：常规线性观测器在齿隙段传扭为零时失效，本文提出**转矩传感器非线性观测器**——接触模式估负载侧状态、齿隙模式**预测**状态，用"实测关节转矩 − 前向齿隙模型预测转矩"的误差做状态校正；配合关节转矩控制器 + 简单 **MPC 齿隙穿越**策略，显著降冲击、对参数摄动鲁棒。链接：https://www.jstage.jst.go.jp/article/ieejjia/advpub/0/advpub_20260218/_article/-char/en
3. **（电气技术 2026）** *无人飞行器柔性舵面自抗扰控制*, **电气技术**, 2026, 27(6):72–76+84. — 舵机与舵面之间为**柔性连接**，建电动伺服**双惯量数学模型**分析舵面振颤成因；用**扩张状态观测器（ESO）**对伺服系统"总和扰动"实时估计与补偿 + 状态误差非线性状态观测器，研制工程实用化 ADRC；加载台试验表明既能快速跟踪位置指令又避免谐振、消除舵面振颤（是"双惯量 + ESO"在柔连机电系统上的工程落地样例）。链接：https://d.wanfangdata.com.cn/periodical/dianqjs202606012
4. **（一种刚度可调柔性联轴器的设计, 2025）** *一种刚度可调柔性联轴器的设计*. — 给出**刚度可调柔性联轴器**结构方案（弹簧 + 柔索半球复合，同时补偿角度/轴向/平行度三类偏差），建旋转机械**动力机—柔性联轴器—工作机**数学模型，推稳态/瞬态响应与调整时间、超调量解析式；MATLAB 仿真给出**联轴器扭转刚度 Kθ 对输出响应的直接影响**（Kθ↑ → 扭转响应↓），为"选刚度即定动态"提供定量依据。链接：http://www.knowcat.cn/p/20250717/2571110.html

### 💡 本批「可参考方向」
- **多智能体有限时间一致性 + 超螺旋滑模观测，作"高阶滑模观测"进阶基线**：Hou 2026 把每台 PMSM 当 agent、用二阶 MAS + 高阶超螺旋滑模观测器估集总扰动后直接生成 q 轴电压，许德智 2026 的 PTSMC + NDO 则给出"收敛时间可预约"的滑模变体——两者都可作本项目 DCC+DOB 的进阶对照（观测器从一阶 DOB 升级为高阶/超螺旋，收敛更快、更抗初值），直接呼应 `innovation.md` 既有候选，本科生先在四策略脚本外挂一个观测器模块即可对照。（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））
- **广义耦合误差（GCE）统一跟踪与同步——耦合拓扑层面的新结构**：Yang 2026 用单一 GCE 把"跟踪误差 + 同步误差"合成一个变量，从根上避免两回路互相耦合，正是本项目"状态均值偏差耦合拓扑"的可落地实现；本科生可在 `control_*` 里把"跟踪误差 + 偏差耦合项"合成一个综合误差再设计反馈，对比四策略看是否同时改善跟踪与同步。（→ §五 候选点：电控·状态均值偏差耦合拓扑（解耦同步与跟踪））
- **虚拟领导 + 分层 MPC（算力降 75%）——"无权重因子快速预测"的可算版**：Energies 2026 的"上层虚拟领导 MPSC + 下层 MPCC"把一个大优化拆成两层小优化，算力降约 75% 还保留协调性，正好解决本项目"模型预测同步"最怕的算力/权重整定难题；可先在 4 电机仿真里用"虚拟均值电机"当上层领导试跑一周，再决定是否进实物。（→ §五 候选点：电控·模型预测同步（无权重因子快速预测））
- **三惯量建模 + RBF-NN 滑模（机械侧结构升级）**：JMST 2026 的"集中质量→三惯量 + RBF 神经网络补偿滑模"给出把本项目线性双惯量扩成**电机—柔轴—中间惯量—负载**的完整建模与仿真路径（含轴承接触载荷），是"三惯量扩展"候选的现成脚本文档；同批"一种刚度可调柔性联轴器（2025）"给出刚度可调结构方案与刚度—响应定量关系，可作为机械组把联轴器做成**分级/可调**的结构依据，让电控侧按 ω_n 换档扫参。（→ §五 候选点：机械·三惯量扩展）；配套结构侧（→ §五 候选点：机械·柔性联轴器刚度分级 / 可调）
- **含齿隙双惯量 + 转矩传感器非线性观测器（让仿真更贴真实）**：IEEJ 2026 的"接触模式估计 / 齿隙模式预测"非线性观测器 + MPC 齿隙穿越策略，可替换本项目线性 Ks-Bs 对象中"刚性轴"假设，让参数摄动/换向冲击场景更真，反哺创新点一/二的鲁棒性论证（本科生可先加一段齿隙非线性区再比四策略）。（→ §五 候选点：机械·含齿隙/摩擦的非线性双惯量模型）

---

## 📅 2026-09-27（每日自动推送）

### 🔌 电控侧（多电机同步 / 先进控制）近期进展 2024–2026
1. **Zhao J., Cai T., Xiong M., Yang C.** *Reinforcement Learning and Singular Perturbation-Based Optimal Speed Synchronous Control of a Flexible Coupling Dual-PMSM System*, **IEEE Trans. Industrial Informatics**, 2025, 21(12):9757–9766. — 面向**柔性耦合双 PMSM** 系统提出**无模型强化学习（RL）最优速度同步**控制：奇异摄动提取慢时间尺度降阶模型，RL 迭代学得最优速度调节器，时变参考下同步跟踪与暂态响应优于现有方案（中国矿大）。DOI:10.1109/TII.2025.3606936
2. **（电机与控制应用 2026）** *基于自抗扰与自适应偏差耦合的多电机协同控制策略*, **电机与控制应用**, 2026（网络首发 2026-07-10）. — 单轴速度环引入 ADRC 统揽内扰 + 突加冲击；针对固定增益协同太"死板"，设计**高斯自适应偏差耦合**调节器：常态平滑死区包容非对称相位滞后，强冲击时高斯阶跃瞬时激发高协同刚度，瞬态最大协同误差降 **60%**、恢复时间缩 **40%~66.7%**（装甲观瞄云台）。链接：https://www.motor-abc.cn/djykzyy/article/html/20260710
3. **Gao P., Zhao C., Pan H., Fang L.** *A Model-Free Fractional-Order Composite Control Strategy for High-Precision Positioning of PMSM*, **Fractal and Fractional**, 2025, 9:161. — **无模型分数阶复合控制**：超扭曲双分数阶微分滑模（STDFDSMC）+ 互补型扩张状态观测器（CESO）前馈补偿内外扰，降抖振且保收敛，PMSM 精密定位优于整数阶方案。DOI:10.3390/fractalfract9030161
4. **Cui Y., Qu P., Liu C.** *Study on Coordinated Control Strategy of Multi-Pass Straight Drawing Machine System*, **Energies**, 2026, 19(7):1798. — LADRC 入速度环 + **卷尾猴搜索算法（CapSA）整定 LADRC 参数**，并改传统偏差耦合引入误差因子强化动态同步；拉丝多电机仿真显示超调与同步误差显著下降。DOI:10.3390/en19071798

### ⚙️ 机械侧（双惯量 / 柔性传动 / 谐振 / 设计）近期进展 2024–2026
1. **Wang X., Su Y., Luo Y., et al.** *Fractional-Order Modeling and Identification for Dual-Inertia Servo Inverter Systems with Lightweight Flexible Shaft or Coupling*, **Fractal and Fractional**, 2025, 9(4). — 把整数阶双惯量扩到**分数阶**以更准刻画轻量柔轴/柔联轴器的粘弹与记忆特性；用输出误差法 + **Levenberg–Marquardt（LM）算法**辨识参数，PMSM 伺服实验平台验证谐振捕捉精度提升（正对 `innovation.md` §五「LM 算法辨识」候选）。DOI:10.3390/fractalfract9040222
2. **Su Y., Wang X., Luo Y., Liang T., Chen Y.Q.** *Disturbance and Vibration Suppression of A Dual-Inertia Servo System with Fractional-Order Model*, **IFAC-PapersOnLine**, 2025, 59(37):91–96. — 分数阶双惯量伺服模型 + **滑模观测器（SMO）**估状态 + 补偿化为三积分器 + 级联控制器抑扰抑振，分数阶建模比整数阶更贴真实柔传系统。链接：https://www.sciencedirect.com/science/article/pii/S2405896326000169
3. **Gong L., Tao J., Xiong Q., Chen J., Hua Z.** *Vibration Control for Active Magnetic Bearing Rotor System Based on Parameter Adaptive-PSO and Notch Filter*, **IEEE Trans. Industrial Electronics**, 2026, 73:1122. — **参数自适应粒子群（PA-PSO）自动整定陷波器**参数压制临界转速区共振峰，Sigmoid 非对称协同机制平衡全局/局部搜索；方法可直接迁移到双惯量伺服"按 ω_n 系统化整定陷波"。DOI:10.1109/TIE.2025.3595976
4. **Zhang F., Chen J., Hu Y., Gao Z., Lv G., Lin Q.** *Disturbance Rejection-Guarded Learning for Vibration Suppression of Two-Inertia Systems*, **arXiv:2404.10240**, 2024. — 提出**学习增强型 ESO（L-ESO）**：机器学习记忆并预测扰动、ESO 做反馈校正兜底，二惯量运动控制实验台验证扰动估计更快更鲁棒（"学习赋能控制"范式）。链接：https://arxiv.org/abs/2404.10240

### 💡 本批「可参考方向」
- **无模型强化学习 + 奇异摄动最优同步（四策略之外的新智能基线）**：Zhao 2025 直接针对"柔性耦合双 PMSM"（与本项目双惯量弹性对象同构）做无模型 RL 最优速度同步，可对照四策略、作"智能优化同步"新基线，呼应 `innovation.md`「无模型自适应预测」方向（本科生先跑仿真对照即可，不必真上 RL 训练）。（→ §五 候选点：电控·无模型自适应预测 + 多智能体事件触发）
- **高斯自适应偏差耦合（动态协同刚度，最贴合本项目）**：把电机与控制应用 2026 的"高斯自适应偏差耦合"搬进本项目偏差耦合/DCC——常态低协同刚度包容相位滞后、强扰时瞬时高协同刚度，正好对应把固定耦合权重升级为随工况自适应的**动态耦合增益**，且瞬态协同误差降 60% 的实证可直接作对照。（→ §五 候选点：电控·动态耦合增益）
- **分数阶超扭曲滑模 + 互补 ESO/SMO（降抖振）**：Gao 2025 的 STDFDSMC + CESO 与 Su 2025 的分数阶双惯量 SMO，把本项目"整数阶滑模 + ESO"升级为分数阶，更准捕捉柔传非线性、降抖振，可作 DCC+DOB 的"高阶滑模观测"进阶基线。（→ §五 候选点：电控·高阶滑模 + 扩张状态滑模观测（平均偏差耦合进阶））
- **系统化整定陷波/有源阻尼（治 `g/ω_n=0.30` 缺口，优先）**：Cui 2026 用 LADRC + 群智能（CapSA）系统化整定 + 改进偏差耦合，Gong 2026 用 PA-PSO 自动整定陷波器参数——共同把本项目"经验定 g=100"升级为"据 ω_n / 群智能系统化整定陷波与有源阻尼"，直接补创新点二 `g/ω_n=0.30` 欠阻尼缺口。（→ §五 候选点：电控·自适应陷波 / 有源阻尼系统化设计（替代经验定 g））
- **分数阶双惯量建模 + LM 辨识（机械侧，直接补 §五 候选）**：Wang 2025 用 LM 算法辨识分数阶双惯量参数、Su 2025 分数阶 SMO 抑振，把本项目线性 Ks-Bs 扩成分数阶并给出可落地辨识流程，直接支撑"分数阶双惯量系统建模与参数辨识"候选；Lin 2024 的 L-ESO（学习增强 ESO）则顺带为 W13 后"数字孪生/边缘计算"的数据驱动扰动估计埋下种子。（→ §五 候选点：机械·分数阶双惯量系统建模与参数辨识）

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
