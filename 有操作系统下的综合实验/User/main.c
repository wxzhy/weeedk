/********************************************************
**--------基于WEEED4.1/4.2 的NUAA_CS_107 kit操作系统uC/OS-II实验------------------------------------------
** File name:           main.c
** Last modified Date:  2020-3-13
** Last Version:        V2.00
** Descriptions:        基于ucos ii的综合实验例程
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

u16 value;   								/*采样处理后的数字量*/			
float ADC_Value=0.00;       /*实际物理量值*/                  
u8 TFlag=0;

INT8U *CommRxBuf1="\n通过按键任务key_task()发送消息邮箱,main_task()任务接收消息邮箱后发消息队列，并被ADCTask()任务正确接收，点亮LED3\n";
INT8U *CommRxBuf2="\n通过中断服务程序USART2_IRQHandler()发送消息邮箱,main_task()任务接收消息邮箱后发消息队列，并被ADCTask()任务正确接收，点亮LED4\n";
    
OS_EVENT * msg_comm;			//通信用消息邮箱事件块	  
OS_EVENT * msg_key;				//按键消息邮箱事件块	  
OS_EVENT * msg_ADCDAC;		//ADC/DAC消息邮箱事件块	  
OS_EVENT * q_msg;			//消息队列
void * MsgGrp[256];			//消息队列存储地址,最大支持256个消息

static OS_STK START_TASK_STK[APP_TASK1_STK_SIZE];	//启动主任务堆栈
static OS_STK AppTask1Stk[APP_TASK1_STK_SIZE];		//任务1堆栈
static OS_STK AppTask2Stk[APP_TASK2_STK_SIZE];		//任务2堆栈
static OS_STK AppTask3Stk[APP_TASK3_STK_SIZE];		//任务3堆栈
static OS_STK AppTask4Stk[APP_TASK4_STK_SIZE];		//任务3堆栈
static OS_STK AppTask5Stk[APP_TASK5_STK_SIZE];		//任务3堆栈

void main_task(void *pdata);
void AppTaskCreate(void);
void ADCTask(void *p_arg);
void DACTask(void *p_arg);
void CommTask(void *p_arg);
void start_task(void *pdata);	
void key_task(void *pdata);
int ReadADCAverageValue(void);
void ADC_Configuration(void);
void DAC_Configuration(void);

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
	
	DisplayInf(); 								/*显示主界面*/
		
	SysTick_Configuration(); 	/*系统定时器初始化*/

	ADC_Configuration();				/*ADC初始化*/
	DAC_Configuration();	   		/*DAC1初始化*/  
	
	USART_Configuration();  	/*串口初始化*/
	NVIC_Configuration();     /*嵌入向量中断控制器初始化，USART2中断*/

	OSInit();      						/*usos ii初始化*/
	
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );/*创建起始任务，其它任务在其中创建*/

	OSStart();      					/*开始任务调度*/
}

/*开始任务：创新消息邮箱、消息队列以及创建任务*/
void start_task(void *pdata)
{
  OS_CPU_SR cpu_sr=0;
	msg_comm=OSMboxCreate((void*)0);		/*创建通信用事件块为msg_comm的消息邮箱*/
	msg_key=OSMboxCreate((void*)0);			/*创建按键用事件块为msg_key的消息邮箱*/
	msg_ADCDAC=OSMboxCreate((void*)0);	/*创建模拟组件用事件块为msg_key的消息邮箱*/
	q_msg=OSQCreate(&MsgGrp[0],256);		/*创建事件块为q_msg的消息队列*/
	  
	OSStatInit();							/*初始化统计任务.这里会延时1秒钟左右	*/
 	OS_ENTER_CRITICAL();			/*进入临界区(无法被中断打断)*/    
 	OSTaskCreate(ADCTask,(void *)0,(OS_STK*)&AppTask1Stk[APP_TASK1_STK_SIZE-1],APP_TASK1_PRIO);						/*创建任务ADCTask*/					   
 	OSTaskCreate(DACTask,(void *)0,(OS_STK*)&AppTask2Stk[APP_TASK2_STK_SIZE-1],APP_TASK2_PRIO);	 				   
	OSTaskCreate(main_task,(void *)0,(OS_STK*)&AppTask3Stk[APP_TASK3_STK_SIZE-1],APP_TASK3_PRIO);	 				   
 	OSTaskCreate(key_task,(void *)0,(OS_STK*)&AppTask4Stk[APP_TASK4_STK_SIZE-1],APP_TASK4_PRIO);	 				   
 	OSTaskCreate(CommTask,(void *)0,(OS_STK*)&AppTask5Stk[APP_TASK5_STK_SIZE-1],APP_TASK5_PRIO);	 				   
	OSTaskSuspend(START_TASK_PRIO);	/*挂起起始任务*/
	OS_EXIT_CRITICAL();					/*退出临界区(可以被中断打断)*/
}

