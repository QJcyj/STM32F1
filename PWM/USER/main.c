#include "stm32f10x.h"                  // Device header
#include "led.h"
#include "key.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "pwm.h"


int main(void)
{
//		LEDInit();
//		keyInit();
    DelayInit(72);
    MY_NVIC_PriorityGroupConfig(2);  //�жϷ���Ϊ��2��2:2
    UsartInit(72,115200);
    TIM2PWMInit(19999,71);			//���ö�ʱ��ʱ��Ƶ��Ϊ1000KHz,����20000�Σ�PWMƵ��Ϊ50Hz
    while(1)
    {

            PWMOut(0,200,400,800);
            Delay_us(500);
    }
}


