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
u8 buff[32]={0,0,0,0,0};

int main(void)
{
	LEDInit();
	DelayInit(72);
	Delay_ms(100);
	Delay_ms(100);
	MY_NVIC_PriorityGroupConfig(2);  //�жϷ���Ϊ��2��2:2
	UsartInit(72,115200);
	ADCInit();
	SPI1_Init();
	NRF_Init(40, RX_Mode);
	Delay_ms(100);
	printf("NRF24L01��ʼ����ɣ����ڽ���ģʽ������\n");
	if(!NRF_Chech())
		printf("NRF24L01�쳣\n");
	else
		printf("NRF24L01��֤���󣬿�����ʹ��\n");
	Delay_ms(100);
	while(1)
	{
		if(!NRF_Rx_Dat(buff))
				printf("����δ����\n");
		else
				printf("buff[0]=%d, buff[1]=%d, buff[2]=%d  buff[3]=%d  buff[4]=%d\n",(int)buff[0],(int)buff[1],buff[2],buff[3],buff[4]);
		LEDFlashing();
		Delay_ms(50);
	}
}


