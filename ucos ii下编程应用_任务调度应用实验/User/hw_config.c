/****************************Copyright(c)*****************************
**                      http://www.openmcu.com
**--------------File Info---------------------------------------------------------------------------------
** File name:           hw_config.c
** Last modified Date:  2012-08-27
** Last Version:        V1.00
** Descriptions:        Ӳ�������ļ�
**
**----------------------------------------------------------------------------------------------------
** Created by:          openmcu
** Created date:        2012-08-27
** Version:             V1.00
** Descriptions:        ��дʾ������
**
**----------------------------------------------------------------------------------------------------
** Modified by:         ZQJ
** Modified date:        
** Version:            
** Descriptions:        
**
*****************************************************************/ 

#include "hw_config.h" 
#include "os_cpu.h"
#include "os_cfg.h"
#include <stdio.h>

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
/***********************************************************
*�������ƣ�void ADC_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����ADC1��ʼ������ 
*************************************************************/
void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_ADC1,ENABLE);/*PA3��6��7ΪADCIN3��6��7*/
/* ����GPIO��PA3/6/7��ΪADCIN3/6/7ģ��ͨ������ˣ�Ƶ��50MHz */			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7;     /*ADC1ͨ��PA3=ADCIN3,PA6=ADIN6.PA7=ADIN7����*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                   /*�ܽ�Ƶ��50MHz*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	                    	/*ģ������ģʽ*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);

/* ��ʼ��ADC������ģʽ����ͨ��ɨ�衢����ת�������������ADC�����Ҷ��� */			
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	                /*��������ģʽ*/
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;					    					/*��ֹ��ͨ��ɨ��*/
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;									/*����ת��ģʽ��ֹ*/
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	/*�����ⲿ�������������������*/
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	            /*ADC�����Ҷ���*/
	ADC_InitStructure.ADC_NbrOfChannel = 1;	                            /*���й���ת����ADCͨ����Ϊ3��ͨ����ADCIN3��6��7��*/
	ADC_Init(ADC1, &ADC_InitStructure);
	

	ADC_Cmd(ADC1, ENABLE);	                     /*ʹ��ADC1*/
	ADC_ResetCalibration(ADC1);	                 /*ʹ��ADC1��λУ׼�Ĵ���*/
	while(ADC_GetResetCalibrationStatus(ADC1));	 /*�ȴ���λУ׼�Ĵ�������*/
	ADC_StartCalibration(ADC1);			         		 /*����ADC1У׼*/
	while(ADC_GetCalibrationStatus(ADC1));	     /*�ȴ�ADC1У׼����*/
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		 	 /*�������ת��*/
}


	int Read_ADC1_MultiChannel(u8 channNo)
{  
u16  ADC_data;
    ADC_RegularChannelConfig(ADC1, channNo, 1, ADC_SampleTime_239Cycles5 );/* ����ָ��channNoͨ����������*/
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		 		/*ʹ��ADC1 */
    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));	/*�ȴ�ADת������*/
    ADC_data=ADC_GetConversionValue(ADC1);  		  /*ȡADת�����*/
//		ADC_SoftwareStartConvCmd(ADC1, DISABLE);	 		/*�ر�ADC1*/
		return(ADC_data);                             /*����ת�����*/                                        
}       



/*****************************************************************************
** ��������: void NVIC_Configuration(void)
** ��������: NVIC��ʼ��
** ��ڲ���: ��
** ���ڲ���: ��
*****************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/*****************************************************************************
** ��������: void  SysTick_Configuration(void)
** ��������: ϵͳ��ʱ����ʼ��
** ��ڲ���: ��
** ���ڲ���: ��
*****************************************************************************/
void  SysTick_Configuration(void)
{
    RCC_ClocksTypeDef  rcc_clocks;
    INT32U         cnts;
    RCC_GetClocksFreq(&rcc_clocks);

    cnts = (INT32U)rcc_clocks.HCLK_Frequency/OS_TICKS_PER_SEC;

    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	SysTick_Config(cnts);
}

/*****************************************************************************
** ��������: void LED_Configuration(void)
** ��������: LED��ʼ��
** ��ڲ���: ��
** ���ڲ���: ��
*****************************************************************************/
void LED_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_LED|RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(LED_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/*---------��ʼ��״̬�ĸ�LEDȫOFF------------*/
	LED1(1);
	LED2(1);
	LED3(1);
	LED4(1);
//	LED5(1);
}

