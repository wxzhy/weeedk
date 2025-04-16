/********************************************************
**--------基于WEEED4.1 的NUAA_CS_107 kit操作系统uC/OS-II实验------------------------------------------
** 实验名：ucos ii任务调度
** KEIL MDK-ARM Standard Version：5.15
** 固件库(ST) Version: V3.5
** 使用外设		: 串口，板载电机，TFT LCD屏
** ucos ii版本	：2.86
**********************************************************************************/
/**********************************************************************************
测试说明：
  任务1 采集电位器电压
	任务2 蜂鸣器控制
	任务3 PWM控制板载直流电机
	任务4 TFT LCD屏显示** Descriptions:        
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

#define us 20   //PWM周期20us（50KHz）

OS_EVENT *CommQ;
void *CommMsg[OS_MAX_QS];	//4个指针的数组


static OS_STK AppTask1Stk[APP_TASK1_STK_SIZE];		//任务1堆栈
static OS_STK AppTask2Stk[APP_TASK2_STK_SIZE];		//任务2堆栈
static OS_STK AppTask3Stk[APP_TASK3_STK_SIZE];		//任务3堆栈
static OS_STK AppTask4Stk[APP_TASK4_STK_SIZE];		//任务4堆栈

static void AppTaskCreate(void);
static void AppTask1(void *p_arg);
static void AppTask2(void *p_arg);
static void AppTask3(void *p_arg);
static void AppTask4(void *p_arg);
	u16 value;    
	float ADC_Value=0.00;                         /*内部电压值*/

/*****************************************************************************
** 函数名称: int main(void)
** 功能描述: 主函数入口
** 入口参数: 无
** 出口参数: 无
*****************************************************************************/
int  main (void)
{
	uint16_t Pulse=7200*0.5;				//占空比为50%  周期7200（1ms）
	LED_Configuration();
	LCD_Init();					//LCD初始化
	Welcome(); 		      //显示主界面
	PWM_GPIO_Init();		    		//PWM输出口PB8，初始化
	Init_TIMER(us);							//定时器4初始化us微秒为PWM周期（us可修改，默认100us）
	Init_PWM(Pulse);			    	//PWM的通道3初始化设置	
			TIM_SetCompare3(TIM4,Pulse);  //写入定时器4的通道3的占空比值
			TIM_SetCompare4(TIM4,0);      //写入定时器4的通道4的占空比值
	ADC_Configuration();
	SysTick_Configuration(); 	//系统定时器初始化
	

	OSInit();      	//usos ii初始化

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
	OSTaskCreate(   // 任务创建任务1
									AppTask1,																				//任务代码指针
									(void *)0,																			//传递给任务的参数指针
									(OS_STK *)&AppTask1Stk[APP_TASK1_STK_SIZE-1],		//任务的堆栈指针
									APP_TASK1_PRIO);									//任务1任务优先级为4


	OSTaskCreate(AppTask2,(void *)0,(OS_STK *)&AppTask2Stk[APP_TASK2_STK_SIZE-1],APP_TASK2_PRIO);	//创建任务2

	OSTaskCreate(AppTask3,(void *)0,(OS_STK *)&AppTask3Stk[APP_TASK3_STK_SIZE-1],APP_TASK3_PRIO);		//创建任务3

	OSTaskCreate(AppTask4,(void *)0,(OS_STK *)&AppTask4Stk[APP_TASK4_STK_SIZE-1],APP_TASK4_PRIO);		//创建任务4

}
/*****************************************************************************
** 函数名称: static  void  AppTask1 (void *p_arg)
** 功能描述: 任务2：采用电位器电压
** 入口参数: p_arg
** 出口参数: 无
*****************************************************************************/

static  void  AppTask1 (void *p_arg)
{
	while(1)
	{ 
		value=Read_ADC1_MultiChannel(ADC_Channel_3);     //获取采样值,没有滤波的情况
		ADC_Value = (3.3*1000/4096)*value;			   			 //将AD值转换为电压值mV
		OSTimeDlyHMSM(0,0,0,100);//延时100ms
		GPIOD->ODR^=(1<<2);//LED1闪烁
	}		 
}

/*****************************************************************************
** 函数名称: static  void  AppTask1 (void *p_arg)
** 功能描述: 任务2:判断如果电压低于1.5V＝2500，蜂鸣器响起，高于1.6V停止响
** 入口参数: p_arg
** 出口参数: 无
*****************************************************************************/
static  void  AppTask2 (void *p_arg)
{
	while(1)
	{
		if(ADC_Value<1500)  GPIO_SetBits(GPIOC,GPIO_Pin_0);//低于2.5V蜂鸣器响
		else if(ADC_Value>1600)  GPIO_ResetBits(GPIOC,GPIO_Pin_0);//高于2.6V解除蜂鸣器响
		OSTimeDlyHMSM(0,0,0,200);//延时200ms
		GPIOD->ODR^=(1<<3);//LED2 每隔0.4秒闪烁一次（定时200ms）改变LED2的状态
 	}
}

/*****************************************************************************
** 函数名称: static  void  AppTask3 (void *p_arg)
** 功能描述: 任务3:根据采集的电压大小控制PWM脉冲宽度去控制电机速度
** 入口参数: p_arg
** 出口参数: 无
*****************************************************************************/
static  void  AppTask3 (void *p_arg)
{
	u16 Pulse;

	while(1)
	{
		Pulse=(SystemCoreClock/(100*us)-1)*value/4096;
			TIM_SetCompare3(TIM4,Pulse);            //写入定时器4的通道3的占空比值
			TIM_SetCompare4(TIM4,0);                //写入定时器4的通道4的占空比值
			OSTimeDlyHMSM(0,0,0,300);//延时300ms
			GPIOD->ODR^=(1<<4);//LED3 每隔4秒闪烁一次（定时2ms）改变LED3的状态
 	}
}

/*****************************************************************************
** 函数名称: static  void  AppTask1 (void *p_arg)
** 功能描述: 任务4：LCD显示采集的电位器的数字量及计算得到的电压值
** 入口参数: p_arg
** 出口参数: 无
*****************************************************************************/
static  void  AppTask4 (void *p_arg)
{
	while(1)
	{
		POINT_COLOR=YELLOW;
		BACK_COLOR=RED;
		LCD_ShowNum(160,230, value,5,16);						/*在LCD屏上显示采样值*/
		LCD_ShowNum(160,250,ADC_Value,5,16);				/*在LCD屏上显示电压值mV*/
		OSTimeDlyHMSM(0,0,0,500);	//延时500ms
		GPIOD->ODR^=(1<<7);//LED4闪烁
 	}
}

/******************* NUAA****/
