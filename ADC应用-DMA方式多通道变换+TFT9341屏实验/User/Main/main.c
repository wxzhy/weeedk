/********************************************************
ADCӦ�ã�DMA��ʽ��ͨ��A/D�任ʵ��
1.��ʵ��Ӳ��ƽ̨��NUAA_CM3_107ʵ�鿪����WEEEDK��MCU��STM32F107VCT6               
2.������ʵ�黷����MDK-ARM Keil V5.15�������ϰ汾
3.����ʹ�õ�STM32F10x�̼���V3.5.0
4.��ʵ����������˵�����ݣ�  
	ADCӦ��,����DMA���䷽ʽ����ͨ��ADC�任������ADC�任ת���Ĵ洢����ʵ��ɲ�����λ����ѹ��PT100���������¶ȡ�ˮλ��������ˮλ���
	�Լ��ڲ��¶ȴ������������¶ȼ���׼��ѹֵ�������ʾ��TFT LCD��Ļ�ϡ�
	��ȱ任��Yx=Y0+(Ym-Y0)*(Nx-N0)/(Nm-N0)
	����PT100������0~125�ȣ�������Χ�޷���⵽��
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"

#define Y1  0.00				/*0��*/

#define Ym1  100.00			/*100��*/
#define N1  2483.0		/*0�ȶ�Ӧ������*/
#define Nm1  3420.0		/*100��Ӧ������*/
#define Y2  0.00				/*0mm*/
#define Ym2  1000.00			/*1000mm*/
#define N2  792.0		/*0�ȶ�Ӧ������*/
#define Nm2  3969.0		/*100��Ӧ������*/

	extern u16 value;    				/*����������������*/			
	float ADC_Value=0.00;       /*ʵ��������ֵ*/                  

int main(void)
{
		SystemInit();
		GPIO_Configuration();
		ADC1_Mode_Config ();		        						/*ADC1ʹ��DMA��ʽ��ͨ�������任��ʼ������*/
		LCD_Init();																	/*  LCD��ʼ��    */
		Welcome(); 										 							/*  ��ʾ������   */
		LED1(1);LED2(1);LED3(1);LED4(1);
	while(1)
	{		
		value=ReadADCAverageValue(0);								/*��ȡ��λ��ADC_IN3,NO.1���Ϊ0����Ӧ����ֵ*/
		ADC_Value = (3.3*1000/4096)*value;			    /*��ADֵת��Ϊ��ѹֵmV*/
		LCD_ShowNum(150,140,ADC_Value,5,16);				/*��LCD������ʾ��ѹֵmV*/
		
		value=ReadADCAverageValue(1);								/*��ȡPT100�����Ӧ�¶�ֵ��ADC_IN6,NO.2���Ϊ1*/
		if(value>=0xFF0) 			Show_Str(150,165,YELLOW,RED," δ��",16,0);
		else{
		ADC_Value=(float)(Y1+(Ym1-Y1)*(value-N1)/(Nm1-N1));	  /*ͨ�����Ա�ȱ任�õ��¶�0~100��*/
			if(ADC_Value<0)												  			/*�������¶�*/
			{
			Show_Str(150-8,165,YELLOW,RED,"-",16,0);
			}					 
			else 
			Show_Str(150-8,165,YELLOW,RED," ",16,0);
			LCD_ShowNum(150, 165, (u8)ADC_Value, 3,16);    /*��ʾ�¶���������	*/
			Show_Str(150+8*3,165,YELLOW,RED,".",16,0);
			ADC_Value-=(u8)ADC_Value;	  								/*��ȥ�������֣�ʣ��С������*/	
			LCD_ShowNum(150+8*4,165,ADC_Value*10,1,16);   /*��ʾ�¶�С������*/	
				}

		value=ReadADCAverageValue(2);								/*��ȡˮλ��������ƽ��ֵ��ADC_IN7 No.3 ���Ϊ2 */			
		ADC_Value=(float)(Y2+(Ym2-Y2)*(value-N2)/(Nm2-N2));	  /*ͨ�����Ա�ȱ任�õ�ˮλ*/
		if (value<0x300) Show_Str(150,190,YELLOW,RED," δ��",16,0);
		else	{if(ADC_Value<=0) ADC_Value=0;;LCD_ShowNum(150,190, ADC_Value,5,16);}						/*��LCD������ʾ����ֵ*/

			value=ReadADCAverageValue(3);								/*��ȡ�ڲ��¶�ƽ��ֵ��ADC_IN16,NO.4���Ϊ3		*/	
			ADC_Value=(1.43-(3.3/4096)*value)/0.0043+25;   /*�������ǰ�¶�ֵ,�μ���Ƕ��ʽϵͳԭ��Ӧ�ý̲ģ������棩����7��7.4.1�������*/	 
			if(ADC_Value<0)												  			/*�������¶�*/
			{
			Show_Str(150,215,WHITE,BLACK,"-",16,0);
			}					 
			else 
			Show_Str(150,215,WHITE,BLACK," ",16,0);
 		
			LCD_ShowNum(150, 215, (u8)ADC_Value, 2,16);    /*��ʾ�¶���������	*/
			Show_Str(150+8*2,215,WHITE,RED,".",16,0);
			ADC_Value-=(u8)ADC_Value;	  
			LCD_ShowNum(150+8*3,215,ADC_Value*100,2,16);   /*��ʾ�¶�С������*/	
		
		value=ReadADCAverageValue(4);								/*��ȡ�ڲ���׼��ѹ����ƽ��ֵ��ADC_IN17,NO.5���Ϊ4 */	
		ADC_Value = (3.3*1000/4096)*value;			    /*��ADֵת��Ϊ��ѹֵmV*/
		LCD_ShowNum(150,240, value,5,16);						/*��LCD������ʾ����ֵ*/

			Delay_ms(500);
		GPIOD->ODR^=(1<<2);													/*��LED1��˸��ʾ*/
	}
}


/***********************END OF FILE****/
