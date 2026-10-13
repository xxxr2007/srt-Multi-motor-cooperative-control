/****************************************

ä½œå“åç§°ï¼šè·¨åž„è½¦

motor:       TIM2(PA0 PA1)   TIM3(PA6 PA7)
ultrasound:  TIM5

Tipsï¼šå…‰ç”µæ£€æµ‹åˆ°ç‰©ä½“è¿”å›ž0

****************************************/
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
#include "pid.h"

#define Distance_Target 87

float pitch,roll,yaw; 		//Å·À­½Ç
short aacx,aacy,aacz;		  //¼ÓËÙ¶È´«¸ÐÆ÷Ô­Ê¼Êý¾Ý
short gyrox,gyroy,gyroz;	//ÍÓÂÝÒÇÔ­Ê¼Êý¾Ý
//short temp;								//ÎÂ¶È	

int error_now=0;
int error_sum=0;
int error_last=0;

float Angle_P=2.0;        //2
float Angle_I=0.03;     //0.4
float Angle_D=0.5;     

int error_distance_now=0;
int error_distance_sum=0;


float Distance_P=0.24;
float Distance_I=0.02;

int Distance1_Show=500;
int Distance2_Show=500;
int Distance3_Show=500;

int Distance_difference;


//int speed_expected=70;
float speed_real=0;
float speed_real2=0;

int pid_switch=1;   //½Ç¶Èpid¿ª¹Ø
int furrow=1;

char Receive_Date;

//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //ÉèÖÃNVICÖÐ¶Ï·Ö×é2:2Î»ÇÀÕ¼ÓÅÏÈ¼¶£¬2Î»ÏìÓ¦ÓÅÏÈ¼¶

long int Tick=0;
u32 uwTick=0;

int i=0;


void test1()
{
	Motor(40,40);
	Motor1(40,40);
}
void test2()
{
	Motor(speed_real,speed_real);
	Motor1(0-speed_real,0-speed_real);
}
void test3()
{
	Motor(speed_real+85-speed_real2,speed_real-85-speed_real2);
	Motor1(0-speed_real-85-speed_real2,0-speed_real+85-speed_real2);
}

void test4()
{
	Motor(speed_real-50+speed_real2,speed_real-50-speed_real2);
	Motor1(0-speed_real-50-speed_real2,0-speed_real-50+speed_real2);
}

//void test5()
//{
//	Motor(speed_real-85+speed_real2,speed_real+85+speed_real2);
//	Motor1(0-speed_real+85+speed_real2,0-speed_real-85+speed_real2);
//}

void test5()
{
	Motor(speed_real-85,speed_real+85);
	Motor1(0-speed_real+85,0-speed_real-85);
}

void test6()
{
	Motor(speed_real+50+speed_real2,speed_real+50-speed_real2);
	Motor1(0-speed_real+50-speed_real2,0-speed_real+50+speed_real2);
}

void test7()
{
	Motor(speed_real-85,speed_real+85);
	Motor1(0-speed_real+85,0-speed_real-85);
}

void test8()
{
	Motor(speed_real-85,speed_real+85);
	Motor1(0-speed_real+85,0-speed_real-85);
}

//void test8()
//{
//	Motor(speed_real-85-speed_real2,speed_real+85-speed_real2);
//	Motor1(0-speed_real+85-speed_real2,0-speed_real-85-speed_real2);
//}

//void LED_Init()
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                              //Ø¡à •Ë¤É«Ã¬Ö½Ó£È©à ¶Ô«ÙÇ·×µÜ˜1
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	
//	
//	GPIO_ResetBits(GPIOA,GPIO_Pin_2|GPIO_Pin_3);	
//}  


void InitAll()
{
	Timer2_PWM_Init();
	Timer3_PWM_Init();
	Motor_Polarity_Init();
	
	Ultrasound_Init();
	Lightswitch_Init();
//	Timer4_Int_Init();
	
	delay_init();				       //ÑÓÊ±³õÊ¼»¯
	OLED_Init();
	OLED_ColorTurn(0);         //0Õý³£ÏÔÊ¾£¬1 ·´É«ÏÔÊ¾
	OLED_DisplayTurn(0);       //0Õý³£ÏÔÊ¾ 1 ÆÁÄ»·­×ªÏÔÊ¾
	MPU_Init();					       //³õÊ¼»¯MPU6050
	USART1_Init();

	OLED_ShowString(15,0,"Waiting",24,1);
	OLED_ShowString(0,28,"Initialise",24,1);
	OLED_Refresh();
		
}

