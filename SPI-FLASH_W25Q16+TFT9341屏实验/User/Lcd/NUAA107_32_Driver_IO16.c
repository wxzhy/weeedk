/*****************************************************
** Descriptions:        主要对3.2寸屏（SSD1298）的初始化配置，以及各种API函数的编写
**																		 
*****************************************************************/ 

#include "font.h"
#include "stdio.h"
#include "delay.h"
#include "stdlib.h"
#include "107_32_Driver_IO16.h"

/* Private variable ---------------------------------------------------------*/
uint16_t DeviceCode; //LCD的ID号变量


/*********************************************************************************
函数名：void LCD_GPIO_Configuration(void)
功能：配置并初始化LCD数据和控制端口
输入参数：无
输出参数：无
*********************************************************************************/
void LCD_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启相应时钟 TFT用到PC、PD、PE端口*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD 
							|RCC_APB2Periph_GPIOE, ENABLE);
							  
	/*所有LCD数据引脚配置为推挽输出*/
	/*GPIOE*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* LCD_CS */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* LCD_RS  LCD_WR  LCD_RD*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/*********************************************************************************
函数名：void LCD_WR_REG(uint16_t LCD_Reg)
功能：写入LCD的控制命令
输入参数：要写入的命令uint16_t LCD_Reg
输出参数：无
**********************************************************************************/
void LCD_WR_REG(uint16_t LCD_Reg)
{
	
	LCD_RD(1);					/*读失能*/
	LCD_CS(0);					/*片选*/
	LCD_RS(0);					/*0表示命令*/
	DataToWrite(LCD_Reg);
	LCD_WR(0);				
	LCD_WR(1);					/*上升沿写入*/
	LCD_RS(1);					/*1表示命令*/	
}
					  
/*********************************************************************************
函数名：void LCD_WR_DATA(uint16_t LCD_Reg)
功能：写入LCD的数据
输入参数：要写入的数据uint16_t LCD_Data
输出参数：无
*********************************************************************************/
void LCD_WR_DATA(uint16_t LCD_Data)	
{	
	LCD_RD(1);					/*读失能*/
	LCD_CS(0);
	LCD_RS(1);					/*1表示数据*/
	DataToWrite(LCD_Data);
	LCD_WR(0);
	LCD_WR(1);					/*上升沿写入*/
	LCD_CS(1);
} 


/*********************************************************************************
函数名：uint16_t LCD_ReadReg(uint16_t LCD_Reg)
功能：读取LCD的数据
输入参数：uint16_t LCD_Reg 要读取的寄存器
输出参数：temp 读取的数据
*********************************************************************************/
uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{
	uint16_t temp;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	LCD_WR_REG(LCD_Reg);

	/* 设置为输入模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOE, &GPIO_InitStructure);	

	GPIO_ResetBits(GPIOC,LCD_CS_PIN); //拉低片选段CS					
	GPIO_SetBits(GPIOD,LCD_RS_PIN); //读取的是数据
	GPIO_ResetBits(GPIOD,LCD_RD_PIN); //拉低读取的引脚，准备读取数据
	GPIO_SetBits(GPIOD,LCD_RD_PIN); //拉高读取的引脚，读取数据
	temp=GPIO_ReadInputData(GPIOE);

	GPIO_SetBits(GPIOC,LCD_CS_PIN); //拉高片选段CS

	/* 设置为输出模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	return temp;				    	 	
}

/*********************************************************************************
函数名：void LCD_WriteReg(uint16_t LCD_Reg ,uint16_t LCD_RegValue)
功能：向指定寄存器写入指定数据
输入参数：uint16_t LCD_Reg       寄存器地址；
					uint16_t LCD_RegValue  要写入的数据
输出参数：无
*********************************************************************************/
void LCD_WriteReg(uint16_t LCD_Reg ,uint16_t LCD_RegValue)
{	
	LCD_RD(1);					/*读失能*/
	LCD_CS(0);					/*片选*/
	LCD_RS(0);					/*0表示命令*/
	DataToWrite(LCD_Reg);
	LCD_WR(0);				
	LCD_WR(1);					/*上升沿写入*/

	LCD_RS(1);					/*1表示数据*/
	DataToWrite(LCD_RegValue);
	LCD_WR(0);
	LCD_WR(1);					/*上升沿写入*/
	LCD_CS(1);	
}

/*********************************************************************************
	*名称：void LCD_WriteRAM_Prepare(void)
	*参数：无
	*返回：无
	*功能：
	*备注：开始写GRAM
*********************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(0x22);
}

/*********************************************************************************
	*名称：void LCD_WriteRAM(u16 RGB_Code)
	*参数：u16 RGB_Code，写入颜色的数值
	*返回：无
	*功能：LCD写GRAM
	*备注：
*********************************************************************************/
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD_WR_DATA(RGB_Code);    /*写十六位GRAM*/
}

