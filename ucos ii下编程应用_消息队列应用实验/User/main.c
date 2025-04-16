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

OS_EVENT *CommQ;
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
	
	CommQ= OSQCreate(&CommMsg[0], 10); //建立消息队列 长度为10
	
	AppTaskCreate();//创建任务

	OSStart();      //开始任务调度
}

/*****************************************************************************
** 函数名称: void  AppTaskCreate(void)
** 功能描述: 创建任务
** 入口参数: 无
** 出口参数: 无
*****************************************************************************/
void  AppTaskCreate(void)
{
OSTaskCreateExt(   // 任务创建任务1
									AppTask1,																				//任务代码指针
									(void *)0,																			//传递给任务的参数指针
									(OS_STK *)&AppTask1Stk[APP_TASK1_STK_SIZE-1],		//任务的堆栈指针
									APP_TASK1_PRIO,																	//任务1任务优先级为4
									OS_PROBE_TASK_ID,																//任务ID（任务ID uc/osII没有使用）
									(OS_STK *)&AppTask1Stk[0],											//栈底指针
									APP_TASK1_STK_SIZE, 														//堆栈大小64字节	
									(void *)0,																			//扩展数据结构的指针
									OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);				//使能本任务堆栈检查，并且任务产生时清除堆栈

		OSTaskCreateExt(AppTask2,(void *)0,(OS_STK *)&AppTask2Stk[APP_TASK2_STK_SIZE-1],APP_TASK2_PRIO,OS_PROBE_TASK_ID,(OS_STK *)&AppTask2Stk[0],
					APP_TASK2_STK_SIZE,(void *)0, OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);		//创建任务2

}

/*****************************************************************************
** 函数名称: void  AppTask1 (void *p_arg)
** 功能描述: 任务1从消息队列获取消息并通过串口输出，LCD上显示
** 入口参数: p_arg
** 出口参数: 无
*****************************************************************************/
void  AppTask1 (void *p_arg)
{
	INT8U err;
	void *msg;
	while(1)
	{
		msg= OSQPend(CommQ, 100, &err);	//获取消息
		if (err == OS_NO_ERR){
		printf("\n\r读取队列成功：%s\r\n",(INT8U *)msg);//读取成功，打印消息
		} else{
			printf("\n\r读取失败\r\n");		//读取失败
		}		
	OSTimeDly(100);
		GPIOD->ODR^=(1<<2);//LED1闪烁
 	}
}

/*****************************************************************************
** 函数名称: void  AppTask2 (void *p_arg)
** 功能描述: 任务2发送消息到消息队列
** 入口参数: p_arg
** 出口参数: 无
*****************************************************************************/
INT8U *CommRxBuf="\n消息队列传送的信息为“南京航空航天大学嵌入式实验开发板RTOS实验之消息队列传递信息”\n";//NUAA_CS_CM3 Kit WEEEDK";

void  AppTask2 (void *p_arg)
{
	INT8U err;
	while(1)
	{
		err= OSQPost(CommQ, (void *)&CommRxBuf[0]);
		if (err == OS_NO_ERR){
			printf("\n\r消息加入队列中 ");//将消息放入消息队列 	
		} else{
			printf("\n\r队列已满");//消息队列已满 	
		}
		GPIOD->ODR^=(1<<3);//LED2闪烁	
		OSTimeDlyHMSM(0,0,0,100);

	}		 
}


/******************* NUAA****/
