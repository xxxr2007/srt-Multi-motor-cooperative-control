
function [Yref,Zref,Xref] = road_ref(Vx,time,road_Id)

Xref = Vx*time;
if time<2
    Xref=0;
else
     Xref= Vx*(time-2);
end
z1 = (2.4/50)*(Xref-27.19)-1.2;
z2 = (2.4/43.9)*(Xref-56.46)-1.2;
%%    ********************定义路径形状
% 移除0处的偏移
z1o = (2.4/50)*(0-27.19)-1.2;
z2o = (2.4/43.9)*(0-56.46)-1.2;
% bias=1*(tanh(z1o))+5*sin(z2o);
% 设计路径
switch road_Id
    case 1
        Yref = 10.1/2*(1+tanh(0.5*z1)) - 11.4/2*(1+tanh(0.5*z2))-0.61;%双移线
    case 2
        bias=1*(tanh(z1o));Yref = 1*(tanh(z1))-bias;%单移线
    case 3
        Yref=2*sin(0.05*Xref);
    otherwise
        Yref=1;
end
Zref =0;
end


