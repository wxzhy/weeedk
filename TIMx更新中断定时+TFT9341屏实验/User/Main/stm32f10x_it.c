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

void LCD1(u8 i)
{
	if(i)
		Gui_StrCenter(0,130,WHITE,BLACK,"TIM1 更新定时 1000ms",16,0);//居中显示	
	else
		Gui_StrCenter(0,130,WHITE,BLACK,"TIM1 更新定时     ms",16,0);//居中显示	
}

void LCD2(u8 i)
{
	if(i)
		Gui_StrCenter(0,150,WHITE,BLACK,"TIM2 更新定时  500ms",16,0);//居中显示	
	else
		Gui_StrCenter(0,150,WHITE,BLACK,"TIM2 更新定时     ms",16,0);//居中显示	
}

void LCD3(u8 i)
{
	if(i)
		Gui_StrCenter(0,170,WHITE,BLACK,"TIM3 更新定时  200ms",16,0);//居中显示	
	else
		Gui_StrCenter(0,170,WHITE,BLACK,"TIM3 更新定时     ms",16,0);//居中显示	
}

void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  /*判断是否是更新中断*/
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);       /*清除中断标志*/
		TP1++;
		if (TP1&1) 			{
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);   /*  PD2=0（LED1亮）*/
			LCD1(1);
		}
		else 						{
			GPIO_SetBits(GPIOD,GPIO_Pin_2); 		/*  PD2=1（LED1灭）*/
			LCD1(0);
		}
	}
}
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 	/*判断是否是更新中断*/
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 			/*清除中断标志*/
		TP2++;
		if (TP2&1) 			{
			GPIO_ResetBits(GPIOD,GPIO_Pin_3);   /*  PD3=0（LED2亮）*/
			LCD2(1);
		}
		else 						{
			GPIO_SetBits(GPIOD,GPIO_Pin_3); 		/*  PD3=1（LED2灭）*/
			LCD2(0);
		}
	}
}
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  /*判断是否是更新中断*/
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);       /*清除中断标志*/
		TP3++;
		if (TP3&1) 			{
			GPIO_ResetBits(GPIOD,GPIO_Pin_4);   /*  PD4=0（LED3亮）*/
			LCD3(1);
		}
		else 						{
			GPIO_SetBits(GPIOD,GPIO_Pin_4); 		/*  PD4=1（LED3灭）*/
			LCD3(0);
		}
	}
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
