#include "mpu6050.h"                  // Device header
#include "usart.h"
#include "iic.h"
#include "delay.h"
/*************************************************
*	函数名：u8 I2CDeviceWriteByte(u8 RegAddress, u8 RegData)
* 参数： RegAddress - 需对IIC设备某一个寄存器操作对应的地址
				 RegData - 需要写入的数据
* 返回值： 无
* 日期 ：2018年3月5日
* 描述 ：利用IIC总线对IIC设备进行一个字节数据写入
****************************************************/
u8 I2CDeviceWriteByte(u8 RegAddress, u8 RegData)
{
		if(!Analogi2cStart()) 						//判断IIC是否启动成功
		{
				printf("IIC写入启动失败\n");
				return 0;											//启动错误，退出
		}		
		Analogi2cSendByte(SlaveAddress);	//发送从机地址+写指令
		if(!Analogi2cWaitAck())
		{
				printf("IIC写入从机地址无应答\n");
				Analogi2cStop();
				return 0;											//无应答，退出
		}
		Analogi2cSendByte(RegAddress);		//发送将要操作的寄存器地址
		Analogi2cWaitAck();
		Analogi2cSendByte(RegData);				//将需写入的数据发送至IIC总线
		if(!Analogi2cWaitAck())
		{
				printf("IIC写入数据无应答\n");
				Analogi2cStop();
				return 0;											//无应答，退出
		}
		Analogi2cStop();									//释放IIC总线
		
		return 1;
}

/*************************************************
*	函数名：u8 I2CDeviceReadByte(u8 RegAddress)
* 参数： RegAddress - 需对IIC设备某一个寄存器操作对应的地址
* 返回值： RegData
* 日期 ：2018年3月5日
* 描述 ：利用IIC总线从IIC设备读取一个字节数据
****************************************************/
u8 I2CDeviceReadByte(u8 RegAddress)
{
		u8 RegData;
	
		if(!Analogi2cStart()) 						//判断IIC是否启动成功
		{
				printf("IIC读取启动失败\n");
				return 0;											//启动错误，退出
		}		
		Analogi2cSendByte(SlaveAddress);	//发送从机地址+写指令
		if(!Analogi2cWaitAck())
		{
				printf("IIC读取无应答\n");
				Analogi2cStop();
				return 0;											//无应答，退出
		}
		Analogi2cSendByte(RegAddress);		//发送将要操作的寄存器地址
		Analogi2cWaitAck();								//等待应答
		Analogi2cStart();									//启动IIC开始传输数据
		Analogi2cSendByte(SlaveAddress+1);//发送设备地址+读指令
		Analogi2cWaitAck();								//等待应答信号
		RegData = Analogi2cReadByte(0);		//读数据无应答								
		Analogi2cStop();									//释放IIC总线
		
		return RegData;
}


/*************************************************
*	函数名：void MPU6050Init(void)
* 参数： 无
* 返回值： 1 硬件验证成功  0验证失败
* 日期 ：2018年3月5日
* 描述 ：mpu6050初始化
****************************************************/
u8 MPU6050Init(void)
{
		
		I2CDeviceWriteByte(PWR_MGMT_1, 0x80);					//复位所以寄存器
		Delay_ms(100);
		I2CDeviceWriteByte(PWR_MGMT_1, 0x00);					//解除休眠状态
		I2CDeviceWriteByte(GYRO_CONFIG, GYRO_FS_2000);//陀螺仪2000度/S 65.5LSB/g
		I2CDeviceWriteByte(ACCEL_CONFIG, ACCEL_FS_4); //加速度+-2g  2048LSB/g
		I2CDeviceWriteByte(SMPLRT_DIV, 0x01);					//6050采样频率为500Hz
		I2CDeviceWriteByte(CONFI, 0x00);							//滤波：ACCEL 21Hz GYRO 21Hz  延时：A8.5ms G8.3ms
		I2CDeviceWriteByte(USER_CTRL, 0x00);					//关闭所有中断
		I2CDeviceWriteByte(INT_EN, 0x00);							//I2C主模式关闭
		I2CDeviceWriteByte(FIFO_EN, 0x00);						//关闭FIFO
		if(I2CDeviceReadByte(WHO_AM_I) != 0x68)
		{
				printf("MPU6050验证失败\n");
			  return 0;
		}
		printf("MPU6050验证成功\n");
		I2CDeviceWriteByte(PWR_MGMT_1, 0x01);					//设置CLKSEL,PLL X轴为参考
		I2CDeviceWriteByte(PWR_MGMT_2, 0x00);					//加速度与陀螺仪都工作
		I2CDeviceWriteByte(SMPLRT_DIV, 0x07);	
		return 1;
}

/*************************************************
*	函数名：int GetData(u8 REG_Address)
* 参数： RegAddress-传感器数据存放地址
* 返回值： 传感器原始数据
* 日期 ：2018年3月5日
* 描述 ：获取IIC设备数据
****************************************************/
short GetData(u8 RegAddress)
{
	char H,L;
	H=I2CDeviceReadByte(RegAddress);
	L=I2CDeviceReadByte(RegAddress+1);
	return (H<<8)+L;   //合成数据
}

