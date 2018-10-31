#include "stm32f10x.h"                  // Device header
#include "delay.h"

void LEDInit(void)
{
		//因为用到PB3，与JTAG-DP冲突，所以要SWJ->CFG设置为010使PB3可用
		RCC->APB2ENR |= 1<<0; 				//使能复用功能调制配置				
		AFIO->MAPR   &= ~(0x7<<24);		//将AFIO->MAPR 的24~26位清零
		AFIO->MAPR   |= (0x2<<24);		//设置AFIO->MAPR 的24~26位为010   使PB3可作为普通IO口使用
		
		RCC->APB2ENR |=1<<3;   				//使能B口时钟
		GPIOB->CRL &=~(0XF<<(3*4));		//清B3口4个位 0 推挽输出
		GPIOB->CRL |= (0X3<<(3*4));		//MODE置11 50M输出  
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
