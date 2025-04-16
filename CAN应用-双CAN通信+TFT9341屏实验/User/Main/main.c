/********************************************************
*  采用NUAA_CM3_107  WEEEDK实验开发板
**	CAN总线通信实验：利用STM32F107特有的两个CAN总线接口，直接相互通信
   实验步骤：
（1）需要短接JP10短接到C2TX（PB6），JP11短接到C2RX（PB5）,用导线将CAN1与CAN2l连接：CANH1连接CAH2，CANL1连接CANL2
（2）插上3.2吋TFT真彩屏到JLCD1（可选），打开电源，两个电源指示灯指示灯亮
（2）下载示例程序后运行，显示屏显示初始信息
（3）按下KEY1键，通过中断服务程序置为KeyFlag=1,从CAN1发送8个字节数据到CAN2并显示
（4）按下KEY2键，通过中断服务程序置为KeyFlag=2,从CAN2发送8个字节数据到CAN1并显示
（3）按下KEY3键，清除LCD屏显示的数据
	编译下载程序到开发板并在线仿真运行，或下载后脱机复位运行
（5）可自行更改要要发送的数据CAN1_DATA和CAN2_DATA是的值，查看收发情况
需要调整的短接器：断开JP6、JP33，连接JP30、JP31和JP32到上方使PA5、PA6和PA7连接SPI总线的SCL、SO和SI
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
   "+---CAN（CAN1与CAN2之间直接通信）总线通信测试程序---+\r\n"
   "|                                                   |\r\n"
   "|    这个程序实现了CAN1与CAN2直接通信。             |\r\n"
   "|    从CAN1发送8个字节的有效数据，CAN2中断接收      |\r\n"
   "|    从CAN2发送8个字节的有效数据，CAN1中断接收      |\r\n"
   "|...................................................|\r\n"
   "|    使用举例：                                     |\r\n"
   "|     按下KEY1，从CAN1发送数据0xC1~0xEF             |\r\n"
   "|     按下KEY2，从CAN2发送数据0xC2~0xF0             |\r\n"
   "+---------------------------------------------------+\r\n";
    
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
	for(i=2000;i>0;i--)
		for(j=1000;j>0;j--);

}

/**************************************************
*文件名称：void RCC_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：系统时钟配置
***************************************************/
void RCC_Configuration(void)
{
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能GPIOB时钟

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	 //使能重新映射端口时钟，因为后面要对I2C重映射到PB8，PB9而不是默认的PB7PB7
 											  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);	 //使能I2C1时钟

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能GPIOA时钟

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //使能USART1时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
}
/**************************************************
*文件名称：void DataToStr(uint8_t *buf,uint8_t *str,uint8_t length)
*
*入口参数：待转换的十六进制数的数组在*buf中，转换的数据个数（字节数）为length
*
*出口参数：转换为字符串在*str中
*
*功能说明：一个数组中多个字节数据转换在一个完整的ASCII码字符串，便于显示
*******************************************************/
void DataToStr(uint8_t *buf,uint8_t *str,uint8_t length)
{
uint8_t i=0,temp;
	for (i=0;i<length;i++)	//将数据转换为ASCC码，以便于显示
	  	 { temp=(buf[i]&0xf0)>>4;//取高4位
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
	SystemInit();	                                         //设置系统时钟
	GPIO_Configuration();
  	LED_Configuration();
	Delay_Init();               //使用SysTick实现精确延时
	LCD_Init();					//LCD初始化
	USART_Configuration(U1Baud,U2Baud);

   USART2_SendString("\r\n UART2输出，CAN通信实验\r\n ");
	printf(menu);   

  BUTTON_Configuration();		//按键
  	 
  NVIC_Configuration();		    //NVIC

  EXTI_Configuration();			//EXTI初始化

  	CAN_Configuration();		//CAN总线初始化


	Welcome(); 		//显示主界面

	KeyFlag=0;
	LED1(1);LED2(1);LED3(1);LED4(1);

	while(1)
	{		
		if (KeyFlag)
		{
		switch(KeyFlag)
		{
			case 0x01:		        //KEY1按下，CAN2接收的数据显示在屏屏幕上
			{
			    KeyFlag=0;			    //清除标志位
				DataToStr(CAN1_DATA,str,8);   
                CAN1_SEND(CAN1ID); //ID标识CAN1发送的是0xaa
		Gui_StrCenter(0,150,RED,GREEN,"                            ",16,0);//居中显示
		Gui_StrCenter(0,150,RED,GREEN,str,16,0);//居中显示
				 break;				
			}
			case 0x02:			     //KEY2按下由CAN2发送数据，CAN1接收的数据显示在屏屏幕上
			{
			   KeyFlag=0;				 //清除标志位
				DataToStr(CAN2_DATA,str,8);   
                CAN2_SEND(CAN2ID); //ID标识CAN2发送的是0xbb
		Gui_StrCenter(0,190,RED,GREEN,"                            ",16,0);//居中显示
		Gui_StrCenter(0,190,RED,GREEN,str,16,0);//居中显示
				break;				
			}
			case 0x03:		        //KEY3按下(中断置标志)清除CAN1和CAN·接收显示的数据
			{
			    KeyFlag=0;			    //清除标志位
		Gui_StrCenter(0,150,RED,GREEN,"                            ",16,0);//居中显示
		Gui_StrCenter(0,190,RED,GREEN,"                            ",16,0);//居中显示
		Gui_StrCenter(0,230,WHITE,RED,"                            ",16,0);//居中显示
		Gui_StrCenter(0,270,WHITE,RED,"                            ",16,0);//居中显示
				 break;				
			}
		
		}
		if (CAN1_Rec_Flag)
		{
		CAN1_Rec_Flag=0;
		LED2(0);LED1(1); ; // 接收到CAN2的数据，在CAN1中断已经接收到CAN1_Rec中，串口回显数据  
		            printf("\r\n从CAN2接收到的数据: ");		 //发送写入的数据
				for(i=0;i<8;i++)
                  {
		            printf("%2X ",CAN1_Rec[i]);		 //发送写入的数据
	              }
		DataToStr(CAN1_Rec,str,8);   
		Gui_StrCenter(0,230,WHITE,RED,"                            ",16,0);//居中显示
		Gui_StrCenter(0,230,WHITE,RED,str,16,0);//居中显示
		}
		if (CAN2_Rec_Flag)
		{
		CAN2_Rec_Flag=0;
		LED1(0);LED2(1); ; // 接收到CAN1的数据，在CAN2中断已经接收到CAN2_Rec中，串口回显数据  
		            printf("\r\n从CAN1接收到的数据: ");		 //发送写入的数据
				for(i=0;i<8;i++)
                  {
		            printf("%2X ",CAN2_Rec[i]);		 //发送写入的数据
	              }
		DataToStr(CAN2_Rec,str,8);   
		Gui_StrCenter(0,270,WHITE,RED,"                            ",16,0);//居中显示
		Gui_StrCenter(0,270,WHITE,RED,str,16,0);//居中显示
		}
		}

	PDout(7)=~PDout(7);	   //LED4(PD7)闪显示
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
