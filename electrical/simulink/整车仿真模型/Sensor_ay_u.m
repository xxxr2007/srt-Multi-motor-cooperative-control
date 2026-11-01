function y =Sensor_ay_u(x,u)
m=1765; %整车质量kg 
y=(u(2)*u(1)+u(6))*x(1)/m+(u(3)*u(1)+u(7))*x(2)/m+u(8)*x(3)/m+u(9)*x(4)/m;
end