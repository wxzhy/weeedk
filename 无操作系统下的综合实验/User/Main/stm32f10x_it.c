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
extern u8 TFlag;
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

void TIM3_IRQHandler(void) // 修改为TIM3_IRQHandler
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) // 修改为TIM3
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 修改为TIM3
		TFlag=1;
	}
}

/***************************************************
*函数名称：void USART1_IRQHandler(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：USART1接收中断
*****************************************************/
extern uint8_t BeepFlag;
extern 	uint8_t res;				
void USART1_IRQHandler(void) // 修改为USART1_IRQHandler
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // 修改为USART1
	{
   USART_ClearFlag(USART1, USART_IT_RXNE); // 修改为USART1
	  res = USART_ReceiveData(USART1); // 修改为USART1
		switch(res)
		{
			case 'R': // 红色亮，绿色灭
				GPIO_SetBits(GPIOB, GPIO_Pin_1); // 红色亮
				GPIO_ResetBits(GPIOB, GPIO_Pin_2); // 绿色灭
				USART1_SendString("\r\nTurn On Red LED.");
				break;
				
			case 'G': // 绿色亮，红色灭
				GPIO_ResetBits(GPIOB, GPIO_Pin_1); // 红色灭
				GPIO_SetBits(GPIOB, GPIO_Pin_2); // 绿色亮
				USART1_SendString(("\r\nTurn On Green LED.");
				break;
				
			case 'S': // 全灭
				GPIO_ResetBits(GPIOB, GPIO_Pin_1); // 红色灭
				GPIO_ResetBits(GPIOB, GPIO_Pin_2); // 绿色灭
				USART1_SendString("\r\nTurn off Red and Green LED.");
				break;
				
			case 'D': // 全亮
				GPIO_SetBits(GPIOB, GPIO_Pin_1); // 红色亮
				GPIO_SetBits(GPIOB, GPIO_Pin_2); // 绿色亮
				USART1_SendString("\r\nTurn On Red and Green LED.");
				break;
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
