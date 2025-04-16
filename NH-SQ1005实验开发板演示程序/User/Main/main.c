/********************************************************
*  NUAA_CM3_107  NH-SQ1005嵌入式实验开发板测试程序
2018.9.27
注意短接相磁引脚
（1）P2上的PB12短接到GND则为语音播放操作
（2）PA1连接GND，则可测试(1)4位共阳LED  （2）OCWJ4CX8C LCD  (3) HJ12864  (4) OLED  (5) TFT 320X240 LCD
（3）PA8连接GND,时钟闪烁进入设置状态，设置时间
（4）PA11连接GND,采用PWM输出控制电机和外接3.3V灯泡，否则GPIO控制电机

 通过红外遥控和蓝牙来控制板载模块以测试其运行情况

  PB13在线路板上与GND短接了，切掉重新连接到LED5的短接器上
 Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "remote.h"
#include "HJ12864-COG-1.h"
#include "lcd.h"
#include "rtc.h"
#include "WEEEDK.h"
#include "delay.h"
#include "DS18B20.h"
#include <stdio.h>
#define Y1  0.00				/*0度*/

#define Ym1  100.00			/*100度*/
#define N1  2502       //2483.0		/*0度对应数字量*/
#define Nm1  3437//3420.0		/*100对应数字量*/
#define Y2  0.00				/*0mm*/
#define Ym2  1000.00			/*1000mm*/
#define N2  688//681      //792.0		/*0度对应数字量*/
#define Nm2  	3513//3485 //3969.0		/*100对应数字量*/
extern u8 gImage_fg[],State,Shan;
u8 key;
u8 TimeDisplay = 0;		


/* DAC地址0x4000 7400 - 0x4000 77FF，
   0x40007420地址属于DAC_DHR12RD
   双DAC的12位右对齐数据保持寄存器
*/
uint32_t DAC_DHR12R2_Address = 0x40007414;	  //0x4000 7400 - 0x4000 77FF为DAC地址范围，其中0x40007414为DAC2右对齐数据保持寄存器
//uint32_t DAC_DHR12RD_Address = 0x40007420;	  //0x4000 7400 - 0x4000 77FF为DAC地址范围，其中0x40007420为双通道DAC右对齐数据保持寄存器

const uint16_t Sine12bit[32] = {   /*32正弦波点*/
                      2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
                      3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909, 
                      599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};

uint32_t DAC2Sine12bit[32];
const uint16_t frebit[24] = {   /*1,2,3,4,5,6,7,1频率*/
                     0,523,587,659,698,784,880,988,523*2,587*2,659*2,698*2,784*2,880*2,988*2,523*4,523*4,523/2,587/2,659/2,698/2,784/2,880/2,988/2};//1300, 1462, 1625, 1733, 1950, 2166, 2437, 2600,2600, 2437,2166, 1950, 1733,1625,1462,1300};

const uint16_t Redfrebit[ ] = {784*2,784*2,784*2,880*2,587*2,587*2,587*2,587*2,523*2,523*2,523*2,880,  587*2,587*2,587*2,587*2,
														 784*2,784*2,784*2,784*2,880*2,523*4,880*2,784*2,523*2,523*2,523*2,880,  587*2,587*2,587*2,587*2,
														 784*2,784*2,587*2,587*2,523*2,523*2,988,  880,  784,  784,  784*2,784*2,587*2,587*2,659*2,587*2,
														 523*2,523*2,523*2,880,  587*2,659*2,587*2,523*2,587*2,523*2,988,  880,  784,  784,  784,  784,  
														 784,  784,  784,784,
															};   /*1,2,3,4,5,6,7,1频率*/
//                              3     2    3      5     6     5     1'    6      5    3      5    5       5     5       6     6    (16)
const uint16_t  Jasmine[ ] = {659*2,587*2,659*2,784*2,880*2,784*2,523*4,880*2,784*2,659*2,784*2,784*2, 784*2,784*2, 880*2,880*2,
	//                   			   1'    1'    2'    3'	    2'    1'    6    1'     5    3      5     5      5      5       5     5 (16)
															523*4,523*4,587*4,659*4,587*4,523*4,880*2,523*4,784*2,659*2,784*2,784*2,  784*2,784*2, 784*2,784*2,														 
	//                   			   5     3      5    5	    5    5    6      6    1'   1'     2'    3'      6     6       5     5 (16)
															784*2,659*2,784*2,784*2,784*2,784*2,880*2,880*2,523*4,523*4,587*4,659*4,  880*2,880*2, 784*2,784*2,														 
	//                   			   5     5      2    2	    3    5    3      2    1      6.     1    1      1     1       1     1 (16)
															784*2,784*2,587*2,587*2,659*2,784*2,659*2,587*2,523*2,880,523*2,523*2,  523*2,523*2, 523*2,523*2, 														 
	//                   			   3     2      1    1	    2    2    2      3    5      5     6    1'      6     6       5     5 (16)
															659*2,587*2,523*2,523*2,587*2,587*2,587*2,659*2,784*2,784*2,880*2,523*4,  880*2,880*2, 784*2,784*2, 														 
	//                   			   5     5      2    2	    3    5    3      2    1      2     6.    6.'      6.   6.      1     1 (16)
															784*2,784*2,587*2,587*2,659*2,784*2,659*2,587*2,523*2,587*2,880, 880,   880,   880,   523*2, 523*2, 														 
//															 2     2      2    3	    1    2    1      6.    1     6.     5.    5.'      5.   5.      5.     5. (16)
															587*2,587*2,587*2,659*2,523*2,587*2,523*2,880,523*2,  880,   784, 784,       784,  784,  784, 784, 														 
															};   /*1,2,3,4,5,6,7,1频率*/
