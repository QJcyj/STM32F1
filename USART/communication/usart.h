#ifndef _usart_h_
#define _usart_h_
#include "stm32f10x.h"                  // Device header
#include "sys.h"
#include "stdio.h"

#define USART_REC_LEN  		200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

void UsartInit(u32 PCLK2, u32 bps);

#endif

