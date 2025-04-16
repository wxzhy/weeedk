/**
  ********************************  STM32F10x  *********************************
  * @�ļ���     �� rtc.c
  * @����       �� NUAAs
  * @��汾     �� V3.5.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2016��08��28��
  * @ժҪ       �� RTCԴ�ļ�
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
  2016-07-18 V1.0.0:��ʼ�汾
  ----------------------------------------------------------------------------*/
/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "rtc.h"



extern u8 RTCYear,RTCMonth,RTCDate,RTCHour,RTCMinute,RTCSecond;
/**************************************************
*�������ƣ�void RTC_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����RTC����
***************************************************/
void RTC_Configuration(void)
{
	
RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE);/*ʹ��PWR��BKPʱ��*/
PWR_BackupAccessCmd(ENABLE); /*�������BKP������*/
//BKP_DeInit();	                    /*��λ������*/

	//RCC_LSEConfig(RCC_LSE_ON); /*ʹ���ⲿʱ��*/		
//while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) /*�ȴ�LSE����*/
//	{}
		
	RCC_LSICmd(ENABLE);					/*ʹ���ڲ�ʱ��*/
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);	  /*ѡ��LSIΪRTCʱ��Դ*/	
	RCC_RTCCLKCmd(ENABLE);	                  /*RTCʱ��ʹ��*/

	RTC_WaitForSynchro();	                  		  /*�ȴ�RTC�Ĵ���ͬ��*/
RTC_WaitForLastTask();	           /*�ȴ�����RTC�Ĵ�����д�������*/
RTC_ITConfig(RTC_IT_SEC, ENABLE);	      /*RTC�ж�ʹ��*/
RTC_WaitForLastTask();	 /*�ȴ�����RTC�Ĵ�����д�������*/
RTC_SetPrescaler(40000-1); /*����RTCʱ�ӷ�Ƶֵ,Ƶ��=1Hz(1s)*/
//RTC_SetPrescaler(32768-1); /*����RTCʱ�ӷ�Ƶֵ,Ƶ��=1Hz(1s)*/
RTC_WaitForLastTask();	  /*�ȴ�����RTC�Ĵ�����д�������*/
}

/************************************************
�������� �� RTC_Initializes
��    �� �� RTC��ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� strongerHuang
*************************************************/
void RTC_Initializes(void)
{
  RTC_DateTimeTypeDef RTC_DateTimeStructure;


  PWR_BackupAccessCmd(ENABLE);                   //�������������

    RTC_Configuration();                         //����RTC
    RTC_SetDateTime(RTC_DateTimeStructure);      //��ʼ������(��ǰ���Year��Month/Data/Hour/Minute/Second����)

  RTC_ITConfig(RTC_IT_SEC, ENABLE);              //ʹ�����ж�
  RTC_WaitForLastTask();                         //�ȴ�TRC���һ�β������
}

