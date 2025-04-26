/********************************************************
*  NUAA_CM3_107  WEEEDKʵ�鿪�����ۺϿ��˳�����
   
**************************************************************************************************/	



/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "remote.h"

#include "lcd.h"
#include "WEEEDK.h"
#include "delay.h"
#include "DS18B20.h"
#include <stdio.h>

extern u8 gImage_fg[];
u8 key;
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


u8 BeepFlag=0;
u8 res;
u16 value;   								/*����������������*/			
float ADC_Value=0.00;       /*ʵ��������ֵ*/                  
u8 TFlag=0;
u16 Pulse=500;

PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART1, (uint8_t) ch);   
	
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}
	
	return ch;
}

int main(void)
{
	u8 led_count = 0;
	u8 name_blink = 0;
	
	Delay_Init();               /*ϵͳ�δ�ʱ��SysTick��ʼ��*/
	TIM_Configuration(500);			/*��ʱ����ʼ��*/
	PWM_GPIO_Init();		    		/*PWM�����PB6����ʼ��*/
	Init_PWM(0);			  				/*PWM��ͨ��1��ʼ������	��ռ�ձȳ�ʼΪ0���õ����ת*/
	GPIO_Configuration();
	LCD_Init();									/*LCD��ʼ��*/
	ADC_Configuration();				/*ADC��ʼ��*/
	DAC_Configuration();	   		/*DAC1��ʼ��*/  
	USART_Configuration();			/*USART��ʼ��*/
	DisplayInf();    						/*��LCD������ʾ��ʼ��Ϣ������ѧ�ź�����*/
	NVIC_Configuration();				/*Ƕ�������жϿ�����NVIC��ʼ��*/
	
	while(1)
	{	
		if (TFlag)										/*��ʱ����־*/
		{
			TFlag=0;
			value=ReadADCAverageValue();	/*ȡA/Dת�������ƽ��ֵ*/
			ADC_Value=value*3300/4095;		/*ͨ����ȱ任ΪmV*/
			DAC_SetChannel1Data(DAC_Align_12b_R,value);/*�����任��DAC���ݷ��Ҷ���Ĵ�����*/
			LCD_ShowNum(130,240,ADC_Value,4,16);

			// ���ݲ���ֵ����PWMռ�ձ�
			Pulse = value * 1000 / 4095; // ����ռ�ձȶ�Ӧ��������
			TIM_SetCompare1(TIM4, Pulse); // ����ͨ��1��ռ�ձ�
			LCD_ShowNum(130,270,Pulse/10,4,16);
			// �������ѧ����Ҫ��˸
			if(name_blink)
			{
				if(led_count % 2 == 0)
				{
					Show_Str(45,40,RED,YELLOW,"ѧ��:162210426",16,0);
					Show_Str(45,60,RED,YELLOW,"����:����",16,0); // ��ʾ����ѧ��
				}
				else
				{
					Show_Str(45,40,RED,YELLOW,"ѧ��:         ",16,0);
					Show_Str(45,60,RED,YELLOW,"����:      ",16,0); // ����
				}
				
				// �����Ч��
				switch(led_count % 4)
				{
					case 0: LED1(0); LED2(1); LED3(1); LED4(1); break;
					case 1: LED1(1); LED2(0); LED3(1); LED4(1); break;
					case 2: LED1(1); LED2(1); LED3(0); LED4(1); break;
					case 3: LED1(1); LED2(1); LED3(1); LED4(0); break;
				}
				
				led_count++;
			}
		}	
		
		// KEY1: ���µ���LED1��̧��LED1��
		if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11) == 0) // ����KEY1
		{
			LED1(0); // LED1��
		}
		else
		{
			if(!name_blink) // �������˸��LED1��
				LED1(1); // LED1��
		}
		
		// KEY2: ���·�������һ��
		if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12) == 0) // ����KEY2
		{
			BEEP(1); // ��������
			Delay_ms(100);
			BEEP(0);
			Delay_ms(200); // ��ֹ��������
		}
		
		// KEY3: ����ѧ����˸��LED�����
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0) // ����KEY3
		{
			name_blink = 1; // ������˸
			Delay_ms(200); // ��ֹ��������
		}
		
		// KEY4: �ָ���ʾ
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0) // ����KEY4
		{
			name_blink = 0; // ֹͣ��˸
			led_count = 0;
			LED1(1); LED2(1); LED3(1); LED4(1); // ����LED��
			Show_Str(45,40,RED,YELLOW,"ѧ��:162210426",16,0);
			Show_Str(45,60,RED,YELLOW,"����:����",16,0); // ������ʾ����ѧ��
			Delay_ms(200); // ��ֹ��������
		}
	}
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
