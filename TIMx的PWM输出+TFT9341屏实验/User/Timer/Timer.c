/***********************************************************************
�ļ����ƣ�Timer.C
��    �ܣ���ʱ��TIM2��������PA0����KEY4���µ͵�ƽ��ʱ��
��дʱ�䣺2017.3.29
�� д �ˣ�
ע    �⣺
***********************************************************************/
#include "stm32f10x.h"


/**************************************************
*�������ƣ�void Init_TIMER(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����TIM4��ʼ������Ϊ��ʱ1ms��1KHz���Զ���װ
***************************************************/
void Init_TIMER(void)
{
	TIM_TimeBaseInitTypeDef	 TIM_BaseInitStructure;				//����һ����ʱ���ṹ�����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //ʹ�ܶ�ʱ��4����Ҫ����
	TIM_DeInit(TIM4);                              				//��IM2��ʱ����ʼ��λ��λֵ
	TIM_InternalClockConfig(TIM4); 												//���� TIM4 �ڲ�ʱ	   
	//TIM_BaseInitStructure.TIM_Period = 7200-1; 						//�����Զ����ؼĴ���ֵΪ���ֵ	0~65535֮��  7200/72000000=1/10000s=0.1ms��10KHz													
															//TIM_Period��TIM1_ARR��=7200�����������ϼ�����7200����������¼���
															//����ֵ���� Ҳ���� 1ms���������¼�һ��
	TIM_BaseInitStructure.TIM_Period = 720-1; 						//�����Զ����ؼĴ���ֵΪ���ֵ	0~65535֮��  7200/7200000=1/1000s=1ms��1KHz								
	TIM_BaseInitStructure.TIM_Prescaler = 0;  				//�Զ���Ԥ��Ƶϵ��Ϊ0������ʱ����ʱ��Ƶ��Ϊ72M�ṩ����ʱ����ʱ��	0~65535֮��
															//����Ԥ��Ƶ����Ƶϵ��0
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ʱ�ӷָ�Ϊ0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   
															//TIM���ϼ���ģʽ ��0��ʼ���ϼ�����������1000����������¼�
	TIM_TimeBaseInit(TIM4, &TIM_BaseInitStructure); 		//����ָ��������ʼ��TIMʱ������Ĵ���	
      
 	TIM_ARRPreloadConfig(TIM4, ENABLE);						//ʹ��TIMx�� ARR �ϵ�Ԥװ�ؼĴ��� 

	TIM_Cmd(TIM4, ENABLE); 		          //ʹ��TIM4
}


/**************************************************
*�������ƣ�void Init_PWM(uint16_t Dutyfactor)
*
*��ڲ�����Dutyfactor ����ռ�ձȴ�С
*
*���ڲ�������
*
*����˵��������PWMͨ����ռ�ձ�
***************************************************/
void Init_PWM(uint16_t Dutyfactor)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;					//����һ��ͨ������ṹ

	TIM_OCStructInit(&TIM_OCInitStructure);					//����ȱʡֵ

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	   	//PWM ģʽ 1 ��� 	
	TIM_OCInitStructure.TIM_Pulse = Dutyfactor; 			//����ռ�ձȣ�ռ�ձ�=(CCRx/ARR)*100%��(TIM_Pulse/TIM_Period)*100%
															  
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM ����Ƚϼ��Ը�   
																    
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    	//ʹ�����״̬  ��ҪPWM�������Ҫ���д���
														
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);				//���ݲ�����ʼ��PWM�Ĵ��� ͨ��3��PB8��   

	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);//ʹ�� TIMx�� CCR3 �ϵ�Ԥװ�ؼĴ���

  TIM_CtrlPWMOutputs(TIM4,ENABLE);  							//����TIM4 ��PWM ���Ϊʹ��  
}

void PWM_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//����һ��GPIO�ṹ�����

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);	
																								//ʹ�ܸ����˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9; //PB8��PB9���ţ�PB6��PB7��PB8��PB9�ֱ�ΪTIM4��Ӧ��CH1��CH2��CH3��CH4�ĸ�ͨ����PWM������ţ�����ʹ��PB8��PB9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   	  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
	GPIO_Init(GPIOB, &GPIO_InitStructure);				   			//���˿�GPIOD���г�ʼ������
}

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

