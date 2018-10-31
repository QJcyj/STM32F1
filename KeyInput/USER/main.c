#include "stm32f10x.h"                  // Device header
#include "led.h"
#include "key.h"
#include "delay.h"


int main(void)
{
		LEDInit();
		keyInit();
		DelayInit(72);
		while(1)
		{
				if(keyScan())		//�а������£�����LED��200MS
				{
					GPIOB->ODR |= 1<<3;
					Delay_ms(100);
				}
				else
					LEDFlashing();
//					GPIOB->ODR &= ~(1<<3);
		}
}


