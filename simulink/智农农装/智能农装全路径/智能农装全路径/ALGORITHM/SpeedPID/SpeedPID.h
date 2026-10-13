#ifndef _SPEED_PID_
#define _SPEED_PID_

#include "stm32f4xx.h"
#include "Motor.h"

/*使用PI PID控制电机运转*/

//速度环PID参数
#define SPEED_PID_KP 0.72f      //待调
#define SPEED_PID_KI 0.25f     //待调
#define SPEED_PID_KD 0.00f         //暂时不用

//速度环常用设定速度定义
#define SPEED_PID_SpeedUntraFast 90     //快速
#define SPEED_PID_SpeedFast 70          //较快
#define SPEED_PID_SpeedNormal 50        //一般
#define SPEED_PID_SpeedSlow 30          //较慢
#define SPEED_PID_SpeedUltraSlow 10     //很慢


#define SPEED_PID_EncoderMAX 450  //PWM输出最大时的编码器读取值 不换算速度 采取百分比控制

//积分限幅定义
#define SPEED_PID_INTEGRAL_LIMIT 1000
#define SPEED_PID_DEADZONE 3

typedef struct 
{
    float Speed_L_Now;
    float Speed_R_Now;

    float Speed_L_Target;
    float Speed_R_Target;

    float Speed_L_Error;
    float Speed_R_Error;

    float Speed_L_Error_Sum;
    float Speed_R_Error_Sum;

    float Speed_L_Error_Last;
    float Speed_R_Error_Last;

    float Speed_L_Output;
    float Speed_R_Output;
}SpeedStateStruct;

void SpeedPID_Init(void);
void SpeedPID_SetTarget(Motor_Type Which,float Target);
float SpeedPID_GetTarget(Motor_Type Which);
void SpeedPID_Control_TEMP(void);
//一下为编码器速度PID,以上为校赛伪PID
void SpeedPID_Refrash(void);
void SpeedPID_Control(void); //仅在主函数中可见
float SpeedPID_ReturnSpeed(Motor_Type Which);
float SpeedPID_ReturnSpeed_Target(Motor_Type Which);
float SpeedPID_GetOutput(Motor_Type Which);
void SpeedPID_Stop(void);
void SpeedPID_EndStop(void);
void SpeedPID_Stop_HC05(void);
void SpeedPID_EndStop_HC05(void);
void SpeedPID_ClearErr(void);

#endif
