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
extern uint32_t DAC_DHR12R2_Address ; 
extern uint32_t DAC2Sine12bit[32];
#define SampleNum 20			/*����20��*/
#define ADC_DR_Address    ((u32)0x4001244C)  /*0x4001244CΪADC_DR�ĵ�ַ���������ݼĴ�����ַ��*/
u16 ADC_ConvertedValue[SampleNum];  /*��ͨ��ADCת�������ű�������ַ��*/
extern u16 value;

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
/*********************************************
*�������ƣ�void USART3_SendString(uint8_t *ch)
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

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_LED, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(LED_PORT, &GPIO_InitStructure);

	/*---------��ʼ��״̬�ĸ�LEDȫOFF------------*/
	LED1(1);
	LED2(1);
	LED3(1);
	LED4(1);
}
/*****************************************************************************
** ��������: void GPIO_Configuration(void)
** ��������: GPIO��ʼ��
** ��ڲ���: ��
** ���ڲ���: ��
*****************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_LED|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(LED_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;/*PC0������������ӷ�����*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIOC->ODR&=~(1<<0);
	/*---------��ʼ��״̬�ĸ�LEDȫOFF------------*/
	LED1(1);
	LED2(1);
	LED3(1);
	LED4(1);
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

/***********************************************************
*�������ƣ�void ADC_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����ADC1��ʼ�����ã�PA3ΪADC1_AIN3 */

/*����ADC1�Ĺ���ģʽΪDMAģʽ  */
 void ADC_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��DMA1ʱ��
	/* DMA channel1 configuration */
  DMA_DeInit(DMA1_Channel1);  //ָ��DMAͨ��1
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC_DR_Address;//����DMA�����ַADC_DR_Adress
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	//����DMA�ڴ��ַ��ADCת�����ֱ�ӷ���õ�ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //����Ϊ����Ϊ���ݴ������Դ
  DMA_InitStructure.DMA_BufferSize = SampleNum;	//DMA����������ΪSampleNum*ChannelNum��
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
  ADC_InitStructure.ADC_NbrOfChannel = 1;  // 1��ͨ��ת��ͨ��
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel3 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5); //ͨ��3����λ������������55.5��ʱ������

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



int ReadADCAverageValue()  //��ƽ��ֵ
{
 u8 i;
 u32 sum = 0;
 for(i=0; i<SampleNum; i++)
 {
  sum+=ADC_ConvertedValue[i];
 }
 return (sum/SampleNum);
}
/**********************************************
*�������ƣ�void DAC_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����DAC��ʼ��ͨ��1
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

	DAC_SetChannel1Data(DAC_Align_12b_R, 0xF00);  //12λ�Ҷ������ݸ�ʽ����DAC��ʼֵ�����0

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
