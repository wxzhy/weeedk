/********************************************************
1.��ʵ��Ӳ��ƽ̨��NUAA_CM3_107ʵ�鿪����WEEEDK��MCU��STM32F107VCT6
               
2.������ʵ�黷����MDK-ARM Keil V5.15�������ϰ汾
3.����ʹ�õ�STM32F10x�̼���V3.5.0
4.��ʵ����������˵�����ݣ�  
  ����JP22������RTCʱ�����ţ��������ӣ�����RTC��������
	����WEEEDK NUAA_CM3_107ʵ�鿪����
		RTCӦ��-����ʱ��ʵ��
		��ʼ��RTC֮��ϵͳÿ�����RTC�жϣ�ʱ�䵽�󽫼���������ʱ���룬��������ʾ��LCD���ϡ�
		����ͨ��KEY4����ʱ�䣬KEY1��KEY2�Ӽ�ʱ�䡣
	 
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"
#include "rtc.h"
  RTC_DateTimeTypeDef RTC_DateTimeStructure;


u8 KEY=0;
u8 Shan=0;
u8 TimeDisplay = 0;			 
	u8 State=0;		//0:��Ч��1�������꣬2�������£�3�������գ�4������ʱ��5�����÷֣�6������
u8 SetState=0;//1������Ч
int main(void)
{
	SystemInit();
	GPIO_Configuration();
	NVIC_Configuration();	
	EXTI_Configuration();
	LCD_Init();												/*  LCD��ʼ��        */
	Welcome(); 										 		/*  ��ʾ������       */
	LED1(1);LED2(1);LED3(1);LED4(1);
  RTC_Init(1);
 while (1)
  {
	if(KEY)    					 /*�ж��жϰ���*/
	{
		switch (KEY)
		{
			case 1:IncTime();/*����ʱ��*/break;
			case 2:DecTime();/*����ʱ��*/break;
			case 4:SetTime();/*����ʱ��*/break;			
		}	
		KEY=0;
	}		
		
	if (State!=0)
	{
	Delay_ms(300);        /*����ʱ��˸ʱ��*/                    
		RTC_ITConfig(RTC_IT_SEC, DISABLE);	      /*��ֹRTC�ж�,��������*/
		GPIOD->ODR&=~(1<<2);/*��PD2��0��LED1����*/
		Shan++;
		DisplayInfSet();		/*ʱ����Ϣ����*/

	switch(State)  /*��������ʱ��״̬��˸��ͬλ��*/
		{
		case 1:if (Shan&1) LCD_ShowNum(28,160,RTC_DateTimeStructure.Year+2000,4,16);else Show_Str(28,160,YELLOW,RED,"    ",16,0);break;
		case 2:if (Shan&1) LCD_ShowNum(20+16*6-8,160,RTC_DateTimeStructure.Month,2,16);else	Show_Str(20+16*6-8,160,YELLOW,RED,"  ",16,0);break;
		case 3:if (Shan&1) LCD_ShowNum(20+16*10-8,160,RTC_DateTimeStructure.Date,2,16);else Show_Str(20+16*10-8,160,YELLOW,RED,"  ",16,0);break;
		case 4:if (Shan&1) LCD_ShowNum(28+16*3,220,RTC_DateTimeStructure.Hour,2,16);else Show_Str(28+16*3,220,YELLOW,RED,"  ",16,0);break;
		case 5:if (Shan&1) LCD_ShowNum(28+16*6,220,RTC_DateTimeStructure.Minute,2,16);else Show_Str(28+16*6,220,YELLOW,RED,"  ",16,0);break;
		case 6:if (Shan&1) LCD_ShowNum(28+16*9,220,RTC_DateTimeStructure.Second,2,16);else Show_Str(28+16*9,220,YELLOW,RED,"  ",16,0);break;
		}	
	}
	else
	{	
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	      /*RTC�ж�ʹ��*/
    if (TimeDisplay == 1)										/*RTC���жϷ����������λ,��ʾʱ�䵽*/
    {
		 DisplayInf();													/*��ʾʱ������*/
     TimeDisplay = 0;												/*��ʱ�䵽��־�����ⲻ��ʱ��Ƶ����LCD������ʾ����ʵ��ʱ��û�б仯����ʾû������*/
    }
	}
  }
 }

 
 
/**************************************************
*�������ƣ�u32 Time_Regulate(void)
*
*��ڲ�������
*
*���ڲ��������ص�ǰʱ��ʱ�Ӽ�������ֵ
*
*����˵���������û������ʱ��
***************************************************/
void Time_Regulate(void)
{

  RTC_DateTimeStructure.Year = 17;
  RTC_DateTimeStructure.Month =4;
  RTC_DateTimeStructure.Date = 3;
  RTC_DateTimeStructure.Hour = 17;
  RTC_DateTimeStructure.Minute = 59;
  RTC_DateTimeStructure.Second = 0;
}

