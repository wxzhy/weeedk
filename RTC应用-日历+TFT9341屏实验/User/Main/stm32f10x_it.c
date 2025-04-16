/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "hw_config.h"
#include "lcd.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */
extern u8 TimeDisplay;
extern uint8_t KEY;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}
u8 TP1=0,TP2=0,TP3=0;

void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  /*判断是否是更新中断*/
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);       /*清除中断标志*/
		TP1++;
		if (TP1&1) 			GPIO_ResetBits(GPIOD,GPIO_Pin_2);   /*  PD2=0（LED1亮）*/
		else 						GPIO_SetBits(GPIOD,GPIO_Pin_2); 		/*  PD2=1（LED1灭）*/
	}
}
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 	/*判断是否是更新中断*/
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 			/*清除中断标志*/
		TP2++;
		if (TP1&1) 			GPIO_ResetBits(GPIOD,GPIO_Pin_3);   /*  PD3=0（LED2亮）*/
		else 						GPIO_SetBits(GPIOD,GPIO_Pin_3); 		/*  PD3=1（LED2灭）*/
	}
}
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  /*判断是否是更新中断*/
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);       /*清除中断标志*/
		TP3++;
		if (TP3&1) 			GPIO_ResetBits(GPIOD,GPIO_Pin_4);   /*  PD4=0（LED3亮）*/
		else 						GPIO_SetBits(GPIOD,GPIO_Pin_4); 		/*  PD4=1（LED3灭）*/
	}
}

/************************************************
函数名称 ： RTC_IRQHandler
功    能 ： RTC中断
参    数 ： 无
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void RTC_IRQHandler(void)
{
  if(RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    RTC_ClearITPendingBit(RTC_IT_SEC);           /*清除中断标志位*/
    RTC_WaitForLastTask();                       /*等待操作完成*/
		GPIOD->ODR^=(1<<2);			 										 /*让LED1闪烁*/
		TimeDisplay=1;															 /*置显示时间到标志*/
  }
}


/***************************************************
*函数名称：void EXTI0_IRQHandler(void)
*								
* 出口：KEY=4表示KEY4按键按下
*功能说明：外部中断处理函数，这里外部接KEY4（PA0）键
*****************************************************/
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!= RESET)  	 /*判断Line0线（PA0）上的中断是否发生 */
	{  
		KEY = 0x04;   Delay_ms(20);    /*如果硬件没有消抖电路可用延时消除抖动*/ 
		EXTI_ClearITPendingBit(EXTI_Line0);   
	}			  
}

/***************************************************
*函数名称：void EXTI15_10_IRQHandler(void)
*								
* 出口：KEY=1表示KEY1按键按下，K=2表示KEY2按键按下
*功能说明：外部中断处理函数，这里外部接KEY1（PD11）、KEY2（PD12）键
*****************************************************/
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line11)!= RESET)  	 /*判断Line11线（PD11）上的中断是否发生 */
	{  
		KEY = 0x01; Delay_ms(20);    /*如果硬件没有消抖电路可用延时消除抖动*/ 
		EXTI_ClearITPendingBit(EXTI_Line11);   
	}else{

	if(EXTI_GetITStatus(EXTI_Line12)!= RESET)   /*判断Line12线（PD12）上的中断是否发生 */
	{  
		KEY = 0x02; Delay_ms(20);    /*如果硬件没有消抖电路可用延时消除抖动*/ 
		EXTI_ClearITPendingBit(EXTI_Line12);   
	}}		  
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
