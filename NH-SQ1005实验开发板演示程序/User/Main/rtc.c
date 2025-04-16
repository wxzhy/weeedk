/**
  ********************************  STM32F10x  *********************************
  * @文件名     ： rtc.c
  * @作者       ： NUAAs
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2016年08月28日
  * @摘要       ： RTC源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2016-07-18 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "rtc.h"



extern u8 RTCYear,RTCMonth,RTCDate,RTCHour,RTCMinute,RTCSecond;
/**************************************************
*函数名称：void RTC_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：RTC配置
***************************************************/
void RTC_Configuration(void)
{
	
RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE);/*使能PWR和BKP时钟*/
PWR_BackupAccessCmd(ENABLE); /*允许访问BKP备份域*/
//BKP_DeInit();	                    /*复位备份域*/

	//RCC_LSEConfig(RCC_LSE_ON); /*使用外部时钟*/		
//while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) /*等待LSE起振*/
//	{}
		
	RCC_LSICmd(ENABLE);					/*使能内部时钟*/
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);	  /*选择LSI为RTC时钟源*/	
	RCC_RTCCLKCmd(ENABLE);	                  /*RTC时钟使能*/

	RTC_WaitForSynchro();	                  		  /*等待RTC寄存器同步*/
RTC_WaitForLastTask();	           /*等待最后对RTC寄存器的写操作完成*/
RTC_ITConfig(RTC_IT_SEC, ENABLE);	      /*RTC中断使能*/
RTC_WaitForLastTask();	 /*等待最后对RTC寄存器的写操作完成*/
RTC_SetPrescaler(40000-1); /*设置RTC时钟分频值,频率=1Hz(1s)*/
//RTC_SetPrescaler(32768-1); /*设置RTC时钟分频值,频率=1Hz(1s)*/
RTC_WaitForLastTask();	  /*等待最后对RTC寄存器的写操作完成*/
}

