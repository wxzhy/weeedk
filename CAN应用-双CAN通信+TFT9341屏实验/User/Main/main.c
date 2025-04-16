/********************************************************
*  ����NUAA_CM3_107  WEEEDKʵ�鿪����
**	CAN����ͨ��ʵ�飺����STM32F107���е�����CAN���߽ӿڣ�ֱ���໥ͨ��
   ʵ�鲽�裺
��1����Ҫ�̽�JP10�̽ӵ�C2TX��PB6����JP11�̽ӵ�C2RX��PB5��,�õ��߽�CAN1��CAN2l���ӣ�CANH1����CAH2��CANL1����CANL2
��2������3.2��TFT�������JLCD1����ѡ�����򿪵�Դ��������Դָʾ��ָʾ����
��2������ʾ����������У���ʾ����ʾ��ʼ��Ϣ
��3������KEY1����ͨ���жϷ��������ΪKeyFlag=1,��CAN1����8���ֽ����ݵ�CAN2����ʾ
��4������KEY2����ͨ���жϷ��������ΪKeyFlag=2,��CAN2����8���ֽ����ݵ�CAN1����ʾ
��3������KEY3�������LCD����ʾ������
	�������س��򵽿����岢���߷������У������غ��ѻ���λ����
��5�������и���ҪҪ���͵�����CAN1_DATA��CAN2_DATA�ǵ�ֵ���鿴�շ����
��Ҫ�����Ķ̽������Ͽ�JP6��JP33������JP30��JP31��JP32���Ϸ�ʹPA5��PA6��PA7����SPI���ߵ�SCL��SO��SI
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"
#include "CAN.h"

#include "delay.h"


#define U1Baud   115200
#define U2Baud   115200

extern u8 gImage_fg[];
u8 key;
uint8_t BeepFlag=0;
uint8_t TX_Buffer[256];
uint8_t RX_Buffer[256];
uint8_t Tx1_Buffer[]={0x12,0x34,0x56,0x78,0x90,0xAB,0xCD,0xEF};
//uint8_t Tx1_Buffer[]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
uint8_t Rx1_Buffer[10];
uint8_t KeyFlag = 0x00;
uint8_t StartAdrr=0x0;


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
const char menu[] =
   "\n"
   "+---CAN��CAN1��CAN2֮��ֱ��ͨ�ţ�����ͨ�Ų��Գ���---+\r\n"
   "|                                                   |\r\n"
   "|    �������ʵ����CAN1��CAN2ֱ��ͨ�š�             |\r\n"
   "|    ��CAN1����8���ֽڵ���Ч���ݣ�CAN2�жϽ���      |\r\n"
   "|    ��CAN2����8���ֽڵ���Ч���ݣ�CAN1�жϽ���      |\r\n"
   "|...................................................|\r\n"
   "|    ʹ�þ�����                                     |\r\n"
   "|     ����KEY1����CAN1��������0xC1~0xEF             |\r\n"
   "|     ����KEY2����CAN2��������0xC2~0xF0             |\r\n"
   "+---------------------------------------------------+\r\n";
    
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
	for(i=2000;i>0;i--)
		for(j=1000;j>0;j--);

}

/**************************************************
*�ļ����ƣ�void RCC_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����ϵͳʱ������
***************************************************/
void RCC_Configuration(void)
{
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��GPIOBʱ��

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	 //ʹ������ӳ��˿�ʱ�ӣ���Ϊ����Ҫ��I2C��ӳ�䵽PB8��PB9������Ĭ�ϵ�PB7PB7
 											  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);	 //ʹ��I2C1ʱ��

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��GPIOAʱ��

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //ʹ��USART1ʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
}
/**************************************************
*�ļ����ƣ�void DataToStr(uint8_t *buf,uint8_t *str,uint8_t length)
*
*��ڲ�������ת����ʮ����������������*buf�У�ת�������ݸ������ֽ�����Ϊlength
*
*���ڲ�����ת��Ϊ�ַ�����*str��
*
*����˵����һ�������ж���ֽ�����ת����һ��������ASCII���ַ�����������ʾ
*******************************************************/
void DataToStr(uint8_t *buf,uint8_t *str,uint8_t length)
{
uint8_t i=0,temp;
	for (i=0;i<length;i++)	//������ת��ΪASCC�룬�Ա�����ʾ
	  	 { temp=(buf[i]&0xf0)>>4;//ȡ��4λ
		  if (temp<=9) temp=temp+0x30;
		  else		   temp=temp+0x37;
		  str[2*i]=temp;
		  if ((buf[i]&(0x0f))<=9) str[2*i+1]=(buf[i]&(0x0f))+0x30;
		  else str[2*i+1]=(buf[i]&(0x0f))+0x37;
		 }
}							   

