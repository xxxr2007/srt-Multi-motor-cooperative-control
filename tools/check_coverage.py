# -*- coding: utf-8 -*-
"""
覆盖一致性校验：确认 three_month_plan.md §三 列出的「理论课 + 工具」都在
weekly_tasks.md 中出现（被排进周表）。每次改两份文档后跑一次：

    python tools/check_coverage.py

缺失即漏排，补进 weekly_tasks 系统课 + 附三（并同步 three_month_plan §三）。
"""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
WT = os.path.join(ROOT, "docs", "weekly_tasks", "weekly_tasks.md")

# (标签, [在 weekly_tasks 里任一出现即算覆盖的别名])
REQUIRED = [
    ("自动控制原理", ["自动控制原理"]),
    ("电机与拖动", ["电机与拖动", "电机学"]),
    ("双惯量/dq", ["双惯量", "dq 变换", "dq变换"]),
    ("信号与系统", ["信号与系统"]),
    ("控制进阶", ["控制进阶", "状态空间", "Bode", "DOB", "陷波器"]),
    ("线性代数", ["线性代数"]),
    ("数值仿真常识", ["数值仿真常识"]),
    ("误差/摄动", ["摄动", "鲁棒性"]),
    ("机械振动", ["机械振动"]),
    ("系统辨识", ["系统辨识"]),
    ("仿真指标", ["仿真指标"]),
    ("离散化PI", ["离散化 PI", "离散化PI"]),
    ("Python数据处理", ["Python 数据处理", "numpy"]),
    ("文献精读", ["文献精读"]),
    ("机械原理", ["机械原理"]),
    ("有限元FEA", ["有限元", "FEA", "ANSYS"]),
    ("文献检索+科技写作", ["文献检索", "科技写作"]),
    ("论文写作结构", ["论文写作结构", "论文写作"]),
    ("多电机同步综述", ["多电机同步控制综述", "多电机同步"]),
    ("根轨迹", ["根轨迹"]),
    ("实验设计DOE", ["实验设计", "DOE"]),
    ("传动误差/背隙", ["传动误差", "背隙", "backlash"]),
    ("摩擦模型", ["摩擦模型", "Stribeck", "LuGre"]),
    ("Zotero", ["Zotero"]),
    ("制图规范", ["制图规范", "draw.io"]),
    ("电路分析", ["电路分析", "基尔霍夫"]),
    ("电力电子", ["电力电子"]),
    ("复变函数", ["复变函数", "拉氏变换"]),
    ("高等数学", ["高等数学"]),
    ("离散数学", ["离散数学"]),
    ("数值分析", ["数值分析"]),
    ("理论力学", ["理论力学"]),
    ("材料力学", ["材料力学"]),
    ("机械设计", ["机械设计"]),
    ("工程制图", ["工程制图"]),
    ("SolidWorks", ["SolidWorks"]),
    ("公差配合/互换性", ["公差", "互换性"]),
    ("机械制造技术基础", ["机械制造技术基础", "车铣刨磨", "定位夹紧"]),
    ("工程材料", ["工程材料", "热处理"]),
    ("电工电子技术", ["电工电子技术", "电工电子"]),
    ("Git+SourceGit", ["Git", "SourceGit"]),
    ("MATLAB/Simulink", ["MATLAB", "Simulink"]),
    ("CATIA", ["CATIA"]),
    ("VS Code", ["VS Code"]),
    ("C语言", ["C 语言"]),
    ("Python(Anaconda)", ["Python（Anaconda）", "Anaconda", "Python 环境"]),
    ("draw.io/Excalidraw", ["draw.io", "Excalidraw"]),
    ("Jupyter", ["Jupyter"]),
    ("STM32+示波器", ["STM32", "示波器"]),
]


def main():
    if not os.path.exists(WT):
        print("找不到 weekly_tasks.md:", WT)
        sys.exit(2)
    text = open(WT, encoding="utf-8").read()
    missing = []
    for label, aliases in REQUIRED:
        if not any(a in text for a in aliases):
            missing.append((label, aliases))
    total = len(REQUIRED)
    covered = total - len(missing)
    print("覆盖校验：%d / %d 项已排进周表" % (covered, total))
    if missing:
        print("\n⚠️  以下学习项在 weekly_tasks 中未找到（可能漏排）：")
        for label, aliases in missing:
            print("  - %s  (期望别名: %s)" % (label, " / ".join(aliases)))
        sys.exit(1)
    print("✅ 全覆盖，无漏项。")
    sys.exit(0)


if __name__ == "__main__":
    main()
