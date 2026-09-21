#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
merge_params.py —— 把"机械组交付"与"电控组配置"合并成仿真参数
====================================================================
用途
    两组各写各的参数文件，本脚本负责合并 + 校验 + 生成仿真能直接吃的东西：

        输入① data/mechanical/mech_params.json   机械组（惯量/摩擦/联轴器刚度）
        输入② data/electrical/ctrl_params.json   电控组（控制增益/带宽/工况设

        输出① data/params.json                   合并后的总参数（给 Python/MATLAB 脚本）
        输出② build/params_generated.h           给 C 内核的宏定义（编译时生效）

为什么要有这一层
    机械组改惯量、电控组改增益，两边都不碰对方的文件；合并脚本是唯一入口，
    保证"仿真用的参数"永远是一套，不会出现两边对数不上。

运行
    python electrical/c/merge_params.py    （在仓库根目录或任意目录均可，路径按脚本自身位置解析）
"""

import json      # 读写参数文件
import math      # sqrt：算机械谐振频率
import os        # 路径拼接与目录创建
import sys       # 退出码：校验失败时返回非 0，便于 CI / 批处理中断

# ---------------------------------------------------------------- 路径解析
HERE = os.path.dirname(os.path.abspath(__file__))       # .../仓库根/electrical/c（本脚本所在目录）
ROOT = os.path.dirname(os.path.dirname(HERE))            # 再上两级 = 仓库根（c → electrical → 根）
MECH_JSON = os.path.join(ROOT, "data", "mechanical", "mech_params.json")   # 机械组交付
CTRL_JSON = os.path.join(ROOT, "data", "electrical", "ctrl_params.json")   # 电控组配置
MERGED_JSON = os.path.join(ROOT, "data", "params.json")                     # 合并产物
GEN_HEADER = os.path.join(ROOT, "build", "params_generated.h")              # 给 C 的宏


def load_json(path, label):
    """读 JSON；文件不存在或语法错误就给出人话提示并终止。"""
    if not os.path.isfile(path):
        print(f"[ERR] 找不到{label}：{path}")
        sys.exit(2)
    try:
        with open(path, "r", encoding="utf-8") as f:
            return json.load(f)
    except json.JSONDecodeError as e:
        # JSON 最常见的错就是多/少一个逗号、少了引号，直接把行列号报出来
        print(f"[ERR] {label} JSON 语法错误：第 {e.lineno} 行第 {e.colno} 列 —— {e.msg}")
        sys.exit(2)


def cnum(v):
    """把数值格式化成安全的 C 浮点字面量。

    坑：若直接输出 3（而不是 3.0），后续有人写 T_END/2 会变成整数除法 1，
    因此整数值一律补成 "3.0" 形式，从根上避免这类隐蔽 bug。
    """
    v = float(v)
    if v == int(v) and abs(v) < 1e15:
        return f"{int(v)}.0"        # 整数值补 .0
    return f"{v:g}"                 # 其余按 g 格式输出，避免多余小数位


def main():
    mech = load_json(MECH_JSON, "机械组参数 mech_params.json")
    ctrl = load_json(CTRL_JSON, "电控组参数 ctrl_params.json")

    # ------------------------------------------------------------ 取字段
    n_motor = 4                                            # 电机台数（课题固定四台）
    J = list(mech["motor"]["J"])                           # 转动惯量 kg·m^2（机械组给）
    B = list(mech["motor"]["B"])                           # 粘性摩擦 N·m·s/rad（机械组给）

    dt = float(ctrl["sim"]["dt"])                          # 仿真步长 s
    t_end = float(ctrl["sim"]["t_end"])                    # 总时长 s
    out_every = int(ctrl["sim"]["out_every"])              # CSV 降采样步数
    rec_threshold = float(ctrl["sim"]["rec_threshold"])    # 恢复判定阈值
    rec_hold = int(ctrl["sim"]["rec_hold"])                # 恢复保持步数

    torque_limit = float(ctrl["motor_capability"]["torque_limit"])   # 转矩限幅 N·m
    w_star = float(ctrl["motor_capability"]["w_star"])               # 目标转速 rad/s
    ramp_end = float(ctrl["motor_capability"]["ramp_end"])           # 斜坡结束时刻 s
    speed_kp = float(ctrl["speed_loop"]["kp"])                       # 转速环 Kp
    speed_ki = float(ctrl["speed_loop"]["ki"])                       # 转速环 Ki
    sync_kp = float(ctrl["sync_loop"]["kp"])                         # 同步环 Ks
    sync_ki = float(ctrl["sync_loop"]["ki"])                         # 同步环 Ksi
    dob_g = float(ctrl["dob"]["g"])                                  # 观测器带宽 rad/s

    loads = ctrl["loads"]                                            # 三台机的负载工况

    # ------------------------------------------------------------ 校验（拦在仿真之前）
    problems = []      # 致命问题：存在则不生成任何输出
    warnings = []      # 提醒：不阻断，但要让人看见

    if len(J) != n_motor:
        problems.append(f"J 应有 {n_motor} 个值，实际 {len(J)} 个")
    if len(B) != n_motor:
        problems.append(f"B 应有 {n_motor} 个值，实际 {len(B)} 个")
    # 量级合理区间：电机转子惯量通常在 1e-4 ~ 1e-1 kg·m^2 之间
    for i, v in enumerate(J if len(J) == n_motor else []):
        if not (1e-5 <= float(v) <= 1e-1):
            warnings.append(f"J[{i+1}]={v} 超出常见量级 1e-5~1e-1 kg·m^2，请确认单位是 kg·m^2（不是 g·mm^2）")
    # 黏性摩擦同样按量级检查（N·m·s/rad）
    for i, v in enumerate(B if len(B) == n_motor else []):
        if not (1e-6 <= float(v) <= 1e-1):
            warnings.append(f"B[{i+1}]={v} 超出常见量级 1e-6~1e-1 N·m·s/rad，请确认单位")

    # 若机械组按 ISO 风格给了 g·mm^2（常见误填），数值会大 1e7 倍，这里额外兜一层
    if J and max(float(v) for v in J) > 1.0:
        problems.append("J 的数值过大，疑似用了 g·mm^2 而非 kg·m^2，请换算后重填")

    # ---- 弹性传动链（方案 A）：Ks 与谐振频率 ----
    cp = mech.get("coupling", {})
    Ks = cp.get("Ks", None)                 # 扭转刚度 N·m/rad（可能为 null）
    Ds = cp.get("Ds", None)                 # 阻尼
    J1 = cp.get("J1", None)
    J2 = cp.get("J2", None)
    omega_n = None                          # 谐振频率 rad/s

    if Ks is not None:
        Ks = float(Ks)
        if Ks <= 0:
            problems.append("coupling.Ks 必须为正数")
        # J1/J2 缺省时用平均惯量兜底，保证仍能算出谐振频率
        J1v = float(J1) if J1 else (sum(float(v) for v in J) / len(J) if J else 0.01)
        J2v = float(J2) if J2 else J1v
        omega_n = math.sqrt(Ks * (1.0 / J1v + 1.0 / J2v))    # ω_n = √(Ks·(1/J1+1/J2))
        # 核心耦合点检查①：观测器带宽必须覆盖谐振频率的 2 倍以上
        if dob_g < 2.0 * omega_n:
            warnings.append(
                f"机电耦合风险：谐振频率 ω_n={omega_n:.1f} rad/s，而观测器带宽 g={dob_g:.1f} rad/s "
                f"不足其 2 倍 → DOB 可能压不住机械谐振。建议机械换更软的联轴器，或提高 g。"
            )
        # 核心耦合点检查②：显式/半隐式欧拉的仿真精度——每谐振周期至少约 125 步
        if omega_n * dt > 0.05:
            warnings.append(
                f"离散化风险：ω_n·dt = {omega_n * dt:.3g} > 0.05，"
                f"谐振周期内积分步数不足，波形会失真。建议减小 dt 或降低 Ks。"
            )
    else:
        warnings.append("coupling.Ks 尚未填写（方案 A 弹性传动链未启用），当前仿真按刚性直连处理")

    if problems:
        print("\n[校验失败] 以下问题必须修正后才能合并：")
        for p in problems:
            print("  × " + p)
        sys.exit(1)

    # ------------------------------------------------------------ 合并输出：data/params.json
    merged = {
        "meta": {
            "merged_by": "electrical/c/merge_params.py",
            "mech_version": mech["meta"].get("version", "?"),
            "ctrl_version": ctrl["meta"].get("version", "?"),
            "note": "本文件由脚本自动生成，请勿手工编辑；改参数请改 data/mechanical 或 data/electrical 下对应的源文件",
        },
        "n_motor": n_motor,
        "dt": dt,
        "t_end": t_end,
        "J": [float(v) for v in J],
        "B": [float(v) for v in B],
        "torque_limit": torque_limit,
        "speed_pi": {"kp": speed_kp, "ki": speed_ki},
        "sync_pi": {"kp": sync_kp, "ki": sync_ki},
        "dob": {"g": dob_g},
        "w_star": w_star,
        "ramp_end": ramp_end,
        "t_load_step": float(loads["step"]["t"]),
        "tl_step": float(loads["step"]["amp"]),
        "t_load_sin": float(loads["sin"]["t"]),
        "tl_sin_amp": float(loads["sin"]["amp"]),
        "tl_sin_freq": float(loads["sin"]["freq"]),
        "t_load_ramp": float(loads["ramp"]["t_start"]),
        "t_ramp_full": float(loads["ramp"]["t_full"]),
        "tl_ramp": float(loads["ramp"]["amp"]),
        "out_every": out_every,
        "rec_threshold": rec_threshold,
        "rec_hold": rec_hold,
        "mechanical": {
            "version": mech["meta"].get("version", "?"),
            "J1": J1, "J2": J2, "Ks": Ks, "Ds": Ds,
            "omega_n_rad_s": omega_n,
            "perturb_pct": mech["motor"].get("perturb_pct", 10),
        },
    }
    with open(MERGED_JSON, "w", encoding="utf-8") as f:
        json.dump(merged, f, ensure_ascii=False, indent=2)      # ensure_ascii=False：中文原样写入

    # ------------------------------------------------------------ 生成 C 头文件
    os.makedirs(os.path.dirname(GEN_HEADER), exist_ok=True)     # build/ 不存在就建
    n_steps = int(round(t_end / dt))                            # 派生量：总步数（宏必须是整型常量）
    j_list = ", ".join(f"{float(v):.6g}" for v in J)             # 展开成 C 数组字面量
    b_list = ", ".join(f"{float(v):.6g}" for v in B)

    with open(GEN_HEADER, "w", encoding="utf-8", newline="\n") as f:
        f.write(f"""/* ============================================================
 * params_generated.h —— 由 scripts/merge_params.py 自动生成，请勿手工编辑！
 *
 *   机械组版本：{merged['meta']['mech_version']}
 *   电控组版本：{merged['meta']['ctrl_version']}
 *   源文件    ：data/mechanical/mech_params.json + data/electrical/ctrl_params.json
 *
 * 改参数的正确姿势：改上面两个源文件 → 跑 merge_params.py → 重新编译。
 * ============================================================ */

