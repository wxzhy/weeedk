/********************************************************
*  ����NUAA_CM3_107  WEEEDKʵ�鿪����
*******************************************************
** GPIOӦ�ã����ؼ̵���������ơ�������LED����ܵĲ���
   Ҫ��
	 ��1���̽�JP21��JP22�̽�����ʹPC4���ӵ�OPEN�����Ƽ̵���UJDQ1��PC15���ӵ�CLOSE�����Ƽ̵���UJDQ2
				 �̽�JP19��JP20���Ӽ̵������Ƶ�Դ
				����LCD����240*320  TFT ILI9341��
	 ��2������ʾ����������У���ʾ����ʾ�����Ϣ
	 ��3������KEY1�����ü̵���1��������պϣ�ͬʱLED1ָʾ�Ƶ���������KEY2�ü̵���2��������պϣ�ͬʱ��LED2����
	      ��KEY3�ü̵���1����Ͽ���LED1ָʾ���𣬰�KEY4�ü̵���2����Ͽ���LED2ָʾ����
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
	LCD_Init();					//LCD��ʼ��
	Welcome(); 		//��ʾ������
  JDQ1CLOSE;JDQ2CLOSE;/*---------��ʼ�����̵���������------------*/	
	LED1(1);LED2(1);LED3(1);LED4(1);
	LEDR(0);LEDG(0);
	while(1)
	{		
	 if (KEY1==0)	 //KEY1
	 {
	 JDQ1OPEN;//KEY1�̵���1������Ч
	Gui_StrCenter(0,160,WHITE,BLACK,"�̵���1����պ�",16,0);//������ʾ
	LED1(0);
	  }
	 else {
	 if (KEY2==0)	  //KEY2
	 {
		JDQ2OPEN; //KEY2�̵���2������Ч
	Gui_StrCenter(0,190,WHITE,BLACK,"�̵���2����պ�",16,0);//������ʾ
	LED2(0);
	  }
	 else{
	 if (KEY3==0)	  //KEY3
	 {
	 JDQ1CLOSE; //KEY3�̵���1������Ч
	Gui_StrCenter(0,160,WHITE,BLACK,"�̵���1����Ͽ�",16,0);//������ʾ	LEDR(0);LEDG(1);
	LED1(1);
	  }
	 else{
	 if (KEY4==0)	  //KEY4
	 {
	  JDQ2CLOSE; //KEY4�̵���2������Ч
	Gui_StrCenter(0,190,WHITE,BLACK,"�̵���2����Ͽ�",16,0);//������ʾ	LEDR(0);LEDG(1);
	LED2(1);
	  }}}}
   }
}



/******************* *****END OF FILE****/
