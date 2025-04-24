/********************************************************
ADC应用－板载电位器分压测量
1.本实验硬件平台：NUAA_CM3_107实验开发板WEEEDK，MCU：STM32F107VCT6               
2.开发和实验环境：MDK-ARM Keil V5.15及其以上版本
3.例程使用的STM32F10x固件库V3.5.0
4.本实验例程现象说明内容：  
	ADC应用,模拟通道3作为板载电位器电压检测引脚（PA3），采集到通道3的变换结果，变换为电压值（mA）显示在LCD屏上。
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"


	extern u16 value;    
	float ADC_Value=0.00;                         /*内部电压值*/
	float Pressure=0.00;                         /*内部压力值*/

int main(void)
{
		SystemInit();
		GPIO_Configuration();
		ADC_Configuration ();		        						/*ADC1 初始化设置*/
		LCD_Init();																	/*  LCD初始化    */
		Welcome(); 										 							/*  显示主界面   */
	  name_display();
		LED1(1);LED2(1);LED3(1);LED4(1);
	while(1)
	{		
//		value=Read_ADC1_MultiChannel(ADC_Channel_3);     //获取采样值,没有滤波的情况 */	
		value=filter_YDPJ();     //获取采样值,有滤波的情况 */	
		ADC_Value = (3.3*1000/4096)*value;			    /*将AD值转换为电压值mV*/
		LCD_ShowNum(100,150, value,5,16);						/*在LCD屏上显示采样值*/
//		LCD_ShowNum(100,190,ADC_Value,5,16);				/*在LCD屏上显示电压值mV*/
		Pressure=10+(1000-10)*(value-100)/(4095-100);
		LCD_ShowNum(100,190,Pressure,5,16);				/*在LCD屏上显示压力值kpa*/
		Delay_ms(300);
		GPIOD->ODR^=(1<<2);													/*让LED1闪烁显示*/
	}
}


/***********************END OF FILE****/