const uint16_t  dolaimi[ ] = {523/2,523,523*3,523*4,523*5,880*2,784*2,523*3,880*2,784*2,659*2,784*2,784*2, 784*2,784*2, 784*2,784*2,784*2,880*2,
	//                   			   1'    1'    2'    3'	    2'    1'    6    1'     5    3      5     5      5      5       5     5    5     5 (18)
															523*3,523*3,587*3,659*3,587*3,523*3,880*2,523*3,784*2,784*2,784*2,7840,  784*2,784*2, 784*2,784*2, 784*2,784*2, 587*2,587*2,587*2,587*2,
														 784*2,784*2,587*2,587*2,523*2,523*2,988,  880,  784,  784,  784*2,784*2,587*2,587*2,659*2,587*2,
														 523*2,523*2,523*2,880,  587*2,659*2,587*2,523*2,587*2,523*2,988,  880,  784,  784,  784,  784,  
														 784,  784,  784,784,
															};   /*1,2,3,4,5,6,7,1频率*/

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#define times 2     //决定LED数码管显示时间
uint32_t segNum;			//数码管要显示的数字
uint8_t seg[] ={0x41,0xE7,0x52,0x62,0xE4,0x68,0x48,0xE3,0x40,0x60,};	   //数码管数字
uint8_t LEDDataSeg1[] ={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xa1,0x86,0x8E,0x8c,0xC1,0x91,0x7f,0xbf,0xC7,0x89,0xce,0xcf,0xaf,0xf1,0x9c,0xc8,0xff,};	   //数码管数字
//                       0   1     2    3   4    5     6    7    8    9    A   b     C   d    E    F     P    U   Y		.	 -  L    H    T    I     r    J   o    N   不显示
//                       0   1     2    3   4    5     6    7    8    9    10  11    12  13   14   15    16   17  18   19    20	 21	 22	  23   24    25   26  27   28   29
uint8_t LEDDataSeg[5] ={0x99,0xC7,0x86,0xa1,0xC7};	   //数码管数字
//                     4     L   E    d     H   
/**									 _
	*名称：void Delay(void)
	*参数：无
	*返回：无
	*功能：简单延时
**/

void Delayus(uint32_t k)
{
	int i,j;
	for(i=k;i>0;i--)
		for(j=1000;j>0;j--);

}
void Delay(void)
{
	int i,j;
	for(i=1000;i>0;i--)
		for(j=1000;j>0;j--);

}
extern u8  INDEX;
char str1[8];
uint8_t strd[8],strhw[8],SoundEnable;
extern u8 		USART_RX_BUF[10];		
uint8_t BeepFlag=0,ConnectBitPt100=0,ConnectBitWater=0,ConnectBitCSB=0;
uint8_t res,LimitBit=0,ReadFlag;
	u16 valued,Vdata1,Vdata2,Vdata3,Vdata5,Vdata,VdataBCD;   				/*采样处理后的数字量*/			
	float ADC_Value=0.00,ADC_ValueV=0.00,ADC_ValuePt100=0.00,ADC_ValueWater=0.00;       /*实际物理量值*/                  
uint8_t LEDCODE;
int main(void)
 {
//	 RTC_DateTimeTypeDef RTC_DateTimeStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	float temp;
	char DisNum[16];
uint8_t KEY=0;//,iRTC=0;
uint8_t Idx,i;
//uint16_t f=523;
//uint8_t str[40];
	SystemInit();
	Delay_Init();               //使用SysTick实现精确延时
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_11|GPIO_Pin_12);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  	GPIO_Configuration();
	 LED5(1);
	USART123_Configuration();
BIT1(1);BIT2(1);BIT3(1);BIT4(1);
	PCout(4)=1;	//继电器不动作	
	PCout(15)=1;//继电器不动作		
	  

  USART2_SendString("\r\n 南航嵌入式系统综合实验开发平台演示系统欢迎您的使用！\r\n");

  USART2_SendString("\r\n 操作提示：多种显示方式展示（LED/LCD/OLED），请用杜邦线连接PA1到GND，否则直接采用TFT屏！\r\n");
  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	      /*RTC中断使能*/

if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))//短接PA1到GND可测试各种非TFT屏：默认LED、KEY1：OCWJ4X8C,KEY2:
{   
  USART2_SendString("\r\n 多种显示方式展示：\r\n");
  USART2_SendString("\r\n （1）4位LED共阳数码管显示，KEY1继续换OCWJ4X8C LCD屏\r\n");
	
	 while (1)
	{  //4位 LED测试，
		
	if (!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11)) break;	//KEY1
	 for (i=0;i<=3;i++)
	 {
	 	switch(i)
		{
		case 3: 
			BIT1(0);BIT2(1);BIT3(1);BIT4(1);GPIO_Write(GPIOE,((GPIO_ReadOutputData(GPIOE)&0xFF00)|LEDDataSeg[i]));break;
		case 2: 
			BIT1(1);BIT2(0);BIT3(1);BIT4(1);GPIO_Write(GPIOE,((GPIO_ReadOutputData(GPIOE)&0xFF00)|LEDDataSeg[i]));break;
		case 1: 
			BIT1(1);BIT2(1);BIT3(0);BIT4(1);GPIO_Write(GPIOE,((GPIO_ReadOutputData(GPIOE)&0xFF00)|LEDDataSeg[i]));break;
		case 0: 
			BIT1(1);BIT2(1);BIT3(1);BIT4(0);GPIO_Write(GPIOE,((GPIO_ReadOutputData(GPIOE)&0xFF00)|LEDDataSeg[i]));break;
		}
		Delayus(times);
	 	
	 }
		Delayus(times);
 }
	LCD_Configuration();    //OCWJ4X8C液晶屏
	LCD_Init_OCWJ4X8C();			
	displayInformation();
  USART2_SendString("\r\n （2）图形点阵LCD模块OCWJ4X8C液晶屏显示，KEY2换HJ12864-COG-1廉价图形点阵LCD屏\r\n");
 while(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12));//KEY2:HJ12864
	
	LCD_HJ12864Configuration();
	LCD_HJ12864_Init();			/* HJ12864 LCD初始化 */
	clear_screenHJ();
  displayInformationHJ();//初始信息显示
   USART2_SendString("\r\n （3）HJ12864-COG-1廉价图形点阵LCD显示，KEY3换1.3吋128X64 OLED屏\r\n");