/*********************************************************************************
	*名称：void LCD_SetCursor(u16 Xpos, u16 Ypos)
	*参数：Xpos 横坐标
				 Ypos 纵坐标
	*返回：无
	*功能：设置光标位置
*********************************************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	
 	if(DeviceCode==0x8999||DeviceCode==0x9919)
	{
		LCD_WriteReg(0x004E, Xpos);
		LCD_WriteReg(0x004F, Ypos);
	}
	else 
	{
		LCD_WriteReg(0x0020, Xpos);
		LCD_WriteReg(0x0021, Ypos);
	}
}

/*********************************************************************************
	*名称：void_LCD_Clear(uint16_t Colour)
	*参数：Colour 16位颜色数据
	*返回：无
	*功能：按指定颜色刷屏
*********************************************************************************/
void LCD_Clear(uint16_t Color)
{
	uint32_t index=0;
	LCD_WindowMax (0, 0, 240, 320);
	LCD_SetCursor(0x00,0x0000);      /*设置光标位置  */
	LCD_WriteRAM_Prepare();          /*开始写入GRAM	 */
				 
	for(index=0;index<76800;index++)
	{
		LCD_WR_DATA(Color);		   /*写入数据 */ 
	}	
}

/*********************************************************************************
	*名称：void LCD_DrawPoint(void)
	*参数：xsta X起始坐标 0~239
		   ysta	Y起始坐标 0~319
	*返回：无
	*功能：POINT_COLOR指定点的颜色

*********************************************************************************/
void LCD_DrawPoint(uint16_t xsta, uint16_t ysta,uint16_t colour)
{
	LCD_WindowMax (xsta, ysta, 240, 320);
	LCD_SetCursor(xsta,ysta);  /*设置光标位置  */
	LCD_WR_REG(0x0022);           /*开始写入GRAM */
	LCD_WR_DATA(colour); 
	/*恢复窗口*/
	LCD_WindowMax (0, 0, 240, 320); 
}

/*********************************************************************************					
	*名称：void LCD_WindowMax()
	*参数：
	*返回：无
	*功能：设置窗口
	*备注：
*********************************************************************************/
void LCD_WindowMax (unsigned int x,unsigned int y,unsigned int x_end,unsigned int y_end) 
{
	
	if(DeviceCode==0x8999)
	{
		LCD_WriteReg(0x44,x|((x_end-1)<<8));
		LCD_WriteReg(0x45,y);
		LCD_WriteReg(0x46,y_end-1);
	}
	else
	{
		LCD_WriteReg(0x50, x);                      /* Horizontal GRAM Start Address      */
		LCD_WriteReg(0x51, x_end-1);               /* Horizontal GRAM End   Address (-1) */
		LCD_WriteReg(0x52, y);                      /* Vertical   GRAM Start Address      */
		LCD_WriteReg(0x53, y_end-1);                /* Vertical   GRAM End   Address (-1) */
	}
}

/*********************************************************************************
	*名称：void LCD_Fill(uint8_t xsta, uint16_t ysta, uint8_t xend, uint16_t yend, uint16_t colour)
	*参数：xsta 起始X坐标
		   ysta 起始Y坐标
		   xend 结束X坐标
		   yend 结束Y坐标
		   color 待填充颜色
	*返回：无
	*功能：在指定矩形区域填充指定颜色，区域大小(xend-xsta)*(yend-ysta)
	*备注：部分区域少一行像素点	
*********************************************************************************/
void LCD_Fill(uint8_t xsta, uint16_t ysta, uint8_t xend, uint16_t yend, uint16_t colour)
{                    
    u32 n;

	/*设置窗口	*/	
	LCD_WindowMax (xsta, ysta, xend, yend); 
	LCD_SetCursor(xsta,ysta);        /*设置光标位置 */ 
	LCD_WriteRAM_Prepare();         /*开始写入GRAM*/	 	   	   
	n=(u32)(yend-ysta+1)*(xend-xsta+1);    
	while(n--){LCD_WR_DATA(colour);} /*显示所填充的颜色*/
	 
	/*恢复窗口*/
	LCD_WindowMax (0, 0, 240, 320); 
}  