void Furrow1()
{
	Angle_P=1.3;        //2
	Angle_I=0.03;     //0.4
	Angle_D=0.5;     
	Distance_P=0.2;
	Distance_I=0.01;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)||Distance2_Show>310)
	{	
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//µÃµ½ÍÓÂÝÒÇÊý¾Ý		
			}
					
			error_now = 0-yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
			
			if (speed_real>25)
				speed_real=25;
			if (speed_real<-25)
				speed_real=-25;
			error_last=error_now;
		}
		
		Distance1_Show=Ultrasound_Getdistance1();
		Distance2_Show=Ultrasound_Getdistance2();
		error_distance_now = Distance_Target - Distance1_Show;
		error_distance_sum = error_distance_sum + error_distance_now;
	
		if (error_distance_sum>400)
			error_distance_sum=400;
		if (error_distance_sum<-400)
			error_distance_sum=-400;
	
		speed_real2 = Distance_P * error_distance_now + Distance_I * error_distance_sum;
	
			
		if (speed_real2>30)
			speed_real2=30;
		if (speed_real2<-30)
			speed_real2=-30;
		test3();
	}
	Motor(90,-90);
	Motor1(-90,90);
	delay_ms(50);
	
	Motor_Polarity_Stop();	
	delay_ms(100);
}

void Furrow2()
{
	Angle_D=4.0;
	Distance3_Show=310;
	furrow=2;
		
	error_distance_now=0;
	error_distance_sum=0;
	Angle_P=1.2;
	Angle_I=0.015;
	
	Distance_P=1.8;
	Distance_I=0.03;
	
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5))
	{
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//µÃµ½ÍÓÂÝÒÇÊý¾Ý		
			}
					
			error_now = 0-yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			if (error_sum>500)
				error_sum=500;
			if (error_sum<-500)
				error_sum=-500;
			
			if (speed_real>50)
				speed_real=50;
			if (speed_real<-50)
				speed_real=-50;
			error_last=error_now;		
		}
		Distance1_Show=Ultrasound_Getdistance3();
		Distance2_Show=Ultrasound_Getdistance4();
//		Distance3_Show=Ultrasound_Getdistance5();
		Distance_difference=Distance1_Show-Distance2_Show;
		error_distance_now = 0 - Distance_difference;
		error_distance_sum = error_distance_sum + error_distance_now;
	
		if (error_distance_sum>800)
			error_distance_sum=800;
		if (error_distance_sum<-800)
			error_distance_sum=-800;
	
		speed_real2 = Distance_P * error_distance_now + Distance_I * error_distance_sum;
	
			
		if (speed_real2>30)
			speed_real2=30;
		if (speed_real2<-30)
			speed_real2=-30;
		test4();
	}
	delay_ms(5);
	uwTick=0;
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0)
		||
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0)
	{		
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//µÃµ½ÍÓÂÝÒÇÊý¾Ý		
			}
					
			error_now = 0-yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
			
			if (speed_real>25)
				speed_real=25;
			if (speed_real<-25)
				speed_real=-25;
			error_last=error_now;		
		}
		Distance1_Show=Ultrasound_Getdistance3();
		Distance2_Show=Ultrasound_Getdistance4();
//		Distance3_Show=Ultrasound_Getdistance5();
		Distance_difference=Distance1_Show-Distance2_Show;
		error_distance_now = 0 - Distance_difference;
		error_distance_sum = error_distance_sum + error_distance_now;
	
		if (error_distance_sum>800)
			error_distance_sum=800;
		if (error_distance_sum<-800)
			error_distance_sum=-800;
	
		speed_real2 = Distance_P * error_distance_now + Distance_I * error_distance_sum;
	
			
		if (speed_real2>30)
			speed_real2=30;
		if (speed_real2<-30)
			speed_real2=-30;
		test4();
	}
	Motor(50,50);
	Motor1(50,50);
	delay_ms(200);
	Motor_Polarity_Stop();
	delay_ms(270);
	Angle_D=0;
