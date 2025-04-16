/*****************************************************
** Descriptions:     对HJ12864-COG-1液晶显示屏相关的硬件配置
**																		 
*****************************************************************/ 

#include "HZ_ASCII.h"
#include "stdio.h"
#include "delay.h"
#include "stdlib.h"
#include "HJ12864-COG-1.h"

/* Private variable ---------------------------------------------------------*/
//uint16_t DeviceCode; //LCD的ID号变量


/*********************************************************************************
函数名：void LCD_GPIO_Configuration(void)
功能：配置并初始化LCD数据和控制端口
输入参数：无
输出参数：无
*********************************************************************************/
void LCD_HJ12864Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启相应时钟 LCD(HJ128564-COG-1)用到PC和PD端口*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD,ENABLE); 
							  
	/*输出引脚配置为推挽输出*/
	/*GPIOC*/
	GPIO_InitStructure.GPIO_Pin = CSB|MOSI|SCLK|nCS|SCL|SDA;  	//如果SDA设置为开漏输出，则SDA对电源要接上拉电阻
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;								
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* A0_RS  RSTB  GPIOD推挽输出*/
	GPIO_InitStructure.GPIO_Pin = A0_RS| RSTB;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/*输入引脚配置为上拉输入*/
	/*GPIOC*/
	GPIO_InitStructure.GPIO_Pin = MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

/*********************************************************************************
	*名称：void LCD_Init(void)
	*输入：无
	*返回：无
	*功能：LCD初始化
	*说明：
*********************************************************************************/

//从字库IC中取汉字或字符数据（一个字节）
static uint8_t get_data_from_ROM( )
{
uint8_t i;
uint8_t ret_data=0;
	for(i=0;i<8;i++)
	{
	LCD_SCLK(0);
	//delay_us(1);
	ret_data=ret_data<<1;
	if (GPIO_ReadInputDataBit(GPIOC,MISO)==1)	ret_data=ret_data+1;
	else		   		ret_data=ret_data+0;
	LCD_SCLK(1);
	//delay_us(100);
	}
	return(ret_data);
}

/*----------------向LCD写数据-----------------*/
void WRITE_DatH(uint8_t dat)
{
   uint8_t i;
   LCD_CSB(0);
   LCD_RS(1);//数据
   for(i = 0;i<8;i++)		//write 8 bits data
    {
	LCD_SCL(0);
	if (dat&0x80) LCD_SDA(1);
	else          LCD_SDA(0);
	LCD_SCL(1);
	dat=dat <<1;
	}
  LCD_CSB(1);
}
/*----------------向LCD写命令-----------------*/
void WRITE_COMH(uint8_t com)
{
   uint8_t i;
   LCD_CSB(0);
   LCD_RS(0);//命令
   for(i = 0;i<8;i++)		//write 8 bits Command
    {
	LCD_SCL(0);
	if (com&0x80) LCD_SDA(1);
	else          LCD_SDA(0);
	LCD_SCL(1);
	com=com <<1;
	}
   LCD_CSB(1);
}

void LCD_HJ12864_Init(void)
{

	LCD_RSTB(0);
//	delayus(3);	//复位要求3us以上，这里用3ms
	LCD_RSTB(1);
//	delayus(10);//给内部复位时间6ms,这里选择10ms
	WRITE_COMH(0xe2);//软复位
//	delayns(10);
	WRITE_COMH(0x2c);//升压
//	delayns(50);
	WRITE_COMH(0x2e);//稳压
//	delayns(50);
	WRITE_COMH(0x2f);//降压
//	delayns(10);
	WRITE_COMH(0x23);//粗调对比度
//	delayns(10);
	WRITE_COMH(0x81);//微调对比度
//	delayns(10);
	WRITE_COMH(0x28);//微调对比度值
//	delayns(10);
	WRITE_COMH(0xa2);//1/9偏压比
//	delayns(10);
	WRITE_COMH(0xC8);//行扫描顺序，由上到下
	WRITE_COMH(0xA0);//列扫描顺序，由左到右
	WRITE_COMH(0x40);//第一行开始
	WRITE_COMH(0xAF);//开显示
}

void clear_screenHJ(void)	   //清LCD屏操作
{
uint8_t i,j;
for (i=0;i<9;i++)
	{
	WRITE_COMH(0xb0+i);
	WRITE_COMH(0x10);
	WRITE_COMH(0);
	for (j=0;j<132;j++)
	{
	WRITE_DatH(0);
	}
	}
}

