#include "stm32f10x.h"                  // Device header
#include "delay.h"

void keyInit(void)
{
		RCC->APB2ENR |= 1<<3;						//ʹ��PBʱ��
		GPIOB->CRL &= ~(0x0f<<(4*5));		//���PB5����λ
		GPIOB->CRL |= 8<<(4*5);					
		GPIOB->ODR |= 1<<5;							//����PB5Ϊ��������
}


char keyScan(void)
{
		if(!((GPIOB->IDR>>5)&0x01))					//��ⰴ���Ƿ���
		{
				Delay_ms(10);										//����
				while(!((GPIOB->IDR>>5)&0x01));	//�ȴ������ɿ�
				return 1;
		}
		else
			return 0;
		
}
	



