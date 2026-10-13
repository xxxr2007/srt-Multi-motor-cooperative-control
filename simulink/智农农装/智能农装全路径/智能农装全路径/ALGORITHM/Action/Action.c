#include "Action.h"
#include "SpeedPID.h"
#include "MPU6050.h"
#include "Delay.h"
#include "GD.h"
#include "Servo.h"
#include "HC05.h"
#include "oled.h"
#include "FMQ.h"
#include "Encoder.h"
#include "csb.h"
/////////////循迹动作/////////////
static TrackState TrackState_Struct;
static TLYState TLYState_Struct;
float Track_kp=11.2f;   //1.5
float Track_kd=4.3f;	//2.0
float Track_ki=0.00f;
float Track_lim=45.0f;
float Track_DeadZone=1.3f;		//1.3
float TLY_DeadZone=2.0f;
float TLY_kp=6.0f;//3.8
float TLY_ki=2.0f;  
float TLY_kd=1.4f;

static float Track_Speed;
static float TLY_Speed;
static CSB_Type Track_Csb;
static float Track_Tar;
static Dir_type Track_Dir;
static int Track_EnFlag=0;
static int TLY_EnFlag=0;


static float TLY_Tar;
static float Speed_L;
static float Speed_R;

void Path_GL(void){
	SpeedPID_Stop();
	Action_Track_Disable();
	CSB_Disable();
	MPU6050_SetState(DISABLE);
	
	SpeedPID_SetTarget(Motor_L,0);
	SpeedPID_SetTarget(Motor_R,0);
}

void Stop(void){
	SpeedPID_SetTarget(Motor_L,0);
	SpeedPID_SetTarget(Motor_R,0);
}

void Action_Start(void){
	
	GD_Wait_Ctype(MF,1);
	FMQ_Beep(3);
	
	while(GD_Read_Ctype(MF)==GD_State_Detected);
	FMQ_Beep(5);
	return;
}

void Action_Track_Enable(void){
	Track_EnFlag=1;
}

void Action_Track_Disable(void){
	Track_EnFlag=0;
	TrackState_Struct.Err=0;
	TrackState_Struct.Err_l1=0;
	TrackState_Struct.Err_l2=0;
}

void Action_TLY_Enable(void){
	TLY_EnFlag=1;
}

void Action_TLY_Disable(void){
	TLY_EnFlag=0;
	TLYState_Struct.Err=0;
	TLYState_Struct.Err_l1=0;
	TLYState_Struct.Err_l2=0;
	TLYState_Struct.Sum=0;
}
static int EncoderCount;
uint8_t EncoderWaitState;//1:等待 0:完成

void Action_EncoderADD(int16_t EncoderCountToAdd)
{
    EncoderCount += EncoderCountToAdd;
}

/// @brief 清除编码器累计计数
void Action_EncoderCLEAR(void)
{
    EncoderCount = 0;
}

/// @brief 主函数在此函数中停留 等待编码器累计计数到达指定值
void Action_EncoderWait(int EncoderWaitCount)
{
    Action_EncoderCLEAR();
    EncoderWaitState=1;
    if(EncoderWaitCount<0)
    {
        while(EncoderCount>EncoderWaitCount);
        EncoderWaitState=0;
    }
    else
    {
        while(EncoderCount<EncoderWaitCount);
        EncoderWaitState=0;
    }
}

