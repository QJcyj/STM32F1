#include "stm32f10x.h"                  // Device header
#include "led.h"

int main(void)
{
		LEDInit();
		while(1)
		{
				LEDFlashing();
		}
}