//	OLED_ShowNum(50, 0,Tick,1,16,1);
	OLED_Refresh();
	Tick=0;
}

void Furrow2_2()
{
	Angle_D=4.0;
	Distance3_Show=310;
	furrow=2;
		
	error_distance_now=0;
	error_distance_sum=0;
	Angle_P=1.3;
	Angle_I=0.015;
	
	Distance_P=1.8;
	Distance_I=0.03;
	
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5))
	{
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//ÖƒÕ½Î“ÃÓ‡Ë½ß		
			}
					
			error_now = 0-yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			if (error_sum>600)
				error_sum=600;
			if (error_sum<-600)
				error_sum=-600;
			
			if (speed_real>60)
				speed_real=60;
			if (speed_real<-60)
				speed_real=-60;
			error_last=error_now;		
		}
		Distance1_Show=Ultrasound_Getdistance3();
		Distance2_Show=Ultrasound_Getdistance4();
//		Distance3_Show=Ultrasound_Getdistance5();
		Distance_difference=Distance1_Show-Distance2_Show;
		error_distance_now = 0 - Distance_difference;
		error_distance_sum = error_distance_sum + error_distance_now;
	
		if (error_distance_sum>800)
			error_distance_sum=800;
		if (error_distance_sum<-800)
			error_distance_sum=-800;
	
		speed_real2 = Distance_P * error_distance_now + Distance_I * error_distance_sum;
	
			
		if (speed_real2>30)
			speed_real2=30;
		if (speed_real2<-30)
			speed_real2=-30;
		test4();
	}
	delay_ms(5);
	uwTick=0;
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0)
		||
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0)
	{		
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//ÖƒÕ½Î“ÃÓ‡Ë½ß		
			}
					
			error_now = 0-yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
			
			if (speed_real>25)
				speed_real=25;
			if (speed_real<-25)
				speed_real=-25;
			error_last=error_now;		
		}
		Distance1_Show=Ultrasound_Getdistance3();
		Distance2_Show=Ultrasound_Getdistance4();
//		Distance3_Show=Ultrasound_Getdistance5();
		Distance_difference=Distance1_Show-Distance2_Show;
		error_distance_now = 0 - Distance_difference;
		error_distance_sum = error_distance_sum + error_distance_now;
	
		if (error_distance_sum>800)
			error_distance_sum=800;
		if (error_distance_sum<-800)
			error_distance_sum=-800;
	
		speed_real2 = Distance_P * error_distance_now + Distance_I * error_distance_sum;
	
			
		if (speed_real2>30)
			speed_real2=30;
		if (speed_real2<-30)
			speed_real2=-30;
		test4();
	}
	Motor(50,50);
	Motor1(50,50);
	delay_ms(200);
	Motor_Polarity_Stop();
	delay_ms(270);
	Angle_D=0;
//	OLED_ShowNum(50, 0,Tick,1,16,1);
	OLED_Refresh();
	Tick=0;
}



void Furrow3()
{
//	furrow=3;
	
	furrow=10;
		
	error_distance_now=0;
	error_distance_sum=0;
	Angle_P=2.0;
	Angle_I=0.03;
	Distance_P=0.24;
	Distance_I=0.02;
	while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==1)
		||(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==1)
		||(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)
		)
	{
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//µÃµ½ÍÓÂÝÒÇÊý¾Ý		
			}
					
			error_now = 0-yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
			
			if (speed_real>25)
				speed_real=25;
			if (speed_real<-25)
				speed_real=-25;
			error_last=error_now;
		}
		test5();
	}
	delay_ms(1);
	while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==0)
		||(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0)
		||(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0)
		||(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0)
		)
	{
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//µÃµ½ÍÓÂÝÒÇÊý¾Ý		
			}
					
			error_now = 0-yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
			
			if (speed_real>25)
				speed_real=25;
			if (speed_real<-25)
				speed_real=-25;
			error_last=error_now;
		}
		test5();
	}
	Motor(-60,+60);
	Motor1(+60,-60);
	delay_ms(240);
	Motor(+60,-60);
	Motor1(-60,+60);
	delay_ms(30);
	Motor_Polarity_Stop();
	delay_ms(70);
}

