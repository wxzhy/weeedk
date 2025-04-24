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
u8 flashFlag=0; // �����˸��־
u16 Pulse=0;    // ���PWM�������
int main(void)
{
	Delay_Init();               /*ϵͳ�δ�ʱ��SysTick��ʼ��*/
  TIM_Configuration(5000);		/*��ʱ����ʼ������ʱ5000ms�����жϣ��Ѹ�ΪTIM3*/
	GPIO_Configuration();
	LCD_Init();									/*LCD��ʼ��*/
	ADC_Configuration();				/*ADC��ʼ��*/
	DAC_Configuration();	   		/*DAC1��ʼ��*/  
	USART_Configuration();			/*USART��ʼ��*/
	DisplayInf();    						/*��LCD������ʾ��ʼ��Ϣ������ѧ�ź�����*/
	NVIC_Configuration();				/*Ƕ�������жϿ�����NVIC��ʼ��*/
while(1)
	{	
		value=ReadADCAverageValue();	/*ȡA/Dת�������ƽ��ֵ*/
		ADC_Value=value*3300/4095;		/*ͨ����ȱ任ΪmV*/
		DAC_SetChannel1Data(DAC_Align_12b_R,value);/*�����任��DAC���ݷ��Ҷ���Ĵ�����*/
		
		// ���ݵ�λ������ֵ���ı�ռ�ձ�
		Pulse=value*1000/4095;
		TIM_SetCompare3(TIM4,Pulse);
		
		// ���TFlag��־����ʾ��λ����ѹֵ
		if (TFlag)									
		{
			TFlag=0;
			LCD_ShowNum(130,240,ADC_Value,4,16);
		}
		
		// ����������
		// KEY1���µ���LED1��̧����
		if(!KEY1()) {
			LED1(0); // ����LED1
		} else {
			LED1(1); // Ϩ��LED1
		}
		
		// KEY2���·�������һ��
		if(!KEY2()) {
			BEEP(1); // ��������
			Delay_ms(100);
			BEEP(0); // ������ͣ
			while(!KEY2()); // �ȴ������ͷ�
		}
		
		// KEY3���£�����ѧ����˸��LED1-LED4�����
		if(!KEY3()) {
			flashFlag = 1; // ������˸��־
			while(!KEY3()); // �ȴ������ͷ�
		}
		
		// KEY4���»ָ�������ʾ
		if(!KEY4()) {
			flashFlag = 0; // �����˸��־
			LED1(1); // Ϩ������LED
			LED2(1);
			LED3(1);
			LED4(1);
			while(!KEY4()); // �ȴ������ͷ�
		}
		
		// �����˸��־���ã�ʵ�������Ч��
		if(flashFlag) {
			static u8 ledIndex = 0;
			static u8 displayFlag = 0;
			
			// �����Ч��
			LED1(1); LED2(1); LED3(1); LED4(1); // ȫ��
			switch(ledIndex) {
				case 0: LED1(0); break; // LED1��
				case 1: LED2(0); break; // LED2��
				case 2: LED3(0); break; // LED3��
				case 3: LED4(0); break; // LED4��
			}
			ledIndex = (ledIndex + 1) % 4;
			
			// ����ѧ����˸
			if(displayFlag) {
				Show_Str(45,40,RED,YELLOW,"ѧ��:162210426",16,0);
				Show_Str(45,60,RED,YELLOW,"����:����",16,0);
			} else {
				Show_Str(45,40,RED,YELLOW,"ѧ��:         ",16,0);
				Show_Str(45,60,RED,YELLOW,"����:   ",16,0);
			}
			displayFlag = !displayFlag;
			
			Delay_ms(300); // ��˸���
		}
	}
}


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  * @note   #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  */
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART1, (uint8_t) ch); // �޸�ΪUSART1
	
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) // �޸�ΪUSART1
	{}
	
	return ch;
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