void Action_StartIn(float Speed){
	
    Angle_Freshed=0;
    while((Angle_Freshed==0))HC05_Printf("Wait!\n");
		FMQ_Beep(3);
    //初始数据获取
    float AngNow=MPU6050_AngleData.Yaw;                     //当前角速度
    float AngTarget=AngNow;                                        //目标角速度

    //PID用参数
    float Error_Ang=0;                                      //角速度误差
    float Error_Ang_Last=0;                                 //上一次的角速度误差
    float Error_Ang_Last2=0;                                //上上次的角速度误差
    float Output=0;                                         //PID输出
    float Kp=2.0f;                                        //比例系数
    float Ki=1.2f;                                        //积分系数
    float Kd=2.0f;                                          //微分系数
		float DeadZone=3;
		int count=20;
		while(count--){
			AngNow=MPU6050_AngleData.Yaw;
			AngTarget=(AngTarget+AngNow)/2;
		}
		delay_ms(100);
		FMQ_Beep(3);
		
	SpeedPID_SetTarget(Motor_L,Speed);
	SpeedPID_SetTarget(Motor_R,-Speed);
		count=2;
	while(1){
//		if(count>2)count=0;
		if(GD_Read_Ctype(MB)==GD_State_Detected){
			FMQ_Beep(3);
			break;
		}
		
		AngNow=MPU6050_AngleData.Yaw;
		Error_Ang=Compute_CircleDiff(AngTarget,AngNow);
		if(Error_Ang>90||Error_Ang<-90){break;}
		OLED_ShowFloatXY(0,0,AngNow,3,3);
		OLED_ShowFloatXY(0,1,AngTarget,3,3);
		Output+=Kp*(Error_Ang-Error_Ang_Last)
                    +Ki*Error_Ang
                    +Kd*(Error_Ang-2*Error_Ang_Last+Error_Ang_Last2);
		Error_Ang_Last2=Error_Ang_Last;
        Error_Ang_Last=Error_Ang;
		 if(Output>32)
            Output=32;
        if(Output<-32)
            Output=-32;
//		8
		if((Error_Ang<=DeadZone&&Error_Ang>0)||(Error_Ang>=-DeadZone&&Error_Ang<0))Output=0;
		if(Output>0){SpeedPID_SetTarget(Motor_L,Speed+0.5f*Output);SpeedPID_SetTarget(Motor_R,-Speed+2.0f*Output);}
		if(Output<0){SpeedPID_SetTarget(Motor_L,Speed+1.5f*Output);SpeedPID_SetTarget(Motor_R,-Speed+0.8f*Output);}
//		if(count==2){
//			SpeedPID_SetTarget(Motor_L,Speed);
//			SpeedPID_SetTarget(Motor_R,-Speed);
//		}
//		count++;
	}
	SpeedPID_SetTarget(Motor_L,0);
	SpeedPID_SetTarget(Motor_R,0);
}

void InRange(float *Tar, float Min, float Max)
{
  if(*Tar>Max)
    *Tar = *Tar - Max + Min;
  else if(*Tar<Min)
    *Tar = *Tar + Max - Min;
}

void Action_LongTrack(Dir_type Which_Dir){
//	if(Track_flag){
//		if(Which_Dir==Right){
//				
//		}
//	}

}

float Compute_CircleDiff(float a,float b)
{
    float diff = a-b;
    if(diff>180)//
    {
        diff-=360;
    }
    else if(diff<-180)
    {
        diff+=360;
    }
    return diff;
}




void Track_Set(float Speed,CSB_Type Which_Type,float Target,Dir_type Dir){
	Track_Speed=Speed;
	Track_Csb=Which_Type;
	Track_Tar=Target;
	Track_Dir=Dir;
	return;
}

float ABS(float a){
	if(a>0)return a;
	else return -a;
}

