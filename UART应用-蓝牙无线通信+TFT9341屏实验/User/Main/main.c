/********************************************************
*  ����WEEEDK NUAA_CM3_107ʵ�鿪����
*  USART3Ӧ�ã�����ͨ��ʵ������
   ��ʾ����������ͨ��ģ�����ֻ�ͨ���������ѯ���ͣ��жϽӿڣ�Ӳ����������
   USART3������ģ�����ӣ�PB10��TXD3����PB11��RXD3������Ҫ����JP15���Ͽ�PB10�룩�����¶ȴ����������ӡ�
   �����ʿ������޸�U3Baud��Ĭ��9600bps
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


#define U3Baud   9600



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
	USART_BHT_Configuration(U3Baud);
	NVIC_Configuration();		
	Welcome(); 		//��ʾ������
	LED1(1);LED2(1);LED3(1);LED4(1);
	 ADDR=1;
	while(1)
	{		
		if (ReceiveFlag==2)
		{
					USART_SendData(USART3,USART_RX_BUF[0]);while(!USART_GetFlagStatus(USART3, USART_FLAG_TXE));
			data=USART_RX_BUF[0];	
			sprintf(str,"%2x",data);
			dstr[0]=0x30;dstr[1]=0x78;dstr[2]=str[0];dstr[3]=str[1];
			Gui_StrCenter(0,230,YELLOW,RED,dstr,16,0);//������ʾ=
			ReceiveFlag=0;	
			switch(USART_RX_BUF[0])
			{
				case 0x53:	//S
					IA(0);IB(0);break;	//���ֹͣ
				case 0x5A:		//Z
					IA(0);IB(1);break;	//�����ת
				case 0x46:		//F
					IA(1);IB(0);break;	//�����ת
				case 0x73:		//s
					BEEP(0);break;			//������ֹͣ
				case 0x62:		//b
					BEEP(1);break;			//��������
				case 0x4D:		//M
					LED1(1);LED2(1);LED3(1);LED4(1);//LED��
					break;			
				case 0x4C:		//L
					LED1(0);LED2(0);LED3(0);LED4(0);//LED��
					break;			
				case 0x52:		//R
				GPIO_SetBits(GPIOB, GPIO_Pin_1);
				GPIO_ResetBits(GPIOB, GPIO_Pin_2);
					break;			
				case 0x47:		//G
				GPIO_ResetBits(GPIOB, GPIO_Pin_1);
				GPIO_SetBits(GPIOB, GPIO_Pin_2);
					break;			
			}
	}
		
else
		if (ReceiveFlag)	   //���ڽ��յ���Ч����
	{
	 
		if (USART_RX_BUF[1]==0x01)
		{
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
					LEDR(1);break;		//LEDR��
				case 0x91:
					LEDR(0);break;		//LEDR��
			}
		for (i=0;i<4;i++) {USART_SendData(USART3,USART_RX_BUF[i]);while(!USART_GetFlagStatus(USART3, USART_FLAG_TXE));}
				ReceiveFlag=0;
	
   	Delay_ms(300);

	
	//	BEEP(1);
	//	Delay_ms(300);
	//	BEEP(0);
		}			
	}}
	
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