/************************************************
�������� �� RTC_SetDateTime
��    �� �� ����RTC����ʱ��
��    �� �� RTC_DateTimeStructure --- RTC����ʱ��ṹ��
�� �� ֵ �� RTC_OK ------------------ �ɹ�
            RTC_OVERFLOW ------------ ʧ��(Խ��)
��    �� �� strongerHuang
*************************************************/
void RTC_SetDateTime(RTC_DateTimeTypeDef RTC_DateTimeStructure)
{
  uint32_t num_leapyear;                         //��������
  uint32_t conut_year;                           //����(��)
  uint32_t conut_month_day;                      //���յ������


//---------------------------  ��������  ----------------------------
                                                 //������
  num_leapyear = RTC_DateTimeStructure.Year/4 + 1;
                                                 //����(��)
  conut_year = RTC_DateTimeStructure.Year*SEC_YEAR + num_leapyear*SEC_DAY;
  switch(RTC_DateTimeStructure.Month)
  {
    case 1:/* һ�� */
      conut_month_day = (RTC_DateTimeStructure.Date-1)*SEC_DAY;
      if(RTC_DateTimeStructure.Year%4 == 0)      //���������꣬��Ҫ��ȥһ��ļ���
         conut_month_day -= SEC_DAY;
      break;

    case 2:/* ���� */
      conut_month_day = (31 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      if(RTC_DateTimeStructure.Year%4 == 0)      //���������꣬��Ҫ��ȥһ��ļ���
         conut_month_day -= SEC_DAY;
      break;

    case 3:/* ���� */
      conut_month_day = (59 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;

    case 4:/* ���� */
      conut_month_day = (90 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;

    case 5:/* ���� */
      conut_month_day = (120 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;

    case 6:/* ���� */
      conut_month_day = (151 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;

    case 7:/* ���� */
      conut_month_day = (181 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;

    case 8:/* ���� */
      conut_month_day = (212 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;

    case 9:/* ���� */
      conut_month_day = (243 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;

    case 10:/* ʮ�� */
      conut_month_day = (273 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;

    case 11:/* ʮһ�� */
      conut_month_day = (304 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;

    case 12:/* ʮ���� */
      conut_month_day = (334 + RTC_DateTimeStructure.Date - 1)*SEC_DAY;
      break;
  }

  RTC_WaitForLastTask();
                                                 //���ü���ֵ������RTC��
  RTC_SetCounter(conut_year + conut_month_day + RTC_DateTimeStructure.Hour*3600 + RTC_DateTimeStructure.Minute*60 + RTC_DateTimeStructure.Second);
                                                 //�ȴ�TRC�������
  RTC_WaitForLastTask();

}



/************************************************
�������� �� RTC_GetDateTime
��    �� �� ��ȡRTCʱ�䣨�����������ڣ�
��    �� �� RTC_DateTimeStructure --- RTC����ʱ��ṹ��
�� �� ֵ �� ��
��    �� �� strongerHuang
*************************************************/
void RTC_GetDateTime(RTC_DateTimeTypeDef *RTC_DateTimeStructure)
{
  uint32_t num_day;                              //����������
  uint32_t num_daytmp;                           //���������
  uint32_t num_leapyear;                         //������

  uint32_t count_value = RTC_GetCounter();       //��ȡRTC����ֵ

  num_day = count_value/SEC_DAY;                 //��0��ʼ��������
  num_leapyear = num_day/(365*4+1) + 1;          //������

  if(num_day%(365*4+1) == 0)                     //�����1��1��
    RTC_DateTimeStructure->Year = (uint8_t)((num_day - num_leapyear + 1)/365);
  else
    RTC_DateTimeStructure->Year = (num_day - num_leapyear)/365;

  if(RTC_DateTimeStructure->Year%4 == 0)
  {
    /* ����һ���ڵ����� */
    num_daytmp = num_day - RTC_DateTimeStructure->Year*365 - (num_leapyear-1);
    if(num_daytmp < 31)     /* һ�� */
    {
      RTC_DateTimeStructure->Month = 1;
      RTC_DateTimeStructure->Date = num_daytmp%31 + 1;
    }
    else if(num_daytmp < 60)/* ���� */
    {
      RTC_DateTimeStructure->Month = 2;
      RTC_DateTimeStructure->Date = (num_daytmp - 31)%29 + 1;
    }
    else if(num_daytmp < 91)/* ���� */
    {
      RTC_DateTimeStructure->Month = 3;
      RTC_DateTimeStructure->Date = (num_daytmp - 60)%31 + 1;
    }
    else if(num_daytmp < 121)/* ���� */
    {
      RTC_DateTimeStructure->Month = 4;
      RTC_DateTimeStructure->Date = (num_daytmp - 91)%30 + 1;
    }
    else if(num_daytmp < 152)/* ���� */
    {
      RTC_DateTimeStructure->Month = 5;
      RTC_DateTimeStructure->Date = (num_daytmp - 121)%31 + 1;
    }
    else if(num_daytmp < 182)/* ���� */
    {
      RTC_DateTimeStructure->Month = 6;
      RTC_DateTimeStructure->Date = (num_daytmp - 152)%30 + 1;
    }
    else if(num_daytmp < 213)/* ���� */
    {
      RTC_DateTimeStructure->Month = 7;
      RTC_DateTimeStructure->Date = (num_daytmp - 182)%31 + 1;
    }
    else if(num_daytmp < 244)/* ���� */
    {
      RTC_DateTimeStructure->Month = 8;
      RTC_DateTimeStructure->Date = (num_daytmp - 213)%31 + 1;
    }
    else if(num_daytmp < 274)/* ���� */
    {
      RTC_DateTimeStructure->Month = 9;
      RTC_DateTimeStructure->Date = (num_daytmp - 244)%30 + 1;
    }
    else if(num_daytmp < 305)/* ʮ�� */
    {
      RTC_DateTimeStructure->Month = 10;
      RTC_DateTimeStructure->Date = (num_daytmp - 274)%31 + 1;
    }
    else if(num_daytmp < 335)/* ʮһ�� */
    {
      RTC_DateTimeStructure->Month = 11;
      RTC_DateTimeStructure->Date = (num_daytmp - 305)%30 + 1;
    }
    else if(num_daytmp < 366)/* ʮ���� */
    {
      RTC_DateTimeStructure->Month = 12;
      RTC_DateTimeStructure->Date = (num_daytmp - 335)%31 + 1;
    }
  }
  else
  {
    /* ƽ��һ���ڵ����� */
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

  /* ���� */
  RTC_DateTimeStructure->Week = (num_day + 5)%7 + 1;
  /* ʱ */
  RTC_DateTimeStructure->Hour = (count_value / 3600) % 24;
  /* �� */
  RTC_DateTimeStructure->Minute = (count_value % 3600) / 60;
  /* �� */
  RTC_DateTimeStructure->Second = count_value % 60;
}



/******* END OF FILE ****/
