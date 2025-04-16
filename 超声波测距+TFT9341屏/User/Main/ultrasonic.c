#include "stm32f10x.h"
#include "ultrasonic.h"
#include"delay.h"

/********************************************************************************************************************************************
@f_name: void Ultran_Init(void)
@brief:	 ������Ӳ��ģ���ʼ��
@param:	 None
@return: None
*********************************************************************************************************************************************/
void Ultran_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;       //���͵�ƽ����PA2
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;		//���ص�ƽ����PA2
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}
/********************************************************************************************************************************************
@f_name: void Ranging(void)
@brief:	 ��������࣬��ֵ5�Σ�ʵ�� �������̷��ֲ�����̬�ıȽ��ȶ�����̬�Ĳ��ȶ���3�׷�Χ�ڱȽ��ȶ�
@param:	 float *p:��໺�����
@return: None
*********************************************************************************************************************************************/
void Ultra_Ranging(float *p)
{
	u8 i=0;
	u32 j=0;
	float Ultr_Temp=0;	
	for(i=0;i<5;i++)
	{
		TRIG_Send=1;
		Delay_ms(10);
		TRIG_Send=0;
		while(!ECHO_Reci);
		while(ECHO_Reci)
		{
			Delay_us(10);
			j++;
		}
		Ultr_Temp+=340/2*j*10;//  ģ�����ɲ��3m 
		j=0;
		Delay_ms(60);//��ֹ�����źŶԻ����źŵ�Ӱ��
	}
	*p=Ultr_Temp/5/1000000; 	
	//*p=Ultr_Temp/1000000; 	
}
