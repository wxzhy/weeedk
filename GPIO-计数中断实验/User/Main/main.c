
/************************************************************************************
**	  WEEEDK NUAA_CM3_107ʵ�鿪���壨V4.1��
**	  �ⲿ�ж�ʵ��
**		���ܣ���һ��KEY1����LED1��˸����KEY2����LED2��˸��ͬʱ��KEY3ȫ��
**    ���ܼ����󣺳�̬�޼����£�LED1��LED2ȫ���𣬰���KEY1��LED1��˸������KEY2��LED2��˸,����KEY3��ֹͣ��˸
**                �����������ж�
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
*�������ƣ�void Delay(void)
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

int main(void)
{
u8 i=0;
	SystemInit();     	    	/*ϵͳ��ʼ�����˾��ʡ����Ϊ�����ļ����е�����γ���*/
	GPIO_Configuration();			/*GPIO��ʼ����LED��ʼ��,�����˿�����*/
	NVIC_Configuration();	  	/*NVIC����*/
	EXTI_Configuration();			/*�����ж���*/
	Times=0;
	while (1)
	{
		if (Flag)
		{
			for (i=0;i<Times;i++)
			{	LED4(0); 
				Delayms(300);
				LED4(1);
				Delayms(300);
			}
			Times=0;Flag=0;
		}	
	}
}
										  

int fputc(int ch, FILE *f)
{
  
  USART_SendData(USARTx, (uint8_t) ch);   				      //����һ���ַ�

  while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)  //�ȴ��������
  {}

  return ch;
}


