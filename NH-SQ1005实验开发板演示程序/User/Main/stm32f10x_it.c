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

extern uint8_t BeepFlag,SoundEnable;
extern 	uint8_t res,ReadFlag;				
u8  INDEX=0;
char str;	
u8    USART_TX_BUF[10];
u8 		USART_RX_BUF[10];		

extern u8 TimeDisplay;
extern uint8_t KEY;

void USART2_IRQHandler(void)
{
/*if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{	USART_ClearFlag(USART2, USART_IT_RXNE);		
			USART_RX_BUF[INDEX]=USART_ReceiveData(USART2);
			if(INDEX>=3) INDEX=0;
			else INDEX++;
		if((USART_RX_BUF[0]+USART_RX_BUF[1]+USART_RX_BUF[2])==USART_RX_BUF[3]) BeepFlag=1;
	}*/
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
		GPIOB->ODR^=(1<<13);			 										 /*让LED5闪烁*/
		TimeDisplay=1;															 /*置显示时间到标志*/
  }
}


/***************************************************
*函数名称：void USART3_IRQHandler(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：USART3接收中断用于串口蓝牙模块通信
*****************************************************/

  void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{	USART_ClearFlag(USART3, USART_IT_RXNE);		
			USART_RX_BUF[INDEX]=USART_ReceiveData(USART3);
			if(INDEX>=3) {INDEX=0;BeepFlag=1;		USART_SendData(USART2, USART_RX_BUF[INDEX]);while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));}
			else INDEX++;
	//	if((USART_RX_BUF[0]+USART_RX_BUF[1]+USART_RX_BUF[2])==USART_RX_BUF[3]) /*累加和校验*/BeepFlag=1;
	}
if((GPIOB->IDR&(1<<12))==0)//PB12为是否发声的判断引脚
	SoundEnable=1;
else SoundEnable=0;
}

