/**************NUAA_CM3_107ʵ�鿪����*********************************
������ Descriptions:        һЩ�������ʼ������																	 
*********************************************************************************/ 

#include "hw_config.h"

void Delay_ms(uint16_t N)
{
	uint32_t i;
	for (i=0;i<(8000*N);i++);
}

/*********************************************************************************
	*�������ƣ�void LED_Configuration(void)
	*
	*��ڲ�������
	*
	*���ڲ�������
	*
	*����˵����LED��ʼ������
*********************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED5_PIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*---------��ʼ��״̬�ĸ�LEDȫOFF------------*/
	LED1(1);
	LED2(1);
	LED3(1);
	LED4(1);
			
}

/***********************************************************************
�������ƣ�NVIC_Configuration(void)
��    �ܣ�����ж����ȼ�������
���������
���������
��дʱ�䣺
�� д �ˣ�
ע    �⣺
***********************************************************************/
void NVIC_Configuration(void)
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);  	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
/**********************************************************
*  �������Ź���ʼ��IWDG_Init
*��ڣ�Ԥ��Ƶϵͳprer,��װ����ֵrlr
*���ڣ���
*���ܣ���prerΪԤ��Ƶֵ��rlr��װֵ����IWDG
*˵����IWDG��40KHzΪ����ʱ�ӣ���˶�ʱIWDG���ʱ��
       TIWDG=4��2^IWDG_PR����1+IWDG_RLR��/40KHz		
			��IWDG_PR=prer=4,IWDG_RLR=rlr=625ʱ��TIWDG=1��
**********************************************************/

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


