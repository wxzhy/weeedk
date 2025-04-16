#include "stm32f10x.h"
#include "ultrasonic.h"
#include"delay.h"

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

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;		//返回电平引脚PA2
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
		TRIG_Send=1;
		Delay_ms(10);
		TRIG_Send=0;
		while(!ECHO_Reci);
		while(ECHO_Reci)
		{
			Delay_us(10);
			j++;
		}
		Ultr_Temp+=340/2*j*10;//  模块最大可测距3m 
		j=0;
		Delay_ms(60);//防止发射信号对回响信号的影响
	}
	*p=Ultr_Temp/5/1000000; 	
	//*p=Ultr_Temp/1000000; 	
}