/*********************************************************************************
	*名称：void LCD_DrawLine(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend)
	*参数：xsta X起始坐标
	   	   ysta Y起始坐标
		   xend X终点坐标
		   yend Y终点坐标	
	*返回：无
	*功能：指定坐表(两点)，画线
	*备注：需要添加颜色参数
*********************************************************************************/ 
void LCD_DrawLine(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend,uint16_t colour)
{
    u16 x, y, t;
	if((xsta==xend)&&(ysta==yend))LCD_DrawPoint(xsta, ysta,colour);
	else if(abs(yend-ysta)>abs(xend-xsta))/*斜率大于1 */ 
	{
		if(ysta>yend) 
		{
			t=ysta;
			ysta=yend;
			yend=t; 
			t=xsta;
			xsta=xend;
			xend=t; 
		}
		for(y=ysta;y<yend;y++)            /*以y轴为基准*/ 
		{
			x=(u32)(y-ysta)*(xend-xsta)/(yend-ysta)+xsta;
			LCD_DrawPoint(x, y,colour);  
		}
	}
	else     /*斜率小于等于1 */
	{
		if(xsta>xend)
		{
			t=ysta;
			ysta=yend;
			yend=t;
			t=xsta;
			xsta=xend;
			xend=t;
		}   
		for(x=xsta;x<=xend;x++)  /*以x轴为基准*/ 
		{
			y =(u32)(x-xsta)*(yend-ysta)/(xend-xsta)+ysta;
			LCD_DrawPoint(x,y,colour); 
		}
	} 
} 

/*********************************************************************************
	*名称：void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r)
	*参数：x0 中心点横坐标
	       y0 中心点纵坐标
		   r  半径
	*返回：无
	*功能：在指定位置画一个指定大小的圆
	*备注：加入颜色参数，是否填充等
*********************************************************************************/
void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r,uint16_t colour)
{
	int a,b;
	int di;
	a=0;b=r;	  									 
	di=3-(r<<1);             /*判断下个点位置的标志*/
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,colour);             //3           
		LCD_DrawPoint(x0+b,y0-a,colour);             //0           
		LCD_DrawPoint(x0-a,y0+b,colour);             //1       
		LCD_DrawPoint(x0-b,y0-a,colour);             //7           
		LCD_DrawPoint(x0-a,y0-b,colour);             //2             
		LCD_DrawPoint(x0+b,y0+a,colour);             //4               
		LCD_DrawPoint(x0+a,y0-b,colour);             //5
		LCD_DrawPoint(x0+a,y0+b,colour);             //6 
		LCD_DrawPoint(x0-b,y0+a,colour);             
		a++;

		/*使用Bresenham算法画圆*/     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b,colour);
	}
} 

/*********************************************************************************
	*名称：void LCD_DrawRectangle(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend)
	*参数：xsta X起始坐标
	       ysta Y起始坐标
		   xend X结束坐标
		   yend Y结束坐标
	*返回：无
	*功能：在指定区域画矩形
	*备注：

*********************************************************************************/
void LCD_DrawRectangle(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend,uint16_t colour)
{
	LCD_DrawLine(xsta,ysta,xend,ysta,colour);
	LCD_DrawLine(xsta,ysta,xsta,yend,colour);
	LCD_DrawLine(xsta,yend,xend,yend,colour);
	LCD_DrawLine(xend,ysta,xend,yend,colour);
} 