void Track(void){
	if(!Track_EnFlag)return;
	TrackState_Struct.Now_Distance=CSB_GetDistance(Track_Csb);
	TrackState_Struct.Err=TrackState_Struct.Now_Distance-Track_Tar;
	TrackState_Struct.Out=Track_kp*TrackState_Struct.Err+Track_kd*(TrackState_Struct.Err-2*TrackState_Struct.Err_l1+TrackState_Struct.Err_l2)+Track_ki*TrackState_Struct.Sum;
	TrackState_Struct.Err_l1=TrackState_Struct.Err;
	TrackState_Struct.Err_l2=TrackState_Struct.Err_l1;
	TrackState_Struct.Sum+=TrackState_Struct.Err;
//	OLED_ShowStringXY(0,3,"Back:");OLED_ShowFloatXY(6,3,TrackState_Struct.Now_Distance,2,3);
//	OLED_ShowStringXY(0,5,"Back:");OLED_ShowFloatXY(6,5,Motor_ReadOutput(Motor_R),2,3);
//	OLED_ShowStringXY(0,5,"Back:");OLED_ShowFloatXY(6,6,Motor_ReadOutput(Motor_L),2,3);
	if(TLYState_Struct.Sum>=5)TLYState_Struct.Sum=5;
	if(TLYState_Struct.Sum<=-5)TLYState_Struct.Sum=-5;
	if(ABS(TrackState_Struct.Out)<=Track_DeadZone){TrackState_Struct.Out=0;}
	
	if(TrackState_Struct.Out>=Track_lim){TrackState_Struct.Out=Track_lim;}
	if(TrackState_Struct.Out<=-Track_lim){TrackState_Struct.Out=-Track_lim;}
	
//	OLED_ShowFloatXY(6,4,TrackState_Struct.Out,2,3);
	switch(Track_Dir){
		case Left: SpeedPID_SetTarget(Motor_L,Track_Speed-TrackState_Struct.Out);SpeedPID_SetTarget(Motor_R,-Track_Speed-TrackState_Struct.Out); break;//左走
		case Right:SpeedPID_SetTarget(Motor_L,-Track_Speed-TrackState_Struct.Out);SpeedPID_SetTarget(Motor_R,Track_Speed-TrackState_Struct.Out);  break;//右走
		case Front:
			if(TrackState_Struct.Out>0){
				
				Motor_SetOuput(Motor_L,Track_Speed+TrackState_Struct.Out);
				Motor_SetOuput(Motor_R,Track_Speed-TrackState_Struct.Out);  
			}
			else{
				Motor_SetOuput(Motor_L,Track_Speed+TrackState_Struct.Out);
				Motor_SetOuput(Motor_R,Track_Speed-TrackState_Struct.Out);  
			}  
			break;//前走
		case Back:
			if(TrackState_Struct.Out>0){
				
				Motor_SetOuput(Motor_L,Track_Speed-TrackState_Struct.Out);
				Motor_SetOuput(Motor_R,Track_Speed+TrackState_Struct.Out);  
			}
			else{
				Motor_SetOuput(Motor_L,Track_Speed-TrackState_Struct.Out);
				Motor_SetOuput(Motor_R,Track_Speed+TrackState_Struct.Out);  
			}
			break;//后走
		default:break;
	
	}

}

void TLY_GetTar(void){
	while(Angle_Freshed==0);
	TLY_Tar=MPU6050_AngleData.Yaw;
	int count=50;
	while(count--){TLY_Tar=(TLY_Tar+MPU6050_AngleData.Yaw)/2;}
	return;
}

void TLY_Set(float Speed){
	TLY_Speed=Speed;
	Motor_SetOuput(Motor_L,TLY_Speed);
	Motor_SetOuput(Motor_R,TLY_Speed);
}


void TLY_Control(void){
	if(!TLY_EnFlag)return;
	Speed_L=TLY_Speed;
	Speed_R=TLY_Speed;
//	OLED_ShowFloatXY(0,0,TLY_Tar,3,2);
//	OLED_ShowFloatXY(0,1,TLYState_Struct.Now_ANG,3,2);
//	OLED_ShowFloatXY(0,2,TLYState_Struct.Out,3,2);
	TLYState_Struct.Now_ANG=MPU6050_AngleData.Yaw;
	TLYState_Struct.Err=TLYState_Struct.Now_ANG-TLY_Tar;
	TLYState_Struct.Out=TLY_kp*TLYState_Struct.Err+TLY_kd*(TLYState_Struct.Err-2*TLYState_Struct.Err_l1+TLYState_Struct.Err_l2)+TLY_ki*TLYState_Struct.Sum;
	
	TLYState_Struct.Sum+=TLYState_Struct.Err;
	TLYState_Struct.Err_l1=TLYState_Struct.Err;
	TLYState_Struct.Err_l2=TLYState_Struct.Err_l1;
	
	if(TLYState_Struct.Sum>=3)TLYState_Struct.Sum=3;
	if(TLYState_Struct.Sum<=-3)TLYState_Struct.Sum=-3;
//	HC05_Printf("%f,%f,%f,%f,%f\n",TLY_Tar,TLYState_Struct.Now_ANG,Speed_L,Speed_R,TLYState_Struct.Out);
	if(ABS(TLYState_Struct.Err)<=TLY_DeadZone)TLYState_Struct.Out=0;
	if(TLYState_Struct.Out>=15){TLYState_Struct.Out=15;}
	if(TLYState_Struct.Out<=-15){TLYState_Struct.Out=-15;}	
	
	if(TLYState_Struct.Out>0){
		Motor_SetOuput(Motor_L,Speed_L-TLYState_Struct.Out);
		Motor_SetOuput(Motor_R,Speed_R+TLYState_Struct.Out);
	}
	
	if(TLYState_Struct.Out<0){
		Motor_SetOuput(Motor_L,Speed_L-TLYState_Struct.Out);
		Motor_SetOuput(Motor_R,Speed_R+TLYState_Struct.Out);
	}
}

