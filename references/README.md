# 原始示例与外部教程

| 位置 | 内容 |
|---|---|
| [vehicle-dynamics/Y0377-车辆动力学模型仿真](vehicle-dynamics/Y0377-车辆动力学模型仿真/) | 原始 7/14DOF 示例、建模文档、参考文献及示例结果图 |
| [motor-control/Design-motor-controllers-with-Simscape-Electrical](motor-control/Design-motor-controllers-with-Simscape-Electrical/) | 已有电机控制教程子模块；仓库保留原来的 fork 来源与固定提交 |
| [机械 CAD 参考库](../mechanical/reference/) | 原根目录电机模型和说明资源 |

原始模型与正式开发模型分开放置。开发和提交车辆修改使用 [electrical/simulink/整车仿真模型](../electrical/simulink/整车仿真模型/)，不要把原始资料目录当作主工程。

子模块初始化：`git submodule update --init --recursive`。外部教程在独立仓库维护；本仓库只记录其版本指针。