while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13));	//KEY3:OLED	
		OLED_Init();		
		OLED_Clear()  	; 
		//for (i=0;i<8;i++) OLED_ShowCHinese(i*16,0,i+8);	//欢迎使用本开发板8
			OLED_ShowString(0,0,"NH-SQ1005 CM3Kit");  
		for (i=0;i<8;i++) OLED_ShowCHinese(i*16,02,i+8);	//欢迎使用本开发板
			OLED_ShowString(0,4,"NUAA_CS MWH OLED");  
		for (i=0;i<8;i++) OLED_ShowCHinese(i*16,6,i);	//南京航空航天大学  
   USART2_SendString("\r\n （4）1.3吋128X64 OLED屏显示，KEY4换3.2吋 320X240 真彩LCD屏\r\n");
 while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));	//KEY4:TFT	
}	

  LCD_Init();					//真彩LCD初始化
	GPIO_Configuration();

	UART4_Configuration();

USART2_SendString("\r\n 初始画面展示的是图片、文字的显示，用到RTC时间、定时器、DAC以及GPIO等组件！\r\n");
USART2_SendString("\r\n 设置时钟时将PA8与GND短接，KEY3选择闪烁位，KEY1,KEY2加减时间，KEY4键确定。\r\n");

USART2_SendString("\r\n 在初始画面中，利用DAC产生正弦波通过功率放大推动喇叭发声，演奏茉莉花乐曲，也可操作电机运转。\r\n");
USART2_SendString("\r\n 按KEY1键让喇叭演奏乐曲。\r\n");
USART2_SendString("\r\n 按KEY2键让板载直流电机运转。\r\n");
USART2_SendString("\r\n 按KEY3键让板载直流电机停止运转且停止演奏音乐。\r\n");
USART2_SendString("\r\n 按KEY4键开始综合实验的演示，进入下一个界面。\r\n");

	Gui_Drawbmp16(0,0,gImage_WEEEDK);
	Gui_StrCenter(1,16,YELLOW,WHITE,"南航 NH-SQ1005 KIT",16,1);//居中显示
	Gui_StrCenter(0,15,RED,WHITE,"南航 NH-SQ1005 KIT",16,1);//居中显示
	if (USE_HORIZONTAL==1)	Show_Str(45,40,WHITE,BLACK,"嵌入式系统综合实验",24,1);
	else					Show_Str(10,40,WHITE,BLACK,"嵌入式系统综合实验",24,1);
	if (USE_HORIZONTAL==1)	Show_Str(47,42,BLUE,BLACK,"嵌入式系统综合实验",24,1);
	else					Show_Str(12,42,BLUE,BLACK,"嵌入式系统综合实验",24,1);

if (USE_HORIZONTAL==1)	Show_Str(28,98,RED,BLACK,"欢迎您",64,1);
	else					Show_Str(28,98,RED,BLACK,"欢迎您",64,1);
if (USE_HORIZONTAL==1)	Show_Str(25,95,YELLOW,BLACK,"欢迎您",64,1);
	else					Show_Str(25,95,YELLOW,BLACK,"欢迎您",64,1);
//Gui_StrCenter(0,46,RED,BLUE,"嵌入式系统原理及应用配套实验板",16,1);//居中显示
Gui_StrCenter(0,230,WHITE,BLACK, " KEY1:Music  KEY3:Stop!  ",16,0);//居中显示	
Gui_StrCenter(0,250,WHITE,BLACK, " KEY2:Moving KEY4:Start! ",16,0);//居中显示	
	Gui_StrCenter(1,270,WHITE,BLACK,"    按功能4键KEY4开始    ",16,0);//居中显示	
	//Gui_StrCenter(0,294,YELLOW,BLACK,"南航NH-SQ综合实验平台演示系统",16,0);//居中显示	
	Gui_StrCenter(2,296,WHITE,BLACK,"南航NH-SQ综合实验平台演示系统",16,0);//居中显示	
	for (Idx = 0; Idx < 32; Idx++)
	{
		DAC2Sine12bit[Idx] = (Sine12bit[Idx] << 16) + (Sine12bit[Idx]);	  //双正弦
	}
//	LED1(0);LED2(0);LED3(0);LED4(0);
	POINT_COLOR=WHITE;BACK_COLOR=BLACK;
	REMOTE_Init();
	NVIC_Configuration();		
	EXTI_Configuration();		
	DMA_Configuration();	   //DMA初始化
	TIM_Configuration(frebit[3]);	   //TIM3初始化
	DAC_Configuration();	   //DAC2初始化
	Delay_ms(300);
	TIM_Configuration(frebit[1]);	   //TIM3初始化
	Delay_ms(300);
	  	TIM_Cmd(TIM3, DISABLE);	//没有蓝牙命令，不使能定时器（不让DAC输出）
if((GPIOB->IDR&(1<<12))==0)//PB12为是否发声的判断引脚
	SoundEnable=1;
else SoundEnable=0;
 	if (SoundEnable)
	{SetVolume(25);//选择音量最大30
	 Delay_ms(50);
	 SendSoundInf(20);//
  }
	USART1_SendString("\r\n USART1(RS-232C)按KEY4键开始综合实验! \r\n");
  //USART2_SendString("\r\n USART2(USB转串口)按KEY4键开始综合实验! \r\n");
	// ADC1_Mode_Config ();	

