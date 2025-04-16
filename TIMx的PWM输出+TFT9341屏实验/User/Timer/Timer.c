/***********************************************************************
文件名称：Timer.C
功    能：定时器TIM2捕获引脚PA0测量KEY4按下低电平的时间
编写时间：2017.3.29
编 写 人：
注    意：
***********************************************************************/
#include "stm32f10x.h"


/**************************************************
*函数名称：void Init_TIMER(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：TIM4初始化配置为定时1ms（1KHz）自动重装
***************************************************/
void Init_TIMER(void)
{
	TIM_TimeBaseInitTypeDef	 TIM_BaseInitStructure;				//定义一个定时器结构体变量
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //使能定时器4，重要！！
	TIM_DeInit(TIM4);                              				//将IM2定时器初始化位复位值
	TIM_InternalClockConfig(TIM4); 												//配置 TIM4 内部时	   
	//TIM_BaseInitStructure.TIM_Period = 7200-1; 						//设置自动重载寄存器值为最大值	0~65535之间  7200/72000000=1/10000s=0.1ms即10KHz													
															//TIM_Period（TIM1_ARR）=7200，计数器向上计数到7200后产生更新事件，
															//计数值归零 也就是 1ms产生更新事件一次
	TIM_BaseInitStructure.TIM_Period = 720-1; 						//设置自动重载寄存器值为最大值	0~65535之间  7200/7200000=1/1000s=1ms即1KHz								
	TIM_BaseInitStructure.TIM_Prescaler = 0;  				//自定义预分频系数为0，即定时器的时钟频率为72M提供给定时器的时钟	0~65535之间
															//设置预分频器分频系数0
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分割为0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   
															//TIM向上计数模式 从0开始向上计数，计数到1000后产生更新事件
	TIM_TimeBaseInit(TIM4, &TIM_BaseInitStructure); 		//根据指定参数初始化TIM时间基数寄存器	
      
 	TIM_ARRPreloadConfig(TIM4, ENABLE);						//使能TIMx在 ARR 上的预装载寄存器 

	TIM_Cmd(TIM4, ENABLE); 		          //使能TIM4
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

void TIM2_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_ICInitTypeDef  TIM2_ICInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); /*使能TIM2时钟*/
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);/*使能GPIOA时钟*/
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;    			 /*PA0 引脚     */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				 /*PA0 上拉输入 */ 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_0);						 
	
	/*初始化定时器2 TIM2	 */
	TIM_TimeBaseStructure.TIM_Period = arr; 						/*设定计数器自动重装值 */
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 					/*预分频器   */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			 /*设置时钟分割:TDTS = Tck_tim*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM向上计数模式*/
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 		/*根据TIM_TimeBaseInitStruct中指定的参数初始化TIM2的时间基数单位*/
  
	/*初始化TIM2输入捕获参数*/
		TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1; 								/*选择输入端 IC1映射到TI1上*/
  	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;			/*下升沿捕获*/
  	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	/*映射到TI1上*/
  	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;						/*配置输入不分频*/ 
  	TIM2_ICInitStructure.TIM_ICFilter = 0x00;													/*配置输入不滤波*/
  	TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	/*中断分组初始化*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 					 /*TIM2中断*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  /*先占优先级2级*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  			 /*从优先级0级*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					 	 /*IRQ通道被使能*/
	NVIC_Init(&NVIC_InitStructure);  /*根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 */
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);/*允许更新中断 ,允许CC1IE捕获中断*/	
	
  TIM_Cmd(TIM2,ENABLE ); 	/*使能定时器2*/
 
}


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

