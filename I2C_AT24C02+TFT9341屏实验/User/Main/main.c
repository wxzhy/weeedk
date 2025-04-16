/********************************************************
*  采用NUAA_CM3_107  WEEEDK实验开发板
*  I2C EEPROM AT24C02读写实验例程
   本示例测试I2C EEPROM存储器AT24C02读写，硬件连接如下:
   JP5短接1-2,JP6短接1-2使PB8和PB9作为I2C的SCL和SDA（需要重新映射引脚）
   实验步骤：
（1）需要短接JP5和JP6的1-2,断开JP8，JP29，使PB8和PB9仅作为I2C的SCL和SDA
（2）插上3.2吋TFT真彩屏到JLCD1（可选），打开电源，两个电源指示灯指示灯亮
（2）下载示例程序后运行，显示屏显示初始信息
（3）按下KEY1键，通过中断服务程序置为Flag=1,写8个字节数据到EEPROM的指定地址并显示
（4）按下KEY2键，通过中断服务程序置为Flag=2,写8个字节数据到EEPROM的指定地址并显示
（5）按下KEY3键，通过中断服务程序置为Flag=3,读EEPROM的指定地址中的8个字节的数据并显示
	编译下载程序到开发板并在线仿真运行，或下载后脱机复位运行
（6）可自行更改要写入的地址StartAdrr和Tx1_Buffer中的数据

说明：
本例是利用重新映射PB8和PB9为I2C引脚的，如果采用PB6和PB7则无需重新映射，
可以让学生把JP5和JP6拔下，用杜邦线将PB6连接到JP5-1,将PB7连接到JP6-1，重新修改重新定义I2C引脚
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
   "+------ NUAA_CM3_107实验开发板 AT24C02读写测试程序-------------------------+\r\n"
   "|                                                                          |\r\n"
   "|    这个程序实现了对AT24C02芯片的读写测试。24C02共 256                    |\r\n"
   "|   字节的存储空间，页大小为8字节，可以按字节或页进行                      |\r\n"
   "|   读写。本程序在MDK-ARM下，ST库V3.50下测试通过。                         |\r\n"
   "|                                                                          |\r\n"
   "|    使用举例：                                                            |\r\n"
   "|     按下KEY1，从0x10地址处写入数据0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88|\r\n"
   "|     按下KEY2，从0x10地址处写入数据0x12,0x34,0x56,0x78,0x90,0xAB,0xCD,0xEF|\r\n"
   "|     按下KEY3, 从0x10地址处读出数据                                       |\r\n"
   "|                                                                          |\r\n"
   "+--------------------------------------------------------------------------+\r\n";
    
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
	for(i=5000;i>0;i--)
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

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能GPIOA时钟

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
  SystemInit();	                                         //设置系统时钟
  RCC_Configuration();			//系统时钟初始化
  LED_Configuration();
	Delay_Init();               //使用SysTick实现精确延时
	LCD_Init();					//LCD初始化
	USART_Configuration(U1Baud,U2Baud);

   USART2_SendString("\r\n UART2输出I2C操作实验\r\n ");
	printf(menu);
  BUTTON_Configuration();		//按键
  	 
  NVIC_Configuration();		    //NVIC

  EXTI_Configuration();			//EXTI初始化
 
  I2C_EE_Init(); 				//初始化I2C EEPROM

	Welcome(); 		//显示主界面
	LED1(1);LED2(1);LED3(1);LED4(1);
	while(1)
	{		
		switch(Flag)
		{
			case 0x01:		        //KEY1按下(中断置标志)
			{
			    Flag=0;			    //清除标志位
				DataToStr(Tx1_Buffer,str,8);   
          I2C_EE_BufferWrite(Tx1_Buffer,StartAdrr,8);		 //向StartAdrr写入数据
					Gui_StrCenter(0,150,WHITE,RED,"                        ",16,0);//居中显示
					Gui_StrCenter(0,150,WHITE,RED,str,16,0);//居中  显示
					printf("\r\n写入的数据: ");		 //打印写入的数据
				for(i=0;i<8;i++)  printf("%2X ",Tx1_Buffer[i]);		 //打印写入的数据
				 break;				
			}
			case 0x02:			     //KEY2按下
			{
			   Flag=0;				 //清除标志位
				DataToStr(Tx2_Buffer,str,8);   
				  I2C_EE_BufferWrite(Tx2_Buffer,StartAdrr,8);		 //向StartAdrr写入数据
					Gui_StrCenter(0,150,WHITE,RED,"                        ",16,0);//居中显示
					Gui_StrCenter(0,150,WHITE,RED,str,16,0);//居中显示
					printf("\r\n写入的数据: ");		 //打印写入的数据
				for(i=0;i<8;i++)  printf("%2X ",Tx2_Buffer[i]);		 //打印写入的数据
				break;				
			}
			case 0x03:			     //KEY3按下
			{
			   Flag=0;				 //清除标志位
			   Flag=0;				 //清除标志位
         I2C_EE_BufferRead(Rx_Buffer,StartAdrr,8);		//从StartAdrr读取数据
				DataToStr(Rx_Buffer,str,8);   
				Gui_StrCenter(0,190,WHITE,RED,str,16,0);//居中显示
	               printf("\r\n读出的数据: ");		//打印读取的数据
			   for(i=0;i<8;i++)
	               printf("%2X ",Rx_Buffer[i]);		//打印读取的数据
				break;				
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
