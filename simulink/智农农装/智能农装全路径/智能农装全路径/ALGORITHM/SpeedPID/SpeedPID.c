#include "SpeedPID.h"
#include "Encoder.h"
#include "Motor.h"
#include "Action.h"
#include "timers.h"
#include "HC05.h"
#include "delay.h"

int16_t OriL,OriR;

static SpeedStateStruct SpeedState;//禁止文件外直接修改

extern void Motor_SetOuput(Motor_Type Which, int Output);

extern uint8_t EncoderWaitState;//编码器Wait标志位 1:等待 0:完成

static uint8_t StopFlag = 0;

static uint8_t StopFlag_HC05 = 0;

//设定初始速度
void SpeedPID_Init(void)
{
    SpeedState.Speed_L_Target = 0;
    SpeedState.Speed_R_Target = 0;
}

void SpeedPID_Control_TEMP(void){
    Motor_SetOuput(Motor_L,SpeedState.Speed_L_Target);
    Motor_SetOuput(Motor_R,SpeedState.Speed_R_Target);
    return;
}

/// @brief 更改位于结构体中的目标速度
/// @param Which Motor_L,Motor_R
/// @param Target -100~100的浮点数
void SpeedPID_SetTarget(Motor_Type Which,float Target)
{
    switch(Which)
    {
        case Motor_L:
            SpeedState.Speed_L_Target = Target;
            break;
        case Motor_R:
            SpeedState.Speed_R_Target = Target;
            break;
    }
}

float SpeedPID_GetTarget(Motor_Type Which)
{
    switch(Which)
    {
        case Motor_L:
            return SpeedState.Speed_L_Target;
        case Motor_R:
            return SpeedState.Speed_R_Target;
    }
    return 0;
}

float SpeedPID_ReturnSpeed_Target(Motor_Type Which)
{
    switch(Which)
    {
        case Motor_L:
            return SpeedState.Speed_L_Target;
        case Motor_R:
            return SpeedState.Speed_R_Target;
    }
    return 0;
}

//-----------------------------------------------------------------------//

void SpeedPID_Refrash(void)
{
    //读取编码器速度
    int16_t SpeedOri_L=Encoder_GetVol(Encoder_L);
    int16_t SpeedOri_R=Encoder_GetVol(Encoder_R);
    OriL=SpeedOri_L;
    OriR=SpeedOri_R;

    if(EncoderWaitState==1)
    {
        Action_EncoderADD((SpeedOri_L+SpeedOri_R)/2);//取两个编码器的平均值
    }
    else
    {
        Action_EncoderCLEAR();
    }
    
    //计算速度
    SpeedState.Speed_L_Now = (float)SpeedOri_L/SPEED_PID_EncoderMAX;
    SpeedState.Speed_R_Now = (float)SpeedOri_R/SPEED_PID_EncoderMAX;

    //放大100倍 全车速度采取0~100的范围
    SpeedState.Speed_L_Now *= 100;
    SpeedState.Speed_R_Now *= 100;
}

