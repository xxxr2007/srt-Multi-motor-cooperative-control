#include "pid.h"

void Timer4_Int_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=4999;   //ARR   TIM_GetCounter(TIM2)    周期长度：500*100us=50ms
	TIM_TimeBaseInitStructure.TIM_Prescaler=71;    //PSC   分频每7200个时钟脉冲，会给定时器发送一个脉冲  即每100us
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;        //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;   //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;     //响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;    //   IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);         //     NVIC寄存器初始化
	
//	TIM_Cmd(TIM4,ENABLE);       //使能TIM
}
	


	