void Action_XZ(void){
	SpeedPID_SetTarget(Motor_L,30);
	SpeedPID_SetTarget(Motor_R,-35);	
	delay_ms(1000);
	delay_ms(1000);

	SpeedPID_SetTarget(Motor_L,0);
	SpeedPID_SetTarget(Motor_R,0);	
	delay_ms(1000);
}

float ComAbs(float a,float b){
	if(a-b>0)return(a-b);
	else return(b-a);
}



void Action_Turn(float AngleToChange,Dir_type Which_Dir,Dir_type TrackDir){
		Action_Track_Disable();
		MPU6050_SetState(ENABLE);
		SpeedPID_EndStop();
    //获取一次最新数据
    Angle_Freshed=0;
    while(Angle_Freshed==0);

    //初始数据获取
    float AngleNow=MPU6050_AngleData.Yaw;               //初始角度
    float AngleLast=AngleNow;                               //上一次的角度  用于计算已经转动的角度
    float AngleTarget;                                      //目标角度
                                        
    //退出用参数
    float Turned_Angle=0;                                   //已经转动的角度

    if(Which_Dir==Left)
    {
        AngleTarget=AngleNow-AngleToChange;
        InRange(&AngleTarget,0,360);
				
        //进入转动
        while(1)
        {		
						AngleNow=MPU6050_AngleData.Yaw;
            //更新已转动角度
            Turned_Angle-=Compute_CircleDiff(AngleNow,AngleLast);
            AngleLast=AngleNow;
						if(Turned_Angle>=AngleToChange)break; 
                 
						if(TrackDir==Back){
							if(ABS(AngleToChange-Turned_Angle)>90)
							{
								Motor_SetOuput(Motor_L,-40);
								Motor_SetOuput(Motor_R,60);
							}
							else if (ABS(AngleToChange-Turned_Angle)<=90&&ABS(AngleToChange-Turned_Angle)>45) 
							{
								Motor_SetOuput(Motor_L,-70);
								Motor_SetOuput(Motor_R,70);
							}
							else 
							{
								Motor_SetOuput(Motor_L,-50);
								Motor_SetOuput(Motor_R,50);
							}
						}
							if(TrackDir==Front){
								if(ABS(AngleToChange-Turned_Angle)>90)
								{
									Motor_SetOuput(Motor_L,40);
									Motor_SetOuput(Motor_R,-60);
								}
								else if (ABS(AngleToChange-Turned_Angle)<=90&&ABS(AngleToChange-Turned_Angle)>45) 
								{
									Motor_SetOuput(Motor_L,-70);
									Motor_SetOuput(Motor_R,70);
								}
								else 
								{
									Motor_SetOuput(Motor_L,-50);
									Motor_SetOuput(Motor_R,50);
							}
						}
 
					} 
    }
    else if(Which_Dir==Right)
    {
				AngleTarget=AngleNow-AngleToChange;
        InRange(&AngleTarget,0,360);
				
        //进入转动
        while(1)
        {		    
						HC05_Printf("%f,%f,%f,%f \n",SpeedPID_ReturnSpeed(Motor_L),SpeedPID_ReturnSpeed(Motor_R),AngleNow,Turned_Angle);
						OLED_ShowStringXY(0,1,"Pitch:");OLED_ShowFloatXY(6,1,Turned_Angle,3,2);
						OLED_ShowStringXY(0,2,"Roll:");OLED_ShowFloatXY(6,2,AngleNow,3,2);
						AngleNow=MPU6050_AngleData.Yaw;
            //更新已转动角度
            Turned_Angle+=Compute_CircleDiff(AngleNow,AngleLast);
            AngleLast=AngleNow;
						if(Turned_Angle>=AngleToChange)break; 
                 
					
            if(TrackDir==Back){
							if(ABS(AngleToChange-Turned_Angle)>90)
							{
								Motor_SetOuput(Motor_L,-40);
								Motor_SetOuput(Motor_R,60);
							}
							else if (ABS(AngleToChange-Turned_Angle)<=90&&ABS(AngleToChange-Turned_Angle)>45) 
							{
								Motor_SetOuput(Motor_L,70);
								Motor_SetOuput(Motor_R,-70);
							}
							else 
							{
								Motor_SetOuput(Motor_L,50);
								Motor_SetOuput(Motor_R,-50);
							}
						}
							if(TrackDir==Front){
								if(ABS(AngleToChange-Turned_Angle)>90)
								{
									Motor_SetOuput(Motor_L,40);
									Motor_SetOuput(Motor_R,-60);
								}
								else if (ABS(AngleToChange-Turned_Angle)<=90&&ABS(AngleToChange-Turned_Angle)>45) 
								{
									Motor_SetOuput(Motor_L,70);
									Motor_SetOuput(Motor_R,-70);
								}
								else 
								{
									Motor_SetOuput(Motor_L,50);
									Motor_SetOuput(Motor_R,-50);
							}
						}
 
        }         
     }
		FMQ_Beep(3);
		Motor_SetOuput(Motor_L,0);
		Motor_SetOuput(Motor_R,0);
}
    

