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
int main(void)
{
	Delay_Init();               /*ϵͳ�δ�ʱ��SysTick��ʼ��*/
  TIM_Configuration(500);			/*��ʱ����ʼ������ʱ500ms�����ж�*/
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
		if (TFlag)										/*��ʱ������TIM2��ʱ500msʱ�䵽��־*/
		{
			TFlag=0;
		LCD_ShowNum(130,240,ADC_Value,4,16);
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
	USART_SendData(USART3, (uint8_t) ch);   
	
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
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
