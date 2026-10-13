#ifndef _KEY_H__
#define _KEY_H__

#include "stm32f4xx.h"

#define UP_Key GPIO_Pin_15
#define DOWN_Key GPIO_Pin_14
#define OK_Key GPIO_Pin_13
#define EXIT_Key GPIO_Pin_12

#define Check_ItemMax 6

#define Key_Port GPIOB

typedef enum{NONE,UP,DOWN,OK,EXIT}Key_type;
void Key_Init(void);
Key_type Key_GetVol(void);
void Key_Let_sCheck(void);
void TEMP(void);
void GD_Check(void);
void CSB_Check(void);
void Encoder_Check(void);
#endif
