#ifndef _ACTION_H_
#define _ACTION_H_

#include "stm32f4xx.h"


#define LongTrack_Closer 3
#define LongTrack_normal 6
#define LongTrack_DeadZone_! 2
#define LongTrack_DeadZone_2 1
#include "csb.h"
/////////////////////循迹相关定义/////////////////////
typedef enum
{
    Left,Right,Front,Back
}Dir_type;

typedef enum
{
    TrackBaseSpeed_UltraSlow=10,
    TrackBaseSpeed_Slow=30,
    TrackBaseSpeed_Normal=50,
    TrackBaseSpeed_Fast=70,
    TrackBaseSpeed_UltraFast=90,
}TrackBaseSpeedType;

typedef struct{
	float Now_Distance;
  float Err;
  float Err_l1;
  float Err_l2;
	float Sum;
	float Out;

}TrackState;
typedef struct{
	float Now_ANG;
  float Err;
	float Sum;
  float Err_l1;
  float Err_l2;
	float Out;
}TLYState;
void Action_Start(void);

void Action_EncoderADD(int16_t EncoderCountToAdd);
void Action_EncoderCLEAR(void);
void Action_EncoderWait(int EncoderWaitCount);

void Action_StartIn(float Speed);
float Compute_CircleDiff(float a,float b);


void Track(void);
void Track_Set(float Speed,CSB_Type Which_Type,float Target,Dir_type Dir);

void TLY_GetTar(void);
void TLY_Set(float Speed);
void TLY_Control(void);
void Action_TLY_Enable(void);
void Action_TLY_Disable(void);
void Action_Track_Enable(void);
void Action_Track_Disable(void);

void Action_XZ(void);
void Action_Turn(float AngleToChange,Dir_type Which_Dir,Dir_type TrackDir);
void Path_GL(void);

void Track_ChangePD(float p_TC,float d_TC,float lim_TC);
void Action_Turn2_0(float AngleToChange,Dir_type Which_Dir);
//------------------------------------------以下为老代码------------------------------------------------//
/*void GrayTrackModeSet(TrackModeType TrackMode_ToSet);
void GrayTrack_Set( TrackBaseSpeedType GrayTrackBaseSpeed_ToSet,
                    int GraySarchStartPoint_ToSet,
                    GrayFindDirection GraySarchDirection_ToSet,
                    uint16_t GrayEnable_ToSet,
                    float TargetLinePosition_ToSet);

TrackModeType GrayTrack_ReturnMode(void);
float GrayTrack_GetGrayLinePosition(void);

/////////////////////编码器相关定义/////////////////////

void Action_EncoderADD(int16_t EncoderCountToAdd);
void Action_EncoderCLEAR(void);
void Action_EncoderWait(int EncoderWaitCount);

/////////////////////总体动作相关定义/////////////////////

void Action_JUSTgo(int8_t Speed);
void Action_JUSTgoMs(int8_t Speed,uint16_t Time_ms);
void Action_JUSTgoEncoder(int8_t Speed,int EncoderCount);
void Action_WaitForLine(ActionDirectionType Direction);
void Action_WaitForLineOnce(ActionDirectionType Direction);
void Action_WaitForGorkUp(void);
void Action_WaitForGorkDown(void);
void Action_WaitForFront_Appear(void);
void Action_WaitForFront_Disappear(void);
void Action_WaitForRedLineAppear(void);
void Action_WaitForRedLineAppearBoth(void);
void Action_WaitForGo(void);
void Action_JUSTgoCM(int8_t Speed,int CM);

typedef enum
{
    WaitType_GD_LL,
    WaitType_GD_L,
    WaitType_GD_R,
    WaitType_GD_RR,
    WaitType_GD_Gork,
    WaitType_GD_ML,
    WaitType_GD_MR,
}WaitType;

void Action_WaitWhatTimes(WaitType What,uint8_t Times);

//提速相关

void Action_TrackEncoder(uint16_t Distance_cm);


void Action_TrackSpeedUp_Time(TrackBaseSpeedType TrackBaseSpeed_ToSet,uint16_t Time_ms);
void Action_TrackSpeedUp_Distance(TrackBaseSpeedType TrackBaseSpeed_ToSet,uint16_t Distance_cm);
void Action_TrackSpeedUp_Sensor(TrackBaseSpeedType TrackBaseSpeed_ToSet,WaitType Sensor);


extern uint8_t Gray01Data[17];
//void Action_TurnLine(ActionDirectionType Direction, uint8_t WhichGray, uint16_t delaytimes_ms);

/////////////////////舵机相关定义/////////////////////

#define ServoGray_Up   0.025f      //待调
#define ServoGray_Down 0.100f      //待调

typedef enum 
{
    ServoState_Up,
    ServoState_Down
}ServoStateType;

typedef enum
{
    Servo_ArmL,
    Servo_ArmR
}ServoWhichType;

void Action_ServoSetState(ServoWhichType ServoWhich,ServoStateType ServoState);

///////////////////速度相关定义/////////////////////
*/
#endif
