/********************************************************
* 1.本实验硬件平台：NUAA_CM3_107实验开发板，MCU：STM32F107VCT6             
				选择配件：TFT LCD 240*320（3.2吋真彩）液晶显示屏
* 2.开发和实验环境：MDK-ARM Keil V5.15及其以上版本
* 3.例程使用的STM32F10x固件库V3.5.0
* 4.实验内容：LCD屏显示文字图像等
*  3.2吋TFT真彩屏（ILI9341：240*320）显示实验
*  采用IO模拟16位总线方式驱动TFT液晶驱动 
*  可选择横向和纵向显示：USE_HORIZONTAL=0竖屏显示，USE_HORIZONTAL=1横屏显示（USE_HORIZONTAL在LCD.h中定义）
*  实验现象：下载程序JTAG或SWD后运行程序，依次显示测试界面
*            当按下KEY1键时显示测试1，2，3为图形颜色等测试
*            当按下KEY2键时显示测试4，5为字符及汉字测试
*            当按下KEY3键时显示测试6为图像显示测试，显示若干QQ图标的图像
*            当按下KEY4键时显示测试5显示汉字测试
*  通过汉字取模软件可以自行选择关注的汉字，将汉字编码，按照示例放入font.h相应位置，可以显示16*16，24*24，32*32和64*64的汉字         
//=======================================液晶屏数据线接线==========================================//
	STM32F107VCT6 PE0~PE15组接液晶屏DB0~DB15,举例依次为DB0接PE0,..DB15接PE15.   
//=======================================液晶屏控制线接线==========================================//
	LCD_CS	接PC6	//片选信号
	LCD_RS	接PD13	//寄存器/数据选择信号
	LCD_WR	接PD14	//写信号
	LCD_RD	接PD15	//读信号
	LCD_RST	接PC5	//复位信号，不用
	LCD_LED	接PB14	//背光控制信号(高电平点亮)
//=========================================触摸屏触接线=========================================//
//不使用触摸或者模块本身不带触摸，则可不连接
	MO(MISO)	接PC11	//SPI总线输出
	MI(MOSI)	接PC12	//SPI总线输入
	PEN		接PC5	//触摸屏中断信号
	TCS		接PC8	//触摸IC片选
	CLK		接PC10	//SPI总线时钟
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"

int main(void)
{
	u8 KEY=0;
	SystemInit();
	GPIO_Configuration();
	LCD_Init();					//LCD初始化	
		main_test(); 		//测试主界面
		Delay_ms(1000);		
		Test_Color();  		//简单刷屏填充测试
		Delay_ms(1000);		
		Test_FillRec();		//GUI矩形绘图测试
		Delay_ms(1000);		
		Test_Circle(); 		//GUI画圆测试
		Delay_ms(1000);		
		English_Font_test();//英文字体示例测试
		Delay_ms(1000);		
		Chinese_Font_test();//中文字体示例测试
		Delay_ms(1000);		
		Pic_test();			//图片显示示例测试
		LED1=1;LED2=1;LED3=1;LED4=1;LED5=1;
	KEY=0;
	while(1)
	{		
	if (PDin(11)==0) {KEY=1;	LED1=0;LED2=1;LED3=1;LED4=1;Test_Color();Delay_ms(1000);Test_Circle();}
	if (PDin(12)==0) {KEY=2;	LED2=0;LED1=1;LED3=1;LED4=1;English_Font_test();Delay_ms(1000);Chinese_Font_test();}
	if (PCin(13)==0) {KEY=3;	LED3=0;LED1=1;LED2=1;LED4=1;Pic_test();}
	if (PAin(0)==0) {KEY=4;	LED4=0;LED1=1;LED2=1;LED3=1;main_test();;}

	   switch(KEY)
	   {
	   case 1:LED1=~LED1;LED2=1;LED3=1;LED4=1;break;
	   case 2:LED1=1;LED2=~LED2;LED3=1;LED4=1;break;
	   case 3:LED1=1;LED2=1;LED3=~LED3;LED4=1;break;
	   case 4:LED1=1;LED2=1;LED3=1;LED4=~LED4;break;
	   }
	Delay_ms(200);
	}				  
}




/******************* ****END OF FILE****/
