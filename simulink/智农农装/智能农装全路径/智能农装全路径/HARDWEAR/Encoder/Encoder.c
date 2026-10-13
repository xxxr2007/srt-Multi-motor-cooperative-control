#include "Encoder.h"

//初始化编码器
void Encoder_Init(void)
{
    {    
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能PA端口时钟
        
        GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2);//PA0引脚复用
        GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM2);//PA1引脚复用
        
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL ;
        GPIO_Init(GPIOA, &GPIO_InitStructure);//根据设定参数初始化GPIOB
            
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能定时器3的时钟
        
        TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
        TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR
        TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC
        TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
        TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
        
        TIM_ICInitTypeDef TIM_ICInitStructure;
        TIM_ICStructInit(&TIM_ICInitStructure);
        TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
        TIM_ICInitStructure.TIM_ICFilter = 0xF;
        TIM_ICInit(TIM2, &TIM_ICInitStructure);
        TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
        TIM_ICInitStructure.TIM_ICFilter = 0xF;
        TIM_ICInit(TIM2, &TIM_ICInitStructure);
        
        TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
        
        TIM_Cmd(TIM2, ENABLE);
    }
	
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能PA端口时钟

        GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);//PA6引脚复用
        GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);//PA7引脚复用

        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL ;
        GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//使能定时器3的时钟
        
        TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
        TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR
        TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC
        TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
        
        TIM_ICInitTypeDef TIM_ICInitStructure;
        TIM_ICStructInit(&TIM_ICInitStructure);
        TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
        TIM_ICInitStructure.TIM_ICFilter = 0xF;
        TIM_ICInit(TIM3, &TIM_ICInitStructure);
        TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
        TIM_ICInitStructure.TIM_ICFilter = 0xF;
        TIM_ICInit(TIM3, &TIM_ICInitStructure);
        
        TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
        
        TIM_Cmd(TIM3, ENABLE);
    }
}

/// @brief 获取指定编码器的值
/// @param Which Encoder_L or Encoder_R
/// @return 指定编码器的值
int16_t Encoder_GetVol(Encoder_Which Which)
{
    int16_t Vol=0;
    switch(Which)
    {
        case Encoder_L:
            Vol =TIM_GetCounter(TIM2);
            TIM_SetCounter(TIM2,0);
            break;
        case Encoder_R:
            Vol =TIM_GetCounter(TIM3);
            TIM_SetCounter(TIM3,0);
            break;
    }
    return Vol;
}