#ifndef PARAMS_GENERATED_H
#define PARAMS_GENERATED_H

/* ---- 机械组交付：电机本体参数 ---- */
#define N_MOTOR        {n_motor}
static const double J_MOTOR[N_MOTOR] = {{ {j_list} }};   /* 转动惯量 kg*m^2（机械组） */
static const double B_MOTOR[N_MOTOR] = {{ {b_list} }};   /* 粘性摩擦 N*m*s/rad（机械组） */

/* ---- 机械组交付：弹性传动链（方案 A 双惯量模型）----
 * Ks<=0 表示机械组尚未交付刚度，C 内核自动退回刚性直连单惯量模型。 */
#define MECH_KS        {cnum(Ks if Ks is not None else 0.0)}          /* 扭转刚度 N*m/rad；0=刚性 */
#define MECH_DS        {cnum(0.0 if Ds is None else float(Ds))}          /* 联轴器阻尼 N*m*s/rad */
#define MECH_JL        {cnum(float(J2) if J2 else (sum(float(v) for v in J) / len(J) if J else 0.01))}          /* 负载侧惯量 kg*m^2（J2） */
#define MECH_OMEGA_N   {cnum(omega_n if omega_n is not None else 0.0)}          /* 谐振频率 rad/s（信息量，内核自算校验） */