void Furrow4()
{	
	Distance3_Show=310;
	furrow=4;
		
	error_distance_now=0;
	error_distance_sum=0;
	Angle_P=1;
	Angle_I=0.015;
	Angle_D=4.0;
	
	Distance_P=1.8;
	Distance_I=0.03;
	uwTick=0;
	
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6))
	{
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//µÃµ½ÍÓÂÝÒÇÊý¾Ý		
			}
					
			error_now = 0-yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			if (error_sum>500)
				error_sum=500;
			if (error_sum<-500)
				error_sum=-500;
			
			if (speed_real>45)
				speed_real=45;
			if (speed_real<-45)
				speed_real=-45;
			error_last=error_now;
		}
		
		Distance1_Show=Ultrasound_Getdistance3();
		Distance2_Show=Ultrasound_Getdistance4();
//		Distance3_Show=Ultrasound_Getdistance5();
		Distance_difference=Distance1_Show-Distance2_Show;
		error_distance_now = 0 - Distance_difference;
		error_distance_sum = error_distance_sum + error_distance_now;
	
		if (error_distance_sum>800)
			error_distance_sum=800;
		if (error_distance_sum<-800)
			error_distance_sum=-800;
	
		speed_real2 = Distance_P * error_distance_now + Distance_I * error_distance_sum;
	
			
		if (speed_real2>30)
			speed_real2=30;
		if (speed_real2<-30)
			speed_real2=-30;
		
		test6();
	}
	delay_ms(5);
	uwTick=0;
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0)
		||
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0)
	{

		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//µÃµ½ÍÓÂÝÒÇÊý¾Ý		
			}
					
			error_now = 0-yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
			
			if (speed_real>25)
				speed_real=25;
			if (speed_real<-25)
				speed_real=-25;
			error_last=error_now;
		}
		
		Distance1_Show=Ultrasound_Getdistance3();
		Distance2_Show=Ultrasound_Getdistance4();
//		Distance3_Show=Ultrasound_Getdistance5();
		Distance_difference=Distance1_Show-Distance2_Show;
		error_distance_now = 0 - Distance_difference;
		error_distance_sum = error_distance_sum + error_distance_now;
	
		if (error_distance_sum>800)
			error_distance_sum=800;
		if (error_distance_sum<-800)
			error_distance_sum=-800;
	
		speed_real2 = Distance_P * error_distance_now + Distance_I * error_distance_sum;
	
			
		if (speed_real2>30)
			speed_real2=30;
		if (speed_real2<-30)
			speed_real2=-30;
		
		test6();
	}
	Motor(-50,-50);
	Motor1(-50,-50);
	delay_ms(200);
	
	Motor_Polarity_Stop();
	delay_ms(270);

	Angle_D=0;
}

void Furrow5()
{
//	furrow=1;
	furrow=10;	
	
	
	error_distance_now=0;
	error_distance_sum=0;
	Angle_P=2.2;
	Angle_I=0.04;
	Distance_P=0.24;
	Distance_I=0.02;
	while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)==1)
		||(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==1)
		||(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1)
		)
	{
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//µÃµ½ÍÓÂÝÒÇÊý¾Ý		
			}
					
			error_now = 0-yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
			
			if (speed_real>25)
				speed_real=25;
			if (speed_real<-25)
				speed_real=-25;
			error_last=error_now;
		}
		
		
		test7();
	}
	delay_ms(1);
	while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)==0)
		||(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0)
		||(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0)
		||(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0)
		)
	{
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//µÃµ½ÍÓÂÝÒÇÊý¾Ý		
			}
					
			error_now = 0-yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
			
			if (speed_real>25)
				speed_real=25;
			if (speed_real<-25)
				speed_real=-25;
			error_last=error_now;
		}
		
		
		test7();
	}
//	Motor(-60,+60);
//	Motor1(+60,-60);
//	delay_ms(70);
	Motor(-60,60);
	Motor1(60,-60);
	delay_ms(40);
		
		
		
	Motor_Polarity_Stop();	
	delay_ms(250);
	
}


