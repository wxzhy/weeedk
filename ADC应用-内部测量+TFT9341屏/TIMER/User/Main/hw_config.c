
/******************************************************************
**	  金龙107开发板（V1.0）
**	  硬件配置文件
**
**	  论    坛：bbs.openmcu.com
**	  旺    宝：www.openmcu.com
**	  邮    箱：support@openmcu.com
**
**    版    本：V1.1
**	  作    者：openmcu
**	  完成日期:	2014.3.11
********************************************************************/
#include "hw_config.h" 

/**************************************************
*函数名称：void NVIC_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：NVIC配置
***************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);   	  //使用优先级分组0
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;       //指定中断源
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    // 指定响应优先级别1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/**************************************************
*函数名称：void LED_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：LED初始化配置
***************************************************/
void LED_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_LED, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(LED_PORT, &GPIO_InitStructure);

	LED1(0);   //初始化状态四个LED全OFF
	LED2(0);
	LED3(0);
	LED4(0);
}
/**************************************************
*函数名称：void TIMER_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：TIM4初始化配置
***************************************************/
void TIMER_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	/* Prescaler 预分频系数为36000-1，这样计数器时钟为72MHz/36000 = 2kHz
	   Period    设置计数溢出大小，每计2000个数就产生一个更新事件
	   这样就实现了1s闪烁一次
    */
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36000-1;                // 分频值  最大65536

	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	  //向上计数模式

	TIM_TimeBaseInitStructure.TIM_Period = 2000-1; 		              //ARR的值 最大65536

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

	TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);	    // 中断使能

 	TIM_Cmd(TIM4, ENABLE);                         //使能定时器
}



