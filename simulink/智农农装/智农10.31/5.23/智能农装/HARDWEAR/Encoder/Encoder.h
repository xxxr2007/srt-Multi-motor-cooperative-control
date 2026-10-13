#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "stm32f4xx.h"

typedef enum
{
    Encoder_L = 0,
    Encoder_R = 1
}Encoder_Which;

void Encoder_Init(void);
int16_t Encoder_GetVol(Encoder_Which Which);

#endif
