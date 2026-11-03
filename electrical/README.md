# 电控与正式仿真

| 目录 | 用途 |
|---|---|
| [simulink/整车仿真模型](simulink/整车仿真模型/) | 正式车辆模型、7DOF 对照模型和依赖脚本；路径保持不变 |
| [matlab](matlab/) | 多电机 MATLAB 仿真与参数设置 |
| [c](c/) | 多电机 C / Python 仿真、绘图、验收和参数合并 |
| [src](src/) | 机械参数同步生成的头文件 |

根目录 `start_vehicle.m` 打开 simulink14dof01.slx。模型及依赖脚本在本目录中维护，初始化路径随项目目录解析。旧 simulink14dof.slx 已移除。

源代码、参数输入与生成物归属见 [协作约定](../docs/collaboration.md)。模型修改与报告入口见 [车辆协作操作](../docs/车辆模型协作与报告.md)。
