/********************************************************
*  采用WEEEDK NUAA_CM3_107实验开发板
*  SysTick定时器定时实验
   初始化SysTick之后，练习定时指定时间定时，判断计时是不到。让LED发光管按照指定闪烁时间闪烁。
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
	DelayOnSysTick_Init();          /*初始化SysTick定时器来实现精确延时*/
	LCD_Init();											/*LCD初始化*/
	Welcome(); 											/*显示主界面*/
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
