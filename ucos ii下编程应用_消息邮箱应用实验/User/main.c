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

OS_EVENT *MyCommMbox;
void *CommMsg[OS_MAX_QS];	//4��ָ�������


static OS_STK AppTask1Stk[APP_TASK1_STK_SIZE];		//����1��ջ
static OS_STK AppTask2Stk[APP_TASK2_STK_SIZE];		//����2��ջ
static OS_STK AppTask3Stk[APP_TASK3_STK_SIZE];		//����3��ջ

void AppTaskCreate(void);
void AppTask1(void *p_arg);
void AppTask2(void *p_arg);
void AppTask3(void *p_arg);

/*****************************************************************************
** ��������: int main(void)
** ��������: ���������
** ��ڲ���: ��
** ���ڲ���: ��
*****************************************************************************/
int  main (void)
{
	GPIO_Configuration();
	LCD_Init();								/*LCD��ʼ��*/
	
	Welcome(); 								/*��ʾ������*/
		
	SysTick_Configuration(); 	/*ϵͳ��ʱ����ʼ��*/
	
	USART_Configuration();  	/*���ڳ�ʼ��*/
	
	OSInit();      						/*usos ii��ʼ��*/
	
	MyCommMbox=OSMboxCreate((void*)0);		/*������Ϣ����*/

	AppTaskCreate();					/*��������*/


	OSStart();      					/*��ʼ�������*/
}

/*****************************************************************************
** ��������: static  void  AppTaskCreate(void)
** ��������: ��������
** ��ڲ���: ��
** ���ڲ���: ��
*****************************************************************************/
void  AppTaskCreate(void)
{
OSTaskCreate(   /* ���񴴽�����1*/
									AppTask1,																				/*�������ָ��*/
									(void *)0,																			/*���ݸ�����Ĳ���ָ��*/
									(OS_STK *)&AppTask1Stk[APP_TASK1_STK_SIZE-1],		/*����Ķ�ջָ��*/
									APP_TASK1_PRIO																/*����1�������ȼ�Ϊ4*/
						);			
		OSTaskCreate(AppTask2,(void *)0,(OS_STK *)&AppTask2Stk[APP_TASK2_STK_SIZE-1],APP_TASK2_PRIO);		/*��������2*/
		OSTaskCreate(AppTask3,(void *)0,(OS_STK *)&AppTask3Stk[APP_TASK3_STK_SIZE-1],APP_TASK3_PRIO);		/*��������3*/
}
u8 KEY_Scan()
{	 
	if(KEY1==0||KEY2==0||KEY3==0||KEY4==0)
	{
		OSTimeDly(50);						/*ȥ���������Բ������Լ�����ʱ���򣬱������ϵͳʱ�亯��������������벻������ */
		if(KEY1==0){GPIOD->ODR&=~(1<<2);return 1;}
		else if(KEY2==0){GPIOD->ODR&=~(1<<3);return 2;}
		else if(KEY3==0){GPIOD->ODR&=~(1<<4);return 3; }
		else if(KEY4==0){GPIOD->ODR&=~(1<<7);return 4;} 
	}
	else GPIOD->ODR|=(1<<2)|(1<<3)|(1<<4)|(1<<7);
	return 0;/* �ް�������*/
}

/*****************************************************************************
** ��������: static  void  AppTask1 (void *p_arg)
** ��������: ����1����Ϣ���л�ȡ��Ϣ��ͨ�����������LCD����ʾ
** ��ڲ���: p_arg
** ���ڲ���: ��
*****************************************************************************/
void  AppTask1 (void *p_arg)
{
	 u8 key;
	while(1)
	{
		key=KEY_Scan();   /*ɨ�谴��*/
				if(key!=0)
				{
				OSMboxPost(MyCommMbox,(void*)key);/*����������Ϣ*/
				printf("�ѷ�����Ϣ������ ");
				}
		OSTimeDly(200);
	}		 
}
/*****************************************************************************
** ��������: static  void  AppTask1 (void *p_arg)
** ��������: ����2����������Ϣ��ͨ���������
** ��ڲ���: p_arg
** ���ڲ���: ��
*****************************************************************************/
void  AppTask2 (void *p_arg)
{
	INT8U err,key;
	while(1)
	{
			key=(u32)OSMboxPend(MyCommMbox,100,&err); /*��ȡ������Ϣ*/
			if(err == OS_NO_ERR)
			{
			printf("����������Ϣ�ɹ�������ֵΪ��");
			USART_SendData(USART2, key+0x30);   
				while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
				printf("\n");
  		GPIOC->ODR|=(1<<0);	     /*��������һ��*/
						Delay_ms(200);	
			GPIOC->ODR&=~(1<<0);	
			}
			OSTimeDly(200);
 	}
}

/*****************************************************************************
** ��������: static  void  AppTask1 (void *p_arg)
** ��������: ����3һ�������񣬳�ʼ������ʾһ����Ϣ����ɾ������,�����Ƭ������Ĳ�����Ҫ�ڴ�����������ʵ�֣������ڳ�ʼ��ʱ������
** ��ڲ���: ��
** ���ڲ���: ��
*****************************************************************************/
void  AppTask3 (void *p_arg)
{
	u8 i=0;
	printf("\n�Ͼ����պ����ѧ�������ѧ�뼼��ѧԺuCOSII��Ϣ����ʵ�飺\n");	
	for(i=0;i<3;i++){
	GPIOC->ODR|=(1<<0);
	OSTimeDly(200);
	GPIOC->ODR&=~(1<<0);
	OSTimeDly(150);}
	OSTaskDel(OS_PRIO_SELF); /*ɾ�����񣬼����������һ��*/
}
	

/******************* NUAA****/