extern uint8_t CAN1_DATA[];  
extern uint8_t CAN2_DATA[];  
int main(void)
{
uint8_t str[40];
uint8_t i=0;
	SystemInit();	                                         //����ϵͳʱ��
	GPIO_Configuration();
  	LED_Configuration();
	Delay_Init();               //ʹ��SysTickʵ�־�ȷ��ʱ
	LCD_Init();					//LCD��ʼ��
	USART_Configuration(U1Baud,U2Baud);

   USART2_SendString("\r\n UART2�����CANͨ��ʵ��\r\n ");
	printf(menu);   

  BUTTON_Configuration();		//����
  	 
  NVIC_Configuration();		    //NVIC

  EXTI_Configuration();			//EXTI��ʼ��

  	CAN_Configuration();		//CAN���߳�ʼ��


	Welcome(); 		//��ʾ������

	KeyFlag=0;
	LED1(1);LED2(1);LED3(1);LED4(1);

	while(1)
	{		
		if (KeyFlag)
		{
		switch(KeyFlag)
		{
			case 0x01:		        //KEY1���£�CAN2���յ�������ʾ������Ļ��
			{
			    KeyFlag=0;			    //�����־λ
				DataToStr(CAN1_DATA,str,8);   
                CAN1_SEND(CAN1ID); //ID��ʶCAN1���͵���0xaa
		Gui_StrCenter(0,150,RED,GREEN,"                            ",16,0);//������ʾ
		Gui_StrCenter(0,150,RED,GREEN,str,16,0);//������ʾ
				 break;				
			}
			case 0x02:			     //KEY2������CAN2�������ݣ�CAN1���յ�������ʾ������Ļ��
			{
			   KeyFlag=0;				 //�����־λ
				DataToStr(CAN2_DATA,str,8);   
                CAN2_SEND(CAN2ID); //ID��ʶCAN2���͵���0xbb
		Gui_StrCenter(0,190,RED,GREEN,"                            ",16,0);//������ʾ
		Gui_StrCenter(0,190,RED,GREEN,str,16,0);//������ʾ
				break;				
			}
			case 0x03:		        //KEY3����(�ж��ñ�־)���CAN1��CAN��������ʾ������
			{
			    KeyFlag=0;			    //�����־λ
		Gui_StrCenter(0,150,RED,GREEN,"                            ",16,0);//������ʾ
		Gui_StrCenter(0,190,RED,GREEN,"                            ",16,0);//������ʾ
		Gui_StrCenter(0,230,WHITE,RED,"                            ",16,0);//������ʾ
		Gui_StrCenter(0,270,WHITE,RED,"                            ",16,0);//������ʾ
				 break;				
			}
		
		}
		if (CAN1_Rec_Flag)
		{
		CAN1_Rec_Flag=0;
		LED2(0);LED1(1); ; // ���յ�CAN2�����ݣ���CAN1�ж��Ѿ����յ�CAN1_Rec�У����ڻ�������  
		            printf("\r\n��CAN2���յ�������: ");		 //����д�������
				for(i=0;i<8;i++)
                  {
		            printf("%2X ",CAN1_Rec[i]);		 //����д�������
	              }
		DataToStr(CAN1_Rec,str,8);   
		Gui_StrCenter(0,230,WHITE,RED,"                            ",16,0);//������ʾ
		Gui_StrCenter(0,230,WHITE,RED,str,16,0);//������ʾ
		}
		if (CAN2_Rec_Flag)
		{
		CAN2_Rec_Flag=0;
		LED1(0);LED2(1); ; // ���յ�CAN1�����ݣ���CAN2�ж��Ѿ����յ�CAN2_Rec�У����ڻ�������  
		            printf("\r\n��CAN1���յ�������: ");		 //����д�������
				for(i=0;i<8;i++)
                  {
		            printf("%2X ",CAN2_Rec[i]);		 //����д�������
	              }
		DataToStr(CAN2_Rec,str,8);   
		Gui_StrCenter(0,270,WHITE,RED,"                            ",16,0);//������ʾ
		Gui_StrCenter(0,270,WHITE,RED,str,16,0);//������ʾ
		}
		}

	PDout(7)=~PDout(7);	   //LED4(PD7)����ʾ
   Delay();
   }
}


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  * @note   #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  */
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART2, (uint8_t) ch);   
	
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
	{}
	
	return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
