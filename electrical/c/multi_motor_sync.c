/*=====================================================================
 * 多电机协作（同步）控制仿真 —— C 语言实现（嵌入式风格）
 *
 * 模型：4 台带参数摄动的电机（转速环 PI + 转矩饱和 + 条件抗积分饱和）
 *       被控对象两种模式（由联轴器刚度 Ks 自动切换，见 MECH_KS 注释）：
 *       - 刚性直连：单惯量 J·ω̇ = Te − B·ω − TL（机械组未交付 Ks 时的回退）
 *       - 弹性传动：双惯量（方案 A）—— 电机侧 J1 / 联轴器 Ks,Ds / 负载侧 J2，
 *         谐振频率 ω_n=√(Ks·(1/J1+1/J2))，与 DOB 带宽 g 构成机电耦合点
 * 策略：主从 / 交叉耦合(CCC) / 惯量加权偏差耦合(DCC) / DCC+扰动观测器(DOB)前馈
 *       —— 第 4 种为本项目的复合创新策略：耦合骨架之上叠加扰动观测器，
 *          把"纯反馈"升级为"前馈+反馈"，对负载扰动与参数摄动双重鲁棒；
 *          弹性模式下 DOB 还顺带吸收联轴器弹性扭矩（机电协同创新点）。
 * 输出：results/sim_data_c.csv  降采样输出转速（刚性=电机侧；弹性=负载侧）
 *       results/metrics_c.csv   指标汇总
 *       results/dob_est_c.csv   dcc_dob 策略 2 号机的扰动估计值与等效真值
 *                               （验证观测器收敛性，供画 DOB 效果图）
 *
 * 电机台数：只改 N_MOTOR 和 J_MOTOR/B_MOTOR 两张参数表即可扩展，
 *           全部循环、积分器、CSV 输出均按 N_MOTOR 自动适配。
 *
 * 编译（在本仓库根目录执行）：
 *   gcc -O2 -o build/multi_motor_sync.exe electrical/c/multi_motor_sync.c -lm   <- 推荐
 *   tcc     -o build/multi_motor_sync.exe electrical/c/multi_motor_sync.c       <- 勿加 -lm
 *   实测 TinyCC 0.9.27 (x64) 对本代码存在传参代码生成 bug（混合 double/int
 *   参数的函数会算错），一律优先用 gcc。
 *
 * 运行：
 *   build\multi_motor_sync.exe          （必须在仓库根目录运行，输出走相对路径）
 *
 * 设计取向：全程静态数组、零动态内存（不用 malloc）、参数用宏集中管理、
 *           函数职责单一，便于日后整体平移到 STM32 等嵌入式目标。
 *====================================================================*/
#include <stdio.h>   /* printf / fprintf / fopen / fclose —— 控制台打印与 CSV 输出 */
#include <stdlib.h>  /* 本文件未直接调用，保留以备扩展（如 atoi / exit） */
#include <math.h>    /* sin() —— 电机3 的周期波动负载；sqrt() —— 指标 RMS 计算 */
#include <direct.h>  /* _mkdir() —— Windows 下创建输出目录 results/ */

/*====================== 参数区 ======================
 * 参数优先级（两级，自动选择）：
 *   ① 【推荐】机械组交付 data/mechanical/mech_params.json ＋ 电控组配置
 *      data/electrical/ctrl_params.json → 经 electrical/c/merge_params.py 合并生成
 *      build/params_generated.h → 本文件直接 include（下面的默认值被覆盖）
 *   ② 【回退】若 ① 尚未生成，则用下方内置默认值（与合并前的初始参数同口径）
 * 好处：两组改参数只需重跑 merge_params.py 再编译，本文件一行都不用动。
 * 注意：本文件现位于 electrical/c/，到仓库根多了一级，include 的相对路径
 *       相应从 ../build/ 变为 ../../build/。
 */
#if defined(__has_include)
#  if __has_include("../../build/params_generated.h")
#    include "../../build/params_generated.h"   /* 合并后的参数：覆盖下方全部默认值 */
#    define PARAMS_FROM_FILE 1
#  endif
#endif

#ifndef PARAMS_FROM_FILE
/* ---- 内置默认参数（回退用，与 data/params.json 初始值一致） ---- */
#define N_MOTOR        4              /* 电机台数；四台同型号，参数带 ±10% 摄动 */
#define DT             1e-4           /* 仿真步长 s（0.1 ms），显式欧拉积分步长 */
#define T_END          3.0            /* 总仿真时长 s                  */
#define N_STEPS        30000          /* = T_END / DT（静态数组长度需整型常量） */

/* 四台同型号电机，±10% 参数摄动（体现多电机不一致性）
   const 修饰 -> 只读，编译器可放入 .rdata 段并做常量传播优化 */
static const double J_MOTOR[N_MOTOR] = { 0.010, 0.011, 0.0095, 0.0105 };   /* 转动惯量 kg*m^2（1/2/3/4 号） */
static const double B_MOTOR[N_MOTOR] = { 0.0012, 0.0011, 0.0013, 0.00115 };/* 粘性摩擦 N*m*s/rad（1/2/3/4 号） */