/* ---- 电控组配置：仿真设置 ---- */
#define DT             {cnum(dt)}           /* 仿真步长 s */
#define T_END          {cnum(t_end)}          /* 总仿真时长 s */
#define N_STEPS        {n_steps}          /* = T_END / DT，静态数组长度需为整型常量 */
#define OUT_EVERY      {out_every}            /* CSV 降采样步数 */
#define REC_THRESHOLD  {cnum(rec_threshold)}            /* 恢复判定阈值 rad/s */
#define REC_HOLD       {rec_hold}            /* 恢复需保持的步数 */

/* ---- 电控组配置：控制与工况 ---- */
#define TORQUE_LIMIT   {cnum(torque_limit)}            /* 转矩限幅 N*m */
#define SPEED_KP       {cnum(speed_kp)}            /* 转速环 Kp  N*m/(rad/s) */
#define SPEED_KI       {cnum(speed_ki)}           /* 转速环 Ki  N*m/rad */
#define SYNC_KP        {cnum(sync_kp)}            /* 同步补偿比例增益 Ks */
#define SYNC_KI        {cnum(sync_ki)}            /* 同步补偿积分增益 Ksi */
#define DOB_G          {cnum(dob_g)}          /* 扰动观测器带宽 rad/s */

#define W_STAR         {cnum(w_star)}          /* 目标转速 rad/s */
#define RAMP_END       {cnum(ramp_end)}            /* 斜坡升速结束时刻 s */
#define T_LOAD_STEP    {cnum(merged['t_load_step'])}            /* 电机2 突加负载时刻 s */
#define TL_STEP        {cnum(merged['tl_step'])}            /* 电机2 突加负载幅值 N*m */
#define T_LOAD_SIN     {cnum(merged['t_load_sin'])}            /* 电机3 波动负载起始 s */
#define TL_SIN_AMP     {cnum(merged['tl_sin_amp'])}            /* 电机3 波动负载幅值 N*m */
#define TL_SIN_FREQ    {cnum(merged['tl_sin_freq'])}            /* 电机3 波动负载频率 Hz */
#define T_LOAD_RAMP    {cnum(merged['t_load_ramp'])}            /* 电机4 斜坡负载起始 s */
#define T_RAMP_FULL    {cnum(merged['t_ramp_full'])}            /* 电机4 达到满负载的时刻 s */
#define TL_RAMP        {cnum(merged['tl_ramp'])}            /* 电机4 斜坡负载终值 N*m */

