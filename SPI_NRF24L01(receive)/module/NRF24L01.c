#include "NRF24L01.h"
#include "delay.h"
#include "usart.h"

#define TxDataWidth    5
#define RxDataWidth    5
u8 TxAddr[TxDataWidth]= {0x20,0x43,0x10,0x10,0x21};
u8 RxAddr[RxDataWidth]= {0x20,0x43,0x10,0x10,0x21};

/**********************************************************
* ��������u8 NRF_WriteReg(u8 RegAddr, u8 Value)
* ���������RegAddr-SPI���豸�Ĵ�����ַ
*						Value-��SPI���豸��ַ����RegAddr�ļĴ���д�������
* ���������status-��ȡ�����ֽ�
* ���� ��2018-3-8
* ����������ģ��д�Ĵ�������
************************************************************/
u8 NRF_WriteReg(u8 RegAddr, u8 Value)
{
		u8 status;
		NRF_CSN_Low();												//ѡͨNRF����
		status = SPI1_ReadWriteByte(RegAddr);	//д��Ĵ�����ַ
		SPI1_ReadWriteByte(Value);						//�Ըո�ѡ�еļĴ�д������
		NRF_CSN_High();												//��ֹNRF����
		
		return status;
}

/**********************************************************
* ��������u8 NRF_ReadReg(u8 RegAddr)
* ���������Reg-SPI���豸�Ĵ�����ַ
* ���������RegValue-��ȡ�����ֽ�
* ���� ��2018-3-8
* ����������ģ����Ĵ������ݺ�������ѡ��ַ��
*				��д0�Ӹõ�ַ����ȡ����
************************************************************/
u8 NRF_ReadReg(u8 RegAddr)
{
		u8 RegValue;
		NRF_CSN_Low();										//ѡͨNRF����
		SPI1_ReadWriteByte(RegAddr);			//д��Ĵ�����ַ
		RegValue = SPI1_ReadWriteByte(0);	//д0����ȡ�üĴ����е�����
		NRF_CSN_High();										//��ֹNRF����
		
		return RegValue;
}

/**********************************************************
* ��������u8 NRF_WriteFIFO(u8 RegAddr,u8 *pBuf,u8 Len)
* ���������RegAddr-SPI���豸�Ĵ�����ַ
						*pBuf-д�뻺����������
						Len-д����ٸ��ֽ�
* ���������RegValue-��ȡ�����ֽ�
* ���� �� 2018-3-8
* ������  �Ի�����д������
************************************************************/
u8 NRF_WriteFIFO(u8 RegAddr,u8 *pBuf,u8 Len)
{
		u8 i,status;
		NRF_CSN_Low();												//ѡͨNRF����
		status = SPI1_ReadWriteByte(RegAddr);	//д��Ĵ�����ַ
		for(i=0;i<Len;i++)
				SPI1_ReadWriteByte(pBuf[i]);			//д������
		NRF_CSN_High();												//��ֹNRF����
	
		return status;
}

/**********************************************************
* ��������u8 NRF_ReadReg(u8 RegAddr)
* ���������RegAddr-SPI���豸�Ĵ�����ַ
						*pBuf-����ȡ�������ݴ�����
						Len-��ȡ���ٸ��ֽ�
* ���������RegValue-��ȡ�����ֽ�
* ���� �� 2018-3-8
* ������  �ӻ�������ȡ���ݴ���*pBuf
************************************************************/
u8 NRF_ReadFIFO(u8 RegAddr,u8 *pBuf,u8 Len)
{
		u8 i,status;
		NRF_CSN_Low();												//ѡͨNRF����
		status = SPI1_ReadWriteByte(RegAddr);	//д��Ĵ�����ַ
		for(i=0;i<Len;i++)
				pBuf[i]=SPI1_ReadWriteByte(0);		//��ȡ����
		NRF_CSN_High();												//��ֹNRF����
	
		return status;
}

/**********************************************************
* ��������void NRF_Init(u8 CHFrequency, u8 Mode)
* ���������CHFrequency-ͨ������Ƶ�� һ�����40
*						Mode-����ģʽ
* �����������
* ���� �� 2018-3-8
* ������  ��NRF24L01���г�ʼ��
************************************************************/
void NRF_Init(u8 CHFrequency, u8 Mode)
{
		NRF_CE_Low();																			//ʹ��NRF�������������ģʽ
		NRF_WriteReg(NRF_WRITE_REG+STATUS,0x70);
		NRF_WriteReg(CleanTX_FIFO, 0xff);									//��շ��ͻ�����
		NRF_WriteReg(CleanRX_FIFO, 0xff);									//��ս��ջ�����
		NRF_WriteFIFO(NRF_WRITE_REG+TX_ADDR,TxAddr,5);		//д�뷢�͵�ַ
		NRF_WriteFIFO(NRF_WRITE_REG+RX_ADDR_P0,RxAddr,5);	//д����յ�ַ
		NRF_WriteReg(NRF_WRITE_REG+EN_AA,0x01);						//ʹ��ͨ��0���Զ�Ӧ��
		NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x01);				//ʹ��ͨ��0�Ľ��յ�ַ
		NRF_WriteReg(NRF_WRITE_REG+SETUP_RETR,0x1A);			//�����Զ��ط����ʱ��Ϊ500us������Զ��ط�10��
		NRF_WriteReg(NRF_WRITE_REG+RF_CH,CHFrequency);    //����RFͨ���Ĺ���Ƶ�ʣ�һ��д��40
		NRF_WriteReg(NRF_WRITE_REG+RX_PW_P0,32);					//����ͨ��0����Ч���ݿ��
		NRF_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0f);				//���÷��������0dB���棬2Mbps,���������濪��
		
		if(Mode==TX_Mode)
				SetTX_Mode();			//����Ϊ����ģʽ
		else if(Mode==RX_Mode)	
				SetRX_Mode();			//����Ϊ����ģʽ
		
		NRF_CE_High();																		//ʹ��
}

