#ifndef _pwm_h_
#define _pwm_h_

#include "stm32f10x.h"

void TIM2PWMInit(u32 arr,u32 psc);
void PWMOut(u16 ch1,u16 ch2,u16 ch3,u16 ch4);

#endif