#endif /* PARAMS_GENERATED_H */
""")

    # ------------------------------------------------------------ 人看的结果摘要
    print("\n==================== 参数合并完成 ====================")
    print(f"  机械组版本 : {merged['meta']['mech_version']}   （{os.path.relpath(MECH_JSON, ROOT)}）")
    print(f"  电控组版本 : {merged['meta']['ctrl_version']}   （{os.path.relpath(CTRL_JSON, ROOT)}）")
    print("  ---------------------------------------------------")
    print(f"  电机台数   : {n_motor}")
    print(f"  转动惯量 J : {', '.join(f'{float(v):.5g}' for v in J)}  kg*m^2")
    print(f"  粘性摩擦 B : {', '.join(f'{float(v):.5g}' for v in B)}  N*m*s/rad")
    print(f"  转矩限幅   : ±{torque_limit} N*m，目标转速 {w_star} rad/s")
    print(f"  转速环 PI  : Kp={speed_kp}, Ki={speed_ki}")
    print(f"  同步环 PI  : Ks={sync_kp}, Ksi={sync_ki}")
    print(f"  DOB 带宽 g : {dob_g} rad/s")
    if omega_n is not None:
        print(f"  被控对象   : 弹性双惯量（Ks={Ks:.5g} N*m/rad, ω_n={omega_n:.1f} rad/s, g/ω_n={dob_g / omega_n:.2f}）")
    else:
        print("  被控对象   : 刚性单惯量（未交付 Ks，方案 A 弹性链未启用）")
    print("  ---------------------------------------------------")
    print(f"  输出① {os.path.relpath(MERGED_JSON, ROOT)}")
    print(f"  输出② {os.path.relpath(GEN_HEADER, ROOT)}")

    if warnings:
        print("\n  [提醒] 以下问题不阻断合并，但请确认：")
        for w in warnings:
            print("    ! " + w)

    print("\n  下一步：重新编译并运行仿真，然后执行 check_acceptance.py 看是否达标。\n")


if __name__ == "__main__":
    main()
