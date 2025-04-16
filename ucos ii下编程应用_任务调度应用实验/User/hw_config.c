/****************************Copyright(c)*****************************
**                      http://www.openmcu.com
**--------------File Info---------------------------------------------------------------------------------
** File name:           hw_config.c
** Last modified Date:  2012-08-27
** Last Version:        V1.00
** Descriptions:        硬件配置文件
**
**----------------------------------------------------------------------------------------------------
** Created by:          openmcu
** Created date:        2012-08-27
** Version:             V1.00
** Descriptions:        编写示例代码
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
*函数名称：void Delayms(void)
*
*入口参数：N  ms
*
*出口参数：无
*
*功能说明：简单延时N ms
**********************************************/
void Delay_ms(uint16_t N)
{
	uint32_t i;
	for (i=0;i<(8000*N);i++);
}
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



/*****************************************************************************
** 函数名称: void NVIC_Configuration(void)
** 功能描述: NVIC初始化
** 入口参数: 无
** 出口参数: 无
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
** 函数名称: void  SysTick_Configuration(void)
** 功能描述: 系统定时器初始化
** 入口参数: 无
** 出口参数: 无
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
** 函数名称: void LED_Configuration(void)
** 功能描述: LED初始化
** 入口参数: 无
** 出口参数: 无
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

	/*---------初始化状态四个LED全OFF------------*/
	LED1(1);
	LED2(1);
	LED3(1);
	LED4(1);
//	LED5(1);
}

/*****************************************************************************
** 函数名称: void USART_Configuration(void)
** 功能描述: 串口1初始化	波特率115200
** 入口参数: 无
** 出口参数: 无
*****************************************************************************/
void USART_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1, ENABLE);
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
	
    /*--------------USART1 USART2配置-------------------*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);

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

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7 ;//PD7为485收发控制设置为推挽输出
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

    /*--------------USART2配置-------------------*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_Cmd(USART2, ENABLE);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//中断配置	
}
/**************************************************
*函数名称：void Init_TIMER(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：TIM4初始化配置
***************************************************/
void Init_TIMER(u16 us)
{
	TIM_TimeBaseInitTypeDef	 TIM_BaseInitStructure;			//定义一个定时器结构体变量

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   	//使能定时器4，重要！！

	TIM_DeInit(TIM4);                              			//将TIM4定时器初始化位复位值

	TIM_InternalClockConfig(TIM4); 							//配置 TIM4 内部时钟
	   
	TIM_BaseInitStructure.TIM_Period = SystemCoreClock/(100*us)-1;//定时us微秒,							
															//TIM_Period（TIM1_ARR）=7200，计数器向上计数到7200后产生更新事件，
															//计数值归零 也就是 1MS产生更新事件一次
	TIM_BaseInitStructure.TIM_Prescaler = 0;  					//自定义预分频系数为0，即定时器的时钟频率为72M提供给定时器的时钟	0~65535之间
																											//设置预分频器分频系数0
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分割为0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   
																											//TIM向上计数模式 从0开始向上计数，计数到1000后产生更新事件
	TIM_TimeBaseInit(TIM4, &TIM_BaseInitStructure); 		//根据指定参数初始化TIM时间基数寄存器	
      
 	TIM_ARRPreloadConfig(TIM4, ENABLE);									//使能TIMx在 ARR 上的预装载寄存器 

	TIM_Cmd(TIM4, ENABLE); 		          								//使能TIM4
}


/**************************************************
*函数名称：void Init_PWM(uint16_t Dutyfactor)
*
*入口参数：Dutyfactor 定义占空比大小
*
*出口参数：无
*
*功能说明：用TIM4配置PWM通道及占空比
***************************************************/
void Init_PWM(uint16_t Dutyfactor)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;					//定义一个通道输出结构

	TIM_OCStructInit(&TIM_OCInitStructure);								//设置缺省值

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	   	//PWM 模式 1 输出 	
	TIM_OCInitStructure.TIM_Pulse = Dutyfactor; 					//设置占空比，占空比=(CCRx/ARR)*100%或(TIM_Pulse/TIM_Period)*100%
															  
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
																												//TIM 输出比较极性高   	    
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    
																												//使能输出状态  需要PWM输出才需要这行代码
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);						//根据参数初始化PWM寄存器    

	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);	   	//使能 TIMx在 CCR4 上的预装载寄存器

    TIM_OC4Init(TIM4, &TIM_OCInitStructure);						//根据参数初始化PWM寄存器    

	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);	   	//使能 TIMx在 CCR4 上的预装载寄存器

    TIM_CtrlPWMOutputs(TIM4,ENABLE);  									//设置TIM4 的PWM 输出为使能  
}

/**************************************************
*函数名称：void PWM_GPIO_Init(uint16_t Dutyfactor)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：配置PB8、PB9为复用推挽输出	
***************************************************/
void PWM_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//定义一个GPIO结构体变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);	
															//使能各个端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   	    //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//配置端口速度为50M
	GPIO_Init(GPIOB, &GPIO_InitStructure);				   	//将端口GPIOD进行初始化配置
}

/*****************************************************************************
** 函数名称: int main(void)
** 功能描述: 主函数入口
** 入口参数: 无
** 出口参数: 无
*****************************************************************************/
int fputc(int ch, FILE *f)
{
  USART_SendData(USART2, (uint8_t) ch);   
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
  return ch;
}
