#include "spi.h"
#include "usart.h"
/************************************************************
* 函数名：void SPI1_Init(void)
* 输入参数：无
* 输出参数：无
* 日期 ：2018-3-8
* 描述：对硬件SPI1进行配置，使用全双工模式、8位数据格式，速率为9MHz
************************************************************/
void SPI1_Init(void)
{
		RCC->APB2ENR |= 1<<2;  			//PORTA时钟使能 
		RCC->APB2ENR |= 1<<3;  			//PORTB时钟使能 	 
		RCC->APB2ENR |= 1<<12;   		//SPI1时钟使能 
																//这里只针对SPI口初始化
		GPIOA->CRL   &= 0X0000FFFF; 
		GPIOA->CRL	 |= 0X99930000;	//PA5、6、7复用推挽输出 	    
		GPIOA->ODR	 |= 0X7<<5;   	//PA5、6、7置高
																//NRF24L01引脚初始化
		GPIOB->CRH   &= 0X0F0FFFFF; 
		GPIOB->CRH	 |= 0X80300000;	//PB13、14推挽输出 ,PB15上拉输入   CE-13 IRQ-15   
		GPIOB->ODR	 |= 0X01<<15;   //PB15上拉，等待低电平
		GPIOA->ODR	 |= 0X01<<4;
		SPI1->CR1	&= ~(1<<10);	//全双工模式	
		SPI1->CR1	|= 1<<9; 		//软件NSS管理
		SPI1->CR1	|= 1<<8;  
		SPI1->CR1 |= 1<<2; 			//SPI主机
		SPI1->CR1 &= ~(1<<11);	//8bit数据格式	
		SPI1->CR1 &= ~(1<<1); 	//空闲模式下SCK为0   CPOL=0
		SPI1->CR1 &= ~(1<<0); 	//数据采样从第一个时间边沿开始,CPHA=0  						
		SPI1->CR1 |= 2<<3; 			//Fsck=Fpclk1/8   8分频，9MHz    010，八分频
		SPI1->CR1 &= ~(1<<7); 	//MSBfirst 高位在前  
		SPI1->CRCPR = 0x07;
		SPI1->CR1 |= 1<<6; 			//SPI设备使能
}

/*******************8************************************
* 函数名：u8 SPI1_ReadWriteByte(u8 WriteData)
* 输入参数：WriteData 写入的数据
* 输出参数：读取到的字节
* 日期 ：2018-3-8
* 描述：SPI总线最基本的读写函数，读与写都是本函数
************************************************************/
u8 SPI1_ReadWriteByte(u8 WriteData)
{		
	u16 retry=0;				 
	while((SPI1->SR&(0x01<<1))==0)			//等待发送区空	
	{
		retry++;
		if(retry>=60000)
		{
				printf("SPI发送区不为空，无法发送\n");
				return 0; 	//超时退出
		}
	}			  
	SPI1->DR=WriteData;	 	  			//发送一个byte 
	retry=0;
	while((SPI1->SR&0x01)==0) 		//等待接收完一个byte  
	{
		retry++;
		if(retry>=0XFFFE)
		{
				printf("SPI接收超时\n");
				return 0;	//超时退出	
		}
	}	  						    
	return SPI1->DR;          		//返回收到的数据				    
}







