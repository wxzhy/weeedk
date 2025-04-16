/********************************************************
*  采用NUAA_CM3_107  WEEEDK实验开发板
*  温度测量测试例程，利用芯片内部温度传感器和外面温度传感器并将结果显示在TFT屏上
插上3.2吋TFT真彩屏到JP25插针上（可选，如已插上），短接JP3将VLCD连接到3.3V，打开电源，LED0和LED6亮电源指示灯亮
（1）短接JP4将V1连接到3.3V，接通DS18B20电源
（2）短接JP15连通DS18B20的DQ至PB10
（3）短接JP18和JP40连通双色发光管
（4）下载示例程序后运行，显示屏显示初始信息：温度值，默认显示内部温度
（5）按下KEY1键，为内部传感器测量的温度，双色的绿灯亮，按下KEY2为外部传感器DS18B20测量的温度，双色的红灯亮
		 当测量外部温度时，用手贴紧温度传感器DS18B20（在右下方喇叭的右上角）
 注意：若下载后不能运行，可按复位键开始
http://www.ndiy.cn/thread-24274-1-1.html关于DS18B20资料的说明
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"
#include "delay.h"
#include <stdio.h>

extern u8 gImage_fg[];
extern u16 BACK_COLOR;
extern u16 POINT_COLOR;
u8 key;


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private functions ---------------------------------------------------------*/


	float temperate;
uint8_t BeepFlag=0;
char res;
//extern float temperate;
uint16_t temp=0;
extern  u8 ADC_Channel,Rank;



/**									 _
	*名称：void Delay(void)
	*参数：无
	*返回：无
	*功能：简单延时
**/

void Delay(void)
{
	int i,j;
	for(i=1000;i>0;i--)
		for(j=1000;j>0;j--);

}



int main(void)
{
  SystemInit();
	GPIO_Configuration();
	ADC_Configuration();	      /*ADC初始化*/
	Delay_Init();               /*使用SysTick实现精确延时*/
	LCD_Init();									/*LCD初始化*/
	Welcome(); 									/*显示主界面*/
	LED1(1);LED2(1);LED3(1);LED4(1);LEDR(0);LEDG(0);
	LEDR(1);LEDG(0);
	 	while(1)
	{		
			LEDR(0);LEDG(1);

		Read_ADC1_MultiChannel(ADC_Channel_TempSensor,1);/*ADC_GetConversionValue(ADC1);*/
			temp = Read_ADC1_MultiChannel(ADC_Channel_TempSensor,1);/*ADC_GetConversionValue(ADC1);*/
			LCD_ShowNum(100, 150, temp, 5,16);    				/*显示A/D变换的数字量	*/
			temperate=(1.43-(3.3/4096)*temp)/0.0043+25;   /*计算出当前温度值,参见《嵌入式系统原理及应用教材（第三版）》第7章7.4.1相关内容*/	 
			if(temperate<0)												  			/*读出负温度*/
			{
			Show_Str(90,190,WHITE,BLACK,"-",16,0);
			}					 
			else 
			Show_Str(90,190,WHITE,BLACK," ",16,0);
 		
			LCD_ShowNum(100, 190, (u8)temperate, 2,16);    /*显示温度整数部分	*/
			Show_Str(100+8*2,190,WHITE,RED,".",16,0);
			temperate-=(u8)temperate;	  									 /*减去整数部分，剩下小数部分*/	
			LCD_ShowNum(100+8*3,190,temperate*100,2,16);   /*显示温度小数部分*/	
			Show_Str(105+8*5,190,WHITE,BLACK,"℃",16,0);

			Read_ADC1_MultiChannel(ADC_Channel_Vrefint,1);/*ADC_GetConversionValue(ADC1);*/
			temp = Read_ADC1_MultiChannel(ADC_Channel_Vrefint,1);/*ADC_GetConversionValue(ADC1);*/
			LCD_ShowNum(100, 230, temp, 5,16);    				/*显示A/D变换的数字量	*/
			LCD_ShowNum(100, 270, temp*3.3*1000/4096, 5,16);    				/*显示A/D变换的数字量	*/
			Show_Str(105+8*5,270,WHITE,BLACK,"mV",16,0);
			Delay_ms(100);

	}

}

/**********************END OF FILE****/
