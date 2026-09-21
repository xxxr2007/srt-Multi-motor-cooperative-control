%% params_setup.m —— 全项目参数的"单一数据源"装载脚本
%
%  用途：把 data/params.json 里的参数一次性载入 base workspace，Simulink 各模块
%        的增益 / 初值直接填写变量名（如 J1、B2、Tmax、Kp），机械组更新 json 后
%        只需重跑本脚本，全模型参数立刻生效——不需要动任何一个模块。
%
%  运行：MATLAB 命令行执行  params_setup   （工作目录任意，脚本自己定位 json）
%
%  依赖：MATLAB R2016b 及以上（jsondecode 为内置函数）；
%        若 json 不存在或字段缺失，自动回退到内置默认值，保证仿真永远能跑起来。
%
%  与 C 内核的一致性：内置默认值与 scripts/multi_motor_sync.c 完全同口径，
%        便于 "Simulink 结果 ↔ C 结果" 交叉验证。

%% 1. 定位 data/params.json（用脚本自身路径解析，不受当前工作目录影响）
thisDir  = fileparts(mfilename('fullpath'));      % 本脚本所在目录（models/）
jsonPath = fullfile(thisDir, '..', '..', 'data', 'params.json');   % 上两级到仓库根，再进 data/params.json

%% 2. 读取 json；读不到就用内置默认值兜底
if isfile(jsonPath)
    P = jsondecode(fileread(jsonPath));            % jsondecode 返回结构体
    fprintf('[params_setup] 参数来源：%s\n', jsonPath);
else
    warning('[params_setup] 未找到 %s，改用内置默认值', jsonPath);
    P = default_params();                          % 见文件末尾的本地函数
    fprintf('[params_setup] 参数来源：内置默认值（与 C 内核同口径）\n');
end

%% 3. 展开成 Simulink 可直接引用的变量名
%  3.1 仿真设置
n     = P.n_motor;          % 电机台数（4）
dt    = P.dt;               % 仿真步长 s（1e-4 = 0.1 ms）
t_end = P.t_end;            % 总时长 s（3.0）

%  3.2 机械参数（向量 + 单机标量两种形式，Simulink 里按块取用）
J  = P.J(:)';               % 转动惯量 kg·m^2，行向量 [J1 J2 J3 J4]
B  = P.B(:)';               % 粘性摩擦 N·m·s/rad，行向量 [B1 B2 B3 B4]
J  = pad_to4(J);            % 容错：台数不足 4 时用末位补足，避免下面索引越界
B  = pad_to4(B);
J1 = J(1);  J2 = J(2);  J3 = J(3);  J4 = J(4);   % 单机惯量：给"每台电机"的独立 Gain/惯量块用
B1 = B(1);  B2 = B(2);  B3 = B(3);  B4 = B(4);   % 单机阻尼：给各台摩擦项用

Tmax  = P.torque_limit;     % 转矩限幅 N·m（饱和块上限/下限 = ±Tmax）

%  3.3 控制参数
Kp  = P.speed_pi.kp;        % 转速环比例增益 N·m/(rad/s)
Ki  = P.speed_pi.ki;        % 转速环积分增益 N·m/rad
Ks  = P.sync_pi.kp;         % 同步补偿比例增益
Ksi = P.sync_pi.ki;         % 同步补偿积分增益

%  3.4 工况参数
w_star      = P.w_star;         % 目标转速 rad/s（斜坡终值）
ramp_end    = P.ramp_end;       % 斜坡升速结束时刻 s
t_load_step = P.t_load_step;    % 2 号机突加负载时刻 s
TL_step     = P.tl_step;        % 2 号机突加负载幅值 N·m
t_load_sin  = P.t_load_sin;     % 3 号机波动负载起始时刻 s
TL_sin_amp  = P.tl_sin_amp;     % 3 号机波动负载幅值 N·m
TL_sin_freq = P.tl_sin_freq;    % 3 号机波动负载频率 Hz
t_load_ramp = P.t_load_ramp;    % 4 号机斜坡负载起始时刻 s
t_ramp_full = P.t_ramp_full;    % 4 号机加载到满的时刻 s
TL_ramp     = P.tl_ramp;        % 4 号机斜坡负载终值 N·m

%% 4. 写入 base workspace（Simulink 只能看到 base workspace 里的变量）
vars = {'n','dt','t_end','J','B','J1','J2','J3','J4','B1','B2','B3','B4', ...
        'Tmax','Kp','Ki','Ks','Ksi','w_star','ramp_end', ...
        't_load_step','TL_step','t_load_sin','TL_sin_amp','TL_sin_freq', ...
        't_load_ramp','t_ramp_full','TL_ramp'};
for k = 1:numel(vars)
    assignin('base', vars{k}, eval(vars{k}));      % 逐个搬到 base workspace
end

%% 5. 打印摘要，跑完扫一眼就知道参数对不对
fprintf('\n  ── 电机台数 n = %d，步长 dt = %g s，总时长 = %g s\n', n, dt, t_end);
fprintf('  ── 转动惯量 J  = [%s] kg·m²   ← 机械组交接单对应项\n', num2str(J, '%.5g '));
fprintf('  ── 粘性摩擦 B  = [%s] N·m·s/rad\n', num2str(B, '%.5g '));
fprintf('  ── 转矩限幅 ±%g N·m，目标转速 %g rad/s\n', Tmax, w_star);
fprintf('  ── 转速环 PI (Kp=%g, Ki=%g)，同步 PI (Ks=%g, Ksi=%g)\n\n', Kp, Ki, Ks, Ksi);
fprintf('  [提示] 机械组更新 docs/mechanical/plant_model.md 后，\n');
fprintf('         把数值写入 data/params.json，重跑 params_setup 即可全模型生效。\n\n');

%% ===================== 本地函数 =====================
function P = default_params()
% 内置默认参数：与 scripts/multi_motor_sync.c 完全同口径，json 缺失时兜底
    P.n_motor = 4;  P.dt = 1e-4;  P.t_end = 3.0;
    P.J = [0.010, 0.011, 0.0095, 0.0105];     % kg·m²
    P.B = [0.0012, 0.0011, 0.0013, 0.00115];  % N·m·s/rad
    P.torque_limit = 5.0;
    P.speed_pi = struct('kp', 0.8, 'ki', 30.0);
    P.sync_pi  = struct('kp', 0.6, 'ki', 5.0);
    P.w_star = 120.0;  P.ramp_end = 0.5;
    P.t_load_step = 1.2;  P.tl_step = 1.8;
    P.t_load_sin = 2.0;   P.tl_sin_amp = 0.4;  P.tl_sin_freq = 2.0;
    P.t_load_ramp = 1.8;  P.t_ramp_full = 2.6; P.tl_ramp = 1.0;
end

function v = pad_to4(v)
% 把长度不足 4 的参数向量补足到 4（用末位值填充），防止索引越界
    if numel(v) < 4
        v(end+1:4) = v(end);
    end
end
