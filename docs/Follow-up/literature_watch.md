# 文献日报（电控 + 机械，每日自动推送）

> **用途**：边做边学——每天自动从网上捞 3~4 篇电控 + 3~4 篇机械的最新文献/方向，
> 并在每条批次末尾写「可参考方向」，给本项目（多电机同步 + 双惯量弹性系统）提供先进借鉴。
> **机制**：由每日自动化任务（recurring automation）追加新批次；本文件首条为种子批次（2026-09-23）。
> **姊妹文档**：创新点汇总 `docs/Follow-up/innovation.md`；跨组数据源 `data/shared/mech_deliverables.csv`。

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
