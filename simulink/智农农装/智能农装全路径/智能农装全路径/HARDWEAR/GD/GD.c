#include "GD.h"


//初始化光电传感器
void GD_Init(void)
{
    {//正常光电传感器
         GPIO_InitTypeDef GPIO_InitStructure;
        //D10~15
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);//使能

        GPIO_InitStructure.GPIO_Pin  = GD_Middle_B|GD_Middle_F|GD_Right_F|GD_Right_B|GD_Left_F|GD_Left_B|GD_Color;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置成上拉输入
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
    }

}

GD_State GD_Wait_Ctype(GD_type Which_type,int What_times){
    int target=What_times;
    int count=0;
    
    switch (Which_type)
    {
    case RF:
        while(1){
					if(count>=What_times)break;
					while(GPIO_ReadInputDataBit(GD_Prot,GD_Right_F)!=GD_State_Detected);
					count++;
					delay_ms(10);
				}
        return  GD_State_Detected;
        break;
    case RB:
       while(1){
					if(count>=What_times)break;
					while(GPIO_ReadInputDataBit(GD_Prot,GD_Right_B)!=GD_State_Detected);
					count++;
					delay_ms(10);
				}
        return  GD_State_Detected;
        break;
    case MF:
			  while(1){
					if(count>=What_times)break;
					while(GPIO_ReadInputDataBit(GD_Prot,GD_Middle_F)!=GD_State_Detected);
					count++;
					delay_ms(10);
				}
        return  GD_State_Detected;
        break;
    case MB:
        while(1){
					if(count>=What_times)break;
					while(GPIO_ReadInputDataBit(GD_Prot,GD_Middle_B)!=GD_State_Detected);
					count++;
					delay_ms(10);
				}
        return  GD_State_Detected;
        break;
    case LF:
        while(1){
					if(count>=What_times)break;
					while(GPIO_ReadInputDataBit(GD_Prot,GD_Left_F)!=GD_State_Detected);
					count++;
					delay_ms(10);
				}
        return  GD_State_Detected;
        break;
    case LB:
        while(1){
					if(count>=What_times)break;
					while(GPIO_ReadInputDataBit(GD_Prot,GD_Left_B)!=GD_State_Detected);
					count++;
					delay_ms(10);
				}
        return  GD_State_Detected;
        break;
    case CL:
        while(GPIO_ReadInputDataBit(GD_Prot,GD_Color)==GD_State_Detected){
            if(count++>=What_times)break;
            delay_ms(10);
        }
        return  GD_State_Detected;
        break;
    default:
        break;
    }
    return GD_State_NotDetected;
}

GD_State GD_Read_Ctype(GD_type Which_type){
    int target=Which_type;
    int count=0;
    
    switch (Which_type)
    {
    case RF:
        return GPIO_ReadInputDataBit(GD_Prot,GD_Right_F);
        break;
    case LF:
        return GPIO_ReadInputDataBit(GD_Prot,GD_Left_F);
        break;
    case MF:
        return GPIO_ReadInputDataBit(GD_Prot,GD_Middle_F);
        break;
    case RB:
        return GPIO_ReadInputDataBit(GD_Prot,GD_Right_B);
        break;
    case LB:
        return GPIO_ReadInputDataBit(GD_Prot,GD_Left_B);
        break;
    case MB:
        return GPIO_ReadInputDataBit(GD_Prot,GD_Middle_B);
        break;
    case CL:
        return GPIO_ReadInputDataBit(GD_Prot,GD_Color);
        break;
    default:
        break;
    }
    return GD_State_NotDetected;
}