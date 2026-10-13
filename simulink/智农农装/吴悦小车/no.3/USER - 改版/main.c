/****************************************
motor:       TIM2(PA0 PA1)   TIM3(PA6 PA7)
ultrasound:  TIM5

Tips	光电检测到物体返回0
****************************************/
#include "stm32f10x.h"
#include "oled.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "uart.h"
#include "stdio.h"
#include "motor.h"
#include "ultrasound.h"
#include "lightswitch.h"
#include "pid.h"	//					

#define Distance_Target1 120//左边垄边
#define Distance_Target2 120//左边垄边
#define Distance_Target3 110//左边垄边90
#define speed 50
#define speed1 45

int Distance6_Show=800;
int Distance2_Show=800;
int Distance3_Show=800;
int Distance_difference;

#define angle_90 -90
float pitch,roll,yaw; 		//欧拉角
short aacx,aacy,aacz;		  //加速度传感器原始数据
short gyrox,gyroy,gyroz;	//陀螺仪原始数据
//short temp;								//温度	

int error_now=0;
int error_sum=0;
int error_last=0;

float Angle_P=1.0;        //2
float Angle_I=0.01;     //0.4
float Angle_D=0.5;     

int error_distance_now=0;
int error_distance_sum=0;
int error_distance_last=0;

float Distance_P=0.24;
float Distance_I=0.02;
float Distance_D=0.15;

float speed_real=0;
float speed_real2=0;

int pid_switch=1;   //角度pid开关
int furrow=1;

char Receive_Date;


long int Tick=0;//！
u32 uwTick=0;

int i=0;



void test1()
{
	Motor(speed,speed);//直行
	Motor1(speed,speed);
}
void test2()
{
	Motor(0-speed_real+speed,speed_real+speed);//直行
	Motor1(0-speed_real+speed,speed_real+speed);
}
void test3()
{
//	Motor(-speed_real*0.3+20*(1-speed_real/18*0)+speed_real2*0,speed_real*0.3+20*(1-speed_real/18*0)-speed_real2*0);//超声波：右平移 		+ - 右移
//	Motor1(-speed_real*0.3+20*(1-speed_real/18*0)-speed_real2*0,speed_real*0.3+20*(1-speed_real/18*0)+speed_real2*0);//
	Motor(-speed_real*0.2+speed1*(1-speed_real/18*0)*1+speed_real2*1.5,speed_real*0.2*1+speed1*(1-speed_real/18*0)*1-speed_real2*1.5);//超声波：右平移 		+ - 右移
	Motor1(-speed_real*0.2+speed1*(1-speed_real/18*0)*1-speed_real2*1.5,speed_real*0.2*1+speed1*(1-speed_real/18*0)*1+speed_real2*1.5);//									- +					
}

void test4()
{
//	Motor(0-speed_real+70-speed_real2,speed_real+70+speed_real2);//	超声波：左平移	- + 左移
//	Motor1(0-speed_real+70+speed_real2,speed_real+70-speed_real2);//								+ -
	Motor(-speed_real*0.2+speed*(1-speed_real/18*0)*1-speed_real2*1.5,speed_real*0.2*1+speed*(1-speed_real/18*0)*1+speed_real2*1.5);//超声波：右平移 		+ - 右移
	Motor1(-speed_real*0.2+speed*(1-speed_real/18*0)*1+speed_real2*1.5,speed_real*0.2*1+speed*(1-speed_real/18*0)*1-speed_real2*1.5);//	
}


void test5()
{
	Motor(0-speed_real+60,speed_real-60);//右平移
	Motor1(0-speed_real-60,speed_real+60);
}

void test6()
{
	Motor(0-speed_real*0.2-speed-speed_real2*1.5,speed_real*0.2-speed+speed_real2*1.5);//后退
	Motor1(0-speed_real*0.2-speed+speed_real2*1.5,speed_real*0.2-speed-speed_real2*1.5);
}

void test7()
{
	Motor(0-speed_real+60,speed_real-60);//右平移
	Motor1(0-speed_real-60,speed_real+60);
}

//void test8()
//{
//	Motor(speed_real-60,speed_real+60);
//	Motor1(0-speed_real+60,0-speed_real-60);
//}
void test9()
{
	Motor(0-speed_real*0.2-speed,speed_real*0.2-speed);//后退
	Motor1(0-speed_real*0.2-speed,speed_real*0.2-speed);
}



