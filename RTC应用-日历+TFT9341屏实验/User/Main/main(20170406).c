/********************************************************
1.本实验硬件平台：NUAA_CM3_107实验开发板WEEEDK，MCU：STM32F107VCT6
               
2.开发和实验环境：MDK-ARM Keil V5.15及其以上版本
3.例程使用的STM32F10x固件库V3.5.0
4.本实验例程现象说明内容：  
  拔下JP22（它是RTC时钟引脚，不能连接，否则RTC不正常）
	采用WEEEDK NUAA_CM3_107实验开发板
		RTC应用-日历时钟实验
		初始化RTC之后，系统每秒进入RTC中断，时间到后将计算年月日时分秒，并将其显示在LCD屏上。
		可以通过KEY4设置时间，KEY1和KEY2加减时间。
	 
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
	u8 State=0;		//0:无效，1：设置年，2：设置月，3：设置日，4：设置时，5：设置分，6设置秒
u8 SetState=0;//1设置有效
int main(void)
{
	SystemInit();
	GPIO_Configuration();
	NVIC_Configuration();	
	EXTI_Configuration();
	LCD_Init();												/*  LCD初始化        */
	Welcome(); 										 		/*  显示主界面       */
	LED1(1);LED2(1);LED3(1);LED4(1);
  RTC_Init(1);
 while (1)
  {
	if(KEY)    					 /*中断判断按键*/
	{
		switch (KEY)
		{
			case 1:IncTime();/*增加时间*/break;
			case 2:DecTime();/*减少时间*/break;
			case 4:SetTime();/*设置时间*/break;			
		}	
		KEY=0;
	}		
		
	if (State!=0)
	{
	Delay_ms(300);        /*设置时闪烁时间*/                    
		RTC_ITConfig(RTC_IT_SEC, DISABLE);	      /*禁止RTC中断,进入设置*/
		GPIOD->ODR&=~(1<<2);/*让PD2＝0，LED1常亮*/
		Shan++;
		DisplayInfSet();		/*时间信息设置*/

	switch(State)  /*根据设置时间状态闪烁不同位置*/
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
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	      /*RTC中断使能*/
    if (TimeDisplay == 1)										/*RTC秒中断服务程序中置位,显示时间到*/
    {
		 DisplayInf();													/*显示时间日历*/
     TimeDisplay = 0;												/*清时间到标志，以免不到时间频繁让LCD更换显示，但实际时间没有变化，显示没有意义*/
    }
	}
  }
 }

 
 
