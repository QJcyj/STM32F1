#include "mpu6050.h"                  // Device header
#include "usart.h"
#include "iic.h"
#include "delay.h"
/*************************************************
*	��������u8 I2CDeviceWriteByte(u8 RegAddress, u8 RegData)
* ������ RegAddress - ���IIC�豸ĳһ���Ĵ���������Ӧ�ĵ�ַ
				 RegData - ��Ҫд�������
* ����ֵ�� ��
* ���� ��2018��3��5��
* ���� ������IIC���߶�IIC�豸����һ���ֽ�����д��
****************************************************/
u8 I2CDeviceWriteByte(u8 RegAddress, u8 RegData)
{
		if(!Analogi2cStart()) 						//�ж�IIC�Ƿ������ɹ�
		{
				printf("IICд������ʧ��\n");
				return 0;											//���������˳�
		}		
		Analogi2cSendByte(SlaveAddress);	//���ʹӻ���ַ+дָ��
		if(!Analogi2cWaitAck())
		{
				printf("IICд��ӻ���ַ��Ӧ��\n");
				Analogi2cStop();
				return 0;											//��Ӧ���˳�
		}
		Analogi2cSendByte(RegAddress);		//���ͽ�Ҫ�����ļĴ�����ַ
		Analogi2cWaitAck();
		Analogi2cSendByte(RegData);				//����д������ݷ�����IIC����
		if(!Analogi2cWaitAck())
		{
				printf("IICд��������Ӧ��\n");
				Analogi2cStop();
				return 0;											//��Ӧ���˳�
		}
		Analogi2cStop();									//�ͷ�IIC����
		
		return 1;
}

/*************************************************
*	��������u8 I2CDeviceReadByte(u8 RegAddress)
* ������ RegAddress - ���IIC�豸ĳһ���Ĵ���������Ӧ�ĵ�ַ
* ����ֵ�� RegData
* ���� ��2018��3��5��
* ���� ������IIC���ߴ�IIC�豸��ȡһ���ֽ�����
****************************************************/
u8 I2CDeviceReadByte(u8 RegAddress)
{
		u8 RegData;
	
		if(!Analogi2cStart()) 						//�ж�IIC�Ƿ������ɹ�
		{
				printf("IIC��ȡ����ʧ��\n");
				return 0;											//���������˳�
		}		
		Analogi2cSendByte(SlaveAddress);	//���ʹӻ���ַ+дָ��
		if(!Analogi2cWaitAck())
		{
				printf("IIC��ȡ��Ӧ��\n");
				Analogi2cStop();
				return 0;											//��Ӧ���˳�
		}
		Analogi2cSendByte(RegAddress);		//���ͽ�Ҫ�����ļĴ�����ַ
		Analogi2cWaitAck();								//�ȴ�Ӧ��
		Analogi2cStart();									//����IIC��ʼ��������
		Analogi2cSendByte(SlaveAddress+1);//�����豸��ַ+��ָ��
		Analogi2cWaitAck();								//�ȴ�Ӧ���ź�
		RegData = Analogi2cReadByte(0);		//��������Ӧ��								
		Analogi2cStop();									//�ͷ�IIC����
		
		return RegData;
}


/*************************************************
*	��������void MPU6050Init(void)
* ������ ��
* ����ֵ�� 1 Ӳ����֤�ɹ�  0��֤ʧ��
* ���� ��2018��3��5��
* ���� ��mpu6050��ʼ��
****************************************************/
u8 MPU6050Init(void)
{
		
		I2CDeviceWriteByte(PWR_MGMT_1, 0x80);					//��λ���ԼĴ���
		Delay_ms(100);
		I2CDeviceWriteByte(PWR_MGMT_1, 0x00);					//�������״̬
		I2CDeviceWriteByte(GYRO_CONFIG, GYRO_FS_2000);//������2000��/S 65.5LSB/g
		I2CDeviceWriteByte(ACCEL_CONFIG, ACCEL_FS_4); //���ٶ�+-2g  2048LSB/g
		I2CDeviceWriteByte(SMPLRT_DIV, 0x01);					//6050����Ƶ��Ϊ500Hz
		I2CDeviceWriteByte(CONFI, 0x00);							//�˲���ACCEL 21Hz GYRO 21Hz  ��ʱ��A8.5ms G8.3ms
		I2CDeviceWriteByte(USER_CTRL, 0x00);					//�ر������ж�
		I2CDeviceWriteByte(INT_EN, 0x00);							//I2C��ģʽ�ر�
		I2CDeviceWriteByte(FIFO_EN, 0x00);						//�ر�FIFO
		if(I2CDeviceReadByte(WHO_AM_I) != 0x68)
		{
				printf("MPU6050��֤ʧ��\n");
			  return 0;
		}
		printf("MPU6050��֤�ɹ�\n");
		I2CDeviceWriteByte(PWR_MGMT_1, 0x01);					//����CLKSEL,PLL X��Ϊ�ο�
		I2CDeviceWriteByte(PWR_MGMT_2, 0x00);					//���ٶ��������Ƕ�����
		I2CDeviceWriteByte(SMPLRT_DIV, 0x07);	
		return 1;
}

/*************************************************
*	��������int GetData(u8 REG_Address)
* ������ RegAddress-���������ݴ�ŵ�ַ
* ����ֵ�� ������ԭʼ����
* ���� ��2018��3��5��
* ���� ����ȡIIC�豸����
****************************************************/
short GetData(u8 RegAddress)
{
	char H,L;
	H=I2CDeviceReadByte(RegAddress);
	L=I2CDeviceReadByte(RegAddress+1);
	return (H<<8)+L;   //�ϳ�����
}