/************************************************
函数名称 ： RTC_Initializes
功    能 ： RTC初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void RTC_Initializes(void)
{
  RTC_DateTimeTypeDef RTC_DateTimeStructure;


  PWR_BackupAccessCmd(ENABLE);                   //允许操作后备区域

    RTC_Configuration();                         //配置RTC
    RTC_SetDateTime(RTC_DateTimeStructure);      //初始化日历(由前面的Year、Month/Data/Hour/Minute/Second决定)

  RTC_ITConfig(RTC_IT_SEC, ENABLE);              //使能秒中断
  RTC_WaitForLastTask();                         //等待TRC最后一次操作完成
}

/************************************************
函数名称 ： RTC_SetDateTime
功    能 ： 设置RTC日期时间
参    数 ： RTC_DateTimeStructure --- RTC日期时间结构体
返 回 值 ： RTC_OK ------------------ 成功
            RTC_OVERFLOW ------------ 失败(越界)
作    者 ： strongerHuang
*************************************************/
void RTC_SetDateTime(RTC_DateTimeTypeDef RTC_DateTimeStructure)
{
  uint32_t num_leapyear;                         //闰年天数
  uint32_t conut_year;                           //计秒(年)
  uint32_t conut_month_day;                      //月日的秒计数


//---------------------------  计算天数  ----------------------------
                                                 //闰年数
  num_leapyear = RTC_DateTimeStructure.Year/4 + 1;
                                                 //计秒(年)
  conut_year = RTC_DateTimeStructure.Year*SEC_YEAR + num_leapyear*SEC_DAY;
  switch(RTC_DateTimeStructure.Month)
  {
    case 1:/* 一月 */
      conut_month_day = (RTC_DateTimeStructure.Date-1)*SEC_DAY;
      if(RTC_DateTimeStructure.Year%4 == 0)      //假如是闰年，需要减去一天的计数
         conut_month_day -= SEC_DAY;
      break;

    case 2:/* 二月 */
      conut_month_day = (31 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      if(RTC_DateTimeStructure.Year%4 == 0)      //假如是闰年，需要减去一天的计数
         conut_month_day -= SEC_DAY;
      break;

    case 3:/* 三月 */
      conut_month_day = (59 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;

    case 4:/* 四月 */
      conut_month_day = (90 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;

    case 5:/* 五月 */
      conut_month_day = (120 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;

    case 6:/* 六月 */
      conut_month_day = (151 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;

    case 7:/* 七月 */
      conut_month_day = (181 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;

    case 8:/* 八月 */
      conut_month_day = (212 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;

    case 9:/* 九月 */
      conut_month_day = (243 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;

    case 10:/* 十月 */
      conut_month_day = (273 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;

    case 11:/* 十一月 */
      conut_month_day = (304 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;

    case 12:/* 十二月 */
      conut_month_day = (334 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;
  }

  RTC_WaitForLastTask();
                                                 //设置计数值（更新RTC）
  RTC_SetCounter(conut_year + conut_month_day + RTC_DateTimeStructure.Hour*3600 + RTC_DateTimeStructure.Minute*60 + RTC_DateTimeStructure.Second);
                                                 //等待TRC操作完成
  RTC_WaitForLastTask();

}



/************************************************
函数名称 ： RTC_GetDateTime
功    能 ： 读取RTC时间（含日历、星期）
参    数 ： RTC_DateTimeStructure --- RTC日期时间结构体
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void RTC_GetDateTime(RTC_DateTimeTypeDef *RTC_DateTimeStructure)
{
  uint32_t num_day;                              //计数的天数
  uint32_t num_daytmp;                           //该年的天数
  uint32_t num_leapyear;                         //闰年数

  uint32_t count_value = RTC_GetCounter();       //读取RTC计数值

  num_day = count_value/SEC_DAY;                 //从0开始计算天数
  num_leapyear = num_day/(365*4+1) + 1;          //闰年数

  if(num_day%(365*4+1) == 0)                     //闰年的1月1日
    RTC_DateTimeStructure->Year = (uint8_t)((num_day - num_leapyear + 1)/365);
  else
    RTC_DateTimeStructure->Year = (num_day - num_leapyear)/365;

  if(RTC_DateTimeStructure->Year%4 == 0)
  {
    /* 闰年一年内的天数 */
    num_daytmp = num_day - RTC_DateTimeStructure->Year*365 - (num_leapyear-1);
    if(num_daytmp < 31)     /* 一月 */
    {
      RTC_DateTimeStructure->Month = 1;
      RTC_DateTimeStructure->Date = num_daytmp%31 + 1;
    }
    else if(num_daytmp < 60)/* 二月 */
    {
      RTC_DateTimeStructure->Month = 2;
      RTC_DateTimeStructure->Date = (num_daytmp - 31)%29 + 1;
    }
    else if(num_daytmp < 91)/* 三月 */
    {
      RTC_DateTimeStructure->Month = 3;
      RTC_DateTimeStructure->Date = (num_daytmp - 60)%31 + 1;
    }
    else if(num_daytmp < 121)/* 四月 */
    {
      RTC_DateTimeStructure->Month = 4;
      RTC_DateTimeStructure->Date = (num_daytmp - 91)%30 + 1;
    }
    else if(num_daytmp < 152)/* 五月 */
    {
      RTC_DateTimeStructure->Month = 5;
      RTC_DateTimeStructure->Date = (num_daytmp - 121)%31 + 1;
    }
    else if(num_daytmp < 182)/* 六月 */
    {
      RTC_DateTimeStructure->Month = 6;
      RTC_DateTimeStructure->Date = (num_daytmp - 152)%30 + 1;
    }
    else if(num_daytmp < 213)/* 七月 */
    {
      RTC_DateTimeStructure->Month = 7;
      RTC_DateTimeStructure->Date = (num_daytmp - 182)%31 + 1;
    }
    else if(num_daytmp < 244)/* 八月 */
    {
      RTC_DateTimeStructure->Month = 8;
      RTC_DateTimeStructure->Date = (num_daytmp - 213)%31 + 1;
    }
    else if(num_daytmp < 274)/* 九月 */
    {
      RTC_DateTimeStructure->Month = 9;
      RTC_DateTimeStructure->Date = (num_daytmp - 244)%30 + 1;
    }
    else if(num_daytmp < 305)/* 十月 */
    {
      RTC_DateTimeStructure->Month = 10;
      RTC_DateTimeStructure->Date = (num_daytmp - 274)%31 + 1;
    }
    else if(num_daytmp < 335)/* 十一月 */
    {
      RTC_DateTimeStructure->Month = 11;
      RTC_DateTimeStructure->Date = (num_daytmp - 305)%30 + 1;
    }
    else if(num_daytmp < 366)/* 十二月 */
    {
      RTC_DateTimeStructure->Month = 12;
      RTC_DateTimeStructure->Date = (num_daytmp - 335)%31 + 1;
    }
  }
  else
  {
    /* 平年一年内的天数 */
    num_daytmp = num_day - RTC_DateTimeStructure->Year*365 - num_leapyear ;
    if(num_daytmp < 31)
    {
      RTC_DateTimeStructure->Month = 1;
      RTC_DateTimeStructure->Date = num_daytmp%31 + 1;
    }
    else if(num_daytmp < 59)
    {
      RTC_DateTimeStructure->Month = 2;
      RTC_DateTimeStructure->Date = (num_daytmp - 31)%28 + 1;
    }
    else if(num_daytmp < 90)
    {
      RTC_DateTimeStructure->Month = 3;
      RTC_DateTimeStructure->Date = (num_daytmp - 59)%31 + 1;
    }
    else if(num_daytmp < 120)
    {
      RTC_DateTimeStructure->Month = 4;
      RTC_DateTimeStructure->Date = (num_daytmp - 90)%30 + 1;
    }
    else if(num_daytmp < 151)
    {
      RTC_DateTimeStructure->Month = 5;
      RTC_DateTimeStructure->Date = (num_daytmp - 120)%31 + 1;
    }
    else if(num_daytmp < 181)
    {
      RTC_DateTimeStructure->Month = 6;
      RTC_DateTimeStructure->Date = (num_daytmp - 151)%30 + 1;
    }
    else if(num_daytmp < 212)
    {
      RTC_DateTimeStructure->Month = 7;
      RTC_DateTimeStructure->Date = (num_daytmp - 181)%31 + 1;
    }
    else if(num_daytmp < 243)
    {
      RTC_DateTimeStructure->Month = 8;
      RTC_DateTimeStructure->Date = (num_daytmp - 212)%31 + 1;
    }
    else if(num_daytmp < 273)
    {
      RTC_DateTimeStructure->Month = 9;
      RTC_DateTimeStructure->Date = (num_daytmp - 243)%30 + 1;
    }
    else if(num_daytmp < 304)
    {
      RTC_DateTimeStructure->Month = 10;
      RTC_DateTimeStructure->Date = (num_daytmp - 273)%31 + 1;
    }
    else if(num_daytmp < 334)
    {
      RTC_DateTimeStructure->Month = 11;
      RTC_DateTimeStructure->Date = (num_daytmp - 304)%30 + 1;
    }
    else if(num_daytmp < 365)
    {
      RTC_DateTimeStructure->Month = 12;
      RTC_DateTimeStructure->Date = (num_daytmp - 334)%31 + 1;
    }
  }

  /* 星期 */
  RTC_DateTimeStructure->Week = (num_day + 5)%7 + 1;
  /* 时 */
  RTC_DateTimeStructure->Hour = (count_value / 3600) % 24;
  /* 分 */
  RTC_DateTimeStructure->Minute = (count_value % 3600) / 60;
  /* 秒 */
  RTC_DateTimeStructure->Second = count_value % 60;
}



/******* END OF FILE ****/
