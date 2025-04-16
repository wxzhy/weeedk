/*****************************************************
** Descriptions:     对OCWJ4x8C液晶显示屏相关的硬件配置
**																		 
*****************************************************************/ 

#include "HZ_ASCII.h"
#include "stdio.h"
#include "delay.h"
#include "stdlib.h"
#include "OCWJ4x8C.h"

/* Private variable ---------------------------------------------------------*/
//uint16_t DeviceCode; //LCD的ID号变量


/*********************************************************************************
函数名：void LCD_GPIO_Configuration(void)
功能：配置并初始化LCD数据和控制端口
输入参数：无
输出参数：无
*********************************************************************************/
void LCD_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启相应时钟 LCD(OCWJ4x8C)用到PC(PC6)、PD（PD13、PD14、PD15）和PE（PE0~PE7）端口*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE,ENABLE); 
							  
	/*输出引脚配置为推挽输出*/
	/*GPIOE   D0~D7 */
	GPIO_InitStructure.GPIO_Pin =0xFF;//PE0~PE7  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;								
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* RS  RW  RSTB GPIOD推挽输出*/
	GPIO_InitStructure.GPIO_Pin = RS| RW |RSTB;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* E  GPIOC推挽输出*/
	GPIO_InitStructure.GPIO_Pin = E;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}
void shortdelay(uint32_t dly)
{
  uint32_t i;
   
  for(; dly>0; dly--) 
      for(i=0; i<300;i++);
}      

void  WRITE_COM(uint8_t DATA)          //液晶命令写入
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
   uint8_t tmp=0;
   LCD_RS(0);
   shortdelay(3);
   LCD_RW(0);
   shortdelay(3);
   GPIOE->ODR = DATA;//GPIO_Write(GPIOE,DATA);
   LCD_E(1);
   shortdelay(3);
   LCD_E(0);
   shortdelay(3);
  // shortdelay(3);
   LCD_RW(1);//IO2SET=RW;
  /**********让D7（PE7）为上拉输入*******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	GPIO_Init(GPIOE, &GPIO_InitStructure);
   if((GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)!=0)&&(tmp<3))
   {
    shortdelay(1);
    tmp++;
   }
  /**********让D7（PE7）回到推挽输出*******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(GPIOE, &GPIO_InitStructure);

   LCD_RW(0);//IO2CLR=RW;
 }
 void WRITE_DAT(uint8_t DATA)   //液晶数据写入
{
 	GPIO_InitTypeDef GPIO_InitStructure;
// shortdelay(1);
 uint8_t tmp=0;
  LCD_RS(1);//IO2SET=LCD_RS;
   shortdelay(3);
  LCD_RW(0);//IO2CLR=RW;
  shortdelay(3);
   GPIOE->ODR = DATA;//GPIO_Write(GPIOE,DATA);
  LCD_E(1);//IO2SET=E;
  shortdelay(3);
  LCD_E(0);//IO2CLR=E;
  shortdelay(3);
  LCD_RW(1);//IO2SET=RW;
  /**********让D7（PE7）为上拉输入*******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	GPIO_Init(GPIOE, &GPIO_InitStructure);
   if((GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)!=0)&&(tmp<3))
   {
    shortdelay(1);
    tmp++;
   }
  /**********让D7（PE7）回到推挽输出*******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(GPIOE, &GPIO_InitStructure);
   LCD_RW(0);//IO2CLR=RW;
  }


/*********************************************************************************
	*名称：void LCD_Init_OCWJ4X8C(void)
	*输入：无
	*返回：无
	*功能：LCD初始化
	*说明：
*********************************************************************************/
void  LCD_Init_OCWJ4X8C(void)                //液晶初始化
{
   uint8_t DATA;
   shortdelay(3000);
	LCD_RSTB(0);
   shortdelay(30000);
	LCD_RSTB(1);
   shortdelay(3);
   DATA=0x01;
   WRITE_COM(DATA);
   shortdelay(3);
   DATA=0x30;
   WRITE_COM(DATA);
   shortdelay(3);
   DATA=0x02;
   WRITE_COM(DATA);
   shortdelay(3);
   DATA=0x04;
   WRITE_COM(DATA);
   shortdelay(3);
   DATA=0x0c;
   WRITE_COM(DATA);
   shortdelay(3);
   DATA=0x01;
   WRITE_COM(DATA);
    shortdelay(3);
   DATA=0x80;
   WRITE_COM(DATA);
    shortdelay(3);
}

/*************************************************************
汉字字符的显示（借助于字库）OCWJ4x8C为带汉字库的液晶屏
入口：t为字符个数，DATA为字符位置
*************************************************************/
void display_GB2312_string(char *a,uint8_t t,uint8_t DATA)       //向对应的位置写对应个数的数字或汉字
 {
   uint16_t i;
   WRITE_COM(0x30);
   shortdelay(3);
   WRITE_COM(DATA);
   for(i=0;i<t;i++)
   {
    WRITE_DAT(a[i]);
     }
  } 
/*************************************************************
汉字字符的反白显示（借助于字库）OCWJ4x8C为带汉字库的液晶屏
入口：t为字符个数，DATA为字符位置
*************************************************************/
void display_GB2312_string_fb(char *a,uint8_t t,uint8_t DATA)       //向对应的位置写对应个数的数字或汉字
 {
   uint16_t i;
   WRITE_COM(0x30);
   shortdelay(3);
   WRITE_COM(DATA);
   for(i=0;i<t;i++)
   {
    WRITE_DAT(~a[i]);
     }
  } 

 void clear_screen()           //清除液晶RAM
 {
   uint16_t DATA;
   DATA=0x30;
   WRITE_COM(DATA);
   
   DATA=0x01;
   WRITE_COM(DATA);
   shortdelay(2000);   //清除RAM的延迟必须足够长,否则将出现花屏
   
 } 
 void fb_line(uint8_t n)           //指定第n行反白（n=0,1,2,3）
 {
uint8_t DATA=0;
 //  DATA=0x30;
  // WRITE_COM(DATA);
   DATA=n|(1<<2);
   WRITE_COM(DATA);
   shortdelay(2000);   //清除RAM的延迟必须足够长,否则将出现花屏
   
 } 	  



void displayInformation(void)		//显示信息（设置状态除外）
{
	fb_line(2);
//  uint8_t i;
//WRITE_COM(0x55);
  //	display_bk();
  //	for (i=0;i<96;i++) Mtext[i]=FlagWLL[i];//图标
//	display_graphic_xy(3,5,16,16,Mtext,0);
//	display_string_5x8(2,16,"NUAA_CM3_107 exp.",0);
	display_GB2312_string("NH-SQ1005CM3 Kit",16,0x80);
	display_GB2312_string("点阵图形字符示例",16,0x90);
	display_GB2312_string(" OCWJ4x8C(12864)!",16,0x88);
	display_GB2312_string("南京航空航天大学",16,0x98);
//	display_string_5x8(8,5,"0123456789ABCDEFGHIJ",0);

}


