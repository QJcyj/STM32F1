#include "stm32f10x.h"                  // Device header
#include "led.h"
#include "key.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "exti.h"

int main(void)
{
		LEDInit();
		keyInit();
		DelayInit(72);
		MY_NVIC_PriorityGroupConfig(2);  //中断分组为组2，2:2
		UsartInit(72,115200);
		EXTI4_Init();
		while(1)
		{
//				if(keyScan())		//有按键按下，触发LED灭200MS
//				{
//					GPIOB->ODR &= ~ (1<<3);
//					printf("按键触发成功，灯亮\n\r");
//					Delay_ms(100);
//				}
//				else
//				{
//					GPIOB->ODR |= 1<<3;
//					printf("按键未触发或触发失败，灯灭\n\r");
//				}
//				if(USART1->DR == 0x40)
//				{
						GPIOB->ODR ^= 1<<3;
//						printf("收到闪烁指令，LED闪烁\n\r");
//				}
		}
}