/*********************************************************************************
	*名称：void LCD_ShowChar(u8 x, u16 y, u8 num, u8 size, u16 PenColor, u16 BackColor)
	*参数：x，y      起始坐标（x:0~234 y:0~308）
		   num       字符ASCII码值
		   size      字符大小，使用默认8*16
		   PenColor  字体颜色
		   BackColor 字体背景颜色
	*功能：
	*备注：注意屏幕大小
*********************************************************************************/
void LCD_ShowChar(u8 x, u16 y, u8 num, u8 size, u16 PenColor, u16 BackColor)
{       
#define MAX_CHAR_POSX 232
#define MAX_CHAR_POSY 304 
    u8 temp;
    u8 pos,t;
    if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;		    
	LCD_WindowMax(x,y,x+size/2,y+size);	   /*设置窗口	*/										
	LCD_SetCursor(x, y);                  /*设置光标位置 */
  
	LCD_WriteRAM_Prepare();               /*开始写入GRAM  */   
	num=num-' ';                         /*得到偏移后的值 */
	for(pos=0;pos<size;pos++)
	{
		if(size==12)
			temp=asc2_1206[num][pos];/*调用1206字体*/
		else 
			temp=asc2_1608[num][pos];		 /*调用1608字体	*/
		for(t=0;t<size/2;t++)
	    {                 
	        if(temp&0x01)			   /*从低位开始*/
			{
				LCD_WR_DATA(PenColor);  /*画字体颜色 一个点*/
			}
			else 
				LCD_WR_DATA(BackColor);	   /*画背景颜色 一个点*/     
	        temp>>=1; 
	    }
	}			
	LCD_WindowMax(0x0000,0x0000,240,320);	/*恢复窗体大小*/	 
} 

  
/*********************************************************************************
	名称：void LCD_ShowCharString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t PenColor, uint16_t BackColor)
	参数：x，y      起始坐标
	      p         指向字符串起始地址
		  PenColor  字符颜色
		  BackColor 背景颜色
	功能：
	备注：用16字体，可以调节 此函数不能单独调用
*********************************************************************************/
void LCD_ShowCharString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t PenColor, uint16_t BackColor)
{   
	uint8_t size = 16;     /*---字符大小默认16*8---*/
	 
    if(x>MAX_CHAR_POSX){x=0;y+=size;}			         /*超出X轴字体最小单位，换行*/
    if(y>MAX_CHAR_POSY){y=x=0;LCD_Clear(WHITE);}	 /*超出Y轴字体最小单位，回到原点，并且清屏*/
    LCD_ShowChar(x, y, *p, size, PenColor, BackColor);			   /*0表示非叠加方式*/
}

/*********************************************************************************
	*名称: u16 findHzIndex(u8 *hz)
	*参数：hz
	*功能：索引汉字存储的内存地址
	*备注：
*********************************************************************************/
u16 findHzIndex(u8 *hz)                            /* 在自定义汉字库在查找所要显示 */
                                                      /* 的汉字的位置 */
{
	u16 i=0;
	FNT_GB16 *ptGb16 = (FNT_GB16 *)GBHZ_16;		  /*ptGb16指向GBHZ_16*/
	while(ptGb16[i].Index[0] > 0x80)
	{
	    if ((*hz == ptGb16[i].Index[0]) && (*(hz+1) == ptGb16[i].Index[1])) /*汉字用两位来表示地址码*/
		{
	        return i;
	    }
	    i++;
	    if(i > (sizeof((FNT_GB16 *)GBHZ_16) / sizeof(FNT_GB16) - 1))  /* 搜索下标约束 */
	    {
		    break;
	    }
	}
	return 0;
}

