/******************************************************************
**	 WEEEDK NUAA_CM3_107ʵ�鿪���壨V4.1��
**	  ʵ�����ݣ��ⲿ�ж�
**    ��    ����V1.1
**	  ��    �ߣ�NUAA
**	  �������:	2016.8.2
********************************************************************/

#include "hw_config.h"

/*********************************************
*�������ƣ�void NVIC_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵�����ж�Ƕ�׹���
**********************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	            /*ʹ�����ȼ�����2*/

	/*�ⲿ0�ж���*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn ;	   	 			/*����EXTI��0�ߵ��ж����� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;	/*��ռ���ȼ�0*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	        /*�����ȼ�1*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;	
	NVIC_Init(&NVIC_InitStructure);
	/*�ⲿ11�ж���*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn ;	 			/*����EXTI��11�ߵ��ж����� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1 ;	/*��ռ���ȼ�1*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	        /*�����ȼ�2*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;	
	NVIC_Init(&NVIC_InitStructure);
}

/*************************************************
*�������ƣ�void GPIO_Configuration(void)
*
*����˵����GPIO��ʼ������,������ʼ������
***************************************************/

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE); /*PD�˿ڼ����ö˿�ʱ��ʹ��*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3  | GPIO_Pin_4  | GPIO_Pin_7 ; /*LED1(PD2)��LED2(PD3)/LED3(PD4)/LED4(PD7)��������Ϊ50MHz���*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 									/*KEY1(PD11)��������Ϊ��������*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 									/*KEY4(PA0)��������Ϊ��������*/	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA , GPIO_PinSource0);/*PA0��Ϊ�ⲿ�ж���*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource11);/*PD11��Ϊ�ⲿ�ж���*/

	/*---------��ʼ��״̬�ĸ�LEDȫOFF------------*/
	LED1(1);
	LED2(1);
	LED3(1);
	LED4(1);		
}

/***************************************************
*�������ƣ�void EXTI_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����PD11��PA0����Ϊ�½��ش������ⲿ�ж�����
*****************************************************/
void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	/*PD11�ⲿ�ж�����*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line11; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//�½��ش���
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/*PC0�ⲿ�ж�����*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_Init(&EXTI_InitStructure);	
	
}

