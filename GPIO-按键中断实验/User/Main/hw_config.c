/******************************************************************
**	 WEEEDK NUAA_CM3_107实验开发板（V4.1）
**	  实验内容：外部中断
**    版    本：V1.1
**	  作    者：NUAA
**	  完成日期:	2016.8.2
********************************************************************/

#include "hw_config.h"

/*********************************************
*函数名称：void NVIC_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：中断嵌套管理
**********************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	            /*使用优先级分组2*/
	/*外部中断线*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn ;	    	/*配置EXTI第15~10线的中断向量*/ 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;	/*抢占优先级0*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	        /*子优先级1*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;	
	NVIC_Init(&NVIC_InitStructure);
}

/*************************************************
*函数名称：void GPIO_Configuration(void)
*
*功能说明：GPIO初始化配置,按键初始化配置
***************************************************/

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE); /*PD端口及复用端口时钟使能*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3  | GPIO_Pin_4  | GPIO_Pin_7 ; /*LED1(PD2)和LED2(PD3)/LED3(PD4)/LED4(PD7)引脚配置为50MHz输出*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;/*KEY1(PD11)和KEY2(PD12)引脚配置为上拉输入*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;/*KEY3(PC13)引脚配置为上拉输入*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC , GPIO_PinSource13);/*PC13作为外部中断线*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource11);/*PD11作为外部中断线*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource12);/*PD12作为外部中断线*/

	/*---------初始化状态四个LED全OFF------------*/
	LED1(1);
	LED2(1);
	LED3(1);
	LED4(1);		
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
	EXTI_Init(&EXTI_InitStructure);	
	/*PC13外部中断输入*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_Init(&EXTI_InitStructure);	
}