#define TORQUE_LIMIT   5.0            /* 转矩限幅 N*m（模拟电机/变频器能力上限） */
#define SPEED_KP       0.8            /* 转速环 Kp  N*m/(rad/s)    */
#define SPEED_KI       30.0           /* 转速环 Ki  N*m/rad        */

#define W_STAR         120.0          /* 目标转速 rad/s（≈1146 r/min） */
#define RAMP_END       0.5            /* 斜坡升速结束时刻 s（软启动，避免冲击） */
#define T_LOAD_STEP    1.2            /* 电机2 突加负载时刻 s      */
#define TL_STEP        1.8            /* 电机2 突加负载幅值 N*m    */
#define T_LOAD_SIN     2.0            /* 电机3 波动负载起始 s      */
#define TL_SIN_AMP     0.4            /* 电机3 波动负载幅值 N*m    */
#define TL_SIN_FREQ    2.0            /* 电机3 波动负载频率 Hz     */
#define T_LOAD_RAMP    1.8            /* 电机4 斜坡负载起始 s      */
#define T_RAMP_FULL    2.6            /* 电机4 达到满负载的时刻 s  */
#define TL_RAMP        1.0            /* 电机4 斜坡负载终值 N*m    */

#define SYNC_KP        0.6            /* 同步补偿比例增益 Ks       */
#define SYNC_KI        5.0            /* 同步补偿积分增益 Ksi      */

/* ---- DCC+DOB 复合策略（创新点）专用参数 ---- */
#define DOB_G          100.0          /* 扰动观测器带宽 rad/s（≈15.9 Hz，即 g/(2π)）。
                                       * 整定依据（三条都要满足）：
                                       * ① 离散稳定：显式欧拉要求 g·DT≪1，此处 100×1e-4=0.01，余量充足；
                                       * ② 跟得上扰动：约为最快扰动（电机3 正弦负载 2 Hz≈12.6 rad/s）的 8 倍，
                                       *    该频率下幅值衰减 <1%、相位滞后 atan(12.6/100)≈7°，估计几乎不失真；
                                       * ③ 不与转速环打架：经验取环带宽的 5~10 倍，前馈/反馈时标分离。 */

/* ---- 方案 A：弹性传动链（双惯量被控对象，第二个创新点）----
 * MECH_KS : 联轴器扭转刚度 N*m/rad —— 机械组交付后由 merge_params.py 写入。
 *           Ks<=0（未交付）时内核自动退回"刚性直连"单惯量模型，
 *           仿真行为与旧版逐位一致，验收基线不受影响；
 *           Ks>0 时启用双惯量模型（见 plant_step2 的推导注释）。
 * MECH_DS : 联轴器等效黏性阻尼 N*m*s/rad（弹性扭矩的阻尼项）。
 * MECH_JL : 负载侧惯量 kg*m^2（电机侧沿用 J_MOTOR[i]，±10% 摄动随之带入两侧）。
 * 机电耦合点：谐振频率 ω_n = √(Ks·(1/Jm+1/JL))。观测器带宽 g 必须覆盖 ω_n 的
 *           2~5 倍才能把谐振"当作扰动"估计掉 —— 这条约束正是两组协同设计的接口。 */
#define MECH_KS        0.0            /* 默认 0 = 刚性（机械组未交付 Ks 时的回退值） */
#define MECH_DS        0.0            /* 阻尼默认 0（刚性模式下不参与计算） */
#define MECH_JL        0.010          /* 负载侧惯量默认值（与 J_MOTOR 名义同量级） */

#define OUT_EVERY      100            /* CSV 降采样步数（100×1e-4 s = 10 ms 记一行） */
#define REC_THRESHOLD  0.5            /* 恢复判定阈值 rad/s（误差回落到此值内算恢复） */
#define REC_HOLD       500            /* 恢复需保持的步数（500×1e-4 s = 50 ms） */

#endif /* PARAMS_FROM_FILE：回退默认值结束；若已 include 生成参数则整块跳过 */

/* 弹性模式总开关：Ks>0 即启用双惯量模型。做成编译期常量条件，
 * 两种模式各自走死代码消除后的直通路径，都不带 if 分支开销。 */
#define USE_ELASTIC   (MECH_KS > 0.0)

/*====================== 数据结构 ======================*/
/* 单个策略跑完后算出的四项性能指标 */
typedef struct {
    const char *name;                 /* 策略名（ASCII，画图时映射中文） */
    double rms;                       /* 升速后同步误差 RMS  rad/s */
    double peak;                      /* 突加载后 0.6s 内误差峰值  */
    double rec;                       /* 恢复时间 s，-1 表示未恢复 */
    double dip;                       /* 电机2 最大转速跌落 rad/s  */
} Metrics;

/* 策略间的共享"轨迹暂存区"：每个策略跑完把轨迹写进来，紧接着由 evaluate()
   读取算指标，随后被下一个策略覆盖。放在静态区而非函数栈上，是因为
   30000 点 × 8 B × 2 条 ≈ 480 KB，会撑爆默认线程栈。 */
static double se_hist[N_STEPS];       /* 当前策略的同步误差历史（复用） */
static double w2_hist[N_STEPS];       /* 当前策略电机2 转速历史（复用） */

