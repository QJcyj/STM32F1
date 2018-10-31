#include "exti.h"
#include "sys.h"
#include "usart.h"

void EXTI4_Init(void)
{
		RCC->APB2ENR |= 1<<3;
		GPIOB->CRL &= ~(0x0f<<4*4); 
		GPIOB->CRL |= 8<<(4*4);     //����Ϊ����ģʽ
		GPIOB->ODR |= 1<<4;					//�ڲ�����
		
		RCC->APB2ENR |= 1<<0;		
		AFIO->EXTICR[1] |= 0X01;		//PB4����Ϊ�ⲿ�жϴ�������
		EXTI->IMR |= 1<<4;					//����������4�ϵ��¼�����
		EXTI->FTSR |= 1<<4;					//����������4�ϵ��½��ش����¼�
		
		MY_NVIC_Init(3,2,EXTI4_IRQn,2);
}

void EXTI4_IRQHandler(void)
{
		if((EXTI->PR>>4)&0x01)
		{
				printf("�����ⲿ�ж�\n");
		}
		EXTI->PR = 1<<4 ;		//�ԸüĴ�Ķ�Ӧ��д1����������ߵ��жϱ�־λ
}


