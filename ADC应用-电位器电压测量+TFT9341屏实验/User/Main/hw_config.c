/**************NUAA_CM3_107实验开发板*********************************
描述： Descriptions:        一些简单外设初始化设置																	 
*********************************************************************************/ 

#include "hw_config.h"

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

/***********************************************************************
函数名称：NVIC_Configuration(void)
功    能：完成中断优先级的配置
输入参数：
输出参数：
编写时间：
编 写 人：
注    意：
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
*  独立看门狗初始化IWDG_Init
*入口：预分频系统prer,重装计数值rlr
*出口：无
*功能：以prer为预分频值，rlr重装值启动IWDG
*说明：IWDG以40KHz为计数时钟，因此定时IWDG溢出时间
       TIWDG=4×2^IWDG_PR×（1+IWDG_RLR）/40KHz		
			当IWDG_PR=prer=4,IWDG_RLR=rlr=625时，TIWDG=1秒
**********************************************************/

/***********************************************************
*函数名称：void ADC_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：ADC1初始化配置 
*************************************************************/
void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_ADC1,ENABLE);/*PA3，6，7为ADCIN3，6，7*/
/* 配置GPIO的PA3/6/7作为ADCIN3/6/7模拟通道输入端，频率50MHz */			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7;     /*ADC1通道PA3=ADCIN3,PA6=ADIN6.PA7=ADIN7引脚*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                   /*管脚频率50MHz*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	                    	/*模拟输入模式*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);

/* 初始化ADC：独立模式、多通道扫描、连续转换、软件触发、ADC数据右对齐 */			
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	                /*独立工作模式*/
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;					    					/*禁止多通道扫描*/
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;									/*连续转换模式禁止*/
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	/*不用外部触发，用软件触发启动*/
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	            /*ADC数据右对齐*/
	ADC_InitStructure.ADC_NbrOfChannel = 1;	                            /*进行规则转换的ADC通道数为3个通道（ADCIN3、6、7）*/
	ADC_Init(ADC1, &ADC_InitStructure);
	

	ADC_Cmd(ADC1, ENABLE);	                     /*使能ADC1*/
	ADC_ResetCalibration(ADC1);	                 /*使能ADC1复位校准寄存器*/
	while(ADC_GetResetCalibrationStatus(ADC1));	 /*等待复位校准寄存器接收*/
	ADC_StartCalibration(ADC1);			         		 /*启动ADC1校准*/
	while(ADC_GetCalibrationStatus(ADC1));	     /*等待ADC1校准结束*/
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		 	 /*启动软件转换*/
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