void InitAll()
{
	Timer2_PWM_Init();
	Timer3_PWM_Init();
	Motor_Polarity_Init();
	
	Ultrasound_Init();
	Lightswitch_Init();
//	Timer4_Int_Init();
	
	delay_init();				       //延时初始化
	OLED_Init();
	OLED_ColorTurn(0);         //0正常显示，1 反色显示！！
	OLED_DisplayTurn(0);       //0正常显示 1 屏幕翻转显示
	MPU_Init();					       //初始化MPU6050
	USART1_Init();///////////////////////////////////////////////////////////////////////////////

	OLED_ShowString(15,0,"Waiting",24,1);
	OLED_ShowString(0,28,"Initialise",24,1);
	OLED_Refresh();
		
}

void Furrow1()
{
	Angle_P=1.2;        //1.5 0.125
	Angle_I=0.04;     //0.02  0.045  0.09
	Angle_D=0;  	//0.15
   
	Distance_P=0.65;		//0.4 0.65
	Distance_I=0;		//0.05
	Distance_D=0;
	
	while((Distance6_Show>100)||(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0))	//前方光电（检测到物体返回0）和2：前方超声波
	{	
		if (pid_switch)		//pid_switch=1
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)	//得到dmp处理后的数据(注意,本函数需要比较多堆栈,局部变量有点多)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据		
			 }
					
			error_now = 0-yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;	//速度位置！！
			
			if (speed_real>20)
				speed_real=20;
			if (speed_real<-20)
				speed_real=-20;
			error_last=error_now;
		}
		
		Distance2_Show=Ultrasound_Getdistance2();		//函数功能:1超声波（左侧）获取距离	
		Distance6_Show=Ultrasound_Getdistance6();		//前面
		
		error_distance_now = Distance_Target3 - Distance2_Show;		//左
		error_distance_sum = error_distance_sum + error_distance_now;
		error_distance_last=error_distance_now-error_distance_last;
		
		if (error_distance_sum>400)
			error_distance_sum=400;
		if (error_distance_sum<-400)
			error_distance_sum=-400;
	
		speed_real2 = Distance_P * error_distance_now + Distance_I * error_distance_sum+Distance_D*error_distance_last;		//速度
		error_distance_last=error_distance_now;
		
		OLED_ShowNum(50, 0,Distance6_Show,5,16,1);
		OLED_ShowFloat(50,15,speed_real,5,16,1);
		OLED_ShowFloat(50,31,yaw,5,16,1);
		OLED_ShowFloat(50,47,speed_real2,5,16,1);
		OLED_Refresh();
		
		if (speed_real2>30)
			speed_real2=30;
		if (speed_real2<-30)
			speed_real2=-30;
		test3();		//s2：+-
	}
	
	Motor(-60,-60);
	Motor1(-60,-60);
	delay_ms(100);			//
	
	Motor(50,-50);//右平移
	Motor1(-50,50);
	delay_ms(100);			//
	
	Motor_Polarity_Stop();	
	delay_ms(100);
}

void revolve()
{
	while(yaw>-73)	
	{	
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据		
			}
					
			error_now = angle_90 -yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			if (error_sum>500)
				error_sum=500;
			if (error_sum<-500)
				error_sum=-500;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			
			if (speed_real>40)
				speed_real=40;
			if (speed_real<-40)
				speed_real=-40;
			error_last=error_now;		
		}
	Motor(0-speed_real+50,speed_real-50);
	Motor1(0-speed_real+50,speed_real-50);

	}
	Motor(0-40,40);	//右转	
	Motor1(0-40,40);
	delay_ms(200);
	
	Motor_Polarity_Stop();	
	delay_ms(120);
}

void Furrow2()
{
	Distance6_Show=310;
		
	error_distance_now=0;
	error_distance_sum=0;
	Angle_P=1.8;		//1.5 0.4
	Angle_I=0.04;		//
	Angle_D=0;		//0.1??
	
	Distance_P=0.65;	//0.4 0.2
	Distance_I=0;	//0.06
	Distance_D=0;

	
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)||GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11))		//右侧光电  未进垄道，往前走
	{
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据		
			}
					
			error_now = angle_90-yaw;	//-90   angle_90
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			
			if (speed_real>18)
				speed_real=18;
			if (speed_real<-18)
				speed_real=-18;
			error_last=error_now;		
		}
		test2();			
	}
