/********************************************************
*  采用NUAA_CM3_107  WEEEDK实验开发板
*  超声腔波测距实验
   连接超声波传感器，到J28超声波传感器插座，注意超声波感知口面向外侧，即电源与电源标识要对齐，别接反了
	 下载运行程序即可在LCD屏上显示测量得到的距离。改进算法可以让显示更稳定。
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "ultrasonic.h"
#include "lcd.h"
#include "delay.h"
#include <stdio.h>

extern u8 gImage_fg[];
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
/**									 _
	*名称：void Delay(void)
	*参数：无
	*返回：无
	*功能：简单延时
**/
void Delay(void)
{
	int i,j;
	for(i=1000;i>0;i--)
		for(j=1000;j>0;j--);

}

uint8_t BeepFlag=0;
char res;

int main(void)
{
	float temp;
	char DisNum[16];
//char str[16];
uint8_t strd[16],i=0;
	SystemInit();
	Delay_Init();               //使用SysTick实现精确延时

	LCD_Init();					//LCD初始化

	USART_Configuration();
	Welcome(); 		//显示主界面
	GPIO_Configuration();
	NVIC_Configuration();		


	LED1(1);LED2(1);LED3(1);LED4(1);
	POINT_COLOR=WHITE;BACK_COLOR=RED;
	Ultran_Init();
	
	while(1)
	{		
	Ultra_Ranging(&temp);
			sprintf(DisNum,"%2.3f",temp);
			for (i=0;i<8;i++) strd[i]=DisNum[i];
	LCD_ShowString(86, 160, 16,strd,0);

  // 	Delay_ms(100);
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
	USART_SendData(USARTx, (uint8_t) ch);   
	
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
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