/// @brief 速度环PID控制 放置于定时服务函数中 10ms/次 速度环PID控制 速度使用结构体中的数据 仅在主函数中可见
void SpeedPID_Control(void)
{
    //获取当前速度
    SpeedPID_Refrash();

    if(StopFlag==1)
    {
        
        SpeedState.Speed_L_Target=0;
        SpeedState.Speed_R_Target=0;
				Motor_SetOuput(Motor_L,0);
				Motor_SetOuput(Motor_L,0);

    }

    if(StopFlag_HC05==1)
    {
        SpeedState.Speed_L_Target=0;
        SpeedState.Speed_R_Target=0;
    }

    //计算误差
    SpeedState.Speed_L_Error = SpeedState.Speed_L_Target - SpeedState.Speed_L_Now;
    SpeedState.Speed_R_Error = SpeedState.Speed_R_Target - SpeedState.Speed_R_Now;

    //计算输出值
    SpeedState.Speed_L_Output   = (SPEED_PID_KP)*SpeedState.Speed_L_Error                                                     //比例项                 
                                + (SPEED_PID_KI)*SpeedState.Speed_L_Error_Sum                                                 //积分项
                                + SPEED_PID_KD*(SpeedState.Speed_L_Error - SpeedState.Speed_L_Error_Last);                  //微分项

    SpeedState.Speed_R_Output   = (SPEED_PID_KP)*SpeedState.Speed_R_Error                                                     //比例项
                                + (SPEED_PID_KI)*SpeedState.Speed_R_Error_Sum                                                 //积分项                         
                                + SPEED_PID_KD*(SpeedState.Speed_R_Error - SpeedState.Speed_R_Error_Last);                  //微分项

    //更新误差
    SpeedState.Speed_L_Error_Last = SpeedState.Speed_L_Error;
    SpeedState.Speed_R_Error_Last = SpeedState.Speed_R_Error;

    //更新积分项
    SpeedState.Speed_L_Error_Sum += SpeedState.Speed_L_Error;
    SpeedState.Speed_R_Error_Sum += SpeedState.Speed_R_Error;

    //积分限幅
    if(SpeedState.Speed_L_Error_Sum>SPEED_PID_INTEGRAL_LIMIT)
    {
        SpeedState.Speed_L_Error_Sum = SPEED_PID_INTEGRAL_LIMIT;
    }
    else if(SpeedState.Speed_L_Error_Sum<-SPEED_PID_INTEGRAL_LIMIT)
    {
        SpeedState.Speed_L_Error_Sum = -SPEED_PID_INTEGRAL_LIMIT;
    }

    if(SpeedState.Speed_R_Error_Sum>SPEED_PID_INTEGRAL_LIMIT)
    {
        SpeedState.Speed_R_Error_Sum = SPEED_PID_INTEGRAL_LIMIT;
    }
    else if(SpeedState.Speed_R_Error_Sum<-SPEED_PID_INTEGRAL_LIMIT)
    {
        SpeedState.Speed_R_Error_Sum = -SPEED_PID_INTEGRAL_LIMIT;
    }

    SpeedState.Speed_L_Output+=SpeedState.Speed_L_Target*0.8f;
    SpeedState.Speed_R_Output+=SpeedState.Speed_R_Target*0.8f;

    //死区控制
    if(SpeedState.Speed_L_Output>0&&SpeedState.Speed_L_Output<SPEED_PID_DEADZONE)
    {
        SpeedState.Speed_L_Output = 0;
    }
    else if(SpeedState.Speed_L_Output<0&&SpeedState.Speed_L_Output>-SPEED_PID_DEADZONE)
    {
        SpeedState.Speed_L_Output = -0;
    }

    if(SpeedState.Speed_R_Output>0&&SpeedState.Speed_R_Output<SPEED_PID_DEADZONE)
    {
        SpeedState.Speed_R_Output = 0;
    }
    else if(SpeedState.Speed_R_Output<0&&SpeedState.Speed_R_Output>-SPEED_PID_DEADZONE)
    {
        SpeedState.Speed_R_Output = -0;
    }

    //输出到Motor
    Motor_SetOuput(Motor_L,(SpeedState.Speed_L_Output));
    Motor_SetOuput(Motor_R,(SpeedState.Speed_R_Output));
		
//    HC05_Printf("%f,%f,%f \n",SpeedState.Speed_L_Now,SpeedState.Speed_R_Now,SpeedState.Speed_L_Target);
}

//获取当前速度
float SpeedPID_ReturnSpeed(Motor_Type Which)
{
    switch(Which)
    {
        case Motor_L:
            return SpeedState.Speed_L_Now;
        case Motor_R:
            return SpeedState.Speed_R_Now;
    }
    return 0;
}


float SpeedPID_GetOutput(Motor_Type Which)
{
    switch(Which)
    {
        case Motor_L:
            return SpeedState.Speed_L_Output;
        case Motor_R:
            return SpeedState.Speed_R_Output;
    }
    return 0;
}

//停止 停止标志位 取消停止必须调用SpeedPID_EndStop
void SpeedPID_Stop(void)
{
    StopFlag = 1;
		Timers_TIM7_Stop();
		Motor_SetOuput(Motor_L,0);
    Motor_SetOuput(Motor_R,0);
		TIM_SetCounter(TIM3,0);
		TIM_SetCounter(TIM3,0);
		SpeedPID_ClearErr();
		Timers_TIM7_Start();
    SpeedPID_SetTarget(Motor_L,0);
    SpeedPID_SetTarget(Motor_R,0);
}

//取消停止
void SpeedPID_EndStop(void)
{
    StopFlag = 0;
}

//停止 另一个停止标志位 用于HC05上位机控制 取消停止必须调用SpeedPID_EndStop_HC05
void SpeedPID_Stop_HC05(void)
{
    StopFlag_HC05 = 1;
    SpeedPID_SetTarget(Motor_L,0);
    SpeedPID_SetTarget(Motor_R,0);
}

//取消停止 另一个停止标志位 用于HC05上位机控制
void SpeedPID_EndStop_HC05(void)
{
    StopFlag_HC05 = 0;
}

void SpeedPID_ClearErr(void){
	SpeedState.Speed_L_Error=0;
	SpeedState.Speed_R_Error=0;
	
	SpeedState.Speed_L_Error_Last=0;
	SpeedState.Speed_R_Error_Last=0;

	SpeedState.Speed_L_Error_Sum=0;
	SpeedState.Speed_R_Error_Sum=0;
	
}
