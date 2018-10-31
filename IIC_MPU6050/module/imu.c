#include "mpu6050.h"
#include "math.h"
#include "usart.h"

#define pi 			3.14159265f                           
#define Kp 			5.0f 				//调节此值，改变输出跟随加速度的速度                     
#define Ki 			0.001f   		//积分常数
#define halfT 	0.005f   		//半周期
//#define T		  	0.01				//周期

float q0=1, q1=0, q2=0, q3=0;									//四元数，定义全局变量
float exInt=0, eyInt=0, ezInt=0;							//误差积分累计值
float Acc_X,Acc_Y,Acc_Z;											//三轴加速度原始数据
float Gyro_X,Gyro_Y,Gyro_Z;										//三轴陀螺仪原始数据
float Acc_rad_X,Acc_rad_Y,Acc_rad_Z;					//三轴加速度角度（弧度制）
float Gyro_angle_X,Gyro_angle_Y,Gyro_angle_Z;	//三轴陀螺仪角度（弧度制）
float roll,pitch,yaw;													//三轴角度（欧拉角）
float Temperature;														//温度
float Angle_Trig_x,Angle_Trig_y;							//两轴角度（三角函数直接解算）

/*************************************************
*	函数名：void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
* 参数： 
* 返回值： 
* 日期 ：2018年3月8日
* 描述 ：利用四元数计算飞行器姿态角
****************************************************/
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
		float norm;
		float vx, vy, vz;
		float ex, ey, ez;

		//把加速度计的三维向量转成单位向量
		norm = sqrt(ax*ax + ay*ay + az*az);      
		ax = ax /norm;
		ay = ay / norm;
		az = az / norm;
	
		//估计重力加速度方向咋飞行器坐标系中的表示，为四元数表示的旋转矩阵的第三行				 
		vx = 2*(q1*q3 - q0*q2);												
		vy = 2*(q0*q1 + q2*q3);
		vz = q0*q0 - q1*q1 - q2*q2 + q3*q3 ;
		
		//加速度读取方向与重力加速度方向的差值，用向量叉乘计算
		ex = (ay*vz - az*vy) ;                           					
		ey = (az*vx - ax*vz) ;
		ez = (ax*vy - ay*vx) ;
		
		//误差累计，已与积分常数相乘
		exInt = exInt + ex * Ki;								 
		eyInt = eyInt + ey * Ki;
		ezInt = ezInt + ez * Ki;
		
		//用叉积(向量外积)误差来做PI修正陀螺仪零偏，即抵消陀螺仪读取中的偏移量
		gx = gx + Kp*ex + exInt;					   							
		gy = gy + Kp*ey + eyInt;
		gz = gz + Kp*ez + ezInt;				   							
		
		//一阶近似算法，即四元数运动学方程的离散化形式和积分	
		q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
		q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
		q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
		q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

		//四元数规范化
		norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
		q0 = q0 / norm;
		q1 = q1 / norm;
		q2 = q2 / norm;
		q3 = q3 / norm;

		roll = asin(2*(q0*q2-q1*q3 ))* 57.2957795f; 										// 横滚 四元数得出的欧拉角
		pitch = asin(2*(q0*q1+q2*q3 ))* 57.2957795f; 										// 俯仰
		yaw = atan2(2*(q0*q1+q2*q3),q0*q0+q1*q1-q2*q2-q3*q3)*57.2957795;// 航向
}

/*************************************************
*	函数名：void GetAngle(void)
* 参数： 无
* 返回值： 无
* 日期 ：2018年3月8日
* 描述 ：获取6050原始数据进行姿态解算
****************************************************/
void GetAngle(void)
{
		
		Temperature = 36.53+((float)GetData(TEMP_OUT_H)/340);
		//三轴加速度原始数据
		Acc_X = GetData(ACCEL_XOUT_H);
		Acc_Y = GetData(ACCEL_YOUT_H);
		Acc_Z = GetData(ACCEL_ZOUT_H);
		
		//三轴陀螺仪原始数据
		Gyro_X = GetData(GYRO_XOUT_H);
		Gyro_Y = GetData(GYRO_YOUT_H);
		Gyro_Z = GetData(GYRO_ZOUT_H);
		
		//将三轴加速度预处理为弧度制
		Acc_rad_X = (Acc_X)/8192;  		//加速度量程 +-4g/S
		Acc_rad_Y = (Acc_Y)/8192;	 		//转换关系8192LSB/g
		Acc_rad_Z = (Acc_Z)/8192;
		
		//将三轴陀螺仪预处理为角度
		Gyro_angle_X = (Gyro_X)/16.4;
		Gyro_angle_Y = (Gyro_Y)/16.4;	//陀螺仪量程 +-2000度/S
		Gyro_angle_Z = (Gyro_Z)/16.4;	//转换关系16.4LSB/度	
	
		//将转化为角度制的角速度数据和转换为弧度制的加速度数据放入四元数进行姿态解算，0.174533为PI/180 目的是将角度转弧度
		IMUupdate((Gyro_angle_X)*0.0174533,(Gyro_angle_Y)*0.0174533,(Gyro_angle_Z)*0.0174533
							, Acc_rad_X, Acc_rad_Y, Acc_rad_Z);
		
		//三角函数直接解算以供比较四元数解算精准度
		Angle_Trig_x=-(atan(Acc_X/sqrt(Acc_Y*Acc_Y+Acc_Z*Acc_Z))*57.2957795f); //后面的数字是180/PI 目的是弧度转角度
		Angle_Trig_y=atan(Acc_Y/sqrt(Acc_X*Acc_X+Acc_Z*Acc_Z))*57.2957795f;
}


