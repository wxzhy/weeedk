/***********************************************************************
�ļ����ƣ�Delay.C
��    �ܣ���ʱ��TIM1��ʱ1�룬TIM2��ʱ500ms��TIM3��ʱ200ms����ʱ����ʱ�ֱ�ʹLED1��LED2��LED3�ı�״̬����˸�� 
��дʱ�䣺2017.3.29
�� д �ˣ�
ע    �⣺
***********************************************************************/
#include "stm32f10x.h"


void TIM_Configuration(void)
{

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
							 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);/*ʹ��TIM1ʱ�ӣ���ΪTIM1��TIM8������APB2��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3, ENABLE);/*ʹ��TIM2/TIM3ʱ�ӣ���ΪTIM2~TIM7������APB1��*/

	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0	;/*�����٣�����TIM1/TIM8�߼���ʱ����������¼������㣬����TIM1��ʱ����׼ȷ���������������²�ͬ�Ķ�ʱ�����ﵽ��ͬ��ʱ*/
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 10*1000-1; //500ms
	TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock/10000-1;//0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 10*500-1; //500ms
	TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock/10000-1;//0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 10*200-1; //200ms
	TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock/10000-1;//0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	
	/* TIM Interrupts enable */
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	/* TIM1/2/3 enable counter */
	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
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

