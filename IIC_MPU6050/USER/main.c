#include "stm32f10x.h"                  // Device header
#include "led.h"
#include "key.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h" 
#include "iic.h"

int main(void)
{

		DelayInit(72);
		Analogi2cInit();
		MY_NVIC_PriorityGroupConfig(2);  //中断分组为组2，2:2
		UsartInit(72,115200);
		MPU6050Init();
		Delay_ms(100);
		while(1)
		{
				GetAngle();
				printf("roll=%10f   pitch=%10f   yaw=%10f \n",roll,pitch,yaw);
//				printf("三角函数直接解算X=%8.4f  三角函数直接解算Y=%8.4f\n",Angle_Trig_x,Angle_Trig_y);
				printf("\n");
				Delay_ms(10);
		}
}


