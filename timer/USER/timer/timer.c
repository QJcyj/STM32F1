#include "timer.h"
#include "usart.h"

/****************************
*  PSC����Ƶϵ����0-65535
*	 ARR��Ԥװ��ֵ����0�ǵ���ֵ��������
*	 ϵͳƵ�ʣ�72MHz
*********************************/

void TIM2_Init(u16 ARR,u16 PSC)	
{
		RCC->APB1ENR |= 0x01;
		TIM2->ARR = ARR;
		TIM2->PSC =PSC;
		TIM2->DIER |= 0x01;
		TIM2->CR1 |=0X01;
		MY_NVIC_Init(2,3,TIM2_IRQn,2);//��ռ 1�������ȼ� 3���� 2
}

void TIM2_IRQHandler()
{
		if(TIM2->SR&0x01)
		{
				printf("������ʱ��2�ж�\n");
		}
		TIM2->SR &= ~0x01;
}


