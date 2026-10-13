#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"
#include "stm32f10x.h"
#include "delay.h"
#include "stdlib.h"

void Timer2_PWM_Init(void);
void Timer3_PWM_Init(void);
void PWM_SetCompare1(uint16_t Compare1);
void PWM_SetCompare1(uint16_t Compare2);
void PWM_SetCompare1(uint16_t Compare3);
void PWM_SetCompare1(uint16_t Compare4);

void Motor_Polarity_Init(void);
void Motor_Polarity_Stop(void);
void Motor1_Polarity_Go(void);
void Motor2_Polarity_Go(void);
void Motor3_Polarity_Go(void);
void Motor4_Polarity_Go(void);
void Motor1_Polarity_Back(void);
void Motor2_Polarity_Back(void);
void Motor3_Polarity_Back(void);
void Motor4_Polarity_Back(void);
void Motor(int pwm1 , int pwm3);
void Motor1(int pwm2 , int pwm4);

#endif

