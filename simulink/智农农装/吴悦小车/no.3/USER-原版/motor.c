#include "motor.h"
#include "sys.h"


/***************************************************
函数功能:定时器2输出2段pwm的初始化  PA0   PA1
频率：100hz 
IO口：PA0   PA1
***************************************************/
void Timer2_PWM_Init()
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure1;
	TIM_OCInitTypeDef TIM_OCInitStructure2;
	
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
//  GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
	
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;  //输出控制权转移给片上外设 而不是输出数字寄存器 所以为AF
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//不分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=100-1;   //ARR   TIM_GetCounter(TIM2)加到 10000为一秒      
	TIM_TimeBaseInitStructure.TIM_Prescaler=7200-1;    //PSC   分频 72,000,000/7200 = 10,000
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	
	TIM_OCStructInit(&TIM_OCInitStructure1);   //不把所有值都赋值 所以先给结构体赋一个初始值
	TIM_OCInitStructure1.TIM_OCMode= TIM_OCMode_PWM1;   //输出比较模式的选择 这里选pwm1
	TIM_OCInitStructure1.TIM_OCPolarity= TIM_OCPolarity_High;  //输出比较极性  这里极性不反转
	TIM_OCInitStructure1.TIM_OutputState= TIM_OutputState_Enable;  //输出使能  enable
	TIM_OCInitStructure1.TIM_Pulse= 50;     //设置CCR寄存器的值   占空比为CCR/ARR 这里ccr/2既为占空比 分辨率为1% 频率为100
	TIM_OC1Init(TIM2,&TIM_OCInitStructure1);   //这里选择通道1    PA0
	
	
	TIM_OCStructInit(&TIM_OCInitStructure2);   //不把所有值都赋值 所以先给结构体赋一个初始值
	TIM_OCInitStructure2.TIM_OCMode= TIM_OCMode_PWM1;   //输出比较模式的选择 这里选pwm1
	TIM_OCInitStructure2.TIM_OCPolarity= TIM_OCPolarity_High;  //输出比较极性  这里极性不反转
	TIM_OCInitStructure2.TIM_OutputState= TIM_OutputState_Enable;  //输出使能  enable
	TIM_OCInitStructure2.TIM_Pulse= 50;     //设置CCR寄存器的值   占空比为CCR/ARR 这里ccr/2既为占空比 分辨率为1% 频率为100
	TIM_OC2Init(TIM2,&TIM_OCInitStructure2);   //这里选择通道2    PA1
	
	
	TIM_Cmd(TIM2,ENABLE);
}

/***************************************************
函数功能:定时器3输出2段pwm的初始化  PA6   PA7
频率：100hz 
IO口：PA6   PA7
***************************************************/
void Timer3_PWM_Init()  //PA6  PA7
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure1;
	TIM_OCInitTypeDef TIM_OCInitStructure2;
	
	
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
//  GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
	
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;  //输出控制权转移给片上外设 而不是输出数字寄存器 所以为AF
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3);
	
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=100-1;   //ARR   TIM_GetCounter(TIM2)加到9999       
	TIM_TimeBaseInitStructure.TIM_Prescaler=7200-1;    //PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	
	TIM_OCStructInit(&TIM_OCInitStructure1);   //不把所有值都赋值 所以先给结构体赋一个初始值
	TIM_OCInitStructure1.TIM_OCMode= TIM_OCMode_PWM1;   //输出比较模式的选择 这里选pwm1
	TIM_OCInitStructure1.TIM_OCPolarity= TIM_OCPolarity_High;  //输出比较极性  这里极性不反转
	TIM_OCInitStructure1.TIM_OutputState= TIM_OutputState_Enable;  //输出使能  enable
	TIM_OCInitStructure1.TIM_Pulse= 50;     //设置CCR寄存器的值   占空比为CCR/ARR 这里ccr/2既为占空比 分辨率为0.5% 频率为1000
	TIM_OC1Init(TIM3,&TIM_OCInitStructure1);   //这里选择通道1    PA0
	
	
	TIM_OCStructInit(&TIM_OCInitStructure2);   //不把所有值都赋值 所以先给结构体赋一个初始值
	TIM_OCInitStructure2.TIM_OCMode= TIM_OCMode_PWM1;   //输出比较模式的选择 这里选pwm1
	TIM_OCInitStructure2.TIM_OCPolarity= TIM_OCPolarity_High;  //输出比较极性  这里极性不反转
	TIM_OCInitStructure2.TIM_OutputState= TIM_OutputState_Enable;  //输出使能  enable
	TIM_OCInitStructure2.TIM_Pulse= 50;     //设置CCR寄存器的值   占空比为CCR/ARR 这里ccr/2既为占空比 分辨率为0.5% 频率为1000
	TIM_OC2Init(TIM3,&TIM_OCInitStructure2);   //这里选择通道2    PA1
	
	
	
	
	TIM_Cmd(TIM3,ENABLE);
}


