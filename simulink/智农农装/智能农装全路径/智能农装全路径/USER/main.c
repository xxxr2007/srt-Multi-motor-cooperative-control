
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "timers.h"

#include "GD.h"
#include "oled.h"
#include "Motor.h"
#include "GD.h"
#include "Encoder.h"
#include "Servo.h"

#include "MPU6050.h"
#include "HC05.h"
#include "FMQ.h"
#include "csb.h"

#include "SpeedPID.h"
#include "Action.h"
#include "key.h"

extern void SpeedPID_Control(void); //仅在主函数中可见
extern void GrayTrack_Control(void); //仅在主函数中可见
extern CSB_Count CSB_OriData;
extern int16_t Path_Color[];//路径颜色数组 赛道用

int TLY_flag=0;
int OLED_Order;
uint8_t Check_flag;
static int Long_Count=0;
struct Pid_tly{
	float p_tly;
	float i_tly;
	float d_tly;
	float error_tly;
	float error_last_tly;
	float error_sum_tly;
	float output_tly;
 
 };
struct Pid_tly PID_TLY;

int Long_flag=0;
float Speed_L=0;
float Speed_R=0;
float Yaw_Target;

static int num=0;
void TIM4_IRQHandler(void)
{
 if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
 {	
		CSB_Service();

    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  }
}
//定时服务函数2 优先级最高，刷新速度环PID
void TIM7_IRQHandler(void)
{

 if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
 {
   TLY_Control();
//	 num++;
//	 int flag=0;
//	 if(num>=60){flag=1;num=0;}
//	 else{flag=0;}
//		if((GD_Read_Ctype(CF)==GD_State_Detected||GD_Read_Ctype(CF)==GD_State_Detected)&&flag&&Long_flag){
//			Action_Track_Disable();
//			Motor_SetOuput(Motor_L,0);
//			Motor_SetOuput(Motor_R,0);
//			Servo_On();
//			int Count=0;
//			int Color=0;
//			while(Count++<50){
//				if(GD_Read_Ctype(CL)==GD_State_Detected)Count+=1;
//				else Count+=0;
//				delay_ms(50);
//			}
//			if(Count>=25)Color=1;
//			else Color=0;
//			
//			if(Color==1)FMQ_On();delay_ms(500);FMQ_Off();
//			delay_ms(1000);
//			Servo_Off();
//			delay_ms(1000);
//			Action_Track_Enable();
		
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
  }
}

//定时服务函数1 刷新循迹控制
void TIM6_DAC_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
  {
    //循迹控制 如果循迹关闭 其不会执行任何操作
	
		Track();
		SpeedPID_Refrash();
   }

    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
  
}

//数据刷新/发送服务函数
void TIM5_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
  {
		
    //FMQ_Serve();


		
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
  }
}

