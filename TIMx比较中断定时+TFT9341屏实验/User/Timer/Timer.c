/***********************************************************************
�ļ����ƣ�Delay.C
��    �ܣ���ʱ��TIM3���ñȽ���1��ʱ1����LED1��˸���Ƚ���2��ʱ500ms����LED2��˸���Ƚ���3��ʱ200ms����LED3��˸���Ƚ���4��ʱ100ms��LED4��˸
��дʱ�䣺2017.3.29
�� д �ˣ�
ע    �⣺
***********************************************************************/
#include "stm32f10x.h"

u16 CCR1_Val = 10*1000;	/*��ʱ1000ms*/
u16 CCR2_Val = 10*500;	/*��ʱ500ms*/
u16 CCR3_Val = 10*200;	/*��ʱ200ms*/
u16 CCR4_Val = 10*100;	/*��ʱ100ms*/

void TIM_Configuration(void)
{

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
								 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period =65535;
	TIM_TimeBaseStructure.TIM_Prescaler =(SystemCoreClock/10000)-1;/*Ԥ��Ƶ*/
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		
	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;						/*�����ʱģʽ*/
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;/*�����ֹ��������Ƚϣ������*/
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;											/*�Ƚ��������*/
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
*�������ƣ�void Delayms(void)
*
*��ڲ�����N  ms
*
*���ڲ�������
*
*����˵��������ʱN ms
**********************************************/
void Delay_ms(uint16_t N)
{
	uint32_t i;
	for (i=0;i<(8000*N);i++);
}

