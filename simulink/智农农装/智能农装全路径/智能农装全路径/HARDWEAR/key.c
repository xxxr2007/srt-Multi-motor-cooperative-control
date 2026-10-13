#include "key.h"
#include "FMQ.h"
#include "oled.h"
#include "delay.h"
#include "GD.h"
#include "Encoder.h"
#include "MPU6050.h"
#include "csb.h"
#include "Motor.h"

int Let_scheck_flag;
static Key_type Key_Vol ;
static int count=1;
void Key_Init(){
     {
        GPIO_InitTypeDef GPIO_InitStructure;
      
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

        GPIO_InitStructure.GPIO_Pin  =UP_Key|DOWN_Key|OK_Key|EXIT_Key;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_Init(Key_Port, &GPIO_InitStructure);
    }
    Let_scheck_flag=0;
}

Key_type Key_GetVol(void){
	Key_type Key_Vol;
   if(GPIO_ReadInputDataBit(Key_Port,UP_Key)==0){
        delay_ms(10);
        while (GPIO_ReadInputDataBit(Key_Port,UP_Key)==0);
        delay_ms(10);
        return UP;
   }
   if(GPIO_ReadInputDataBit(Key_Port,DOWN_Key)==0){
        delay_ms(10);
        while (GPIO_ReadInputDataBit(Key_Port,DOWN_Key)==0);
        delay_ms(10);
        return DOWN;
   }
   if(GPIO_ReadInputDataBit(Key_Port,OK_Key)==0){
        delay_ms(10);
        while (GPIO_ReadInputDataBit(Key_Port,OK_Key)==0);
        delay_ms(10);
        return OK;
   }
   if(GPIO_ReadInputDataBit(Key_Port,EXIT_Key)==0){
        delay_ms(10);
        while (GPIO_ReadInputDataBit(Key_Port,EXIT_Key)==0);
        delay_ms(10);
        return EXIT;
   }

   return NONE;
}
void TEMP(void){
		OLED_Clear();
    while(1){
        Key_Vol=Key_GetVol();
        if(Key_Vol==EXIT){OLED_Clear();break;}//判断是否退出
        OLED_ShowStringXY(0,0,"TO BE CONTINUED!!!"); 
    }
}
void GD_Check(void){
    GD_Init();
		OLED_Clear();
    while(1){
        Key_Vol=Key_GetVol();
        if(Key_Vol==EXIT){OLED_Clear();break;}//判断是否退出
        OLED_ShowStringXY(0,0,"GD Check!!!"); 
        OLED_ShowStringXY(0,1,"LF:");OLED_ShowUnsignedNumXY(4,1,GD_Read_Ctype(LF),1);
        OLED_ShowStringXY(6,1,"LB:");OLED_ShowUnsignedNumXY(10,1,GD_Read_Ctype(LB),1);
        OLED_ShowStringXY(0,2,"RF:");OLED_ShowUnsignedNumXY(4,2,GD_Read_Ctype(RF),1);
        OLED_ShowStringXY(6,2,"RB:");OLED_ShowUnsignedNumXY(10,2,GD_Read_Ctype(RB),1);
        OLED_ShowStringXY(0,3,"MF:");OLED_ShowUnsignedNumXY(4,3,GD_Read_Ctype(MF),1);
        OLED_ShowStringXY(6,3,"MB:");OLED_ShowUnsignedNumXY(10,3,GD_Read_Ctype(MB),1);
        OLED_ShowStringXY(0,4,"Color:");
        if(GD_Read_Ctype(CL)==GD_State_Detected)OLED_ShowStringXY(6,4,"Red");
        else {OLED_ShowStringXY(6,4,"Green");}
    }
	return;	
}