/*====================== 工况与被控对象 ======================*/
/* 给定转速：0~RAMP_END 段线性斜坡升速，之后保持 W_STAR */
static double speed_ref(double t)
{
    return (t < RAMP_END) ? (W_STAR * t / RAMP_END) : W_STAR;
}

/* 各电机负载转矩（四种典型扰动各占一台，覆盖"无扰/突变/周期/渐变"谱系）：
     - 1 号机 全程空载（基准机）；
     - 2 号机 t≥1.2 s 起突加 1.8 N*m 恒负载（模拟单机卡滞 / 切削阻力）；
     - 3 号机 t≥2.0 s 起叠加 0.4·sin(2π·2t) N*m 周期波动负载；
     - 4 号机 t∈[1.8, 2.6] s 线性斜坡加载至 1.0 N*m 并保持（渐变负载）。
   OUT: tl[N_MOTOR] —— 本拍四台电机的负载转矩，缓冲区由调用者提供 */
static void load_torque(double t, double tl[N_MOTOR])
{
    int i;
    for (i = 0; i < N_MOTOR; i++) tl[i] = 0.0;   /* 先整体清零，避免残留上一拍的旧值 */
    if (t >= T_LOAD_STEP) tl[1] = TL_STEP;                       /* 电机2 突加恒负载 */
    if (t >= T_LOAD_SIN)
        tl[2] = TL_SIN_AMP * sin(2.0 * 3.14159265358979 * TL_SIN_FREQ
                                 * (t - T_LOAD_SIN));            /* 电机3 波动负载   */
    if (t >= T_LOAD_RAMP) {
        /* 线性斜坡：未到满载时刻按比例上升，到点后钳位在终值 TL_RAMP */
        double a = (t < T_RAMP_FULL)
                   ? (t - T_LOAD_RAMP) / (T_RAMP_FULL - T_LOAD_RAMP) : 1.0;
        tl[3] = TL_RAMP * a;                                     /* 电机4 渐变负载   */
    }
    /* 注：π 直接写字面量，避免依赖非标准宏 M_PI，便于跨编译器移植 */
}

/* 转速环 PI -> 电磁转矩，条件积分抗饱和
   IN : ref     本步参考转速 rad/s
        w       本步实际转速   rad/s
   IN/OUT: integ 积分累加项（跨步长保持，故传指针）
   OUT: 限幅后的电磁转矩 N*m
   抗饱和思路：若 PI 输出触限幅，就撤销本步的积分增量，
               避免"积分继续累积 → 退出饱和时大幅过冲"。 */
static double torque_pi(double ref, double w, double *integ)
{
    double e = ref - w;                        /* 转速误差 */
    double te;
    *integ += e * DT;                          /* 后向欧拉累加积分项 */
    te = SPEED_KP * e + SPEED_KI * (*integ);   /* PI 输出的原始转矩 */
    if (te > TORQUE_LIMIT || te < -TORQUE_LIMIT) {
        te = (te > 0.0) ? TORQUE_LIMIT : -TORQUE_LIMIT;   /* 双向饱和限幅 */
        *integ -= e * DT;                      /* 饱和时退积分（撤销本步增量） */
    }
    return te;
}

/* 机械方程欧拉离散 —— 刚性/弹性双模式的统一入口（方案 A 创新点的被控对象侧）
 *
 * ① 刚性模式（MECH_KS<=0，即机械组尚未交付联轴器刚度）：
 *      J·dω/dt = Te − B·ω − TL
 *      => ω(k+1) = ω(k) + DT·(Te − B·ω(k) − TL) / J
 *    与旧版单惯量 plant_step 逐步等价 —— 保证验收基线（RMS 0.041 等）不受本次
 *    重构影响； wl 恒等于 wm、th 恒为 0（刚性无扭转变形）。
 *
 * ② 弹性模式（MECH_KS>0，机械组交付 Ks 后自动切换）：双惯量弹性传动链
 *      电机侧：J1·ω̇m = Te − B·ωm − Ks·θ − Ds·(ωm−ωl)   ← 电磁转矩先扭联轴器
 *      负载侧：J2·ω̇l = Ks·θ + Ds·(ωm−ωl) − TL            ← 弹性扭矩驱动负载
 *      扭转角：θ̇ = ωm − ωl                                ← 弹性扭矩 = Ks·θ
 *    稳态自检：ωm=ωl、θ=TL/Ks —— 弹性扭矩恰好扛住负载，转速环只控电机侧。
 *    离散化采用"半隐式（辛）欧拉"：先用旧 θ 更新两侧转速、再用新转速差更新 θ。
 *    相比全显式欧拉，对无阻尼振子不做能量漂移，谐振仿真更稳（精度要求
 *    ω_n·DT ≪ 1，main() 里有运行时巡检）。
 *
 * IN/OUT: wm 电机侧转速 rad/s（控制器反馈用：编码器装在电机轴上）
 *         wl 负载侧转速 rad/s（同步误差评价用：实际"干活"的那一端）
 *         th 扭转角 θ rad（弹性状态，刚性模式下恒 0）
 *         三者均为跨步保持的状态量，故传指针（调用方按策略各自持有数组）。
 * IN    : te 电磁转矩 N*m、tl 负载转矩 N*m、i 电机序号（取 J/B 的摄动值）。
 * 说明  : 负载侧轴承摩擦相对很小，按理想处理（不给负载侧另设摩擦系数）。 */
