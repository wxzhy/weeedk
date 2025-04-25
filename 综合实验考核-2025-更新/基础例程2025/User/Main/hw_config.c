/**************NUAA_CM3_107实验开发板*********************************
描述： Descriptions:        一些简单外设初始化设置																	 
*********************************************************************************/ 

#include "hw_config.h"
extern uint32_t DAC_DHR12R2_Address ; 
extern uint32_t DAC2Sine12bit[32];
#define SampleNum 20			/*采样20次*/
#define ADC_DR_Address    ((u32)0x4001244C)  /*0x4001244C为ADC_DR的地址（规则数据寄存器地址）*/
u16 ADC_ConvertedValue[SampleNum];  /*单通道ADC转换结果存放变量（地址）*/
extern u16 value;

/**********************************************
*函数名称：void DAC_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：DAC初始化通道1
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

	DAC_SetChannel1Data(DAC_Align_12b_R, 0xF00);  //12位右对齐数据格式设置DAC初始值，输出0

}
void PWM_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//定义一个GPIO结构体变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);	
																								//使能各个端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9; //PB8和PB9引脚，PB6、PB7、PB8和PB9分别为TIM4对应的CH1、CH2、CH3和CH4四个通道的PWM输出引脚，这里使用PB8和PB9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   	  //复用输出推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//配置端口速度为50M
	GPIO_Init(GPIOB, &GPIO_InitStructure);				   			//将端口GPIOD进行初始化配置
}

/**************************************************
*函数名称：void Init_PWM(uint16_t Dutyfactor)
*
*入口参数：Dutyfactor 定义占空比大小
*
*出口参数：无
*
*功能说明：配置PWM通道及占空比
***************************************************/
void Init_PWM(uint16_t Dutyfactor)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;					//定义一个通道输出结构

	TIM_OCStructInit(&TIM_OCInitStructure);					//设置缺省值

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	   	//PWM 模式 1 输出 	
	TIM_OCInitStructure.TIM_Pulse = Dutyfactor; 			//设置占空比，占空比=(CCRx/ARR)*100%或(TIM_Pulse/TIM_Period)*100%
															  
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM 输出比较极性高   
																    
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    	//使能输出状态  需要PWM输出才需要这行代码
														
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);				//根据参数初始化PWM寄存器 通道3（PB8）   

	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);//使能 TIMx在 CCR3 上的预装载寄存器

  TIM_CtrlPWMOutputs(TIM4,ENABLE);  							//设置TIM4 的PWM 输出为使能  
}
/**********************************************
*函数名称：void TIMER_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：TIM3初始化
*********************************************/
void TIM_Configuration(u16 nms)
{

	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM4 ,ENABLE);
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = 10*nms;         //ms数
	TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock/10000-1;       
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		
	TIM_Cmd(TIM2, ENABLE);	   //使能TIM2
	
	TIM_DeInit(TIM4);                              				//将IM2定时器初始化位复位值
	TIM_InternalClockConfig(TIM4); 												//配置 TIM4 内部时	   
	
	TIM_TimeBaseStructure.TIM_Period =1000000/ 1000-1; 						//设置自动重载寄存器值为最大值	0~65535之间  1000Hz
															//TIM_Period（TIM1_ARR）=7200，计数器向上计数到7200后产生更新事件，
															//计数值归零 也就是 1ms产生更新事件一次
	TIM_TimeBaseStructure.TIM_Prescaler =SystemCoreClock/1000000-1;// 72-1;  				//自定义预分频系数为0，即定时器的时钟频率为72M提供给定时器的时钟	0~65535之间

															//设置预分频器分频系数0
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分割为0
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
															//TIM向上计数模式 从0开始向上计数，计数到1000后产生更新事件
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 		//根据指定参数初始化TIM时间基数寄存器	
      
 	TIM_ARRPreloadConfig(TIM4, ENABLE);						//使能TIMx在 ARR 上的预装载寄存器 

	TIM_Cmd(TIM4, ENABLE); 		          //使能TIM4	
}



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

	NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn; 		  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //次占优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 		  //USART3接收中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //先占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;		  //次占优先级5
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);
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

	GPIO_InitStructure.GPIO_Pin = LED5_PIN|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_13;//PB1和PB2为双色发光管控制引脚，1亮，0灭
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3  AIN3 模拟输入引脚定义
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN |GPIO_Pin_4|GPIO_Pin_15; //PC4/PC15控制继电器
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*---------初始化状态五个LED全OFF------------*/
	LED1(1);//LED1~LED4全灭
	LED2(1);
	LED3(1);
	LED4(1);
	LED5(1);

}
/***********************************************************
*函数名称：void ADC_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：ADC1初始化配置，PA3为ADC1_AIN3 */

/*配置ADC1的工作模式为DMA模式  */
 void ADC_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能DMA1时钟
	/* DMA channel1 configuration */
  DMA_DeInit(DMA1_Channel1);  //指定DMA通道1
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC_DR_Address;//设置DMA外设地址ADC_DR_Adress
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	//设置DMA内存地址，ADC转换结果直接放入该地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //外设为设置为数据传输的来源
  DMA_InitStructure.DMA_BufferSize = SampleNum;	//DMA缓冲区设置为SampleNum*ChannelNum；
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
  ADC_InitStructure.ADC_NbrOfChannel = 1;  // 1个通道转换通道
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel11 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5); //通道3（电位器）采样周期55.5个时钟周期

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



int ReadADCAverageValue()  //求平均值
{
 u8 i;
 u32 sum = 0;
 for(i=0; i<SampleNum; i++)
 {
  sum+=ADC_ConvertedValue[i];
 }
 return (sum/SampleNum);
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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3, ENABLE);	 

	USART_DeInit(USART1);
	USART_DeInit(USART2);
	USART_DeInit(USART3);

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

    /*USART3端口配置
	  PB10 TX 复用推挽输出 PB11 RX 浮空输入模式*/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*--------------USART3 配置-------------------*/
	USART_InitStructure.USART_BaudRate = 115200;
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
*功能说明：USART3 发送字符串
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
uint8_t phasecw[4] ={0x08,0x04,0x02,0x01};//正转 电机导通相序 D-C-B-A
uint8_t phaseccw[4]={0x01,0x02,0x04,0x08};//反转 电机导通相序 A-B-C-D
uint8_t MotorData=0;
/*********************************************
*函数名称：void Delay_ms(void)
*
*入口参数：N  ms
*
*出口参数：无
*
*功能说明：简单延时N ms
**********************************************/
void Delayms(uint16_t N)
{
	uint32_t i;
	for (i=0;i<(8000*N);i++);
}



//顺时针转动
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
   ReadOD&=~(0x9C);			//将2,3,4,7位清零
   temp=ReadOD+temp;
   GPIO_Write(GPIOD, temp);
//   Delay_us(4500);//转速调节
   Delayms(5);//转速调节
  }
}
//逆时针转动
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
   ReadOD&=~(0x9C);			//将2,3,4,7位清零
   temp=ReadOD+temp;
   GPIO_Write(GPIOD, temp);
//   Delay_us(4500);//转速调节
   Delayms(5);//转速调节
  }
}
//停止转动
void MotorStop(void)
{
 uint16_t ReadOD=0,temp=0;
   ReadOD=GPIO_ReadOutputData(GPIOD);
   ReadOD&=~(0x9C);			//将2,3,4,7位清零,其它位不变
   temp=ReadOD;
   GPIO_Write(GPIOD, temp);
}

