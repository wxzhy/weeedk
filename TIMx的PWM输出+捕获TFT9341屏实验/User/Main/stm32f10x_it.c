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

u8  TIM2CH1_CAPTURE_STA=0;	/*输入捕获状态,低6位（0~5位）表示低电平有效时纪录更新中断的次数，每次更新中断计数65536次（65536us for 1MHz）
																					第6位为下降沿标志，1=有下降沿捕获，0无效
																					第7位为上升沿标志，1=有上升沿捕获，0无效							                */
u16	TIM2CH1_CAPTURE_VAL;		/*输入捕获值	*/
 

/*定时器2中断服务程序	 */
void TIM2_IRQHandler(void)
{ 

 	if((TIM2CH1_CAPTURE_STA&(1<<7))==0)				/*还未成功捕获到	*/
	{ 
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM2CH1_CAPTURE_STA&(1<<6))				/*已经捕获到下降沿*/
			{
				if((TIM2CH1_CAPTURE_STA&0X3F)==0X3F)/*低电平太长了(达到64倍的65536次更新中断为4194304us)*/
				{
					TIM2CH1_CAPTURE_STA|=(1<<7);			/*标记成功捕获了一次*/
					TIM2CH1_CAPTURE_VAL=0XFFFF;				/*有一次更新中断捕获值置为65535*/
				}else TIM2CH1_CAPTURE_STA++;				/*更新中断次数加1*/
			}	 
		}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)/*捕获1发生捕获事件,上升或下降沿*/
		{	
			if(TIM2CH1_CAPTURE_STA&(1<<6))				/*如果原来是下升沿，则捕获到一个上降沿 */		
			{	  			
				TIM2CH1_CAPTURE_STA|=(1<<7);				/*标记成功捕获到一次上降沿 */
				TIM2CH1_CAPTURE_VAL=TIM_GetCapture1(TIM2);/*取捕获时的计数值（us数，因为计数频率为1MHz）*/
		   	TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling); /*捕获到上升沿后，设置下一次为下升沿捕获*/
			}else  																/*还未开始,第一次捕获下降沿*/
			{
				TIM2CH1_CAPTURE_STA=0;							/*奇次捕获，清空计数值及状态 */
				TIM2CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM2,0);							/*重新设置计数器初始为0*/
				TIM2CH1_CAPTURE_STA|=(1<<6);				/*标记捕获到了下升沿*/
		   	TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising);		/*捕获到下降沿后，设置下一次为上降沿捕获*/
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); /*清除中断标志位*/
}



/***********************END OF FILE****/