static void plant_step2(double *wm, double *wl, double *th, double te, double tl, int i)
{
    if (USE_ELASTIC) {
        double tel  = MECH_KS * (*th) + MECH_DS * ((*wm) - (*wl)); /* 弹性扭矩=弹簧 Ks·θ + 阻尼 Ds·Δω */
        double dwm  = (te - B_MOTOR[i] * (*wm) - tel) / J_MOTOR[i];/* 电机侧角加速度（弹性扭矩是"内耗"） */
        double dwl  = (tel - tl) / MECH_JL;                        /* 负载侧角加速度（弹性扭矩是"动力"） */
        (*wm) += DT * dwm;                    /* 先更新两侧转速（用旧 θ 算的加速度） */
        (*wl) += DT * dwl;
        (*th) += DT * ((*wm) - (*wl));        /* 再用新转速差更新扭转角：半隐式欧拉 */
    } else {
        (*wm) += DT * (te - B_MOTOR[i] * (*wm) - tl) / J_MOTOR[i]; /* 刚性：单惯量，同旧版 */
        (*wl) = (*wm);                        /* 刚性直连：负载侧与电机侧同速 */
        /* th 保持调用方的初值 0：刚性模式无扭转变形，无需更新 */
    }
}

/* 输出转速选择：评价/存档用哪一端的转速。
 * 弹性模式取负载侧（ωl，真实输出端，谐振会在它上面显形）；刚性模式两者恒等。
 * 把这个口径集中成一个函数，四种策略共用，避免各处写法漂移导致指标不可比。 */
static void out_speeds(const double wm[N_MOTOR], const double wl[N_MOTOR],
                       double wo[N_MOTOR])
{
    int i;
    for (i = 0; i < N_MOTOR; i++)
        wo[i] = USE_ELASTIC ? wl[i] : wm[i];   /* 编译期常量分支，等于直接选一路 */
}

/* 同步误差：同型电机下 max|ωi−ωj| 等价于 max − min
   三种策略共用同一评价口径，对比才公平 */
static double sync_error(const double w[N_MOTOR])
{
    double mx = w[0], mn = w[0];   /* 以 1 号机转速初始化最大/最小值 */
    int i;
    for (i = 1; i < N_MOTOR; i++) {
        if (w[i] > mx) mx = w[i];  /* 更新最大转速 */
        if (w[i] < mn) mn = w[i];  /* 更新最小转速 */
    }
    return mx - mn;                /* 极差即最大同步偏差 */
}

/*====================== 三种策略 ======================*/
/* 主从：1 号机为主机跟踪给定，2/3/4 号跟踪主机"上一拍"的实际转速。
   IN: fp 已打开的 CSV 句柄（按 OUT_EVERY 降采样写转速）
   说明：从机参考取 prev_master 而非本拍的 w[0]，对应实际系统中
         "主机测速 → 通信 → 从机执行"必然存在的一拍延迟。 */
static void run_master_slave(FILE *fp)
{
    double w[N_MOTOR] = { 0 };                 /* 四台电机【电机侧】转速状态，从静止起步 */
    double wl[N_MOTOR] = { 0 };                /* 负载侧转速（弹性模式用；刚性模式恒等于 w） */
    double th[N_MOTOR] = { 0 };                /* 联轴器扭转角 θ（弹性模式用；刚性恒 0） */
    double wo[N_MOTOR];                        /* 输出转速（评价/存档口径，见 out_speeds） */
    double integ[N_MOTOR] = { 0 };             /* 四路转速环 PI 积分器 */
    double tl[N_MOTOR], refs[N_MOTOR], prev_master = 0.0;
    int k, i;

    for (k = 0; k < N_STEPS; k++) {
        double t = k * DT;                     /* 当前仿真时刻 s */
        load_torque(t, tl);                    /* 求本拍负载转矩 */
        refs[0] = speed_ref(t);                /* 主机：跟踪给定斜坡 */
        for (i = 1; i < N_MOTOR; i++)
            refs[i] = prev_master;             /* 全部从机：跟踪主机上一拍转速 */
        for (i = 0; i < N_MOTOR; i++) {
            double te = torque_pi(refs[i], w[i], &integ[i]);   /* 转速环算转矩（控电机侧） */
            plant_step2(&w[i], &wl[i], &th[i], te, tl[i], i);  /* 双惯量机械方程推进一拍 */
        }
        prev_master = w[0];                    /* 缓存主机本拍转速（电机侧，编码器位置），供下拍从机 */
        out_speeds(w, wl, wo);                 /* 选取评价口径的输出转速 */
        se_hist[k] = sync_error(wo);           /* 记录本拍同步误差 */
        w2_hist[k] = wo[1];                    /* 记录本拍电机2 输出转速（算跌落用） */
        if (k % OUT_EVERY == 0)                /* 降采样写 CSV，避免文件过大 */
            for (i = 0; i < N_MOTOR; i++)
                fprintf(fp, "%.4f,master_slave,%d,%.4f\n", t, i + 1, wo[i]);
    }
}

