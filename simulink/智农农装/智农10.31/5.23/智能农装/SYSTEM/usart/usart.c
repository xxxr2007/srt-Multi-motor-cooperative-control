#include "usart.h"	

//--------------------留作备用----------------------
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
#endif
//-------------------------------------------------


//初始化USART1 用于OPENMV 注意：默认不使能串口1
void USART1_Init(uint32_t bound)
{
	//初始化 GPIOA9 10
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

        //设置复用功能
        GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
        GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

        //设置引脚属性
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
        GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
        GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;
        GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
        GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;

        GPIO_Init(GPIOA,&GPIO_InitStruct);
    }   

    {//初始化UART1 开启中断
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

        USART_InitTypeDef USART_InitStruct;
        USART_InitStruct.USART_BaudRate=bound;
        USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
        USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
        USART_InitStruct.USART_Parity=USART_Parity_No;
        USART_InitStruct.USART_StopBits=USART_StopBits_1;
        USART_InitStruct.USART_WordLength=USART_WordLength_8b;

        USART_Init(USART1,&USART_InitStruct);

        USART_Cmd(USART1, ENABLE); 
    }
	
	{//设置中断
        USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);//默认不使能接收中断 
    }

    {//初始化NVIC
        NVIC_InitTypeDef NVIC_InitStruct;
        NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;
        NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
        NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;//颜色识别
        NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
        NVIC_Init(&NVIC_InitStruct);
    }
}

//设置串口1的工作状态
void USART1_SetState(FunctionalState State)
{
	USART_Cmd(USART1, State);
}

//初始化USART2 用于陀螺仪 注意：默认不使能串口2
void USART2_Init(uint32_t bound)
{
	//初始化 GPIOD5 6
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

		//设置复用功能
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);

		//设置引脚属性
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6;
		GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
		GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOD,&GPIO_InitStruct);
	}   

	{//初始化UART2 开启中断
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

		USART_InitTypeDef USART_InitStruct;
		USART_InitStruct.USART_BaudRate=bound;
		USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
		USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
		USART_InitStruct.USART_Parity=USART_Parity_No;
		USART_InitStruct.USART_StopBits=USART_StopBits_1;
		USART_InitStruct.USART_WordLength=USART_WordLength_8b;

		USART_Init(USART2,&USART_InitStruct);

		USART_Cmd(USART2, ENABLE);  
	}
	
	{//设置中断
		USART_ITConfig(USART2,USART_IT_RXNE,DISABLE); //默认不使能接收中断
	}

	{//初始化NVIC
		NVIC_InitTypeDef NVIC_InitStruct;
		NVIC_InitStruct.NVIC_IRQChannel=USART2_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;//陀螺仪
		NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;//响应优先级0
		NVIC_Init(&NVIC_InitStruct);
	}
}

//设置串口2的工作状态
void USART2_SetState(FunctionalState State)
{
	USART_Cmd(USART2, State);
}

//初始化UART4 用于HC05蓝牙 注意：默认使能串口4
void UART4_Init(uint32_t bound)
{
	//初始化 GPIOC10 11
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

		//设置复用功能
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4);
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4);

		//设置引脚属性
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
		GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
		GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOC,&GPIO_InitStruct);
	}   

	{//初始化UART4 开启中断
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);

		USART_InitTypeDef USART_InitStruct;
		USART_InitStruct.USART_BaudRate=bound;
		USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
		USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
		USART_InitStruct.USART_Parity=USART_Parity_No;
		USART_InitStruct.USART_StopBits=USART_StopBits_1;
		USART_InitStruct.USART_WordLength=USART_WordLength_8b;

		USART_Init(UART4,&USART_InitStruct);

		USART_Cmd(UART4, ENABLE);  //默认使能串口4 
	}
	
	{//设置中断
		USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	}

	{//初始化NVIC
		NVIC_InitTypeDef NVIC_InitStruct;
		NVIC_InitStruct.NVIC_IRQChannel=UART4_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;//蓝牙
		NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;//响应优先级0
		NVIC_Init(&NVIC_InitStruct);
	}
}

//设置串口4的工作状态
void UART4_SetState(FunctionalState State)
{
	USART_Cmd(UART4, State);
}

//使用某一个串口发送一个字节
void SendByte(USART_TypeDef* USARTx, uint8_t Byte)
{
	USART_SendData(USARTx, Byte);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

//使用某一个串口发送一个字符串
void SendString(USART_TypeDef* USARTx, uint8_t* String)
{
	while(*String)
	{
		SendByte(USARTx, *String);
		String++;
	}
}
