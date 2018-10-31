#include "stm32f10x.h"                  // Device header
#include "led.h"
//#include "key.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "adc.h"
#include "NRF24L01.h"


u16 adcValue=0;
float al=0.0;
u8 buff[32]={0x05,0x01,0x06,0x06,0x06};

int main(void)
{
		LEDInit();
		DelayInit(72);
		Delay_ms(100);
		MY_NVIC_PriorityGroupConfig(2);  //�жϷ���Ϊ��2��2:2
		UsartInit(72,115200);
		ADCInit();
		SPI1_Init();
		NRF_Init(40, TX_Mode);
		printf("NRF24L01��ʼ����ɣ�����\n");
		if(!NRF_Chech())
				printf("NRF24L01�쳣\n");
		else
				printf("NRF24L01��֤���󣬿�����ʹ��\n");
		Delay_ms(100);
		while(1)
		{
				if(NRF_SendData(buff,32))
						printf("NRF24L01�������ݳɹ�\n");
				else
						printf("NRF24L01��������ʧ��\n");
			  LEDFlashing();
				Delay_ms(2);
		}
}


