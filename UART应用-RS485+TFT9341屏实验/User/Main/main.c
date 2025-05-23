/********************************************************
*  采用WEEEDK NUAA_CM3_107实验开发板
*  RS-485串行通信实验例程
   本示例测试RS－485通信情况，查询发送，中断接口，硬件连接如下
   RS485：PD5（TXD2），PD6（RXD2） ，PD7（485DIR），需要短接JP25和JP26连接到485TXD和485RXD
   波特率可设置修改U2Baud，默认9600bps
通信协议：
主机发送：地址字节 命令字节 数据字节  校验和
从机回送：地址字节 命令字节 数据域 		校验和
（1）01命令=开关控制命令， 
主机发送命令：
		数据＝0x00直流电机停止，0x01直流电机正转，0x02直流电机反转,
		数据＝0x10蜂鸣器停，0x11蜂鸣器响
		数据＝0x20继电器1停止，0x21继电器1动作
		数据＝0x40继电器2停止，0x41=继电器2动作
		数据＝0x50 LED1灭，0x51 LED1亮
		数据＝0x60 LED2灭，0x61 LED2亮
		数据＝0x70 LED3灭，0x71 LED3亮
		数据＝0x80 LED4灭，0x81 LED4亮
		数据＝0x90 双色LEDR灭，0x91 LEDR亮，0x92 LEDG灭，0x93亮
		数据＝0xA0 步进电机停止，0x91步进电机正转，0x92步进电机反转
从机返回信息：
		地址字节 命令 数据 校验和
（2）02命令=获取模拟量转换的数字量
主机发送命令：
		数据=0x00内部温度，0x01由DS18B20测量的温度，0x02由PT100测量的温度
		数据=0x03板载电位器电位，0x04模拟电流检测		
从机返回信息：
		地址字节 命令 数据 校验和     数据为测量得到的数字量对应物理量如温度电压等（2字节，高字节在前为整数，低字节在后为小数）
（3）03命令=控制喇叭发声
    数据=0x00停止发声，0x01~08 发1，2，3，4，5，6，7，1（高）					

编译下载程序到开发板并在线仿真运行，或下载后脱机复位运行

**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"
#include "delay.h"
#include "DS18B20.h"


#define U2Baud   9600



u8 key;


#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

/* Private functions ---------------------------------------------------------*/
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
#define LastAddrNo  10
//extern u8 UreceiveSize;
uint8_t ReceiveFlag=0,ADDR;
extern u16 INDEX;
extern u8 USART_RX_BUF[10];
extern u8 USART_TX_BUF[10];
	u32 data;
int main(void)
{

uint8_t i;
	char str[8];
	u8 dstr[8];
	SystemInit();
	GPIO_Configuration();
	Delay_Init();               //使用SysTick实现精确延时
	LCD_Init();					//LCD初始化
	USART_RS485Configuration(U2Baud);
	NVIC_Configuration();		
	Welcome(); 		//显示主界面
	LED1(1);LED2(1);LED3(1);LED4(1);
	 if(MasterSlave==0) 	//从机时平常处于被动接收，只有返回信息给主机时才发送。
	 {
		 GPIO_ResetBits(GPIOD,GPIO_Pin_7);  //PD7=0接收允许（参见485接口连接）
	 } 
	 ADDR=1;
	while(1)
	{		
 if(MasterSlave==0)
 {
		if (ReceiveFlag)	   //串口接收到有效命令
	{
		if (USART_RX_BUF[1]==0x01)
		{
		 GPIO_SetBits(GPIOD,GPIO_Pin_7);  //PD7=1发送	
		 	data=(USART_RX_BUF[0]<<24)|(USART_RX_BUF[1]<<16)|(USART_RX_BUF[2]<<8)|USART_RX_BUF[3];		
			sprintf(str,"%8x",data);
			for (i=0;i<8;i++) dstr[i]=str[i];
			dstr[0]=0x30;
			Gui_StrCenter(0,230,YELLOW,RED,dstr,16,0);//居中显示=
			switch(USART_RX_BUF[2])
			{
				case 0x00:
					IA(0);IB(0);break;	//电机停止
				case 0x01:
					IA(1);IB(0);break;	//电机正转m
				case 0x02:
					IA(0);IB(1);break;	//电机反转
				case 0x10:
					BEEP(1);break;			//蜂鸣器停止
				case 0x11:
					BEEP(0);break;			//蜂鸣器响
				case 0x20:
					RELAY1(1);break;		//继电器1停止
				case 0x21:
					RELAY1(0);break;		//继电器1动作
				case 0x40:
					RELAY2(1);break;		//继电器2停止
				case 0x41:
					RELAY2(0);break;		//继电器2动作
				case 0x50:
					LED1(1);break;			//LED1灭
				case 0x51:
					LED1(0);break;			//LED1亮
				case 0x60:
					LED2(1);break;			//LED2灭
				case 0x61:
					LED2(0);break;			//LED2亮
				case 0x70:
					LED3(1);break;			//LED3灭
				case 0x71:
					LED3(0);break;			//LED3亮
				case 0x80:
					LED4(1);break;			//LED4灭
				case 0x81:
					LED4(0);break;			//LED4亮
				case 0x90:
					LEDRed(1);break;		//LEDR灭
				case 0x91:
					LEDRed(0);break;		//LEDR亮
			}
		for (i=0;i<4;i++) {USART_SendData(USART2,USART_RX_BUF[i]);while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));}
				ReceiveFlag=0;
	
   	Delay_ms(300);

			GPIO_ResetBits(GPIOD,GPIO_Pin_7);  //PD7=0接收允许（参见485接口连接）
	
	//	BEEP(1);
	//	Delay_ms(300);
	//	BEEP(0);
		}			
		}	
	}else
 
 	//主机模式允许发送。
	 {
		 
		 GPIO_SetBits(GPIOD,GPIO_Pin_7);  //PD7=1发送允许（参见485接口连接）
	 	 USART_TX_BUF[0]=ADDR;
	 	 USART_TX_BUF[1]=0x01;//发01命令
	 	 USART_TX_BUF[2]=0x01;//电机正转
	 	 USART_TX_BUF[3]=USART_TX_BUF[0]+USART_TX_BUF[1]+USART_TX_BUF[2];
	 	 for (i=0;i<4;i++) USART_SendData(USART2,USART_TX_BUF[i]);		
		Delay_ms(10);	
		 GPIO_ResetBits(GPIOD,GPIO_Pin_7);  //PD7=0接收允许
		 
	 } 	
	  
	 }
}


/************************************
*  将printf应用于USART2发送
***************************************/
int fputc(int ch, FILE *f)
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART1, (uint8_t) ch);   
	
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}
	
	return ch;
}



/***********************END OF FILE****/
