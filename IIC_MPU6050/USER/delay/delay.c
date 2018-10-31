#include "delay.h"

static u8  fac_us=0;//us��ʱ������
static u16 fac_ms=0;//ms��ʱ������

void DelayInit(u8 SYSCLK)
{
		SysTick->CTRL &= 0xfffffffb;		//���λ2 CLKSOURCE ��ѡ���ⲿʱ��Դ��STCLK����HCLK/8
		fac_us = SYSCLK/8;
		fac_ms = (u16)fac_us*1000;
}

//��72M������,nus<=1864135
void Delay_us(u32 nus)
{
	SysTick->LOAD = (u32)nus*fac_us;		//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL  = 0x00;           		//��ռ�����
	SysTick->CTRL = 0x01;          	  //ʹ�ܵ���������SysTick�ж�

	while( !(SysTick->CTRL & (1<<16)) );//�ȴ�ʱ�䵽�� 
	SysTick->CTRL=0x00;       					//�رռ�����
	SysTick->VAL |=0X00;       					//��ռ�����	
}

void Delay_ms(u16 nms)
{
	SysTick->LOAD = (u32)nms*fac_ms;		//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL  = 0x00;           		//��ռ�����
	SysTick->CTRL = 0x01;          	   //ʹ�ܵ���������SysTick�ж�

	while( !(SysTick->CTRL & (1<<16)) );//�ȴ�ʱ�䵽�� 
	SysTick->CTRL=0x00;       					//�رռ�����
	SysTick->VAL |=0X00;       					//��ռ�����	
}






