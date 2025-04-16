
/************************************************************************************
**	  金龙107开发板（V1.0）
**	  TIMER实验
**
**	  论    坛：bbs.openmcu.com
**	  旺    宝：www.openmcu.com
**	  邮    箱：support@openmcu.com
**
**    版    本：V1.1
**	  作    者：openmcu
**	  完成日期:	2014-3-11
------------------------------------------------------------------------------------
**	程序测试说明: 
**				  
**
**				  下载程序，按下复位键S3，程序开始运行，LD1快闪烁，通过简单延时实现的;
**				LD5慢闪烁，通过定时器延时实现的，闪烁间隔1S;		   
**                          
************************************************************************************/
#include <stdio.h>
#include "hw_config.h"


/**************************************************
*函数名称：void Delay(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：简单延时
***************************************************/
void Delay(void)
{
	int x,y;
	for(x=1000;x>0;x--)
		for(y=1000;y>0;y--);

}


int main(void)
{
  SystemInit();     	  //系统初始化
  LED_Configuration();	  //LED初始化配置
  NVIC_Configuration();	  //NVIC初始化
  TIMER_Configuration();  //TIM4初始化	
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