//	delay_ms(5);
//	uwTick=0;
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0)||(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0)||Distance6_Show>300)		//右侧光电 检测到垄道
	{		
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据		
			}
					
			error_now = angle_90-yaw;			
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
				speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
		
			if (speed_real>18)
				speed_real=18;
			if (speed_real<-18)
				speed_real=-18;
			error_last=error_now;		
		}
		Distance3_Show=Ultrasound_Getdistance3();		//3右侧超声波
		Distance2_Show=Ultrasound_Getdistance2();		//2左侧超声波
		Distance6_Show=Ultrasound_Getdistance6();
		Distance_difference=Distance2_Show-Distance3_Show;	//2
		error_distance_now = Distance_difference;
		error_distance_sum = error_distance_sum + error_distance_now;
		error_distance_last=error_distance_now-error_distance_last;		//
		
		if (error_distance_sum>400)
			error_distance_sum=400;
		if (error_distance_sum<-400)
			error_distance_sum=-400;
	
		speed_real2 = Distance_P * error_distance_now + Distance_I * error_distance_sum+ Distance_D * error_distance_last ;
		error_distance_last=error_distance_now;

		OLED_ShowNum(50, 0,Distance3_Show,5,16,1);
		OLED_ShowFloat(50,15,speed_real,5,16,1);
		OLED_ShowFloat(50,31,yaw,5,16,1);
		OLED_ShowNum(50,47,Distance2_Show,5,16,1);
		OLED_Refresh();

		if (speed_real2>20)
			speed_real2=20;
		if (speed_real2<-20)
			speed_real2=-20;
		test4();			//前进  s2:-+
	}

	Motor(-62,-62);
	Motor1(-62,-62);
	delay_ms(180);
	
	Motor_Polarity_Stop();
	delay_ms(120);

}

void Furrow3()
{
	error_distance_now=0;
	error_distance_sum=0;
	
	Angle_P=0.8;	//1.0
	Angle_I=0;
	Angle_D=0;
	
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12))//后面
	{
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据		
			}
					
			error_now = angle_90-yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			
			if (speed_real>18)
				speed_real=18;
			if (speed_real<-18)
				speed_real=-18;
			error_last=error_now;
		}
		test5();//平移
	}
	delay_ms(1);
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0))//后面
	{
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据		
			}
					
			error_now = angle_90-yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			
			if (speed_real>18)
				speed_real=18;
			if (speed_real<-18)
				speed_real=-18;
			error_last=error_now;
		}
		test5();
	}
//	Motor(+50,-50);//右移
//	Motor1(-50,+50);
//	delay_ms(50);

	Motor_Polarity_Stop();
	delay_ms(120);
}

void Furrow4()
{	
	error_distance_now=0;
	error_distance_sum=0;
	
	Angle_P=2.1;		//1.5 2.0 0.25
	Angle_I=0.04;//0.02 0.2
	Angle_D=0;		//
	
	Distance_P=0.8;	//0.8
	Distance_I=0;	//
	Distance_D=0;
	
	uwTick=0;
	
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4))||(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)))		//右侧光电未检测到垄道	！
	{
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据		
			}
					
			error_now = angle_90-yaw;			//
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			
			if (speed_real>18)
				speed_real=18;
			if (speed_real<-18)
				speed_real=-18;
			error_last=error_now;
		}
		test9();//后退
	}
	delay_ms(5);
	uwTick=0;
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0)||GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0)	//右侧光电检测到垄道
	{
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据		
			}
					
			error_now = angle_90-yaw;			//
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
				speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
		
			if (speed_real>18)
				speed_real=18;
			if (speed_real<-18)
				speed_real=-18;
			error_last=error_now;
		}
		
		Distance2_Show=Ultrasound_Getdistance2();
		Distance3_Show=Ultrasound_Getdistance3();			//右侧
		Distance_difference=Distance2_Show-Distance3_Show;
		error_distance_now = Distance_difference;
		error_distance_sum = error_distance_sum + error_distance_now;
		error_distance_last=error_distance_now-error_distance_last;			//????反了吗
		
		if (error_distance_sum>400)
			error_distance_sum=400;
		if (error_distance_sum<-400)
			error_distance_sum=-400;
		speed_real2 = Distance_P * error_distance_now + Distance_I * error_distance_sum+Distance_D*error_distance_last;
		error_distance_last=error_distance_now;
	
		if (speed_real2>30)
			speed_real2=30;
		if (speed_real2<-30)
			speed_real2=-30;
		
		test6();//后退		s2:-+
	}
	Motor(+55,+55);
	Motor1(+55,+55);
	delay_ms(100);
	
	Motor_Polarity_Stop();
	delay_ms(100);

	Angle_D=0;
}