//ADC1 初始化设置
//for (i=0;i<11;i++) 		 {SendSoundInf(i);	Delay_ms(500);}//为
			Delay_ms(350);
KEY=0;//iRTC=0;
	Shan=0;State=1;
	RTC_Init(1);
	
while(1)
	 {
		 
	if (SetTimeFlag==0)   //PA8短接GND
	{
	KEY=KEY_Scan(0);
	if(KEY!=0)    					 
	{
		switch (KEY)
		{
			case 1:IncTime();/*增加时间*/break;
			case 2:DecTime();/*减少时间*/break;
			case 3:SelectTime();/*选择时间*/break;			
			case 4:SetTime(); /*设置时间，写入RTC*/break;			
		}	
		KEY=0;
	}		
		Delay_ms(200);        /*设置时闪烁时间*/                    
		RTC_ITConfig(RTC_IT_SEC, DISABLE);	      /*禁止RTC中断,进入设置*/
		GPIOD->ODR&=~(1<<2);/*让PD2＝0，LED1常亮*/
		Shan++;
		DisplayInfSet();		/*时间信息设置*/

	switch(State)  /*根据设置时间状态闪烁不同位置*/
		{
		case 1:if (Shan&1)/* LCD_ShowNum(28,180,RTC_DateTimeStructure.Year+2000,4,16);else */Show_Str(28,180,YELLOW,RED,"    ",16,0);break;
		case 2:if (Shan&1) /*LCD_ShowNum(20+16*6-8,180,RTC_DateTimeStructure.Month,2,16);else	*/Show_Str(20+16*6-8,180,YELLOW,RED,"  ",16,0);break;
		case 3:if (Shan&1) /*LCD_ShowNum(20+16*10-8,180,RTC_DateTimeStructure.Date,2,16);else */Show_Str(20+16*10-8,180,YELLOW,RED,"  ",16,0);break;
		case 4:if (Shan&1) /*LCD_ShowNum(28+16*3,200,RTC_DateTimeStructure.Hour,2,16);else */Show_Str(28+16*3,200,YELLOW,RED,"  ",16,0);break;
		case 5:if (Shan&1) /*LCD_ShowNum(28+16*6,200,RTC_DateTimeStructure.Minute,2,16);else */Show_Str(28+16*6,200,YELLOW,RED,"  ",16,0);break;
		case 6:if (Shan&1) /*LCD_ShowNum(28+16*9,200,RTC_DateTimeStructure.Second,2,16);else */Show_Str(28+16*9,200,YELLOW,RED,"  ",16,0);break;
		}	
	}
	else
	{	
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	      /*RTC中断使能*/
    if (TimeDisplay == 1)										/*RTC秒中断服务程序中置位,显示时间到*/
    {
		 DisplayInf();													/*显示时间日历*/
     TimeDisplay = 0;												/*清时间到标志，以免不到时间频繁让LCD更换显示，但实际时间没有变化，显示没有意义*/
    }
	KEY=KEY_Scan(0);
	}
		
loopmain:
			TIM_Cmd(TIM3, DISABLE);	//没有蓝牙命令，不使能定时器（不让DAC输出		
if(KEY!=0)	
{	if(KEY==1||KEY==2||KEY==3) {BEEP(1);Delay_ms(200);BEEP(0);}
  if(KEY==4) {BEEP(1);Delay_ms(200);BEEP(0);break;}
	switch (KEY)
		{		
		case 1:
			
	USART2_SendString("\r\n 按下KEY1键正在演奏茉莉花乐曲。\r\n");
			
		for (i=0;i<112;i++)
			{
		KEY=KEY_Scan(0);
		if(KEY==2||KEY==3||KEY==4) {
		BEEP(1);Delay_ms(200);BEEP(0);goto loopmain;}
		TIM_Configuration(Jasmine[i]);	   //TIM2赋值改变频率
	Delay_ms(200);
		
			}	
			KEY=0;
			TIM_Cmd(TIM3, DISABLE);	//没有蓝牙命令，不使能定时器（不让DAC输出）			
			break;
		case 2:
	USART2_SendString("\r\n 按下KEY2键乐曲停止，让电机转动。\r\n");
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);	
	  TIM_Cmd(TIM3, DISABLE);	//没有蓝牙命令，不使能定时器（不让DAC输出）
		KEY=0;
		break;
		case 3:
	USART2_SendString("\r\n 按下KEY3键乐曲停止，让电机停止转动。\r\n");
		{
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);	
	  TIM_Cmd(TIM3, DISABLE);	//没有蓝牙命令，不使能定时器（不让DAC输出）
			KEY=0;
		}
		break;
		case 4:break;
	}
	}	
}
	  USART2_SendString("\r\n 按下KEY4键开始进行综合实验演示。\r\n");
TIM_Cmd(TIM3, DISABLE);	//没有蓝牙命令，不使能定时器（不让DAC输出）
		 ADC1_Mode_Config ();		                            //ADC1 初始化设置
Ultran_Init();
	Welcome(); 		//显示主界面
			GPIOC->ODR|=(1<<15);//继电器断开
			GPIOC->ODR|=(1<<4);			
	if(SoundEnable)	
	{SendSoundInf(28);//开始实验
	//			Delay_ms(1400);
  			Delay_ms(400);
  }
	USART2_SendString("\r\n 在综合实验演示界面，展示通过ADC检测温度、水位、等，通过超声波传感器测量距离等。\r\n");
	USART2_SendString("\r\n 展示PWM输出（与电位器电位相关）控制电机或灯泡，可连接PA1到GND,旋转电位器看显示变化，观察电机速度或灯泡亮度变化。\r\n");
	USART2_SendString("\r\n 可以通过按键、红外遥控器操作电机，也要可以通过手机蓝牙操作板载设备，并通过语音输出操作结果。\r\n");
	USART2_SendString("\r\n 按键操作电机（利用GPIO组件推挽输出）：KEY1正转，KEY2反转，KEY3停止。\r\n");
	USART2_SendString("\r\n 通过三键红外遥控器操作电机（通过定时器和GPIO中断检测红外脉冲序列编码）：上翻键正转，下翻键反转，电源键停止。\r\n");
	USART2_SendString("\r\n 通过手机蓝牙操作设备（通过USART3读取蓝牙模块发来的序列琮控制设备）：手机采用蓝牙助手设置了12个按键，可进行相关操作。\r\n");
