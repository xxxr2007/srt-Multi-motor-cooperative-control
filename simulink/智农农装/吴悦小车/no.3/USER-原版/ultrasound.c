#include "ultrasound.h"
#include "delay.h"

#define Trig1 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)
#define Trig2 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)
#define Trig3 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5)
#define Trig4 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)
#define Trig5 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9)
#define Trig6 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)

/***************************************************
函数功能:超声波初始化，TIM5
***************************************************/
void Ultrasound_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD,ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_10;     //发出脉冲的输出口 PD0 2 4 6 8 10  Trig
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_11;     //接收脉冲的输入口 PA4 Echo
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;      //下拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD , &GPIO_InitStructure);
	
	GPIO_ResetBits( GPIOD,GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_10);   //软件下拉 置低电平
	
	//TIM5初始化
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);				//72M / 72 = 1us
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_0);
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	GPIO_ResetBits(GPIOD,GPIO_Pin_4);
	GPIO_ResetBits(GPIOD,GPIO_Pin_6);
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);
	GPIO_ResetBits(GPIOD,GPIO_Pin_10);
}


/***************************************************
函数功能:超声波获取距离
返回：距离
单位：mm
注意：返回10000则超过量程
***************************************************/
float Ultrasound_Getdistance1(void)
{
	u32 count=0;
	float speed;
	speed=340.0;
	GPIO_SetBits(GPIOD,GPIO_Pin_0);				//触发
	delay_us(11);
	GPIO_ResetBits(GPIOD,GPIO_Pin_0);	
	TIM_SetCounter(TIM5, 0);
	TIM_Cmd(TIM5, ENABLE);
	while(Trig1==0)					//等待收到信号
	{
		if(TIM_GetCounter(TIM5)>24024)			//如果1.176ms内未收到回复   50cm
		{
			TIM_Cmd(TIM5, DISABLE);
			return 10000;
		}
	}
	TIM_SetCounter(TIM5, 0);				//接受到信号开始计时
	while(Trig1==1)
	{
		if(TIM_GetCounter(TIM5)>24024)
		{
			TIM_Cmd(TIM5, DISABLE);			//超时出错结束
			return 10000;
		}
	}
	count=TIM_GetCounter(TIM5);				//保存计数
	TIM_Cmd(TIM5, DISABLE);
	//声速340m/s  计数单位us  结果单位mm   
	return  (u16)(count*speed/2000.0); //return  (u16)(count*speed*1000/1000000/2);
}
float Ultrasound_Getdistance2(void)
{
	u32 count=0;
	float speed = 340;                      //340不准  365左右更接近实际值
	GPIO_SetBits(GPIOD,GPIO_Pin_2);			//触发
	delay_us(11);                           //超声波持续11us
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);	    //超声波停止
	TIM_SetCounter(TIM5, 0);                //自动重装值归零
	TIM_Cmd(TIM5, ENABLE);
	while(Trig2 == 0)					//等待收到信号
	{
		if(TIM_GetCounter(TIM5) > 2352)	//2352表示如果1.176ms*2内未收到回复
		{
			TIM_Cmd( TIM5, DISABLE);
			return 10000;
		}
	}
	TIM_SetCounter(TIM5, 0);				//接受到信号开始计时
	while(Trig2 == 1)
	{
		if(TIM_GetCounter(TIM5) > 2352)    //2352
		{
			TIM_Cmd( TIM5, DISABLE);		//超时出错结束
			return 10000;
		}
	}
	count = TIM_GetCounter( TIM5 );			//保存计数
	TIM_Cmd( TIM5, DISABLE );
	//声速340m/s  计数单位us  结果单位mm   
	return  (float)(count*speed/2000.0); //return  (u16)(count*speed*1000/1000000/2);
}
float Ultrasound_Getdistance3(void)
{
	u32 count=0;
	float speed = 340;                      //340不准  365左右更接近实际值
	GPIO_SetBits(GPIOD,GPIO_Pin_4);			//触发
	delay_us(11);                           //超声波持续11us
	GPIO_ResetBits(GPIOD,GPIO_Pin_4);	    //超声波停止
	TIM_SetCounter(TIM5, 0);                //自动重装值归零
	TIM_Cmd(TIM5, ENABLE);
	while(Trig3 == 0)					//等待收到信号
	{
		if(TIM_GetCounter(TIM5) > 2352)	//2352表示如果1.176ms*2内未收到回复
		{
			TIM_Cmd( TIM5, DISABLE);
			return 10000;
		}
	}
	TIM_SetCounter(TIM5, 0);				//接受到信号开始计时
	while(Trig3 == 1)
	{
		if(TIM_GetCounter(TIM5) > 2352)    //2352
		{
			TIM_Cmd( TIM5, DISABLE);		//超时出错结束
			return 10000;
		}
	}
	count = TIM_GetCounter( TIM5 );			//保存计数
	TIM_Cmd( TIM5, DISABLE );
	//声速340m/s  计数单位us  结果单位mm   
	return  (float)(count*speed/2000.0); //return  (u16)(count*speed*1000/1000000/2);
}
float Ultrasound_Getdistance4(void)
{
	u32 count=0;
	float speed = 340;                      //340不准  365左右更接近实际值
	GPIO_SetBits(GPIOD,GPIO_Pin_6);			//触发
	delay_us(11);                           //超声波持续11us
	GPIO_ResetBits(GPIOD,GPIO_Pin_6);	    //超声波停止
	TIM_SetCounter(TIM5, 0);                //自动重装值归零
	TIM_Cmd(TIM5, ENABLE);
	while(Trig4 == 0)					//等待收到信号
	{
		if(TIM_GetCounter(TIM5) > 2352)	//2352表示如果1.176ms*2内未收到回复
		{
			TIM_Cmd( TIM5, DISABLE);
			return 10000;
		}
	}
	TIM_SetCounter(TIM5, 0);				//接受到信号开始计时
	while(Trig4 == 1)
	{
		if(TIM_GetCounter(TIM5) > 2352)    //2352
		{
			TIM_Cmd( TIM5, DISABLE);		//超时出错结束
			return 10000;
		}
	}
	count = TIM_GetCounter( TIM5 );			//保存计数
	TIM_Cmd( TIM5, DISABLE );
	//声速340m/s  计数单位us  结果单位mm   
	return  (float)(count*speed/2000.0); //return  (u16)(count*speed*1000/1000000/2);
}
float Ultrasound_Getdistance5(void)
{
	u32 count=0;
	float speed = 340;                      //340不准  365左右更接近实际值
	GPIO_SetBits(GPIOD,GPIO_Pin_8);			//触发
	delay_us(11);                           //超声波持续11us
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);	    //超声波停止
	TIM_SetCounter(TIM5, 0);                //自动重装值归零
	TIM_Cmd(TIM5, ENABLE);
	while(Trig5 == 0)					//等待收到信号
	{
		if(TIM_GetCounter(TIM5) > 2352)	//2352表示如果1.176ms*2内未收到回复
		{
			TIM_Cmd( TIM5, DISABLE);
			return 10000;
		}
	}
	TIM_SetCounter(TIM5, 0);				//接受到信号开始计时
	while(Trig5 == 1)
	{
		if(TIM_GetCounter(TIM5) > 2352)    //2352
		{
			TIM_Cmd( TIM5, DISABLE);		//超时出错结束
			return 10000;
		}
	}
	count = TIM_GetCounter( TIM5 );			//保存计数
	TIM_Cmd( TIM5, DISABLE );
	//声速340m/s  计数单位us  结果单位mm   
	return  (float)(count*speed/2000.0); //return  (u16)(count*speed*1000/1000000/2);
}
float Ultrasound_Getdistance6(void)
{
	u32 count=0;
	float speed = 340;                      //340不准  365左右更接近实际值
	GPIO_SetBits(GPIOD,GPIO_Pin_10);			//触发
	delay_us(11);                           //超声波持续11us
	GPIO_ResetBits(GPIOD,GPIO_Pin_10);	    //超声波停止
	TIM_SetCounter(TIM5, 0);                //自动重装值归零
	TIM_Cmd(TIM5, ENABLE);
	while(Trig6 == 0)					//等待收到信号
	{
		if(TIM_GetCounter(TIM5) > 2352)	//2352表示如果1.176ms*2内未收到回复
		{
			TIM_Cmd( TIM5, DISABLE);
			return 10000;
		}
	}
	TIM_SetCounter(TIM5, 0);				//接受到信号开始计时
	while(Trig6 == 1)
	{
		if(TIM_GetCounter(TIM5) > 2352)    //2352
		{
			TIM_Cmd( TIM5, DISABLE);		//超时出错结束
			return 10000;
		}
	}
	count = TIM_GetCounter( TIM5 );			//保存计数
	TIM_Cmd( TIM5, DISABLE );
	//声速340m/s  计数单位us  结果单位mm   
	return  (float)(count*speed/2000.0); //return  (u16)(count*speed*1000/1000000/2);
}

