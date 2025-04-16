/********************************************************
*  采用WEEEDK NUAA_CM3_107实验开发板
*  UART串行通信实验例程
   本示例测试UART1、UART2两个串口的通信情况，查询发送，中断接口，硬件连接如下
   UART1：PA9（TXD1），PA10（RXD1），需要短接JP23和JP24连接到U1TXD和U1RXD
   UART2：PD5（TXD2），PD6（RXD2） ，需要短接JP25和JP26连接到U2TXD和U2RXD
   波特率可设置修改U1Baud和U2Baud，默认115200bps
   实验步骤：
（1）需要短接JP23和JP24连接到U1TXD和U1RXD，短接JP25和JP26连接到U2TXD和U2RXD
（2）插上3.2吋TFT真彩屏到JLCD1（可选），打开电源，两个电源指示灯指示灯亮
（2）下载示例程序后运行，显示屏显示初始信息
（3）打开串口助手，设置好对应的串口和波特率。在发送端输入数据看接收端显示情况
     输入数字1字符（不是HEX）发送，点亮LED1指示灯，接收端显示Turn on LED1
		 输入数字2字符发送，同时点亮LED2指示灯，接收端显示Turn on LED2
		 输入数字3字符发送，同时点亮LED3指示灯，接收端显示Turn on LED3
		 输入数字5字符发送，同时点亮LED1~LED3指示全灯，接收端显示Turn on all LED
		 输入数字0或6字符发送，LED1~LED3指示全灭，接收端显示Turn off all LED
	编译下载程序到开发板并在线仿真运行，或下载后脱机复位运行

**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"
#include "delay.h"
#include "DS18B20.h"

#define U1Baud   115200

u8 key;


#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

/* Private functions ---------------------------------------------------------*/
/**									 _
	*名称：void Delay(void)
	*参数：无
	*返回：无
	*功能：简单延时
**/
void Delay(void)
{
	int i,j;
	for(i=1000;i>0;i--)
		for(j=1000;j>0;j--);

}

uint8_t ReceiveFlag=0;
extern char res;
int main(void)
{
uint8_t KEY=0;
	SystemInit();
	GPIO_Configuration();
	Delay_Init();               //使用SysTick实现精确延时
	LCD_Init();					//LCD初始化
	USART_Configuration(U1Baud);

   USART1_SendString("\r\n UART1通信实验");

	NVIC_Configuration();		
	Welcome(); 		//显示主界面
	LED1(1);LED2(1);LED3(1);LED4(1);
  printf("\r\n 请输入1~3的字符控制LED的亮灭 \r\n");
	while(1)
	{		
	 if (ReceiveFlag)	   //串口有有效命令响一声蜂鸣器
	{
		switch(res)
		{
			case '1':USART1_SendString("\r\n Turn on LED1.");
				LED1(0);LED2(1);LED3(1);
				Gui_StrCenter(0,230,WHITE,BLACK,"   Turn on LED1!    ",16,0);//居中显示
				ReceiveFlag=1;
				break;
			case '2':USART1_SendString("\r\n Turn on LED2.");
				LED1(1);LED2(0);LED3(1);
				Gui_StrCenter(0,230,WHITE,BLACK,"   Turn on LED2!    ",16,0);//居中显示
				ReceiveFlag=1;
				break;
			case '3':USART1_SendString("\r\n Turn on LED3.");
				LED1(1);LED2(1);LED3(0);
				Gui_StrCenter(0,230,WHITE,BLACK,"   Turn on LED3!    ",16,0);//居中显示
				ReceiveFlag=1;
				break;
			case '5':USART1_SendString("\r\n Turn on all LED.");
				LED1(0);LED2(0);LED3(0);
				Gui_StrCenter(0,230,WHITE,BLACK,"   Turn on LED1~LED3!    ",16,0);//居中显示
				ReceiveFlag=1;
				break;
			case '0':
			case '6':USART1_SendString("\r\n Turn off all LED. ");
				ReceiveFlag=1;
				LED1(1);LED2(1);LED3(1);
				Gui_StrCenter(0,230,WHITE,BLACK,"Turn off LED1~LED3!",16,0);//居中显示
				break;
	default:
				ReceiveFlag=1;
				LED1(1);LED2(1);LED3(1);
				Show_Str(50,230,WHITE,BLACK,"非法命令:         ",16,0);//居中显示
				LCD_ShowChar(0,230,WHITE,BLACK,res,16,0);//居中显示
				break;		
		}
   	Delay_ms(300);
		BEEP(1);
		Delay_ms(300);
		BEEP(0);
		ReceiveFlag=0;
	}			 
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)==0) KEY=1;
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)==0) KEY=2;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==0) KEY=3;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0) KEY=4;
	switch(KEY)
		{
		case 1:/*KEY1键，发送i*/
			USART1_SendString("\r\n UART1通信实验:你按下了KEY1键！");
			Show_Str(50,230,WHITE,BLACK,"你按下了KEY1键！",16,0);//居中显示
		
			KEY=0;
			break;
		case 2:/*KEY2键，发送字符串*/
			USART1_SendString("\r\n UART1通信实验:你按下了KEY2键！");
			Show_Str(50,230,WHITE,BLACK,"你按下了KEY2键！",16,0);//居中显示
			KEY=0;
			break;
		case 3:/*KEY3键，发送字符串*/
			USART1_SendString("\r\n UART1通信实验:你按下了KEY3键！");
			Show_Str(50,230,WHITE,BLACK,"你按下了KEY3键！",16,0);//居中显示
			KEY=0;
			break;
		case 4:/*KEY3键，发送字符串*/
			USART1_SendString("\r\n UART1通信实验:你按下了KEY4键！");
			Show_Str(50,230,WHITE,BLACK,"你按下了KEY4键！",16,0);//居中显示
			KEY=0;
			break;
		}
    
	PDout(7)=~PDout(7);	   //LED4(PD7)闪显示
   Delay();
   }
}


/************************************
*  将printf应用于USART2发送
***************************************/
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);   
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return ch;
}



/***********************END OF FILE****/