/* 交叉耦合：对"自身与其他电机平均转速之差"做同步补偿
   ε_i = ω_i − avg(ω_j, j≠i)，再补偿到参考转速：
   ω_ref,i = ω* − (Ks·ε_i + Ksi·∫ε_i dt)
   直观理解：谁比大家快就把谁的参考压低、反之抬高 —— 全场一起"扶"受扰的那台。 */
static void run_cross_coupling(FILE *fp)
{
    double w[N_MOTOR] = { 0 };                 /* 四台电机【电机侧】转速状态 */
    double wl[N_MOTOR] = { 0 };                /* 负载侧转速（弹性模式用） */
    double th[N_MOTOR] = { 0 };                /* 联轴器扭转角（弹性模式用） */
    double wo[N_MOTOR];                        /* 输出转速（评价/存档口径） */
    double integ[N_MOTOR] = { 0 };             /* 四路转速环 PI 积分器 */
    double isync[N_MOTOR] = { 0 };             /* 四路同步补偿积分器 ∫ε_i dt */
    double tl[N_MOTOR];
    int k, i, j;

    for (k = 0; k < N_STEPS; k++) {
        double t = k * DT;
        load_torque(t, tl);
        for (i = 0; i < N_MOTOR; i++) {
            double sum = 0.0, se, comp, te;
            for (j = 0; j < N_MOTOR; j++)
                if (j != i) sum += w[j];       /* 累加除自身外其他电机转速（电机侧，编码器量） */
            se = w[i] - sum / (N_MOTOR - 1);   /* ε_i = ω_i − avg(others) */
            isync[i] += se * DT;               /* 同步误差积分 */
            comp = -(SYNC_KP * se + SYNC_KI * isync[i]);   /* 负号：快的压低、慢的抬高 */
            te = torque_pi(speed_ref(t) + comp, w[i], &integ[i]);  /* 修正参考后过转速环 */
            plant_step2(&w[i], &wl[i], &th[i], te, tl[i], i);      /* 双惯量机械方程推进一拍 */
        }
        out_speeds(w, wl, wo);                 /* 评价口径：弹性模式取负载侧 */
        se_hist[k] = sync_error(wo);
        w2_hist[k] = wo[1];
        if (k % OUT_EVERY == 0)
            for (i = 0; i < N_MOTOR; i++)
                fprintf(fp, "%.4f,cross_coupling,%d,%.4f\n", t, i + 1, wo[i]);
    }
}

/* 惯量加权偏差耦合：对每一对 (i,j) 的偏差分别补偿，权重取对方惯量占比
   w_ij = J_j / Σ_{k≠i} J_k，即大惯量电机的偏差更"可信"、更值得跟随。
   与 CCC 的区别：CCC 只有一条"对平均值"的补偿通道、一个积分器；
                 DCC 对每对偏差各有一个积分器，响应更偏向跟随高惯量电机。 */
static void run_deviation_coupling(FILE *fp)
{
    double w[N_MOTOR] = { 0 };                    /* 四台电机【电机侧】转速状态 */
    double wl[N_MOTOR] = { 0 };                   /* 负载侧转速（弹性模式用） */
    double th[N_MOTOR] = { 0 };                   /* 联轴器扭转角（弹性模式用） */
    double wo[N_MOTOR];                           /* 输出转速（评价/存档口径） */
    double integ[N_MOTOR] = { 0 };                /* 四路转速环 PI 积分器 */
    double ipair[N_MOTOR][N_MOTOR] = { { 0 } };   /* 成对同步积分器 ipair[i][j]：i 对 j 的偏差积分 */
    double tl[N_MOTOR];
    int k, i, j;

    for (k = 0; k < N_STEPS; k++) {
        double t = k * DT;
        load_torque(t, tl);
        for (i = 0; i < N_MOTOR; i++) {
            double wsum = 0.0, comp = 0.0, te;
            for (j = 0; j < N_MOTOR; j++)
                if (j != i) wsum += J_MOTOR[j];   /* 权重分母 Σ_{j≠i} J_j */
            for (j = 0; j < N_MOTOR; j++) {
                if (j == i) continue;             /* 跳过自身，无自偏差项 */
                {
                    double wj = J_MOTOR[j] / wsum;    /* 惯量加权系数 w_ij */
                    double dev = w[i] - w[j];         /* 成对偏差 ω_i − ω_j（电机侧） */
                    ipair[i][j] += dev * DT;          /* 该对偏差的积分累加 */
                    comp -= wj * (SYNC_KP * dev + SYNC_KI * ipair[i][j]);  /* 加权累加补偿 */
                }
            }
            te = torque_pi(speed_ref(t) + comp, w[i], &integ[i]);   /* 修正参考后过转速环 */
            plant_step2(&w[i], &wl[i], &th[i], te, tl[i], i);       /* 双惯量机械方程推进一拍 */
        }
        out_speeds(w, wl, wo);                    /* 评价口径：弹性模式取负载侧 */
        se_hist[k] = sync_error(wo);
        w2_hist[k] = wo[1];
        if (k % OUT_EVERY == 0)
            for (i = 0; i < N_MOTOR; i++)
                fprintf(fp, "%.4f,deviation_coupling,%d,%.4f\n", t, i + 1, wo[i]);
    }
}