void Furrow5()
{

	error_distance_now=0;
	error_distance_sum=0;
	Angle_P=1.0;
	Angle_I=0.01;
	Angle_D=0;
	
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==1))		//前方光电未检测到垄道
	{
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据		
			}
					
			error_now = angle_90-yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			
			if (speed_real>18)
				speed_real=18;
			if (speed_real<-18)
				speed_real=-18;
			error_last=error_now;
		}	
		test7();//平移
	}
	delay_ms(1);
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0))		//前方光电检测到垄道
	{
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据		
			}
					
			error_now = angle_90-yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			
			if (speed_real>18)
				speed_real=18;
			if (speed_real<-18)
				speed_real=-18;
			error_last=error_now;
		}	
		test7();
	}

	Motor(50,-50);		//右平移
	Motor1(-50,50);
	delay_ms(50);
	
	Motor_Polarity_Stop();	
	delay_ms(150);
}


void Furrow_last()
{	
	Distance6_Show=200;
	Distance2_Show=200;
	
	error_distance_now=0;
	error_distance_sum=0;
	
	Angle_P=1.5;		//2.5
	Angle_I=0.03;
	Angle_D=0;
	
	Distance_P=0.65;	//0.4
	Distance_I=0;
	Distance_D=0;
	
		
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0)||(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0))			//右侧	
	{
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);		
			}
					
			error_now = angle_90-yaw;		//angle_90
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			
			if (speed_real>18)
				speed_real=18;
			if (speed_real<-18)
				speed_real=-18;
			error_last=error_now;
		}
		test9();//后退	
	}
	
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==1)||(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0)||(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0))			//右侧
	{
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);		
			}
					
			error_now = angle_90-yaw;		//angle_90
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			
			if (speed_real>18)
				speed_real=18;
			if (speed_real<-18)
				speed_real=-18;
			error_last=error_now;
		}
//		Distance3_Show=Ultrasound_Getdistance3();			//右侧
		Distance2_Show=Ultrasound_Getdistance2();			//左侧
		Distance_difference=Distance2_Show-Distance_Target1;
		error_distance_now =  Distance_difference;
		error_distance_sum = error_distance_sum + error_distance_now;
		error_distance_last=error_distance_now-error_distance_last;			
		
		if (error_distance_sum>400)
			error_distance_sum=400;
		if (error_distance_sum<-400)
			error_distance_sum=-400;
	
		speed_real2 = Distance_P * error_distance_now + Distance_I * error_distance_sum+Distance_D*error_distance_last;
		error_distance_last=error_distance_now;

		if (speed_real2>30)
			speed_real2=30;
		if (speed_real2<-30)
			speed_real2=-30;
		
		test6();//后退 s2:-+
	}
	Motor(50,50);
	Motor1(50,50);
	delay_ms(50);
	
	Motor_Polarity_Stop();
	delay_ms(100);

}


void USART1_SendChar(char c)
{
    while (!(USART1->SR & 0x0080));  // 等待上一次发送的数据传输完成
    USART1->DR = (c & 0xFF);
}




