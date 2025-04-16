/**************NUAA_CM3_107实验开发板*********************************
描述： Descriptions:        一些简单外设初始化设置																	 
*********************************************************************************/ 

#include "hw_config.h"
#include "rtc.h"


extern uint32_t DAC_DHR12R2_Address ; 
extern uint32_t DAC2Sine12bit[32];
extern uint8_t ConnectBitCSB;
#define SampleNum 40			/*每个通道采样40次*/
#define ChannelNum 5			/*共5个通道:0＝chan3,1=chan6,2=chan7,3=chan16,4-chan17   */
#define ADC_DR_Address    ((u32)0x4001244C)  /*0x4001244C为ADC_DR的地址（规则数据寄存器地址）*/
u16 ADC_ConvertedValue[SampleNum][ChannelNum];  /*ADC转换结果存放变量（地址）*/
extern u16 value;



////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY1PRES，KEY1按下
//KEY2_PRES，KEY2按下
//KEY3_PRES，KEY3按下 
//注意此函数有响应优先级,KEY1>KEY2>KEY3>KEY4!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	 
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0|KEY4==0))
	{
		Delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==0)return 1;
		else if(KEY2==0)return 2;
		else if(KEY3==0)return 3; 
		else if(KEY4==0)return 4; 
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)key_up=1; 	     
	return 0;// 无按键按下
}

/********************************************************************************************************************************************
@f_name: void Ultran_Init(void)
@brief:	 超声波硬件模块初始化
@param:	 None
@return: None
*********************************************************************************************************************************************/
void Ultran_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;       //发送电平引脚PA2
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1;		//返回电平引脚PC1
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}
/********************************************************************************************************************************************
@f_name: void Ranging(void)
@brief:	 超声波测距，均值5次，实际 测量过程发现测量静态的比较稳定，动态的不稳定，3米范围内比较稳定
@param:	 float *p:测距缓存变量
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
		Ultr_Temp+=340/2*j*10;//  模块最大可测距3m 
		j=0;
		Delay_ms(60);//防止发射信号对回响信号的影响
	}
	*p=Ultr_Temp/5/1000000; 	
		if(j>10000) ConnectBitCSB=0;
		else    ConnectBitCSB=1;
	if ((Ultr_Temp/5/1000000)>10) ConnectBitCSB=0;
	//*p=Ultr_Temp/1000000; 	
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
	DAC_InitTypeDef DAC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);		   

	/*-----------DAC端口配置 复用输出模式-------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	 //PA5:DAC2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA ,&GPIO_InitStructure);
	DAC_DeInit();									  //还原到初始状态

	DAC_SetChannel1Data(DAC_Align_12b_R, 0x0);  //12位右对齐数据格式设置DAC初始值，输出0

	/* DAC 通道2配置 */
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T3_TRGO;		       //选择定时器3作外部触发源
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	   //自定义波形产生
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;	   //失能输出缓冲
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);					   //DAC初始化
	DAC_Cmd(DAC_Channel_2,  ENABLE);								   //使能DAC
	DAC_SoftwareTriggerCmd(DAC_Channel_2, DISABLE);					   //通道2由软件触发
	
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
	PWM_GPIO_Init();

	TIM_OCStructInit(&TIM_OCInitStructure);					//设置缺省值

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	   	//PWM 模式 1 输出 	
	TIM_OCInitStructure.TIM_Pulse = Dutyfactor; 			//设置占空比，占空比=(CCRx/ARR)*100%或(TIM_Pulse/TIM_Period)*100%
															  
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM 输出比较极性高   
																    
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    	//使能输出状态  需要PWM输出才需要这行代码
														
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);				//根据参数初始化PWM寄存器 通道3（PB8）   

	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);//使能 TIMx在 CCR3 上的预装载寄存器

  TIM_CtrlPWMOutputs(TIM4,ENABLE);  							//设置TIM4 的PWM 输出为使能  
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


/**********************************************
*函数名称：void TIMER_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：TIM3初始化
*********************************************/
void TIM_Configuration(uint16_t f)
{

	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 ,ENABLE);
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = SystemCoreClock/1/32/f-1;//0xFF;         //改变这个值能改变正弦波的频率 反比例变化    
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;       //这个值要为零 否则无输出   	 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;   //这个值要为零 否则无输出
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);  //使用更新事件作为触发输出
	
	TIM_Cmd(TIM3, ENABLE);	   //使能TIM3
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //使能定时器4，重要！！
	TIM_DeInit(TIM4);                              				//将IM2定时器初始化位复位值
	TIM_InternalClockConfig(TIM4); 												//配置 TIM4 内部时	   
	TIM_TimeBaseStructure.TIM_Period = 7200-1; 						//设置自动重载寄存器值为最大值	0~65535之间  7200/72000000=1/10000s=0.1ms即10KHz													
															//TIM_Period（TIM1_ARR）=7200，计数器向上计数到7200后产生更新事件，
															//计数值归零 也就是 1ms产生更新事件一次
	TIM_TimeBaseStructure.TIM_Prescaler = 0;  				//自定义预分频系数为0，即定时器的时钟频率为72M提供给定时器的时钟	0~65535之间
															//设置预分频器分频系数0
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分割为0
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
															//TIM向上计数模式 从0开始向上计数，计数到1000后产生更新事件
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 		//根据指定参数初始化TIM时间基数寄存器	
      
 	TIM_ARRPreloadConfig(TIM4, ENABLE);						//使能TIMx在 ARR 上的预装载寄存器 

	TIM_Cmd(TIM4, ENABLE); 		          //使能TIM4
	}


