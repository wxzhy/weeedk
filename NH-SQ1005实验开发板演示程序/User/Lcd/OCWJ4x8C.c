/*****************************************************
** Descriptions:     ��OCWJ4x8CҺ����ʾ����ص�Ӳ������
**																		 
*****************************************************************/ 

#include "HZ_ASCII.h"
#include "stdio.h"
#include "delay.h"
#include "stdlib.h"
#include "OCWJ4x8C.h"

/* Private variable ---------------------------------------------------------*/
//uint16_t DeviceCode; //LCD��ID�ű���


/*********************************************************************************
��������void LCD_GPIO_Configuration(void)
���ܣ����ò���ʼ��LCD���ݺͿ��ƶ˿�
�����������
�����������
*********************************************************************************/
void LCD_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*������Ӧʱ�� LCD(OCWJ4x8C)�õ�PC(PC6)��PD��PD13��PD14��PD15����PE��PE0~PE7���˿�*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE,ENABLE); 
							  
	/*�����������Ϊ�������*/
	/*GPIOE   D0~D7 */
	GPIO_InitStructure.GPIO_Pin =0xFF;//PE0~PE7  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;								
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* RS  RW  RSTB GPIOD�������*/
	GPIO_InitStructure.GPIO_Pin = RS| RW |RSTB;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* E  GPIOC�������*/
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

void  WRITE_COM(uint8_t DATA)          //Һ������д��
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
  /**********��D7��PE7��Ϊ��������*******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	GPIO_Init(GPIOE, &GPIO_InitStructure);
   if((GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)!=0)&&(tmp<3))
   {
    shortdelay(1);
    tmp++;
   }
  /**********��D7��PE7���ص��������*******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(GPIOE, &GPIO_InitStructure);

   LCD_RW(0);//IO2CLR=RW;
 }
 void WRITE_DAT(uint8_t DATA)   //Һ������д��
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
  /**********��D7��PE7��Ϊ��������*******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	GPIO_Init(GPIOE, &GPIO_InitStructure);
   if((GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)!=0)&&(tmp<3))
   {
    shortdelay(1);
    tmp++;
   }
  /**********��D7��PE7���ص��������*******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_Init(GPIOE, &GPIO_InitStructure);
   LCD_RW(0);//IO2CLR=RW;
  }


/*********************************************************************************
	*���ƣ�void LCD_Init_OCWJ4X8C(void)
	*���룺��
	*���أ���
	*���ܣ�LCD��ʼ��
	*˵����
*********************************************************************************/
void  LCD_Init_OCWJ4X8C(void)                //Һ����ʼ��
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
�����ַ�����ʾ���������ֿ⣩OCWJ4x8CΪ�����ֿ��Һ����
��ڣ�tΪ�ַ�������DATAΪ�ַ�λ��
*************************************************************/
void display_GB2312_string(char *a,uint8_t t,uint8_t DATA)       //���Ӧ��λ��д��Ӧ���������ֻ���
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
�����ַ��ķ�����ʾ���������ֿ⣩OCWJ4x8CΪ�����ֿ��Һ����
��ڣ�tΪ�ַ�������DATAΪ�ַ�λ��
*************************************************************/
void display_GB2312_string_fb(char *a,uint8_t t,uint8_t DATA)       //���Ӧ��λ��д��Ӧ���������ֻ���
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

 void clear_screen()           //���Һ��RAM
 {
   uint16_t DATA;
   DATA=0x30;
   WRITE_COM(DATA);
   
   DATA=0x01;
   WRITE_COM(DATA);
   shortdelay(2000);   //���RAM���ӳٱ����㹻��,���򽫳��ֻ���
   
 } 
 void fb_line(uint8_t n)           //ָ����n�з��ף�n=0,1,2,3��
 {
uint8_t DATA=0;
 //  DATA=0x30;
  // WRITE_COM(DATA);
   DATA=n|(1<<2);
   WRITE_COM(DATA);
   shortdelay(2000);   //���RAM���ӳٱ����㹻��,���򽫳��ֻ���
   
 } 	  



void displayInformation(void)		//��ʾ��Ϣ������״̬���⣩
{
	fb_line(2);
//  uint8_t i;
//WRITE_COM(0x55);
  //	display_bk();
  //	for (i=0;i<96;i++) Mtext[i]=FlagWLL[i];//ͼ��
//	display_graphic_xy(3,5,16,16,Mtext,0);
//	display_string_5x8(2,16,"NUAA_CM3_107 exp.",0);
	display_GB2312_string("NH-SQ1005CM3 Kit",16,0x80);
	display_GB2312_string("����ͼ���ַ�ʾ��",16,0x90);
	display_GB2312_string(" OCWJ4x8C(12864)!",16,0x88);
	display_GB2312_string("�Ͼ����պ����ѧ",16,0x98);
//	display_string_5x8(8,5,"0123456789ABCDEFGHIJ",0);

}


