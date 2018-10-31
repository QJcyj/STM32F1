#include "stm32f10x.h"                  // Device header
#include "led.h"
#include "key.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "adc.h"

u16 adcValue=0;
float al=0.0;
int main(void)
{
		LEDInit();
		keyInit();
		DelayInit(72);
		MY_NVIC_PriorityGroupConfig(2);  //中断分组为组2，2:2
		UsartInit(72,115200);
		ADCInit();
	
		while(1)
		{
				adcValue = GetADC(1);
				al = (float)(adcValue*3.3/4096);
				printf("adcValue=%f\n",al);
				Delay_ms(100);
		}
}


