/********************************************************
1.��ʵ��Ӳ��ƽ̨��NUAA_CM3_107ʵ�鿪����WEEEDK��MCU��STM32F107VCT6
               
2.������ʵ�黷����MDK-ARM Keil V5.15�������ϰ汾
3.����ʹ�õ�STM32F10x�̼���V3.5.0
4.��ʵ����������˵�����ݣ�  
	����WEEEDK NUAA_CM3_107ʵ�鿪����
  STLINK�������к�PB8����PWM���
			  	���г����PWM���Ƶ��10kHz����ʼռ�ձ�50%
					
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"


extern u8  	TIM2CH1_CAPTURE_STA;		/*���벶��״̬,��6λ��0~5λ����ʾ�͵�ƽ��Чʱ��¼�����жϵĴ�����ÿ�θ����жϼ���65536�Σ�65536us for 1MHz��
																					��6λΪ�½��ر�־��1=���½��ز���0��Ч
																					��7λΪ�����ر�־��1=�������ز���0��Ч							                */
extern u16	TIM2CH1_CAPTURE_VAL;		/*���벶��ֵ    */


int main(void)
 {	
	 
	uint16_t Pulse=7200*0.5;				//ռ�ձ�Ϊ50%  ����7200��1ms��
	SystemInit();
	PWM_GPIO_Init();		    		//PWM�����PB8����ʼ��
	Init_TIMER();								//��ʱ��4��ʼ��
	Init_PWM(Pulse);			  //PWM��ͨ��3��ʼ������	
	GPIO_Configuration();
	TIM2_Cap_Init(0xffff,72-1);					/*��1MHz��Ƶ�ʼ�������ʼ��TIM2��ʱ��  */
	LCD_Init();													/*  LCD��ʼ��        */
	Welcome(); 										 			/*  ��ʾ������       */
	LED1(1);LED2(1);LED3(1);LED4(1);
	 name_display();
	while(1)
	{		
 		if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11)==0)   //KEY1����
		{
			Pulse+=100;									   //ռ�ձȼ�100
			if(Pulse>=7200-1) Pulse=1;
			TIM_SetCompare3(TIM4,Pulse);                   //д�붨ʱ��4��ͨ��3��ռ�ձ�ֵ
		}
	
		
		if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12)==0)   //KEY2����
		{
			Pulse-=100;									   //ռ�ձȼ�100
			if((Pulse<1)||(Pulse>7200))   Pulse=7200-1;
			TIM_SetCompare3(TIM4,Pulse);                   //д�붨ʱ��4��ͨ��3��ռ�ձ�ֵ
		}
			Delay_ms(100);
			LCD_ShowNum(80,190,Pulse,8,16);		/*��ʾ�ܵ��������*/
			LCD_ShowNum(80,230,Pulse*100/7200,8,16);	/*��ʾ�ܵ��������*/

}
}


/***********************END OF FILE****/
