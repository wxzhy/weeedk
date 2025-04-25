/********************************************************
*  NUAA_CM3_107  WEEEDK实验开发板综合考核程序框架
   
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
u16 value;   								/*采样处理后的数字量*/			
float ADC_Value=0.00;       /*实际物理量值*/                  
u8 TFlag=0;
u16 Pulse=500;
int main(void)
{
	Delay_Init();               /*系统滴答定时器SysTick初始化*/
  TIM_Configuration(500);			/*定时器初始化*/
	PWM_GPIO_Init();		    		/*PWM输出口PB8，初始化*/
	Init_PWM(0);			  				/*PWM的通道3初始化设置	，占空比初始为0，让电机不转*/
	GPIO_Configuration();
	LCD_Init();									/*LCD初始化*/
	ADC_Configuration();				/*ADC初始化*/
	DAC_Configuration();	   		/*DAC1初始化*/  
	USART_Configuration();			/*USART初始化*/
	DisplayInf();    						/*在LCD屏上显示初始信息，包括学号和姓名*/
	NVIC_Configuration();				/*嵌入向量中断控制器NVIC初始化*/
while(1)
	{	
		if (TFlag)										/*定时到标志*/
		{
			TFlag=0;
			value=ReadADCAverageValue();	/*取A/D转换结果的平均值*/
			ADC_Value=value*3300/4095;		/*通过标度变换为mV*/
			DAC_SetChannel1Data(DAC_Align_12b_R,value);/*将待变换的DAC数据放右对齐寄存器中*/
			LCD_ShowNum(130,240,ADC_Value,4,16);


			//此处添加代码根据采样值value成正比的改变占空比，使用TIM_SetCompare3(TIM4,Pulse)函数写入CCR3中

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