void SetTime(void)
{
	if (State==6) 
		{State=0;State=0;
			
		PWR_BackupAccessCmd(ENABLE);                   //�������������
		BKP_WriteBackupRegister(BKP_DR1, 0x0);	    //��0д��BKP_DR1�Ĵ���		
			RTC_Init(0);
		}
	else State++;
	
}
void IncTime(void)
{
	switch(State)
	{
		case 1:RTC_DateTimeStructure.Year++;break;
		case 2:if((RTC_DateTimeStructure.Month)==12) RTC_DateTimeStructure.Month=1;else RTC_DateTimeStructure.Month++;break;		
		case 3:if((RTC_DateTimeStructure.Date)==31) RTC_DateTimeStructure.Date=1;else RTC_DateTimeStructure.Date++;break;
		case 4:if((RTC_DateTimeStructure.Hour)==24) RTC_DateTimeStructure.Hour=1;else RTC_DateTimeStructure.Hour++;break;
		case 5:if((RTC_DateTimeStructure.Minute)==24) RTC_DateTimeStructure.Minute=1;else RTC_DateTimeStructure.Minute++;break;
		case 6:if((RTC_DateTimeStructure.Second)==60) RTC_DateTimeStructure.Second=1;else RTC_DateTimeStructure.Second++;break;
	}	
}
void DecTime(void)
{
	switch(State)
	{
		case 1:RTC_DateTimeStructure.Year--;break;
		case 2:if((RTC_DateTimeStructure.Month)==1) RTC_DateTimeStructure.Month=12;else RTC_DateTimeStructure.Month--;break;
		case 3:if((RTC_DateTimeStructure.Date)==1) RTC_DateTimeStructure.Date=31;else RTC_DateTimeStructure.Date--;break;
		case 4:if((RTC_DateTimeStructure.Hour)==0) RTC_DateTimeStructure.Hour=23;else RTC_DateTimeStructure.Hour--;break;
		case 5:if((RTC_DateTimeStructure.Minute)==0) RTC_DateTimeStructure.Minute=59;else RTC_DateTimeStructure.Minute--;break;
		case 6:if((RTC_DateTimeStructure.Second)==0) RTC_DateTimeStructure.Second=59;else RTC_DateTimeStructure.Second--;break;
	}	
}
void DisplayInfSet(void)
{
			POINT_COLOR =YELLOW;BACK_COLOR =RED; 
			LCD_ShowNum(20+16-8,160,RTC_DateTimeStructure.Year+2000,4,16);
			Show_Str(20+16*4-8,160,WHITE,BLACK,"��",16,0);
			POINT_COLOR =YELLOW;BACK_COLOR =RED; 
			LCD_ShowNum(20+16*6-8,160,RTC_DateTimeStructure.Month,2,16);
			Show_Str(20+16*8-8,160,WHITE,BLACK,"��",16,0);
			POINT_COLOR =YELLOW;BACK_COLOR =RED; 
			LCD_ShowNum(20+16*10-8,160,RTC_DateTimeStructure.Date,2,16);
 			Show_Str(20+16*12-16,160,WHITE,BLACK,"��",16,0);
			
			Show_Str(80,190,WHITE,BLACK,"��  ��",16,0);		
  switch(RTC_DateTimeStructure.Week)
		{
			case 1:Show_Str(150,190,YELLOW,RED,"һ",16,0);break;
			case 2:Show_Str(150,190,YELLOW,RED,"��",16,0);break;
			case 3:Show_Str(150,190,YELLOW,RED,"��",16,0);break;
			case 4:Show_Str(150,190,YELLOW,RED,"��",16,0);break;
			case 5:Show_Str(150,190,YELLOW,RED,"��",16,0);break;
			case 6:Show_Str(150,190,YELLOW,RED,"��",16,0);break;
			case 7:Show_Str(150,190,YELLOW,RED,"��",16,0);break;
		}
		Show_Str(28,220,WHITE,BLACK,"ʱ��:",16,0);
		LCD_ShowNum(28+16*3,220,RTC_DateTimeStructure.Hour,2,16);//��ʾ20
		Show_Str(38+16*4,220,WHITE,BLACK,"ʱ",16,0);
		LCD_ShowNum(28+16*6,220,RTC_DateTimeStructure.Minute,2,16);//��ʾ20
		Show_Str(38+16*7,220,WHITE,BLACK,"��",16,0);
		LCD_ShowNum(28+16*9,220,RTC_DateTimeStructure.Second,2,16);//��ʾ20
		Show_Str(38+16*10,220,WHITE,BLACK,"��",16,0);
	
}
void DisplayInf(void)
{
			RTC_GetDateTime(&RTC_DateTimeStructure);
			POINT_COLOR =YELLOW;BACK_COLOR =RED; 
			LCD_ShowNum(20+16-8,160,RTC_DateTimeStructure.Year+2000,4,16);
			Show_Str(20+16*4-8,160,WHITE,BLACK,"��",16,0);
			POINT_COLOR =YELLOW;BACK_COLOR =RED; 
			LCD_ShowNum(20+16*6-8,160,RTC_DateTimeStructure.Month,2,16);
			Show_Str(20+16*8-8,160,WHITE,BLACK,"��",16,0);
			POINT_COLOR =YELLOW;BACK_COLOR =RED; 
			LCD_ShowNum(20+16*10-8,160,RTC_DateTimeStructure.Date,2,16);
 			Show_Str(20+16*12-16,160,WHITE,BLACK,"��",16,0);
			
			Show_Str(80,190,WHITE,BLACK,"��  ��",16,0);		
  switch(RTC_DateTimeStructure.Week)
		{
			case 1:Show_Str(150,190,YELLOW,RED,"һ",16,0);break;
			case 2:Show_Str(150,190,YELLOW,RED,"��",16,0);break;
			case 3:Show_Str(150,190,YELLOW,RED,"��",16,0);break;
			case 4:Show_Str(150,190,YELLOW,RED,"��",16,0);break;
			case 5:Show_Str(150,190,YELLOW,RED,"��",16,0);break;
			case 6:Show_Str(150,190,YELLOW,RED,"��",16,0);break;
			case 7:Show_Str(150,190,YELLOW,RED,"��",16,0);break;
		}
		Show_Str(28,220,WHITE,BLACK,"ʱ��:",16,0);
		LCD_ShowNum(28+16*3,220,RTC_DateTimeStructure.Hour,2,16);//��ʾ20
		Show_Str(38+16*4,220,WHITE,BLACK,"ʱ",16,0);
		LCD_ShowNum(28+16*6,220,RTC_DateTimeStructure.Minute,2,16);//��ʾ20
		Show_Str(38+16*7,220,WHITE,BLACK,"��",16,0);
		LCD_ShowNum(28+16*9,220,RTC_DateTimeStructure.Second,2,16);//��ʾ20
		Show_Str(38+16*10,220,WHITE,BLACK,"��",16,0);
	
}
void RTC_Init(u8 select)
{
  
//if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)   //�ж�BKP_DR1�Ĵ���ֵ�Ƿ�Ϊ0xA5A5
	{
	Gui_StrCenter(0,110,YELLOW,BLACK,"RTC��ʼ�������Ժ�...",16,0);//������ʾ	
		RTC_Configuration();       //RTC����
		RTC_Initializes();		      //����ʱ��ֵ
		if (select!=0) Time_Regulate();   //�������ʱ����
		PWR_BackupAccessCmd(ENABLE);                   //�������������
		RTC_Configuration();       //RTC����
		RTC_SetDateTime(RTC_DateTimeStructure);      //��ʼ������(2017��4��18�� 17:00:00)		
	Gui_StrCenter(0,110,YELLOW,BLACK,"   RTC��ʼ�����     ",16,0);//������ʾ	
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);	    //��0xA5A5д��BKP_DR1�Ĵ���

	}
	//else
	{
//	RCC_LSICmd(ENABLE);					/*ʹ���ڲ�ʱ��*/
//	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);	  /*ѡ��LSIΪRTCʱ��Դ*/	
//	RCC_RTCCLKCmd(ENABLE);	                  /*RTCʱ��ʹ��*/
//	RTC_SetPrescaler(40000-1); /*����RTCʱ�ӷ�Ƶֵ,Ƶ��=1Hz(1s)*/
		
/*		GPIO_SetBits(GPIOD,GPIO_Pin_2);				//LED2��
		GPIO_SetBits(GPIOD,GPIO_Pin_3);		
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)  //���POWER�Ƿ�����λ��־
		{
	Gui_StrCenter(0,110,YELLOW,BLACK,"      �����ϵ縴λ       ",16,0);//������ʾ	
		}		
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)	 //���PIN��λ��־����
		{
	Gui_StrCenter(0,110,YELLOW,BLACK,"      �����ⲿ��λ       ",16,0);//������ʾ	
		}	
	Gui_StrCenter(0,110,YELLOW,BLACK,"      ��������RTC        ",16,0);//������ʾ	
		//RTC_WaitForSynchro();	    //�ȴ�RTC�Ĵ���ͬ��		
		//RTC_ITConfig(RTC_IT_SEC, ENABLE);	 //RTC�ж�ʹ��	
		//RTC_WaitForLastTask();	    //�ȴ�����RTC�Ĵ�����д�������*/
	}	
	RCC_ClearFlag();               //�����־λ
}



/***********************END OF FILE****/
