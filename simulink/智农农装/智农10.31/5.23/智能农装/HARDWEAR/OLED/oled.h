#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	 

//OLED模式设置
//4线串行模式
#define OLED_MODE 0
#define SIZE 12
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED端口定义----------------  					   
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_5)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOE,GPIO_Pin_5)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_4)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOE,GPIO_Pin_4)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_3)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOE,GPIO_Pin_3)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_2)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOE,GPIO_Pin_2)

#define OLED_CS_Clr()  GPIO_ResetBits(GPIOE,GPIO_Pin_1)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOE,GPIO_Pin_1)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void OLED_Clear(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);

//易用函数
void OLED_ShowCharXY(uint8_t x,uint8_t y,uint8_t chr);
void OLED_ShowStringXY(uint8_t x,uint8_t y, uint8_t *p);
void OLED_ShowUnsignedNumXY(uint8_t x,uint8_t y,uint32_t num,uint8_t len);
void OLED_ShowFloatXY(uint8_t x,uint8_t y,float num,uint8_t len_int,uint8_t len_float);
void OLED_ShowSignedNumXY(uint8_t x,uint8_t y,int32_t num,uint8_t len);

#endif  
	 



