/********************************************************
*  采用WEEEDK NUAA_CM3_107实验开发板
*  SysTick定时器定时实验
初始化SysTick之后，通过按键确定定时时间，利用中断让LED发光管按照指定闪烁时间闪烁。
按KEY1键,让LED1到LED4四个发光二极管指示灯，每隔200ms闪烁一次（亮和灭各100ms）;
按KEY2键,让LED1到LED4四个发光二极管指示灯，每隔500ms闪烁一次（亮和灭各250ms）;
按KEY3键,让LED1到LED4四个发光二极管指示灯，每隔1秒闪烁一次（亮和灭各500ms）;
按KEY4键,让LED1到LED4四个发光二极管指示灯，每隔2闪烁一次（亮和灭各1秒）;
按键用查询方式，LED闪烁用中断方式
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

	LCD_Init();											/*LCD初始化*/
	
	Welcome(); 											/*显示主界面*/
	Delay_ms(1000);
	DelayOnSysTick_Init(50);          /*初始化SysTick定时器来实现精确延时50ms中断一次*/
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
			SysTick->LOAD=(u32)(DTimes*(SystemCoreClock/8000));/*时间加载Dtimes ms*/
			LCD_ShowNum(100,250,DTimes,4,16);	
				KEY=0;
		}
	
	}
}


/***********************END OF FILE****/
