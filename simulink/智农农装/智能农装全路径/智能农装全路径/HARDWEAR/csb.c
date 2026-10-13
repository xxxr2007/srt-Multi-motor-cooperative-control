#include "stm32f4xx.h"                  // Device header
#include "OLED.h"
#include "timers.h"

#include "csb.h"
#include "delay.h"
#include "OLED.h"
//Ĭ�ϲ�����
CSB_Count CSB_OriData;
CSB_Distance_Data CSB_Disance;
static CSB_Allow_type CSB_Allow;

static int CSB_Freshed_Flag;
static int CSB_Require_Flag;
static int CSB_GetData_Flag;

static int count=0;
	
void CSB_Init(){
  Timers_TIM4_Init();
	
  GPIO_InitTypeDef GPIO_InitStructure;
        
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);

  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//��ʼ����������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;//��ʼ��TRIG����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

	CSB_Freshed_Flag=1;//开启
}

void CSB_Allow_Which(uint8_t Allow_order){
    CSB_Allow.CSB_Allow_Front=Allow_order&0x01;
    CSB_Allow.CSB_Allow_Back=(Allow_order&0x02)>>1;
    CSB_Allow.CSB_Allow_Left=(Allow_order&0x04)>>2;
    CSB_Allow.CSB_Allow_Right=(Allow_order&0x08)>>3;
		return;
}

void CSB_Enable(){
		Timers_TIM4_Start();
		CSB_Freshed_Flag=1;
		CSB_Require_Flag=0;
		CSB_GetData_Flag=0;
}

void CSB_Disable(){
     Timers_TIM4_Stop();
		CSB_Freshed_Flag=1;
		CSB_Require_Flag=0;
		CSB_GetData_Flag=0;
}

void CSB_Fresh_Count(){

  if(CSB_Allow.CSB_Allow_Front&&GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_4)&&CSB_OriData.CSB_Count_Front<=32){CSB_OriData.CSB_Count_Front++;}
	if(CSB_Allow.CSB_Allow_Back&&GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5)&&CSB_OriData.CSB_Count_Back<=32){CSB_OriData.CSB_Count_Back++;}
	if(CSB_Allow.CSB_Allow_Left&&GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_6)&&CSB_OriData.CSB_Count_Left<=32){CSB_OriData.CSB_Count_Left++;}
	if(CSB_Allow.CSB_Allow_Right&&GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_7)&&CSB_OriData.CSB_Count_Right<=32){CSB_OriData.CSB_Count_Right++;}
	return;

}
void CSB_Clear_Count(){
    CSB_OriData.CSB_Count_Front=0;
	CSB_OriData.CSB_Count_Back=0;
	CSB_OriData.CSB_Count_Left=0;
	CSB_OriData.CSB_Count_Right=0;
	return;
}
void CSB_Fresh_Distance(){
    CSB_Disance.CSB_Distance_Front=(CSB_OriData.CSB_Count_Front)*0.85;
    CSB_Disance.CSB_Distance_Back=(CSB_OriData.CSB_Count_Back)*0.85;
    CSB_Disance.CSB_Distance_Left=(CSB_OriData.CSB_Count_Left)*0.85;
    CSB_Disance.CSB_Distance_Right=(CSB_OriData.CSB_Count_Right)*0.85;
	return;
}

int CSB_AllGot(){
    if((!GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_4))&&(!GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5))&&(!GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_6))&&(!GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_6)))
    return 1;
    else return 0;
}

float CSB_GetDistance(CSB_Type which_type){
    float result=0;
     switch (which_type)
    {
    case CSB_Front:
        result=CSB_Disance.CSB_Distance_Front;
        break;
    case CSB_Back:
        result=CSB_Disance.CSB_Distance_Back;
        break;
    case CSB_Left:
        result=CSB_Disance.CSB_Distance_Left;
        break;
    case CSB_Right:
        result=CSB_Disance.CSB_Distance_Right;
        break;
    default:
        break;
    }
    return result;
}
void CSB_Show(){
	OLED_ShowFloatXY(0,0,CSB_Disance.CSB_Distance_Front,2,2);
	OLED_ShowFloatXY(0,1,CSB_Disance.CSB_Distance_Back,2,2);
	OLED_ShowFloatXY(0,2,CSB_Disance.CSB_Distance_Left,2,2);
	OLED_ShowFloatXY(0,3,CSB_Disance.CSB_Distance_Right,2,2);
}

void CSB_Service(){
	if (CSB_Freshed_Flag)
		{
			delay_ms(110);
			GPIO_SetBits(GPIOG,GPIO_Pin_8);//发送一次测量请求
			delay_us(40);
			GPIO_ResetBits(GPIOG,GPIO_Pin_8);

			CSB_Freshed_Flag=0;
			CSB_Require_Flag=1;
			count=0;
			CSB_Clear_Count();
		}
	 
		if (CSB_Require_Flag)
		{
			CSB_Fresh_Count();
			count++;
			if(count>=130){
				CSB_Require_Flag=0;
				CSB_GetData_Flag=1;
			}
		}                     
		
			if(CSB_GetData_Flag){
			CSB_Fresh_Distance();
			CSB_Clear_Count();

			CSB_GetData_Flag=0;
			CSB_Freshed_Flag=1;
	}
}
