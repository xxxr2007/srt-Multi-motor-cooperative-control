clc;
%% 整车参数
m = 1412; %整车质量kg
mb = 1400; %车身质量kg
mw = (m-mb)/4; %轮胎重力kg
Bf = 1.6; %前轴轮距m
Br = 1.6; %后轴轮距m 1.55
Lf = 1.02; %质心距离前轴的距离m 
Lr = 1.89; %质心距离后轴的距离m
L = Lf+Lr; %轴距m
hg = 0.5; %质心距离地面高度m
Iz = 1536.7; %整车z轴转动惯量kg*m^2
I_tire = 2.5; %轮胎转动惯量/ kg*m^2
f = 0.015; %滚动阻力系数
r = 0.354; %车轮滚动半径/ m
i_sw = 20; %转向系传动比
g = 9.8; %重力加速度
Pn_motor = 80; %电机最大功率 Kw
Tn_motor = 1000; %电机最大扭矩 N*m
Tregen_max = 0;

vx0 = 22; %初始Vx速度 m/s
w0 = vx0/r; %初始轮速 rad/s
v_obj = 22; %期望车速 m/s
u = 0.3; %路面附着系数



k1 = -43214*2;%%两前轮侧偏刚度之和 N/rad
k2 = -43214*2;%%两后轮侧偏刚度之和 N/rad
c1 = k1*L/(m*Lr); %侧偏系数 1/rad
c2 = k2*L/(m*Lf);
K = -(1/c1-1/c2)/L; %稳定因数

%% 滑移率控制模块
lamda_ASR = 0.15;  %ASR开启滑移率值
lamda_opt = 0.15; %滑移率控制最优值

%% PID控制器参数
kp_beta = 40000;
ki_beta = 1;
kd_beta = 0;
kp_omega = 40000;
ki_omega = 1;
kd_omega = 0;
Mz_K = 0.05;

%% LQR 参数矩阵计算
v = 85/3.6;%  km/h 转换为 m/s
a11 = (k1+k2)/(m*v);
a12 = ((Lf*k1-Lr*k2)/(m*v*v))-1;
a21 = ((Lf*k1-Lr*k2)/Iz);
a22 = (Lf*Lf*k1+Lr*Lr*k2)/(Iz*v);

A = [a11 a12;a21 a22];
B = [0;1/Iz];
% B = [-k1/(m*v); -Lf*k1/Iz];
Q = [10 0; 0 100000];
R = 0.00002;
K_LQR = lqr(A, B, Q, R);




