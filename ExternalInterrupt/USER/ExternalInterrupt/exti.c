#include "exti.h"
#include "sys.h"
#include "usart.h"

void EXTI4_Init(void)
{
		RCC->APB2ENR |= 1<<3;
		GPIOB->CRL &= ~(0x0f<<4*4); 
		GPIOB->CRL |= 8<<(4*4);     //配置为输入模式
		GPIOB->ODR |= 1<<4;					//内部上拉
		
		RCC->APB2ENR |= 1<<0;		
		AFIO->EXTICR[1] |= 0X01;		//PB4复用为外部中断触发引脚
		EXTI->IMR |= 1<<4;					//开放来自线4上的事件请求
		EXTI->FTSR |= 1<<4;					//允许输入线4上的下降沿触发事件
		
		MY_NVIC_Init(3,2,EXTI4_IRQn,2);
}

void EXTI4_IRQHandler(void)
{
		if((EXTI->PR>>4)&0x01)
		{
				printf("触发外部中断\n");
		}
		EXTI->PR = 1<<4 ;		//对该寄存的对应线写1可以清除该线的中断标志位
}