/*****************************************************************************
** ��������: void USART_Configuration(void)
** ��������: ����1��ʼ��	������115200
** ��ڲ���: ��
** ���ڲ���: ��
*****************************************************************************/
void USART_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	 


    /*USART1�˿�����
	  PA9 TX ����������� PA10 RX ��������ģʽ*/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    /*--------------USART1 USART2����-------------------*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);

	/*USART2�˿�����
	  PD5 TX ����������� PD6 RX ��������ģʽ*/
	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);	  //���ʹ��PA2��PA3����Ҫ����ӳ��

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 ;			
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7 ;//PD7Ϊ485�շ���������Ϊ�������
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

    /*--------------USART2����-------------------*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_Cmd(USART2, ENABLE);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//�ж�����	
}
/**************************************************
*�������ƣ�void Init_TIMER(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����TIM4��ʼ������
***************************************************/
void Init_TIMER(u16 us)
{
	TIM_TimeBaseInitTypeDef	 TIM_BaseInitStructure;			//����һ����ʱ���ṹ�����

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   	//ʹ�ܶ�ʱ��4����Ҫ����

	TIM_DeInit(TIM4);                              			//��TIM4��ʱ����ʼ��λ��λֵ

	TIM_InternalClockConfig(TIM4); 							//���� TIM4 �ڲ�ʱ��
	   
	TIM_BaseInitStructure.TIM_Period = SystemCoreClock/(100*us)-1;//��ʱus΢��,							
															//TIM_Period��TIM1_ARR��=7200�����������ϼ�����7200����������¼���
															//����ֵ���� Ҳ���� 1MS���������¼�һ��
	TIM_BaseInitStructure.TIM_Prescaler = 0;  					//�Զ���Ԥ��Ƶϵ��Ϊ0������ʱ����ʱ��Ƶ��Ϊ72M�ṩ����ʱ����ʱ��	0~65535֮��
																											//����Ԥ��Ƶ����Ƶϵ��0
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ʱ�ӷָ�Ϊ0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   
																											//TIM���ϼ���ģʽ ��0��ʼ���ϼ�����������1000����������¼�
	TIM_TimeBaseInit(TIM4, &TIM_BaseInitStructure); 		//����ָ��������ʼ��TIMʱ������Ĵ���	
      
 	TIM_ARRPreloadConfig(TIM4, ENABLE);									//ʹ��TIMx�� ARR �ϵ�Ԥװ�ؼĴ��� 

	TIM_Cmd(TIM4, ENABLE); 		          								//ʹ��TIM4
}


/**************************************************
*�������ƣ�void Init_PWM(uint16_t Dutyfactor)
*
*��ڲ�����Dutyfactor ����ռ�ձȴ�С
*
*���ڲ�������
*
*����˵������TIM4����PWMͨ����ռ�ձ�
***************************************************/
void Init_PWM(uint16_t Dutyfactor)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;					//����һ��ͨ������ṹ

	TIM_OCStructInit(&TIM_OCInitStructure);								//����ȱʡֵ

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	   	//PWM ģʽ 1 ��� 	
	TIM_OCInitStructure.TIM_Pulse = Dutyfactor; 					//����ռ�ձȣ�ռ�ձ�=(CCRx/ARR)*100%��(TIM_Pulse/TIM_Period)*100%
															  
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
																												//TIM ����Ƚϼ��Ը�   	    
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    
																												//ʹ�����״̬  ��ҪPWM�������Ҫ���д���
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);						//���ݲ�����ʼ��PWM�Ĵ���    

	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);	   	//ʹ�� TIMx�� CCR4 �ϵ�Ԥװ�ؼĴ���

    TIM_OC4Init(TIM4, &TIM_OCInitStructure);						//���ݲ�����ʼ��PWM�Ĵ���    

	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);	   	//ʹ�� TIMx�� CCR4 �ϵ�Ԥװ�ؼĴ���

    TIM_CtrlPWMOutputs(TIM4,ENABLE);  									//����TIM4 ��PWM ���Ϊʹ��  
}

/**************************************************
*�������ƣ�void PWM_GPIO_Init(uint16_t Dutyfactor)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵��������PB8��PB9Ϊ�����������	
***************************************************/
void PWM_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//����һ��GPIO�ṹ�����

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);	
															//ʹ�ܸ����˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   	    //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
	GPIO_Init(GPIOB, &GPIO_InitStructure);				   	//���˿�GPIOD���г�ʼ������
}

/*****************************************************************************
** ��������: int main(void)
** ��������: ���������
** ��ڲ���: ��
** ���ڲ���: ��
*****************************************************************************/
int fputc(int ch, FILE *f)
{
  USART_SendData(USART2, (uint8_t) ch);   
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
  return ch;
}
