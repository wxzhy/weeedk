/**************NUAA_CM3_107实验开发板*********************************
描述： Descriptions:        一些简单外设初始化设置																	 
*********************************************************************************/ 

#include "hw_config.h"

/*********************************************
*函数名称：void NVIC_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：NVIC初始化配置
**********************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn ;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);			  //中断分组3
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 		  //USART3接收中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //先占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //次占优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);
}

/***************************************************
*函数名称：void EXTI_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：EXTI初始化配置
*****************************************************/
void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	/*PB0外部中断输入*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line0; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//PB0下沿触发 
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/*********************************************
*函数名称：void REMOTE_Init()
*
*入口参数：无
*
*出口参数：无
*
*功能说明: 红外接收管脚配置
**********************************************/
void REMOTE_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;					 //PB0引脚为红外接收输入引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			     //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		     //管脚频率50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);	 //GPIOB0 外部中断触发
}

/**************************************************
*函数名称：void ADC_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：ADC初始化配置  测内核电压使用ADC_Channel_17
*	       测温度传感器输出电压使用ADC_Channel_16
***************************************************/
void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);      //使能ADC时钟
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	      //ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;		      //多通道扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;		  //连续模数转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	 //转换由软件触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				 //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	 							 //进行规则转换的通道数为1
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_239Cycles5);

	ADC_TempSensorVrefintCmd(ENABLE);			  //使能温度传感器和内部参考电压通道
		  
	ADC_ResetCalibration(ADC1);	                  //复位校准寄存器

	while(ADC_GetResetCalibrationStatus(ADC1));	  //等待ADC1复位校准寄存器结束
	
	ADC_StartCalibration(ADC1);			          //开始校准

	while(ADC_GetCalibrationStatus(ADC1));		  //等待校准结束
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		  //启动软件转换，这句必须加上去
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

	GPIO_InitStructure.GPIO_Pin = LED5_PIN|GPIO_Pin_8|GPIO_Pin_9;
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

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);//电机停止
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	
	/*---------初始化状态五个LED全OFF------------*/
//	LED1=1;
//	LED2=1;
//	LED3=1;
//	LED4=1;
//	LED5=1;		
}

/*********************************************************************************
	*函数名称：void USART_Configuration(void)
	*
	*入口参数：无
	*
	*出口参数：无
	*
	*功能说明：USART3初始化配置 包括GPIO初始化 TX必须配置为复用输出
*********************************************************************************/
void USART_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3, ENABLE);	 

	USART_DeInit(USART3);

    /*USART3端口配置
	  PB10 TX 复用推挽输出 PB11 RX 浮空输入模式*/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  /*--------------USART3 USART2配置-------------------*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	USART_Cmd(USART3, ENABLE);

  	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);		
}
/*********************************************
*函数名称：void USART3_SendString(uint8_t *ch)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：USART3 发送字符串
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

