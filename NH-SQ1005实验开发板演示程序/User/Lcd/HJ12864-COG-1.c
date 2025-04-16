/*****************************************************
** Descriptions:     ��HJ12864-COG-1Һ����ʾ����ص�Ӳ������
**																		 
*****************************************************************/ 

#include "HZ_ASCII.h"
#include "stdio.h"
#include "delay.h"
#include "stdlib.h"
#include "HJ12864-COG-1.h"

/* Private variable ---------------------------------------------------------*/
//uint16_t DeviceCode; //LCD��ID�ű���


/*********************************************************************************
��������void LCD_GPIO_Configuration(void)
���ܣ����ò���ʼ��LCD���ݺͿ��ƶ˿�
�����������
�����������
*********************************************************************************/
void LCD_HJ12864Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*������Ӧʱ�� LCD(HJ128564-COG-1)�õ�PC��PD�˿�*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD,ENABLE); 
							  
	/*�����������Ϊ�������*/
	/*GPIOC*/
	GPIO_InitStructure.GPIO_Pin = CSB|MOSI|SCLK|nCS|SCL|SDA;  	//���SDA����Ϊ��©�������SDA�Ե�ԴҪ����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;								
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* A0_RS  RSTB  GPIOD�������*/
	GPIO_InitStructure.GPIO_Pin = A0_RS| RSTB;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/*������������Ϊ��������*/
	/*GPIOC*/
	GPIO_InitStructure.GPIO_Pin = MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

/*********************************************************************************
	*���ƣ�void LCD_Init(void)
	*���룺��
	*���أ���
	*���ܣ�LCD��ʼ��
	*˵����
*********************************************************************************/

//���ֿ�IC��ȡ���ֻ��ַ����ݣ�һ���ֽڣ�
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

/*----------------��LCDд����-----------------*/
void WRITE_DatH(uint8_t dat)
{
   uint8_t i;
   LCD_CSB(0);
   LCD_RS(1);//����
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
/*----------------��LCDд����-----------------*/
void WRITE_COMH(uint8_t com)
{
   uint8_t i;
   LCD_CSB(0);
   LCD_RS(0);//����
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
//	delayus(3);	//��λҪ��3us���ϣ�������3ms
	LCD_RSTB(1);
//	delayus(10);//���ڲ���λʱ��6ms,����ѡ��10ms
	WRITE_COMH(0xe2);//��λ
//	delayns(10);
	WRITE_COMH(0x2c);//��ѹ
//	delayns(50);
	WRITE_COMH(0x2e);//��ѹ
//	delayns(50);
	WRITE_COMH(0x2f);//��ѹ
//	delayns(10);
	WRITE_COMH(0x23);//�ֵ��Աȶ�
//	delayns(10);
	WRITE_COMH(0x81);//΢���Աȶ�
//	delayns(10);
	WRITE_COMH(0x28);//΢���Աȶ�ֵ
//	delayns(10);
	WRITE_COMH(0xa2);//1/9ƫѹ��
//	delayns(10);
	WRITE_COMH(0xC8);//��ɨ��˳�����ϵ���
	WRITE_COMH(0xA0);//��ɨ��˳��������
	WRITE_COMH(0x40);//��һ�п�ʼ
	WRITE_COMH(0xAF);//����ʾ
}

void clear_screenHJ(void)	   //��LCD������
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
	WRITE_COMH(0xb0+page-1);//����ҳ��ַ
	WRITE_COMH(0x10+(column>>4&0x0f));//�����е�ַ�ĸ�4λ
	WRITE_COMH(column&0x0f);//�����е�ַ�ĵ�4λ
}
//��ָ��ֿ�IC
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

