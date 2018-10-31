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
		MY_NVIC_PriorityGroupConfig(2);  //�жϷ���Ϊ��2��2:2
		UsartInit(72,115200);
		TIM2_Init(9999,7199);		//7199+1��Ƶ����ʱ��ʱ��Ƶ��Ϊ10K����10000��Ϊ1S
		while(1)
		{
				LEDFlashing();
		}
}