void Furrow_last()
{	
	Distance3_Show=310;
	furrow=4;
		
	error_distance_now=0;
	error_distance_sum=0;
	Angle_P=1.5;
	Angle_I=0.015;
	Angle_D=0.1;
	
	Distance_P=1.8;
	Distance_I=0.03;
	Tick=0;
	
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6))
	{
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//ÖƒÕ½Î“ÃÓ‡Ë½ß		
			}
					
			error_now = 0-yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			if (error_sum>600)
				error_sum=600;
			if (error_sum<-600)
				error_sum=-600;
			
			if (speed_real>60)
				speed_real=60;
			if (speed_real<-60)
				speed_real=-60;
			error_last=error_now;
		}
		
		Distance1_Show=Ultrasound_Getdistance3();
		Distance2_Show=Ultrasound_Getdistance4();
//		Distance3_Show=Ultrasound_Getdistance5();
		Distance_difference=Distance1_Show-Distance2_Show;
		error_distance_now = 0 - Distance_difference;
		error_distance_sum = error_distance_sum + error_distance_now;
	
		if (error_distance_sum>800)
			error_distance_sum=800;
		if (error_distance_sum<-800)
			error_distance_sum=-800;
	
		speed_real2 = Distance_P * error_distance_now + Distance_I * error_distance_sum;
	
			
		if (speed_real2>30)
			speed_real2=30;
		if (speed_real2<-30)
			speed_real2=-30;
		
		test6();
	}
	delay_ms(5);
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0)
		||
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0)
	{
		Tick++;
		if (pid_switch)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{						  
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//ÖƒÕ½Î“ÃÓ‡Ë½ß		
			}
					
			error_now = 0-yaw;
			error_sum = error_sum+error_now;
			error_last = error_now - error_last;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			if (error_sum>200)
				error_sum=200;
			if (error_sum<-200)
				error_sum=-200;
			
			if (speed_real>25)
				speed_real=25;
			if (speed_real<-25)
				speed_real=-25;
			error_last=error_now;
		}
		
		Distance1_Show=Ultrasound_Getdistance3();
		Distance2_Show=Ultrasound_Getdistance4();
//		Distance3_Show=Ultrasound_Getdistance5();
		Distance_difference=Distance1_Show-Distance2_Show;
		error_distance_now = 0 - Distance_difference;
		error_distance_sum = error_distance_sum + error_distance_now;
	
		if (error_distance_sum>800)
			error_distance_sum=800;
		if (error_distance_sum<-800)
			error_distance_sum=-800;
	
		speed_real2 = Distance_P * error_distance_now + Distance_I * error_distance_sum;
	
			
		if (speed_real2>30)
			speed_real2=30;
		if (speed_real2<-30)
			speed_real2=-30;
		
		test6();
	}
	Motor(-45,-45);
	Motor1(-45,-45);
	delay_ms(250);
	
	Motor_Polarity_Stop();
	delay_ms(270);

	Angle_D=0;
}

