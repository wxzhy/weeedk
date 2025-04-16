#include "delay.h"



/*********************************************************************************
	*���ƣ�void Delay_Init(void)
	*������nmsΪҪ��ʱ��ms��
	*���أ���
	*���ܣ���׼��ʱ��ʼ����ʹ��SysTick
	*��ע��SysTickʱ��Դ��HCLK/8�ṩ����ϵͳƵ��Ϊ72MHzʱ
           ��С��������Ϊ1/9MHz,����9��Ϊ1us��fac_us��9Ϊ��λ  
*********************************************************************************/

void DelayOnSysTick_Init(u16 nms)
{	
	SysTick->CTRL  &=~(1<<2);			 /*ѡ��AHB/8ʱ��,��ͬ	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);*/
	SysTick->LOAD=(u32)nms*(SystemCoreClock/8000);/*ʱ�����(SysTick->LOADΪ24bit)*/
	SysTick->VAL =0x00;           /*��ռ����� */
  SysTick->CTRL  |=(1<<0)|(1<<1);/*ʹ�ܶ�ʱ�жϣ�ʹ�ܶ�ʱ��*/
}



/*---LCD��ʱ���� 1MS*nCount-----*/
void Delay_ms(u16 nCount)
{
	volatile int i;	 	
	for (i=0;i<nCount*8000;i++);
}
