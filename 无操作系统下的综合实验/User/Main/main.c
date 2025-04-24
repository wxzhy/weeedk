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
u8 flashFlag=0; // 添加闪烁标志
u16 Pulse=0;    // 添加PWM脉冲变量
int main(void)
{
	Delay_Init();               /*系统滴答定时器SysTick初始化*/
  TIM_Configuration(5000);		/*定时器初始化，定时5000ms允许中断，已改为TIM3*/
	GPIO_Configuration();
	LCD_Init();									/*LCD初始化*/
	ADC_Configuration();				/*ADC初始化*/
	DAC_Configuration();	   		/*DAC1初始化*/  
	USART_Configuration();			/*USART初始化*/
	DisplayInf();    						/*在LCD屏上显示初始信息，包括学号和姓名*/
	NVIC_Configuration();				/*嵌入向量中断控制器NVIC初始化*/
while(1)
	{	
		value=ReadADCAverageValue();	/*取A/D转换结果的平均值*/
		ADC_Value=value*3300/4095;		/*通过标度变换为mV*/
		DAC_SetChannel1Data(DAC_Align_12b_R,value);/*将待变换的DAC数据放右对齐寄存器中*/
		
		// 根据电位器采样值来改变占空比
		Pulse=value*1000/4095;
		TIM_SetCompare3(TIM4,Pulse);
		
		// 检测TFlag标志，显示电位器电压值
		if (TFlag)									
		{
			TFlag=0;
			LCD_ShowNum(130,240,ADC_Value,4,16);
		}
		
		// 按键处理部分
		// KEY1按下点亮LED1，抬起灭
		if(!KEY1()) {
			LED1(0); // 点亮LED1
		} else {
			LED1(1); // 熄灭LED1
		}
		
		// KEY2按下蜂鸣器响一声
		if(!KEY2()) {
			BEEP(1); // 蜂鸣器响
			Delay_ms(100);
			BEEP(0); // 蜂鸣器停
			while(!KEY2()); // 等待按键释放
		}
		
		// KEY3按下，姓名学号闪烁，LED1-LED4跑马灯
		if(!KEY3()) {
			flashFlag = 1; // 设置闪烁标志
			while(!KEY3()); // 等待按键释放
		}
		
		// KEY4按下恢复正常显示
		if(!KEY4()) {
			flashFlag = 0; // 清除闪烁标志
			LED1(1); // 熄灭所有LED
			LED2(1);
			LED3(1);
			LED4(1);
			while(!KEY4()); // 等待按键释放
		}
		
		// 如果闪烁标志设置，实现跑马灯效果
		if(flashFlag) {
			static u8 ledIndex = 0;
			static u8 displayFlag = 0;
			
			// 跑马灯效果
			LED1(1); LED2(1); LED3(1); LED4(1); // 全灭
			switch(ledIndex) {
				case 0: LED1(0); break; // LED1亮
				case 1: LED2(0); break; // LED2亮
				case 2: LED3(0); break; // LED3亮
				case 3: LED4(0); break; // LED4亮
			}
			ledIndex = (ledIndex + 1) % 4;
			
			// 姓名学号闪烁
			if(displayFlag) {
				Show_Str(45,40,RED,YELLOW,"学号:162210426",16,0);
				Show_Str(45,60,RED,YELLOW,"姓名:朱浩瑜",16,0);
			} else {
				Show_Str(45,40,RED,YELLOW,"学号:         ",16,0);
				Show_Str(45,60,RED,YELLOW,"姓名:   ",16,0);
			}
			displayFlag = !displayFlag;
			
			Delay_ms(300); // 闪烁间隔
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
	USART_SendData(USART1, (uint8_t) ch); // 修改为USART1
	
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) // 修改为USART1
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
