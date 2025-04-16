/********************************************************
1.本实验硬件平台：NUAA_CM3_107实验开发板WEEEDK，MCU：STM32F107VCT6
               
2.开发和实验环境：MDK-ARM Keil V5.15及其以上版本
3.例程使用的STM32F10x固件库V3.5.0
4.本实验例程现象说明内容：  
	采用WEEEDK NUAA_CM3_107实验开发板
  STLINK下载运行后，PB8引脚PWM输出
			  	运行程序后PWM输出频率10kHz，起始占空比50%
					
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"


extern u8  	TIM2CH1_CAPTURE_STA;		/*输入捕获状态,低6位（0~5位）表示低电平有效时纪录更新中断的次数，每次更新中断计数65536次（65536us for 1MHz）
																					第6位为下降沿标志，1=有下降沿捕获，0无效
																					第7位为上升沿标志，1=有上升沿捕获，0无效							                */
extern u16	TIM2CH1_CAPTURE_VAL;		/*输入捕获值    */


int main(void)
 {	
	 
	uint16_t Pulse=7200*0.5;				//占空比为50%  周期7200（1ms）
	SystemInit();
	PWM_GPIO_Init();		    		//PWM输出口PB8，初始化
	Init_TIMER();								//定时器4初始化
	Init_PWM(Pulse);			  //PWM的通道3初始化设置	
	GPIO_Configuration();
	TIM2_Cap_Init(0xffff,72-1);					/*以1MHz的频率计数，初始化TIM2定时器  */
	LCD_Init();													/*  LCD初始化        */
	Welcome(); 										 			/*  显示主界面       */
	LED1(1);LED2(1);LED3(1);LED4(1);
	 name_display();
	while(1)
	{		
 		if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11)==0)   //KEY1按下
		{
			Pulse+=100;									   //占空比加100
			if(Pulse>=7200-1) Pulse=1;
			TIM_SetCompare3(TIM4,Pulse);                   //写入定时器4的通道3的占空比值
		}
	
		
		if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12)==0)   //KEY2按下
		{
			Pulse-=100;									   //占空比减100
			if((Pulse<1)||(Pulse>7200))   Pulse=7200-1;
			TIM_SetCompare3(TIM4,Pulse);                   //写入定时器4的通道3的占空比值
		}
			Delay_ms(100);
			LCD_ShowNum(80,190,Pulse,8,16);		/*显示总的脉冲个数*/
			LCD_ShowNum(80,230,Pulse*100/7200,8,16);	/*显示总的脉冲个数*/

}
}


/***********************END OF FILE****/
