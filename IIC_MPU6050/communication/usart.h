#ifndef _usart_h_
#define _usart_h_
#include "stm32f10x.h"                  // Device header
#include "sys.h"
#include "stdio.h"

#define USART_REC_LEN  		200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收

void UsartInit(u32 PCLK2, u32 bps);

#endif

