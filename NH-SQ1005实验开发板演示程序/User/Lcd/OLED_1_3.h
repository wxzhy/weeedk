/************************************************************************
** 本程序测试OLED模块，实验开发板：NUAA_CM3_107 Kit
** 使用的OLED：1.3吋  128X64
//  文 件 名   : OLED_1_3.c
//  版 本 号   : v2.0
//  功能描述   : OLED 4接口演示例程
//              引脚说明: 
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PC9（SCL）
//              D1   接PC7（SDA）
//              RES  接PD15
//              DC   接PD13 
//              CS   接PC5               
// 修改历史   :
// 日    期   : 
// 作    者   : NUAA
******************************************************************************/

#ifndef __OLED_H
#define __OLED_H			  	 
#include "hw_config.h"
#include "stdlib.h"	    	
//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED端口定义----------------  					   
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOC,GPIO_Pin_5)//CS=PC5
#define OLED_CS_Set()  GPIO_SetBits(GPIOC,GPIO_Pin_5)

#define OLED_RST(x)   ((x) ? (GPIO_SetBits(GPIOD, GPIO_Pin_15)) : (GPIO_ResetBits(GPIOD, GPIO_Pin_15)))
#define OLED_DC(x)    ((x) ? (GPIO_SetBits(GPIOD, GPIO_Pin_13)) : (GPIO_ResetBits(GPIOD, GPIO_Pin_13)))
#define OLED_SCLK(x)  ((x) ? (GPIO_SetBits(GPIOC, GPIO_Pin_9)) : (GPIO_ResetBits(GPIOC, GPIO_Pin_9)))
#define OLED_SDIN(x)  ((x) ? (GPIO_SetBits(GPIOC, GPIO_Pin_7)) : (GPIO_ResetBits(GPIOC, GPIO_Pin_7)))

#define OLED_RST_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_15)//RES=PD15
#define OLED_RST_Set() GPIO_SetBits(GPIOD,GPIO_Pin_15)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_13)//DC=PD13
#define OLED_DC_Set() GPIO_SetBits(GPIOD,GPIO_Pin_13)

//使用4线串行接口时使用 

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_9)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOC,GPIO_Pin_9)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_7)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOC,GPIO_Pin_7)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_ShowCHineseMotor(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  
	 