int main(void)
{	 
	InitAll();		
//	while(1)
//	{
//		Serial_SendByte(0x41);
//	}		
	
	while(mpu_dmp_init())		//返回值:0,正常
 	{
//		OLED_Clear();
//		OLED_Refresh();
	}
	
	OLED_Clear();
	OLED_ShowString(35,20,"RUN",24,1);
	OLED_Refresh();
	
	
	OLED_Clear();
	OLED_ShowString(0,0,"DIST2:",16,1);
	OLED_ShowString(0,15,"REAL:",16,1);
	OLED_ShowString(0,31,"YAW:",16,1);
	OLED_ShowString(0,47,"TEAL:",16,1);
	OLED_Refresh();
/**********************************************************************/
//	Serial_SendByte('A');
/*****************************************正文****************************************/
	for(i=0;i<10;i++)//没有围栏
	{
			Angle_P=1.0;        //2
			Angle_I=0.00;     //0.4
			Angle_D=0;     	
	
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{						  
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);		
		}
		error_now = 0-yaw;
		error_sum = error_sum+error_now;
		error_last = error_now - error_last;
			if (error_sum>300)
				error_sum=300;
			if (error_sum<-300)
				error_sum=-300;
		speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
		
			if (speed_real>30)
				speed_real=30;
			if (speed_real<-30)
				speed_real=-30;
		error_last=error_now;
			
		Motor(0-speed_real+30,speed_real+30);    //直走
		Motor1(0-speed_real+30,speed_real+30);
			
		OLED_ShowNum(50, 0,Distance6_Show,5,16,1);
		OLED_ShowFloat(50,15,speed_real,5,16,1);
		OLED_ShowFloat(50,31,yaw,5,16,1);
		OLED_ShowNum(50,47,speed_real2,5,16,1);
		OLED_Refresh();
	}
	
		Furrow1();
		revolve();
	
		Furrow2();
		Furrow3();
		Furrow4();
		Furrow5();
		
		Furrow2();
		Furrow3();
		Furrow4();
		Furrow5();
		
		Furrow2();			
////		USART_Cmd(USART1,DISABLE);	
		Furrow3();
		Furrow_last();
	
//		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5))			//前方光电未检测到   1
//		{
//			Angle_P=1.5;
//			Angle_I=0.02;
//		  Angle_D=0;
//				if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
//				{						  
//					MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);		
//				}
//						
//				error_now = 0-yaw;//angle_90
//				error_sum = error_sum+error_now;
//				error_last = error_now - error_last;
//				if (error_sum>200)
//					error_sum=200;
//				if (error_sum<-200)
//					error_sum=-200;
//				speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
//				
//				if (speed_real>18)
//					speed_real=18;
//				if (speed_real<-18)
//					speed_real=-18;
//				error_last=error_now;
//				
//				test5();						
//		}
		Motor(60,-60);//右移
		Motor1(-60,60);
		delay_ms(1000);
		Motor_Polarity_Stop();	
		while(1);
}




unsigned int seed_flag=0;


void USART1_IRQHandler(void)///////////////////////////////串口1中断函数
{											
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
	{		
		Receive_Date = USART_ReceiveData(USART1);
		
		if(Receive_Date==49)
		{
			GPIO_SetBits(GPIOC,GPIO_Pin_8);
			GPIO_SetBits(GPIOC,GPIO_Pin_14);
			TIM_Cmd(TIM7,ENABLE );
		}
		else if(Receive_Date==52)
		{
			GPIO_SetBits(GPIOC,GPIO_Pin_8);
			TIM_Cmd(TIM7,ENABLE );
		}
		OLED_Clear();
		OLED_ShowString(40,20,"STOP",24,1);
		OLED_ShowNum(50, 0,uwTick,1,16,1);
		OLED_Refresh();
		
//		if (furrow == 2)
//		{
//			Motor(40,40);
//			Motor1(40,40);
//			delay_ms(200);	
//		}
//		if (furrow == 4)
//		{
//				Motor(-30,-30);
//				Motor1(-30,-30);
//				delay_ms(200);
//		}
//		Motor_Polarity_Stop();
		
//		while(Receive_Date!='g')
//		{			
//			USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//			Receive_Date = USART_ReceiveData(USART1);
//		}
		uwTick=0;
		OLED_Clear();
		OLED_ShowString(35,20,"RUN",24,1);
		OLED_Refresh();

		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}


/*******************************************

SOLIDWORKS文件，PCB文件，OpenMv代码

更多资料联系QQ:2231821988，欢迎交流

自动化 刘威 祝：

比赛顺利！！

*******************************************/

