/********************************************************
*  ����WEEEDK NUAA_CM3_107ʵ�鿪����
*  SysTick��ʱ����ʱʵ��
   ��ʼ��SysTick֮����ϰ��ʱָ��ʱ�䶨ʱ���жϼ�ʱ�ǲ�������LED����ܰ���ָ����˸ʱ����˸��
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"
#include "delay.h"

extern u16 Dtimes;

#define KEY1	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)
#define KEY2	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)
#define KEY3	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)
#define KEY4	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define RLED3 GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_4)
#define RLED4 GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_7)

int main(void)
{
	SystemInit();
	GPIO_Configuration();
	DelayOnSysTick_Init();          /*��ʼ��SysTick��ʱ����ʵ�־�ȷ��ʱ*/
	LCD_Init();											/*LCD��ʼ��*/
	Welcome(); 											/*��ʾ������*/
	LED1(1);LED2(1);LED3(1);LED4(1);
	Dtimes=50;
	while(1)
	{		
	if (KEY1==0)	Dtimes=100;	
		else {if (KEY2==0)	Dtimes=250;	
		else {if (KEY3==0)	Dtimes=500;	
		else {if (KEY4==0)	Dtimes=1000;}}}		
	LED1(0);LED2(0);LED3(0);LED4(0);	
  Delay_ms(Dtimes);
  LED1(1);LED2(1);LED3(1);LED4(1);	
  Delay_ms(Dtimes);
	 }
}


/***********************END OF FILE****/
