/********************************************************
*  ����WEEEDK NUAA_CM3_107ʵ�鿪����
*  UART����ͨ��ʵ������
   ��ʾ������UART1��UART2�������ڵ�ͨ���������ѯ���ͣ��жϽӿڣ�Ӳ����������
   UART1��PA9��TXD1����PA10��RXD1������Ҫ�̽�JP23��JP24���ӵ�U1TXD��U1RXD
   UART2��PD5��TXD2����PD6��RXD2�� ����Ҫ�̽�JP25��JP26���ӵ�U2TXD��U2RXD
   �����ʿ������޸�U1Baud��U2Baud��Ĭ��115200bps
   ʵ�鲽�裺
��1����Ҫ�̽�JP23��JP24���ӵ�U1TXD��U1RXD���̽�JP25��JP26���ӵ�U2TXD��U2RXD
��2������3.2��TFT�������JLCD1����ѡ�����򿪵�Դ��������Դָʾ��ָʾ����
��2������ʾ����������У���ʾ����ʾ��ʼ��Ϣ
��3���򿪴������֣����úö�Ӧ�Ĵ��ںͲ����ʡ��ڷ��Ͷ��������ݿ����ն���ʾ���
     ��������1�ַ�������HEX�����ͣ�����LED1ָʾ�ƣ����ն���ʾTurn on LED1
		 ��������2�ַ����ͣ�ͬʱ����LED2ָʾ�ƣ����ն���ʾTurn on LED2
		 ��������3�ַ����ͣ�ͬʱ����LED3ָʾ�ƣ����ն���ʾTurn on LED3
		 ��������5�ַ����ͣ�ͬʱ����LED1~LED3ָʾȫ�ƣ����ն���ʾTurn on all LED
		 ��������0��6�ַ����ͣ�LED1~LED3ָʾȫ�𣬽��ն���ʾTurn off all LED
	�������س��򵽿����岢���߷������У������غ��ѻ���λ����

**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"
#include "delay.h"
#include "DS18B20.h"

#define U1Baud   115200

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

uint8_t ReceiveFlag=0;
extern char res;
int main(void)
{
uint8_t KEY=0;
	SystemInit();
	GPIO_Configuration();
	Delay_Init();               //ʹ��SysTickʵ�־�ȷ��ʱ
	LCD_Init();					//LCD��ʼ��
	USART_Configuration(U1Baud);

   USART1_SendString("\r\n UART1ͨ��ʵ��");

	NVIC_Configuration();		
	Welcome(); 		//��ʾ������
	LED1(1);LED2(1);LED3(1);LED4(1);
  printf("\r\n ������1~3���ַ�����LED������ \r\n");
	while(1)
	{		
	 if (ReceiveFlag)	   //��������Ч������һ��������
	{
		switch(res)
		{
			case '1':USART1_SendString("\r\n Turn on LED1.");
				LED1(0);LED2(1);LED3(1);
				Gui_StrCenter(0,230,WHITE,BLACK,"   Turn on LED1!    ",16,0);//������ʾ
				ReceiveFlag=1;
				break;
			case '2':USART1_SendString("\r\n Turn on LED2.");
				LED1(1);LED2(0);LED3(1);
				Gui_StrCenter(0,230,WHITE,BLACK,"   Turn on LED2!    ",16,0);//������ʾ
				ReceiveFlag=1;
				break;
			case '3':USART1_SendString("\r\n Turn on LED3.");
				LED1(1);LED2(1);LED3(0);
				Gui_StrCenter(0,230,WHITE,BLACK,"   Turn on LED3!    ",16,0);//������ʾ
				ReceiveFlag=1;
				break;
			case '5':USART1_SendString("\r\n Turn on all LED.");
				LED1(0);LED2(0);LED3(0);
				Gui_StrCenter(0,230,WHITE,BLACK,"   Turn on LED1~LED3!    ",16,0);//������ʾ
				ReceiveFlag=1;
				break;
			case '0':
			case '6':USART1_SendString("\r\n Turn off all LED. ");
				ReceiveFlag=1;
				LED1(1);LED2(1);LED3(1);
				Gui_StrCenter(0,230,WHITE,BLACK,"Turn off LED1~LED3!",16,0);//������ʾ
				break;
	default:
				ReceiveFlag=1;
				LED1(1);LED2(1);LED3(1);
				Show_Str(50,230,WHITE,BLACK,"�Ƿ�����:         ",16,0);//������ʾ
				LCD_ShowChar(0,230,WHITE,BLACK,res,16,0);//������ʾ
				break;		
		}
   	Delay_ms(300);
		BEEP(1);
		Delay_ms(300);
		BEEP(0);
		ReceiveFlag=0;
	}			 
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)==0) KEY=1;
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)==0) KEY=2;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==0) KEY=3;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0) KEY=4;
	switch(KEY)
		{
		case 1:/*KEY1��������i*/
			USART1_SendString("\r\n UART1ͨ��ʵ��:�㰴����KEY1����");
			Show_Str(50,230,WHITE,BLACK,"�㰴����KEY1����",16,0);//������ʾ
		
			KEY=0;
			break;
		case 2:/*KEY2���������ַ���*/
			USART1_SendString("\r\n UART1ͨ��ʵ��:�㰴����KEY2����");
			Show_Str(50,230,WHITE,BLACK,"�㰴����KEY2����",16,0);//������ʾ
			KEY=0;
			break;
		case 3:/*KEY3���������ַ���*/
			USART1_SendString("\r\n UART1ͨ��ʵ��:�㰴����KEY3����");
			Show_Str(50,230,WHITE,BLACK,"�㰴����KEY3����",16,0);//������ʾ
			KEY=0;
			break;
		case 4:/*KEY3���������ַ���*/
			USART1_SendString("\r\n UART1ͨ��ʵ��:�㰴����KEY4����");
			Show_Str(50,230,WHITE,BLACK,"�㰴����KEY4����",16,0);//������ʾ
			KEY=0;
			break;
		}
    
	PDout(7)=~PDout(7);	   //LED4(PD7)����ʾ
   Delay();
   }
}


/************************************
*  ��printfӦ����USART2����
***************************************/
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);   
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return ch;
}



/***********************END OF FILE****/
