/********************************************************
*  ����WEEEDK NUAA_CM3_107ʵ�鿪����
*  RS-485����ͨ��ʵ������
   ��ʾ������RS��485ͨ���������ѯ���ͣ��жϽӿڣ�Ӳ����������
   RS485��PD5��TXD2����PD6��RXD2�� ��PD7��485DIR������Ҫ�̽�JP25��JP26���ӵ�485TXD��485RXD
   �����ʿ������޸�U2Baud��Ĭ��9600bps
ͨ��Э�飺
�������ͣ���ַ�ֽ� �����ֽ� �����ֽ�  У���
�ӻ����ͣ���ַ�ֽ� �����ֽ� ������ 		У���
��1��01����=���ؿ������ 
�����������
		���ݣ�0x00ֱ�����ֹͣ��0x01ֱ�������ת��0x02ֱ�������ת,
		���ݣ�0x10������ͣ��0x11��������
		���ݣ�0x20�̵���1ֹͣ��0x21�̵���1����
		���ݣ�0x40�̵���2ֹͣ��0x41=�̵���2����
		���ݣ�0x50 LED1��0x51 LED1��
		���ݣ�0x60 LED2��0x61 LED2��
		���ݣ�0x70 LED3��0x71 LED3��
		���ݣ�0x80 LED4��0x81 LED4��
		���ݣ�0x90 ˫ɫLEDR��0x91 LEDR����0x92 LEDG��0x93��
		���ݣ�0xA0 �������ֹͣ��0x91���������ת��0x92���������ת
�ӻ�������Ϣ��
		��ַ�ֽ� ���� ���� У���
��2��02����=��ȡģ����ת����������
�����������
		����=0x00�ڲ��¶ȣ�0x01��DS18B20�������¶ȣ�0x02��PT100�������¶�
		����=0x03���ص�λ����λ��0x04ģ��������		
�ӻ�������Ϣ��
		��ַ�ֽ� ���� ���� У���     ����Ϊ�����õ�����������Ӧ���������¶ȵ�ѹ�ȣ�2�ֽڣ����ֽ���ǰΪ���������ֽ��ں�ΪС����
��3��03����=�������ȷ���
    ����=0x00ֹͣ������0x01~08 ��1��2��3��4��5��6��7��1���ߣ�					

�������س��򵽿����岢���߷������У������غ��ѻ���λ����

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
	*���ƣ�void Delay(void)
	*��������
	*���أ���
	*���ܣ�����ʱ
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
	Delay_Init();               //ʹ��SysTickʵ�־�ȷ��ʱ
	LCD_Init();					//LCD��ʼ��
	USART_RS485Configuration(U2Baud);
	NVIC_Configuration();		
	Welcome(); 		//��ʾ������
	LED1(1);LED2(1);LED3(1);LED4(1);
	 if(MasterSlave==0) 	//�ӻ�ʱƽ�����ڱ������գ�ֻ�з�����Ϣ������ʱ�ŷ��͡�
	 {
		 GPIO_ResetBits(GPIOD,GPIO_Pin_7);  //PD7=0���������μ�485�ӿ����ӣ�
	 } 
	 ADDR=1;
	while(1)
	{		
 if(MasterSlave==0)
 {
		if (ReceiveFlag)	   //���ڽ��յ���Ч����
	{
		if (USART_RX_BUF[1]==0x01)
		{
		 GPIO_SetBits(GPIOD,GPIO_Pin_7);  //PD7=1����	
		 	data=(USART_RX_BUF[0]<<24)|(USART_RX_BUF[1]<<16)|(USART_RX_BUF[2]<<8)|USART_RX_BUF[3];		
			sprintf(str,"%8x",data);
			for (i=0;i<8;i++) dstr[i]=str[i];
			dstr[0]=0x30;
			Gui_StrCenter(0,230,YELLOW,RED,dstr,16,0);//������ʾ=
			switch(USART_RX_BUF[2])
			{
				case 0x00:
					IA(0);IB(0);break;	//���ֹͣ
				case 0x01:
					IA(1);IB(0);break;	//�����תm
				case 0x02:
					IA(0);IB(1);break;	//�����ת
				case 0x10:
					BEEP(1);break;			//������ֹͣ
				case 0x11:
					BEEP(0);break;			//��������
				case 0x20:
					RELAY1(1);break;		//�̵���1ֹͣ
				case 0x21:
					RELAY1(0);break;		//�̵���1����
				case 0x40:
					RELAY2(1);break;		//�̵���2ֹͣ
				case 0x41:
					RELAY2(0);break;		//�̵���2����
				case 0x50:
					LED1(1);break;			//LED1��
				case 0x51:
					LED1(0);break;			//LED1��
				case 0x60:
					LED2(1);break;			//LED2��
				case 0x61:
					LED2(0);break;			//LED2��
				case 0x70:
					LED3(1);break;			//LED3��
				case 0x71:
					LED3(0);break;			//LED3��
				case 0x80:
					LED4(1);break;			//LED4��
				case 0x81:
					LED4(0);break;			//LED4��
				case 0x90:
					LEDRed(1);break;		//LEDR��
				case 0x91:
					LEDRed(0);break;		//LEDR��
			}
		for (i=0;i<4;i++) {USART_SendData(USART2,USART_RX_BUF[i]);while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));}
				ReceiveFlag=0;
	
   	Delay_ms(300);

			GPIO_ResetBits(GPIOD,GPIO_Pin_7);  //PD7=0���������μ�485�ӿ����ӣ�
	
	//	BEEP(1);
	//	Delay_ms(300);
	//	BEEP(0);
		}			
		}	
	}else
 
 	//����ģʽ�����͡�
	 {
		 
		 GPIO_SetBits(GPIOD,GPIO_Pin_7);  //PD7=1���������μ�485�ӿ����ӣ�
	 	 USART_TX_BUF[0]=ADDR;
	 	 USART_TX_BUF[1]=0x01;//��01����
	 	 USART_TX_BUF[2]=0x01;//�����ת
	 	 USART_TX_BUF[3]=USART_TX_BUF[0]+USART_TX_BUF[1]+USART_TX_BUF[2];
	 	 for (i=0;i<4;i++) USART_SendData(USART2,USART_TX_BUF[i]);		
		Delay_ms(10);	
		 GPIO_ResetBits(GPIOD,GPIO_Pin_7);  //PD7=0��������
		 
	 } 	
	  
	 }
}


/************************************
*  ��printfӦ����USART2����
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