/**********************************************************
* ��������void SetTX_Mode(void) 
* �����������
* �����������
* ���� �� 2018-3-8
* ������  ��NRF24L01����Ϊ����ģʽ
************************************************************/
void SetTX_Mode(void) 
{
		NRF_CE_Low();																			//ʹ��NRF�������������ģʽ
		NRF_WriteReg(NRF_WRITE_REG+CONFIG,0x0E);			    // ����Ϊ����ģʽ
		NRF_CE_High();																		//ʹ�ܷ���
		Delay_us(150);
}


/**********************************************************
* ��������void SetRX_Mode(void)
* �����������
* �����������
* ���� �� 2018-3-8
* ������  ��NRF24L01����Ϊ����ģʽ
************************************************************/
void SetRX_Mode(void) 
{
		NRF_CE_Low();																			//ʹ��NRF�������������ģʽ
		NRF_WriteReg(NRF_WRITE_REG+CONFIG,0x0F);			    // IRQ�շ�����ж���Ӧ��16λCRC	��������
		NRF_CE_High();																		
		Delay_us(150);
}

/**********************************************************
* ��������u8 NRF_Chech(void)
* �����������
* ���������1-NRF24L01����  0-NRF24L01�쳣
* ���� �� 2018-3-8
* ������  ��NRF24L01�����쳣���
************************************************************/
u8 NRF_Chech(void)
{
		u8 buf[5];
		u8 i;
		NRF_WriteFIFO(NRF_WRITE_REG+TX_ADDR,TxAddr,5);		//д�뷢�͵�ַ
		NRF_ReadFIFO(TX_ADDR,buf,5);											//д�뷢�͵�ַ
		for(i=0;i<5;i++)
				if(buf[i]!=TxAddr[i])													//���ݲ�׼���˳�����0��NRF24L01�쳣
						return 0;
				
		return 1;			//����1��NRF24L01���쳣��������ʹ��
}

/**********************************************************
* ��������u8 NRF_SendData(u8 *TxBuf,u8 Len)
* ���������*TxBuf-���͵�����   
*						len-���ݳ��ȣ��ֽڣ�
* ���������1-NRF24L01��������   0-NRF24L01����ʧ��
* ���� �� 2018-3-8
* ������  NRF24L01��������
************************************************************/
u8 NRF_SendData(u8 *TxBuf,u8 Len)
{
		u8 conut=0,status;
		SetTX_Mode();				//����ģʽ
		NRF_CE_Low();																	//�������ģʽ
		NRF_WriteFIFO(WR_TX_PLOAD,TxBuf,Len);						//д������
		NRF_CE_High();																//ʹ�ܷ���
		while(NRF_IRQRead)
		{
				conut++;
				if(conut==200)
						return 0;
		}
		status = NRF_ReadReg(NRF_WRITE_REG+STATUS);	
		NRF_WriteReg(NRF_WRITE_REG+CONFIG,status);			//���TX_DS��MAX_RT�жϱ�־
		if(status&(1<<MAX_RT))				//�ﵽ�����ط�
		{
				if(status&(1<<TXFULL))		//����������
				{
						NRF_WriteReg(CONFIG,0xff);			//��ջ�����
				}
		}
		if(status&(1<<TX_DS))			//�������
		{
				SetRX_Mode(); 				//�ص�����ģʽ
		}
		return 1;
}


/*
 * ��������NRF_Rx_Dat
 * ����  �����ڴ�NRF�Ľ��ջ������ж�������
 * ����  ��rxBuf�����ڽ��ո����ݵ����飬�ⲿ����
 * ���  �����ս����
 * ����  ���ⲿ����
 */
u8 NRF_Rx_Dat(u8 *Rxbuf)
{
		u8 state;
		u16 i=0;
		SetRX_Mode();
		NRF_CE_High(); //�������״̬				
		while(NRF_IRQRead)			//�ȴ������ж�
		{
				i++;
				if(i>=10000)
				{
						i = 0;
//						printf("δ���յ�����\n"); 
						return 0;
				}

		}
		NRF_CE_Low();   //�������״̬          
		state=NRF_ReadReg(STATUS);						//��ȡstatus�Ĵ�����ֵ
//		printf("STATE= %d\n",(int)state);	 
		if(state&(1<<RX_DR))                        //���յ�����
		{
				NRF_ReadFIFO(RD_RX_PLOAD,Rxbuf,32);			//��ȡ����
				NRF_WriteReg(CleanRX_FIFO,0xff);         //���RX FIFO�Ĵ���	     
		}
		else   
			return 0;                    //û�յ��κ�����
		if(state&(1<<MAX_RT))						 //����ط����
		{
				if(state&(1<<TXFULL))
						NRF_WriteReg(CleanRX_FIFO,0xff);          //���RX FIFO�Ĵ���
		}
		NRF_WriteReg(NRF_WRITE_REG+STATUS,state);				// ����жϱ�־
 		return 1; 
}


