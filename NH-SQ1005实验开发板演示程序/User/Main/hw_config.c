/**************NUAA_CM3_107ʵ�鿪����*********************************
������ Descriptions:        һЩ�������ʼ������																	 
*********************************************************************************/ 

#include "hw_config.h"
#include "rtc.h"


extern uint32_t DAC_DHR12R2_Address ; 
extern uint32_t DAC2Sine12bit[32];
extern uint8_t ConnectBitCSB;
#define SampleNum 40			/*ÿ��ͨ������40��*/
#define ChannelNum 5			/*��5��ͨ��:0��chan3,1=chan6,2=chan7,3=chan16,4-chan17   */
#define ADC_DR_Address    ((u32)0x4001244C)  /*0x4001244CΪADC_DR�ĵ�ַ���������ݼĴ�����ַ��*/
u16 ADC_ConvertedValue[SampleNum][ChannelNum];  /*ADCת�������ű�������ַ��*/
extern u16 value;



////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ��
//0��û���κΰ�������
//KEY1PRES��KEY1����
//KEY2_PRES��KEY2����
//KEY3_PRES��KEY3���� 
//ע��˺�������Ӧ���ȼ�,KEY1>KEY2>KEY3>KEY4!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	 
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0|KEY4==0))
	{
		Delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY1==0)return 1;
		else if(KEY2==0)return 2;
		else if(KEY3==0)return 3; 
		else if(KEY4==0)return 4; 
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)key_up=1; 	     
	return 0;// �ް�������
}

