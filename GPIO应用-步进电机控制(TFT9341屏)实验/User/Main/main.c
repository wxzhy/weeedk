/********************************************************
*  采用NUAA_CM3_107  WEEEDK实验开发板
*******************************************************
ULN2003驱动5V减速步进电机程序,步进机电型号28BYJ-48
Target:STM32F107VCT6
Crystal:25MHz
实验步骤：
  1、短接JP42连接器，使步进电机电源接通，将步进电机连接器连接到J32插座上
	2、下载程序，并运行
	3、按下KEY1键，步进电机按照顺时钟正转，显示屏显示正在正转，双色指示灯绿灯亮，
	   按KEY2键，按逆时针反转，显示屏显示正在反转，双色指示灯红灯亮，
	   按KEY3停止，指示灯灭 
********************************************************  
通过汉字取模软件可以自行选择关注的汉字，将汉字编码，按照示例放入font.h相应位置，可以显示16*16，24*24，32*32和64*64的汉字         
**步进电机接法
接线方式:
	IN1 ---- PD2
	IN2 ---- PD3
	IN3 ---- PD4
	IN4 ---- PD7
	+   ---- +5V
	-   ---- GND
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"
#include "delay.h"
#include "DS18B20.h"
#include <stdio.h>

#define KEY1 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11)
#define KEY2 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12)
#define KEY3 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)
#define KEY4 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)


extern u8 gImage_fg[];
u8 key;


char res;

int main(void)
{
uint8_t KEY=0;
	SystemInit();
	GPIO_Configuration();
	LCD_Init();					//LCD初始化
	Welcome(); 		//显示主界面
	LED1(0);LED2(0);LED3(0);LED4(0);
	LEDR(1);LEDG(1);


	while(1)
	{		
	 if (KEY1==0)	 
	 {
	  KEY=1; //KEY1正转
	Gui_StrCenter(0,160,WHITE,BLACK,"步进电机正在正转",16,0);//居中显示
	LEDR(0);LEDG(1);
	  }
	 else{
	 if (KEY2==0)	  //KEY2反转
	 {
	  KEY=2; //KEY2反转
	Gui_StrCenter(0,160,WHITE,BLACK,"步进电机正在反转",16,0);//居中显示
	LEDR(1);LEDG(0);
	  }
	 else{
	 if (KEY3==0)	  //KEY3停止
	 {
	  KEY=3; //KEY3正转
	Gui_StrCenter(0,160,WHITE,BLACK,"步进电机已经停止",16,0);//居中显示	LEDR(0);LEDG(1);
	LEDR(1);LEDG(1);
	  }}}
	 switch(KEY)
	 {
	 case 1:MotorCW();break;
	 case 2:MotorCCW();break;
	 case 3:MotorStop();break;
	 }
   }
}


/******************* *****END OF FILE****/
