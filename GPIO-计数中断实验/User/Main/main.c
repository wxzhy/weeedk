
/************************************************************************************
**	  WEEEDK NUAA_CM3_107实验开发板（V4.1）
**	  外部中断实验
**		功能：接一次KEY1键，LED1闪烁，按KEY2键，LED2闪烁，同时按KEY3全灭。
**    功能及现象：常态无键按下，LED1、LED2全常灭，按下KEY1，LED1闪烁，按下KEY2，LED2闪烁,按下KEY3，停止闪烁
**                单个按键用中断
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
*函数名称：void Delay(void)
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

int main(void)
{
u8 i=0;
	SystemInit();     	    	/*系统初始化（此句可省，因为启动文件中有调用这段程序）*/
	GPIO_Configuration();			/*GPIO初始化：LED初始化,按键端口配置*/
	NVIC_Configuration();	  	/*NVIC设置*/
	EXTI_Configuration();			/*设置中断线*/
	Times=0;
	while (1)
	{
		if (Flag)
		{
			for (i=0;i<Times;i++)
			{	LED4(0); 
				Delayms(300);
				LED4(1);
				Delayms(300);
			}
			Times=0;Flag=0;
		}	
	}
}
										  

int fputc(int ch, FILE *f)
{
  
  USART_SendData(USARTx, (uint8_t) ch);   				      //发送一个字符

  while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)  //等待发送完成
  {}

  return ch;
}


