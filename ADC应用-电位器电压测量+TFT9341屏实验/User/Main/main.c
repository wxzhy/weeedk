/********************************************************
ADCӦ�ã����ص�λ����ѹ����
1.��ʵ��Ӳ��ƽ̨��NUAA_CM3_107ʵ�鿪����WEEEDK��MCU��STM32F107VCT6               
2.������ʵ�黷����MDK-ARM Keil V5.15�������ϰ汾
3.����ʹ�õ�STM32F10x�̼���V3.5.0
4.��ʵ����������˵�����ݣ�  
	ADCӦ��,ģ��ͨ��3��Ϊ���ص�λ����ѹ������ţ�PA3�����ɼ���ͨ��3�ı任������任Ϊ��ѹֵ��mA����ʾ��LCD���ϡ�
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"


	extern u16 value;    
	float ADC_Value=0.00;                         /*�ڲ���ѹֵ*/
	float Pressure=0.00;                         /*�ڲ�ѹ��ֵ*/

int main(void)
{
		SystemInit();
		GPIO_Configuration();
		ADC_Configuration ();		        						/*ADC1 ��ʼ������*/
		LCD_Init();																	/*  LCD��ʼ��    */
		Welcome(); 										 							/*  ��ʾ������   */
	  name_display();
		LED1(1);LED2(1);LED3(1);LED4(1);
	while(1)
	{		
//		value=Read_ADC1_MultiChannel(ADC_Channel_3);     //��ȡ����ֵ,û���˲������ */	
		value=filter_YDPJ();     //��ȡ����ֵ,���˲������ */	
		ADC_Value = (3.3*1000/4096)*value;			    /*��ADֵת��Ϊ��ѹֵmV*/
		LCD_ShowNum(100,150, value,5,16);						/*��LCD������ʾ����ֵ*/
//		LCD_ShowNum(100,190,ADC_Value,5,16);				/*��LCD������ʾ��ѹֵmV*/
		Pressure=10+(1000-10)*(value-100)/(4095-100);
		LCD_ShowNum(100,190,Pressure,5,16);				/*��LCD������ʾѹ��ֵkpa*/
		Delay_ms(300);
		GPIOD->ODR^=(1<<2);													/*��LED1��˸��ʾ*/
	}
}


/***********************END OF FILE****/
