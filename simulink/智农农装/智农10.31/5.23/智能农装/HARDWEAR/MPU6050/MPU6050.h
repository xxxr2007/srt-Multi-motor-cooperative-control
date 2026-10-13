#ifndef _MPU6050_H
#define _MPU6050_H

#include "stm32f4xx.h"

typedef struct
{
    float Pitch;    //俯仰角
    float Roll;     //横滚角
    float Yaw;      //偏航角
}MPU6050_AngleType;//姿态

typedef struct 
{
    float ACC_X;
    float ACC_Y;
    float ACC_Z;
}MPU6050_AccType;//加速度 单位g

typedef struct 
{
    float PAL_X;
    float PAL_Y;
    float PAL_Z;
}MPU6050_PalType;//角速度

extern MPU6050_AngleType MPU6050_AngleData;
extern MPU6050_AccType MPU6050_AccData;
extern MPU6050_PalType MPU6050_PalData;


extern uint8_t Angle_Freshed;         //姿态数据更新标志
extern uint8_t ACC_Freshed;           //加速度数据更新标志
extern uint8_t Palstance_Freshed;     //角速度数据更新标志
extern uint8_t Angle_Aux_Freshed;     //辅助姿态数据更新标志

extern MPU6050_AngleType MPU6050_ParsedData;

void MPU6050_Init(void);
void MPU6050_SetState(FunctionalState state);
void MPU6050_WaitForFresh(void);

#endif
