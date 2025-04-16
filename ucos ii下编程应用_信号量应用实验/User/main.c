/********************************************************
**--------基于WEEED4.1 的NUAA_CS_107 kit操作系统uC/OS-II实验------------------------------------------
** File name:           main.c
** Last modified Date:  2017-5-1
** Last Version:        V2.00
** Descriptions:        ucos ii消息队列应用
**
*****************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "lcd.h"
#include "stm32f10x.h"
#include <ucos_ii.h>
#include "os_cpu.h"
#include  <cpu.h>
#include  <app_cfg.h>

OS_EVENT *MyCommSem;
void *CommMsg[OS_MAX_QS];	//4个指针的数组


static OS_STK AppTask1Stk[APP_TASK1_STK_SIZE];		//任务1堆栈
static OS_STK AppTask2Stk[APP_TASK2_STK_SIZE];		//任务2堆栈

static void AppTaskCreate(void);
static void AppTask1(void *p_arg);
static void AppTask2(void *p_arg);

/*****************************************************************************
** 函数名称: int main(void)
** 功能描述: 主函数入口
** 入口参数: 无
** 出口参数: 无
*****************************************************************************/
int  main (void)
{
	LED_Configuration();
	LCD_Init();					//LCD初始化
	
	
	Welcome(); 		//显示主界面
		
	SysTick_Configuration(); 	//系统定时器初始化
	
	USART_Configuration();  	//串口初始化

	OSInit();      	//usos ii初始化
	
	MyCommSem= OSSemCreate(0); //建立信号量，初始为0表示等待一个事件或多个事件发生
	
	AppTaskCreate();//创建任务

	OSStart();      //开始任务调度
}

/*****************************************************************************
** 函数名称: static  void  AppTaskCreate(void)
** 功能描述: 创建任务
** 入口参数: 无
** 出口参数: 无
*****************************************************************************/
static  void  AppTaskCreate(void)
{
	OSTaskCreate(AppTask1,(void *)0,(OS_STK *)&AppTask1Stk[APP_TASK1_STK_SIZE-1],APP_TASK1_PRIO);

	OSTaskCreate(AppTask2,(void *)0,(OS_STK *)&AppTask2Stk[APP_TASK2_STK_SIZE-1],APP_TASK2_PRIO);		//创建任务2

}



/*****************************************************************************
** 函数名称: static  void  AppTask1 (void *p_arg)
** 功能描述: 任务1发送信号量
** 入口参数: p_arg
** 出口参数: 无
*****************************************************************************/

void  AppTask1 (void *p_arg)
{
	while(1)
	{
		if (GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)==0)  /*有KEY1按下*/
		{	
		OSSemPost(MyCommSem);
		GPIOD->ODR&=~(1<<2);				/*LED1亮*/	
			printf("已发送信号量，\n");
		}
		else GPIOD->ODR|=(1<<2)		;/*LED1灭	*/
		OSTimeDly(200);
	}		 
}
/*****************************************************************************
** 函数名称: static  void  AppTask1 (void *p_arg)
** 功能描述: 任务2接收信号量并通过串口输出，LCD上显示
** 入口参数: p_arg
** 出口参数: 无
*****************************************************************************/
void  AppTask2 (void *p_arg)
{
	INT8U err;
	while(1)
	{
		
		OSSemPend(MyCommSem, 0, &err);	/*获取消息*/
			if(err == OS_NO_ERR)
			{
			printf("接收信号量成功！\n");
			GPIOC->ODR|=(1<<0);	
						Delay_ms(200);	
			GPIOC->ODR&=~(1<<0);	
			}
 	}
}

/******************* NUAA****/
