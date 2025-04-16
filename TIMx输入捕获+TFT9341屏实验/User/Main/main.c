/********************************************************
1.��ʵ��Ӳ��ƽ̨��NUAA_CM3_107ʵ�鿪����WEEEDK��MCU��STM32F107VCT6
               
2.������ʵ�黷����MDK-ARM Keil V5.15�������ϰ汾
3.����ʹ�õ�STM32F10x�̼���V3.5.0
4.��ʵ����������˵�����ݣ�  
	����WEEEDK NUAA_CM3_107ʵ�鿪����
	��ʱ��2(TIM2),TIM2��ͨ��1(TIM2_CH1,������PA0����),ʹ�ö�ʱ�������벶������
	��׽PA0����ĵ͵�ƽ����. 	
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"
#include "delay.h"

extern u8  	TIM2CH1_CAPTURE_STA;		/*���벶��״̬,��6λ��0~5λ����ʾ�͵�ƽ��Чʱ��¼�����жϵĴ�����ÿ�θ����жϼ���65536�Σ�65536us for 1MHz��
																					��6λΪ�½��ر�־��1=���½��ز���0��Ч
																					��7λΪ�����ر�־��1=�������ز���0��Ч							                */
extern u16	TIM2CH1_CAPTURE_VAL;		/*���벶��ֵ    */


int main(void)
 {	
	 
	u32 temp=0; 
	SystemInit();
	GPIO_Configuration();
	TIM2_Cap_Init(0xffff,72-1);					/*��1MHz��Ƶ�ʼ�������ʼ��TIM2��ʱ��  */
	LCD_Init();													/*  LCD��ʼ��        */
	Welcome(); 										 			/*  ��ʾ������       */
	LED1(1);LED2(1);LED3(1);LED4(1);
	while(1)
	{		
 		Delay_ms(100);
		if(TIM2CH1_CAPTURE_STA&(1<<7))		/*�ɹ�������һ�ε͵�ƽ*/
		{
			temp=TIM2CH1_CAPTURE_VAL+65536*(TIM2CH1_CAPTURE_STA&0x3F)+1;/*�õ��ܵĵ͵�ƽʱ��*/
			TIM2CH1_CAPTURE_STA=0;					/*������һ�β���*/
			LCD_ShowNum(80,160,temp,8,16);	/*��ʾ�ܵĵ͵�ƽʱ��*/
	}
}
}


/***********************END OF FILE****/
