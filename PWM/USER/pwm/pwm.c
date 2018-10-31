#include "pwm.h"


void TIM2PWMInit(u32 arr,u32 psc)
{

	RCC->APB1ENR |=1; 	//TIM2ʱ��ʹ��   
	RCC->APB2ENR |=1<<2;    	//ʹ��PORTAʱ��
	RCC->APB2ENR |=1<<3;    	//ʹ��PORTBʱ��	
	GPIOA->CRL   &=0XFFFF0000;	//PA���
	GPIOA->CRL   |=0X0000BBBB;	//���ù������ 
	TIM2->ARR=arr;			//�趨�������Զ���װֵ 
	TIM2->PSC=psc;			//Ԥ��Ƶ������Ƶ
	TIM2->CCMR1|=7<<4;  	//CH1 PWM2ģʽ		 
	TIM2->CCMR1|=1<<3; 	  //CH1Ԥװ��ʹ��	 
	TIM2->CCMR1|=7<<12;  	//CH2 PWM2ģʽ		 
	TIM2->CCMR1|=1<<11; 	//CH2Ԥװ��ʹ��	
	TIM2->CCMR2|=7<<4;  	//CH3 PWM2ģʽ		 
	TIM2->CCMR2|=1<<3; 	  //CH3Ԥװ��ʹ��	 
	TIM2->CCMR2|=7<<12;  	//CH4 PWM2ģʽ		 
	TIM2->CCMR2|=1<<11; 	//CH4Ԥװ��ʹ��	
	TIM2->CCER|=0x1111;   	//OC1-4 ���ʹ��	 
	TIM2->CR1=0x0080;   	//ARPEʹ�� 
	TIM2->CR1|=0x01;    	//ʹ�ܶ�ʱ��2����
}

/******************************
*	ch1-ch4��ӦPA0-PA4,ȡֵ��0-1000
*	999��Ӧ1ms
*
*
*******************************/
/************************************************************************************************
*PWMԭ������:���Ǽٶ���ʱ�����������ϼ��� PWMģʽ���ҵ�CNT<CCRxʱ�����0��CNT>=CCRxʱ���1��
��ô�Ϳ��Եõ����µ� PWMʾ��ͼ����CNTֵС��CCRx��ʱ��IO����͵�ƽ(0)����CNTֵ���ڵ���CCRx��ʱ��
IO����ߵ�ƽ(1)����CNT�ﵽARRֵ��ʱ�����¹��㣬Ȼ���������ϼ���������ѭ�����ı� CCRx ��ֵ���Ϳ���
�ı� PWM �����ռ�ձȣ��ı� ARR ��ֵ���Ϳ��Ըı� PWM �����Ƶ��

        ^ CNT
        |___________________________________________ARR
        |	  /|	 /|	    /|	   /|	  /|
        |____/_|____/_|____/_|____/_|____/_|________CCRx
        |	/  |   /  |	  /	 |	 /	|	/  |
        |  /   |  /	  |	 /	 |	/   |  /   |
        | /	   | /	  |	/	 | /    | /	   |
        |/_____|/_____|/_____|/_____|/_____|_______> t
    1	|	 __	    __	   __	  __	 __
        |	|  |   |  |	  |  |	 | 	|	|  |
    0   |___|  |___|  |___|__|___|__|___|__|_________
*****************************************************************************************************/
void PWMOut(u16 ch1,u16 ch2,u16 ch3,u16 ch4)
{
    if(ch2>1000)	ch1=1000;
    if(ch2>1000)	ch2=1000;
    if(ch3>1000)	ch3=1000;
    if(ch4>1000)	ch4=1000;
    TIM2->CCR1 = 19999-(ch1+999);
    TIM2->CCR2 = 19999-(ch2+999);
    TIM2->CCR3 = 19999-(ch3+999);
    TIM2->CCR4 = 19999-(ch4+999);
}