//��ָ����ַ��������16X16д��Һ����ָ��(page,column)������
void get_and_write_16x16(uint32_t fontaddr,uint8_t page,uint8_t column,uint8_t fb)
{
uint8_t i,j,disp_data,tut[100]={0};
	LCD_nCS(0);
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16); //��ַ�ĸ�8 λ,��24 λ
	send_command_to_ROM((fontaddr&0xff00)>>8); //��ַ����8 λ,��24 λ
	send_command_to_ROM(fontaddr&0xff); //��ַ�ĵ�8 λ,��24 λ
	for (j=0;j<2;j++)
	{
	LCD_Addr(page+j,column);
	for (i=0;i<16;i++)
	{
	  disp_data=get_data_from_ROM();
	  tut[i+j*16]=disp_data;
	  if (fb==1)  disp_data=~disp_data;
	  WRITE_DatH(disp_data);//д���ݵ�LCD��ÿд��һ�����ݣ���ַ��1
	}	
	}
	LCD_nCS(1);

}

//��ָ����ַ������8X16��д��Һ����ָ��(page,column)
void get_and_write_8x16(uint32_t fontaddr,uint8_t page,uint8_t column,uint8_t fb)
{
uint8_t i,j,disp_data;
	LCD_nCS(0);
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16); //��ַ�ĸ�8 λ,��24 λ
	send_command_to_ROM((fontaddr&0xff00)>>8); //��ַ����8 λ,��24 λ
	send_command_to_ROM(fontaddr&0xff); //��ַ�ĵ�8 λ,��24 λ
	for (j=0;j<2;j++)
	{
	LCD_Addr(page+j,column);
	for (i=0;i<8;i++)
	{
	  disp_data=get_data_from_ROM();
	  if (fb==1) disp_data=~disp_data; 
	  WRITE_DatH(disp_data);//д���ݵ�LCD��ÿд��һ�����ݣ���ַ��1
	}	
	}
	LCD_nCS(1);
}
//��ָ����ַ��������д��Һ����ָ����page,column)������
void get_and_write_5x8(uint32_t fontaddr,uint8_t page,uint8_t column,uint8_t fb)
{
uint8_t i,disp_data;
	LCD_nCS(0);
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16); //��ַ�ĸ�8 λ,��24 λ
	send_command_to_ROM((fontaddr&0xff00)>>8); //��ַ����8 λ,��24 λ
	send_command_to_ROM(fontaddr&0xff); //��ַ�ĵ�8 λ,��24 λ
	LCD_Addr(page,column);
	for (i=0;i<5;i++)
	{
	  disp_data=get_data_from_ROM();
	  if (fb==1) disp_data=~disp_data; 
	  WRITE_DatH(disp_data);//д���ݵ�LCD��ÿд��һ�����ݣ���ַ��1
	}	
	LCD_nCS(1);
}

void display_HJGB2312_string(uint8_t page,uint8_t column,uint8_t *text,uint8_t fb)
{
	uint8_t i=0;
	uint32_t fontaddr;//,MSB,LSB;
//	uint32_t BaseAdd=0;
 //������庺�����ֿ�IC�еĵ�ַ���㣺
 //address=((MSB-0xb0)*94+(LSB-0xa1)+846)*32+BaseAddr;BaseAdd=0
 //	MSB=text[i];
//	LSB=text[i+1];
	while((text[i]>0x00))
	{
	if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1))
	{
//������壨GB2312���������ֿ�IC �еĵ�ַ�����¹�ʽ�����㣺
//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
//���ڵ���8 λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ
	fontaddr = (text[i]- 0xb0)*94;
	fontaddr += (text[i+1]-0xa1)+846;
	fontaddr = (fontaddr*32);
	get_and_write_16x16(fontaddr,page,column,fb); //��ָ����ַ��������д��Һ����ָ����page,column)������
	i+=2;
	column+=16;
	}
	else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
{
//������壨GB2312��15x16 ����ַ����ֿ�IC �еĵ�ַ�����¹�ʽ�����㣺
//Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0
//���ڵ���8 λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ
	fontaddr = (text[i]- 0xa1)*94;
	fontaddr += (text[i+1]-0xa1);
	fontaddr = (fontaddr*32);
	get_and_write_16x16(fontaddr,page,column,fb); //��ָ����ַ��������д��Һ����ָ����page,column)������
	i+=2;
	column+=16;
}
	else if((text[i]>=0x20) &&(text[i]<=0x7e))

