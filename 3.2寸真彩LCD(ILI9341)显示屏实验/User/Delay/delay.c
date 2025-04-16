#include "delay.h"



/*********************************************
*函数名称：void Delay_ms(void)
*
*入口参数：N  ms
*
*出口参数：无
*
*功能说明：简单延时N ms
**********************************************/
void Delay_ms(uint16_t N)
{
	uint32_t i;
	for (i=0;i<(8000*N);i++);
}



/*---LCD延时函数 10MS*nCount-----*/
void Delay_10ms(uint32_t nCount)
{
	volatile int i;	 	
	for (i=0;i<nCount*100;i++);
}