void Track_ChangePD(float p_TC,float d_TC,float lim_TC){
	Track_kp=p_TC;
	Track_kd=d_TC;
	Track_lim=lim_TC;
}
float Com_ABS(float a,float b){
	if((a-b)>=0)return a-b;
	else return b-a;
}
	
float Circle_ABS(float a,float b,Dir_type Which_Dir){
	float res1_L=0;
	float res2_L=0;
	float res1_R=0;
	float res2_R=0;
	float res_L=0;
	float res_R=0;
	float res=0;
	
	res1_L=a-b;
	res2_L=a+360-b;
	if(res1_L>0)res_L=res1_L;
	else res_L=res2_L;

	
	res1_R=a-b;
	res2_R=a-360-b;
	if(res1_R>res2_R&&res1_R<0)res_R=res1_R;
	else res_R=res2_R;
	
	
	
	if(ABS(res_R)>ABS(res_L))return res_L;
	else return res_R;
}
void Action_Turn2_0(float AngleToChange,Dir_type Which_Dir){
		int Finish_Flag=0;
		int Stop_Flag=0;
		int Finish_Count=0;
		Action_Track_Disable();
		MPU6050_SetState(ENABLE);
    //获取一次最新数据
    Angle_Freshed=0;
    while(Angle_Freshed==0);

    //初始数据获取
    float AngleNow=MPU6050_AngleData.Yaw;               //初始角度
    float AngleLast=AngleNow;                               //上一次的角度  用于计算已经转动的角度
    float AngleTarget;                                      //目标角度
                 
		float output=0;
		float kp_turn=0.75;
    //退出用参数
    float Turned_Angle=0;                                   //已经转动的角度
	
	
		if(Which_Dir==Left){
						AngleTarget=AngleNow-AngleToChange;
						InRange(&AngleTarget,0,360);
				}
		else{
						AngleTarget=AngleNow+AngleToChange;
						InRange(&AngleTarget,0,360);
				}
		while(1){
			OLED_ShowFloatXY(0,0,AngleNow,3,2);
			OLED_ShowFloatXY(0,1,AngleTarget,3,2);
			OLED_ShowFloatXY(0,2,output,3,2);
			OLED_ShowFloatXY(0,3,Circle_ABS(AngleNow,AngleTarget,Which_Dir),3,2);
			AngleNow=MPU6050_AngleData.Yaw; 
			
				if(Finish_Flag)break;
			
				if(Com_ABS(AngleNow,AngleTarget)<=3.0f){
					Motor_SetOuput(Motor_L,0);
					Motor_SetOuput(Motor_R,0);
					Stop_Flag=1;
				}
				else{Stop_Flag=0;}
				if(Com_ABS(AngleNow,AngleTarget)<=3.0f){Finish_Count++;delay_ms(200);if(Finish_Count>=5)Finish_Flag=1;}
				if(!Stop_Flag){
				output=Circle_ABS(AngleNow,AngleTarget,Which_Dir)*kp_turn;//小于零右转，大于零左转
				if(output>15)output=15;
				if(output<-15)output=-15;
				if(output<0){
					Motor_SetOuput(Motor_L,35+ABS(output));
					Motor_SetOuput(Motor_R,-(35+ABS(output)));
				}
				else{
					Motor_SetOuput(Motor_R,35+ABS(output));
					Motor_SetOuput(Motor_L,-(35+ABS(output)));
				
				}
			}
		}
		Motor_SetOuput(Motor_L,0);
		Motor_SetOuput(Motor_R,0);
				
}
	





