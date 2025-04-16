/********************************************************
**--------����WEEED4.1 ��NUAA_CS_107 kit����ϵͳuC/OS-IIʵ��------------------------------------------
** File name:           main.c
** Last modified Date:  2017-5-1
** Last Version:        V2.00
** Descriptions:        ucos ii��Ϣ����Ӧ��
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
void *CommMsg[OS_MAX_QS];	//4��ָ�������


static OS_STK AppTask1Stk[APP_TASK1_STK_SIZE];		//����1��ջ
static OS_STK AppTask2Stk[APP_TASK2_STK_SIZE];		//����2��ջ

static void AppTaskCreate(void);
static void AppTask1(void *p_arg);
static void AppTask2(void *p_arg);

/*****************************************************************************
** ��������: int main(void)
** ��������: ���������
** ��ڲ���: ��
** ���ڲ���: ��
*****************************************************************************/
int  main (void)
{
	LED_Configuration();
	LCD_Init();					//LCD��ʼ��
	
	
	Welcome(); 		//��ʾ������
		
	SysTick_Configuration(); 	//ϵͳ��ʱ����ʼ��
	
	USART_Configuration();  	//���ڳ�ʼ��

	OSInit();      	//usos ii��ʼ��
	
	MyCommSem= OSSemCreate(0); //�����ź�������ʼΪ0��ʾ�ȴ�һ���¼������¼�����
	
	AppTaskCreate();//��������

	OSStart();      //��ʼ�������
}

/*****************************************************************************
** ��������: static  void  AppTaskCreate(void)
** ��������: ��������
** ��ڲ���: ��
** ���ڲ���: ��
*****************************************************************************/
static  void  AppTaskCreate(void)
{
	OSTaskCreate(AppTask1,(void *)0,(OS_STK *)&AppTask1Stk[APP_TASK1_STK_SIZE-1],APP_TASK1_PRIO);

	OSTaskCreate(AppTask2,(void *)0,(OS_STK *)&AppTask2Stk[APP_TASK2_STK_SIZE-1],APP_TASK2_PRIO);		//��������2

}



/*****************************************************************************
** ��������: static  void  AppTask1 (void *p_arg)
** ��������: ����1�����ź���
** ��ڲ���: p_arg
** ���ڲ���: ��
*****************************************************************************/

void  AppTask1 (void *p_arg)
{
	while(1)
	{
		if (GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)==0)  /*��KEY1����*/
		{	
		OSSemPost(MyCommSem);
		GPIOD->ODR&=~(1<<2);				/*LED1��*/	
			printf("�ѷ����ź�����\n");
		}
		else GPIOD->ODR|=(1<<2)		;/*LED1��	*/
		OSTimeDly(200);
	}		 
}
/*****************************************************************************
** ��������: static  void  AppTask1 (void *p_arg)
** ��������: ����2�����ź�����ͨ�����������LCD����ʾ
** ��ڲ���: p_arg
** ���ڲ���: ��
*****************************************************************************/
void  AppTask2 (void *p_arg)
{
	INT8U err;
	while(1)
	{
		
		OSSemPend(MyCommSem, 0, &err);	/*��ȡ��Ϣ*/
			if(err == OS_NO_ERR)
			{
			printf("�����ź����ɹ���\n");
			GPIOC->ODR|=(1<<0);	
						Delay_ms(200);	
			GPIOC->ODR&=~(1<<0);	
			}
 	}
}

/******************* NUAA****/
