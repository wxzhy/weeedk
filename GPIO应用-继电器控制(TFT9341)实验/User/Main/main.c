/********************************************************
*  采用NUAA_CM3_107  WEEEDK实验开发板
*******************************************************
** GPIO应用：板载继电器输出控制、按键、LED发光管的操作
   要求：
	 （1）短接JP21和JP22短接器，使PC4连接到OPEN，控制继电器UJDQ1，PC15连接到CLOSE，控制继电器UJDQ2
				 短接JP19和JP20连接继电器控制电源
				连接LCD屏（240*320  TFT ILI9341）
	 （2）下载示例程序后运行，显示屏显示相关信息
	 （3）按下KEY1键，让继电器1常开触点闭合，同时LED1指示灯点亮，按下KEY2让继电器2常开触点闭合，同时让LED2点亮
	      按KEY3让继电器1触点断开，LED1指示灯灭，按KEY4让继电器2触点断开，LED2指示灯灭。
***********************************************************************************************************/  


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

#define JDQ1OPEN 	GPIO_WriteBit(GPIOC, GPIO_Pin_4,Bit_RESET)
#define JDQ2OPEN 	GPIO_WriteBit(GPIOC, GPIO_Pin_15,Bit_RESET)
#define JDQ1CLOSE GPIO_WriteBit(GPIOC, GPIO_Pin_4,Bit_SET)
#define JDQ2CLOSE GPIO_WriteBit(GPIOC, GPIO_Pin_15,Bit_SET)

extern u8 gImage_fg[];
u8 key;


char res;

int main(void)
{
SystemInit();
	GPIO_Configuration();
	LCD_Init();					//LCD初始化
	Welcome(); 		//显示主界面
  JDQ1CLOSE;JDQ2CLOSE;/*---------初始化，继电器不动作------------*/	
	LED1(1);LED2(1);LED3(1);LED4(1);
	LEDR(0);LEDG(0);
	while(1)
	{		
	 if (KEY1==0)	 //KEY1
	 {
	 JDQ1OPEN;//KEY1继电器1控制有效
	Gui_StrCenter(0,160,WHITE,BLACK,"继电器1触点闭合",16,0);//居中显示
	LED1(0);
	  }
	 else {
	 if (KEY2==0)	  //KEY2
	 {
		JDQ2OPEN; //KEY2继电器2控制有效
	Gui_StrCenter(0,190,WHITE,BLACK,"继电器2触点闭合",16,0);//居中显示
	LED2(0);
	  }
	 else{
	 if (KEY3==0)	  //KEY3
	 {
	 JDQ1CLOSE; //KEY3继电器1控制无效
	Gui_StrCenter(0,160,WHITE,BLACK,"继电器1触点断开",16,0);//居中显示	LEDR(0);LEDG(1);
	LED1(1);
	  }
	 else{
	 if (KEY4==0)	  //KEY4
	 {
	  JDQ2CLOSE; //KEY4继电器2控制无效
	Gui_StrCenter(0,190,WHITE,BLACK,"继电器2触点断开",16,0);//居中显示	LEDR(0);LEDG(1);
	LED2(1);
	  }}}}
   }
}



/******************* *****END OF FILE****/
