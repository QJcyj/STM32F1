#include "mpu6050.h"
#include "math.h"
#include "usart.h"

#define pi 			3.14159265f                           
#define Kp 			5.0f 				//���ڴ�ֵ���ı����������ٶȵ��ٶ�                     
#define Ki 			0.001f   		//���ֳ���
#define halfT 	0.005f   		//������
//#define T		  	0.01				//����

float q0=1, q1=0, q2=0, q3=0;									//��Ԫ��������ȫ�ֱ���
float exInt=0, eyInt=0, ezInt=0;							//�������ۼ�ֵ
float Acc_X,Acc_Y,Acc_Z;											//������ٶ�ԭʼ����
float Gyro_X,Gyro_Y,Gyro_Z;										//����������ԭʼ����
float Acc_rad_X,Acc_rad_Y,Acc_rad_Z;					//������ٶȽǶȣ������ƣ�
float Gyro_angle_X,Gyro_angle_Y,Gyro_angle_Z;	//���������ǽǶȣ������ƣ�
float roll,pitch,yaw;													//����Ƕȣ�ŷ���ǣ�
float Temperature;														//�¶�
float Angle_Trig_x,Angle_Trig_y;							//����Ƕȣ����Ǻ���ֱ�ӽ��㣩

/*************************************************
*	��������void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
* ������ 
* ����ֵ�� 
* ���� ��2018��3��8��
* ���� ��������Ԫ�������������̬��
****************************************************/
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
		float norm;
		float vx, vy, vz;
		float ex, ey, ez;

		//�Ѽ��ٶȼƵ���ά����ת�ɵ�λ����
		norm = sqrt(ax*ax + ay*ay + az*az);      
		ax = ax /norm;
		ay = ay / norm;
		az = az / norm;
	
		//�����������ٶȷ���զ����������ϵ�еı�ʾ��Ϊ��Ԫ����ʾ����ת����ĵ�����				 
		vx = 2*(q1*q3 - q0*q2);												
		vy = 2*(q0*q1 + q2*q3);
		vz = q0*q0 - q1*q1 - q2*q2 + q3*q3 ;
		
		//���ٶȶ�ȡ�������������ٶȷ���Ĳ�ֵ����������˼���
		ex = (ay*vz - az*vy) ;                           					
		ey = (az*vx - ax*vz) ;
		ez = (ax*vy - ay*vx) ;
		
		//����ۼƣ�������ֳ������
		exInt = exInt + ex * Ki;								 
		eyInt = eyInt + ey * Ki;
		ezInt = ezInt + ez * Ki;
		
		//�ò��(�������)�������PI������������ƫ�������������Ƕ�ȡ�е�ƫ����
		gx = gx + Kp*ex + exInt;					   							
		gy = gy + Kp*ey + eyInt;
		gz = gz + Kp*ez + ezInt;				   							
		
		//һ�׽����㷨������Ԫ���˶�ѧ���̵���ɢ����ʽ�ͻ���	
		q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
		q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
		q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
		q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

		//��Ԫ���淶��
		norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
		q0 = q0 / norm;
		q1 = q1 / norm;
		q2 = q2 / norm;
		q3 = q3 / norm;

		roll = asin(2*(q0*q2-q1*q3 ))* 57.2957795f; 										// ��� ��Ԫ���ó���ŷ����
		pitch = asin(2*(q0*q1+q2*q3 ))* 57.2957795f; 										// ����
		yaw = atan2(2*(q0*q1+q2*q3),q0*q0+q1*q1-q2*q2-q3*q3)*57.2957795;// ����
}

/*************************************************
*	��������void GetAngle(void)
* ������ ��
* ����ֵ�� ��
* ���� ��2018��3��8��
* ���� ����ȡ6050ԭʼ���ݽ�����̬����
****************************************************/
void GetAngle(void)
{
		
		Temperature = 36.53+((float)GetData(TEMP_OUT_H)/340);
		//������ٶ�ԭʼ����
		Acc_X = GetData(ACCEL_XOUT_H);
		Acc_Y = GetData(ACCEL_YOUT_H);
		Acc_Z = GetData(ACCEL_ZOUT_H);
		
		//����������ԭʼ����
		Gyro_X = GetData(GYRO_XOUT_H);
		Gyro_Y = GetData(GYRO_YOUT_H);
		Gyro_Z = GetData(GYRO_ZOUT_H);
		
		//��������ٶ�Ԥ����Ϊ������
		Acc_rad_X = (Acc_X)/8192;  		//���ٶ����� +-4g/S
		Acc_rad_Y = (Acc_Y)/8192;	 		//ת����ϵ8192LSB/g
		Acc_rad_Z = (Acc_Z)/8192;
		
		//������������Ԥ����Ϊ�Ƕ�
		Gyro_angle_X = (Gyro_X)/16.4;
		Gyro_angle_Y = (Gyro_Y)/16.4;	//���������� +-2000��/S
		Gyro_angle_Z = (Gyro_Z)/16.4;	//ת����ϵ16.4LSB/��	
	
		//��ת��Ϊ�Ƕ��ƵĽ��ٶ����ݺ�ת��Ϊ�����Ƶļ��ٶ����ݷ�����Ԫ��������̬���㣬0.174533ΪPI/180 Ŀ���ǽ��Ƕ�ת����
		IMUupdate((Gyro_angle_X)*0.0174533,(Gyro_angle_Y)*0.0174533,(Gyro_angle_Z)*0.0174533
							, Acc_rad_X, Acc_rad_Y, Acc_rad_Z);
		
		//���Ǻ���ֱ�ӽ����Թ��Ƚ���Ԫ�����㾫׼��
		Angle_Trig_x=-(atan(Acc_X/sqrt(Acc_Y*Acc_Y+Acc_Z*Acc_Z))*57.2957795f); //�����������180/PI Ŀ���ǻ���ת�Ƕ�
		Angle_Trig_y=atan(Acc_Y/sqrt(Acc_X*Acc_X+Acc_Z*Acc_Z))*57.2957795f;
}


