/********************************************************
1.本实验硬件平台：NUAA_CM3_107实验开发板WEEEDK，MCU：STM32F107VCT6
               
2.开发和实验环境：MDK-ARM Keil V5.15及其以上版本
3.例程使用的STM32F10x固件库V3.5.0
4.本实验例程现象说明内容：  
	采用WEEEDK NUAA_CM3_107实验开发板
	定时器2(TIM2),TIM2的通道1(TIM2_CH1,连接在PA0上面),使用定时器的输入捕获功能来
	捕捉PA0上面的低电平脉宽. 	
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"
#include "delay.h"

extern u8  	TIM2CH1_CAPTURE_STA;		/*输入捕获状态,低6位（0~5位）表示低电平有效时纪录更新中断的次数，每次更新中断计数65536次（65536us for 1MHz）
																					第6位为下降沿标志，1=有下降沿捕获，0无效
																					第7位为上升沿标志，1=有上升沿捕获，0无效							                */
extern u16	TIM2CH1_CAPTURE_VAL;		/*输入捕获值    */


int main(void)
 {	
	 
	u32 temp=0; 
	SystemInit();
	GPIO_Configuration();
	TIM2_Cap_Init(0xffff,72-1);					/*以1MHz的频率计数，初始化TIM2定时器  */
	LCD_Init();													/*  LCD初始化        */
	Welcome(); 										 			/*  显示主界面       */
	LED1(1);LED2(1);LED3(1);LED4(1);
	while(1)
	{		
 		Delay_ms(100);
		if(TIM2CH1_CAPTURE_STA&(1<<7))		/*成功捕获到了一次低电平*/
		{
			temp=TIM2CH1_CAPTURE_VAL+65536*(TIM2CH1_CAPTURE_STA&0x3F)+1;/*得到总的低电平时间*/
			TIM2CH1_CAPTURE_STA=0;					/*开启下一次捕获*/
			LCD_ShowNum(80,160,temp,8,16);	/*显示总的低电平时间*/
	}
}
}


/***********************END OF FILE****/
