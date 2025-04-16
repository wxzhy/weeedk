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
	DAC_InitTypeDef DAC_InitType;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE );	  //ʹ��PORTAͨ��ʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //ʹ��DACͨ��ʱ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�������죨ģ�⣩���
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_SetBits(GPIOA,GPIO_Pin_4)	;//PA.4 �����
	
	DAC_DeInit();
	
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô�������
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //��ʼ��DACͨ��1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //ʹ��DAC1

	DAC_SetChannel1Data(DAC_Align_12b_R, 0x0);  //12λ�Ҷ������ݸ�ʽ����DAC��ʼֵ�����0

}

//����ͨ��1�����ѹ
//vol:0~3300,����0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	float temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
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
/****************ȥ��ֵƽ���˲� ************************/
int ReadADCQJZQPJValue(u16 Channel)
{
u16 count,i,j,temp,sum;
for (j=0;j<SampleNum-1;j++)   //����ֵ��С�������У��������ð�ݷ�
{
for (i=0;i<SampleNum-j;i++)
{
if(ADC_ConvertedValue[i][Channel]>ADC_ConvertedValue[i+1][Channel])
{
temp=ADC_ConvertedValue[i][Channel];
ADC_ConvertedValue[i][Channel]=ADC_ConvertedValue[i+1][Channel];
ADC_ConvertedValue[i][Channel]=temp;
}
}
}
sum=0;
for(count=1;count<(SampleNum-1);count++)     /*ȥ����һ����ĩһ����*/
{
sum+=ADC_ConvertedValue[count][Channel];
//delay();
}
return (u16)(sum/(SampleNum-2));
}
/**********************��ͨ�˲�**********************/
int filter_DTLB(void)
{
u16 new_value;    //new valueΪ��ǰ����ֵ
new_value=ADC_ConvertedValue[5][0];
return ((float)(0.3*new_value+(1-0.3)*value));
}

