#include "NRF24L01.h"
#include "delay.h"
#include "usart.h"

#define TxAddrWidth    5				//发送地址宽度（字节）
#define RxAddrWidth    5				//接收地址宽度（字节）
#define TxDataWidth    32				//发送数据宽度（字节）
#define RxDataWidth    32				//接收有效数据宽度（字节）
u8 TxAddr[TxAddrWidth]= {0x20,0x43,0x10,0x10,0x21};
u8 RxAddr[RxAddrWidth]= {0x20,0x43,0x10,0x10,0x21};

/**********************************************************
* 函数名：u8 NRF_WriteReg(u8 RegAddr, u8 Value)
* 输入参数：RegAddr-SPI从设备寄存器地址
*						Value-对SPI从设备地址等于RegAddr的寄存器写入的数据
* 输出参数：status-读取到的字节
* 日期 ：2018-3-8
* 描述：无线模块写寄存器函数
************************************************************/
u8 NRF_WriteReg(u8 RegAddr, u8 Value)
{
		u8 status;
		NRF_CSN_Low();												//选通NRF器件
		status = SPI1_ReadWriteByte(RegAddr);	//写入寄存器地址
		SPI1_ReadWriteByte(Value);						//对刚刚选中的寄存写入数据
		NRF_CSN_High();												//禁止NRF器件
		
		return status;
}

/**********************************************************
* 函数名：u8 NRF_ReadReg(u8 RegAddr)
* 输入参数：Reg-SPI从设备寄存器地址
* 输出参数：RegValue-读取到的字节
* 日期 ：2018-3-8
* 描述：无线模块读寄存器数据函数，先选地址，
*				后写0从该地址中提取数据
************************************************************/
u8 NRF_ReadReg(u8 RegAddr)
{
		u8 RegValue;
		NRF_CSN_Low();										//选通NRF器件
		SPI1_ReadWriteByte(RegAddr);			//写入寄存器地址
		RegValue = SPI1_ReadWriteByte(0);	//写0，读取该寄存器中的数据
		NRF_CSN_High();										//禁止NRF器件
		
		return RegValue;
}

/**********************************************************
* 函数名：u8 NRF_WriteFIFO(u8 RegAddr,u8 *pBuf,u8 Len)
* 输入参数：RegAddr-SPI从设备寄存器地址
						*pBuf-写入缓冲区的数据
						Len-写入多少个字节
* 输出参数：RegValue-读取到的字节
* 日期 ： 2018-3-8
* 描述：  对缓冲区写入数据
************************************************************/
u8 NRF_WriteFIFO(u8 RegAddr,u8 *pBuf,u8 Len)
{
		u8 i,status;
		NRF_CSN_Low();												//选通NRF器件
		status = SPI1_ReadWriteByte(RegAddr);	//写入寄存器地址
		for(i=0;i<Len;i++)
				SPI1_ReadWriteByte(pBuf[i]);			//写入数据
		NRF_CSN_High();												//禁止NRF器件
	
		return status;
}

/**********************************************************
* 函数名：u8 NRF_ReadReg(u8 RegAddr)
* 输入参数：RegAddr-SPI从设备寄存器地址
						*pBuf-将读取到的数据存入这
						Len-读取多少个字节
* 输出参数：RegValue-读取到的字节
* 日期 ： 2018-3-8
* 描述：  从缓冲区读取数据存至*pBuf
************************************************************/
u8 NRF_ReadFIFO(u8 RegAddr,u8 *pBuf,u8 Len)
{
		u8 i,status;
		NRF_CSN_Low();												//选通NRF器件
		status = SPI1_ReadWriteByte(RegAddr);	//写入寄存器地址
		for(i=0;i<Len;i++)
				pBuf[i]=SPI1_ReadWriteByte(0);		//读取数据
		NRF_CSN_High();												//禁止NRF器件
	
		return status;
}

