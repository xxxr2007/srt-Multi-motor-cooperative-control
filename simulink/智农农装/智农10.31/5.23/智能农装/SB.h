#ifndef SB_H
#define SB_H

#include "stm32f4xx.h"

typedef enum{SB_UP,SB_DOWN}SB_type;

void SB_Init(void);
void SB_OpenWhich(SB_type WhichSB);
void SB_CloseWhich(SB_type WhichSB);

#endif
