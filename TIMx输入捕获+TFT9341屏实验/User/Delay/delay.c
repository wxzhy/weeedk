/***********************************************************************
�ļ����ƣ�Delay.C
��    �ܣ���ʱ��TIM2��������PA0����KEY4���µ͵�ƽ��ʱ��
��дʱ�䣺2017.3.29
�� д �ˣ�
ע    �⣺
***********************************************************************/
#include "stm32f10x.h"

void TIM2_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_ICInitTypeDef  TIM2_ICInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); /*ʹ��TIM2ʱ��*/
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);/*ʹ��GPIOAʱ��*/
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;    			 /*PA0 ����     */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				 /*PA0 �������� */ 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_0);						 
	
	/*��ʼ����ʱ��2 TIM2	 */
	TIM_TimeBaseStructure.TIM_Period = arr; 						/*�趨�������Զ���װֵ */
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 					/*Ԥ��Ƶ��   */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			 /*����ʱ�ӷָ�:TDTS = Tck_tim*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM���ϼ���ģʽ*/
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 		/*����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIM2��ʱ�������λ*/
  
	/*��ʼ��TIM2���벶�����*/
		TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1; 								/*ѡ������� IC1ӳ�䵽TI1��*/
  	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;			/*�����ز���*/
  	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	/*ӳ�䵽TI1��*/
  	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;						/*�������벻��Ƶ*/ 
  	TIM2_ICInitStructure.TIM_ICFilter = 0x00;													/*�������벻�˲�*/
  	TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	/*�жϷ����ʼ��*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 					 /*TIM2�ж�*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  /*��ռ���ȼ�2��*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  			 /*�����ȼ�0��*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					 	 /*IRQͨ����ʹ��*/
	NVIC_Init(&NVIC_InitStructure);  /*����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� */
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);/*��������ж� ,����CC1IE�����ж�*/	
	
  TIM_Cmd(TIM2,ENABLE ); 	/*ʹ�ܶ�ʱ��2*/
 
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