void LCD_Addr(uint8_t page,uint8_t column)
{
column=column-0x01;
	WRITE_COMH(0xb0+page-1);//设置页地址
	WRITE_COMH(0x10+(column>>4&0x0f));//设置列地址的高4位
	WRITE_COMH(column&0x0f);//设置列地址的低4位
}
//送指令到字库IC
void send_command_to_ROM( uint8_t datu )
{

uint8_t i;
	for(i=0;i<8;i++ )
	{
	LCD_SCLK(0);

	if(datu&0x80) LCD_MOSI(1);
	else LCD_MOSI(0);
	datu = datu<<1;
	LCD_SCLK(1);

}
}

//从指定地址读出数据16X16写到液晶屏指定(page,column)坐标中
void get_and_write_16x16(uint32_t fontaddr,uint8_t page,uint8_t column,uint8_t fb)
{
uint8_t i,j,disp_data,tut[100]={0};
	LCD_nCS(0);
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16); //地址的高8 位,共24 位
	send_command_to_ROM((fontaddr&0xff00)>>8); //地址的中8 位,共24 位
	send_command_to_ROM(fontaddr&0xff); //地址的低8 位,共24 位
	for (j=0;j<2;j++)
	{
	LCD_Addr(page+j,column);
	for (i=0;i<16;i++)
	{
	  disp_data=get_data_from_ROM();
	  tut[i+j*16]=disp_data;
	  if (fb==1)  disp_data=~disp_data;
	  WRITE_DatH(disp_data);//写数据到LCD，每写入一个数据，地址加1
	}	
	}
	LCD_nCS(1);

}

//从指定地址读出数8X16据写到液晶屏指定(page,column)
void get_and_write_8x16(uint32_t fontaddr,uint8_t page,uint8_t column,uint8_t fb)
{
uint8_t i,j,disp_data;
	LCD_nCS(0);
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16); //地址的高8 位,共24 位
	send_command_to_ROM((fontaddr&0xff00)>>8); //地址的中8 位,共24 位
	send_command_to_ROM(fontaddr&0xff); //地址的低8 位,共24 位
	for (j=0;j<2;j++)
	{
	LCD_Addr(page+j,column);
	for (i=0;i<8;i++)
	{
	  disp_data=get_data_from_ROM();
	  if (fb==1) disp_data=~disp_data; 
	  WRITE_DatH(disp_data);//写数据到LCD，每写入一个数据，地址加1
	}	
	}
	LCD_nCS(1);
}
//从指定地址读出数据写到液晶屏指定（page,column)座标中
void get_and_write_5x8(uint32_t fontaddr,uint8_t page,uint8_t column,uint8_t fb)
{
uint8_t i,disp_data;
	LCD_nCS(0);
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16); //地址的高8 位,共24 位
	send_command_to_ROM((fontaddr&0xff00)>>8); //地址的中8 位,共24 位
	send_command_to_ROM(fontaddr&0xff); //地址的低8 位,共24 位
	LCD_Addr(page,column);
	for (i=0;i<5;i++)
	{
	  disp_data=get_data_from_ROM();
	  if (fb==1) disp_data=~disp_data; 
	  WRITE_DatH(disp_data);//写数据到LCD，每写入一个数据，地址加1
	}	
	LCD_nCS(1);
}

void display_HJGB2312_string(uint8_t page,uint8_t column,uint8_t *text,uint8_t fb)
{
	uint8_t i=0;
	uint32_t fontaddr;//,MSB,LSB;
//	uint32_t BaseAdd=0;
 //国标简体汉字在字库IC中的地址计算：
 //address=((MSB-0xb0)*94+(LSB-0xa1)+846)*32+BaseAddr;BaseAdd=0
 //	MSB=text[i];
//	LSB=text[i+1];
	while((text[i]>0x00))
	{
	if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1))
	{
//国标简体（GB2312）汉字在字库IC 中的地址由以下公式来计算：
//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
//由于担心8 位单片机有乘法溢出问题，所以分三部取地址
	fontaddr = (text[i]- 0xb0)*94;
	fontaddr += (text[i+1]-0xa1)+846;
	fontaddr = (fontaddr*32);
	get_and_write_16x16(fontaddr,page,column,fb); //从指定地址读出数据写到液晶屏指定（page,column)座标中
	i+=2;
	column+=16;
	}
	else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
{
//国标简体（GB2312）15x16 点的字符在字库IC 中的地址由以下公式来计算：
//Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0
//由于担心8 位单片机有乘法溢出问题，所以分三部取地址
	fontaddr = (text[i]- 0xa1)*94;
	fontaddr += (text[i+1]-0xa1);
	fontaddr = (fontaddr*32);
	get_and_write_16x16(fontaddr,page,column,fb); //从指定地址读出数据写到液晶屏指定（page,column)座标中
	i+=2;
	column+=16;
}
	else if((text[i]>=0x20) &&(text[i]<=0x7e))