/*====================== 创新策略：DCC + 扰动观测器(DOB)前馈 ======================*/
/* 思想：前三种策略都是"纯反馈"——误差已经出现了，转速环 PI 才被动调节；
   本策略给每台电机再配一个扰动观测器，在线估计"集总扰动"并前馈抵消：
   不等误差出现就先把扰动补掉，反馈环只负责收拾估计残差。
   耦合骨架沿用 DCC（三种经典策略中最强者），区别仅在转矩指令多了前馈项。

   ① 集总扰动的定义：用名义参数 Jn、Bn 改写机械方程
        Jn·dω/dt = Te − Bn·ω − d
      对比真实方程 J·dω/dt = Te − B·ω − TL，可得
        d = TL + (J−Jn)·dω/dt + (B−Bn)·ω
      —— 真实负载和 ±10% 参数摄动的影响全被"吸"进 d 里，
         所以估计并抵消 d，等价于对负载扰动和参数失配同时鲁棒。

   ② 观测器构造（关键是避开微分）：由上式反解 d = Te − Bn·ω − Jn·dω/dt，
      直接对转速做数值微分会放大噪声、工程上不可用，故串联一阶低通 Q(s)=g/(s+g)：
        d̂ = Q(s)·(Te − Bn·ω − Jn·s·ω)
      利用恒等式 s·Q(s) = g·s/(s+g) = g·(1 − Q(s)) 把微分项消掉：
        d̂ = Q(s)·(Te − Bn·ω) − Jn·g·(1 − Q(s))·ω
          = Q(s)·(Te + (g·Jn − Bn)·ω) − g·Jn·ω
      写成状态方程（x 为滤波器状态），即下面的代码实现：
        u = Te + (g·Jn − Bn)·ω        （滤波器输入）
        ẋ = g·(u − x)                  （一阶惯性环节）
        d̂ = x − g·Jn·ω                 （读出方程）
      稳态自检（s→0 时 Q→1、x→u）：d̂ = Te − Bn·ω = d（此时 dω/dt=0）✓ 无静差。

   ③ 两条实现纪律（都是踩坑点）：
      - 观测器输入 Te 必须取【限幅后实际施加】的转矩：饱和期间若拿限幅前的
        PI 原始输出喂观测器，观测器会以为电机输出了并没有输出的转矩，
        d̂ 被系统性估偏 —— 这相当于观测器层面的"抗饱和"；
      - 前馈用【上一拍】的 d̂：本拍转矩还没施加，本拍 d̂（依赖本拍 Te）根本
        算不出来，硬用会构成代数环；用上一拍估计只引入 0.1 ms 延迟，
        相对观测器时间常数 1/g = 10 ms 可以忽略。

   IN: fp 已打开的转速 CSV 句柄（与三种经典策略同格式、同降采样）
   附带输出: results/dob_est_c.csv —— 2 号机（突加负载机）的 d̂ 与等效真值 d，
             供 plot_results.py 画观测器收敛效果图。

   弹性模式下的角色（方案 A 创新点的电控侧）：双惯量对象里，电机侧方程为
        J1·ω̇m = Te − B·ωm − [Ks·θ + Ds·(ωm−ωl)]
   对照 DOB 的名义模型 Jn·ω̇m = Te − Bn·ωm − d，可见【弹性扭矩整段被吸进 d】。
   于是观测器对 d 的前馈抵消顺带补偿了弹性扭矩 —— 但前提是 g 能覆盖谐振频率
   ω_n（详见文件头 MECH_KS 注释）：g < ω_n 时谐振频段 Q(s)≈0，DOB "看不见"
   谐振，此时必须靠机械侧让步（换软联轴器降 ω_n）或电控侧加陷波 —— 这条
   带宽-刚度匹配准则就是两组"机电协同设计"的定量接口。 */
