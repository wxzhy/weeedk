/********************************************************
**--------����WEEED4.1 ��NUAA_CS_107 kit����ϵͳuC/OS-IIʵ��------------------------------------------
** ʵ������ucos ii�������
** KEIL MDK-ARM Standard Version��5.15
** �̼���(ST) Version: V3.5
** ʹ������		: ���ڣ����ص����TFT LCD��
** ucos ii�汾	��2.86
**********************************************************************************/
/**********************************************************************************
����˵����
  ����1 �ɼ���λ����ѹ
	����2 ����������
	����3 PWM���ư���ֱ�����
	����4 TFT LCD����ʾ** Descriptions:        
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

#define us 20   //PWM����20us��50KHz��

OS_EVENT *CommQ;
void *CommMsg[OS_MAX_QS];	//4��ָ�������


static OS_STK AppTask1Stk[APP_TASK1_STK_SIZE];		//����1��ջ
static OS_STK AppTask2Stk[APP_TASK2_STK_SIZE];		//����2��ջ
static OS_STK AppTask3Stk[APP_TASK3_STK_SIZE];		//����3��ջ
static OS_STK AppTask4Stk[APP_TASK4_STK_SIZE];		//����4��ջ

static void AppTaskCreate(void);
static void AppTask1(void *p_arg);
static void AppTask2(void *p_arg);
static void AppTask3(void *p_arg);
static void AppTask4(void *p_arg);
	u16 value;    
	float ADC_Value=0.00;                         /*�ڲ���ѹֵ*/

/*****************************************************************************
** ��������: int main(void)
** ��������: ���������
** ��ڲ���: ��
** ���ڲ���: ��
*****************************************************************************/
int  main (void)
{
	uint16_t Pulse=7200*0.5;				//ռ�ձ�Ϊ50%  ����7200��1ms��
	LED_Configuration();
	LCD_Init();					//LCD��ʼ��
	Welcome(); 		      //��ʾ������
	PWM_GPIO_Init();		    		//PWM�����PB8����ʼ��
	Init_TIMER(us);							//��ʱ��4��ʼ��us΢��ΪPWM���ڣ�us���޸ģ�Ĭ��100us��
	Init_PWM(Pulse);			    	//PWM��ͨ��3��ʼ������	
			TIM_SetCompare3(TIM4,Pulse);  //д�붨ʱ��4��ͨ��3��ռ�ձ�ֵ
			TIM_SetCompare4(TIM4,0);      //д�붨ʱ��4��ͨ��4��ռ�ձ�ֵ
	ADC_Configuration();
	SysTick_Configuration(); 	//ϵͳ��ʱ����ʼ��
	

	OSInit();      	//usos ii��ʼ��

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
	OSTaskCreate(   // ���񴴽�����1
									AppTask1,																				//�������ָ��
									(void *)0,																			//���ݸ�����Ĳ���ָ��
									(OS_STK *)&AppTask1Stk[APP_TASK1_STK_SIZE-1],		//����Ķ�ջָ��
									APP_TASK1_PRIO);									//����1�������ȼ�Ϊ4


	OSTaskCreate(AppTask2,(void *)0,(OS_STK *)&AppTask2Stk[APP_TASK2_STK_SIZE-1],APP_TASK2_PRIO);	//��������2

	OSTaskCreate(AppTask3,(void *)0,(OS_STK *)&AppTask3Stk[APP_TASK3_STK_SIZE-1],APP_TASK3_PRIO);		//��������3

	OSTaskCreate(AppTask4,(void *)0,(OS_STK *)&AppTask4Stk[APP_TASK4_STK_SIZE-1],APP_TASK4_PRIO);		//��������4

}
/*****************************************************************************
** ��������: static  void  AppTask1 (void *p_arg)
** ��������: ����2�����õ�λ����ѹ
** ��ڲ���: p_arg
** ���ڲ���: ��
*****************************************************************************/

static  void  AppTask1 (void *p_arg)
{
	while(1)
	{ 
		value=Read_ADC1_MultiChannel(ADC_Channel_3);     //��ȡ����ֵ,û���˲������
		ADC_Value = (3.3*1000/4096)*value;			   			 //��ADֵת��Ϊ��ѹֵmV
		OSTimeDlyHMSM(0,0,0,100);//��ʱ100ms
		GPIOD->ODR^=(1<<2);//LED1��˸
	}		 
}

/*****************************************************************************
** ��������: static  void  AppTask1 (void *p_arg)
** ��������: ����2:�ж������ѹ����1.5V��2500�����������𣬸���1.6Vֹͣ��
** ��ڲ���: p_arg
** ���ڲ���: ��
*****************************************************************************/
static  void  AppTask2 (void *p_arg)
{
	while(1)
	{
		if(ADC_Value<1500)  GPIO_SetBits(GPIOC,GPIO_Pin_0);//����2.5V��������
		else if(ADC_Value>1600)  GPIO_ResetBits(GPIOC,GPIO_Pin_0);//����2.6V�����������
		OSTimeDlyHMSM(0,0,0,200);//��ʱ200ms
		GPIOD->ODR^=(1<<3);//LED2 ÿ��0.4����˸һ�Σ���ʱ200ms���ı�LED2��״̬
 	}
}

/*****************************************************************************
** ��������: static  void  AppTask3 (void *p_arg)
** ��������: ����3:���ݲɼ��ĵ�ѹ��С����PWM������ȥ���Ƶ���ٶ�
** ��ڲ���: p_arg
** ���ڲ���: ��
*****************************************************************************/
static  void  AppTask3 (void *p_arg)
{
	u16 Pulse;

	while(1)
	{
		Pulse=(SystemCoreClock/(100*us)-1)*value/4096;
			TIM_SetCompare3(TIM4,Pulse);            //д�붨ʱ��4��ͨ��3��ռ�ձ�ֵ
			TIM_SetCompare4(TIM4,0);                //д�붨ʱ��4��ͨ��4��ռ�ձ�ֵ
			OSTimeDlyHMSM(0,0,0,300);//��ʱ300ms
			GPIOD->ODR^=(1<<4);//LED3 ÿ��4����˸һ�Σ���ʱ2ms���ı�LED3��״̬
 	}
}

/*****************************************************************************
** ��������: static  void  AppTask1 (void *p_arg)
** ��������: ����4��LCD��ʾ�ɼ��ĵ�λ����������������õ��ĵ�ѹֵ
** ��ڲ���: p_arg
** ���ڲ���: ��
*****************************************************************************/
static  void  AppTask4 (void *p_arg)
{
	while(1)
	{
		POINT_COLOR=YELLOW;
		BACK_COLOR=RED;
		LCD_ShowNum(160,230, value,5,16);						/*��LCD������ʾ����ֵ*/
		LCD_ShowNum(160,250,ADC_Value,5,16);				/*��LCD������ʾ��ѹֵmV*/
		OSTimeDlyHMSM(0,0,0,500);	//��ʱ500ms
		GPIOD->ODR^=(1<<7);//LED4��˸
 	}
}

/******************* NUAA****/