{
	fontaddr = (text[i]- 0x20);
	fontaddr = fontaddr*16;
	fontaddr = fontaddr+0x3cf80;
	get_and_write_8x16(fontaddr,page,column,fb); //从指定地址读出数据写到液晶屏指定（page,column)座标中
	i+=1;
	column+=8;
}
	else
	i++;
	}
}  
/*显示16x16 点阵图像、汉字、生僻字或16x16 点阵的其他图标
 入口：fb=1反白显示，fb=0正常显示
       page为页号，1,3,5,7表示第一二三四行（汉字行），cloumn为列1～128
	   dp为要显示汉字等信息的数组
                                                           */
void display_graphic_16x16(uint8_t page,uint8_t column,uint8_t *dp,uint8_t fb)
{
uint8_t i,j;
for(j=0;j<2;j++)
{
LCD_Addr(page+j,column);
for (i=0;i<16;i++)
{
if (fb==0) WRITE_DatH(*dp); //写数据到LCD,每写完一个8 位的数据后列地址自动加1
else       WRITE_DatH(~*dp);
dp++;
}
}
}
//x2 汉字的宽度，y2为汉字高度
void display_graphic_xy1(uint8_t page,uint8_t column,uint8_t x2,uint8_t y2,const uint8_t *dp,uint8_t fb)
{
	uint8_t i,j;
	for(j=0;j<y2/8;j++)
	{
		LCD_Addr(page+j,column);
		for (i=0;i<x2;i++)	   //
		{
			if (fb==1)  WRITE_DatH(~*dp); //写数据到LCD,每写完一个8 位的数据后列地址自动加1  反白显示
			WRITE_DatH(*dp); //写数据到LCD,每写完一个8 位的数据后列地址自动加1
			dp++;
		}
	}
}
//x2 汉字的宽度，y2为汉字高度
void display_graphic_xy(uint8_t page,uint8_t column,uint8_t x2,uint8_t y2,uint8_t *dp,uint8_t fb)
{
	uint8_t i,j;
	for(j=0;j<y2/8;j++)
	{
		LCD_Addr(page+j,column);
		for (i=0;i<x2;i++)	   //
		{
			if (fb==1)  WRITE_DatH(~*dp); //写数据到LCD,每写完一个8 位的数据后列地址自动加1  反白显示
			WRITE_DatH(*dp); //写数据到LCD,每写完一个8 位的数据后列地址自动加1
			dp++;
		}
	}
}

void display_string_5x8(uint8_t page,uint8_t column, uint8_t *text,uint8_t fb)
{
unsigned char i= 0;
int32_t fontaddr;
while((text[i]>0x00))
{
if((text[i]>=0x20) &&(text[i]<=0x7e))
{
fontaddr = (text[i]- 0x20);
fontaddr = (unsigned long)(fontaddr*8);
fontaddr = (unsigned long)(fontaddr+0x3bfc0);
get_and_write_5x8(fontaddr,page,column,fb); //从指定地址读出数据写到液晶屏指定（page,column)座标中
i+=1;
column+=6;
}
else
i++;
}
}

void display_Line(uint8_t column)   //绘制竖线,column为列号
{
uint8_t j;

for(j=0;j<8;j++)
{
LCD_Addr(j+1,column);
WRITE_DatH(0xff); //写数据到LCD,每写完一个8 位的数据后列地址自动加1
} 

}  
//显示128x64 点阵边框


void display_bk()
{
uint8_t i,j;

//左框
for(j=0;j<8;j++)
{
LCD_Addr(j+1,1);
WRITE_DatH(0xff); 
}

//右框
for(j=0;j<8;j++)
{
LCD_Addr(j+1,127);
WRITE_DatH(0xff); 
}

//上框
LCD_Addr(1,2);
for (i=0;i<125;i++)		 //0
{
WRITE_DatH(0x01);
}

//下框
/*LCD_Addr(8,2);
for (i=0;i<125;i++)
{
WRITE_Dat(0x80); 
}	 */

}										  


void displayInformationHJ(void)		//显示信息（设置状态除外）
{
  display_HJGB2312_string(1,1,"NH-SQ1005CM3 Kit",0);
	display_HJGB2312_string(3,1,"点阵图形字符示例 ",0);
	display_HJGB2312_string(5,1," HJ12864-COG-1! ",0);
	display_HJGB2312_string(7,1,"南京航空航天大学 ",0);	
}


