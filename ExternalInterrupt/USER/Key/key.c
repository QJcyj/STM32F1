#include "stm32f10x.h"                  // Device header
#include "delay.h"

void keyInit(void)
{
		RCC->APB2ENR |= 1<<3;						//使能PB时钟
		GPIOB->CRL &= ~(0x0f<<(4*5));		//清除PB5设置位
		GPIOB->CRL |= 8<<(4*5);					
		GPIOB->ODR |= 1<<5;							//设置PB5为上拉输入
}


char keyScan(void)
{
		if(!((GPIOB->IDR>>5)&0x01))					//检测按键是否按下
		{
				Delay_ms(10);										//消抖
				while(!((GPIOB->IDR>>5)&0x01));	//等待按键松开
				return 1;
		}
		else
			return 0;
		
}
	