/**********************************************************
* 函数名：void NRF_Init(u8 CHFrequency, u8 Mode)
* 输入参数：CHFrequency-通道工作频率 一般采用40
*						Mode-工作模式
* 输出参数：无
* 日期 ： 2018-3-8
* 描述：  对NRF24L01进行初始化
************************************************************/
void NRF_Init(u8 CHFrequency, u8 Mode)
{
		NRF_CE_Low();																								//使能NRF器件，进入待机模式
		NRF_WriteReg(CleanTX_FIFO, 0xff);														//清空发送缓冲区
		NRF_WriteReg(CleanRX_FIFO, 0xff);														//清空接收缓冲区
		NRF_WriteFIFO(NRF_WRITE_REG+TX_ADDR,TxAddr,TxAddrWidth);		//写入发送地址
		NRF_WriteFIFO(NRF_WRITE_REG+RX_ADDR_P0,RxAddr,RxAddrWidth);	//写入接收地址
		NRF_WriteReg(NRF_WRITE_REG+EN_AA,0x01);											//使能通道0的自动应答
		NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x01);									//使能通道0的接收地址
		NRF_WriteReg(NRF_WRITE_REG+SETUP_RETR,0x1A);								//设置自动重发间隔时间为500us，最大自动重发10次
		NRF_WriteReg(NRF_WRITE_REG+RF_CH,CHFrequency);    					//设置RF通道的工作频率，一般写入40
		NRF_WriteReg(NRF_WRITE_REG+RX_PW_P0,RxDataWidth);						//设置通道0的有效数据宽度
		NRF_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0f);									//设置发射参数，0dB增益，2Mbps,低噪声增益开启
		
		if(Mode==TX_Mode)
				SetTX_Mode();			//配置为发射模式
		else if(Mode==RX_Mode)	
				SetRX_Mode();			//配置为接收模式
		
		NRF_CE_High();																		//使能
}

/**********************************************************
* 函数名：void SetTX_Mode(void) 
* 输入参数：无
* 输出参数：无
* 日期 ： 2018-3-8
* 描述：  对NRF24L01配置为发射模式
************************************************************/
void SetTX_Mode(void) 
{
		NRF_CE_Low();																			//使能NRF器件，进入待机模式
		NRF_WriteReg(NRF_WRITE_REG+CONFIG,0x0E);			    // 配置为发射模式
		NRF_CE_High();																		//使能发射
		Delay_us(150);
}


/**********************************************************
* 函数名：void SetRX_Mode(void)
* 输入参数：无
* 输出参数：无
* 日期 ： 2018-3-8
* 描述：  对NRF24L01配置为接收模式
************************************************************/
void SetRX_Mode(void) 
{
		NRF_CE_Low();																			//使能NRF器件，进入待机模式
		NRF_WriteReg(NRF_WRITE_REG+CONFIG,0x0F);			    // IRQ收发完成中断响应，16位CRC	，主接收
		NRF_CE_High();																		
		Delay_us(150);
}

/**********************************************************
* 函数名：u8 NRF_Chech(void)
* 输入参数：无
* 输出参数：1-NRF24L01正常  0-NRF24L01异常
* 日期 ： 2018-3-8
* 描述：  对NRF24L01进行异常检查
************************************************************/
u8 NRF_Chech(void)
{
		u8 buf[5];
		u8 i;
		NRF_WriteFIFO(NRF_WRITE_REG+TX_ADDR,TxAddr,5);		//写入发送地址
		NRF_ReadFIFO(TX_ADDR,buf,5);											//写入发送地址
		for(i=0;i<5;i++)
				if(buf[i]!=TxAddr[i])													//数据不准，退出返回0，NRF24L01异常
						return 0;
				
		return 1;			//返回1，NRF24L01无异常，可正常使用
}

/**********************************************************
* 函数名：u8 NRF_SendData(u8 *TxBuf,u8 Len)
* 输入参数：*TxBuf-发送的数据   
*						len-数据长度（字节）
* 输出参数：1-NRF24L01正常发送   0-NRF24L01发送失败
* 日期 ： 2018-3-8
* 描述：  对NRF24L01发送数据
************************************************************/
u8 NRF_SendData(u8 *TxBuf,u8 Len)
{
		u8 conut=0,status;
//		SetTX_Mode();				//发射模式
		NRF_CE_Low();																	//进入待机模式
//		NRF_WriteReg(NRF_WRITE_REG+CONFIG,0x0E);			//配置为发射模式
		NRF_WriteFIFO(WR_TX_PLOAD,TxBuf,Len);					//写入数据
		NRF_CE_High();																//使能发射
		while(NRF_IRQRead)
		{
				conut++;
				if(conut==200)
						return 0;
		}
		status = NRF_ReadReg(STATUS);	
		printf("status= %d\n",status);
		NRF_WriteReg(NRF_WRITE_REG+STATUS,status);			//清除TX_DS或MAX_RT中断标志
		if(status&(1<<MAX_RT))				//达到最多次重发
		{
				if(status&(1<<TXFULL))		//缓冲区已满
				{
						NRF_WriteReg(CONFIG,0xff);			//清空缓冲区
				}
		}
//		if(status&(1<<TX_DS))			//发送完成
//		{
//				SetRX_Mode(); 				//回到接收模式
//		}
		NRF_WriteReg(NRF_WRITE_REG+STATUS,status);			//清除TX_DS或MAX_RT中断标志
		return 1;
}





