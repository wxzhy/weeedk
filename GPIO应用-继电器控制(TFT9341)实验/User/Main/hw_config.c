/**************NUAA_CM3_107实验开发板*********************************
描述： Descriptions:        一些简单外设初始化设置																	 
*********************************************************************************/ 

#include "hw_config.h"

uint8_t phasecw[4] ={0x08,0x04,0x02,0x01};//正转 电机导通相序 D-C-B-A
uint8_t phaseccw[4]={0x01,0x02,0x04,0x08};//反转 电机导通相序 A-B-C-D

uint8_t MotorData=0;
uint8_t BeepFlag=0;

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
   Delay_ms(5);//转速调节
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
   Delay_ms(5);//转速调节
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			  //中断分组2
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 		  //USART3接收中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //次占优先级
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

	GPIO_InitStructure.GPIO_Pin = LED5_PIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN |GPIO_Pin_4|GPIO_Pin_15;//继电器控制引脚PC4、PC15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2 ;//PB1,PB2双色LED指示灯
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
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

