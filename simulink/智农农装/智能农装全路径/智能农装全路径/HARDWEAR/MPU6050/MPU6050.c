#include "MPU6050.h"
#include "usart.h"

#include "delay.h"

#include "HC05.h"

//MPU6050 连接至USART2 115200 8-N-1
extern uint32_t count_ACC;
extern uint32_t count_Palstance;
extern uint32_t count_Angle;

uint8_t Angle_Freshed=0;         //姿态数据更新标志
uint8_t ACC_Freshed=0;           //加速度数据更新标志
uint8_t Palstance_Freshed=0;     //角速度数据更新标志
uint8_t Angle_Aux_Freshed=0;     //辅助姿态数据更新标志

static uint8_t MPU6050_Data[14]; //MPU6050数据包

MPU6050_AngleType MPU6050_ParsedData;

MPU6050_AngleType MPU6050_AngleData;
MPU6050_AccType MPU6050_AccData;
MPU6050_PalType MPU6050_PalData;
//MPU6050数据包解析
void MPU6050_Parse(void)
{
    //解析数据 
    //先校验数据和
    uint8_t CheckSum=MPU6050_Data[0]+MPU6050_Data[1]+MPU6050_Data[2]+MPU6050_Data[3]
                    +MPU6050_Data[4]+MPU6050_Data[5]+MPU6050_Data[6]
                    +MPU6050_Data[7]+MPU6050_Data[8]+MPU6050_Data[9];

    static int16_t Data_16;
    if(CheckSum!=MPU6050_Data[10])
        goto err_checksum;
    else 
    {
        switch (MPU6050_Data[1])
        {
        case 0x51://加速度数据
            Data_16=MPU6050_Data[2]|(short)MPU6050_Data[3]<<8;
            MPU6050_AccData.ACC_X=(float)Data_16/32768*16;
            Data_16=MPU6050_Data[4]|(short)MPU6050_Data[5]<<8;
            MPU6050_AccData.ACC_Y=(float)Data_16/32768*16;
            Data_16=MPU6050_Data[6]|(short)MPU6050_Data[7]<<8;
            MPU6050_AccData.ACC_Z=(float)Data_16/32768*16;
            ACC_Freshed=1;
            goto Nor_Done;
        case 0x52://角速度数据
            Data_16=MPU6050_Data[2]|(short)MPU6050_Data[3]<<8;
            MPU6050_PalData.PAL_X=(float)Data_16/32768*2000;  
            Data_16=MPU6050_Data[4]|(short)MPU6050_Data[5]<<8;
            MPU6050_PalData.PAL_Y=(float)Data_16/32768*2000;
            Data_16=MPU6050_Data[6]|(short)MPU6050_Data[7]<<8;
            MPU6050_PalData.PAL_Z=-(float)Data_16/32768*2000;
            Palstance_Freshed=1;
            goto Nor_Done;
        case 0x53://角度数据 //由于安装问题 需要对Pitch和Roll进行调换
            Data_16=MPU6050_Data[2]|MPU6050_Data[3]<<8;
            MPU6050_AngleData.Pitch=(float)Data_16/32768*180;
            Data_16=MPU6050_Data[4]|MPU6050_Data[5]<<8;
            MPU6050_AngleData.Roll=(float)Data_16/32768*180;
            Data_16=MPU6050_Data[6]|MPU6050_Data[7]<<8;
            MPU6050_AngleData.Yaw=-(float)Data_16/32768*180+180;
            Angle_Freshed=1;  
            goto Nor_Done;
        default:
            break;
        }
    }
	if(MPU6050_Data[0]==0xAA&&MPU6050_Data[7]==0x55)
    {
     
        int16_t Data_16;
        Data_16=(float)(MPU6050_Data[1]<<8|MPU6050_Data[2]);
        MPU6050_AngleData.Yaw=-(float)Data_16/100.0f+180;
        Data_16=(float)(MPU6050_Data[3]<<8|MPU6050_Data[4]);
        MPU6050_AngleData.Roll=(float)Data_16/100.0f;
        Data_16=(float)(MPU6050_Data[5]<<8|MPU6050_Data[6]);
        MPU6050_AngleData.Pitch=-(float)Data_16/100.0f;

        Angle_Freshed=1;
    }
    err_checksum:
      
    Nor_Done:;
      
}



//数据包接收
void USART2_IRQHandler(void)
{
    static uint8_t Count = 0;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
				
        MPU6050_Data[Count] = USART_ReceiveData(USART2);
        // HC05_SendByte(MPU6050_Data[Count]);
        if((Count == 0) && (MPU6050_Data[0] != 0x55)) //数据包头检测 
        {
            Count = 0;
        }
        else //进入数据包接收
        {
            Count++;
            if(Count >= 11) //数据包接收完成
            {
                Count = 0;
                MPU6050_Parse();
            }
        }
        USART_ClearITPendingBit(USART2, USART_IT_RXNE); // 清除中断标志
    }
    
    
}

//MPU6050初始化 注意接收中断未开启
void MPU6050_Init(void)
{
    USART2_Init(9600);
}

//使能或失能MPU6050串口中断
void MPU6050_SetState(FunctionalState state)
{
    if(state==ENABLE)
    {
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    }
    else
    {
        USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
    }
}