/********************************************************************************************************************************************
@f_name: void Ultran_Init(void)
@brief:	 ������Ӳ��ģ���ʼ��
@param:	 None
@return: None
*********************************************************************************************************************************************/
void Ultran_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;       //���͵�ƽ����PA2
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1;		//���ص�ƽ����PC1
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}
/********************************************************************************************************************************************
@f_name: void Ranging(void)
@brief:	 ��������࣬��ֵ5�Σ�ʵ�� �������̷��ֲ�����̬�ıȽ��ȶ�����̬�Ĳ��ȶ���3�׷�Χ�ڱȽ��ȶ�
@param:	 float *p:��໺�����
@return: None
*********************************************************************************************************************************************/
void Ultra_Ranging(float *p)
{
	u8 i=0;
	u32 j=0;
	float Ultr_Temp=0;	
	for(i=0;i<5;i++)
	{
		GPIOA->ODR|=(1<<2);//PA2=1
		Delay_ms(10);
		GPIOA->ODR&=~(1<<2);//PA2=0//TRIG_Send=0;
		j=0;
		while(!(GPIOC->IDR&(1<<1)))
		{
			if(j>10000) break;
			Delay_us(10);
			j++;
		}
		if(j>10000) break;
		//while(!(GPIOC->IDR&(1<<1)));//PC1==0
		
	//	while(!ECHO_Reci);
//		while(ECHO_Reci)
		j=0;
		while((GPIOC->IDR&(1<<1)))
		{
			if(j>10000) break;
			Delay_us(10);
			j++;
		}
		Ultr_Temp+=340/2*j*10;//  ģ�����ɲ��3m 
		j=0;
		Delay_ms(60);//��ֹ�����źŶԻ����źŵ�Ӱ��
	}
	*p=Ultr_Temp/5/1000000; 	
		if(j>10000) ConnectBitCSB=0;
		else    ConnectBitCSB=1;
	if ((Ultr_Temp/5/1000000)>10) ConnectBitCSB=0;
	//*p=Ultr_Temp/1000000; 	
}


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

	DAC_SetChannel1Data(DAC_Align_12b_R, 0x0);  //12λ�Ҷ������ݸ�ʽ����DAC��ʼֵ�����0

	/* DAC ͨ��2���� */
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T3_TRGO;		       //ѡ��ʱ��3���ⲿ����Դ
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	   //�Զ��岨�β���
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;	   //ʧ���������
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);					   //DAC��ʼ��
	DAC_Cmd(DAC_Channel_2,  ENABLE);								   //ʹ��DAC
	DAC_SoftwareTriggerCmd(DAC_Channel_2, DISABLE);					   //ͨ��2���������
	
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
	PWM_GPIO_Init();

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
	TIM_TimeBaseStructure.TIM_Period = SystemCoreClock/1/32/f-1;//0xFF;         //�ı����ֵ�ܸı����Ҳ���Ƶ�� �������仯    
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;       //���ֵҪΪ�� ���������   	 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;   //���ֵҪΪ�� ���������
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);  //ʹ�ø����¼���Ϊ�������
	
	TIM_Cmd(TIM3, ENABLE);	   //ʹ��TIM3
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //ʹ�ܶ�ʱ��4����Ҫ����
	TIM_DeInit(TIM4);                              				//��IM2��ʱ����ʼ��λ��λֵ
	TIM_InternalClockConfig(TIM4); 												//���� TIM4 �ڲ�ʱ	   
	TIM_TimeBaseStructure.TIM_Period = 7200-1; 						//�����Զ����ؼĴ���ֵΪ���ֵ	0~65535֮��  7200/72000000=1/10000s=0.1ms��10KHz													
															//TIM_Period��TIM1_ARR��=7200�����������ϼ�����7200����������¼���
															//����ֵ���� Ҳ���� 1ms���������¼�һ��
	TIM_TimeBaseStructure.TIM_Prescaler = 0;  				//�Զ���Ԥ��Ƶϵ��Ϊ0������ʱ����ʱ��Ƶ��Ϊ72M�ṩ����ʱ����ʱ��	0~65535֮��
															//����Ԥ��Ƶ����Ƶϵ��0
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ʱ�ӷָ�Ϊ0
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
															//TIM���ϼ���ģʽ ��0��ʼ���ϼ�����������1000����������¼�
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 		//����ָ��������ʼ��TIMʱ������Ĵ���	
      
 	TIM_ARRPreloadConfig(TIM4, ENABLE);						//ʹ��TIMx�� ARR �ϵ�Ԥװ�ؼĴ��� 

	TIM_Cmd(TIM4, ENABLE); 		          //ʹ��TIM4
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
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	//�������ݿ��32λ������Ϊ16λ��12λDAC��
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;	  //�ڴ����ݿ��32λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;			 //CAN��������������ģʽ(������ʹ��)
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;		 //����DMAͨ�����ȼ�Ϊ��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;			 //������Ϊ�ڴ浽�ڴ洫��

	DMA_Init(DMA2_Channel4, &DMA_InitStructure);

	DMA_Cmd(DMA2_Channel4, ENABLE);		   

	/*ʹ��DACͨ��2:PA.5���Զ����ӵ�DAC��ת����*/
	DAC_Cmd(DAC_Channel_2, ENABLE);
	
	/* Enable DMA for DAC Channel2 */
	DAC_DMACmd(DAC_Channel_2, ENABLE);				  //ʹ��DACͨ��2��DMA
}


/*********************************************
*�������ƣ�void NVIC_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����NVIC��ʼ������
**********************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn ;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel =RTC_IRQn  ;	    	/*����EXTI��15~10��(PD11=KEY1,PD12=KEY2)���ж�����*/ 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);			  //�жϷ���3
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 		  //USART3�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);
}

/***************************************************
*�������ƣ�void EXTI_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����EXTI��ʼ������
*****************************************************/
void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	/*PB0�ⲿ�ж�����*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line0; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//PB0���ش��� 
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/*********************************************
*�������ƣ�void REMOTE_Init()
*
*��ڲ�������
*
*���ڲ�������
*
*����˵��: ������չܽ�����
**********************************************/
void REMOTE_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;					 //PB0����Ϊ���������������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			     //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		     //�ܽ�Ƶ��50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);	 //GPIOB0 �ⲿ�жϴ���
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

	GPIO_InitStructure.GPIO_Pin = LED5_PIN|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_8|GPIO_Pin_9|LED5_PIN;//PB1��PB2Ϊ˫ɫ����ܿ������ţ�1����0��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN |GPIO_Pin_4|GPIO_Pin_15; //PC4/PC15���Ƽ̵���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_8|GPIO_Pin_11;//PA1����ȷ���Ƿ���Զ�����ʾ������PA8����RTC�Ƿ�����,PA11����PWM������Ƶ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
