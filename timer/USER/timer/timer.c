#include "timer.h"
#include "usart.h"

/****************************
*  PSC：分频系数，0-65535
*	 ARR：预装载值，从0记到此值则产生溢出
*	 系统频率：72MHz
*********************************/

void TIM2_Init(u16 ARR,u16 PSC)	
{
		RCC->APB1ENR |= 0x01;
		TIM2->ARR = ARR;
		TIM2->PSC =PSC;
		TIM2->DIER |= 0x01;
		TIM2->CR1 |=0X01;
		MY_NVIC_Init(2,3,TIM2_IRQn,2);//抢占 1，子优先级 3，组 2
}

void TIM2_IRQHandler()
{
		if(TIM2->SR&0x01)
		{
				printf("触发定时器2中断\n");
		}
		TIM2->SR &= ~0x01;
}


