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
*功能说明：NVIC初始化配置， 使能UART1和UART2串口接收中断
**********************************************/

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			  //中断分组2
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 		  //USART1接收中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //次占优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 		  //USART2接收中断
	NVIC_Init(&NVIC_InitStructure);
 
 	/*外部中断线*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn ;	//配置EXTI第5~10线的中断向量 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;	//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	  //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;	
	NVIC_Init(&NVIC_InitStructure);

}
void BUTTON_Configuration(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC , GPIO_PinSource13);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource11);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource12);
}


/**************************************************
*函数名称：void EXTI_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：EXTI初始化配置
***************************************************/
void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	/*PD11外部中断输入*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line11; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/*PD12外部中断输入*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	 //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE; 
	EXTI_Init(&EXTI_InitStructure);	

	/*PC13外部中断输入*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	 //下降沿触发
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
void LED_Configuration(void)
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

	/*---------初始化状态五个LED全OFF------------*/
//	LED1=1;
//	LED2=1;
//	LED3=1;
//	LED4=1;
//	LED5=1;		
}

/*********************************************
*函数名称：void USART_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：USART1、2 初始化配置 包括GPIO初始化 TX必须配置为复用输出
**********************************************/
void USART_Configuration(uint32_t UART1_Baud,uint32_t UART2_Baud)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD 
						   |RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	 

    /*USART1端口配置
	  PA9 TX 复用推挽输出 PA10 RX 浮空输入模式*/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*USART2端口配置
	  PD5 TX 复用推挽输出 PD6 RX 浮空输入模式*/
	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);	  //如果使用PA2、PA3不需要重新映射

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
      	
    /*--------------USART1 USART2配置-------------------*/
	USART_InitStructure.USART_BaudRate = UART1_Baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_InitStructure.USART_BaudRate = UART2_Baud;
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
	USART_Cmd(USART2, ENABLE);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//中断配置	
}
/*********************************************
*函数名称：void USART1_SendString(uint8_t *ch)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：USART1 发送字符串
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

/*********************************************
*函数名称：void USART2_SendString(uint8_t *ch)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：USART2 发送字符串
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

