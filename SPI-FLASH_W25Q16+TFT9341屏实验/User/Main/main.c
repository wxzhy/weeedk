/********************************************************
*  采用NUAA_CM3_107  WEEEDK实验开发板
*  SPI-FLASH_W25Q16读写实验例程
**	  实验内容：SPI接口应用－FLASH 16Mbits W25Q16
      JP30/JP31/JP32短接到到SPI1上,JP8短接片选
   本示例测试SPI-FLASH_W25Q16 FLASH存储器读写，硬件连接如下:
   JP5短接1-2,JP6短接1-2使PB8和PB9作为I2C的SCL和SDA（需要重新映射引脚）
   实验步骤：
（1）需要短接JP30短接到SCK（PA5），JP31短接到SO（PA6），JP32短接到SI,JP8短接（PB9为片选）
（2）插上3.2吋TFT真彩屏到JLCD1（可选），打开电源，两个电源指示灯指示灯亮
（2）下载示例程序后运行，显示屏显示初始信息
（3）按下KEY1键，通过中断服务程序置为Flag=1,写8个字节数据到Fash的指定地址并显示
（4）按下KEY2键，通过中断服务程序置为Flag=2,读Flash的指定地址中的8个字节的数据并显示
	编译下载程序到开发板并在线仿真运行，或下载后脱机复位运行
（5）可自行更改要写入的地址StartAdrr和Tx1_Buffer中的数据,当改写数据时如果不擦除则写不进去，写入的与读出的就不正确
     此时可以按KEY3键擦除0号扇区。借助于擦除指定扇区的操作，再写入指定扇区对应地址的数据方才能正确。

说明：
本例还可以用PB3、PB5、PB5作为SPI（重新映射），
可以让学生把JP30、JP31和JP32拔下，用杜邦线将PB3连接到JP30的SCK,将PB4连接到JP31的SO，将PB5连接到JP32的SI，重新修改重新定义SPI引脚
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
   "+------ SPI Flash W25Q16读写测试程序----------------+\r\n"
   "|                                                   |\r\n"
   "|    这个程序实现了对W25Q16芯片的读写测试。         |\r\n"
   "|    写256个字节的数据，看读出的结果是否正确        |\r\n"
   "|   读写。本程序在MDK-ARM下，ST库V3.50下测试通过。  |\r\n"
   "|...................................................|\r\n"
   "|    使用举例：                                     |\r\n"
   "|     按下KEY1，从0x0地址处写入数据0x12,0x34,0x56,..|\r\n"
   "|     按下KEY2, 从0x0地址处读出数据0x12,0x34,0x56,..|\r\n"
   "|     按下KEY3, 擦除0号扇区512的数据（读出均为FF）  |\r\n"
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
	for(i=1000;i>0;i--)
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

   USART2_SendString("\r\n UART2输出，SPI Flash操作实验\r\n ");
	printf(menu);   

  BUTTON_Configuration();		//按键
  	 
  NVIC_Configuration();		    //NVIC

  EXTI_Configuration();			//EXTI初始化
 

	Welcome(); 		//显示主界面

	if(!W25Q16_Init())				             //判断初始化是否成功
	{
		printf("\r\n W25Q16 Init OK! \r\n");
		Gui_StrCenter(0,270,BLACK,WHITE,"      W25Q16初始化正常        ",16,0);//居中显示
	}								   
	else
	{
		printf("\r\n W25Q16 Init Failure! \r\n");
		Gui_StrCenter(0,270,BLACK,WHITE,"      W25Q16初始化失败        ",16,0);//居中显示
	}
	LED1(1);LED2(1);LED3(1);LED4(1);

	while(1)
	{		
		switch(Flag)
		{
			case 0x01:		        //KEY1按下(中断置标志)
			{
			    Flag=0;			    //清除标志位
				DataToStr(Tx1_Buffer,str,8);   
					SPI_Flash_Write_Page(Tx1_Buffer, StartAdrr, 8);	// 从地址StartAdrr写256Byte		
					Gui_StrCenter(0,150,WHITE,RED,str,16,0);//居中显示
	               printf("\r\n读出Flash中的数据: ");		//打印读取的数据
				for(i=0;i<8;i++)
                  {
		            printf("%2X ",Tx1_Buffer[i]);		 //打印写入的数据
	              }
				 break;				
			}
			case 0x02:			     //KEY2按下
			{
			   Flag=0;				 //清除标志位
   				 SPI_Flash_Read(Rx1_Buffer, StartAdrr, 8);          //从地址StartAdrr读8Byte
				DataToStr(Rx1_Buffer,str,8);   
				Gui_StrCenter(0,190,WHITE,RED,str,16,0);//居中显示
	               printf("\r\n读出Flash中的数据: ");		//打印读取的数据
			   for(i=0;i<8;i++)
	             {	
	               printf("%2X ",Rx1_Buffer[i]);		//打印读取的数据

	             }
				break;				
			}
			case 0x03:			     //KEY3按下，擦除0号扇区
			{
			   Flag=0;				 //清除标志位
   				SPI_Flash_Erase_Sector(0);          //从擦除0号扇区0~4095单元的全部内容
				DataToStr(Rx1_Buffer,str,8);   
				Gui_StrCenter(0,190,WHITE,RED,str,16,0);//居中显示
	               printf("\r\n读出Flash中的数据: ");		//打印读取的数据
			   for(i=0;i<8;i++)
	             {	
	               printf("%2X ",Rx1_Buffer[i]);		//打印读取的数据

	             }
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
