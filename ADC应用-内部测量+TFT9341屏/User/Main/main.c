/********************************************************
*  ����NUAA_CM3_107  WEEEDKʵ�鿪����
*  �¶Ȳ����������̣�����оƬ�ڲ��¶ȴ������������¶ȴ��������������ʾ��TFT����
����3.2��TFT�������JP25�����ϣ���ѡ�����Ѳ��ϣ����̽�JP3��VLCD���ӵ�3.3V���򿪵�Դ��LED0��LED6����Դָʾ����
��1���̽�JP4��V1���ӵ�3.3V����ͨDS18B20��Դ
��2���̽�JP15��ͨDS18B20��DQ��PB10
��3���̽�JP18��JP40��ͨ˫ɫ�����
��4������ʾ����������У���ʾ����ʾ��ʼ��Ϣ���¶�ֵ��Ĭ����ʾ�ڲ��¶�
��5������KEY1����Ϊ�ڲ��������������¶ȣ�˫ɫ���̵���������KEY2Ϊ�ⲿ������DS18B20�������¶ȣ�˫ɫ�ĺ����
		 �������ⲿ�¶�ʱ�����������¶ȴ�����DS18B20�������·����ȵ����Ͻǣ�
 ע�⣺�����غ������У��ɰ���λ����ʼ
http://www.ndiy.cn/thread-24274-1-1.html����DS18B20���ϵ�˵��
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"
#include "delay.h"
#include <stdio.h>

extern u8 gImage_fg[];
extern u16 BACK_COLOR;
extern u16 POINT_COLOR;
u8 key;


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private functions ---------------------------------------------------------*/


	float temperate;
uint8_t BeepFlag=0;
char res;
//extern float temperate;
uint16_t temp=0;
extern  u8 ADC_Channel,Rank;



/**									 _
	*���ƣ�void Delay(void)
	*��������
	*���أ���
	*���ܣ�����ʱ
**/

void Delay(void)
{
	int i,j;
	for(i=1000;i>0;i--)
		for(j=1000;j>0;j--);

}



int main(void)
{
  SystemInit();
	GPIO_Configuration();
	ADC_Configuration();	      /*ADC��ʼ��*/
	Delay_Init();               /*ʹ��SysTickʵ�־�ȷ��ʱ*/
	LCD_Init();									/*LCD��ʼ��*/
	Welcome(); 									/*��ʾ������*/
	LED1(1);LED2(1);LED3(1);LED4(1);LEDR(0);LEDG(0);
	LEDR(1);LEDG(0);
	 	while(1)
	{		
			LEDR(0);LEDG(1);

		Read_ADC1_MultiChannel(ADC_Channel_TempSensor,1);/*ADC_GetConversionValue(ADC1);*/
			temp = Read_ADC1_MultiChannel(ADC_Channel_TempSensor,1);/*ADC_GetConversionValue(ADC1);*/
			LCD_ShowNum(100, 150, temp, 5,16);    				/*��ʾA/D�任��������	*/
			temperate=(1.43-(3.3/4096)*temp)/0.0043+25;   /*�������ǰ�¶�ֵ,�μ���Ƕ��ʽϵͳԭ��Ӧ�ý̲ģ������棩����7��7.4.1�������*/	 
			if(temperate<0)												  			/*�������¶�*/
			{
			Show_Str(90,190,WHITE,BLACK,"-",16,0);
			}					 
			else 
			Show_Str(90,190,WHITE,BLACK," ",16,0);
 		
			LCD_ShowNum(100, 190, (u8)temperate, 2,16);    /*��ʾ�¶���������	*/
			Show_Str(100+8*2,190,WHITE,RED,".",16,0);
			temperate-=(u8)temperate;	  									 /*��ȥ�������֣�ʣ��С������*/	
			LCD_ShowNum(100+8*3,190,temperate*100,2,16);   /*��ʾ�¶�С������*/	
			Show_Str(105+8*5,190,WHITE,BLACK,"��",16,0);

			Read_ADC1_MultiChannel(ADC_Channel_Vrefint,1);/*ADC_GetConversionValue(ADC1);*/
			temp = Read_ADC1_MultiChannel(ADC_Channel_Vrefint,1);/*ADC_GetConversionValue(ADC1);*/
			LCD_ShowNum(100, 230, temp, 5,16);    				/*��ʾA/D�任��������	*/
			LCD_ShowNum(100, 270, temp*3.3*1000/4096, 5,16);    				/*��ʾA/D�任��������	*/
			Show_Str(105+8*5,270,WHITE,BLACK,"mV",16,0);
			Delay_ms(100);

	}

}

/**********************END OF FILE****/
