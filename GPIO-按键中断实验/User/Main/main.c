
/************************************************************************************
**	  WEEEDK NUAA_CM3_107ʵ�鿪���壨V4.1��
**	  �ⲿ�ж�ʵ��
**		���ܣ���һ��KEY1����LED1��˸����KEY2����LED2��˸��ͬʱ��KEY3ȫ��
**    ���ܼ����󣺳�̬�޼����£�LED1��LED2ȫ���𣬰���KEY1��LED1��˸������KEY2��LED2��˸,����KEY3��ֹͣ��˸
**               �����������ж�
**    ���жϷ������stm32f10x_it.c�����ж���λFlag��־�����������ݲ�ͬ��־����ͬ����
**    ��    ����V1.1
**	  ��    �ߣ�MUAA
**	  �������:	2016.8.26
************************************************************************************/
#include <stdio.h>
#include "hw_config.h"

uint8_t Flag = 0x00;
uint8_t Times = 0x00;


/*********************************************
*�������ƣ�void Delayms(void)
*
*��ڲ�����N  ms
*
*���ڲ�������
*
*����˵��������ʱN ms
**********************************************/
void Delayms(uint16_t N)
{
	uint32_t i;
	for (i=0;i<(8000*N);i++);
}
#define n 200
int main(void)
{
	SystemInit();     	    	/*ϵͳ��ʼ�����˾��ʡ����Ϊ�����ļ����е�����γ���*/
	GPIO_Configuration();			/*�˴����������г�ʼ����������hw_config.c�н��У�GPIO��ʼ����LED��ʼ��,�����˿�����*/
	NVIC_Configuration();	  	/*NVIC����*/
	EXTI_Configuration();			/*�����ж���*/
	while (1)
	{
	switch(Flag)	/*���жϷ���������Flagֵ*/
		 {
			case 0x01:
				LED1(0); 
				Delayms(n);
				LED1(1);
				Delayms(n);
				LED2(1);LED3(1);		  
				break;				
			case 0x02:
				LED2(0);
				Delayms(n);
				LED2(1);
				Delayms(n);
				LED2(1);LED3(1);		  
				break;				
			case 0x03:
				LED1(1);LED2(1);LED3(1);LED4(1);		  
				break;				
			default   :		   
				LED1(1);LED2(1);LED3(1);LED4(1);		  
				break;
	      }
		  }

}
										  

