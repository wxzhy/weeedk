/********************************************************
*  ����NUAA_CM3_107  WEEEDKʵ�鿪����
*  I2C EEPROM AT24C02��дʵ������
   ��ʾ������I2C EEPROM�洢��AT24C02��д��Ӳ����������:
   JP5�̽�1-2,JP6�̽�1-2ʹPB8��PB9��ΪI2C��SCL��SDA����Ҫ����ӳ�����ţ�
   ʵ�鲽�裺
��1����Ҫ�̽�JP5��JP6��1-2,�Ͽ�JP8��JP29��ʹPB8��PB9����ΪI2C��SCL��SDA
��2������3.2��TFT�������JLCD1����ѡ�����򿪵�Դ��������Դָʾ��ָʾ����
��2������ʾ����������У���ʾ����ʾ��ʼ��Ϣ
��3������KEY1����ͨ���жϷ��������ΪFlag=1,д8���ֽ����ݵ�EEPROM��ָ����ַ����ʾ
��4������KEY2����ͨ���жϷ��������ΪFlag=2,д8���ֽ����ݵ�EEPROM��ָ����ַ����ʾ
��5������KEY3����ͨ���жϷ��������ΪFlag=3,��EEPROM��ָ����ַ�е�8���ֽڵ����ݲ���ʾ
	�������س��򵽿����岢���߷������У������غ��ѻ���λ����
��6�������и���Ҫд��ĵ�ַStartAdrr��Tx1_Buffer�е�����

˵����
��������������ӳ��PB8��PB9ΪI2C���ŵģ��������PB6��PB7����������ӳ�䣬
������ѧ����JP5��JP6���£��öŰ��߽�PB6���ӵ�JP5-1,��PB7���ӵ�JP6-1�������޸����¶���I2C����
**************************************************************************************************/	

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "lcd.h"
#include "i2c_ee.h"

#include "delay.h"
#include "DS18B20.h"

#define U1Baud   115200
#define U2Baud   115200

extern u8 gImage_fg[];
u8 key;
uint8_t BeepFlag=0;
//uint8_t Tx1_Buffer[]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
uint8_t Tx1_Buffer[]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
uint8_t Tx2_Buffer[]={0x12,0x34,0x56,0x78,0x90,0xAB,0xCD,0xEF};
uint8_t Rx_Buffer[8];
uint8_t Flag = 0x00;
uint8_t StartAdrr=0x10;
uint64_t sum;

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
   "+------ NUAA_CM3_107ʵ�鿪���� AT24C02��д���Գ���-------------------------+\r\n"
   "|                                                                          |\r\n"
   "|    �������ʵ���˶�AT24C02оƬ�Ķ�д���ԡ�24C02�� 256                    |\r\n"
   "|   �ֽڵĴ洢�ռ䣬ҳ��СΪ8�ֽڣ����԰��ֽڻ�ҳ����                      |\r\n"
   "|   ��д����������MDK-ARM�£�ST��V3.50�²���ͨ����                         |\r\n"
   "|                                                                          |\r\n"
   "|    ʹ�þ�����                                                            |\r\n"
   "|     ����KEY1����0x10��ַ��д������0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88|\r\n"
   "|     ����KEY2����0x10��ַ��д������0x12,0x34,0x56,0x78,0x90,0xAB,0xCD,0xEF|\r\n"
   "|     ����KEY3, ��0x10��ַ����������                                       |\r\n"
   "|                                                                          |\r\n"
   "+--------------------------------------------------------------------------+\r\n";
    
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
	for(i=5000;i>0;i--)
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

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��GPIOAʱ��

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
		  else		   temp=temp+0x07+0x30;
		  str[2*i]=temp;
		  if ((buf[i]&(0x0f))<=9) str[2*i+1]=(buf[i]&(0x0f))+0x30;
		  else str[2*i+1]=(buf[i]&(0x0f))+0x07+0x30;
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

   USART2_SendString("\r\n UART2���I2C����ʵ��\r\n ");
	printf(menu);
  BUTTON_Configuration();		//����
  	 
  NVIC_Configuration();		    //NVIC

  EXTI_Configuration();			//EXTI��ʼ��
 
  I2C_EE_Init(); 				//��ʼ��I2C EEPROM

	Welcome(); 		//��ʾ������
	LED1(1);LED2(1);LED3(1);LED4(1);
	while(1)
	{		
		switch(Flag)
		{
			case 0x01:		        //KEY1����(�ж��ñ�־)
			{
			    Flag=0;			    //�����־λ
				DataToStr(Tx1_Buffer,str,8);   
          I2C_EE_BufferWrite(Tx1_Buffer,StartAdrr,8);		 //��StartAdrrд������
					Gui_StrCenter(0,150,WHITE,RED,"                        ",16,0);//������ʾ
					Gui_StrCenter(0,150,WHITE,RED,str,16,0);//����  ��ʾ
					printf("\r\nд�������: ");		 //��ӡд�������
				for(i=0;i<8;i++)  printf("%2X ",Tx1_Buffer[i]);		 //��ӡд�������
				 break;				
			}
			case 0x02:			     //KEY2����
			{
			   Flag=0;				 //�����־λ
				DataToStr(Tx2_Buffer,str,8);   
				  I2C_EE_BufferWrite(Tx2_Buffer,StartAdrr,8);		 //��StartAdrrд������
					Gui_StrCenter(0,150,WHITE,RED,"                        ",16,0);//������ʾ
					Gui_StrCenter(0,150,WHITE,RED,str,16,0);//������ʾ
					printf("\r\nд�������: ");		 //��ӡд�������
				for(i=0;i<8;i++)  printf("%2X ",Tx2_Buffer[i]);		 //��ӡд�������
				break;				
			}
			case 0x03:			     //KEY3����
			{
			   Flag=0;				 //�����־λ
			   Flag=0;				 //�����־λ
         I2C_EE_BufferRead(Rx_Buffer,StartAdrr,8);		//��StartAdrr��ȡ����
				DataToStr(Rx_Buffer,str,8);   
				Gui_StrCenter(0,190,WHITE,RED,str,16,0);//������ʾ
	               printf("\r\n����������: ");		//��ӡ��ȡ������
			   for(i=0;i<8;i++)
	               printf("%2X ",Rx_Buffer[i]);		//��ӡ��ȡ������
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