/***************************************************
函数功能:设置占空比
范围：0-100
***************************************************/
void PWM_SetCompare1(uint16_t Compare1)
{
	TIM_SetCompare1(TIM2,Compare1);
}

void PWM_SetCompare2(uint16_t Compare2)
{
	TIM_SetCompare2(TIM2,Compare2);
}

void PWM_SetCompare3(uint16_t Compare3)
{
	TIM_SetCompare1(TIM3,Compare3);
}

void PWM_SetCompare4(uint16_t Compare4)
{
	TIM_SetCompare2(TIM3,Compare4);
}

/***************************************************
函数功能:电机极性初始化
IO口：PG5 6 7 8 9 10 11 12
 56--78
 |    |
 |    |
 |    |
 |    |
 910--1112

 m1--m2
 |    |
 |    |
 |    |
 |    |
 m3--m4

***************************************************/
void Motor_Polarity_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOG,&GPIO_InitStructure);
}

/***************************************************
函数功能:电机停止转动
***************************************************/
void Motor_Polarity_Stop(void)
{
	
	GPIO_SetBits(GPIOG,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8||GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
	
	PWM_SetCompare1(0);
	PWM_SetCompare2(0);
	PWM_SetCompare3(0);
	PWM_SetCompare4(0);
	
	
}

/***************************************************
函数功能:电机正转
***************************************************/
void Motor1_Polarity_Go(void)
{
	GPIO_SetBits(GPIOG,GPIO_Pin_5);
	GPIO_ResetBits(GPIOG,GPIO_Pin_6);
}

void Motor2_Polarity_Go(void)
{
	GPIO_SetBits(GPIOG,GPIO_Pin_7);
	GPIO_ResetBits(GPIOG,GPIO_Pin_8);
}

void Motor3_Polarity_Go(void)
{
	GPIO_SetBits(GPIOG,GPIO_Pin_9);
	GPIO_ResetBits(GPIOG,GPIO_Pin_10);
}

void Motor4_Polarity_Go(void)
{
	GPIO_SetBits(GPIOG,GPIO_Pin_11);
	GPIO_ResetBits(GPIOG,GPIO_Pin_12);
}
/***************************************************
函数功能:电机反转
***************************************************/
void Motor1_Polarity_Back(void)
{
	GPIO_SetBits(GPIOG,GPIO_Pin_6);
	GPIO_ResetBits(GPIOG,GPIO_Pin_5);
}

void Motor2_Polarity_Back(void)
{
	GPIO_SetBits(GPIOG,GPIO_Pin_8);
	GPIO_ResetBits(GPIOG,GPIO_Pin_7);
}

void Motor3_Polarity_Back(void)
{
	GPIO_SetBits(GPIOG,GPIO_Pin_10);
	GPIO_ResetBits(GPIOG,GPIO_Pin_9);
}

void Motor4_Polarity_Back(void)
{
	GPIO_SetBits(GPIOG,GPIO_Pin_12);
	GPIO_ResetBits(GPIOG,GPIO_Pin_11);
}

/***************************************************
函数功能:电机控制
Motor(20, -20)
Motor1(-20, 20)
***************************************************/
void Motor(int pwm1, int pwm2)
{
	if (pwm1 >= 99)
	{
		pwm1=99;
	}
	if (pwm1 <= -99)
	{
		pwm1=-99;
	}
	if (pwm2 >= 99)
	{
		pwm2=99;
	}
	if (pwm2 <= -99)
	{
		pwm2=-99;
	}
	
	
	if (pwm1 >= 0)
	{
		Motor1_Polarity_Go();
		PWM_SetCompare1(pwm1);
	}
	else
	{
		Motor1_Polarity_Back();
		PWM_SetCompare1(0-pwm1);
	}
	
	if (pwm2 >= 0)
	{
		Motor2_Polarity_Go();
		PWM_SetCompare2(pwm2);
	}
	else
	{
		Motor2_Polarity_Back();
		PWM_SetCompare2(0-pwm2);
	}
}

void Motor1(int pwm3 , int pwm4)
{
	if (pwm3 >= 99)
	{
		pwm3=99;
	}
	if (pwm3 <= -99)
	{
		pwm3=-99;
	}
	if (pwm4 >= 99)
	{
		pwm4=99;
	}
	if (pwm4 <= -99)
	{
		pwm4=-99;
	}
	
	
	
	if (pwm3 >= 0)
	{
		Motor3_Polarity_Go();
		PWM_SetCompare3(pwm3);
	}
	else
	{
		Motor3_Polarity_Back();
		PWM_SetCompare3(0-pwm3);
	}
	
	if (pwm4 >= 0)
	{
		Motor4_Polarity_Go();
		PWM_SetCompare4(pwm4);
	}
	else
	{
		Motor4_Polarity_Back();
		PWM_SetCompare4(0-pwm4);
	}
}
