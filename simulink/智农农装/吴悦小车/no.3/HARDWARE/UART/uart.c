#include "uart.h"
#include "stdio.h"
/*  上位机测数据    */

void USART1_Init(void)
{
	 /* 定义GPIO、NVIC和USART初始化的结构体 */
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	/* 使能GPIO和USART的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	/* 将USART TX（A9）的GPIO设置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	/* 将USART RX（A10）的GPIO设置为浮空输入模式 */
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/* 配置串口 */
	USART_InitStructure.USART_BaudRate=115200;										//波特率了设置为115200
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//不使用硬件流控制
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;						//使能接收和发送
	USART_InitStructure.USART_Parity=USART_Parity_No;								//不使用奇偶校验位
	USART_InitStructure.USART_StopBits=USART_StopBits_1;							//1位停止位
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;						//字长设置为8位
	USART_Init(USART1, &USART_InitStructure);	
	
	/* Usart1 NVIC配置 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);					//设置NVIC中断分组2
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;					//设置NVIC-USART1路线
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	/*初始化串口，开启串口接收中断 */
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	/* 使能串口1 */
	USART_Cmd(USART1,ENABLE);
//	USART_Cmd(USART1,DISABLE);
}
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	
}

//void USART1_IRQHandler(void)			 
//{

//		if( USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)  	   //接收中断  
//		{
//				
//			//function
//			
//			USART_ClearITPendingBit(USART1,USART_IT_RXNE);   //清除中断标志
//		}
//		
//}
