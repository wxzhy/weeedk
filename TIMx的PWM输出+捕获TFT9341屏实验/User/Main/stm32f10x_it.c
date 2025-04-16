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

u8  TIM2CH1_CAPTURE_STA=0;	/*���벶��״̬,��6λ��0~5λ����ʾ�͵�ƽ��Чʱ��¼�����жϵĴ�����ÿ�θ����жϼ���65536�Σ�65536us for 1MHz��
																					��6λΪ�½��ر�־��1=���½��ز���0��Ч
																					��7λΪ�����ر�־��1=�������ز���0��Ч							                */
u16	TIM2CH1_CAPTURE_VAL;		/*���벶��ֵ	*/
 

/*��ʱ��2�жϷ������	 */
void TIM2_IRQHandler(void)
{ 

 	if((TIM2CH1_CAPTURE_STA&(1<<7))==0)				/*��δ�ɹ�����	*/
	{ 
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM2CH1_CAPTURE_STA&(1<<6))				/*�Ѿ������½���*/
			{
				if((TIM2CH1_CAPTURE_STA&0X3F)==0X3F)/*�͵�ƽ̫����(�ﵽ64����65536�θ����ж�Ϊ4194304us)*/
				{
					TIM2CH1_CAPTURE_STA|=(1<<7);			/*��ǳɹ�������һ��*/
					TIM2CH1_CAPTURE_VAL=0XFFFF;				/*��һ�θ����жϲ���ֵ��Ϊ65535*/
				}else TIM2CH1_CAPTURE_STA++;				/*�����жϴ�����1*/
			}	 
		}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)/*����1���������¼�,�������½���*/
		{	
			if(TIM2CH1_CAPTURE_STA&(1<<6))				/*���ԭ���������أ��򲶻�һ���Ͻ��� */		
			{	  			
				TIM2CH1_CAPTURE_STA|=(1<<7);				/*��ǳɹ�����һ���Ͻ��� */
				TIM2CH1_CAPTURE_VAL=TIM_GetCapture1(TIM2);/*ȡ����ʱ�ļ���ֵ��us������Ϊ����Ƶ��Ϊ1MHz��*/
		   	TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling); /*���������غ�������һ��Ϊ�����ز���*/
			}else  																/*��δ��ʼ,��һ�β����½���*/
			{
				TIM2CH1_CAPTURE_STA=0;							/*��β�����ռ���ֵ��״̬ */
				TIM2CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM2,0);							/*�������ü�������ʼΪ0*/
				TIM2CH1_CAPTURE_STA|=(1<<6);				/*��ǲ�����������*/
		   	TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising);		/*�����½��غ�������һ��Ϊ�Ͻ��ز���*/
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); /*����жϱ�־λ*/
}



/***********************END OF FILE****/
