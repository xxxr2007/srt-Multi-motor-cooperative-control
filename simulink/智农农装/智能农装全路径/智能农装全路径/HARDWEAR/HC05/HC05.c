#include "HC05.h"
#include "usart.h"
#include <stdio.h>
#include <stdarg.h>

//蓝牙串口连接至UART4 初始化 38400 8-N-1
uint8_t HC05_RxData;
uint8_t HC05_RxFlag;

//初始化 默认启用接收中断
void HC05_Init(void)
{
    UART4_Init(9600);
}

void HC05_SendByte(uint8_t Byte)
{
	USART_SendData(UART4, Byte);
	while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
}

void HC05_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		HC05_SendByte(Array[i]);
	}
}

void HC05_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		HC05_SendByte(String[i]);
	}
}

uint32_t HC05_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void HC05_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		HC05_SendByte(Number / HC05_Pow(10, Length - i - 1) % 10 + '0');
	}
}

int fputc(int ch, FILE *f)
{
	HC05_SendByte(ch);
	return ch;
}

void HC05_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	HC05_SendString(String);
}

uint8_t HC05_GetRxFlag(void)
{
	if (HC05_RxFlag == 1)
	{
		HC05_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t HC05_GetRxData(void)
{
	return HC05_RxData;
}

void UART4_IRQHandler(void)
{
	if (USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
	{
		HC05_RxData = USART_ReceiveData(UART4);
		HC05_RxFlag = 1;
		// HC05_Printf("%c", HC05_RxData);
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
	}
}

static uint8_t OpenMV_Color=0;
static uint8_t OpenMV_HaveData=0;

//OpenMV颜色识别后通过串口接收数据
/*void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART4, USART_IT_RXNE) != RESET)
    {
        OpenMV_Color = USART_ReceiveData(USART1);
        OpenMV_HaveData = 1;
    }
    USART_ClearITPendingBit(USART1, USART_IT_RXNE); // 清除中断标志
}*/

//OpenMV初始化 注意:OpenMV串口接收中断默认关闭
/*void OpenMV_Init(void)
{
    USART4_Init(9600);
}*/

/// @brief 使能或失能OpenMV串口中断
/// @param state DISABLE:失能 ENABLE:使能
void OpenMV_SetState(FunctionalState state)
{
    if(state==ENABLE)
    {
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    }
    else
    {
        USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    }
}

/// @brief 等待一次OpenMV数据 会阻塞
/// @param void
/// @return 刚得到的OpenMV颜色数据
OpenMV_ColorType OpenMV_WaitColor(void)
{
    
    while(OpenMV_HaveData==0);
    
    OpenMV_HaveData = 0;
    return (OpenMV_ColorType)OpenMV_Color;
}

/// @brief 直接返回最近一次的OpenMV颜色数据 不会阻塞
/// @param  void
/// @return 最近一次的OpenMV颜色数据
OpenMV_ColorType OpenMV_GetColor(void)
{
    HC05_SendByte(OpenMV_Color);
    return (OpenMV_ColorType)OpenMV_Color;
}