static void run_dcc_dob(FILE *fp)
{
    double w[N_MOTOR] = { 0 };                    /* 四台电机【电机侧】转速状态，从静止起步 */
    double wl[N_MOTOR] = { 0 };                   /* 负载侧转速（弹性模式用） */
    double th[N_MOTOR] = { 0 };                   /* 联轴器扭转角（弹性模式用） */
    double wo[N_MOTOR];                           /* 输出转速（评价/存档口径） */
    double integ[N_MOTOR] = { 0 };                /* 四路转速环 PI 积分器 */
    double ipair[N_MOTOR][N_MOTOR] = { { 0 } };   /* 成对同步积分器（DCC 骨架，同前） */
    double x[N_MOTOR] = { 0 };                    /* 四路 DOB 滤波器状态 x */
    double dhat[N_MOTOR] = { 0 };                 /* 四路扰动估计 d̂（初值 0：静止空载无扰动） */
    double tl[N_MOTOR];
    double j_nom = 0.0, b_nom = 0.0;              /* 名义参数：取全组均值，N_MOTOR 通用化 */
    FILE *fpd;                                    /* dob_est_c.csv 句柄（观测器效果存档） */
    int k, i, j;

    for (i = 0; i < N_MOTOR; i++) { j_nom += J_MOTOR[i]; b_nom += B_MOTOR[i]; }
    j_nom /= N_MOTOR;   /* Jn = 0.01025 kg*m^2：观测器只需"差不多"的名义模型， */
    b_nom /= N_MOTOR;   /* Bn = 0.0011875 N*m*s/rad；失配部分由 d 一并兜底估计 */

    fpd = fopen("results/dob_est_c.csv", "w");
    if (fpd) fprintf(fpd, "t_s,d_true,d_hat\n");  /* 打不开就只跳过存档，不影响仿真本体 */

    for (k = 0; k < N_STEPS; k++) {
        double t = k * DT;
        load_torque(t, tl);
        for (i = 0; i < N_MOTOR; i++) {
            double wsum = 0.0, comp = 0.0, te_pi, te, u, w_old, w_new;
            for (j = 0; j < N_MOTOR; j++)
                if (j != i) wsum += J_MOTOR[j];   /* DCC 权重分母 Σ_{j≠i} J_j（同前） */
            for (j = 0; j < N_MOTOR; j++) {
                if (j == i) continue;
                {
                    double wj = J_MOTOR[j] / wsum;    /* 惯量加权系数 w_ij */
                    double dev = w[i] - w[j];         /* 成对偏差 ω_i − ω_j */
                    ipair[i][j] += dev * DT;
                    comp -= wj * (SYNC_KP * dev + SYNC_KI * ipair[i][j]);
                }
            }
            te_pi = torque_pi(speed_ref(t) + comp, w[i], &integ[i]);  /* 限幅后的 PI 转矩 */
            te = te_pi + dhat[i];                 /* 前馈：用【上一拍】的 d̂ 抵消扰动 */
            if (te > TORQUE_LIMIT) te = TORQUE_LIMIT;          /* 前馈后仍可能触限， */
            else if (te < -TORQUE_LIMIT) te = -TORQUE_LIMIT;   /* 必须重新钳位 */

            /* ---- 观测器更新（严格按推导的三行来） ---- */
            w_old = w[i];
            plant_step2(&w[i], &wl[i], &th[i], te, tl[i], i);   /* 双惯量真实对象推进一拍 */
            w_new = w[i];                         /* 观测器全部用【电机侧】转速（编码器量）：
                                                     弹性模式下弹性扭矩落入 d，由前馈一并补偿 */
            u = te + (DOB_G * j_nom - b_nom) * w_old;         /* 滤波器输入 u */
            x[i] += DT * DOB_G * (u - x[i]);                  /* ẋ=g(u−x) 的欧拉积分 */
            dhat[i] = x[i] - DOB_G * j_nom * w_new;           /* d̂ = x − g·Jn·ω（用新转速对齐同拍） */

            /* 2 号机的观测器效果存档：等效真值由 d = Te − Bn·ω_old − Jn·Δω/DT 反解，
               离散意义下严格成立；它与真实负载之差正是参数失配项（与弹性扭矩）的贡献 */
            if (fpd && i == 1 && k % OUT_EVERY == 0) {
                double d_true = te - b_nom * w_old
                              - j_nom * (w_new - w_old) / DT;
                fprintf(fpd, "%.4f,%.4f,%.4f\n", t, d_true, dhat[i]);
            }
        }
        out_speeds(w, wl, wo);                    /* 评价口径：弹性模式取负载侧 */
        se_hist[k] = sync_error(wo);
        w2_hist[k] = wo[1];
        if (k % OUT_EVERY == 0)
            for (i = 0; i < N_MOTOR; i++)
                fprintf(fp, "%.4f,dcc_dob,%d,%.4f\n", t, i + 1, wo[i]);
    }
    if (fpd) fclose(fpd);   /* 只关成功打开的句柄 */
}

/*====================== 指标评价 ======================*/
/* 依据缓存在 se_hist / w2_hist 里的本次仿真轨迹，算出四项指标
   IN : name 策略名（写进结构体，便于最后成表输出）
   OUT: Metrics —— RMS / 突加载峰值 / 恢复时间 / 电机2 最大转速跌落 */
