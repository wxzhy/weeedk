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

OS_EVENT *MyCommMbox;
void *CommMsg[OS_MAX_QS];	//4个指针的数组


static OS_STK AppTask1Stk[APP_TASK1_STK_SIZE];		//任务1堆栈
static OS_STK AppTask2Stk[APP_TASK2_STK_SIZE];		//任务2堆栈
static OS_STK AppTask3Stk[APP_TASK3_STK_SIZE];		//任务3堆栈

void AppTaskCreate(void);
void AppTask1(void *p_arg);
void AppTask2(void *p_arg);
void AppTask3(void *p_arg);

/*****************************************************************************
** 函数名称: int main(void)
** 功能描述: 主函数入口
** 入口参数: 无
** 出口参数: 无
*****************************************************************************/
int  main (void)
{
	GPIO_Configuration();
	LCD_Init();								/*LCD初始化*/
	
	Welcome(); 								/*显示主界面*/
		
	SysTick_Configuration(); 	/*系统定时器初始化*/
	
	USART_Configuration();  	/*串口初始化*/
	
	OSInit();      						/*usos ii初始化*/
	
	MyCommMbox=OSMboxCreate((void*)0);		/*创建消息邮箱*/

	AppTaskCreate();					/*创建任务*/


	OSStart();      					/*开始任务调度*/
}

/*****************************************************************************
** 函数名称: static  void  AppTaskCreate(void)
** 功能描述: 创建任务
** 入口参数: 无
** 出口参数: 无
*****************************************************************************/
void  AppTaskCreate(void)
{
OSTaskCreate(   /* 任务创建任务1*/
									AppTask1,																				/*任务代码指针*/
									(void *)0,																			/*传递给任务的参数指针*/
									(OS_STK *)&AppTask1Stk[APP_TASK1_STK_SIZE-1],		/*任务的堆栈指针*/
									APP_TASK1_PRIO																/*任务1任务优先级为4*/
						);			
		OSTaskCreate(AppTask2,(void *)0,(OS_STK *)&AppTask2Stk[APP_TASK2_STK_SIZE-1],APP_TASK2_PRIO);		/*创建任务2*/
		OSTaskCreate(AppTask3,(void *)0,(OS_STK *)&AppTask3Stk[APP_TASK3_STK_SIZE-1],APP_TASK3_PRIO);		/*创建任务3*/
}
u8 KEY_Scan()
{	 
	if(KEY1==0||KEY2==0||KEY3==0||KEY4==0)
	{
		OSTimeDly(50);						/*去抖动：绝对不能用自己的延时程序，必须采用系统时间函数，否则后续进入不了任务 */
		if(KEY1==0){GPIOD->ODR&=~(1<<2);return 1;}
		else if(KEY2==0){GPIOD->ODR&=~(1<<3);return 2;}
		else if(KEY3==0){GPIOD->ODR&=~(1<<4);return 3; }
		else if(KEY4==0){GPIOD->ODR&=~(1<<7);return 4;} 
	}
	else GPIOD->ODR|=(1<<2)|(1<<3)|(1<<4)|(1<<7);
	return 0;/* 无按键按下*/
}

/*****************************************************************************
** 函数名称: static  void  AppTask1 (void *p_arg)
** 功能描述: 任务1从消息队列获取消息并通过串口输出，LCD上显示
** 入口参数: p_arg
** 出口参数: 无
*****************************************************************************/
void  AppTask1 (void *p_arg)
{
	 u8 key;
	while(1)
	{
		key=KEY_Scan();   /*扫描按键*/
				if(key!=0)
				{
				OSMboxPost(MyCommMbox,(void*)key);/*发送邮箱消息*/
				printf("已发送信息到邮箱 ");
				}
		OSTimeDly(200);
	}		 
}
/*****************************************************************************
** 函数名称: static  void  AppTask1 (void *p_arg)
** 功能描述: 任务2接收邮箱信息并通过串口输出
** 入口参数: p_arg
** 出口参数: 无
*****************************************************************************/
void  AppTask2 (void *p_arg)
{
	INT8U err,key;
	while(1)
	{
			key=(u32)OSMboxPend(MyCommMbox,100,&err); /*获取邮箱消息*/
			if(err == OS_NO_ERR)
			{
			printf("接收邮箱信息成功！按键值为：");
			USART_SendData(USART2, key+0x30);   
				while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
				printf("\n");
  		GPIOC->ODR|=(1<<0);	     /*蜂鸣器响一声*/
						Delay_ms(200);	
			GPIOC->ODR&=~(1<<0);	
			}
			OSTimeDly(200);
 	}
}

/*****************************************************************************
** 函数名称: static  void  AppTask1 (void *p_arg)
** 功能描述: 任务3一次性任务，初始化后显示一行信息，即删除任务,任务对片上外设的操作均要在创建的任务中实现，不能在初始化时操作。
** 入口参数: 无
** 出口参数: 无
*****************************************************************************/
void  AppTask3 (void *p_arg)
{
	u8 i=0;
	printf("\n南京航空航天大学计算机科学与技术学院uCOSII消息邮箱实验：\n");	
	for(i=0;i<3;i++){
	GPIOC->ODR|=(1<<0);
	OSTimeDly(200);
	GPIOC->ODR&=~(1<<0);
	OSTimeDly(150);}
	OSTaskDel(OS_PRIO_SELF); /*删除任务，即本任务仅用一次*/
}
	

/******************* NUAA****/
