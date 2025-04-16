/********************************************************
**--------����WEEED4.1/4.2 ��NUAA_CS_107 kit����ϵͳuC/OS-IIʵ��------------------------------------------
** File name:           main.c
** Last modified Date:  2020-3-13
** Last Version:        V2.00
** Descriptions:        ����ucos ii���ۺ�ʵ������
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

u16 value;   								/*����������������*/			
float ADC_Value=0.00;       /*ʵ��������ֵ*/                  
u8 TFlag=0;

INT8U *CommRxBuf1="\nͨ����������key_task()������Ϣ����,main_task()���������Ϣ�������Ϣ���У�����ADCTask()������ȷ���գ�����LED3\n";
INT8U *CommRxBuf2="\nͨ���жϷ������USART2_IRQHandler()������Ϣ����,main_task()���������Ϣ�������Ϣ���У�����ADCTask()������ȷ���գ�����LED4\n";
    
OS_EVENT * msg_comm;			//ͨ������Ϣ�����¼���	  
OS_EVENT * msg_key;				//������Ϣ�����¼���	  
OS_EVENT * msg_ADCDAC;		//ADC/DAC��Ϣ�����¼���	  
OS_EVENT * q_msg;			//��Ϣ����
void * MsgGrp[256];			//��Ϣ���д洢��ַ,���֧��256����Ϣ

static OS_STK START_TASK_STK[APP_TASK1_STK_SIZE];	//�����������ջ
static OS_STK AppTask1Stk[APP_TASK1_STK_SIZE];		//����1��ջ
static OS_STK AppTask2Stk[APP_TASK2_STK_SIZE];		//����2��ջ
static OS_STK AppTask3Stk[APP_TASK3_STK_SIZE];		//����3��ջ
static OS_STK AppTask4Stk[APP_TASK4_STK_SIZE];		//����3��ջ
static OS_STK AppTask5Stk[APP_TASK5_STK_SIZE];		//����3��ջ

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
** ��������: int main(void)
** ��������: ���������
** ��ڲ���: ��
** ���ڲ���: ��
*****************************************************************************/
int  main (void)
{
	GPIO_Configuration();
	LCD_Init();								/*LCD��ʼ��*/
	
	DisplayInf(); 								/*��ʾ������*/
		
	SysTick_Configuration(); 	/*ϵͳ��ʱ����ʼ��*/

	ADC_Configuration();				/*ADC��ʼ��*/
	DAC_Configuration();	   		/*DAC1��ʼ��*/  
	
	USART_Configuration();  	/*���ڳ�ʼ��*/
	NVIC_Configuration();     /*Ƕ�������жϿ�������ʼ����USART2�ж�*/

	OSInit();      						/*usos ii��ʼ��*/
	
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );/*������ʼ�����������������д���*/

	OSStart();      					/*��ʼ�������*/
}

/*��ʼ���񣺴�����Ϣ���䡢��Ϣ�����Լ���������*/
void start_task(void *pdata)
{
  OS_CPU_SR cpu_sr=0;
	msg_comm=OSMboxCreate((void*)0);		/*����ͨ�����¼���Ϊmsg_comm����Ϣ����*/
	msg_key=OSMboxCreate((void*)0);			/*�����������¼���Ϊmsg_key����Ϣ����*/
	msg_ADCDAC=OSMboxCreate((void*)0);	/*����ģ��������¼���Ϊmsg_key����Ϣ����*/
	q_msg=OSQCreate(&MsgGrp[0],256);		/*�����¼���Ϊq_msg����Ϣ����*/
	  
	OSStatInit();							/*��ʼ��ͳ������.�������ʱ1��������	*/
 	OS_ENTER_CRITICAL();			/*�����ٽ���(�޷����жϴ��)*/    
 	OSTaskCreate(ADCTask,(void *)0,(OS_STK*)&AppTask1Stk[APP_TASK1_STK_SIZE-1],APP_TASK1_PRIO);						/*��������ADCTask*/					   
 	OSTaskCreate(DACTask,(void *)0,(OS_STK*)&AppTask2Stk[APP_TASK2_STK_SIZE-1],APP_TASK2_PRIO);	 				   
	OSTaskCreate(main_task,(void *)0,(OS_STK*)&AppTask3Stk[APP_TASK3_STK_SIZE-1],APP_TASK3_PRIO);	 				   
 	OSTaskCreate(key_task,(void *)0,(OS_STK*)&AppTask4Stk[APP_TASK4_STK_SIZE-1],APP_TASK4_PRIO);	 				   
 	OSTaskCreate(CommTask,(void *)0,(OS_STK*)&AppTask5Stk[APP_TASK5_STK_SIZE-1],APP_TASK5_PRIO);	 				   
	OSTaskSuspend(START_TASK_PRIO);	/*������ʼ����*/
	OS_EXIT_CRITICAL();					/*�˳��ٽ���(���Ա��жϴ��)*/
}

