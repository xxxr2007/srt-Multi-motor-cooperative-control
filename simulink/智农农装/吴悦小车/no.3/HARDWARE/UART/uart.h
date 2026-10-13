#ifndef __UART_H
#define __UART_H

#include "sys.h"
#include "stm32f10x.h"

void USART1_Init(void);//串口1初始化并启动
void Serial_SendByte(uint8_t Byte);


#endif
