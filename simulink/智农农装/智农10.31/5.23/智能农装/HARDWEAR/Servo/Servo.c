#include "Servo.h"

//初始化舵机
void Servo_Init()
{
	//产生20ms的PWM周期
	{
		//此部分需手动修改IO口设置
		/* InitTypeDef */
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		/* 时钟 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1时钟使能    
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
		
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1); //复用GPIOA_Pin8为TIM1_Ch1, 
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);//复用GPIOA_Pin11为TIM1_Ch4,注意没有CH4N 
		/* GPIO */	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;           //GPIO
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
		GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化A
		/* Time Base */
		TIM_TimeBaseStructure.TIM_Prescaler=16799;  //定时器分频
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
		TIM_TimeBaseStructure.TIM_Period=199;   //自动重装载值
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;//默认就为0
		TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器1
	
		//初始化TIM1  PWM模式	 
		//PWM 模式 1–– 在递增计数模式下，只要 TIMx_CNT<TIMx_CCR1，通道 1 便为有效状态，否则为无效状态。在递减计数模式下，
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //PWM1为正常占空比模式，PWM2为反极性模式
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低,有效电平为低电平
	
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//在空闲时输出     低,这里的设置可以改变TIM_OCPolarity 如果没这句，第1通道有问题
		
		TIM_OCInitStructure.TIM_Pulse = 0; //输入通道1 CCR1（占空比数值）
		TIM_OC1Init(TIM1, &TIM_OCInitStructure); //Ch1初始化
		
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);//通道4
		
		/* 使能 */
		TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器,CCR自动装载默认也是打开的
		
		TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 
		
		TIM_Cmd(TIM1, ENABLE);  //使能TIM1
		
		TIM_CtrlPWMOutputs(TIM1, ENABLE);//使能TIM1的PWM输出，TIM1与TIM8有效,如果没有这行会问题				
	}

//	//产生20ms的PWM周期
//	{
//		//此部分需手动修改IO口设置
//		/* InitTypeDef */
//		GPIO_InitTypeDef GPIO_InitStructure;
//		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//		TIM_OCInitTypeDef  TIM_OCInitStructure;
//		/* 时钟 */
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM14时钟使能    
//		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB时钟	
//		
//		GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4); //GPIOB8复用为定时器4
//		GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4); //GPIOB8复用为定时器4
//		/* GPIO */
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8;           //GPIOB 8 9
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
//		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
//		GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PB8 9
//		/* Time Base */
//		TIM_TimeBaseStructure.TIM_Prescaler=8399;  //定时器分频
//		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
//		TIM_TimeBaseStructure.TIM_Period=199;   //自动重装载值
//		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
//		TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化定时器4
//		
//		//初始化TIM4 Channel3 4 PWM模式	 
//		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
//		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
//		TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC3
//		TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC4
//		
//		/* 使能 */
//		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR3上的预装载寄存器
//		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR4上的预装载寄存器
//		
//		TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE使能 
//		
//		TIM_Cmd(TIM4, ENABLE);  //使能TIM14
//	}

	// //初始占空比参数 全部置0
	// //1.5ms 不转
	// TIM_SetCompare1(TIM1,0);//PA8		//左手臂 360°舵机 控制转速
	// TIM_SetCompare4(TIM1,0);//PA11		//右手臂 360°舵机 控制转速

	// //1.5ms 90°
	// TIM_SetCompare3(TIM4,0);//PB8		左灰度舵机
	// TIM_SetCompare4(TIM4,0);//PB9		右灰度舵机


	Servo_SetDuty(Servo_Wait);
}

/// @brief 设置舵机输出
/// @param Which Servo_Arm_L Servo_Arm_R Servo_Gray_L Servo_Gray_R
/// @param Duty 0.000~0.125
void Servo_SetDuty(float Duty)
{
	//占空比从0.025~0.125
	if(Duty<0.025f)
	{
		Duty = 0.025f;
	}
	if(Duty>0.125f)
	{
		Duty = 0.125f;
	}
	TIM_SetCompare4(TIM1,200*Duty);//舵机1 CH4

}


void Servo_Off(void){
	Servo_SetDuty(Servo_Wait);
}

void Servo_On(void){
	Servo_SetDuty(Servo_Ready);
}
