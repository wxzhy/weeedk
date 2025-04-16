
/************************************************************************************
**	  WEEEDK NUAA_CM3_107实验开发板（V4.1）
**	  GPIO应用:电机控制实验
**    操作及现象：下载程序运行后
			当按下KEY1键时，双色红灯闪烁且电机正转，当按下KEY2键时，双色绿灯闪烁且电机反转，当按下KEY3键时，双色灯灭，电机停止
**    版    本：V4.1
**	  作    者：NUAA
**	  完成日期:	2017.3.19
************************************************************************************/
#include "stm32f10x_conf.h"

#define KEY1	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)
#define KEY2	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)
#define KEY3	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)
#define IA(x)   	((x) ? (GPIO_SetBits(GPIOB, GPIO_Pin_8)) : (GPIO_ResetBits(GPIOB, GPIO_Pin_8)))
#define IB(x)   	((x) ? (GPIO_SetBits(GPIOB, GPIO_Pin_9)) : (GPIO_ResetBits(GPIOB, GPIO_Pin_9)))
#define LEDR(x)   ((x) ? (GPIO_SetBits(GPIOB, GPIO_Pin_1)) : (GPIO_ResetBits(GPIOB, GPIO_Pin_1)))
#define LEDG(x)   ((x) ? (GPIO_SetBits(GPIOB, GPIO_Pin_2)) : (GPIO_ResetBits(GPIOB, GPIO_Pin_2)))
/*********************************************
*函数名称：void Delayms(void)
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

/*********************************************
*函数名称：void GPIO_MOTO_Init(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：初始化GPIO（LED、MOTO、KEY）
**********************************************/
void GPIO_MOTO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);  /*初始化GPIOD端口PD11=KEY1,PD12=KEY2上拉输入，10MHz*/

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);  /*初始化GPIOC端口PC13=KEY3上拉输入，10MHz*/

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);  /*初始化GPIOD端口PD2、3、4、7 10MHz推挽输出*/
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);  /*初始化GPIOB端口PB1=RED,PB2=GREEN,PB8=IA,PB9=IB 10MHz推挽输出*/
	
	/*---------初始化状态指示灯灭OFF------------*/
	IA(0);IB(0);					/*  电机停止  */
	LEDR(0);LEDG(0);			/*  双色指示灯全灭  */	
	GPIO_SetBits(GPIOD,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7);/*  4只指示灯灭 */
}

#define  n 200    /*延时时间常数*/
int main(void)
{
u8 KEY=0;	
	SystemInit();     		/*系统初始化*/
	GPIO_MOTO_Init();			/*GPIO初始化*/
	while (1)
	{if (KEY1==0) 		KEY=1;
	else{if (KEY2==0) KEY=2;
	else{if (KEY3==0) KEY=3;}}	
		switch(KEY)
			{
			case 1:		/* 电机正转  */
					IA(1);			/*  PB8=IA=1  			*/
					IB(0);			/*  PB9=IB=0  			*/
					LEDR(1);		/*  PB1=1 红灯闪烁 	*/
					Delayms(n);
					LEDR(0);
					Delayms(n);
					LEDG(0);		/*  PB2=1 绿灯灭  */
					break;
			case 2:		/* 电机反转  */
					IA(0);			/*  PB9=IB=1  			*/
					IB(1);			/*  PB8=IA=0  			*/
					LEDR(0);		/*  PB1=0 红灯灭  	*/
					LEDG(1);		/*  PB2=1 绿灯闪烁  */
					Delayms(n);
					LEDG(0);
					Delayms(n);
					break;
			case 3:		/* 电机停止  */
					IA(0);			/*  PB9=IB=0  			*/
					IB(0);			/*  PB8=IA=0  			*/
					LEDR(0);		/*  PB1=0 红灯灭  	*/
					LEDG(0);		/*  PB2=1 绿灯灭  	*/
					break;
			}	
	}
}
										  






