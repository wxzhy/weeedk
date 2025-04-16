
/************************************************************************************
**	  ����107�����壨V1.0��
**	  TIMERʵ��
**
**	  ��    ̳��bbs.openmcu.com
**	  ��    ����www.openmcu.com
**	  ��    �䣺support@openmcu.com
**
**    ��    ����V1.1
**	  ��    �ߣ�openmcu
**	  �������:	2014-3-11
------------------------------------------------------------------------------------
**	�������˵��: 
**				  
**
**				  ���س��򣬰��¸�λ��S3������ʼ���У�LD1����˸��ͨ������ʱʵ�ֵ�;
**				LD5����˸��ͨ����ʱ����ʱʵ�ֵģ���˸���1S;		   
**                          
************************************************************************************/
#include <stdio.h>
#include "hw_config.h"


/**************************************************
*�������ƣ�void Delay(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵��������ʱ
***************************************************/
void Delay(void)
{
	int x,y;
	for(x=1000;x>0;x--)
		for(y=1000;y>0;y--);

}


int main(void)
{
  SystemInit();     	  //ϵͳ��ʼ��
  LED_Configuration();	  //LED��ʼ������
  NVIC_Configuration();	  //NVIC��ʼ��
  TIMER_Configuration();  //TIM4��ʼ��	
  while(1)
  {
	  LED1(0);	          //LED OFF 
	  Delay();
	  LED1(1);		      //LED ON  
	  Delay();
  }
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);   

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
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