void CSB_Check(void){
    CSB_Init();
    CSB_Allow_Which(0x0F);
    CSB_Enable();
		OLED_Clear();
    while(1){
        Key_Vol=Key_GetVol();
        if(Key_Vol==EXIT){OLED_Clear();break;}//判断是否退出
        OLED_ShowStringXY(0,0,"CSB Check!!!"); 
        OLED_ShowStringXY(0,1,"Left:");OLED_ShowFloatXY(6,1,CSB_GetDistance(CSB_Left),2,3);
        OLED_ShowStringXY(0,2,"Rifht:");OLED_ShowFloatXY(6,2,CSB_GetDistance(CSB_Right),2,3);
        OLED_ShowStringXY(0,3,"Front:");OLED_ShowFloatXY(6,3,CSB_GetDistance(CSB_Front),2,3);
        OLED_ShowStringXY(0,4,"Back:");OLED_ShowFloatXY(6,4,CSB_GetDistance(CSB_Back),2,3);
    }
    CSB_Allow_Which(0x00);
    CSB_Disable();
	return;	
}
void Encoder_Check(void){
		Motor_Init();
		Encoder_Init();
		OLED_Clear();
    while(1){
        Key_Vol=Key_GetVol();
				static int Speed=60;
        if(Key_Vol==EXIT){OLED_Clear();break;}//判断是否退出
				{
        OLED_ShowStringXY(0,0,"Encoder Check!!!"); 
				OLED_ShowStringXY(0,1,"SpeedL:");OLED_ShowSignedNumXY(6,1,Motor_ReadOutput(Motor_L),3);
				OLED_ShowStringXY(0,2,"SpeedR:");OLED_ShowSignedNumXY(6,2,Motor_ReadOutput(Motor_R),3);
        OLED_ShowStringXY(0,3,"Left:");OLED_ShowSignedNumXY(6,3,TIM_GetCounter(TIM2),4);
        OLED_ShowStringXY(0,4,"Right:");OLED_ShowSignedNumXY(6,4,TIM_GetCounter(TIM3),4);
				}
				
				{
				if(Key_Vol==UP){Speed+=5;}
				if(Key_Vol==DOWN){Speed-=5;}
				if(Speed>90){Speed=90;}
				if(Speed<-90){Speed=-90;}
			  }
				
				Motor_SetOuput(Motor_L,Speed);
				Motor_SetOuput(Motor_R,Speed);
        
    }
		Motor_SetOuput(Motor_L,0);
		Motor_SetOuput(Motor_R,0);
		return;
}

void MCUSCheck(void){
	MPU6050_Init();
	MPU6050_SetState(ENABLE);
	OLED_Clear();
	while(1){
        Key_Vol=Key_GetVol();
        if(Key_Vol==EXIT){OLED_Clear();break;}//判断是否退出
				{
        OLED_ShowStringXY(0,0,"MPU6050 Check!!!"); 
				OLED_ShowStringXY(0,1,"Pitch:");OLED_ShowFloatXY(6,1,MPU6050_AngleData.Pitch,3,2);
        OLED_ShowStringXY(0,2,"Roll:");OLED_ShowFloatXY(6,2,MPU6050_AngleData.Roll,3,2);
        OLED_ShowStringXY(0,3,"Yaw:");OLED_ShowFloatXY(6,3,MPU6050_AngleData.Yaw,3,2);
        OLED_ShowStringXY(0,4,"PAL_X:");OLED_ShowFloatXY(6,4,MPU6050_PalData.PAL_X,3,2);
        OLED_ShowStringXY(0,5,"PAL_Y:");OLED_ShowFloatXY(6,5,MPU6050_PalData.PAL_Y,3,2);
        OLED_ShowStringXY(0,6,"PAL_Z:");OLED_ShowFloatXY(6,6,MPU6050_PalData.PAL_Z,3,2);
				}
				
		
        
    }
		return;
}
void Key_Let_sCheck(void){
    if (Key_GetVol()==OK)
    {
        FMQ_On();delay_ms(200);FMQ_Off();//Let us check it!!!

        while(1){
			Key_Vol=Key_GetVol();
            if(Key_Vol==EXIT){OLED_Clear();break;}//判断是否退出
             
            {		
				if (Key_Vol==UP)
                {
                    count-=1;
                }
                if (Key_Vol==DOWN)
                {
                    count+=1;
                }
                
                if (count>Check_ItemMax)
                {
                    count=1;
                }
                else if (count<1)
                {
                    count=Check_ItemMax;
                }
            }//计算位置

            for (int i = 0; i <=Check_ItemMax; i++)
            {
                if (i==count)
                {
                    OLED_ShowCharXY(0,i,'>');
                }
                else
								{
                    OLED_ShowCharXY(0,i,' ');
                }
                
            }
            //显示要检测的模块
			OLED_ShowStringXY(0,0,"Check System!!!"); 
			OLED_ShowStringXY(2,1,"GD");
			OLED_ShowStringXY(2,2,"CSB");
			OLED_ShowStringXY(2,3,"Encoder");
			OLED_ShowStringXY(2,4,"PID");
			OLED_ShowStringXY(2,5,"MCUS");

       if(Key_Vol==OK)//按下确认键
			{
				switch (count)
				{
				case 0:{
					break;
				}
				case 1:
					GD_Check();
					break;
				case 2:
					CSB_Check();
					break;
				case 3:
					Encoder_Check();
					break;
				case 4:
                    TEMP();
				    //PIDCheck();
					break;
				case 5:
//                    TEMP();
				    MCUSCheck();
					break;
				default:
					break;
			 }
			}
     }    
	 }   
}
