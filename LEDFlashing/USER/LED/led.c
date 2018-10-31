#include "stm32f10x.h"                  // Device header
#include "delay.h"

void LEDInit(void)
{
		//��Ϊ�õ�PB3����JTAG-DP��ͻ������ҪSWJ->CFG����Ϊ010ʹPB3����
		RCC->APB2ENR |= 1<<0; 				//ʹ�ܸ��ù��ܵ�������				
		AFIO->MAPR   &= ~(0x7<<24);		//��AFIO->MAPR ��24~26λ����
		AFIO->MAPR   |= (0x2<<24);		//����AFIO->MAPR ��24~26λΪ010   ʹPB3����Ϊ��ͨIO��ʹ��
		
		RCC->APB2ENR |=1<<3;   				//ʹ��B��ʱ��
		GPIOB->CRL &=~(0XF<<(3*4));		//��B3��4��λ 0 �������
		GPIOB->CRL |= (0X3<<(3*4));		//MODE��11 50M���  
		GPIOB->ODR |=1<<3;
}

void LEDFlashing(void)
{
		GPIOB->ODR ^= 1<<3;
		delayms(200);
}

void SystemInit(void)
{

}
