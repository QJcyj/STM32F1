#include "stm32f10x.h"                  // Device header
#include "led.h"
#include "key.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"

int main(void)
{
		LEDInit();
		keyInit();
		DelayInit(72);
		MY_NVIC_PriorityGroupConfig(2);  //中断分组为组2，2:2
		UsartInit(72,115200);
		TIM2_Init(9999,7199);		//7199+1分频，定时器时钟频率为10K，记10000次为1S
		while(1)
		{
				LEDFlashing();
		}
}