Init_PWM(10);
	USART2_SendString("\r\n =====================================\r\n");
	USART2_SendString("\r\n 注意：PB12短接到GND则为语音播放操作。\r\n");
	USART2_SendString("\r\n =====================================\r\n");
	
while(1)
	{	
	Ultra_Ranging(&temp);
		Vdata5=temp*1000;
		if ((ConnectBitCSB==0)||(Vdata5>8000)) Show_Str(150,240,YELLOW,RED," 未接",16,0);
		else{				sprintf(DisNum,"%d",Vdata5);
			for (i=0;i<8;i++) strd[i]=DisNum[i];
		LCD_ShowNum(150,240,Vdata5,5,16);				/*在LCD屏上显示电压值mV*/
		//LCD_ShowString(150,240,16,strd,0);LCD_ShowString(150+32,240,16," ",0);*/
			}
	//	LCD_ShowNum(150,240, valued,5,16);						/*在LCD屏上显示采样值*/
			
 		valued=ReadADCAverageValue(0);								/*获取电位器ADC_IN3,NO.1序号为0，对应测量值*/
		ADC_ValueV= (3.3*1000/4096)*valued;			    /*将AD值转换为电压值mV*/
		LCD_ShowNum(150,140,ADC_ValueV,5,16);				/*在LCD屏上显示电压值mV*/
			if (!SelectPWMOut) Init_PWM(valued);//PB8=PWM OUT
			else 		GPIO_Configuration();
	
	Delay_ms(50);

		valued=ReadADCAverageValue(1);								/*获取PT100电阻对应温度值，ADC_IN6,NO.2序号为1*/
		if(valued>=0xFF0) 			{ConnectBitPt100=0;Show_Str(150,165,YELLOW,RED," 未接",16,0);}
		else{ConnectBitPt100=1;
		ADC_ValuePt100=(float)(Y1+(Ym1-Y1)*(valued-N1)/(Nm1-N1)-1.1);	  /*通过线性标度变换得到温度0~100度*/
			if (ADC_ValuePt100>120) {ConnectBitPt100=0;Show_Str(150,165,YELLOW,RED," 未接",16,0);}
		else{	
			ConnectBitPt100=1;
			LCD_ShowNum(150, 165, (u8)ADC_ValuePt100, 3,16);    /*显示温度整数部分	*/
			Show_Str(150+8*3,165,YELLOW,RED,".",16,0);
			ADC_Value-=(u8)ADC_ValuePt100;	  								/*减去整数部分，剩下小数部分*/	
			LCD_ShowNum(150+8*4,165,ADC_ValuePt100*10,1,16);   /*显示温度小	数部分*/	
				}
			}
		Delay_ms(50);

		valued=ReadADCAverageValue(2);								/*获取水位传感器的平均值，ADC_IN7 No.3 序号为2 */			
		ADC_ValueWater=(float)(Y2+(Ym2-Y2)*(valued-N2)/(Nm2-N2));	  /*通过线性标度变换得到水位*/
		if (valued<(N2-200.0)) {ConnectBitWater=0;Show_Str(150,190,YELLOW,RED," 未接",16,0);}
		else	{ConnectBitWater=1;
		if(ADC_ValueWater<=0) ADC_ValueWater=0;;LCD_ShowNum(150,190, ADC_ValueWater,5,16);}						/*在LCD屏上显示采样值*/
    
		if ((ADC_ValueWater<200.0)&&(valued>N2-200.0)&&(SoundEnable==1)) 
					{SendSoundInf(23);Delay_ms(600);SendSoundInf(21);LimitBit=1;Delay_ms(1800);Delay_ms(1800);}//水位低于0.2米，报警且上水
		else {if ((ADC_ValueWater>=250.0)|(valued<N2-200)) 
				  {LimitBit=0;}}
		
		Delay_ms(50);
			valued=ReadADCAverageValue(3);								/*获取内部温度平均值，ADC_IN16,NO.4序号为3		*/	 	
			ADC_Value=(1.43-(3.2/4096)*valued)/0.0043+25;   /*计算出当前温度值,参见《嵌入式系统原理及应用教材（第三版）》第7章7.4.1相关内容*/	 
			if(ADC_Value<0)												  			/*读出负温度*/
			{
			Show_Str(150,215,WHITE,BLACK,"-",16,0);
			}					 
			else 
			Show_Str(150,215,WHITE,BLACK," ",16,0);
 		
			LCD_ShowNum(150, 215, (u8)ADC_Value, 2,16);    /*显示温度整数部分	*/
			Show_Str(150+8*2,215,WHITE,RED,".",16,0);
			ADC_Value-=(u8)ADC_Value;	  
			LCD_ShowNum(150+8*3,215,ADC_Value*100,2,16);   /*显示温度小数部分*/	
		
		Delay_ms(50);
		valued=ReadADCAverageValue(4);								/*获取内部基准电压采样平均值，ADC_IN17,NO.5序号为4 */	
		ADC_Value = (3.3*1000/4096)*valued;			    /*将AD值转换为电压值mV*/
  if(SelectPWMOut)//不是PWM输出（PA11没有短接GND)
	{	
		KEY=KEY_Scan(0);
	if(KEY!=0)
	{
		BEEP(1);Delay_ms(200);BEEP(0);
   switch (KEY)
	 {
		 case 1:
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);	
		LEDR(0);LEDG(1);
  	USART2_SendString("\r\n KEY1让电机正转。");
	if(SoundEnable==1)
	{
		SendSoundInf(29);//电机
				Delay_ms(900);
		SendSoundInf(30);//正转
			Delay_ms(900);
	}		 
		 KEY=0;
		 break;
		 case 2:
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_SetBits(GPIOB, GPIO_Pin_9);	
  	USART2_SendString("\r\n KEY2让电机反转。");
		LEDR(1);LEDG(0);
	if(SoundEnable==1)
	{
		SendSoundInf(29);//电机
				Delay_ms(900);
		SendSoundInf(31);//反转
			Delay_ms(900);
	}		 
		 KEY=0;
		 break;
		 case 3:
		LEDR(0);LEDG(0);
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);	
  	USART2_SendString("\r\n KEY3让电机停止。");
	if(SoundEnable==1)
	{
		SendSoundInf(29);//电机
				Delay_ms(900);
		SendSoundInf(37);//停止
			Delay_ms(900);
	}		 
		 KEY=0;
		 break;
		 case 4:
		LEDR(0);LEDG(0);
		 KEY=0;
		 break;
	 } 
	}	
