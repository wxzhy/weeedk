/**************NUAA_CM3_107ʵ�鿪����*********************************
������ Descriptions:        һЩ�������ʼ������																	 
*********************************************************************************/ 

#include "hw_config.h"
#define SampleNum 10			/*ÿ��ͨ������40��*/
#define ChannelNum 5			/*��5��ͨ��:0��chan3,1=chan6,2=chan7,3=chan16,4-chan17   */
#define ADC_DR_Address    ((u32)0x4001244C)  /*0x4001244CΪADC_DR�ĵ�ַ���������ݼĴ�����ַ��*/
#define DAC_DHR12RD_Address  ((u32) 0x40007420);/* DAC���ݼĴ�����ַ*/
u16 ADC_ConvertedValue[SampleNum][ChannelNum];  /*ADCת�������ű�������ַ��*/
extern u16 value;

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


/*********************************************************************************
	*�������ƣ�voidADC_Mode_config(void)
	*
	*��ڲ�������
	*
	*���ڲ�������
	*
	*����˵��������ADC1�Ĺ���ģʽΪDMAģʽ 
*********************************************************************************/

 void ADC_Mode_Config(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��DMA1ʱ��
	/* DMA channel1 configuration */
  DMA_DeInit(DMA1_Channel1);  //ָ��DMAͨ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC_DR_Address;//����DMA�����ַADC_DR_Adress
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	//����DMA�ڴ��ַ��ADCת�����ֱ�ӷ���õ�ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //����Ϊ����Ϊ���ݴ������Դ
  DMA_InitStructure.DMA_BufferSize = SampleNum*ChannelNum;	//DMA����������ΪSampleNum*ChannelNum��
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//DMA�������ַ����
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ����
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);  //ʹ��DMAͨ��

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//ʹ��ADC1ʱ��
     
  /* ADC1 configuration */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ʹ�ö���ģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;			 //ɨ��ģʽ������ͨ������ʹ��
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //����ת��ģʽ��������Ӵ�����
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�����ⲿ����
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ʹ�������Ҷ���
  ADC_InitStructure.ADC_NbrOfChannel = ChannelNum;  // ChannelNum��ͨ��ת��ͨ��
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel11 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5); //ͨ��3����λ������������55.5��ʱ������
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_55Cycles5); //ͨ��6��PT100����������55.5��ʱ������
  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 3, ADC_SampleTime_55Cycles5); //ͨ��7��������ѹ����������55.5��ʱ������
  ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 4, ADC_SampleTime_55Cycles5); //ͨ��16���ڲ��¶ȣ���������55.5��ʱ������
  ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 5, ADC_SampleTime_55Cycles5); //ͨ��17����׼��ѹ����������55.5��ʱ������

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);	 //ʹ��ADC��DMA
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE); //ʹ��ADC1

	ADC_TempSensorVrefintCmd(ENABLE);			 					 /*ʹ���¶ȴ��������ڲ��ο���ѹͨ��*/	

  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
     
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //��ʼת��
}

extern uint32_t DAC_DHR12R2_Address ; 
extern uint32_t DAC2Sine12bit[32];

/**********************************************
*�������ƣ�void DAC_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����DAC��ʼ��
*********************************************/
void DAC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);		   

	/*-----------DAC�˿����� �������ģʽ-------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	 //PA5:DAC2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA ,&GPIO_InitStructure);
	DAC_DeInit();									  //��ԭ����ʼ״̬

	/* DAC ͨ��2���� */
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T3_TRGO;		       //ѡ��ʱ��3���ⲿ����Դ
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	   //�Զ��岨�β���
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;	   //ʧ���������
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);					   //DAC��ʼ��
	DAC_Cmd(DAC_Channel_2,  ENABLE);								   //ʹ��DAC
	DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);					   //ͨ��2���������
	
}


/**********************************************
*�������ƣ�void TIMER_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����TIM3��ʼ��
*********************************************/
void TIM_Configuration(uint16_t f)
{

	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 ,ENABLE);
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = SystemCoreClock/1/32/f-1;//һ������32���㣬�������ʱ��ĵ�����Ƶ�ʣ��ı����ֵ�ܸı����Ҳ���Ƶ�� �������仯    
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;       //���ֵҪΪ�� ���������   	 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;   //���ֵҪΪ�� ���������
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);  //ʹ�ø����¼���Ϊ�������
	
	TIM_Cmd(TIM3, ENABLE);	   //ʹ��TIM3
}