/*    �˶�LED����ܶ�������  D0~D7  PE0~PE7     */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
/*    �˶�LED�����λ������  BIT1~BIT4��BIT1=PC11��BIT2=PC12 BIT3=PD13,BIT4=PD14 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	BIT1(1);
	BIT2(1);
	BIT3(1);
	BIT4(1);		
	/*---------��ʼ��״̬���LEDȫOFF------------*/
	LED1(1);//LED1~LED4ȫ�𣬲�Ȼ�������ȫ���е�ѹ�����ջ�������������ý����ʱ������������
	LED2(1);
	LED3(1);
	LED4(1);
	LED5(1);
}
/**********************************************************
*  ADC��ʼ��
**********************************************************/

/*����ADC1�Ĺ���ģʽΪDMAģʽ  */
 void ADC1_Mode_Config(void)
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

	int Read_ADC1_MultiChannel(u8 channNo)
{  
u16  ADC_data;
    ADC_RegularChannelConfig(ADC1, channNo, 1, ADC_SampleTime_239Cycles5 );/* ����ָ��channNoͨ����������*/
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		 		/*ʹ��ADC1 */
    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));	/*�ȴ�ADת������*/
    ADC_data=ADC_GetConversionValue(ADC1);  		  /*ȡADת�����*/
//		ADC_SoftwareStartConvCmd(ADC1, DISABLE);	 	/*�ر�ADC1*/
		return(ADC_data);                             /*����ת�����*/                                        
}       
int ReadADCAverageValue(uint16_t Channel)  //��ƽ��ֵ
{
 u8 i;
 u32 sum = 0;
 for(i=0; i<SampleNum; i++)
 {
  sum+=ADC_ConvertedValue[i][Channel];
 }
 return (sum/SampleNum);
}
/*********************************************************************************
	*�������ƣ�void USART_Configuration(void)
	*
	*��ڲ�������
	*
	*���ڲ�������
	*
	*����˵����USART3��ʼ������ ����GPIO��ʼ�� TX��������Ϊ�������
*********************************************************************************/
void USART123_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3, ENABLE);	 

	USART_DeInit(USART1);
	USART_DeInit(USART2);
	USART_DeInit(USART3);

    /*USART1�˿�����
	  PA9 TX ����������� PA10 RX ��������ģʽ*/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

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

    /*USART3�˿�����
	  PB10 TX ����������� PB11 RX ��������ģʽ*/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


  /*--------------USART1 ����-------------------*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
  /*--------------USART2����-------------------*/
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
  /*--------------USART3����-------------------*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_Init(USART3, &USART_InitStructure);
	USART_Cmd(USART3, ENABLE);
  
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//USART3��������ģ��ͨ�ţ���Ҫ�ж�		
}
/*********************************************************************************
	*�������ƣ�void USART4_Configuration(void)
	*
	*��ڲ�������
	*
	*���ڲ�������
	*
	*����˵����USART3��ʼ������ ����GPIO��ʼ�� TX��������Ϊ�������
*********************************************************************************/
void UART4_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	 

	USART_DeInit(UART4);

  /*--------------USART4 ����-------------------*/
	 /* PC10 TX ����������� PC11 RX ��������ģʽ*/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);
	USART_Cmd(UART4, ENABLE);
}
/*********************************************
*�������ƣ�void USART1_SendString(uint8_t *ch)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����USART1 �����ַ���
**********************************************/
void USART1_SendString(uint8_t *ch)
{
	while(*ch!=0)
	{		
		while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
		USART_SendData(USART1, *ch);
		ch++;
	}   	
}

void Send_Byte(uint8_t Dat)
{
	
		USART_SendData(UART4, Dat);
		while(!USART_GetFlagStatus(UART4, USART_FLAG_TXE));		
}