if (Remote_Rdy)
{
  Remote_Rdy=0;
  sprintf(str1,"%08X",Remote_Odr);

	for (i=0;i<8;i++) strhw[i]=str1[i];

	 if(Remote_Odr!=0)
      switch (Remote_Odr)
    {

	case 0x00FDA05F://设置键码
					key=1;
					IA(0);IB(0);
  	USART2_SendString("\r\n 接收的红外命令和代码为：电源键=停止  代码为：");
	USART2_SendString(strhw);
  	USART1_SendString("\r\n USART1接收的红外命令和代码为：电源键=停止  代码为：");
	USART1_SendString(strhw);
	if(SoundEnable==1)
	{
		SendSoundInf(29);//电机
				Delay_ms(900);
		SendSoundInf(37);//停止
			Delay_ms(900);
	}					key=0;
					LEDR(0);LEDG(0);
		break;
	case 0x00FDB04F://上键码
					IA(1);IB(0);
					LEDR(1);LEDG(0);  //红亮绿灭
  	USART2_SendString("\r\n 接收的红外命令和代码为：上翻键=正转  代码为：");
	USART2_SendString(strhw);
  	USART1_SendString("\r\n USART1接收的红外命令和代码为：上翻键=正转  代码为：");
	USART1_SendString(strhw);
	if(SoundEnable==1)
	{
		SendSoundInf(29);//电机
				Delay_ms(900);
		SendSoundInf(30);//正转
			Delay_ms(900);
	}		 				key=2;
		break;
	case 0x00FD8877://下键码
					IA(0);IB(1);
					LEDR(0);LEDG(1);   //绿亮红灭
  	USART2_SendString("\r\n 接收的红外命令和代码为：下翻键=反转  代码为：");
	USART2_SendString(strhw);
  	USART1_SendString("\r\n USART1接收的红外命令和代码为：下翻键=反转  代码为：");
	USART1_SendString(strhw);
if(SoundEnable==1)
	{
		SendSoundInf(29);//电机
				Delay_ms(900);
		SendSoundInf(31);//反转
			Delay_ms(900);
	}		 					key=3;
		break;
	default:
					IA(1);IB(1);	//同时亮			
  	USART2_SendString("\r\n 接收的红外命令和代码为：其它键=无效  代码为：");
	USART2_SendString(strhw);
  	USART1_SendString("\r\n USART1接收的红外命令和代码为：其它键=无效  代码为：");
	USART1_SendString(strhw);
					key=0;
		break;

	}
 /* Delay_ms(100);
	BEEP(1);
	Delay_ms(100);
	BEEP(0);*/

}
}
if (BeepFlag)	   //有蓝牙有效命令响一声蜂鸣器
	{

		USART2_SendString("\r\n 接收蓝牙的命令为：");
		
		USART1_SendString("\r\n USART1接收蓝牙的命令为：");
		UART3_Operation();
  
		
	Delay_ms(200);
	BEEP(1);
	Delay_ms(200);
	BEEP(0);
		if(ReadFlag!=0)
	{	
		
		Vdata1=ADC_ValueV;
		Vdata2=ADC_ValuePt100*10;
		Vdata3=ADC_ValueWater;
	switch (ReadFlag)
  {
		case 1:
			sprintf(str1, "%d", Vdata1);
			for (i=0;i<8;i++) strhw[i]=str1[i];		
			USART3_SendString("\r\n V=");USART3_SendString(strhw);USART3_SendString("mV");//USART3_SendString("\r\n");
			USART2_SendString("读电位器电压\r\n 电位器电位值=");USART2_SendString(strhw);USART2_SendString("毫伏\r\n");//USART3_SendString("\r\n");
	if(SoundEnable==1)
	{
	SendSoundInf(22);//电位器电压
				Delay_ms(18500);
		SendSoundInf(19);//为
			Delay_ms(400);
		Vdata=Vdata1;
		SoundFormNum(0);
		SendSoundInf(15);	//毫伏	
			Delay_ms(800);	  
	}
		break;
		case 2:
			if(ConnectBitPt100==1)
			{
			sprintf(str1, "%3.1f", ADC_ValuePt100);
			for (i=0;i<8;i++) strhw[i]=str1[i];		
			USART3_SendString("\r\n T=");USART3_SendString(strhw);USART3_SendString("du");//USART3_SendString("\r\n");
			USART2_SendString("读取温度\r\n PT100温度测量值=");USART2_SendString(strhw);USART2_SendString("度\r\n");//USART3_SendString("\r\n");
			sprintf(str1, "%d", Vdata2);
			for (i=0;i<8;i++) strhw[i]=str1[i];		
		if(SoundEnable==1)
	{
		SendSoundInf(16);//Pt100
				Delay_ms(1000);
		SendSoundInf(13);//温度
				Delay_ms(600);
		SendSoundInf(19);//为
			Delay_ms(300);
		Vdata=Vdata2;
		SoundFormNum(1);
	SendSoundInf(14);	//度
			Delay_ms(300);	  
	}
			}
			else
				{
			USART3_SendString("\r\n Pt100 sensor not connected");//USART3_SendString("\r\n");
			USART2_SendString("读取温度\r\n Pt100传感器没有连接！\r\n");//USART3_SendString("\r\n");
	if(SoundEnable==1)
	{
	SendSoundInf(16);//Pt100
				Delay_ms(1000);
		SendSoundInf(13);//温度
				Delay_ms(600);
		SendSoundInf(24);//传感器
				Delay_ms(900);
		SendSoundInf(25);//未连接
				Delay_ms(900);				
	}
	}
		break;
		case 3:			
			if(ConnectBitWater==1)
			{	
			sprintf(str1, "%d", Vdata3);
			for (i=0;i<8;i++) strhw[i]=str1[i];		
			USART3_SendString("\r\n H=");USART3_SendString(strhw);USART3_SendString("mm");//USART3_SendString("\r\n");
			USART2_SendString("读取水位高度\r\n 水位高度=");USART2_SendString(strhw);USART2_SendString("毫米\r\n");//USART3_SendString("\r\n");
	if(SoundEnable==1)
	{
		SendSoundInf(23);//水位
				Delay_ms(600);
		SendSoundInf(17);//高度
				Delay_ms(600);
		SendSoundInf(19);//为
			Delay_ms(200);
		Vdata=Vdata3;
		SoundFormNum(0);
	SendSoundInf(12);	//毫米
			Delay_ms(600);	  
	}
	}
		else
				{
		USART3_SendString("\r\nwater level sensor not connected");//USART3_SendString("\r\n");
		USART2_SendString("读取水位高度\r\n 水位传感器没有连接! \r\n");//USART3_SendString("\r\n");
	if(SoundEnable==1)
	{
		SendSoundInf(23);//水位
				Delay_ms(600);
		SendSoundInf(24);//传感器
				Delay_ms(900);
		SendSoundInf(25);//未连接
				Delay_ms(900);				
	}
	}
				break;
		case 5:			
			if(ConnectBitCSB==1)
			{	
			//
			for (i=0;i<8;i++) strhw[i]=strd[i];		
			USART3_SendString("\r\n L=");USART3_SendString(strhw);USART3_SendString("mm");//USART3_SendString("\r\n");
			USART2_SendString("读取超声波测量的距离\r\n 距离=");USART2_SendString(strhw);USART2_SendString("毫米\r\n");//USART3_SendString("\r\n");
	if(SoundEnable==1)
	{
		SendSoundInf(27);//超声波
				Delay_ms(900);
		SendSoundInf(26);//距离
				Delay_ms(500);
		SendSoundInf(19);//为
			Delay_ms(300);
		Vdata=Vdata5;
		SoundFormNum(0);//
		SendSoundInf(12);	//毫米
			Delay_ms(600);	  
	}
	}
		else
				{
		USART3_SendString("\r\nwater level sensor not connected");//USART3_SendString("\r\n");
		USART2_SendString("读取水位高度\r\n 水位传感器没有连接! \r\n");//USART3_SendString("\r\n");
	if(SoundEnable==1)
	{
		SendSoundInf(27);//超声波
				Delay_ms(700);
		SendSoundInf(24);//传感器
				Delay_ms(900);
		SendSoundInf(25);//未连接
				Delay_ms(900);				
	}
	}
				break;  
 		case 211:			
	if(SoundEnable==1)
	{
		SendSoundInf(36);//LED
				Delay_ms(900);
		SendSoundInf(1);//1
				Delay_ms(500);
		SendSoundInf(34);//亮
			Delay_ms(300);
	}
		break;
 		case 201:			
	if(SoundEnable==1)
	{
		SendSoundInf(36);//LED
				Delay_ms(1000);
		SendSoundInf(1);//1
				Delay_ms(300);
		SendSoundInf(35);//灭
			Delay_ms(300);
	}
		break;
 		case 212:			
	if(SoundEnable==1)
	{
		SendSoundInf(36);//LED
				Delay_ms(1000);
		SendSoundInf(2);//2
				Delay_ms(300);
		SendSoundInf(34);//亮
			Delay_ms(300);
	}
		break;
 		case 202:			
	if(SoundEnable==1)
	{
		SendSoundInf(36);//LED
				Delay_ms(1000);
		SendSoundInf(2);//2
				Delay_ms(300);
		SendSoundInf(35);//灭
			Delay_ms(300);
	}
		break;
 		case 213:			
	if(SoundEnable==1)
	{
		SendSoundInf(36);//LED
				Delay_ms(1000);
		SendSoundInf(3);//3
				Delay_ms(300);
		SendSoundInf(34);//亮
			Delay_ms(300);
	}
		break;
 		case 203:			
	if(SoundEnable==1)
	{
		SendSoundInf(36);//LED
				Delay_ms(1000);
		SendSoundInf(3);//3
				Delay_ms(300);
		SendSoundInf(35);//灭
			Delay_ms(300);
	}
		break;
 		case 214:			
	if(SoundEnable==1)
	{
		SendSoundInf(36);//LED
				Delay_ms(1000);
		SendSoundInf(4);//4
				Delay_ms(300);
		SendSoundInf(34);//亮
			Delay_ms(300);
	}
		break;
 		case 204:			
	if(SoundEnable==1)
	{
		SendSoundInf(36);//LED
				Delay_ms(1000);
		SendSoundInf(4);//4
				Delay_ms(300);
		SendSoundInf(35);//灭
			Delay_ms(300);
	}
		break;
 		case 215:			
	if(SoundEnable==1)
	{
		SendSoundInf(32);//红灯
				Delay_ms(900);
		SendSoundInf(34);//亮
			Delay_ms(300);
	}
		break;
 		case 205:			
	if(SoundEnable==1)
	{
		SendSoundInf(32);//红灯
				Delay_ms(900);
		SendSoundInf(35);//灭
			Delay_ms(300);
	}
				break;  
 		case 216:			
	if(SoundEnable==1)
	{
		SendSoundInf(33);//白灯
				Delay_ms(900);
		SendSoundInf(34);//亮
			Delay_ms(300);
	}
		break;
 		case 206:			
	if(SoundEnable==1)
	{
		SendSoundInf(33);//白灯
				Delay_ms(900);
		SendSoundInf(35);//灭
			Delay_ms(300);
	}
				break;  
 		case 217:			
	if((SoundEnable==1)&&(SelectPWMOut==1))
	{
		SendSoundInf(29);//电机
				Delay_ms(900);
		SendSoundInf(30);//正转
			Delay_ms(900);
	}
		break;
 		case 227:			
	if((SoundEnable==1)&&(SelectPWMOut==1))
	{
		SendSoundInf(29);//电机
				Delay_ms(900);
		SendSoundInf(31);//反转
			Delay_ms(900);
	}
				break;  
 		case 207:			
	if((SoundEnable==1)&&(SelectPWMOut==1))
	{
		SendSoundInf(29);//电机
				Delay_ms(900);
		SendSoundInf(37);//停止
			Delay_ms(900);
	}
				break;  
	
	}		
	ReadFlag=0;
	}
BeepFlag=0;INDEX=0;
	}
  	Delay_10ms(10);
   }
}

