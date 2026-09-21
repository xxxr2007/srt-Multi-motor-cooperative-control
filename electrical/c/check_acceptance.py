#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
check_acceptance.py —— 按验收规则判定仿真效果是否达标
====================================================================
用途
    读取仿真输出 results/metrics_c.csv，对照 data/acceptance.json 里的规则，
    逐项判定 PASS / FAIL，并给出总体结论。回答的是那个关键问题：
        "这个效果，到底压到什么地步才算过？"

判定四类规则
    ① 绝对阈值   ：RMS / 峰值 / 恢复时间 / 跌落 各自的上限
    ② 相对提升   ：目标策略必须比最强经典策略再降至少 X%（证明创新点有效）
    ③ 不恶化条款 ：任一项指标不得比上一轮差 5% 以上
    ④ 迭代停止   ：连续两轮改进 <5%，判定收敛，停止迭代

运行
    python electrical/c/check_acceptance.py                 # 只看判定
    python electrical/c/check_acceptance.py --record        # 判定并把本轮结果记入迭代历史
"""

import argparse        # 解析命令行参数
import csv             # 读 metrics CSV
import json            # 读验收规则
import os              # 路径处理
import sys             # 退出码

HERE = os.path.dirname(os.path.abspath(__file__))     # .../仓库根/electrical/c（本脚本所在目录）
ROOT = os.path.dirname(os.path.dirname(HERE))          # 再上两级 = 仓库根（c → electrical → 根）

METRICS_DEFAULT = os.path.join(ROOT, "results", "metrics_c.csv")        # 默认指标文件
CONFIG_DEFAULT = os.path.join(ROOT, "data", "acceptance.json")          # 默认规则文件
HISTORY = os.path.join(ROOT, "results", "iter_history.csv")             # 迭代历史（自动生成）


def read_metrics(path):
    """读指标 CSV -> {策略名: {指标: 值}}；文件缺失直接给出可操作的提示。"""
    if not os.path.isfile(path):
        print(f"[ERR] 找不到指标文件：{path}")
        print("      请先编译运行仿真：gcc -O2 -o build/multi_motor_sync.exe "
              "electrical/c/multi_motor_sync.c -lm（在仓库根目录执行该 exe）")
        sys.exit(2)
    table = {}
    with open(path, "r", encoding="utf-8") as f:
        for row in csv.DictReader(f):                       # 首行是表头，按名字取值更抗列序变化
            name = row["strategy"]
            table[name] = {k: float(v) for k, v in row.items() if k != "strategy"}
    return table


def verdict(ok):
    """把布尔判定转成终端里一眼能看到的标记。"""
    return "PASS" if ok else "FAIL"


def main():
    ap = argparse.ArgumentParser(description="按验收规则判定仿真效果是否达标")
    ap.add_argument("--metrics", default=METRICS_DEFAULT, help="指标 CSV 路径")
    ap.add_argument("--config", default=CONFIG_DEFAULT, help="验收规则 JSON 路径")
    ap.add_argument("--record", action="store_true", help="把本轮结果记入迭代历史，用于停止条件判定")
    args = ap.parse_args()

    with open(args.config, "r", encoding="utf-8") as f:
        cfg = json.load(f)                                  # 验收规则
    metrics = read_metrics(args.metrics)                    # 本轮仿真指标

    target = cfg["meta"]["target_strategy"]                 # 目标策略（通常是 dcc_dob）
    baseline = cfg["meta"]["baseline_strategy"]             # 对照的经典策略
    if target not in metrics:
        print(f"[ERR] 指标表里没有目标策略 {target}，实际有：{list(metrics)}")
        sys.exit(2)

    all_pass = True                                         # 总体结论（任一 FAIL 即 False）

    # ---------------------------------------------------------- ① 绝对阈值
    print("\n==================== 验收判定 ====================")
    print(f"目标策略：{target}    对照策略：{baseline}    规则版本：{cfg['meta']['version']}")
    print("\n① 绝对阈值")
    print(f"  {'指标':<28}{'实测':>10}{'达标线':>12}   结果")
    print("  " + "-" * 60)
    for key, rule in cfg["thresholds"].items():
        val = metrics[target].get(key)                      # 实测值
        ok = (val is not None) and (val <= rule["max"])      # 越小越好，故用 <=
        all_pass = all_pass and ok
        print(f"  {key:<28}{val:>10.3f}{'≤ ' + str(rule['max']):>12}   {verdict(ok)}")
        print(f"      ({rule['desc']})")

    # ---------------------------------------------------------- ② 相对提升
    if cfg["relative"]["enabled"]:
        rel = cfg["relative"]
        m = rel["metric"]
        base_val = metrics[baseline][m]                     # 对照策略的同一指标
        tgt_val = metrics[target][m]
        improve = (base_val - tgt_val) / base_val * 100.0    # 相对提升百分比
        ok = improve >= rel["min_improve_pct"]
        all_pass = all_pass and ok
        print(f"\n② 相对提升（{m}，对照 {baseline}）")
        print(f"  对照 {base_val:.3f} → 目标 {tgt_val:.3f}，提升 {improve:.1f}%"
              f"，要求 ≥ {rel['min_improve_pct']}%   {verdict(ok)}")

    # ---------------------------------------------------------- ③ 不恶化条款
    if cfg["no_regression"]["enabled"]:
        nr = cfg["no_regression"]
        prev_path = os.path.join(ROOT, nr["prev_metrics_file"])
        print("\n③ 不恶化条款")
        if os.path.isfile(prev_path):
            prev = read_metrics(prev_path).get(target, {})
            worst_ok = True                                 # 是否所有指标都没恶化
            for key in cfg["thresholds"]:
                if key in prev:
                    delta_pct = (metrics[target][key] - prev[key]) / prev[key] * 100.0 if prev[key] else 0.0
                    ok = delta_pct <= nr["tol_pct"]          # 恶化超过容差即 FAIL
                    worst_ok = worst_ok and ok
                    print(f"  {key:<28}上一轮 {prev[key]:.3f} → 本轮 {metrics[target][key]:.3f}"
                          f"（{delta_pct:+.1f}%）   {verdict(ok)}")
            all_pass = all_pass and worst_ok
        else:
            print(f"  尚无上一轮记录（{os.path.relpath(prev_path, ROOT)} 不存在），本轮跳过；")
            print("  下一轮起请把上一轮的 metrics_c.csv 另存为 results/metrics_prev.csv 再比较。")

    # ---------------------------------------------------------- ④ 迭代停止条件
    st = cfg["stop_rule"]
    print("\n④ 迭代停止条件")
    history = []
    if os.path.isfile(HISTORY):
        with open(HISTORY, "r", encoding="utf-8") as f:
            history = list(csv.DictReader(f))               # 历次迭代的 RMS 记录
    if args.record:
        # 把本轮目标策略的 RMS 追加进历史，供后续轮次判断是否已收敛
        with open(HISTORY, "a", encoding="utf-8", newline="") as f:
            w = csv.writer(f)
            if not history:
                w.writerow(["round", "rms_rad_s"])           # 首次写入补表头
            w.writerow([len(history) + 1, f"{metrics[target]['rms_rad_s']:.4f}"])
        print(f"  已记录本轮结果到 {os.path.relpath(HISTORY, ROOT)}（第 {len(history) + 1} 轮）")
        history.append({"rms_rad_s": str(metrics[target]["rms_rad_s"])})

    if len(history) >= st["consecutive_rounds"] + 1:
        # 取最近 (N+1) 轮，逐一算改进百分比，看是否连续 N 轮都低于阈值
        vals = [float(h["rms_rad_s"]) for h in history[-(st["consecutive_rounds"] + 1):]]
        improves = [(vals[i] - vals[i + 1]) / vals[i] * 100.0 for i in range(len(vals) - 1)]
        converged = all(p < st["improve_below_pct"] for p in improves)
        print(f"  最近 {len(improves)} 轮改进幅度：" + "、".join(f"{p:.1f}%" for p in improves))
        print(f"  连续 {st['consecutive_rounds']} 轮改进 < {st['improve_below_pct']}%"
              f" → {'已收敛，可冻结参数与算法' if converged else '尚未收敛，继续迭代'}")
    else:
        print(f"  历史轮次不足（当前 {len(history)} 轮），需 ≥ {st['consecutive_rounds'] + 1} 轮才判定；")
        print("  每轮加 --record 参数即可累计。")

    # ---------------------------------------------------------- 总体结论
    print("\n" + "=" * 50)
    print(f"总体判定：{verdict(all_pass)}" + ("  —— 效果达标，可冻结参数进入下一阶段"
                                             if all_pass else "  —— 未达标，继续迭代（先调电控，压不住再动机械）"))
    print("=" * 50 + "\n")
    sys.exit(0 if all_pass else 1)


if __name__ == "__main__":
    main()
