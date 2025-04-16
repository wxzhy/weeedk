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

OS_EVENT *CommQ;
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
	
	CommQ= OSQCreate(&CommMsg[0], 10); //������Ϣ���� ����Ϊ10
	
	AppTaskCreate();//��������

	OSStart();      //��ʼ�������
}

/*****************************************************************************
** ��������: void  AppTaskCreate(void)
** ��������: ��������
** ��ڲ���: ��
** ���ڲ���: ��
*****************************************************************************/
void  AppTaskCreate(void)
{
OSTaskCreateExt(   // ���񴴽�����1
									AppTask1,																				//�������ָ��
									(void *)0,																			//���ݸ�����Ĳ���ָ��
									(OS_STK *)&AppTask1Stk[APP_TASK1_STK_SIZE-1],		//����Ķ�ջָ��
									APP_TASK1_PRIO,																	//����1�������ȼ�Ϊ4
									OS_PROBE_TASK_ID,																//����ID������ID uc/osIIû��ʹ�ã�
									(OS_STK *)&AppTask1Stk[0],											//ջ��ָ��
									APP_TASK1_STK_SIZE, 														//��ջ��С64�ֽ�	
									(void *)0,																			//��չ���ݽṹ��ָ��
									OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);				//ʹ�ܱ������ջ��飬�����������ʱ�����ջ

		OSTaskCreateExt(AppTask2,(void *)0,(OS_STK *)&AppTask2Stk[APP_TASK2_STK_SIZE-1],APP_TASK2_PRIO,OS_PROBE_TASK_ID,(OS_STK *)&AppTask2Stk[0],
					APP_TASK2_STK_SIZE,(void *)0, OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);		//��������2

}

/*****************************************************************************
** ��������: void  AppTask1 (void *p_arg)
** ��������: ����1����Ϣ���л�ȡ��Ϣ��ͨ�����������LCD����ʾ
** ��ڲ���: p_arg
** ���ڲ���: ��
*****************************************************************************/
void  AppTask1 (void *p_arg)
{
	INT8U err;
	void *msg;
	while(1)
	{
		msg= OSQPend(CommQ, 100, &err);	//��ȡ��Ϣ
		if (err == OS_NO_ERR){
		printf("\n\r��ȡ���гɹ���%s\r\n",(INT8U *)msg);//��ȡ�ɹ�����ӡ��Ϣ
		} else{
			printf("\n\r��ȡʧ��\r\n");		//��ȡʧ��
		}		
	OSTimeDly(100);
		GPIOD->ODR^=(1<<2);//LED1��˸
 	}
}

/*****************************************************************************
** ��������: void  AppTask2 (void *p_arg)
** ��������: ����2������Ϣ����Ϣ����
** ��ڲ���: p_arg
** ���ڲ���: ��
*****************************************************************************/
INT8U *CommRxBuf="\n��Ϣ���д��͵���ϢΪ���Ͼ����պ����ѧǶ��ʽʵ�鿪����RTOSʵ��֮��Ϣ���д�����Ϣ��\n";//NUAA_CS_CM3 Kit WEEEDK";

void  AppTask2 (void *p_arg)
{
	INT8U err;
	while(1)
	{
		err= OSQPost(CommQ, (void *)&CommRxBuf[0]);
		if (err == OS_NO_ERR){
			printf("\n\r��Ϣ��������� ");//����Ϣ������Ϣ���� 	
		} else{
			printf("\n\r��������");//��Ϣ�������� 	
		}
		GPIOD->ODR^=(1<<3);//LED2��˸	
		OSTimeDlyHMSM(0,0,0,100);

	}		 
}


/******************* NUAA****/
