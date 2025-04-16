#include "delay.h"
#define KEY1	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)
#define KEY2	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)
#define KEY3	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)
#define KEY4	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define RLED3 GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_4)
#define RLED4 GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_7)


static  uint8_t  fac_us=0;      /*us��ʱ������	*/
static  uint16_t fac_ms=0;      /*ms��ʱ������ */

/*********************************************************************************
	*���ƣ�void Delay_Init(void)
	*��������
	*���أ���
	*���ܣ���׼��ʱ��ʼ����ʹ��SysTick
	*��ע��SysTickʱ��Դ��HCLK/8�ṩ����ϵͳƵ��Ϊ72MHzʱ
           ��С��������Ϊ1/9MHz,����9��Ϊ1us��fac_us��9Ϊ��λ  
*********************************************************************************/

void DelayOnSysTick_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SystemCoreClock/8000000;		   	/* us��������*/ 
	fac_ms=(u16)fac_us*1000;							/* ms��������*/
}


/*********************************************************************************
	*���ƣ�void Delay_ms(u16 nms)
	*������nms(ms number) ������ ���ܳ���1864
	*���أ���
	*���ܣ����뼶��ʱ
	*��ע��SysTick->LOADΪ24λ�Ĵ���,���������ʱС�ڻ����1864ms��
*********************************************************************************/
void Delay_ms(u16 nms)
{	 		  	  
	SysTick->LOAD=(u32)nms*fac_ms;/*ʱ�����(SysTick->LOADΪ24bit)*/
	SysTick->VAL =0x00;           /*��ռ����� */
	SysTick->CTRL=0x01 ;          /*��ʼ����  */  
	while(1)
	{
		if ((KEY1==0)||(KEY2==0)||(KEY3==0)||(KEY4==0)) break;
		if((SysTick->CTRL)&(1<<16))  break;
	}
	SysTick->CTRL=0x00;       /*�رռ�����	 */
	SysTick->VAL =0X00;       /*��ռ�����	*/  	    
} 


/*********************************************************************************
	*���ƣ�void Delay_us(u32 nus)
	*������us(number us) ��ʱ΢���� ���1864135
	*���أ���
	*����: ʵ��΢�뼶��ʱ�������ʱ1864ms
	*��ע��������������ͨ���Ĵ������õģ�û�ж�Ӧ�Ĺ̼��⺯�����ο�misc.h
*********************************************************************************/		    								   
void Delay_us(u32 nus)
{		
	
	SysTick->LOAD=nus*fac_us; /*ʱ����� fac_us=9,��9�ξ���1us	*/  		 
	SysTick->VAL=0x00;        /*��ռ����� */
	SysTick->CTRL=0x01 ;      /*��ʼ���� */	 
	while(1)
	{
		if((SysTick->CTRL)&(1<<16))  break;/*�ȴ�ʱ�䵽��,���ƼĴ�����16λΪ������0��־*/ 
	}

	SysTick->CTRL=0x00;       /*���ƼĴ������㣬�رռ�����	  */
	SysTick->VAL =0X00;       /*��ռ�����	*/
}

/*---LCD��ʱ���� 10MS*nCount-----*/
void Delay_10ms(uint32_t nCount)
{
	volatile int i;	 	
	for (i=0;i<nCount*100;i++);
}
