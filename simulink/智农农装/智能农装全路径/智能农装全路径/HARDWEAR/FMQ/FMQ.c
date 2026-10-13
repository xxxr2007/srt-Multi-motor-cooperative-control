#include "stm32f4xx.h"                  // Device header
int FMQ_BEEPCT=0;
void FMQ_Init(void)
{
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//??

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    FMQ_BEEPCT=0;
}

void FMQ_On(void){
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,1);
	return ;
}

void FMQ_Off(void){
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,0);
	return ;
}
void FMQ_Beep(int What_Times){
    FMQ_BEEPCT=What_Times;
    return;
}
void FMQ_Serve(void){
		if(FMQ_BEEPCT<0)FMQ_BEEPCT=0;
	
    if(FMQ_BEEPCT){FMQ_On();}
    else{FMQ_Off();}
		FMQ_BEEPCT--;
    return;
}