/*********************************************************
*
*  接收基于USART3的蓝牙命令：
*  格式：0x00 COMM DAT HH   HH=COMM+DAT 
*  读命令：COMM=1,写控制:COMM=2
*  具体命令：
*  00 01 01 02  读电位器电压
*  00 01 02 03  读Pt100温度
*  00 01 03 04  读水位高度
*  00 01 04 05  读内部温度
*  00 01 05 06  读超声波测量的距离
*  
*	 00 02 11 13  LED1点亮
*	 00 02 01  3  LED1灭
*	 00 02 12 14  LED2点亮
*	 00 02 02  4  LED2灭
*	 00 02 13 15  LED3点亮
*	 00 02 03  5  LED3灭
*	 00 02 14 16  LED4点亮
*	 00 02 04  6  LED4灭

*	 00 02 15 17  LEDR点亮,继电器控制的220V红灯亮
*	 00 02 05  7  LEDR灭
*	 00 02 16 18  LEDG点亮，继电器控制的220V绿灯亮
*	 00 02 06 08  LEDG灭

*	 00 02 17 19  电机正转
*  00 02 27 29  电机反转
*  00 02 07 09  电机停止

*  00 02 18 1A  使能语音
*  00 02 08 0A  禁止语音

*  00 02 19 1B  使能蜂鸣器
*  00 02 09 0B  禁止蜂鸣器

**********************************************************/
	
	
void UART3_Operation()	
{
	
	if (USART_RX_BUF[1]==1)
	{//读数据

	 	switch(USART_RX_BUF[2])
		{		
			case 1:
			ReadFlag=1;
			BeepFlag=1;
			break;
			case 2:
			ReadFlag=2;
			BeepFlag=1;
			break;
			case 3:
			ReadFlag=3;
			BeepFlag=1;
			break;
			case 4:
			ReadFlag=4;
			BeepFlag=1;
			break;
			case 5:
			ReadFlag=5;
			BeepFlag=1;
			break;
		}
	}
else if (USART_RX_BUF[1]==2)
	{//写控制
	
  	switch(USART_RX_BUF[2])
		{
			case 0x11:USART3_SendString("\r\n Turn on LED1.");
			USART2_SendString("\r\n 开LED1指示灯.\r\n");
			LED1(0);
			ReadFlag=211;
			BeepFlag=1;
			break;
			case 0x01:USART3_SendString("\r\n Turn off LED1.");
			USART2_SendString("\r\n 关LED1指示灯.\r\n");
			LED1(1);
			ReadFlag=201;
			BeepFlag=1;
			break;
			case 0x12:USART3_SendString("\r\n Turn on LED2.");
			USART2_SendString("\r\n 开LED2指示灯.\r\n");
			LED2(0);
			ReadFlag=212;
			BeepFlag=1;
			break;
			case 0x02:USART3_SendString("\r\n Turn off LED2.");
			USART2_SendString("\r\n 关LED2指示灯.\r\n");
			LED2(1);
			ReadFlag=202;
			BeepFlag=1;
			break;
			case 0x13:USART3_SendString("\r\n Turn on LED3.");
			USART2_SendString("\r\n 开LED3指示灯.\r\n");
			LED3(0);
			ReadFlag=213;
			BeepFlag=1;
			break;
			case 0x03:USART3_SendString("\r\n Turn off LED3.");
			USART2_SendString("\r\n 关LED3指示灯.\r\n");
			LED3(1);
			ReadFlag=203;
			BeepFlag=1;
			break;
			case 0x14:USART3_SendString("\r\n Turn on LED4.");
			USART2_SendString("\r\n 开LED4指示灯.\r\n");
			LED4(0);
			ReadFlag=214;
			BeepFlag=1;
			break;
			case 0x04:USART3_SendString("\r\n Turn off LED4.");
			USART2_SendString("\r\n 关LED4指示灯.\r\n");
			LED4(1);
			ReadFlag=204;
			BeepFlag=1;
			break;
			case 0x15:USART3_SendString("\r\n Turn on Red Light.");
			USART2_SendString("\r\n 开红灯.\r\n");
			ReadFlag=215;
			BeepFlag=1;
			LEDG(1);
			GPIOC->ODR&=~(1<<4);
			break;
			case 0x05:USART3_SendString("\r\n Turn off Red Light.");
			USART2_SendString("\r\n 开红灯.\r\n");
			ReadFlag=205;
			BeepFlag=1;
			LEDG(0);
			GPIOC->ODR|=(1<<4);
			break;
			case 0x16:USART3_SendString("\r\n Turn on Green Light.");
			USART2_SendString("\r\n 开绿灯.\r\n");
			ReadFlag=216;
			BeepFlag=1;
			LEDR(1);
			GPIOC->ODR&=~(1<<15);
			break;
			case 0x06:USART3_SendString("\r\n Turn off Green Light.");
			USART2_SendString("\r\n 关绿灯.\r\n");
			ReadFlag=206;
			BeepFlag=1;
			LEDR(0);
			GPIOC->ODR|=(1<<15);
			break;
			case 0x17:USART3_SendString("\r\n Turn on motor=Open. ");
			USART2_SendString("\r\n 让板载电机正转.\r\n ");
			ReadFlag=217;
			BeepFlag=1;
			GPIOB->ODR&=~(1<<8);
			GPIOB->ODR|=(1<<9);
			LEDG(0);LEDR(1);
			break;
			case 0x27:USART3_SendString("\r\n Turn on motor=Close. ");
			USART2_SendString("\r\n 让板载电机反转.\r\n ");
			ReadFlag=227;
			BeepFlag=1;
			GPIOB->ODR|=(1<<8);
			GPIOB->ODR&=~(1<<9);
			LEDR(0);LEDG(1);
			break;
			case 0x07:USART3_SendString("\r\n Turn off motor. ");
			USART2_SendString("\r\n 板载电机停止运转.\r\n ");
			ReadFlag=207;
			BeepFlag=1;
			GPIOB->ODR&=~(1<<8);
			GPIOB->ODR&=~(1<<9);
			LEDG(0);LEDR(0);
			break;
			case 0x18:USART3_SendString("\r\n Turn on Sound. ");
			USART2_SendString("\r\n 开语音提示.\r\n ");
			BeepFlag=1;
			ReadFlag=218;
			SoundEnable=1;
			break;
			case 0x08:USART3_SendString("\r\n Turn off Sound. ");
			USART2_SendString("\r\n 关语音提示.\r\n ");
			ReadFlag=208;
			BeepFlag=1;
			SoundEnable=0;
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
