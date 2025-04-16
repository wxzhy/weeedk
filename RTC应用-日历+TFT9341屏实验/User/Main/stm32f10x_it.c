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
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  /*�ж��Ƿ��Ǹ����ж�*/
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);       /*����жϱ�־*/
		TP1++;
		if (TP1&1) 			GPIO_ResetBits(GPIOD,GPIO_Pin_2);   /*  PD2=0��LED1����*/
		else 						GPIO_SetBits(GPIOD,GPIO_Pin_2); 		/*  PD2=1��LED1��*/
	}
}
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 	/*�ж��Ƿ��Ǹ����ж�*/
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 			/*����жϱ�־*/
		TP2++;
		if (TP1&1) 			GPIO_ResetBits(GPIOD,GPIO_Pin_3);   /*  PD3=0��LED2����*/
		else 						GPIO_SetBits(GPIOD,GPIO_Pin_3); 		/*  PD3=1��LED2��*/
	}
}
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  /*�ж��Ƿ��Ǹ����ж�*/
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);       /*����жϱ�־*/
		TP3++;
		if (TP3&1) 			GPIO_ResetBits(GPIOD,GPIO_Pin_4);   /*  PD4=0��LED3����*/
		else 						GPIO_SetBits(GPIOD,GPIO_Pin_4); 		/*  PD4=1��LED3��*/
	}
}

/************************************************
�������� �� RTC_IRQHandler
��    �� �� RTC�ж�
��    �� �� ��
�� �� ֵ �� ��
��    �� �� strongerHuang
*************************************************/
void RTC_IRQHandler(void)
{
  if(RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    RTC_ClearITPendingBit(RTC_IT_SEC);           /*����жϱ�־λ*/
    RTC_WaitForLastTask();                       /*�ȴ��������*/
		GPIOD->ODR^=(1<<2);			 										 /*��LED1��˸*/
		TimeDisplay=1;															 /*����ʾʱ�䵽��־*/
  }
}


/***************************************************
*�������ƣ�void EXTI0_IRQHandler(void)
*								
* ���ڣ�KEY=4��ʾKEY4��������
*����˵�����ⲿ�жϴ������������ⲿ��KEY4��PA0����
*****************************************************/
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!= RESET)  	 /*�ж�Line0�ߣ�PA0���ϵ��ж��Ƿ��� */
	{  
		KEY = 0x04;   Delay_ms(20);    /*���Ӳ��û��������·������ʱ��������*/ 
		EXTI_ClearITPendingBit(EXTI_Line0);   
	}			  
}

/***************************************************
*�������ƣ�void EXTI15_10_IRQHandler(void)
*								
* ���ڣ�KEY=1��ʾKEY1�������£�K=2��ʾKEY2��������
*����˵�����ⲿ�жϴ������������ⲿ��KEY1��PD11����KEY2��PD12����
*****************************************************/
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line11)!= RESET)  	 /*�ж�Line11�ߣ�PD11���ϵ��ж��Ƿ��� */
	{  
		KEY = 0x01; Delay_ms(20);    /*���Ӳ��û��������·������ʱ��������*/ 
		EXTI_ClearITPendingBit(EXTI_Line11);   
	}else{

	if(EXTI_GetITStatus(EXTI_Line12)!= RESET)   /*�ж�Line12�ߣ�PD12���ϵ��ж��Ƿ��� */
	{  
		KEY = 0x02; Delay_ms(20);    /*���Ӳ��û��������·������ʱ��������*/ 
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
