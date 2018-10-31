#include "delay.h"

static u8  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数

void DelayInit(u8 SYSCLK)
{
		SysTick->CTRL &= 0xfffffffb;		//清空位2 CLKSOURCE ，选择外部时钟源（STCLK）即HCLK/8
		fac_us = SYSCLK/8;
		fac_ms = (u16)fac_us*1000;
}

//对72M条件下,nus<=1864135
void Delay_us(u32 nus)
{
	SysTick->LOAD = (u32)nus*fac_us;		//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL  = 0x00;           		//清空计数器
	SysTick->CTRL = 0x01;          	  //使能倒数，屏蔽SysTick中断

	while( !(SysTick->CTRL & (1<<16)) );//等待时间到达 
	SysTick->CTRL=0x00;       					//关闭计数器
	SysTick->VAL |=0X00;       					//清空计数器	
}

void Delay_ms(u16 nms)
{
	SysTick->LOAD = (u32)nms*fac_ms;		//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL  = 0x00;           		//清空计数器
	SysTick->CTRL = 0x01;          	   //使能倒数，屏蔽SysTick中断

	while( !(SysTick->CTRL & (1<<16)) );//等待时间到达 
	SysTick->CTRL=0x00;       					//关闭计数器
	SysTick->VAL |=0X00;       					//清空计数器	
}