int main(void)
{	 
	InitAll();
		
	while(mpu_dmp_init())
 	{
//		OLED_Clear();
//		OLED_Refresh();
	}
	

	OLED_Clear();
	OLED_ShowString(35,20,"RUN",24,1);
	OLED_Refresh();
//	
	Timer4_Int_Init();
	TIM_Cmd(TIM4,ENABLE);       //Ê¹ÄÜTIM
	OLED_Clear();
	OLED_ShowString(0,0,"DIST2:",16,1);
	OLED_ShowString(0,15,"REAL:",16,1);
	OLED_ShowString(0,31,"YAW:",16,1);
	OLED_ShowString(0,47,"TEAL:",16,1);
	OLED_Refresh();

 	while(1)
	{
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
			
			if (speed_real>30)
				speed_real=30;
			if (speed_real<-30)
				speed_real=-30;
			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
			error_last=error_now;
			OLED_ShowFloat(50,15,speed_real,5,16,1);		//yaw
			OLED_ShowFloat(50,31,yaw,5,16,1);				//
			
			Motor(0-speed_real+50,speed_real+50);    //Ö±×ß
			Motor1(0-speed_real+50,speed_real+50);

			delay_ms(5);
		}
//		Furrow1();

//		Furrow2();
//		
//		Furrow3();
//		
//		Furrow4();
//		
//		Furrow5();
//		
//		Furrow2();
//		
//		Furrow3();
//		
//		Furrow4();
//		
//		Furrow5();
//		
//		Furrow2_2();
//		
//		USART_Cmd(USART1,DISABLE);
//		
//		Furrow_last();
//	
//		furrow=10;
//		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4))
//		{
//			test8();
//		}
//		delay_ms(1);
//		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)==0)
//		{
//			test8();
//		}
//		
//		Angle_P=2.2;
//		Angle_I=0.04;
//		for(i=0;i<130;i++)
//		{
//			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
//			{						  
//				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);		
//			}
//					
//			error_now = 0-yaw;
//			error_sum = error_sum+error_now;
//			error_last = error_now - error_last;
//			speed_real = Angle_P*error_now + Angle_I*error_sum + Angle_D*error_last;
//			if (error_sum>300)
//				error_sum=300;
//			if (error_sum<-300)
//				error_sum=-300;
//			
//			if (speed_real>30)
//				speed_real=30;
//			if (speed_real<-30)
//				speed_real=-30;
//			error_last=error_now;
//			Motor(speed_real-95,speed_real+85);
//			Motor1(85-speed_real,0-speed_real-95);
//			delay_ms(5);
//		}
		
//		Motor(-95,+95);
//		Motor1(+95,-95);
//		delay_ms(3000);


		
//		Motor_Polarity_Stop();	
//		while(1);
				
	
}



void TIM4_IRQHandler()
{
	if (TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET)
	{	
		uwTick++;
		
		
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}		
}

unsigned int seed_flag=0;

void USART1_IRQHandler(void)
{											
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
	{		
		Receive_Date = USART_ReceiveData(USART1);
		
		OLED_Clear();
		OLED_ShowString(40,20,"STOP",24,1);
		OLED_ShowNum(50, 0,uwTick,1,16,1);
		OLED_Refresh();
		
		if (furrow == 2)
		{
			if (uwTick < 25)
			{	
				Motor(25,25);
				Motor1(25,25);
				delay_ms(uwTick*10);
			}
			
			else if (uwTick < 30)
			{
				Motor(30,30);
				Motor1(30,30);
				delay_ms(uwTick*10);
			}
			else if (uwTick < 50)
			{
				Motor(30,30);
				Motor1(30,30);
				delay_ms(200);
			}
			else
			{
				Motor(40,40);
				Motor1(40,40);
				delay_ms(200);
			}
		}
		if (furrow == 4)
		{
			if (uwTick < 25)
			{	
				Motor(-25,-25);
				Motor1(-25,-25);
				delay_ms(uwTick*10);
			}
			
			else if (uwTick < 30)
			{
				Motor(-30,-30);
				Motor1(-30,-30);
				delay_ms(uwTick*10);
			}
			else if (uwTick < 50)
			{
				Motor(-30,-30);
				Motor1(-30,-30);
				delay_ms(200);
			}
			else
			{
				Motor(-40,-40);
				Motor1(-40,-40);
				delay_ms(200);
			}
		}
		Motor_Polarity_Stop();
		
		while(Receive_Date!='g')
		{			
//			USART_ClearITPendingBit(USART1,USART_IT_RXNE);
			Receive_Date = USART_ReceiveData(USART1);
		}
		uwTick=0;
		OLED_Clear();
		OLED_ShowString(35,20,"RUN",24,1);
		OLED_Refresh();
				
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}

/*******************************************

SOLIDWORKSæ–‡ä»¶, PCBæ–‡ä»¶, OpenMvä»£ç 

æ›´å¤šèµ„æ–™è”ç³» QQ:2231821988, æ¬¢è¿Žäº¤æµ

è‡ªåŠ¨åŒ– åˆ˜å¨ ç¥ï¼š

æ¯”èµ›é¡ºåˆ©ï¼ï¼

*******************************************/

/*******************************************

SOLIDWORKSÎÄ¼þ£¬PCBÎÄ¼þ£¬OpenMv´úÂë

¸ü¶à×ÊÁÏÁªÏµQQ:2231821988£¬»¶Ó­½»Á÷

×Ô¶¯»¯ ÁõÍþ ×££º

±ÈÈüË³Àû£¡£¡

*******************************************/

