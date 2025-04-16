
/************************************************************************************
**	  WEEEDK NUAA_CM3_107实验开发板（V4.1）
**	  GPIO应用:简易人机交互接口实验
**    操作及现象：下载程序运行后看到四个LED指示灯闪光(跑马灯)
			当按下KEY1键时，LED1闪光，当按下KEY2键时，LED2闪光，当按下KEY3键时，LED3闪光，当按下KEY4键时，LED4闪光
			以原有代码基础上，添加代码让按一次按键，蜂鸣器响一声
**    版    本：V4.1
**	  作    者：NUAA
**	  完成日期:	2017.3.19
************************************************************************************/
#include "stm32f10x_conf.h"

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
*函数名称：GPIO_KEYLED_Init(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：初始化GPIO，按键和LED初始化
**********************************************/
void GPIO_KEYLED_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);  /*初始化GPIOD输出端口*/
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);  /*初始化GPIOD端口PD11=KEY1,PD12=KEY2上拉输入，50MHz*/

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);  /*初始化GPIOC端口PC13=KEY3上拉输入，50MHz*/
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);  /*初始化GPIOC端口PC0推挽输出，50MHz*/
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);  /*初始化GPIOA端口PA0=KEY4上拉输入，50MHz*/
	/*---------初始化状态四个LED全灭OFF------------*/
	GPIO_SetBits(GPIOD,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7);
}

#define n 200
u8 KEY=0;
void Beep(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	Delayms(n);
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
}

int main(void)
{
//u8 KEY=0;	
u8 i=0;
u16 Pinx[4]={GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4,GPIO_Pin_7};
	SystemInit();     		/* 系统初始化 */
	GPIO_KEYLED_Init();		/* GPIO初始化	*/
	while (1)
	{	
	//if (GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)==0) 			 KEY=1;
	if ((GPIOD->IDR&(1<<11))==0) 			 KEY=1;
	//else {if (GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)==0) KEY=2;
	else {if ((GPIOD->IDR&(1<<12))==0) KEY=2;
	//else {if (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==0) KEY=3;
	else {if ((GPIOC->IDR&(1<<13))==0) KEY=3;
	//if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0)  			 KEY=4;}}
	if ((GPIOA->IDR&1)==0)  			 KEY=4;}}
		  //if(KEY==0){GPIO_ResetBits(GPIOD,Pinx[i]);	/*  LEDi+1亮 		*/	
			if(KEY==0){GPIOD->ODR&=~Pinx[i];
					Delayms(n);													/*  延时n ms  	*/
					//GPIO_SetBits(GPIOD,Pinx[i]);	 			/*  LEDi+1灭  	*/
					GPIOD->ODR|=Pinx[i];
					Delayms(n);
				  i++;
					if (i>=4) i=0;	}
			//else {GPIO_ResetBits(GPIOD,Pinx[KEY-1]);/*  LED[KEY-1]亮  */
			else {GPIOD->ODR&=~Pinx[KEY-1];
					Beep();
					Delayms(n);													/*  延时n ms      */
					//GPIO_SetBits(GPIOD,Pinx[KEY-1]);		/*  LED[KEY-1]灭  */
				  GPIOD->ODR|=Pinx[KEY-1];
					Delayms(n);		
					KEY=0;
			}
	}
}
										  