/**************************************************
*函数名称：u32 Time_Regulate(void)
*
*入口参数：无
*
*出口参数：返回当前时间时钟计数器的值
*
*功能说明：返回用户输入的时间
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
			
		PWR_BackupAccessCmd(ENABLE);                   //允许操作后备区域
		BKP_WriteBackupRegister(BKP_DR1, 0x0);	    //把0写入BKP_DR1寄存器		
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
			Show_Str(20+16*4-8,160,WHITE,BLACK,"年",16,0);
			POINT_COLOR =YELLOW;BACK_COLOR =RED; 
			LCD_ShowNum(20+16*6-8,160,RTC_DateTimeStructure.Month,2,16);
			Show_Str(20+16*8-8,160,WHITE,BLACK,"月",16,0);
			POINT_COLOR =YELLOW;BACK_COLOR =RED; 
			LCD_ShowNum(20+16*10-8,160,RTC_DateTimeStructure.Date,2,16);
 			Show_Str(20+16*12-16,160,WHITE,BLACK,"日",16,0);
			
			Show_Str(80,190,WHITE,BLACK,"星  期",16,0);		
  switch(RTC_DateTimeStructure.Week)
		{
			case 1:Show_Str(150,190,YELLOW,RED,"一",16,0);break;
			case 2:Show_Str(150,190,YELLOW,RED,"二",16,0);break;
			case 3:Show_Str(150,190,YELLOW,RED,"三",16,0);break;
			case 4:Show_Str(150,190,YELLOW,RED,"四",16,0);break;
			case 5:Show_Str(150,190,YELLOW,RED,"五",16,0);break;
			case 6:Show_Str(150,190,YELLOW,RED,"六",16,0);break;
			case 7:Show_Str(150,190,YELLOW,RED,"天",16,0);break;
		}
		Show_Str(28,220,WHITE,BLACK,"时间:",16,0);
		LCD_ShowNum(28+16*3,220,RTC_DateTimeStructure.Hour,2,16);//显示20
		Show_Str(38+16*4,220,WHITE,BLACK,"时",16,0);
		LCD_ShowNum(28+16*6,220,RTC_DateTimeStructure.Minute,2,16);//显示20
		Show_Str(38+16*7,220,WHITE,BLACK,"分",16,0);
		LCD_ShowNum(28+16*9,220,RTC_DateTimeStructure.Second,2,16);//显示20
		Show_Str(38+16*10,220,WHITE,BLACK,"秒",16,0);
	
}
void DisplayInf(void)
{
			RTC_GetDateTime(&RTC_DateTimeStructure);
			POINT_COLOR =YELLOW;BACK_COLOR =RED; 
			LCD_ShowNum(20+16-8,160,RTC_DateTimeStructure.Year+2000,4,16);
			Show_Str(20+16*4-8,160,WHITE,BLACK,"年",16,0);
			POINT_COLOR =YELLOW;BACK_COLOR =RED; 
			LCD_ShowNum(20+16*6-8,160,RTC_DateTimeStructure.Month,2,16);
			Show_Str(20+16*8-8,160,WHITE,BLACK,"月",16,0);
			POINT_COLOR =YELLOW;BACK_COLOR =RED; 
			LCD_ShowNum(20+16*10-8,160,RTC_DateTimeStructure.Date,2,16);
 			Show_Str(20+16*12-16,160,WHITE,BLACK,"日",16,0);
			
			Show_Str(80,190,WHITE,BLACK,"星  期",16,0);		
  switch(RTC_DateTimeStructure.Week)
		{
			case 1:Show_Str(150,190,YELLOW,RED,"一",16,0);break;
			case 2:Show_Str(150,190,YELLOW,RED,"二",16,0);break;
			case 3:Show_Str(150,190,YELLOW,RED,"三",16,0);break;
			case 4:Show_Str(150,190,YELLOW,RED,"四",16,0);break;
			case 5:Show_Str(150,190,YELLOW,RED,"五",16,0);break;
			case 6:Show_Str(150,190,YELLOW,RED,"六",16,0);break;
			case 7:Show_Str(150,190,YELLOW,RED,"天",16,0);break;
		}
		Show_Str(28,220,WHITE,BLACK,"时间:",16,0);
		LCD_ShowNum(28+16*3,220,RTC_DateTimeStructure.Hour,2,16);//显示20
		Show_Str(38+16*4,220,WHITE,BLACK,"时",16,0);
		LCD_ShowNum(28+16*6,220,RTC_DateTimeStructure.Minute,2,16);//显示20
		Show_Str(38+16*7,220,WHITE,BLACK,"分",16,0);
		LCD_ShowNum(28+16*9,220,RTC_DateTimeStructure.Second,2,16);//显示20
		Show_Str(38+16*10,220,WHITE,BLACK,"秒",16,0);
	
}
void RTC_Init(u8 select)
{
  
//if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)   //判断BKP_DR1寄存器值是否为0xA5A5
	{
	Gui_StrCenter(0,110,YELLOW,BLACK,"RTC初始化，请稍候...",16,0);//居中显示	
		RTC_Configuration();       //RTC配置
		RTC_Initializes();		      //调整时间值
		if (select!=0) Time_Regulate();   //输入调整时分秒
		PWR_BackupAccessCmd(ENABLE);                   //允许操作后备区域
		RTC_Configuration();       //RTC配置
		RTC_SetDateTime(RTC_DateTimeStructure);      //初始化日历(2017年4月18日 17:00:00)		
	Gui_StrCenter(0,110,YELLOW,BLACK,"   RTC初始化完毕     ",16,0);//居中显示	
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);	    //把0xA5A5写入BKP_DR1寄存器

	}
	//else
	{
//	RCC_LSICmd(ENABLE);					/*使能内部时钟*/
//	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);	  /*选择LSI为RTC时钟源*/	
//	RCC_RTCCLKCmd(ENABLE);	                  /*RTC时钟使能*/
//	RTC_SetPrescaler(40000-1); /*设置RTC时钟分频值,频率=1Hz(1s)*/
		
/*		GPIO_SetBits(GPIOD,GPIO_Pin_2);				//LED2灭
		GPIO_SetBits(GPIOD,GPIO_Pin_3);		
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)  //检查POWER是否发生复位标志
		{
	Gui_StrCenter(0,110,YELLOW,BLACK,"      发生上电复位       ",16,0);//居中显示	
		}		
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)	 //检查PIN复位标志设置
		{
	Gui_StrCenter(0,110,YELLOW,BLACK,"      发生外部复位       ",16,0);//居中显示	
		}	
	Gui_StrCenter(0,110,YELLOW,BLACK,"      无需配置RTC        ",16,0);//居中显示	
		//RTC_WaitForSynchro();	    //等待RTC寄存器同步		
		//RTC_ITConfig(RTC_IT_SEC, ENABLE);	 //RTC中断使能	
		//RTC_WaitForLastTask();	    //等待最后对RTC寄存器的写操作完成*/
	}	
	RCC_ClearFlag();               //清除标志位
}



/***********************END OF FILE****/
