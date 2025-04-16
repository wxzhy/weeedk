/***********************************************************************
文件名称：Delay.C
功    能：定时器TIM3利用比较器1定时1秒让LED1闪烁，比较器2定时500ms，让LED2闪烁，比较器3定时200ms，让LED3闪烁，比较器4定时100ms让LED4闪烁
编写时间：2017.3.29
编 写 人：
注    意：
***********************************************************************/
#include "stm32f10x.h"

u16 CCR1_Val = 10*1000;	/*定时1000ms*/
u16 CCR2_Val = 10*500;	/*定时500ms*/
u16 CCR3_Val = 10*200;	/*定时200ms*/
u16 CCR4_Val = 10*100;	/*定时100ms*/

void TIM_Configuration(void)
{

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
								 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period =65535;
	TIM_TimeBaseStructure.TIM_Prescaler =(SystemCoreClock/10000)-1;/*预分频*/
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		
	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;						/*输出定时模式*/
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;/*输出禁止，这里仅比较，不输出*/
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;											/*比较脉冲个数*/
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	
	
	/* Output Compare Timing Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	
	/* Output Compare Timing Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	
	//TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	
	
	
	/* TIM Interrupts enable */
	TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 |TIM_IT_CC4, ENABLE);
	
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
}

/*********************************************
*函数名称：void Delayms(void)
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