/*********************************************************************** 
*FunName: ����оƬ��������Ϊָ����������0-30��
************************************************************************/ 
void SetVolume(uint8_t Num)   //0B 01 1E 2A   ��������Ϊ���30(1E)
{	uint8_t sum;
	Send_Byte(0x0B);
	Send_Byte(0x01);
	Send_Byte(Num);
	sum=0x0B+1+Num;
	Send_Byte(sum);
}
/*********************************************************************** 
*FunName: ��������ģ��ָ�������������
************************************************************************/ 
void SendSoundInf(uint8_t Num)   
{
	uint8_t sum;
//�������θ��ɿ�
	Send_Byte(0x03);
	Send_Byte(0x02);
	Send_Byte(Num);
	Send_Byte(0x00);
	sum=3+2+Num;
	Send_Byte(sum);
	
	Delay_ms(100);
	
	Send_Byte(0x03);
	Send_Byte(0x02);
	Send_Byte(Num);
	Send_Byte(0x00);
	sum=3+2+Num;
	Send_Byte(sum);
}
/*********************************************
*�������ƣ�void USART2_SendString(uint8_t *ch)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����USART3 �����ַ���
**********************************************/
void USART2_SendString(uint8_t *ch)
{
	while(*ch!=0)
	{		
		while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
		USART_SendData(USART2, *ch);
		ch++;
	}   	
}

/*********************************************
*�������ƣ�void USART3_SendString(uint8_t *ch)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����USART3 �����ַ���
**********************************************/
void USART3_SendString(uint8_t *ch)
{
	while(*ch!=0)
	{		
		while(!USART_GetFlagStatus(USART3, USART_FLAG_TXE));
		USART_SendData(USART3, *ch);
		ch++;
	}   	
}
uint8_t phasecw[4] ={0x08,0x04,0x02,0x01};//��ת �����ͨ���� D-C-B-A
uint8_t phaseccw[4]={0x01,0x02,0x04,0x08};//��ת �����ͨ���� A-B-C-D
uint8_t MotorData=0;
/*********************************************
*�������ƣ�void Delay_ms(void)
*
*��ڲ�����N  ms
*
*���ڲ�������
*
*����˵��������ʱN ms
**********************************************/
void Delayms(uint16_t N)
{
	uint32_t i;
	for (i=0;i<(8000*N);i++);
}



//˳ʱ��ת��
void MotorCW(void)
{
 uint8_t i;
 uint16_t ReadOD=0,temp=0;
 for(i=0;i<4;i++)
  {
   MotorData=phasecw[i];
   ReadOD=GPIO_ReadOutputData(GPIOD);
   temp=MotorData<<2;		//0,1,2,3->PD2PD3PD4PD7
   if (MotorData&(1<<3)) temp|=(1<<7);
   else 				 temp&=~(1<<7);
   ReadOD&=~(0x9C);			//��2,3,4,7λ����
   temp=ReadOD+temp;
   GPIO_Write(GPIOD, temp);
//   Delay_us(4500);//ת�ٵ���
   Delayms(5);//ת�ٵ���
  }
}
//��ʱ��ת��
void MotorCCW(void)
{
 uint8_t i;
 uint16_t ReadOD=0,temp=0;
 for(i=0;i<4;i++)
  {
   MotorData=phaseccw[i];
   ReadOD=GPIO_ReadOutputData(GPIOD);
   temp=MotorData<<2;		//0,1,2,3->PD2PD3PD4PD7
   if (MotorData&(1<<3)) temp|=(1<<7);
   else 				 temp&=~(1<<7);
   ReadOD&=~(0x9C);			//��2,3,4,7λ����
   temp=ReadOD+temp;
   GPIO_Write(GPIOD, temp);
//   Delay_us(4500);//ת�ٵ���
   Delayms(5);//ת�ٵ���
  }
}
//ֹͣת��
void MotorStop(void)
{
 uint16_t ReadOD=0,temp=0;
   ReadOD=GPIO_ReadOutputData(GPIOD);
   ReadOD&=~(0x9C);			//��2,3,4,7λ����,����λ����
   temp=ReadOD;
   GPIO_Write(GPIOD, temp);
}