/*****************************************************************************
** ��������: void  ADCTask (void *pdata)
** ��������: ͨ��A/D�任��ȡƽ��ֵ����ͨ�����䴫�ݲ�����ƽ��ֵ����ͨ����ȱ任Ϊ��ѹֵ��ʾ��LCD��Ļ��
*****************************************************************************/
void  ADCTask (void *pdata)
{
	
	while(1)
	{
		value=ReadADCAverageValue();	/*ȡA/Dת�������ƽ��ֵ*/
		OSMboxPost(msg_ADCDAC,(void*)value);/*������Ϣ*/
		ADC_Value=value*3300/4095;		/*ͨ����ȱ任ΪmV*/
		LCD_ShowNum(130,240,ADC_Value,4,16);
		GPIOD->ODR^=(1<<2);						/*LED1��˸*/
	OSTimeDly(100);		
 	}

}
/*****************************************************************************
** ��������: void  DACTask (void *pdata)
** ��������: ����ADCTaskͨ�����䴫�ݵ�ADCֵ��ͨ��DAC���
*****************************************************************************/
void  DACTask (void *pdata)
{
	u16 ADCDAC=0;
	u8 err=0;
	while(1)
	{
		ADCDAC=(u32)OSMboxPend(msg_ADCDAC,10,&err); 
		DAC_SetChannel1Data(DAC_Align_12b_R,ADCDAC);/*�����任��DAC���ݷ��Ҷ���Ĵ�����*/
 	}
}

	
/*****************************************************************************
** ��������: void  CommTask (void *pdata)
** ��������: ������Ϣ���в�����Ϣ�����������Ӧָʾ��
��*****************************************************************************/
void  CommTask (void *pdata)
{
	INT8U err;
	void *msg;
	while(1)
	{
		msg= OSQPend(q_msg, 0, &err);	//ͨ����Ϣ���л�ȡ��Ϣ
		if (err == OS_NO_ERR){
		printf("\n\r��ȡ���гɹ���%s\r\n",(INT8U *)msg);//��ȡ�ɹ���ͨ�����������Ϣ
		if(msg==CommRxBuf1) {GPIOD->ODR&=~(1<<4);GPIOD->ODR|=(1<<7);}
			else	{if(msg==CommRxBuf2) {GPIOD->ODR&=~(1<<7);GPIOD->ODR|=(1<<4);}
			else GPIOD->ODR|=(1<<4)|(1<<7);}	
												} 
		else{
			printf("\n\r��ȡʧ��\r\n");		//��ȡʧ��
				}		
 	}
}
	


/*****************************************************************************
** ��������: void  main_ask (void *pdata)
** ��������: ���а������µ���������Ϣ�������USART2�����жϷ������͵�������Ϣʱ���ж��Ǻ���������ִ����Ӧ����
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
			case 1://���ܼ�1	 
				printf("\n���¹��ܼ�1\n");
 				break; 
			case 2://���ܼ�2
				printf("\n���¹��ܼ�2\n");
				break;	
			case 3://���ܼ�3
				printf("\n���¹��ܼ�3\n");
				break;			 
			case 4://���ܼ�4
				printf("\n���¹��ܼ�4\n");
				break;			 
				
			}
		}  
		if(comm!=0)
		{
		OSQPost(q_msg, (void *)&CommRxBuf2[0]);
	switch(comm)
			{ 
			case 'Z'://�õ����ת	 
			case 'z':
				printf("\n���ڽ��յ���Z�ַ����õ����ת\n");
				GPIOB->ODR|=(1<<8);GPIOB->ODR&=~(1<<9);
 				break; 
			case 'F'://�õ����ת
			case 'f':
				printf("\n���ڽ��յ���F�ַ����õ����ת\n");
				GPIOB->ODR|=(1<<9);GPIOB->ODR&=~(1<<8);
				break;	
			case 'S'://���ֹͣ
			case 's':
				printf("\n���ڽ��յ���S�ַ����õ��ֹͣ\n");
				GPIOB->ODR&=~(1<<8);GPIOB->ODR&=~(1<<9);
				break;			 
			case 'B'://��������
			case 'b':
				printf("\n���ڽ��յ���B�ַ����÷�������\n");
				GPIOC->ODR|=(1<<0);
				break;			 
			case 'C'://��������
			case 'c':
				printf("\n���ڽ��յ���C�ַ����÷�������\n");
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
		OSTimeDly(100);						/*ȥ���������Բ������Լ�����ʱ���򣬱������ϵͳʱ�亯��������������벻������ */
		if(KEY1==0){return 1;}
		else if(KEY2==0){return 2;}
		else if(KEY3==0){return 3; }
		else if(KEY4==0){return 4;} 
	}
	return 0;/* �ް�������*/
}   		    
/*****************************************************************************
** ��������: void  key_task (void *pdata)
** ��������: ÿ100msɨ��һ�ΰ�������ͨ����Ϣ���䷢�ͳ�ȥ
*****************************************************************************/
/*����ɨ������*/
void key_task(void *pdata)
{	
	u8 key;		    						 
	while(1)
	{
		key=KEY_Scan();   
		if(key!=0)OSMboxPost(msg_key,(void*)key);//������Ϣ
 		OSTimeDly(100);
	}
}

/******************* NUAA****/
