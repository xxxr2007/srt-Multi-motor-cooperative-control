#include "SB.h"



void SB_Init(void){
		 GPIO_InitTypeDef GPIO_InitStructure;
        //D10~15
     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//??

     GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
     GPIO_Init(GPIOD, &GPIO_InitStructure);

     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//??

     GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
     GPIO_Init(GPIOD, &GPIO_InitStructure);
	
			GPIO_WriteBit(GPIOA,GPIO_Pin_2,1);
			GPIO_WriteBit(GPIOB,GPIO_Pin_1,1);//Ä¬ÈÏ¶¼¹Ø±Õ
}

void SB_OpenWhich(SB_type WhichSB){
	switch(WhichSB){
		case SB_UP:GPIO_WriteBit(GPIOA,GPIO_Pin_2,0);break;
		case SB_DOWN:GPIO_WriteBit(GPIOB,GPIO_Pin_1,0);break;
		default :break;
	}
}

void SB_CloseWhich(SB_type WhichSB){
	switch(WhichSB){
		case SB_UP:GPIO_WriteBit(GPIOA,GPIO_Pin_2,1);break;
		case SB_DOWN:GPIO_WriteBit(GPIOB,GPIO_Pin_1,1);break;
		default :break;
	}
}
