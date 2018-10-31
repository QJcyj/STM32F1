#ifndef _adc_h_
#define _adc_h_

#include "stm32f10x.h"                  // Device header

void ADCInit(void);
u16 GetADC(u8 ch);

#endif

