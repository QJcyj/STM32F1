#include "stm32f10x.h"                  // Device header

void delayms(int z)
{
		int i,j;
		for(i=z;i>0;i--)
			for(j=1700;j>0;j--);
}