int main(void){
//  //系统初始化
	delay_init(84);
  Timers_TIM4_Stop();
  Timers_TIM6_Stop();
  Timers_TIM7_Stop();
  Timers_TIM5_Stop();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
  //拥有八个抢占优先级 0->7 0最高 7最低 
  //0 -- 陀螺仪   		串口1中断
  //1 -- 速度环PID    串口2中断
  //2 -- 超声波  			定时器4中断
  //3 -- 循迹服务   	定时器6中断
  //4 -- 蓝牙       	串口4中断
  //5 -- 刷新数据   	定时器5中断 
  
  //硬件初始化

  //////////////////////////////点亮左右LED 代表进入初始化////////////////////////////////////
  FMQ_Init();
  OLED_Init();
  Motor_Init();
	//OpenMV_Init();
  GD_Init();
  Encoder_Init();
  Servo_Init();
  MPU6050_Init();
  HC05_Init();
  Key_Init();
	CSB_Init();
  SpeedPID_Init();
//  Timers_TIM7_Init();
//  Timers_TIM5_Init();
//  Timers_TIM6_Init();
////  初始化结束 开启定时器中断
	Key_Let_sCheck();
//	Timers_TIM6_Start();
//	Timers_TIM7_Start();
//	Timers_TIM5_Start();
	
//	CSB_Enable();CSB_Allow_Which(0x0F);
//	MPU6050_SetState(ENABLE);	
//	Action_Track_Disable();
//	Action_TLY_Disable();
//	
//	delay_ms(1000);
//	FMQ_Beep(3);//准备开始

//	Action_Start();//等待出发动作
//	OLED_Clear();
	
	Motor_SetOuput(Motor_L,-40);
	Motor_SetOuput(Motor_R,-40);
	while(GD_Read_Ctype(LB)==GD_State_NotDetected);FMQ_Beep(3);
	while(GD_Read_Ctype(RF)==GD_State_NotDetected);FMQ_Beep(3);
	
	int count=0;//计数垄
	int flag=1;
	//出发，为防止出界，采用水管向内原则
	//路线1，从出发点直入至对面边界
	
			Track_ChangePD(3.0f,4.3f,10.0f);
			Track_Set(-60,CSB_Right,10,Back);
			Action_Track_Enable();
			//识别五垄
			
			
			while(1){
				if(count>=5){count=0;break;}
				if(GD_Read_Ctype(LB)==GD_State_Detected&&flag){FMQ_Beep(3);count++;flag=0;}
				if(GD_Read_Ctype(LB)==GD_State_NotDetected){flag=1;} 
			}
			
			Track_Set(-30,CSB_Right,11,Back);

			while(GD_Read_Ctype(LB)==GD_State_Detected);FMQ_Beep(3);delay_ms(1000);
			while(GD_Read_Ctype(LF)==GD_State_Detected);FMQ_Beep(3);
			delay_ms(550);
			Action_Track_Disable();
			Action_TLY_Disable();
			//第一转弯停
			Motor_SetOuput(Motor_L,0);
			Motor_SetOuput(Motor_R,0);
			FMQ_Beep(3);
			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			Action_Turn2_0(88,Right);
//			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);

			CSB_Enable();
			CSB_Allow_Which(0x0F);
			MPU6050_SetState(ENABLE);
			Action_TLY_Disable();

			FMQ_Beep(3);
			Action_Track_Enable();
			Track_Set(-40,CSB_Right,12,Back);
			Track_ChangePD(3.0f,4.3f,10.0f);//11.3f,2.3f,42.0f
			//Track_Set(-45,CSB_Left,10,Back);
			//CSB_Enable();
			
			delay_ms(500);
			while(GD_Read_Ctype(LB)==GD_State_NotDetected);
			while(GD_Read_Ctype(LB)==GD_State_Detected);FMQ_Beep(3);
			Track_Set(-30,CSB_Right,12,Back);
			while(GD_Read_Ctype(LF)==GD_State_Detected);FMQ_Beep(3);
			delay_ms(550);
			Action_Track_Disable();
			//第二转弯停
			Motor_SetOuput(Motor_L,0);
			Motor_SetOuput(Motor_R,0);
			delay_ms(700);
//			delay_ms(1000);
//			delay_ms(1000);
			FMQ_Beep(3);FMQ_Beep(3);
			Action_Turn2_0(88,Right);
			Motor_SetOuput(Motor_L,0);
			Motor_SetOuput(Motor_R,0);
//			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
//			 
			
			Track_Set(-30,CSB_Right,10,Back);
			Track_ChangePD(6.3f,2.7f,20.0f);
			Action_Track_Enable();
			while(GD_Read_Ctype(LF)==GD_State_NotDetected);FMQ_Beep(3);delay_ms(1000);
			while(GD_Read_Ctype(LF)==GD_State_Detected);FMQ_Beep(3);
//			delay_ms(300);
			Action_Track_Disable();
			//第三转弯停
			Motor_SetOuput(Motor_L,0);
			Motor_SetOuput(Motor_R,0);
			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			
			
			FMQ_Beep(3);
			Action_Turn2_0(87,Right);
			
			Motor_SetOuput(Motor_L,-30);
			Motor_SetOuput(Motor_R,-30);

			while(GD_Read_Ctype(LF)==GD_State_NotDetected);FMQ_Beep(3);
//			Motor_SetOuput(Motor_L,0);
//			Motor_SetOuput(Motor_R,0);
//			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			Long_flag=1;
			Track_Set(-40,CSB_Right,10,Back);
			Track_ChangePD(3.0f,4.3f,10.0f);
			Action_Track_Enable();
			
			//第四转弯停
			while(GD_Read_Ctype(LB)==GD_State_Detected);FMQ_Beep(3);
			Track_Set(-30,CSB_Right,10,Back);
			while(GD_Read_Ctype(LF)==GD_State_Detected);FMQ_Beep(3);
			delay_ms(700);
			Action_Track_Disable();
			Long_flag=0;
			Motor_SetOuput(Motor_L,0);
			Motor_SetOuput(Motor_R,0);
			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			Action_Turn2_0(88.5,Right);
//			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			
			Motor_SetOuput(Motor_L,30);
			Motor_SetOuput(Motor_R,30);	
		
			Track_Set(30,CSB_Right,10,Front);
			Track_ChangePD(6.2f,1.3f,20.0f);//10.8f,1.3f,40.0f
			Action_Track_Enable();
			while(GD_Read_Ctype(LF)==GD_State_NotDetected);FMQ_Beep(3);
			while(GD_Read_Ctype(LB)==GD_State_Detected||GD_Read_Ctype(LF)==GD_State_Detected);FMQ_Beep(3);
			delay_ms(700);
			Action_Track_Disable();	
			//第五转弯停
			Motor_SetOuput(Motor_L,0);
			Motor_SetOuput(Motor_R,0);
			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			Action_Turn2_0(90,Left);
//			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			
			Motor_SetOuput(Motor_L,30);
			Motor_SetOuput(Motor_R,30);	
			while(GD_Read_Ctype(LF)==GD_State_NotDetected);FMQ_Beep(3);
			Long_flag=1;
			FMQ_Beep(3);
			Track_Set(40,CSB_Right,10,Front);
			Track_ChangePD(3.0f,4.3f,10.0f);
			Action_Track_Enable();
			
			while(GD_Read_Ctype(LF)==GD_State_Detected);FMQ_Beep(3);
			Track_Set(30,CSB_Right,10,Front);
			while(GD_Read_Ctype(LB)==GD_State_Detected);FMQ_Beep(3);
			delay_ms(270);
			Action_Track_Disable();	
			//第六转弯停
			Motor_SetOuput(Motor_L,0);
			Motor_SetOuput(Motor_R,0);
			Long_flag=0;
			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			
			Action_Turn2_0(88,Left);
//			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			
			Track_Set(-30,CSB_Right,10,Back);
			Track_ChangePD(7.2f,2.1f,20.0f);
			Action_Track_Enable();
			delay_ms(1000);delay_ms(1000);delay_ms(1000);
			while(GD_Read_Ctype(LB)==GD_State_NotDetected);FMQ_Beep(3);
			delay_ms(1000);
			while(GD_Read_Ctype(LB)==GD_State_Detected||GD_Read_Ctype(LF)==GD_State_Detected);FMQ_Beep(3);
//			while(GD_Read_Ctype(LF)==GD_State_NotDetected);FMQ_Beep(3);
			delay_ms(400);
			Action_Track_Disable();	
			//第七转弯停
			Motor_SetOuput(Motor_L,0);
			Motor_SetOuput(Motor_R,0);
			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			Action_Turn2_0(90,Right);
//			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			
			Motor_SetOuput(Motor_L,-30);
			Motor_SetOuput(Motor_R,-30);	
			while(GD_Read_Ctype(LF)==GD_State_NotDetected);FMQ_Beep(3);
			Long_flag=1;
			
			FMQ_Beep(3);
			Track_Set(-40,CSB_Right,10,Back);
			Track_ChangePD(3.0f,4.3f,10.0f);
			Action_Track_Enable();
			while(GD_Read_Ctype(LB)==GD_State_Detected);FMQ_Beep(3);
			Track_Set(-30,CSB_Right,10,Back);
			while(GD_Read_Ctype(LF)==GD_State_Detected);FMQ_Beep(3);
			delay_ms(900);
			Action_Track_Disable();	
			//第八转弯停
			Motor_SetOuput(Motor_L,0);
			Motor_SetOuput(Motor_R,0);
			Long_flag=0;
			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			Action_Turn2_0(89.5,Right);
//			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			
			Track_Set(30,CSB_Right,10,Front);
			Track_ChangePD(6.2f,1.3f,20.0f);//10.8f,1.3f,40.0f
			Action_Track_Enable();
			while(GD_Read_Ctype(LF)==GD_State_NotDetected);FMQ_Beep(3);
			delay_ms(1000);
			while(GD_Read_Ctype(LB)==GD_State_Detected||GD_Read_Ctype(LF)==GD_State_Detected);FMQ_Beep(3);
			delay_ms(950);
			Action_Track_Disable();	
			Motor_SetOuput(Motor_L,0);
			Motor_SetOuput(Motor_R,0);
			//第九转弯停
			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			Action_Turn2_0(89,Left);
//			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
//			
			Motor_SetOuput(Motor_L,30);
			Motor_SetOuput(Motor_R,30);	
			while(GD_Read_Ctype(LF)==GD_State_NotDetected);FMQ_Beep(3);
			Long_flag=1;
			FMQ_Beep(3);
			Track_Set(40,CSB_Right,10,Front);
			Track_ChangePD(3.0f,4.3f,10.0f);
			Action_Track_Enable();
			
			while(GD_Read_Ctype(LF)==GD_State_Detected);FMQ_Beep(3);
			Track_Set(30,CSB_Right,10,Front);
			while(GD_Read_Ctype(LB)==GD_State_Detected);FMQ_Beep(3);
			delay_ms(660);
			Action_Track_Disable();	
			Motor_SetOuput(Motor_L,0);
			Motor_SetOuput(Motor_R,0);
			Long_flag=0;
			//第十转弯停
			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			Long_flag=0;
			Action_Turn2_0(88,Left);
//			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			
			FMQ_Beep(3);
			Track_Set(-30,CSB_Right,10,Back);
			Track_ChangePD(6.3f,2.3f,20.0f);
			Action_Track_Enable();
			delay_ms(1000);delay_ms(1000);delay_ms(1000);
			while(GD_Read_Ctype(LB)==GD_State_NotDetected);FMQ_Beep(3);
			delay_ms(1000);
			while(GD_Read_Ctype(LB)==GD_State_Detected||GD_Read_Ctype(LF)==GD_State_Detected);FMQ_Beep(3);
			delay_ms(330);
			Action_Track_Disable();	
			Motor_SetOuput(Motor_L,0);
			Motor_SetOuput(Motor_R,0);
			
			//第十一转弯停
			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			Action_Turn2_0(88,Right);
//			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			
			Motor_SetOuput(Motor_L,-30);
			Motor_SetOuput(Motor_R,-30);	
			while(GD_Read_Ctype(LF)==GD_State_NotDetected);FMQ_Beep(3);
			Long_flag=1;
			FMQ_Beep(3);
			Track_Set(-40,CSB_Right,10,Back);
			Track_ChangePD(3.0f,4.3f,10.0f);
			Action_Track_Enable();
			
			while(GD_Read_Ctype(LB)==GD_State_Detected);FMQ_Beep(3);
			Track_Set(-30,CSB_Right,10,Back);
			while(GD_Read_Ctype(LB)==GD_State_Detected||GD_Read_Ctype(LF)==GD_State_Detected);FMQ_Beep(3);
			delay_ms(400);
			Action_Track_Disable();	
			Motor_SetOuput(Motor_L,0);
			Motor_SetOuput(Motor_R,0);
			//第十二转弯停
			delay_ms(700);
//			delay_ms(1000);
//			delay_ms(1000);
			Long_flag=0;
			Action_Turn2_0(88,Right);
//			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);
			
			FMQ_Beep(3);
			Track_Set(50,CSB_Right,10,Front);
			Track_ChangePD(3.0f,4.3f,10.0f);
			Action_Track_Enable();
			delay_ms(1000);delay_ms(1000);
			while(GD_Read_Ctype(RB)==GD_State_Detected||GD_Read_Ctype(RF)==GD_State_Detected);FMQ_Beep(3);
			delay_ms(1000);
			Action_Track_Disable();	
			Motor_SetOuput(Motor_L,0);
			Motor_SetOuput(Motor_R,0);FMQ_Beep(10);
////		
  while(1){
		Action_Start();
		delay_ms(1000);
		Action_Turn2_0(80,Left);
		delay_ms(1000);
//		SpeedPID_SetTarget(Motor_L,0);
//		SpeedPID_SetTarget(Motor_R,0);
//		OLED_ShowStringXY(0,0,"MPU6050 Check!!!"); 
//		OLED_ShowStringXY(0,1,"Pitch:");OLED_ShowFloatXY(6,1,MPU6050_AngleData.Pitch,3,2);
//    OLED_ShowStringXY(0,2,"Roll:");OLED_ShowFloatXY(6,2,MPU6050_AngleData.Roll,3,2);
//    OLED_ShowStringXY(0,3,"Yaw:");OLED_ShowFloatXY(6,3,MPU6050_AngleData.Yaw,3,2);
//		OLED_ShowStringXY(0,4,"Back:");OLED_ShowFloatXY(6,4,CSB_GetDistance(CSB_Back),2,3);
		//HC05_Printf("%d,%d\n",Encoder_GetVol(Encoder_L),Encoder_GetVol(Encoder_R));
		
//	FMQ_On();
//	delay_ms(3000);
//	FMQ_Off();

	
	}
}
////////////////////END///////////////////// 
