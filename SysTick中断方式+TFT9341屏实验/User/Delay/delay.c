#include "delay.h"



/*********************************************************************************
	*名称：void Delay_Init(void)
	*参数：nms为要定时的ms数
	*返回：无
	*功能：基准延时初始化，使用SysTick
	*备注：SysTick时钟源由HCLK/8提供，当系统频率为72MHz时
           最小计数周期为1/9MHz,计满9次为1us，fac_us以9为单位  
*********************************************************************************/

void DelayOnSysTick_Init(u16 nms)
{	
	SysTick->CTRL  &=~(1<<2);			 /*选择AHB/8时钟,等同	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);*/
	SysTick->LOAD=(u32)nms*(SystemCoreClock/8000);/*时间加载(SysTick->LOAD为24bit)*/
	SysTick->VAL =0x00;           /*清空计数器 */
  SysTick->CTRL  |=(1<<0)|(1<<1);/*使能定时中断，使能定时器*/
}



/*---LCD延时函数 1MS*nCount-----*/
void Delay_ms(u16 nCount)
{
	volatile int i;	 	
	for (i=0;i<nCount*8000;i++);
}
