#ifndef _SERVO_SG90_H
#define _SERVO_SG90_H

#include "stm32f4xx.h"
#define Servo_Wait 0.025
#define Servo_Ready 0.075
typedef enum
{
    Servo_Arm_L,
    Servo_Arm_R,
    Servo_Gray_L,
    Servo_Gray_R,
}Servo_Which;

void Servo_Init(void);
void Servo_SetDuty(float Duty);

void Servo_Off(void);
void Servo_On(void);


#endif