{
	fontaddr = (text[i]- 0x20);
	fontaddr = fontaddr*16;
	fontaddr = fontaddr+0x3cf80;
	get_and_write_8x16(fontaddr,page,column,fb); //��ָ����ַ��������д��Һ����ָ����page,column)������
	i+=1;
	column+=8;
}
	else
	i++;
	}
}  
/*��ʾ16x16 ����ͼ�񡢺��֡���Ƨ�ֻ�16x16 ���������ͼ��
 ��ڣ�fb=1������ʾ��fb=0������ʾ
       pageΪҳ�ţ�1,3,5,7��ʾ��һ�������У������У���cloumnΪ��1��128
	   dpΪҪ��ʾ���ֵ���Ϣ������
                                                           */
void display_graphic_16x16(uint8_t page,uint8_t column,uint8_t *dp,uint8_t fb)
{
uint8_t i,j;
for(j=0;j<2;j++)
{
LCD_Addr(page+j,column);
for (i=0;i<16;i++)
{
if (fb==0) WRITE_DatH(*dp); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
else       WRITE_DatH(~*dp);
dp++;
}
}
}
//x2 ���ֵĿ�ȣ�y2Ϊ���ָ߶�
void display_graphic_xy1(uint8_t page,uint8_t column,uint8_t x2,uint8_t y2,const uint8_t *dp,uint8_t fb)
{
	uint8_t i,j;
	for(j=0;j<y2/8;j++)
	{
		LCD_Addr(page+j,column);
		for (i=0;i<x2;i++)	   //
		{
			if (fb==1)  WRITE_DatH(~*dp); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1  ������ʾ
			WRITE_DatH(*dp); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
			dp++;
		}
	}
}
//x2 ���ֵĿ�ȣ�y2Ϊ���ָ߶�
void display_graphic_xy(uint8_t page,uint8_t column,uint8_t x2,uint8_t y2,uint8_t *dp,uint8_t fb)
{
	uint8_t i,j;
	for(j=0;j<y2/8;j++)
	{
		LCD_Addr(page+j,column);
		for (i=0;i<x2;i++)	   //
		{
			if (fb==1)  WRITE_DatH(~*dp); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1  ������ʾ
			WRITE_DatH(*dp); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
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
get_and_write_5x8(fontaddr,page,column,fb); //��ָ����ַ��������д��Һ����ָ����page,column)������
i+=1;
column+=6;
}
else
i++;
}
}

void display_Line(uint8_t column)   //��������,columnΪ�к�
{
uint8_t j;

for(j=0;j<8;j++)
{
LCD_Addr(j+1,column);
WRITE_DatH(0xff); //д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1
} 

}  
//��ʾ128x64 ����߿�


void display_bk()
{
uint8_t i,j;

//���
for(j=0;j<8;j++)
{
LCD_Addr(j+1,1);
WRITE_DatH(0xff); 
}

//�ҿ�
for(j=0;j<8;j++)
{
LCD_Addr(j+1,127);
WRITE_DatH(0xff); 
}

//�Ͽ�
LCD_Addr(1,2);
for (i=0;i<125;i++)		 //0
{
WRITE_DatH(0x01);
}

//�¿�
/*LCD_Addr(8,2);
for (i=0;i<125;i++)
{
WRITE_Dat(0x80); 
}	 */

}										  


void displayInformationHJ(void)		//��ʾ��Ϣ������״̬���⣩
{
  display_HJGB2312_string(1,1,"NH-SQ1005CM3 Kit",0);
	display_HJGB2312_string(3,1,"����ͼ���ַ�ʾ�� ",0);
	display_HJGB2312_string(5,1," HJ12864-COG-1! ",0);
	display_HJGB2312_string(7,1,"�Ͼ����պ����ѧ ",0);	
}


