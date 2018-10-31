#ifndef _NRF24L01_H_
#define _NRF24L01_H_

#include "stm32f10x.h"
#include "spi.h"

/**********�����źŶ���*******************/
#define NRF_CE_High()  	{GPIOB->ODR |= 0X01<<13;}			//ʹ���ź��ø�
#define NRF_CE_Low()   	{GPIOB->ODR &= ~(0X01<<13);}	//ʹ���ź��õ�
//#define NRF_CSN_High()  {GPIOA->ODR |= 0X01<<4;}			//��ֹѡ������
//#define NRF_CSN_Low()   {GPIOA->ODR &= ~(0X01<<4);}		//Ƭѡ�ź�ѡͨ


#define NRF_CSN_High()  {GPIOA->ODR |= 0X01<<8;}			//��ֹѡ������
#define NRF_CSN_Low()   {GPIOA->ODR &= ~(0X01<<8);}		//Ƭѡ�ź�ѡͨ


#define NRF_IRQRead		   GPIOB->IDR &= (0X01<<15)			//�ж��źŶ�ȡ



/****************����������**********************/

// NRF24L01�Ĵ���ָ��
#define RD_RX_PLOAD   0x61  // ��ȡ��������ָ��
#define WR_TX_PLOAD   0xA0  // д��������ָ��
#define NRF_WRITE_REG 0X20	 //д�Ĵ���ָ��
#define CleanTX_FIFO  0xE1	//�������FIFO����
#define CleanRX_FIFO  0xE2	//�������FIFO����

#define CONFIG				0x00	//���üĴ�����ַ
#define EN_AA					0x01	//�Զ�Ӧ�����üĴ�����ַ
#define EN_RXADDR			0x02	//�����ŵ����üĴ�����ַ
#define SETUP_AW			0x03	//������յ�ַ���üĴ�����ַ
#define SETUP_RETR		0x04	//�Զ��ط����üĴ�����ַ
#define RF_CH					0x05	//����Ƶ�����üĴ�����ַ
#define RF_SETUP      0x06  //�������ʡ����Ĺ�������
#define STATUS        0x07  //״̬�Ĵ���     
#define RX_ADDR_P0    0x0A  //Ƶ��0�������ݵ�ַ
														//0B-0FΪͨ��1-5�ļĴ���
#define TX_ADDR       0x10  //���͵�ַ�Ĵ���
#define RX_PW_P0      0x11  //����Ƶ��0�������ݳ���
														//12-16Ϊͨ��1-5�ļĴ���

#define TX_Mode   1
#define RX_Mode   2

#define TX_DS     5
#define RX_DR     6
#define MAX_RT		4
#define TXFULL    0

/*****************����������************************/
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


