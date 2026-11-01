function y =Sensor_ax_u(x,u)
m=1765; %整车质量kg
y=(u(2)-u(6)*u(1))*x(1)/m+(u(3)-u(7)*u(1))*x(2)/m+u(4)*x(3)/m+u(5)*x(4)/m; 
end