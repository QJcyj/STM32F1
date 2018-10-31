#ifndef _NRF24L01_H_
#define _NRF24L01_H_

#include "stm32f10x.h"
#include "spi.h"

/**********引脚信号定义*******************/
#define NRF_CE_High()  	{GPIOB->ODR |= 0X01<<13;}			//使能信号置高
#define NRF_CE_Low()   	{GPIOB->ODR &= ~(0X01<<13);}	//使能信号置低
//#define NRF_CSN_High()  {GPIOA->ODR |= 0X01<<4;}			//禁止选择器件
//#define NRF_CSN_Low()   {GPIOA->ODR &= ~(0X01<<4);}		//片选信号选通


#define NRF_CSN_High()  {GPIOA->ODR |= 0X01<<8;}			//禁止选择器件
#define NRF_CSN_Low()   {GPIOA->ODR &= ~(0X01<<8);}		//片选信号选通


#define NRF_IRQRead		   GPIOB->IDR &= (0X01<<15)			//中断信号读取



/****************参数定义区**********************/

// NRF24L01寄存器指令
#define RD_RX_PLOAD   0x61  // 读取接收数据指令
#define WR_TX_PLOAD   0xA0  // 写待发数据指令
#define NRF_WRITE_REG 0X20	 //写寄存器指令
#define CleanTX_FIFO  0xE1	//清除发送FIFO命令
#define CleanRX_FIFO  0xE2	//清除接收FIFO命令

#define CONFIG				0x00	//配置寄存器地址
#define EN_AA					0x01	//自动应答设置寄存器地址
#define EN_RXADDR			0x02	//可用信道设置寄存器地址
#define SETUP_AW			0x03	//发射接收地址设置寄存器地址
#define SETUP_RETR		0x04	//自动重发设置寄存器地址
#define RF_CH					0x05	//工作频率设置寄存器地址
#define RF_SETUP      0x06  //发射速率、功耗功能设置
#define STATUS        0x07  //状态寄存器     
#define RX_ADDR_P0    0x0A  //频道0接收数据地址
														//0B-0F为通道1-5的寄存器
#define TX_ADDR       0x10  //发送地址寄存器
#define RX_PW_P0      0x11  //接收频道0接收数据长度
														//12-16为通道1-5的寄存器

#define TX_Mode   1
#define RX_Mode   2

#define TX_DS     5
#define RX_DR     6
#define MAX_RT		4
#define TXFULL    0

/*****************函数声明区************************/
u8 NRF_WriteReg(u8 RegAddr, u8 Value);
u8 NRF_ReadReg(u8 RegAddr);
u8 NRF_WriteFIFO(u8 RegAddr,u8 *pBuf,u8 Len);
u8 NRF_ReadFIFO(u8 RegAddr,u8 *pBuf,u8 Len);
extern void NRF_Init(u8 CHFrequency, u8 Mode);
u8 NRF_Chech(void);
void SetRX_Mode(void);
void SetTX_Mode(void);
u8 NRF_SendData(u8 *TxBuf,u8 Len);
u8 NRF_Rx_Dat(u8 *Rxbuf);

#endif


