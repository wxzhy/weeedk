#include "delay.h"
#define KEY1	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)
#define KEY2	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)
#define KEY3	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)
#define KEY4	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define RLED3 GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_4)
#define RLED4 GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_7)


static  uint8_t  fac_us=0;      /*us延时倍乘数	*/
static  uint16_t fac_ms=0;      /*ms延时倍乘数 */

/*********************************************************************************
	*名称：void Delay_Init(void)
	*参数：无
	*返回：无
	*功能：基准延时初始化，使用SysTick
	*备注：SysTick时钟源由HCLK/8提供，当系统频率为72MHz时
           最小计数周期为1/9MHz,计满9次为1us，fac_us以9为单位  
*********************************************************************************/

void DelayOnSysTick_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SystemCoreClock/8000000;		   	/* us个数变量*/ 
	fac_ms=(u16)fac_us*1000;							/* ms个数变量*/
}


/*********************************************************************************
	*名称：void Delay_ms(u16 nms)
	*参数：nms(ms number) 毫秒数 不能超过1864
	*返回：无
	*功能：毫秒级延时
	*备注：SysTick->LOAD为24位寄存器,所以最大延时小于或等于1864ms，
*********************************************************************************/
void Delay_ms(u16 nms)
{	 		  	  
	SysTick->LOAD=(u32)nms*fac_ms;/*时间加载(SysTick->LOAD为24bit)*/
	SysTick->VAL =0x00;           /*清空计数器 */
	SysTick->CTRL=0x01 ;          /*开始倒数  */  
	while(1)
	{
		if ((KEY1==0)||(KEY2==0)||(KEY3==0)||(KEY4==0)) break;
		if((SysTick->CTRL)&(1<<16))  break;
	}
	SysTick->CTRL=0x00;       /*关闭计数器	 */
	SysTick->VAL =0X00;       /*清空计数器	*/  	    
} 


/*********************************************************************************
	*名称：void Delay_us(u32 nus)
	*参数：us(number us) 延时微秒数 最大1864135
	*返回：无
	*功能: 实现微秒级延时，最大延时1864ms
	*备注：这两个函数是通过寄存器配置的，没有对应的固件库函数，参考misc.h
*********************************************************************************/		    								   
void Delay_us(u32 nus)
{		
	
	SysTick->LOAD=nus*fac_us; /*时间加载 fac_us=9,走9次就是1us	*/  		 
	SysTick->VAL=0x00;        /*清空计数器 */
	SysTick->CTRL=0x01 ;      /*开始倒数 */	 
	while(1)
	{
		if((SysTick->CTRL)&(1<<16))  break;/*等待时间到达,控制寄存器第16位为计数到0标志*/ 
	}

	SysTick->CTRL=0x00;       /*控制寄存器清零，关闭计数器	  */
	SysTick->VAL =0X00;       /*清空计数器	*/
}

/*---LCD延时函数 10MS*nCount-----*/
void Delay_10ms(uint32_t nCount)
{
	volatile int i;	 	
	for (i=0;i<nCount*100;i++);
}
