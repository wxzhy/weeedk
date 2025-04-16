
/************************************************************************************
**	  WEEEDK NUAA_CM3_107实验开发板（V4.1）
**	  外部中断实验
**		功能：接一次KEY1键，LED1闪烁，按KEY2键，LED2闪烁，同时按KEY3全灭。
**    功能及现象：常态无键按下，LED1、LED2全常灭，按下KEY1，LED1闪烁，按下KEY2，LED2闪烁,按下KEY3，停止闪烁
**               单个按键用中断
**    在中断服务程序stm32f10x_it.c中有中断置位Flag标志，主函数根据不同标志做不同处理
**    版    本：V1.1
**	  作    者：MUAA
**	  完成日期:	2016.8.26
************************************************************************************/
#include <stdio.h>
#include "hw_config.h"

uint8_t Flag = 0x00;
uint8_t Times = 0x00;


/*********************************************
*函数名称：void Delayms(void)
*
*入口参数：N  ms
*
*出口参数：无
*
*功能说明：简单延时N ms
**********************************************/
void Delayms(uint16_t N)
{
	uint32_t i;
	for (i=0;i<(8000*N);i++);
}
#define n 200
int main(void)
{
	SystemInit();     	    	/*系统初始化（此句可省，因为启动文件中有调用这段程序）*/
	GPIO_Configuration();			/*此处及下面三行初始化工作均在hw_config.c中进行，GPIO初始化：LED初始化,按键端口配置*/
	NVIC_Configuration();	  	/*NVIC设置*/
	EXTI_Configuration();			/*设置中断线*/
	while (1)
	{
	switch(Flag)	/*在中断服务函数中置Flag值*/
		 {
			case 0x01:
				LED1(0); 
				Delayms(n);
				LED1(1);
				Delayms(n);
				LED2(1);LED3(1);		  
				break;				
			case 0x02:
				LED2(0);
				Delayms(n);
				LED2(1);
				Delayms(n);
				LED2(1);LED3(1);		  
				break;				
			case 0x03:
				LED1(1);LED2(1);LED3(1);LED4(1);		  
				break;				
			default   :		   
				LED1(1);LED2(1);LED3(1);LED4(1);		  
				break;
	      }
		  }

}
										  