/*********************************************************************************
	*名称：void WriteOneHz(uint16_t x0, uint16_t y0, uint8_t *pucMsk, uint16_t PenColor, uint16_t BackColor)
	*参数：x0,y0     起始坐标
		   *pucMsk   指向
		   PenColor	 字符颜色
		   BackColor 背景颜色
	*功能：
	*备注：此函数不能单独作为汉字字符显示											  
*********************************************************************************/					
void WriteOneHz(u16 x0, u16 y0, u8 *pucMsk, u16 PenColor, u16 BackColor)
{
    u16 i,j;
    u16 mod[16];                                      /* 当前字模 16*16 */
    u16 *pusMsk;                                      /* 当前字库地址  */
    u16 y;

	u16 size = 16;       /*汉字默认大小16*16*/

    pusMsk = (u16 *)pucMsk;


    for(i=0; i<16; i++)                                    /* 保存当前汉字点阵式字模       */
    {
        mod[i] = *pusMsk;                                /* 取得当前字模，半字对齐访问   */
        mod[i] = ((mod[i] & 0xff00) >> 8) | ((mod[i] & 0x00ff) << 8);/* 字模交换高低字节*/
		pusMsk = pusMsk+1;
    }
    y = y0;
	LCD_WindowMax(x0,y0,x0+size,y0+size);	 	/*设置窗口*/
	LCD_SetCursor(x0,y0);                       /*设置光标位置 */ 
	LCD_WriteRAM_Prepare();                     /*开始写入GRAM*/  
    for(i=0; i<16; i++)                                    /* 16行   */
    {                                              
        for(j=0; j<16; j++)                                /* 16列   */
        {
		    if((mod[i] << j) & 0x8000)       /* 显示第i行 共16个点 */
            { 
			    LCD_WriteRAM(PenColor);
            } 
			else 
			{
                LCD_WriteRAM(BackColor);      /* 用读方式跳过写空白点的像素*/
			}
        }
        y++;
    }
	LCD_WindowMax(0x0000,0x0000,240,320);  	/*恢复窗体大小*/
}

/*********************************************************************************
	*名称：void LCD_ShowHzString(u16 x0, u16 y0, u8 *pcStr, u16 PenColor, u16 BackColor)
	*参数：x0，y0    起始坐标
		   pcStr     指向
		   PenColor  字体颜色
		   BackColor 字体背景
	*功能：显示汉字字符串
	*备注：这个函数不能单独调用	       
*********************************************************************************/
void LCD_ShowHzString(u16 x0, u16 y0, u8 *pcStr, u16 PenColor, u16 BackColor)
{
#define MAX_HZ_POSX 224	 //224+16=240
#define MAX_HZ_POSY 304  //304+16=320
	u16 usIndex;
	u8 size = 16; 
	FNT_GB16 *ptGb16 = 0;    
    ptGb16 = (FNT_GB16 *)GBHZ_16; 

	if(x0>MAX_HZ_POSX){x0=0;y0+=size;}			         /*超出X轴字体最小单位，换行*/
    if(y0>MAX_HZ_POSY){y0=x0=0;LCD_Clear(WHITE);}	   /*超出Y轴字体最小单位，回到原点，并且清屏*/

	usIndex = findHzIndex(pcStr);
	WriteOneHz(x0, y0, (u8 *)&(ptGb16[usIndex].Msk[0]),  PenColor, BackColor); /* 显示字符 */
}

/*********************************************************************************
	*名称：void LCD_ShowString(u16 x0, u16 y0, u8 *pcstr, u16 PenColor, u16 BackColor)
	*参数：x0 y0     起始坐标
		   pcstr     字符串指针
		   PenColor  字体颜色
		   BackColor 字体背景色
	*功能：调用字符和汉字显示函数，实现字符串显示
	*备注：	
*********************************************************************************/
void LCD_ShowString(u16 x0, u16 y0, u8 *pcStr, u16 PenColor, u16 BackColor)
{
	while(*pcStr!='\0')
	{
	 	if(*pcStr>0x80) /*显示汉字*/
		{
			LCD_ShowHzString(x0, y0, pcStr, PenColor, BackColor);
			pcStr += 2;
			x0 += 16;	
		}
		else           /*显示字符*/
		{
			LCD_ShowCharString(x0, y0, pcStr, PenColor, BackColor);	
			pcStr +=1;
			x0+= 8;
		}
	
	}	

}

/****************************************************************************
* 名    称：u16 ili9320_BGRtoRGB(u16 Color)
* 功    能：RRRRRGGGGGGBBBBB 改为 BBBBBGGGGGGRRRRR 格式
* 入口参数：Color      BRG 颜色值
* 出口参数：RGB 颜色值
* 说    明：内部函数调用
* 调用方法：
****************************************************************************/
u16 LCD_RGBtoBGR(u16 Color)
{						   
  u16  r, g, b, bgr;

  b = (Color>>0)  & 0x1f;	/* 提取B    */
  g = (Color>>5)  & 0x3f;	/* 中间六位 */
  r = (Color>>11) & 0x1f;	/* 提取R    */
  
  bgr =  (b<<11) + (g<<5) + (r<<0);

  return( bgr );
}

