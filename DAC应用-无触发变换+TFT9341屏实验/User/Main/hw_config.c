/**************NUAA_CM3_107实验开发板*********************************
描述： Descriptions:        一些简单外设初始化设置																	 
*********************************************************************************/ 

#include "hw_config.h"
#define SampleNum 10			/*每个通道采样40次*/
#define ChannelNum 5			/*共5个通道:0＝chan3,1=chan6,2=chan7,3=chan16,4-chan17   */
#define ADC_DR_Address    ((u32)0x4001244C)  /*0x4001244C为ADC_DR的地址（规则数据寄存器地址）*/
#define DAC_DHR12RD_Address  ((u32) 0x40007420);/* DAC数据寄存器地址*/
u16 ADC_ConvertedValue[SampleNum][ChannelNum];  /*ADC转换结果存放变量（地址）*/
extern u16 value;
void Delay_ms(uint16_t N)
{
	uint32_t i;
	for (i=0;i<(8000*N);i++);
}

/*********************************************************************************
	*函数名称：void LED_Configuration(void)
	*
	*入口参数：无
	*
	*出口参数：无
	*
	*功能说明：LED初始化配置
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

	/*---------初始化状态四个LED全OFF------------*/
	LED1(1);
	LED2(1);
	LED3(1);
	LED4(1);
			
}


/*********************************************************************************
	*函数名称：voidADC_Mode_config(void)
	*
	*入口参数：无
	*
	*出口参数：无
	*
	*功能说明：配置ADC1的工作模式为DMA模式 
*********************************************************************************/

 void ADC_Mode_Config(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能DMA1时钟
	/* DMA channel1 configuration */
  DMA_DeInit(DMA1_Channel1);  //指定DMA通道
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC_DR_Address;//设置DMA外设地址ADC_DR_Adress
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	//设置DMA内存地址，ADC转换结果直接放入该地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //外设为设置为数据传输的来源
  DMA_InitStructure.DMA_BufferSize = SampleNum*ChannelNum;	//DMA缓冲区设置为SampleNum*ChannelNum；
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//DMA不允许地址递增
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址递增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);  //使能DMA通道

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//使能ADC1时钟
     
  /* ADC1 configuration */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //使用独立模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;			 //扫描模式允许，多通道必须使能
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //连接转换模式，无需外接触发器
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//不用外部触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //使用数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = ChannelNum;  // ChannelNum个通道转换通道
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel11 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5); //通道3（电位器）采样周期55.5个时钟周期
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_55Cycles5); //通道6（PT100）采样周期55.5个时钟周期
  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 3, ADC_SampleTime_55Cycles5); //通道7（电流电压）采样周期55.5个时钟周期
  ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 4, ADC_SampleTime_55Cycles5); //通道16（内部温度）采样周期55.5个时钟周期
  ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 5, ADC_SampleTime_55Cycles5); //通道17（基准电压）采样周期55.5个时钟周期

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);	 //使能ADC的DMA
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE); //使能ADC1

	ADC_TempSensorVrefintCmd(ENABLE);			 					 /*使能温度传感器和内部参考电压通道*/	

  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
     
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //开始转换
}

/**********************************************
*函数名称：void DAC_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：DAC初始化
*********************************************/
void DAC_Configuration(void)
{
  
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE );	  //使能PORTA通道时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //使能DAC通道时钟 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽（模拟）输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_SetBits(GPIOA,GPIO_Pin_4)	;//PA.4 输出高
	
	DAC_DeInit();
	
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC1

	DAC_SetChannel1Data(DAC_Align_12b_R, 0x0);  //12位右对齐数据格式设置DAC初始值，输出0

}

//设置通道1输出电压
//vol:0~3300,代表0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	float temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12位右对齐数据格式设置DAC值
}


	int Read_ADC1_MultiChannel(u8 channNo)
{  
u16  ADC_data;
    ADC_RegularChannelConfig(ADC1, channNo, 1, ADC_SampleTime_239Cycles5 );/* 设置指定channNo通道及采样率*/
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		 		/*使能ADC1 */
    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));	/*等待AD转换结束*/
    ADC_data=ADC_GetConversionValue(ADC1);  		  /*取AD转换结果*/
//		ADC_SoftwareStartConvCmd(ADC1, DISABLE);	 		/*关闭ADC1*/
		return(ADC_data);                             /*返回转换结果*/                                        
}       
int ReadADCAverageValue(uint16_t Channel)  //求平均值
{
 u8 i;
 u32 sum = 0;
 for(i=0; i<SampleNum; i++)
 {
  sum+=ADC_ConvertedValue[i][Channel];
 }
 return (sum/SampleNum);
}
/****************去极值平均滤波 ************************/
int ReadADCQJZQPJValue(u16 Channel)
{
u16 count,i,j,temp,sum;
for (j=0;j<SampleNum-1;j++)   //采样值由小到大排列，排序采用冒泡法
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
for(count=1;count<(SampleNum-1);count++)     /*去掉第一个和末一个数*/
{
sum+=ADC_ConvertedValue[count][Channel];
//delay();
}
return (u16)(sum/(SampleNum-2));
}
/**********************低通滤波**********************/
int filter_DTLB(void)
{
u16 new_value;    //new value为当前采样值
new_value=ADC_ConvertedValue[5][0];
return ((float)(0.3*new_value+(1-0.3)*value));
}

