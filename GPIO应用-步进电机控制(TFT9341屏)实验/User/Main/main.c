/********************************************************
*  ����NUAA_CM3_107  WEEEDKʵ�鿪����
*******************************************************
ULN2003����5V���ٲ����������,���������ͺ�28BYJ-48
Target:STM32F107VCT6
Crystal:25MHz
ʵ�鲽�裺
  1���̽�JP42��������ʹ���������Դ��ͨ��������������������ӵ�J32������
	2�����س��򣬲�����
	3������KEY1���������������˳ʱ����ת����ʾ����ʾ������ת��˫ɫָʾ���̵�����
	   ��KEY2��������ʱ�뷴ת����ʾ����ʾ���ڷ�ת��˫ɫָʾ�ƺ������
	   ��KEY3ֹͣ��ָʾ���� 
********************************************************  
ͨ������ȡģ�����������ѡ���ע�ĺ��֣������ֱ��룬����ʾ������font.h��Ӧλ�ã�������ʾ16*16��24*24��32*32��64*64�ĺ���         
**��������ӷ�
���߷�ʽ:
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
	LCD_Init();					//LCD��ʼ��
	Welcome(); 		//��ʾ������
	LED1(0);LED2(0);LED3(0);LED4(0);
	LEDR(1);LEDG(1);


	while(1)
	{		
	 if (KEY1==0)	 
	 {
	  KEY=1; //KEY1��ת
	Gui_StrCenter(0,160,WHITE,BLACK,"�������������ת",16,0);//������ʾ
	LEDR(0);LEDG(1);
	  }
	 else{
	 if (KEY2==0)	  //KEY2��ת
	 {
	  KEY=2; //KEY2��ת
	Gui_StrCenter(0,160,WHITE,BLACK,"����������ڷ�ת",16,0);//������ʾ
	LEDR(1);LEDG(0);
	  }
	 else{
	 if (KEY3==0)	  //KEY3ֹͣ
	 {
	  KEY=3; //KEY3��ת
	Gui_StrCenter(0,160,WHITE,BLACK,"��������Ѿ�ֹͣ",16,0);//������ʾ	LEDR(0);LEDG(1);
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
