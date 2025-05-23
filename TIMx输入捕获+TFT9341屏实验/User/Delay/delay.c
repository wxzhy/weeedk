/***********************************************************************
文件名称：Delay.C
功    能：定时器TIM2捕获引脚PA0测量KEY4按下低电平的时间
编写时间：2017.3.29
编 写 人：
注    意：
***********************************************************************/
#include "stm32f10x.h"

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

