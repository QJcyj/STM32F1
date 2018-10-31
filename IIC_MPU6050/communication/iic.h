#ifndef _iic_h_
#define _iic_h_

#include "stm32f10x.h"
#include "sys.h"

/*******ʱ�ӡ������߸ߵ͡�������**************/

//IO��������
#define SDA_IN()  {GPIOB->CRL&=0XFFFFFFF0;GPIOB->CRL|=8<<0;}
#define SDA_OUT() {GPIOB->CRL&=0XFFFFFFF0;GPIOB->CRL|=3<<0;}

#define SCL_Low() 			{GPIOA->ODR &= ~(1<<4);}		//ʱ��������͵�ƽ
#define SCL_High() 			{GPIOA->ODR |= 1<<4;}				//ʱ��������ߵ�ƽ
#define SDA_Low()  			{GPIOB->ODR &= ~(1<<0);}		//����������͵�ƽ
#define SDA_High()  	  {GPIOB->ODR |= 1<<0;}				//����������ߵ�ƽ
#define SDA_Read				 PBin(0)			  						//��ȡSDA�ĵ�ƽ
#define IIC_SDA					 PBout(0)


/*****************����������******************************/

void Analogi2cInit(void);
u8 Analogi2cStart(void);
void Analogi2cStop(void);
u8 Analogi2cWaitAck(void);
void Analogi2cAck(void);
void Analogi2cNAck(void);
void Analogi2cSendByte(u8 Tx);
u8 Analogi2cReadByte(u8 ACK);


#endif


