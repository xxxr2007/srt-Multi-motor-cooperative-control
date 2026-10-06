# 辅导材料与学习范围

配套 [新版周任务表](weekly_tasks.md)，2026-10-06 整理。按知识点选读，不按不同版本的固定章号要求；现有教材优先，借阅即可，不要求购买整套。每周最多一份主材料加一份辅导材料，阅读与练习时间已经包含在周表内。

英文网页可用浏览器翻译；先认清公式符号，再做最小练习。课程证书、整本书读完和视频截图均不是验收条件。官方网页可能展示较新软件版本，以已安装版本的帮助为准，不为教程升级整个环境。

## 电控材料

### E1｜电机结构与工作原理（W3，后续按需回顾）

- 主材料：现有《电机学》或《电机与拖动》教材，选“直流电机基本结构、换向器与电刷、电磁转矩、反电动势”小节；此前正在读的教材可以继续用。
- 辅助：[NPTEL Electrical Machines I：电枢绕组讲义](https://archive.nptel.ac.in/content/storage2/courses/108106071/pdfs/2_4.pdf)。只用于理解线圈、换向片和叠绕/波绕接线区别，不要求完成全部绕组计算。
- 学到什么程度：能在模型上认出主要构件，说明电流如何进入旋转线圈；分清转轴中心线、主极轴线与几何中性线。仅看端部外形不能确定叠绕/波绕。
- 暂跳过：交流电机全套等效电路、复杂绕组设计、FOC。

### E2｜直流电机建模（W3–W8）

- 辅导：[密歇根大学 CTMS](https://ctms.engin.umich.edu/CTMS/)，进入 MOTOR SPEED → SYSTEM MODELING / SYSTEM ANALYSIS。
- 直接入口：[Motor Speed: System Modeling](https://ctms.engin.umich.edu/CTMS/index.php?example=MotorSpeed&section=SystemModeling)。若直链加载异常，从总入口进入。
- 学习顺序：看电路和受力图 → 理解机械方程 → 辨认输入输出 → 推导传递函数。
- **模型差别**：CTMS 的完整电机示例以电压为输入，含电感/电阻/反电动势；本项目当前简化机械对象以转矩为输入。不能把两套传递函数或参数直接混用。
- 最小练习：不看教材写出 `Jω'=Te-Bω-TL`，逐项写单位和符号方向。

### E3｜自动控制原理（W7–W9、W13–W16、W19）

- 主材料：《自动控制原理》（胡寿松等），只选拉普拉斯变换、传递函数、反馈框图、一阶/二阶响应、稳态误差。章节号以所持版本目录为准。
- 辅助：CTMS 的 Introduction 与 Motor Speed 控制教程，或本校自动控制原理课程对应讲次。不要同时追多个完整网课。
- 学习顺序：初值和微分变换 → 一阶对象 → P → PI → 限幅 → 多机误差反馈；W19 再补一阶低通与带宽。
- 最小练习：零初值推导转矩到转速 `1/(Js+B)`；闭卷画 PI 框图；给出比例控制为何可能存在稳态误差的例子。
- 暂跳过：复杂梅森公式题、完整根轨迹、状态反馈设计、H∞。多机程序用到向量时只补矩阵乘法。

### E4｜PI、离散实现与抗积分饱和（W9–W10、W20）

- 主辅导：[MathWorks 抗积分饱和示例](https://www.mathworks.com/help/simulink/slref/anti-windup-control-using-a-pid-controller.html)。重点看输出限幅、clamping/条件积分的作用，back-calculation 作选读。
- 参考：[CTMS 数字控制示例](https://ctms.engin.umich.edu/CTMS/index.php?example=MotorSpeed&section=ControlDigital)，先看采样的概念，进阶离散设计后移。
- 最小练习：大给定使控制量饱和，再降低给定；比较有/无抗饱和时的积分状态和转速恢复。
- 无 Simulink 也可按概念写 MATLAB/C 小程序；不要求为这个示例安装额外工具箱或购买许可证。

## 数学与数值基础

### B1｜一阶微分方程与数值分析（W4–W7）

- 主材料：现有高等数学教材“一阶线性微分方程”；数值分析教材“Euler 方法、Runge–Kutta 方法、误差与稳定性”。只读这几节。
- 辅助：[MIT 18.03 微分方程讲义目录](https://www.ocw.mit.edu/courses/18-03-differential-equations-spring-2010/pages/lecture-notes/)，选 First-order differential equations 与数值求解对应讲义，不跟完整学期。
- 最小练习：解 `ω'=50-5ω, ω(0)=0`；手算 Euler 前两步；做 h、h/2、h/4 比较。
- 核对：`ω(t)=10(1-exp(-5t))`，h=.1 时 Euler 得到第一步 5、第二步 7.5；解析值约为 3.935、6.321，差异是练习分析对象。

### B2｜二阶响应与机械振动（W17–W19）

- 主材料：机械振动或理论力学教材中的单自由度自由振动、阻尼振动；再映射到双惯量扭转。
- 辅助：[MIT Damped Harmonic Oscillators](https://ocw.mit.edu/courses/18-03sc-differential-equations-fall-2011/pages/unit-ii-second-order-constant-coefficient-linear-equations/damped-harmonic-oscillators/)。只读模型、特征根与欠阻尼曲线。
- 最小练习：将平移质量—弹簧—阻尼对应到转动惯量—扭转刚度—扭转阻尼，解释弹性储能和阻尼耗能。
- 两惯量模型的频率公式要注明两侧惯量所在的轴及无阻尼等假设，不能用一个经验带宽比替代闭环稳定性分析。

## 机械材料

### M1｜CAD 基础与惯量（W3、W5–W6、W11）

- 默认主材料：已安装 SolidWorks 的内置教程，按顺序练“草图约束—拉伸/旋转—装配—材料—质量属性”。现有电机模型只用来认结构，练惯量先用简单圆盘。
- 官方辅助：[质量和截面属性说明](https://help.solidworks.com/2021/english/SolidWorks/sldworks/c_Mass_and_Section_Properties_Overview.htm)。此页属于较早版本，概念适用，按钮位置以本机帮助为准。
- 最小练习：同一个圆盘改密度或半径，预测质量与 J 的变化后再查看软件输出；明确旋转轴与输出坐标系。
- CATIA 熟练成员用本机 Part Design / Assembly / Measure Inertia 帮助完成同样练习即可，不重复学 SolidWorks。B 和 Ds 不属于 CAD 几何质量属性结果。

### M2｜理论力学与转动惯量（W4–W8、W13、W16）

- 主材料：本校《理论力学》中的力矩、定轴转动、动量矩定理、转动动能；不展开整个静力学/运动学题库。
- 免费辅助：[OpenStax 转动动力学与转动惯量](https://openstax.org/books/college-physics-2e/pages/10-3-dynamics-of-rotational-motion-rotational-inertia)。先读转矩与惯量关系，再做简单刚体例题。
- 最小练习：手算圆盘/圆环惯量，与同几何 CAD 核对；用能量关系推导 `JL/i²`，明确 `i=ωm/ωL`。
- 不要把不同轴的惯量直接相加，或把质量惯量与截面惯性矩混为一谈。

### M3｜轴、联轴器与参数可信度（W7–W10、W14、W17–W22）

- 主材料：《材料力学》（如刘鸿文版）圆轴扭转小节；《机械设计》（如濮良贵等）轴与联轴器小节；《机械原理》传动比小节。学校已有教材可替代，不需购买三个新版本。
- 查样本：到实际候选联轴器制造商的官方产品页下载该型号数据表，查允许转矩、转速、孔径、扭转刚度及静态/动态定义。尚无型号时不指定虚构手册或页码。
- 最小练习：均匀圆轴 `Ks=G*Jp/L` 量纲核对；对比两份实际候选参数，记录单位、页码、工况与发布日期。
- Ds/B 无明确来源时写假设范围，后续做敏感性分析；不能把假设标成实测。制造工艺、热处理与完整有限元暂不要求。

## 工具与项目材料

### T1｜MATLAB 最小工具集（W5–W6）

- [MathWorks 学习入口](https://www.mathworks.com/support/learn-with-matlab-tutorials.html)：选择 MATLAB Onramp，练变量、数组、脚本和绘图；Simulink Onramp 可选，不另计强制课时。
- 用本机 `help ode45` / `doc ode45` 查语法，练参数、初值、时间跨度、图例和单位。
- 最小成果：一个别人从头运行就能重现的 .m 文件。完成互动课程可能需要账号/网络；无法访问时用本机帮助和项目脚本替代。

### T2｜C 语言按需阅读（W11 起）

- 主材料：现有 C 入门教材，选变量、数组、循环、函数、结构体、指针传参、文件输出。K&R《C 程序设计语言》适合有基础者查阅，零基础优先本校入门讲义。
- 辅助：`electrical/c/multi_motor_sync.c` 的现有注释，按调用链读。不要把内核注释视为无需核对的理论证明。
- 最小练习：用循环做 Euler 更新，通过指针修改一个状态，再输出两列 CSV；暂不做位运算、volatile 或嵌入式中断专题。

### T3｜Python 读数与画图（W12、W21）

- [NumPy 初学者指南](https://numpy.org/doc/stable/user/absolute_beginners.html)：选数组、索引与简单统计。
- [Matplotlib pyplot 教程](https://matplotlib.org/stable/tutorials/pyplot.html)：选 plot、标签、图例、多曲线与保存。
- 主练习材料：`electrical/c/plot_results.py`。先复现已有图，再改一个标题或曲线选择，最后独立读取一个 CSV。
- 不要求重复安装多个 Python 环境；用仓库已有可运行环境。动态图、深度学习、完整数据科学课程均不在本阶段范围。

### R1｜仓库中的学习路线

1. `README.md`：认识数据流和运行命令；历史数值、旧日期只供参考。
2. `docs/control_scheme.md`：先读被控对象和 PI，再按周读主从、CCC、DCC、DOB。
3. `tools/sync_mech_to_elec.py` 与 `electrical/c/merge_params.py`：追踪参数如何进入内核。
4. `electrical/c/multi_motor_sync.c`：按周读状态更新、控制器、sync_error、指标统计；核对代码与文档是否一致。
5. `mechanical/docs/plant_model.md`、`data/shared/mech_deliverables.csv`：检查单位、轴和来源，不自动相信“定案”字样。

读到冲突时写在周报：原说法、代码行为、证据、待处理问题。尤其不要把“相对给定的误差”与“电机间同步极差”混为同一个指标，也不要因 README 有排序就预设实验答案。

## 学习方法与求助

每次 90 分钟可分为：20 分钟阅读、20 分钟推导/手算、40 分钟动手、10 分钟写疑问。卡住 30 分钟就记录“期望—实际—已尝试—最小文件”，在答疑时间解决。资源是辅助，不以刷完课替代独立完成练习。
