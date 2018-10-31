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
    MY_NVIC_PriorityGroupConfig(2);  //中断分组为组2，2:2
    UsartInit(72,115200);
    TIM2PWMInit(19999,71);			//设置定时器时钟频率为1000KHz,计数20000次，PWM频率为50Hz
    while(1)
    {

            PWMOut(0,200,400,800);
            Delay_us(500);
    }
}