/*****************************************************************************
** 函数名称: void  ADCTask (void *pdata)
** 功能描述: 通过A/D变换获取平均值，并通过邮箱传递采样的平均值，并通过标度变换为电压值显示在LCD屏幕上
*****************************************************************************/
void  ADCTask (void *pdata)
{
	
	while(1)
	{
		value=ReadADCAverageValue();	/*取A/D转换结果的平均值*/
		OSMboxPost(msg_ADCDAC,(void*)value);/*发送消息*/
		ADC_Value=value*3300/4095;		/*通过标度变换为mV*/
		LCD_ShowNum(130,240,ADC_Value,4,16);
		GPIOD->ODR^=(1<<2);						/*LED1闪烁*/
	OSTimeDly(100);		
 	}

}
/*****************************************************************************
** 函数名称: void  DACTask (void *pdata)
** 功能描述: 接收ADCTask通过邮箱传递的ADC值并通过DAC输出
*****************************************************************************/
void  DACTask (void *pdata)
{
	u16 ADCDAC=0;
	u8 err=0;
	while(1)
	{
		ADCDAC=(u32)OSMboxPend(msg_ADCDAC,10,&err); 
		DAC_SetChannel1Data(DAC_Align_12b_R,ADCDAC);/*将待变换的DAC数据放右对齐寄存器中*/
 	}
}

	
/*****************************************************************************
** 函数名称: void  CommTask (void *pdata)
** 功能描述: 接收消息队列并将消息输出并点亮相应指示灯
、*****************************************************************************/
void  CommTask (void *pdata)
{
	INT8U err;
	void *msg;
	while(1)
	{
		msg= OSQPend(q_msg, 0, &err);	//通过消息队列获取消息
		if (err == OS_NO_ERR){
		printf("\n\r读取队列成功：%s\r\n",(INT8U *)msg);//读取成功，通过串口输出消息
		if(msg==CommRxBuf1) {GPIOD->ODR&=~(1<<4);GPIOD->ODR|=(1<<7);}
			else	{if(msg==CommRxBuf2) {GPIOD->ODR&=~(1<<7);GPIOD->ODR|=(1<<4);}
			else GPIOD->ODR|=(1<<4)|(1<<7);}	
												} 
		else{
			printf("\n\r读取失败\r\n");		//读取失败
				}		
 	}
}
	


/*****************************************************************************
** 函数名称: void  main_ask (void *pdata)
** 功能描述: 当有按键按下的邮箱有信息到达或有USART2串口中断发生后发送的邮箱信息时，判断是何种邮箱以执行相应操作
*****************************************************************************/
void main_task(void *pdata)
{							 
	u8 key=0,comm=0;	
	u8 err;	
 
 	while(1)
	{
		key=(u32)OSMboxPend(msg_key,10,&err); 
		comm=(u32)OSMboxPend(msg_comm,10,&err); 
		if(key!=0)
		{
		OSQPost(q_msg, (void *)&CommRxBuf1[0]);
		
			switch(key)
			{ 
			case 1://功能键1	 
				printf("\n按下功能键1\n");
 				break; 
			case 2://功能键2
				printf("\n按下功能键2\n");
				break;	
			case 3://功能键3
				printf("\n按下功能键3\n");
				break;			 
			case 4://功能键4
				printf("\n按下功能键4\n");
				break;			 
				
			}
		}  
		if(comm!=0)
		{
		OSQPost(q_msg, (void *)&CommRxBuf2[0]);
	switch(comm)
			{ 
			case 'Z'://让电机正转	 
			case 'z':
				printf("\n串口接收的是Z字符，让电机正转\n");
				GPIOB->ODR|=(1<<8);GPIOB->ODR&=~(1<<9);
 				break; 
			case 'F'://让电机反转
			case 'f':
				printf("\n串口接收的是F字符，让电机反转\n");
				GPIOB->ODR|=(1<<9);GPIOB->ODR&=~(1<<8);
				break;	
			case 'S'://电机停止
			case 's':
				printf("\n串口接收的是S字符，让电机停止\n");
				GPIOB->ODR&=~(1<<8);GPIOB->ODR&=~(1<<9);
				break;			 
			case 'B'://蜂鸣器响
			case 'b':
				printf("\n串口接收的是B字符，让蜂鸣器响\n");
				GPIOC->ODR|=(1<<0);
				break;			 
			case 'C'://蜂鸣器关
			case 'c':
				printf("\n串口接收的是C字符，让蜂鸣器关\n");
				GPIOC->ODR&=~(1<<0);	
			break;			 				
			}
		}  
	}
}		   

u8 KEY_Scan()
{	 
	if(KEY1==0||KEY2==0||KEY3==0||KEY4==0)
	{
		OSTimeDly(100);						/*去抖动：绝对不能用自己的延时程序，必须采用系统时间函数，否则后续进入不了任务 */
		if(KEY1==0){return 1;}
		else if(KEY2==0){return 2;}
		else if(KEY3==0){return 3; }
		else if(KEY4==0){return 4;} 
	}
	return 0;/* 无按键按下*/
}   		    
/*****************************************************************************
** 函数名称: void  key_task (void *pdata)
** 功能描述: 每100ms扫描一次按键，并通过消息邮箱发送出去
*****************************************************************************/
/*按键扫描任务*/
void key_task(void *pdata)
{	
	u8 key;		    						 
	while(1)
	{
		key=KEY_Scan();   
		if(key!=0)OSMboxPost(msg_key,(void*)key);//发送消息
 		OSTimeDly(100);
	}
}

/******************* NUAA****/