/********************************************************
* 将小于四位的数字含小数点，用语音播放出来
*入口：（1）数据在Vdata中
*			（2）Type=0表示没有小数，Type=1表示有小数为一位，但给出了的是整数如198，实际上为19.8
****************************************************/
void SoundFormNum(uint8_t Type)
{
if(Type==0)
{	
if(Vdata<10)
		{
		SendSoundInf(strhw[0]&0x0f);
			Delay_ms(400);
		}
else if(Vdata<100)
		{
		SendSoundInf(strhw[0]&0x0f);
			Delay_ms(400);
		if ((strhw[1]&0x0f)!=0) SendSoundInf(strhw[1]&0x0f);
			else SendSoundInf(10);
			Delay_ms(400);
		}
else if (Vdata<1000)
		{
		SendSoundInf(strhw[0]&0x0f);
			Delay_ms(400);
		if ((strhw[1]&0x0f)!=0) SendSoundInf(strhw[1]&0x0f);
			else SendSoundInf(10);
			Delay_ms(400);
		if ((strhw[2]&0x0f)!=0) SendSoundInf(strhw[2]&0x0f);
			else SendSoundInf(10);
			Delay_ms(400);
		}	
else
	{
		SendSoundInf(strhw[0]&0x0f);
			Delay_ms(400);
		if ((strhw[1]&0x0f)!=0) SendSoundInf(strhw[1]&0x0f);
			else SendSoundInf(10);		
			Delay_ms(400);
		if ((strhw[2]&0x0f)!=0) SendSoundInf(strhw[2]&0x0f);
			else SendSoundInf(10);		
			Delay_ms(400);
		if ((strhw[3]&0x0f)!=0) 
						SendSoundInf(strhw[3]&0x0f);
			else 
						SendSoundInf(10);		
			Delay_ms(400);
	}
}
else if(Type==1)
{
if(Vdata<10)
		{
		SendSoundInf(10);//0
			Delay_ms(400);
		SendSoundInf(11);//.
			Delay_ms(400);
		SendSoundInf(strhw[0]&0x0f);
			Delay_ms(400);
		}
else if(Vdata<100)
		{
		SendSoundInf(strhw[0]&0x0f);
			Delay_ms(400);
		SendSoundInf(11);//.
			Delay_ms(400);
		if ((strhw[1]&0x0f)!=0) SendSoundInf(strhw[1]&0x0f);
			else SendSoundInf(10);
			Delay_ms(400);
		}
else if (Vdata<1000)
		{
		SendSoundInf(strhw[0]&0x0f);
			Delay_ms(400);
		if ((strhw[1]&0x0f)!=0) SendSoundInf(strhw[1]&0x0f);
			else SendSoundInf(10);
			Delay_ms(400);
		SendSoundInf(11);//.
			Delay_ms(400);			
		if ((strhw[2]&0x0f)!=0) SendSoundInf(strhw[2]&0x0f);
			else SendSoundInf(10);
			Delay_ms(400);
		}	
else
	{
		SendSoundInf(strhw[0]&0x0f);
			Delay_ms(400);
		if ((strhw[1]&0x0f)!=0) SendSoundInf(strhw[1]&0x0f);
			else SendSoundInf(10);		
			Delay_ms(400);
		if ((strhw[2]&0x0f)!=0) SendSoundInf(strhw[2]&0x0f);
			else SendSoundInf(10);		
			Delay_ms(400);
		SendSoundInf(11);//.
			Delay_ms(400);
		if ((strhw[3]&0x0f)!=0) 
				SendSoundInf(strhw[3]&0x0f);
			else 
				SendSoundInf(10);		
			Delay_ms(400);
	}	
}
}
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  * @note   #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  */
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART1, (uint8_t) ch);   
	
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}
	
	return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
