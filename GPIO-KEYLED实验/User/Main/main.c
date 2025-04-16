
/************************************************************************************
**	  WEEEDK NUAA_CM3_107ʵ�鿪���壨V4.1��
**	  GPIOӦ��:�����˻������ӿ�ʵ��
**    �������������س������к󿴵��ĸ�LEDָʾ������(�����)
			������KEY1��ʱ��LED1���⣬������KEY2��ʱ��LED2���⣬������KEY3��ʱ��LED3���⣬������KEY4��ʱ��LED4����
			��ԭ�д�������ϣ���Ӵ����ð�һ�ΰ�������������һ��
**    ��    ����V4.1
**	  ��    �ߣ�NUAA
**	  �������:	2017.3.19
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

/*********************************************
*�������ƣ�GPIO_KEYLED_Init(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵������ʼ��GPIO��������LED��ʼ��
**********************************************/
void GPIO_KEYLED_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);  /*��ʼ��GPIOD����˿�*/
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);  /*��ʼ��GPIOD�˿�PD11=KEY1,PD12=KEY2�������룬50MHz*/

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);  /*��ʼ��GPIOC�˿�PC13=KEY3�������룬50MHz*/
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);  /*��ʼ��GPIOC�˿�PC0���������50MHz*/
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);  /*��ʼ��GPIOA�˿�PA0=KEY4�������룬50MHz*/
	/*---------��ʼ��״̬�ĸ�LEDȫ��OFF------------*/
	GPIO_SetBits(GPIOD,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7);
}

#define n 200
u8 KEY=0;
void Beep(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	Delayms(n);
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
}

int main(void)
{
//u8 KEY=0;	
u8 i=0;
u16 Pinx[4]={GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4,GPIO_Pin_7};
	SystemInit();     		/* ϵͳ��ʼ�� */
	GPIO_KEYLED_Init();		/* GPIO��ʼ��	*/
	while (1)
	{	
	//if (GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)==0) 			 KEY=1;
	if ((GPIOD->IDR&(1<<11))==0) 			 KEY=1;
	//else {if (GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)==0) KEY=2;
	else {if ((GPIOD->IDR&(1<<12))==0) KEY=2;
	//else {if (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==0) KEY=3;
	else {if ((GPIOC->IDR&(1<<13))==0) KEY=3;
	//if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0)  			 KEY=4;}}
	if ((GPIOA->IDR&1)==0)  			 KEY=4;}}
		  //if(KEY==0){GPIO_ResetBits(GPIOD,Pinx[i]);	/*  LEDi+1�� 		*/	
			if(KEY==0){GPIOD->ODR&=~Pinx[i];
					Delayms(n);													/*  ��ʱn ms  	*/
					//GPIO_SetBits(GPIOD,Pinx[i]);	 			/*  LEDi+1��  	*/
					GPIOD->ODR|=Pinx[i];
					Delayms(n);
				  i++;
					if (i>=4) i=0;	}
			//else {GPIO_ResetBits(GPIOD,Pinx[KEY-1]);/*  LED[KEY-1]��  */
			else {GPIOD->ODR&=~Pinx[KEY-1];
					Beep();
					Delayms(n);													/*  ��ʱn ms      */
					//GPIO_SetBits(GPIOD,Pinx[KEY-1]);		/*  LED[KEY-1]��  */
				  GPIOD->ODR|=Pinx[KEY-1];
					Delayms(n);		
					KEY=0;
			}
	}
}
										  









