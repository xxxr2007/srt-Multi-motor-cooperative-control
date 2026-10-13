#ifndef _GD_H_
#define _GD_H_

#include "stm32f4xx.h"

typedef enum
{
    GD_State_Detected = 0,
    GD_State_NotDetected =1
}GD_State;

#define GD12_Pin GPIO_Pin_3
#define GD11_Pin GPIO_Pin_2
#define GD10_Pin GPIO_Pin_1
#define GD9_Pin GPIO_Pin_0
#define GD8_Pin GPIO_Pin_8
#define GD7_Pin GPIO_Pin_9
#define GD6_Pin GPIO_Pin_10
#define GD5_Pin GPIO_Pin_11
#define GD4_Pin GPIO_Pin_12
#define GD3_Pin GPIO_Pin_13
#define GD2_Pin GPIO_Pin_14
#define GD1_Pin GPIO_Pin_15

#define GD_Right_F GD10_Pin
#define GD_Middle_F GD11_Pin
#define GD_Left_F GD12_Pin
#define GD_Right_B GD7_Pin
#define GD_Middle_B GD8_Pin
#define GD_Left_B GD9_Pin

#define GD_Color GD6_Pin
#define GD_Prot     GPIOD

typedef enum{RF,MF,LF,RB,MB,LB,CL}GD_type;

void GD_Init(void);
GD_State GD_Wait_Ctype(GD_type Which_type,int What_times); 
GD_State GD_Read_Ctype(GD_type Which_type);



#endif
