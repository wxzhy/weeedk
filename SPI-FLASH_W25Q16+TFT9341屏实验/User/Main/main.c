/********************************************************
*  ����NUAA_CM3_107  WEEEDKʵ�鿪����
*  SPI-FLASH_W25Q16��дʵ������
**	  ʵ�����ݣ�SPI�ӿ�Ӧ�ã�FLASH 16Mbits W25Q16
      JP30/JP31/JP32�̽ӵ���SPI1��,JP8�̽�Ƭѡ
   ��ʾ������SPI-FLASH_W25Q16 FLASH�洢����д��Ӳ����������:
   JP5�̽�1-2,JP6�̽�1-2ʹPB8��PB9��ΪI2C��SCL��SDA����Ҫ����ӳ�����ţ�
   ʵ�鲽�裺
��1����Ҫ�̽�JP30�̽ӵ�SCK��PA5����JP31�̽ӵ�SO��PA6����JP32�̽ӵ�SI,JP8�̽ӣ�PB9ΪƬѡ��
��2������3.2��TFT�������JLCD1����ѡ�����򿪵�Դ��������Դָʾ��ָʾ����
��2������ʾ����������У���ʾ����ʾ��ʼ��Ϣ
��3������KEY1����ͨ���жϷ��������ΪFlag=1,д8���ֽ����ݵ�Fash��ָ����ַ����ʾ
��4������KEY2����ͨ���жϷ��������ΪFlag=2,��Flash��ָ����ַ�е�8���ֽڵ����ݲ���ʾ
	�������س��򵽿����岢���߷������У������غ��ѻ���λ����
��5�������и���Ҫд��ĵ�ַStartAdrr��Tx1_Buffer�е�����,����д����ʱ�����������д����ȥ��д���������ľͲ���ȷ
     ��ʱ���԰�KEY3������0�������������ڲ���ָ�������Ĳ�������д��ָ��������Ӧ��ַ�����ݷ�������ȷ��

˵����
������������PB3��PB5��PB5��ΪSPI������ӳ�䣩��
������ѧ����JP30��JP31��JP32���£��öŰ��߽�PB3���ӵ�JP30��SCK,��PB4���ӵ�JP31��SO����PB5���ӵ�JP32��SI�������޸����¶���SPI����
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"
#include "w25q16.h"

#include "delay.h"
#include "DS18B20.h"

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
uint8_t Flag = 0x00;
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
   "+------ SPI Flash W25Q16��д���Գ���----------------+\r\n"
   "|                                                   |\r\n"
   "|    �������ʵ���˶�W25Q16оƬ�Ķ�д���ԡ�         |\r\n"
   "|    д256���ֽڵ����ݣ��������Ľ���Ƿ���ȷ        |\r\n"
   "|   ��д����������MDK-ARM�£�ST��V3.50�²���ͨ����  |\r\n"
   "|...................................................|\r\n"
   "|    ʹ�þ�����                                     |\r\n"
   "|     ����KEY1����0x0��ַ��д������0x12,0x34,0x56,..|\r\n"
   "|     ����KEY2, ��0x0��ַ����������0x12,0x34,0x56,..|\r\n"
   "|     ����KEY3, ����0������512�����ݣ�������ΪFF��  |\r\n"
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
	for(i=1000;i>0;i--)
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

  
int main(void)
{
uint8_t str[40];
uint8_t i=0;
  SystemInit();	                                         //����ϵͳʱ��
  RCC_Configuration();			//ϵͳʱ�ӳ�ʼ��
  LED_Configuration();
	Delay_Init();               //ʹ��SysTickʵ�־�ȷ��ʱ
	LCD_Init();					//LCD��ʼ��
	USART_Configuration(U1Baud,U2Baud);

   USART2_SendString("\r\n UART2�����SPI Flash����ʵ��\r\n ");
	printf(menu);   

  BUTTON_Configuration();		//����
  	 
  NVIC_Configuration();		    //NVIC

  EXTI_Configuration();			//EXTI��ʼ��
 

	Welcome(); 		//��ʾ������

	if(!W25Q16_Init())				             //�жϳ�ʼ���Ƿ�ɹ�
	{
		printf("\r\n W25Q16 Init OK! \r\n");
		Gui_StrCenter(0,270,BLACK,WHITE,"      W25Q16��ʼ������        ",16,0);//������ʾ
	}								   
	else
	{
		printf("\r\n W25Q16 Init Failure! \r\n");
		Gui_StrCenter(0,270,BLACK,WHITE,"      W25Q16��ʼ��ʧ��        ",16,0);//������ʾ
	}
	LED1(1);LED2(1);LED3(1);LED4(1);

	while(1)
	{		
		switch(Flag)
		{
			case 0x01:		        //KEY1����(�ж��ñ�־)
			{
			    Flag=0;			    //�����־λ
				DataToStr(Tx1_Buffer,str,8);   
					SPI_Flash_Write_Page(Tx1_Buffer, StartAdrr, 8);	// �ӵ�ַStartAdrrд256Byte		
					Gui_StrCenter(0,150,WHITE,RED,str,16,0);//������ʾ
	               printf("\r\n����Flash�е�����: ");		//��ӡ��ȡ������
				for(i=0;i<8;i++)
                  {
		            printf("%2X ",Tx1_Buffer[i]);		 //��ӡд�������
	              }
				 break;				
			}
			case 0x02:			     //KEY2����
			{
			   Flag=0;				 //�����־λ
   				 SPI_Flash_Read(Rx1_Buffer, StartAdrr, 8);          //�ӵ�ַStartAdrr��8Byte
				DataToStr(Rx1_Buffer,str,8);   
				Gui_StrCenter(0,190,WHITE,RED,str,16,0);//������ʾ
	               printf("\r\n����Flash�е�����: ");		//��ӡ��ȡ������
			   for(i=0;i<8;i++)
	             {	
	               printf("%2X ",Rx1_Buffer[i]);		//��ӡ��ȡ������

	             }
				break;				
			}
			case 0x03:			     //KEY3���£�����0������
			{
			   Flag=0;				 //�����־λ
   				SPI_Flash_Erase_Sector(0);          //�Ӳ���0������0~4095��Ԫ��ȫ������
				DataToStr(Rx1_Buffer,str,8);   
				Gui_StrCenter(0,190,WHITE,RED,str,16,0);//������ʾ
	               printf("\r\n����Flash�е�����: ");		//��ӡ��ȡ������
			   for(i=0;i<8;i++)
	             {	
	               printf("%2X ",Rx1_Buffer[i]);		//��ӡ��ȡ������

	             }
				break;				
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