/**********************************************
*�������ƣ�void DMA_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����DMA2��ʼ��
*********************************************/
void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2 ,ENABLE);				 //��DMA2ʱ��
	DMA_DeInit(DMA2_Channel4);								         //��DAMͨ��4 �ļĴ�����ΪĬ��ֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R2_Address;	 //DAC2  12λ�Ҷ������ݼĴ�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&DAC2Sine12bit; //������DAC2��������
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;				 //������Ϊ���ݴ������Դ
	DMA_InitStructure.DMA_BufferSize = 32;						     //DMA�����С��32�����ݣ�һ�����ڵ����Ҳ�������
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //����Ĵ�����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			 //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//����Ϊ16λ��12λDAC��
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�ڴ����ݿ�Ȳ���16λģʽ����ӦDAC12λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;			 //������ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;		 //����DMAͨ�����ȼ�Ϊ��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;			 //������Ϊ�ڴ浽�ڴ洫��

	DMA_Init(DMA2_Channel4, &DMA_InitStructure);

	DMA_Cmd(DMA2_Channel4, ENABLE);		   

	/*ʹ��DACͨ��2:PA.5���Զ����ӵ�DAC��ת����*/
	DAC_Cmd(DAC_Channel_2, ENABLE);
	
	/* Enable DMA for DAC Channel2 */
	DAC_DMACmd(DAC_Channel_2, ENABLE);				  //ʹ��DACͨ��2��DMA
}

static	uint8_t  sysclk = 72;   /*Ĭ��ϵͳʱ��Ϊ72MHz*/
static  uint8_t  fac_us=0;      /*us��ʱ������	*/
static  uint16_t fac_ms=0;      /*ms��ʱ������ */

/*********************************************************************************
	*���ƣ�void Delay_Init(void)
	*��������
	*���أ���
	*���ܣ���׼��ʱ��ʼ����ʹ��SysTick
	*��ע��SysTickʱ��Դ��HCLK/8�ṩ����ϵͳƵ��Ϊ72MHzʱ
           ��С��������Ϊ1/9MHz,����9��Ϊ1us��fac_us��9Ϊ��λ  
*********************************************************************************/

void Delay_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = sysclk/8;		    
	fac_ms=(u16)fac_us*1000;
}





/*********************************************************************************
	*���ƣ�void Delay_ms(u16 nms)
	*������nms(ms number) ������ ���ܳ���1864
	*���أ���
	*���ܣ����뼶��ʱ
	*��ע��SysTick->LOADΪ24λ�Ĵ���,���������ʱС�ڻ����1864ms��
*********************************************************************************/
void Delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;/*ʱ�����(SysTick->LOADΪ24bit)*/
	SysTick->VAL =0x00;           /*��ռ����� */
	SysTick->CTRL=0x01 ;          /*��ʼ����  */  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16))); /*�ȴ�ʱ�䵽��   */
	SysTick->CTRL=0x00;       /*�رռ�����	 */
	SysTick->VAL =0X00;       /*��ռ�����	*/  	    
} 


/*********************************************************************************
	*���ƣ�void Delay_us(u32 nus)
	*������us(number us) ��ʱ΢���� ���1864135
	*���أ���
	*����: ʵ��΢�뼶��ʱ�������ʱ1864ms
	*��ע��������������ͨ���Ĵ������õģ�û�ж�Ӧ�Ĺ̼��⺯�����ο�misc.h
*********************************************************************************/		    								   
void Delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; /*ʱ����� fac_us=9,��9�ξ���1us	*/  		 
	SysTick->VAL=0x00;        /*��ռ����� */
	SysTick->CTRL=0x01 ;      /*��ʼ���� */	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));/*�ȴ�ʱ�䵽��*/   
	SysTick->CTRL=0x00;       /*�رռ�����	*/
	SysTick->VAL =0X00;       /*��ռ�����	*/
}

/*---LCD��ʱ���� 10MS*nCount-----*/
void Delay_10ms(uint32_t nCount)
{
	volatile int i;	 	
	for (i=0;i<nCount*100;i++);
}
void Delay(void)
{
	int i,j;
	for(i=1000;i>0;i--)
		for(j=1000;j>0;j--);

}
