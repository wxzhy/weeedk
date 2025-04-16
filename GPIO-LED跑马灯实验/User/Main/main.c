
/************************************************************************************
**	  WEEEDK NUAA_CM3_107实验开发板（V4.1/V4.2）
**	  GPIO-LED应用跑马灯实验
**    操作及现象：下载程序运行后看到四个LED指示灯闪光（轮流亮灭，似跑马灯）
**    版    本：V4.1
**	  作    者：NUAA
**	  完成日期:	2017.3.18  
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
void GPIO_LED_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); /*使能PD口时钟*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7 ;/*选择PD2/PD3/PD4/PD7引脚*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;  		/*频率10MHz*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			/*推挽输出*/
	GPIO_Init(GPIOD, &GPIO_InitStructure);                /*初始化GPIOD端口*/	
	/*---------初始化状态四个LED全灭OFF------------*/
	GPIO_SetBits(GPIOD,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7);/*PD2/PD3/PD4/PD7输出为高电平，四LED全灭*/
}

#define  n 300    /*延时时间常数*/

int main(void)
{
//	SystemInit();     	//系统初始化，此处可以省，因为在启动文件中有该函数的调用
	GPIO_LED_Init();		//GPIO初始化
	
	while (1)
	{	
  GPIO_ResetBits(GPIOD,GPIO_Pin_2);/* PD2=0 LED1亮  */
	Delayms(n);											 /* 延时          */
  GPIO_SetBits(GPIOD,GPIO_Pin_2);	 /* PD2=1 LED1灭  */
	Delayms(n);											 /* 延时          */
  GPIO_ResetBits(GPIOD,GPIO_Pin_3);/* PD3=0 LED2亮  */
	Delayms(n);											 /* 延时          */
  GPIO_SetBits(GPIOD,GPIO_Pin_3);  /* PD3=1 LED2灭  */
	Delayms(n);											 /* 延时          */
  GPIO_ResetBits(GPIOD,GPIO_Pin_4);/* PD4=0 LED3亮  */
	Delayms(n);											 /* 延时          */
  GPIO_SetBits(GPIOD,GPIO_Pin_4);  /* PD4=1 LED4灭  */
	Delayms(n);											 /* 延时          */
  GPIO_ResetBits(GPIOD,GPIO_Pin_7);/* PD7=0 LED4亮  */
	Delayms(n);											 /* 延时          */
  GPIO_SetBits(GPIOD,GPIO_Pin_7);  /* PD7=1 LED4灭  */
	Delayms(n);										 	 /* 延时          */
	}	
}
										  








