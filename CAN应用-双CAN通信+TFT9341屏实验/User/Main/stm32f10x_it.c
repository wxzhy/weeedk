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

 extern uint8_t KeyFlag;

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
/**************************************************
*函数名称：void EXTI15_10_IRQHandler(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：外部中断响应函数，这里外部接USER1,2键
***************************************************/
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line11)!= RESET)  	 //判断Line11线上的中断是否发生 
	{  
		EXTI_ClearITPendingBit(EXTI_Line11);   
		KeyFlag = 0x01;
	}

	if(EXTI_GetITStatus(EXTI_Line12)!= RESET)   //判断Line12线上的中断是否发生
	{  
		EXTI_ClearITPendingBit(EXTI_Line12);   
		KeyFlag = 0x02;
	}    
	if(EXTI_GetITStatus(EXTI_Line13)!= RESET)   //判断Line13线上的中断是否发生
	{  
		EXTI_ClearITPendingBit(EXTI_Line13);   
		KeyFlag = 0x03;
	}    
}


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}
extern uint8_t BeepFlag;
extern 	char res;
/***************************************************
*函数名称：void USART3_IRQHandler(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：USART1接收中断用于串口蓝牙模块通信
*****************************************************/
			
  void USART1_IRQHandler(void)
{
char res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
   USART_ClearFlag(USART1, USART_IT_RXNE);		
	  res =USART_ReceiveData(USART1);		       // 读取接收到的数据USART1->DR
	  USART1_SendString("\r\n 接收到的字符是");
		while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
		USART_SendData(USART1, res);
		switch(res)
		{
			case '1':USART1_SendString("\r\n Turn on LED1.");
			LED1(0);LED2(1);LED3(1);
	  	Gui_StrCenter(0,170,WHITE,BLACK,"   Turn on LED1!   ",16,0);//居中显示
			BeepFlag=1;
			break;
			case '2':USART1_SendString("\r\n Turn on LED2.");
			LED1(1);LED2(0);LED3(1);
	  	Gui_StrCenter(0,170,WHITE,BLACK,"   Turn on LED2!   ",16,0);//居中显示
			BeepFlag=1;
			break;
			case '3':USART1_SendString("\r\n Turn on LED3.");
			LED1(1);LED2(1);LED3(0);
	  	Gui_StrCenter(0,170,WHITE,BLACK,"   Turn on LED3!   ",16,0);//居中显示
			BeepFlag=1;
			break;
			case '5':USART1_SendString("\r\n Turn on all LED.");
			LED1(0);LED2(0);LED3(0);
	  	Gui_StrCenter(0,170,WHITE,BLACK,"Turn on LED1~LED3! ",16,0);//居中显示
			BeepFlag=1;
			break;
			case '6':USART1_SendString("\r\n Turn off all LED. ");
			BeepFlag=1;
			LED1(1);LED2(1);LED3(1);
	   	Gui_StrCenter(0,170,WHITE,BLACK,"Turn off LED1~LED3!",16,0);//居中显示
			break;

		}
	}  	
}
/***************************************************
*函数名称：void USART2_IRQHandler(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：USART2接收中断用于串口蓝牙模块通信
*****************************************************/
				
  void USART2_IRQHandler(void)
{
char res;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
   USART_ClearFlag(USART2, USART_IT_RXNE);		
	  res =USART_ReceiveData(USART2);		       // 读取接收到的数据USART1->DR
	  USART1_SendString("\r\n 接收到的字符是");
		while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
		USART_SendData(USART2, res);
		switch(res)
		{
			case '1':USART2_SendString("\r\n Turn on LED1.");
			LED1(0);LED2(1);LED3(1);
	  	Gui_StrCenter(0,170,WHITE,BLACK,"   Turn on LED1!    ",16,0);//居中显示
			BeepFlag=1;
			break;
			case '2':USART2_SendString("\r\n Turn on LED2.");
			LED1(1);LED2(0);LED3(1);
	  	Gui_StrCenter(0,170,WHITE,BLACK,"   Turn on LED2!    ",16,0);//居中显示
			BeepFlag=1;
			break;
			case '3':USART2_SendString("\r\n Turn on LED3.");
			LED1(1);LED2(1);LED3(0);
	  	Gui_StrCenter(0,170,WHITE,BLACK,"   Turn on LED3!    ",16,0);//居中显示
			BeepFlag=1;
			break;
			case '5':USART2_SendString("\r\n Turn on all LED.");
			LED1(0);LED2(0);LED3(0);
	  	Gui_StrCenter(0,170,WHITE,BLACK,"   Turn on LED1~LED3!    ",16,0);//居中显示
			BeepFlag=1;
			break;
			case '6':USART2_SendString("\r\n Turn off all LED. ");
			BeepFlag=1;
			LED1(1);LED2(1);LED3(1);
	  	Gui_StrCenter(0,170,WHITE,BLACK,"Turn off LED1~LED3!",16,0);//居中显示
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
