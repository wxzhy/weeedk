/********************************************************
DAC应用－软件触发DAC变换，由ADC控制DAC输出大小
1.本实验硬件平台：NUAA_CM3_107实验开发板WEEEDK，MCU：STM32F107VCT6               
2.开发和实验环境：MDK-ARM Keil V5.15及其以上版本
3.例程使用的STM32F10x固件库V3.5.0
4.本实验例程现象说明内容：  
	DAC应用：采用软件触发方式进行D/A变换,由ADC采集的大小控制DAC输出大小
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"

#define Y1  0.00				/*0度*/

#define Ym1  100.00			/*100度*/
#define N1  2483.0		/*0度对应数字量*/
#define Nm1  3420.0		/*100对应数字量*/
#define Y2  0.00				/*0mm*/
#define Ym2  1000.00			/*1000mm*/
#define N2  792.0		/*0度对应数字量*/
#define Nm2  3969.0		/*100对应数字量*/

	extern u16 value;    				/*采样处理后的数字量*/			
	float ADC_Value=0.00;       /*实际物理量值*/                  

int main(void)
{
		SystemInit();
		GPIO_Configuration();
		ADC_Mode_Config();		        						/*ADC1使用DMA方式多通道连续变换初始化设置*/
		DAC_Configuration();
		LCD_Init();																	/*  LCD初始化    */
		Welcome(); 										 							/*  显示主界面   */
		LED1(1);LED2(1);LED3(1);LED4(1);
		Show_Str(20,275,RED,WHITE,"电位器控制DAC输出相应电压",16,0);	
	while(1)
	{		
		value=ReadADCAverageValue(0);								/*获取电位器中心点测量值值*/
		ADC_Value = (3.3*1000/4096)*value;			    /*将AD值转换为电压值mV*/
		LCD_ShowNum(150,140,ADC_Value,5,16);				/*在LCD屏上显示电压值mV*/
		
		 DAC_SetChannel1Data(DAC_Align_12b_R, value);/*将等变换的DC数据放右对齐寄存器中*/
		 DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);	 /*软件启动变换*/
		value=ReadADCAverageValue(1);								/*获取PT100电阻对应温度值*/
		if(value>=0xFF0) 			Show_Str(150,165,YELLOW,BLUE," 未接",16,0);
		else{
		ADC_Value=(float)(Y1+(Ym1-Y1)*(value-N1)/(Nm1-N1));	  /*通过线性标度变换得到温度0~100度*/
			if(ADC_Value<0)												  			/*读出负温度*/
			{
			Show_Str(150-8,165,WHITE,BLUE,"-",16,0);
			}					 
			else 
			Show_Str(150-8,165,WHITE,BLUE," ",16,0);
			LCD_ShowNum(150, 165, (u8)ADC_Value, 3,16);    /*显示温度整数部分	*/
			Show_Str(150+8*3,165,WHITE,BLUE,".",16,0);
			ADC_Value-=(u8)ADC_Value;	  								/*减去整数部分，剩下小数部分*/	
			LCD_ShowNum(150+8*4,165,ADC_Value*10,1,16);   /*显示温度小数部分*/	
				}

		value=ReadADCAverageValue(2);								/*获取水位传感器的平均值 */			
		ADC_Value=(float)(Y2+(Ym2-Y2)*(value-N2)/(Nm2-N2));	  /*通过线性标度变换得到水位*/
		if (value<0x300) Show_Str(150,190,WHITE,BLUE," 未接",16,0);
		else	{if(ADC_Value<=0) ADC_Value=0;;LCD_ShowNum(150,190, ADC_Value,5,16);}						/*在LCD屏上显示采样值*/

			value=ReadADCAverageValue(3);								/*获取内部温度平均值 */	
			ADC_Value=(1.43-(3.3/4096)*value)/0.0043+25;   /*计算出当前温度值,参见《嵌入式系统原理及应用教材（第三版）》第7章7.4.1相关内容*/	 
			if(ADC_Value<0)												  			/*读出负温度*/
			{
			Show_Str(150,215,WHITE,BLACK,"-",16,0);
			}					 
			else 
			Show_Str(150,215,WHITE,BLACK," ",16,0);
 		
			LCD_ShowNum(150, 215, (u8)ADC_Value, 2,16);    /*显示温度整数部分	*/
			Show_Str(150+8*2,215,WHITE,BLUE,".",16,0);
			ADC_Value-=(u8)ADC_Value;	  
			LCD_ShowNum(150+8*3,215,ADC_Value*100,2,16);   /*显示温度小数部分*/	
		
		value=ReadADCAverageValue(4);								/*获取内部基准电压采样平均值 */	
		ADC_Value = (3.3*1000/4096)*value;			    /*将AD值转换为电压值mV*/
		LCD_ShowNum(150,240, value,5,16);						/*在LCD屏上显示采样值*/

			Delay_ms(500);
		GPIOD->ODR^=(1<<2);													/*让LED1闪烁显示*/
	}
}


/***********************END OF FILE****/
