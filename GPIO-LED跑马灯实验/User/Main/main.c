
/************************************************************************************
**	  WEEEDK NUAA_CM3_107ʵ�鿪���壨V4.1/V4.2��
**	  GPIO-LEDӦ�������ʵ��
**    �������������س������к󿴵��ĸ�LEDָʾ�����⣨��������������ƣ�
**    ��    ����V4.1
**	  ��    �ߣ�NUAA
**	  �������:	2017.3.18  
************************************************************************************/
#include "stm32f10x_conf.h"

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
void GPIO_LED_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); /*ʹ��PD��ʱ��*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7 ;/*ѡ��PD2/PD3/PD4/PD7����*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;  		/*Ƶ��10MHz*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			/*�������*/
	GPIO_Init(GPIOD, &GPIO_InitStructure);                /*��ʼ��GPIOD�˿�*/	
	/*---------��ʼ��״̬�ĸ�LEDȫ��OFF------------*/
	GPIO_SetBits(GPIOD,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7);/*PD2/PD3/PD4/PD7���Ϊ�ߵ�ƽ����LEDȫ��*/
}

#define  n 300    /*��ʱʱ�䳣��*/

int main(void)
{
//	SystemInit();     	//ϵͳ��ʼ�����˴�����ʡ����Ϊ�������ļ����иú����ĵ���
	GPIO_LED_Init();		//GPIO��ʼ��
	
	while (1)
	{	
  GPIO_ResetBits(GPIOD,GPIO_Pin_2);/* PD2=0 LED1��  */
	Delayms(n);											 /* ��ʱ          */
  GPIO_SetBits(GPIOD,GPIO_Pin_2);	 /* PD2=1 LED1��  */
	Delayms(n);											 /* ��ʱ          */
  GPIO_ResetBits(GPIOD,GPIO_Pin_3);/* PD3=0 LED2��  */
	Delayms(n);											 /* ��ʱ          */
  GPIO_SetBits(GPIOD,GPIO_Pin_3);  /* PD3=1 LED2��  */
	Delayms(n);											 /* ��ʱ          */
  GPIO_ResetBits(GPIOD,GPIO_Pin_4);/* PD4=0 LED3��  */
	Delayms(n);											 /* ��ʱ          */
  GPIO_SetBits(GPIOD,GPIO_Pin_4);  /* PD4=1 LED4��  */
	Delayms(n);											 /* ��ʱ          */
  GPIO_ResetBits(GPIOD,GPIO_Pin_7);/* PD7=0 LED4��  */
	Delayms(n);											 /* ��ʱ          */
  GPIO_SetBits(GPIOD,GPIO_Pin_7);  /* PD7=1 LED4��  */
	Delayms(n);										 	 /* ��ʱ          */
	}	
}
										  