/*********************************************************************************
* 名    称：void LCD_DrawPicture(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u16 *pic)
* 功    能：在指定座标范围显示一副图片
* 入口参数：StartX     行起始座标
*           StartY     列起始座标
*           EndX       行结束座标
*           EndY       列结束座标
            pic        图片头指针
* 出口参数：无
* 说    明：图片取模格式为水平扫描，16位颜色模式
* 调用方法：LCD_DrawPicture(0,0,100,100,(u16*)demo);
*********************************************************************************/
void LCD_DrawPicture(u16 StartX,u16 StartY,u16 Xend,u16 Yend,u8 *pic)
{
	static	u16 i=0,j=0;
	u16 *bitmap = (u16 *)pic;
	/*设置图片显示窗口大小*/
	LCD_WindowMax(StartX, StartY, Xend, Yend);	
	LCD_SetCursor(StartX,StartY);
	LCD_WriteRAM_Prepare();
	for(j=0; j<Yend-StartY; j++)
	{
		for(i=0; i<Xend-StartX; i++) LCD_WriteRAM(*bitmap++); 	
	}
	/*恢复窗口*/
	LCD_WindowMax(0, 0, 240, 320);
}



/*********************************************************************************
	*名称：void LCD_Init(void)
	*输入：无
	*返回：无
	*功能：LCD初始化
	*说明：
*********************************************************************************/
void LCD_Init(void)
{
	LCD_Configuration();

	Delay_10ms(10); /* delay 50 ms */ 
	Delay_10ms(10); /* delay 50 ms */
	DeviceCode = LCD_ReadReg(0x0000); /*读取屏的ID号*/
	Delay_10ms(10); /* delay 50 ms */
	if(DeviceCode==0x8999)	   /*对应的驱动IC为SSD1298*/
	{
		/*-----   Start Initial Sequence ------*/
		LCD_WriteReg(0x00, 0x0001); /*开启内部振荡器*/
		LCD_WriteReg(0x01, 0x3B3F); /*驱动输出控制 */
		LCD_WriteReg(0x02, 0x0600); /* set 1 line inversion	*/
		/*-------- Power control setup --------*/
		LCD_WriteReg(0x0C, 0x0007); /* Adjust VCIX2 output voltage */
		LCD_WriteReg(0x0D, 0x0006); /* Set amplitude magnification of VLCD63 */
		LCD_WriteReg(0x0E, 0x3200); /* Set alternating amplitude of VCOM */
		LCD_WriteReg(0x1E, 0x00BB); /* Set VcomH voltage */
		LCD_WriteReg(0x03, 0x6A64); /* Step-up factor/cycle setting  */
		/*-------- RAM position control --------*/
		LCD_WriteReg(0x0F, 0x0000); /* Gate scan position start at G0 */
		LCD_WriteReg(0x44, 0xEF00); /* Horizontal RAM address position */
		LCD_WriteReg(0x45, 0x0000); /* Vertical RAM address start position*/
		LCD_WriteReg(0x46, 0x013F); /* Vertical RAM address end position */
		/* ------ Adjust the Gamma Curve -------*/
		LCD_WriteReg(0x30, 0x0000);
		LCD_WriteReg(0x31, 0x0706);
		LCD_WriteReg(0x32, 0x0206);
		LCD_WriteReg(0x33, 0x0300);
		LCD_WriteReg(0x34, 0x0002);
		LCD_WriteReg(0x35, 0x0000);
		LCD_WriteReg(0x36, 0x0707);
		LCD_WriteReg(0x37, 0x0200);
		LCD_WriteReg(0x3A, 0x0908);
		LCD_WriteReg(0x3B, 0x0F0D);
		/*--------- Special command -----------*/
		LCD_WriteReg(0x28, 0x0006); /* Enable test command	*/
		LCD_WriteReg(0x2F, 0x12EB); /* RAM speed tuning	 */
		LCD_WriteReg(0x26, 0x7000); /* Internal Bandgap strength */
		LCD_WriteReg(0x20, 0xB0E3); /* Internal Vcom strength */
		LCD_WriteReg(0x27, 0x0044); /* Internal Vcomh/VcomL timing */
		LCD_WriteReg(0x2E, 0x7E45); /* VCOM charge sharing time */ 
		/*--------- Turn On display ------------*/
		LCD_WriteReg(0x10, 0x0000); /* Sleep mode off */
		Delay_10ms(3);              /* Wait 30mS  */
		LCD_WriteReg(0x11, 0x6870); /* Entry mode setup. 262K type B, take care on the data bus with 16it only */
		LCD_WriteReg(0x07, 0x0033); /* Display ON	*/
	}
else if(DeviceCode==0x9325||DeviceCode==0x9328)
	{
				LCD_WriteReg(0x00e7,0x0010);      
        LCD_WriteReg(0x0000,0x0001);  			//start internal osc
        LCD_WriteReg(0x0001,0x0100);     
        LCD_WriteReg(0x0002,0x0700); 				//power on sequence 
		//核心 
	  LCD_WriteReg(0x0003,(1<<12)|(1<<7)|(1<<5)|(1<<4)|(0<<3) ); 	//65K 
		
        LCD_WriteReg(0x0004,0x0000);                                   
        LCD_WriteReg(0x0008,0x0207);	           
        LCD_WriteReg(0x0009,0x0000);         
        LCD_WriteReg(0x000a,0x0000); 				//display setting         
        LCD_WriteReg(0x000c,0x0001);				//display setting          
        LCD_WriteReg(0x000d,0x0000); 				//0f3c          
        LCD_WriteReg(0x000f,0x0000);
//Power On sequence //
        LCD_WriteReg(0x0010,0x0000);   
        LCD_WriteReg(0x0011,0x0007);
        LCD_WriteReg(0x0012,0x0000);                                                                 
        LCD_WriteReg(0x0013,0x0000);                 
//        for(i=50000;i>0;i--);
//		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0010,0x1590);   
        LCD_WriteReg(0x0011,0x0227);
//        for(i=50000;i>0;i--);
//		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0012,0x009c);                  
//        for(i=50000;i>0;i--);
//		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0013,0x1900);   
        LCD_WriteReg(0x0029,0x0023);
        LCD_WriteReg(0x002b,0x000e);
//        for(i=50000;i>0;i--);
//		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0020,0x0000);                                                            
        LCD_WriteReg(0x0021,0x0000);           
///////////////////////////////////////////////////////      
//        for(i=50000;i>0;i--);
//		for(i=50000;i>0;i--);
				LCD_WriteReg(0x0030,0x0007); 
				LCD_WriteReg(0x0031,0x0707);   
        LCD_WriteReg(0x0032,0x0006);
        LCD_WriteReg(0x0035,0x0704);
        LCD_WriteReg(0x0036,0x1f04); 
        LCD_WriteReg(0x0037,0x0004);
        LCD_WriteReg(0x0038,0x0000);        
        LCD_WriteReg(0x0039,0x0706);     
        LCD_WriteReg(0x003c,0x0701);
        LCD_WriteReg(0x003d,0x000f);
//        for(i=50000;i>0;i--);
//		for(i=50000;i>0;i--);

        LCD_WriteReg(0x0050,0x0000);        
        LCD_WriteReg(0x0051,0x00ef);   
        LCD_WriteReg(0x0052,0x0000);     
        LCD_WriteReg(0x0053,0x013f);

        LCD_WriteReg(0x0060,0xa700);        
        LCD_WriteReg(0x0061,0x0001); 
        LCD_WriteReg(0x006a,0x0000);
        LCD_WriteReg(0x0080,0x0000);
        LCD_WriteReg(0x0081,0x0000);
        LCD_WriteReg(0x0082,0x0000);
        LCD_WriteReg(0x0083,0x0000);
        LCD_WriteReg(0x0084,0x0000);
        LCD_WriteReg(0x0085,0x0000);
      
        LCD_WriteReg(0x0090,0x0010);     
        LCD_WriteReg(0x0092,0x0000);  
        LCD_WriteReg(0x0093,0x0003);
        LCD_WriteReg(0x0095,0x0110);
        LCD_WriteReg(0x0097,0x0000);        
        LCD_WriteReg(0x0098,0x0000);  
         //display on sequence     
        LCD_WriteReg(0x0007,0x0133);
    
        LCD_WriteReg(0x0020,0x0000);                                                            
        LCD_WriteReg(0x0021,0x0000);
	}		
	Delay_10ms(5);	     /*延时50ms*/
	LCD_Clear(BLACK);
}


