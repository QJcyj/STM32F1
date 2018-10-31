#ifndef _mpu6050_h_
#define _mpu6050_h_

#include "stm32f10x.h"                  // Device header

/**********MPU6050�Ĵ�����ַ*********************/

#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz) 0x01(500Hz)
#define	CONFI         0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)	 0x01(ACCEL 184Hz GYRO 188Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ
#define FIFO_EN     	0X23	//FIFOʹ�ܼĴ���
#define INT_EN   	    0X38	//�ж�ʹ�ܼĴ���
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define USER_CTRL 		0X6A	//�û����ƼĴ���
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define PWR_MGMT_2		0X6C
#define	WHO_AM_I		  0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define	SlaveAddress	0xD0	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ


/**********���ò���******************/
#define GYRO_FS_250   0x00	//
#define GYRO_FS_500   0x08
#define GYRO_FS_1000  0x10
#define GYRO_FS_2000  0x18
#define ACCEL_FS_2    0x00
#define ACCEL_FS_4    0x08
#define ACCEL_FS_8    0x10
#define ACCEL_FS_16   0x18

extern float roll;
extern float pitch;
extern float yaw;	
extern float Acc_rad_X;
extern float Acc_rad_Y;
extern float Acc_rad_Z;
extern float Gyro_angle_X;
extern float Gyro_angle_Y;
extern float Gyro_angle_Z;
extern float Temperature;
extern float Angle_Trig_x,Angle_Trig_y;
/***********����������************/

u8 I2CDeviceWriteByte(u8 RegAddress, u8 RegData);
u8 I2CDeviceReadByte(u8 RegAddress);
u8 MPU6050Init(void);
short GetData(u8 RegAddress);
extern void GetAngle(void);

#endif

