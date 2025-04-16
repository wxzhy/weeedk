/**************NUAA_CM3_107实验开发板*********************************
描述： Descriptions:        一些简单外设初始化设置																	 
*********************************************************************************/ 

#include "hw_config.h"

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

/*********************************************************************************
	*函数名称：void LED_Configuration(void)
	*
	*入口参数：无
	*
	*出口参数：无
	*
	*功能说明：LED初始化配置
*********************************************************************************/
void GPIO_Configuration(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);  /*初始化GPIOD输出端口*/
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);  /*初始化GPIOD端口PD11=KEY1,PD12=KEY2上拉输入，50MHz*/

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);  /*初始化GPIOA端口PA0=KEY4上拉输入，50MHz*/
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);  /*初始化GPIOA端口PA5推挽输出，50MHz*/

/* MCO pin configuration------------------------------------------------- */
  /* Configure MCO (PA8) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  RCC_MCOConfig(RCC_MCO_HSE);/*PA8输出25MHz*/


	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA , GPIO_PinSource0);/*PA0作为外部中断线*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource11);/*PD11作为外部中断线*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource12);/*PD12作为外部中断线*/

/*---------初始化状态四个LED全灭OFF------------*/
	GPIO_SetBits(GPIOD,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);//不让喇叭发声
			
}

/***********************************************************************
函数名称：NVIC_Configuration(void)
功    能：完成中断优先级的配置
输入参数：
输出参数：
编写时间：
编 写 人：
注    意：
***********************************************************************/
void NVIC_Configuration(void)
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);  	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel =EXTI15_10_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;						/*配置EXTI第0线(PA0=KEY4)的中断向量*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel =RTC_IRQn  ;	    	/*配置EXTI第15~10线(PD11=KEY1,PD12=KEY2)的中断向量*/ 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
}
/***************************************************
*函数名称：void EXTI_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：PD11/PD12/PC13和PA0配置为下降沿触发的外部中断输入
*****************************************************/
void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	/*PD11外部中断输入*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	/*下降沿触发*/
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
	/*PD12外部中断输入*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	/*下降沿触发*/
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
	/*PA0外部中断输入*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	/*下降沿触发*/
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
}

/**************************************************
*函数名称：void USART_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：配置USART1
***************************************************/
void USART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	 //打开USART1时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	 //打开USART2时钟

  USART_InitStructure.USART_BaudRate = 115200;  //USRAT1/2波特率均设置为115200
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

  USART_Init(USART1, &USART_InitStructure);  
  USART_Init(USART2, &USART_InitStructure);  
 
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //使能USART1接收中断
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //使能USART2接收中断

  USART_Cmd(USART1, ENABLE);//使能USART1外设
  USART_Cmd(USART2, ENABLE);//使能USART2外设
}




