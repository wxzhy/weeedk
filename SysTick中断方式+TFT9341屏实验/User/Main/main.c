/********************************************************
*  ����WEEEDK NUAA_CM3_107ʵ�鿪����
*  SysTick��ʱ����ʱʵ��
��ʼ��SysTick֮��ͨ������ȷ����ʱʱ�䣬�����ж���LED����ܰ���ָ����˸ʱ����˸��
��KEY1��,��LED1��LED4�ĸ����������ָʾ�ƣ�ÿ��200ms��˸һ�Σ��������100ms��;
��KEY2��,��LED1��LED4�ĸ����������ָʾ�ƣ�ÿ��500ms��˸һ�Σ��������250ms��;
��KEY3��,��LED1��LED4�ĸ����������ָʾ�ƣ�ÿ��1����˸һ�Σ��������500ms��;
��KEY4��,��LED1��LED4�ĸ����������ָʾ�ƣ�ÿ��2��˸һ�Σ��������1�룩;
�����ò�ѯ��ʽ��LED��˸���жϷ�ʽ
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"
#include "delay.h"




#define KEY1	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)
#define KEY2	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)
#define KEY3	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)
#define KEY4	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)

int main(void)
{
	u8 KEY=0;
	u16 DTimes;
	SystemInit();
	GPIO_Configuration();

	LCD_Init();											/*LCD��ʼ��*/
	
	Welcome(); 											/*��ʾ������*/
	Delay_ms(1000);
	DelayOnSysTick_Init(50);          /*��ʼ��SysTick��ʱ����ʵ�־�ȷ��ʱ50ms�ж�һ��*/
	LED1(1);LED2(1);LED3(1);LED4(1);
	while(1)
	{		
		
		if (KEY1==0) KEY=1;else {
		if (KEY2==0) KEY=2;else {
		if (KEY3==0) KEY=3;else {
		if (KEY4==0) KEY=4;}}}
		if (KEY!=0)
		{
			switch (KEY)
			{
				case 1:DTimes=100;break;
				case 2:DTimes=250;break;
				case 3:DTimes=500;break;
				case 4:DTimes=1000;break;
			}
			SysTick->LOAD=(u32)(DTimes*(SystemCoreClock/8000));/*ʱ�����Dtimes ms*/
			LCD_ShowNum(100,250,DTimes,4,16);	
				KEY=0;
		}
	
	}
}


/***********************END OF FILE****/
