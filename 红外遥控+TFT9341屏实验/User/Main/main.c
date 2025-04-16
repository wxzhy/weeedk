/********************************************************
*  ����NUAA_CM3_107  WEEEDKʵ�鿪����
*  ������Ʋ�������
	һ���̽���Ӧ�źź͵�Դ
	��1�� �̽�JP41
	��2���̽�JP4��JP5��JP6��2��3,��ֱ��������ӵ�GPIO����
 	�����������س��򵽿����岢���߷������У������غ��ѻ���λ����
	������������ɫ����ң������׼������������ͷ������Ӧ�����������LCD������ʾ��Ӧ������롣

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "remote.h"

#include "lcd.h"
#include "delay.h"
#include "DS18B20.h"
#include <stdio.h>

extern u8 gImage_fg[];
u8 key;


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

uint8_t BeepFlag=0;
char res;

int main(void)
{
char str[8];
uint8_t strd[8],i=0;
	SystemInit();
	Delay_Init();               //ʹ��SysTickʵ�־�ȷ��ʱ

	LCD_Init();					//LCD��ʼ��

	USART_Configuration();
	Welcome(); 		//��ʾ������
	GPIO_Configuration();
	REMOTE_Init();
	NVIC_Configuration();		
	EXTI_Configuration();		


	Delay_Init();               //ʹ��SysTickʵ�־�ȷ��ʱ

	LCD_Init();					//LCD��ʼ��

	USART_Configuration();
	Welcome(); 		//��ʾ������
	LED1(1);LED2(1);LED3(1);LED4(1);
	POINT_COLOR=WHITE;BACK_COLOR=RED;
	while(1)
	{		if (Remote_Rdy)
{
  sprintf(str,"%08X",Remote_Odr);
	for (i=0;i<8;i++) strd[i]=str[i];
	LCD_ShowString(86, 160, 16,strd,0);

      switch (Remote_Odr)
    {
	case 0x00FDA05F://���ü���
	Gui_StrCenter(0,190,WHITE,BLACK," ��Դ��=���ֹͣ ",16,0);//������ʾ
					//LCD_ShowString(150,252, " ��Դ�� ", RED, YELLOW);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);		//���ֹͣ		
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);				
	key=1;
		break;
	case 0x00FDB04F://�ϼ���
	Gui_StrCenter(0,190,WHITE,BLACK," �Ϸ���=�����ת ",16,0);//������ʾ
				//	LCD_ShowString(150,252, " �Ϸ��� ", BLUE, YELLOW);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);			//�����ת	
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);				
	key=2;
		break;
	case 0x00FD8877://�¼���
	Gui_StrCenter(0,190,WHITE,BLACK," �·���=�����ת ",16,0);//������ʾ
				//	LCD_ShowString(150,252, " �·��� ", BLACK, YELLOW);
	GPIO_SetBits(GPIOB,GPIO_Pin_9);			//�����ת	
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);				
	key=3;
		break;
	default:
	Gui_StrCenter(0,190,WHITE,BLACK," ������=��Ч���� ",16,0);//������ʾ
				//	LCD_ShowString(150,252, " ������ ", BLACK, YELLOW);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);				
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);				
	key=0;
		break;

	}

}

  // 	Delay_ms(100);
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
	USART_SendData(USARTx, (uint8_t) ch);   
	
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
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
