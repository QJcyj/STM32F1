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
		MY_NVIC_PriorityGroupConfig(2);  //�жϷ���Ϊ��2��2:2
		UsartInit(72,115200);
		EXTI4_Init();
		while(1)
		{
//				if(keyScan())		//�а������£�����LED��200MS
//				{
//					GPIOB->ODR &= ~ (1<<3);
//					printf("���������ɹ�������\n\r");
//					Delay_ms(100);
//				}
//				else
//				{
//					GPIOB->ODR |= 1<<3;
//					printf("����δ�����򴥷�ʧ�ܣ�����\n\r");
//				}
//				if(USART1->DR == 0x40)
//				{
						GPIOB->ODR ^= 1<<3;
//						printf("�յ���˸ָ�LED��˸\n\r");
//				}
		}
}


