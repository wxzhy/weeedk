/******************************************************************
**	  NUAA_CM3_107ʵ�鿪���壨V1.0��
**	  ���������ļ�
**    ��    ����V1.1
**	  ��    �ߣ�NUAA
**	  �������:	2016.8.1
********************************************************************/

#include "hw_config.h"

/*********************************************
*�������ƣ�void NVIC_Configuration(void)
*����˵�����ж�Ƕ�׹���
**********************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	            //ʹ�����ȼ�����2

	/*�ⲿ�ж���*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn ;	    //����EXTI��15~10�ߵ��ж����� 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;	//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	        //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;	
	NVIC_Init(&NVIC_InitStructure);
}

/*************************************************
*�������ƣ�void GPIO_Configuration(void)
LED1:PD2,LED2:PD3,LED3:PD4,LED4:PD5;����Ϊ�������
GPIO����Ƶ��10MHz
*����˵����GPIO��ʼ������
***************************************************/

void GPIO_Configuration(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/*---------��ʼ��״̬�ĸ�LEDȫ��OFF------------*/
	GPIO_SetBits(GPIOD,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7);
}

/*****************************************************
*�������ƣ�void BUTTON_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����BUTTON��ʼ������
*******************************************************/
void BUTTON_Configuration(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource11);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource12);
}

/***************************************************
*�������ƣ�void EXTI_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����EXTI��ʼ������
*****************************************************/
void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	/*PD11�ⲿ�ж�����*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line11; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/*PD12�ⲿ�ж�����*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	 //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE; 
	EXTI_Init(&EXTI_InitStructure);	
}

