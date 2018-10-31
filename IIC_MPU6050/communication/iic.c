#include "iic.h"
#include "delay.h"


/*************************************************
* 函数名：void Analogi2cInit(void)
* 参数： 无
* 返回值： 无
* 日期 ：2018年3月5日
* 描述：初始化模拟IIC的IO口
****************************************************/
void Analogi2cInit(void)
{
		RCC->APB2ENR |= 3<<2;
		GPIOA->CRL 	 &= 0xfff0ffff;
		GPIOA->CRL 	 |= 0x00030000;			//PA4推挽输出
		GPIOA->ODR 	 |= 0x01<<4;				//将PA4置高
	
		GPIOB->CRL 	 &= 0xfffffff0;
		GPIOB->CRL 	 |= 0x00000007;			//PB0开漏输出，可做输出又可读
		GPIOB->ODR 	 |= 0x01<<0;				//将PB0置高
}

/*************************************************
*	函数名：void Analogi2cStart(void)
* 参数： 无
* 返回值： 0 - IIC起始失败   1 - IIC起始成功
* 日期 ：2018年3月5日
* 描述 ：产生IIC起始信号
****************************************************/
u8 Analogi2cStart(void)
{	
		SCL_High();			//时钟置高
		SDA_High();			//数据置高
		if(!SDA_Read)		//数据线为低电平，IIC总线忙，退出
				return 0;
		SDA_Low();			//拉低数据线触发IIC起始信号
		if(SDA_Read)		//数据线为高电平，出错退出
				return 0;
		SCL_Low();			//修改部分，原是SDA=1
		
		return 1;
}

/*************************************************
*	函数名：void Analogi2cStop(void)
* 参数： 无
* 返回值： 无
* 日期 ：2018年3月5日
* 描述 ：产生IIC停止信号
****************************************************/
void Analogi2cStop(void)
{
		SCL_Low();			
		SDA_Low();
		SCL_High();			//时钟置高
		SDA_High();			//数据置高
}

/*************************************************
*	函数名：u8 Analogi2cWaitAck(void)
* 参数： 无
* 返回值： 1 - 应答   0 - 无应答退出
* 日期 ：2018年3月5日
* 描述 ：IIC总线等待应答信号
****************************************************/
u8 Analogi2cWaitAck(void)
{
		u8 ErrTime=0;
		SCL_Low();						//时钟线置低
		SDA_High();						//数据线置高
		SCL_High();						//时钟线置高
		while(SDA_Read)				//数据线是否变低
		{
				ErrTime++;
				if(ErrTime>200)		//无应答，则返回0
				{
						Analogi2cStop();
						return 0;
				}
		}
		SCL_Low();						//有应答，拉低时钟线，准备接收或发送数据，返回0
		return 1;
}

/*************************************************
*	函数名：void Analogi2cAck(void)
* 参数： 无
* 返回值： 无
* 日期 ：2018年3月5日
* 描述 ：IIC总线发送应答信号
****************************************************/
void Analogi2cAck(void)
{
		SCL_Low();				//时钟线置低
		Delay_us(1);
		SDA_Low();				//数据线置低		写应答信号
		Delay_us(1);
		SCL_High();				//时钟线置高
		Delay_us(1);
		SCL_Low();				//时钟线置低
}

/*************************************************
*	函数名：void Analogi2cNAck(void)
* 参数： 无
* 返回值： 无
* 日期 ：2018年3月5日
* 描述 ：IIC总线发送非应答信号
****************************************************/
void Analogi2cNAck(void)
{
		SCL_Low();
		SDA_High();				//数据线置高  不写应答信号
		Delay_us(1);
		SCL_High();
		Delay_us(1);
		SCL_Low();
}

/*************************************************
*	函数名：Analogi2cSendByte(u8 Tx)
* 参数： Tx - 要发送的字节数据
* 返回值： 无
* 日期 ：2018年3月5日
* 描述 ：向IIC总线发送（写入）一个字节数据
****************************************************/
void Analogi2cSendByte(u8 Tx)
{
		u8 loop;
		for(loop=0;loop<8;loop++)
		{
				SCL_Low();								//拉低时钟开始数据传输
				Delay_us(1);
				IIC_SDA = (Tx&0x80)>>7;		//将最高位送给SDA
				Tx <<= 1;									//数据左移一位，等待下一次传输
				SCL_High();	
				Delay_us(1);
		}
		SCL_Low();										//拉低，从机在低电平期间输出应答，从机发出的应答信号为SDA=0
}

/*******************************************************
*	函数名：Analogi2cReadByte(void)
* 参数： 
* 返回值： receive - 从IIC读取到的一个字节数据
* 日期 ：2018年3月5日
* 描述 ：从IIC总线读取（获取）一个字节数据
*******************************************************/
u8 Analogi2cReadByte(u8 ACK)
{
		u8 i,receive=0;
		for(i=0;i<8;i++ )
		{
				SCL_Low();
				Delay_us(1);
				SCL_High();
				receive<<=1;
				if(SDA_Read)
					  receive |= 0x01;
				Delay_us(1);
		}
		if(!ACK)
				Analogi2cNAck();
		else
				Analogi2cAck();

		return receive;
}







