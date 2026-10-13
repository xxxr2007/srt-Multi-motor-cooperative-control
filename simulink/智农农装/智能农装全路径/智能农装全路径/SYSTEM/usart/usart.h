#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

//void USART1_Init(uint32_t bound);
void USART2_Init(uint32_t bound);
void UART4_Init(uint32_t bound);

void USART1_SetState(FunctionalState State);
void USART2_SetState(FunctionalState State);
void UART4_SetState(FunctionalState State);

void SendByte(USART_TypeDef* USARTx, uint8_t Byte);
void SendString(USART_TypeDef* USARTx, uint8_t* String);

#endif