static Metrics evaluate(const char *name)
{
    Metrics m;
    int k, k0, ok;
    /* 先把各时刻换算成"步下标"，避免在循环里反复做浮点除法 */
    int start = (int)((RAMP_END + 0.1) / DT);       /* RMS 统计起点：升速结束后 0.1 s */
    int step0 = (int)(T_LOAD_STEP / DT);            /* 突加负载时刻的步号 */
    int step1 = (int)((T_LOAD_STEP + 0.6) / DT);    /* 突加后 0.6 s 观察窗终点 */
    double sumsq = 0.0, w2min = 1e9;
    int n_rms = 0;

    m.name = name;
    m.rms = 0.0; m.peak = 0.0; m.rec = -1.0; m.dip = 0.0;   /* 先填默认值，杜绝读到未初始化内存 */

    /* ① 同步误差 RMS：升速稳定后全程均方根，反映整体同步水平 */
    for (k = start; k < N_STEPS; k++) { sumsq += se_hist[k] * se_hist[k]; n_rms++; }
    m.rms = sqrt(sumsq / n_rms);          /* 步数 3e4 量级，int 计数足够 */

    /* ② 突加载误差峰值、③ 电机2 最低转速：都只看突加后 0.6 s 窗口 */
    for (k = step0; k <= step1; k++) {
        if (se_hist[k] > m.peak) m.peak = se_hist[k];   /* 窗口内最大同步误差 */
        if (w2_hist[k] < w2min) w2min = w2_hist[k];     /* 窗口内电机2 最低转速 */
    }
    m.dip = W_STAR - w2min;   /* 电机2 最大转速跌落（真实值） */

    /* ④ 恢复时间：从突加时刻起，找第一个"误差回落到阈值内、且连续保持 REC_HOLD 步"的时刻 */
    for (k = step0; k < N_STEPS; k++) {
        ok = 1;
        for (k0 = k; k0 < k + REC_HOLD && k0 < N_STEPS; k0++)
            if (se_hist[k0] > REC_THRESHOLD) { ok = 0; break; }   /* 保持期内一旦超阈值即判失败 */
        if (ok) { m.rec = (k - step0) * DT; break; }              /* 恢复时间 = 步差 × 步长 */
    }
    return m;   /* 若始终未满足保持条件，m.rec 保持 -1，语义为"未恢复" */
}

/*====================== 主流程 ======================*/
#define N_STRATEGY 4                  /* 策略数量（主从/CCC/DCC/DCC+DOB），与电机台数无关 */
int main(void)
{
    FILE *fp;
    Metrics ms[N_STRATEGY];           /* 四个策略各存一份指标 */
    int i;
    double j_mean = 0.0, wn = 0.0;    /* 名义平均惯量 / 谐振频率（弹性模式巡检用） */

    _mkdir("results");   /* 已存在则返回 -1，此处忽略即可 */

    /* ---- 被控对象模式自检：启动时就告诉用户当前跑的是哪种物理模型 ---- */
    for (i = 0; i < N_MOTOR; i++) j_mean += J_MOTOR[i];
    j_mean /= N_MOTOR;                        /* Jm 名义均值（与 DOB 的 Jn 同口径） */
    if (USE_ELASTIC) {
        wn = sqrt(MECH_KS * (1.0 / j_mean + 1.0 / MECH_JL));  /* ω_n=√(Ks·(1/Jm+1/JL)) */
        printf("plant : elastic two-inertia (Ks=%.4g N*m/rad, Ds=%.4g, J_L=%.4g kg*m^2, w_n=%.1f rad/s)\n",
               MECH_KS, MECH_DS, MECH_JL, wn);
        if (wn * DT > 0.05)
            /* 半隐式欧拉的精度经验：每谐振周期至少 ~125 步（ω_n·DT<0.05），再大波形失真 */
            printf("[WARN] w_n*DT=%.4g 偏大，谐振仿真精度不足，建议减小 DT 或降低 Ks\n",
                   wn * DT);
    } else {
        printf("plant : rigid single-inertia (Ks not delivered yet, fallback)\n");
    }

    /* ---- 跑三种策略，转速数据顺带写进同一个 CSV ---- */
    fp = fopen("results/sim_data_c.csv", "w");
    if (!fp) { printf("[ERR] cannot open results/sim_data_c.csv\n"); return 1; }
    fprintf(fp, "t_s,strategy,motor,speed_rad_s\n");   /* 长表表头 */

    printf("run master_slave ...\n");       run_master_slave(fp);
    ms[0] = evaluate("master_slave");       /* 必须跑完立刻评分：se_hist 会被下一策略覆盖 */
    printf("run cross_coupling ...\n");     run_cross_coupling(fp);
    ms[1] = evaluate("cross_coupling");
    printf("run deviation_coupling ...\n"); run_deviation_coupling(fp);
    ms[2] = evaluate("deviation_coupling");
    printf("run dcc_dob ...\n");            run_dcc_dob(fp);
    ms[3] = evaluate("dcc_dob");
    fclose(fp);                             /* 转速数据已全部落盘 */

    /* ---- 指标写 CSV ---- */
    fp = fopen("results/metrics_c.csv", "w");
    if (!fp) { printf("[ERR] cannot open results/metrics_c.csv\n"); return 1; }
    fprintf(fp, "strategy,rms_rad_s,peak_rad_s,recover_s,dip_rad_s\n");
    for (i = 0; i < N_STRATEGY; i++)
        fprintf(fp, "%s,%.3f,%.3f,%.3f,%.3f\n",
                ms[i].name, ms[i].rms, ms[i].peak, ms[i].rec, ms[i].dip);
    fclose(fp);

    /* ---- 控制台汇总表 ---- */
    printf("\n===== metrics =====\n");
    printf("%-20s%10s%10s%10s%10s\n", "strategy", "RMS", "peak", "rec(s)", "dip");
    for (i = 0; i < N_STRATEGY; i++)
        printf("%-20s%10.3f%10.3f%10.3f%10.3f\n",
               ms[i].name, ms[i].rms, ms[i].peak, ms[i].rec, ms[i].dip);
    printf("\noutput: results/sim_data_c.csv, results/metrics_c.csv\n");
    return 0;   /* 正常退出 */
}