/**********************************************
*函数名称：void DMA_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：DMA2初始化
*********************************************/
void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2 ,ENABLE);				 //打开DMA2时钟
	DMA_DeInit(DMA2_Channel4);								         //将DAM通道4 的寄存器设为默认值
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R2_Address;	 //DAC2  12位右对齐数据寄存器地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&DAC2Sine12bit; //待送入DAC2的数字量
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;				 //外设作为数据传输的来源
	DMA_InitStructure.DMA_BufferSize = 32;						     //DMA缓存大小，32个数据（一个周期的正弦波点数）
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设寄存器地址不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			 //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	//外设数据宽度32位，数据为16位（12位DAC）
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;	  //内存数据宽度32位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;			 //CAN工作在正常缓存模式(本例无使用)
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;		 //设置DMA通道优先级为高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;			 //不设置为内存到内存传输

	DMA_Init(DMA2_Channel4, &DMA_InitStructure);

	DMA_Cmd(DMA2_Channel4, ENABLE);		   

	/*使能DAC通道2:PA.5被自动连接到DAC的转换器*/
	DAC_Cmd(DAC_Channel_2, ENABLE);
	
	/* Enable DMA for DAC Channel2 */
	DAC_DMACmd(DAC_Channel_2, ENABLE);				  //使能DAC通道2的DMA
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn ;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel =RTC_IRQn  ;	    	/*配置EXTI第15~10线(PD11=KEY1,PD12=KEY2)的中断向量*/ 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
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

	GPIO_InitStructure.GPIO_Pin = LED5_PIN|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_8|GPIO_Pin_9|LED5_PIN;//PB1和PB2为双色发光管控制引脚，1亮，0灭
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN |GPIO_Pin_4|GPIO_Pin_15; //PC4/PC15控制继电器
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_8|GPIO_Pin_11;//PA1用来确定是否测试多种显示器件，PA8决定RTC是否设置,PA11决定PWM输出控制电机或电灯泡
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
/*    八段LED数码管段码配置  D0~D7  PE0~PE7     */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
/*    八段LED数码管位码配置  BIT1~BIT4：BIT1=PC11，BIT2=PC12 BIT3=PD13,BIT4=PD14 */
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
	/*---------初始化状态五个LED全OFF------------*/
	LED1(1);//LED1~LED4全灭，不然步进电机全部有电压容易烧坏步进电机，不让进电机时可以让它们灭
	LED2(1);
	LED3(1);
	LED4(1);
	LED5(1);
}
/**********************************************************
*  ADC初始化
**********************************************************/

/*配置ADC1的工作模式为DMA模式  */
 void ADC1_Mode_Config(void)
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

	int Read_ADC1_MultiChannel(u8 channNo)
{  
u16  ADC_data;
    ADC_RegularChannelConfig(ADC1, channNo, 1, ADC_SampleTime_239Cycles5 );/* 设置指定channNo通道及采样率*/
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		 		/*使能ADC1 */
    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));	/*等待AD转换结束*/
    ADC_data=ADC_GetConversionValue(ADC1);  		  /*取AD转换结果*/
//		ADC_SoftwareStartConvCmd(ADC1, DISABLE);	 	/*关闭ADC1*/
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
/*********************************************************************************
	*函数名称：void USART_Configuration(void)
	*
	*入口参数：无
	*
	*出口参数：无
	*
	*功能说明：USART3初始化配置 包括GPIO初始化 TX必须配置为复用输出
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


  /*--------------USART1 配置-------------------*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
  /*--------------USART2配置-------------------*/
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
  /*--------------USART3配置-------------------*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_Init(USART3, &USART_InitStructure);
	USART_Cmd(USART3, ENABLE);
  
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//USART3用于蓝牙模块通信，需要中断		
}
/*********************************************************************************
	*函数名称：void USART4_Configuration(void)
	*
	*入口参数：无
	*
	*出口参数：无
	*
	*功能说明：USART3初始化配置 包括GPIO初始化 TX必须配置为复用输出
*********************************************************************************/
void UART4_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	 

	USART_DeInit(UART4);

  /*--------------USART4 配置-------------------*/
	 /* PC10 TX 复用推挽输出 PC11 RX 浮空输入模式*/
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

void Send_Byte(uint8_t Dat)
{
	
		USART_SendData(UART4, Dat);
		while(!USART_GetFlagStatus(UART4, USART_FLAG_TXE));		
}

/*********************************************************************** 
*FunName: 语音芯片音量设置为指定音量级（0-30）
************************************************************************/ 
void SetVolume(uint8_t Num)   //0B 01 1E 2A   设置音量为最大30(1E)
{	uint8_t sum;
	Send_Byte(0x0B);
	Send_Byte(0x01);
	Send_Byte(Num);
	sum=0x0B+1+Num;
	Send_Byte(sum);
}
/*********************************************************************** 
*FunName: 利用语音模块指定序号语音播放
************************************************************************/ 
void SendSoundInf(uint8_t Num)   
{
	uint8_t sum;
//发送两次更可靠
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

