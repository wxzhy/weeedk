/********************************************************
*  NUAA_CM3_107  NH-SQ1005Ƕ��ʽʵ�鿪������Գ���
2018.9.27
ע��̽��������
��1��P2�ϵ�PB12�̽ӵ�GND��Ϊ�������Ų���
��2��PA1����GND����ɲ���(1)4λ����LED  ��2��OCWJ4CX8C LCD  (3) HJ12864  (4) OLED  (5) TFT 320X240 LCD
��3��PA8����GND,ʱ����˸��������״̬������ʱ��
��4��PA11����GND,����PWM������Ƶ�������3.3V���ݣ�����GPIO���Ƶ��

 ͨ������ң�غ����������ư���ģ���Բ������������

  PB13����·������GND�̽��ˣ��е��������ӵ�LED5�Ķ̽�����
 Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "hw_config.h"
#include "stm32f10x.h"
#include "remote.h"
#include "HJ12864-COG-1.h"
#include "lcd.h"
#include "rtc.h"
#include "WEEEDK.h"
#include "delay.h"
#include "DS18B20.h"
#include <stdio.h>
#define Y1  0.00				/*0��*/

#define Ym1  100.00			/*100��*/
#define N1  2502       //2483.0		/*0�ȶ�Ӧ������*/
#define Nm1  3437//3420.0		/*100��Ӧ������*/
#define Y2  0.00				/*0mm*/
#define Ym2  1000.00			/*1000mm*/
#define N2  688//681      //792.0		/*0�ȶ�Ӧ������*/
#define Nm2  	3513//3485 //3969.0		/*100��Ӧ������*/
extern u8 gImage_fg[],State,Shan;
u8 key;
u8 TimeDisplay = 0;		


/* DAC��ַ0x4000 7400 - 0x4000 77FF��
   0x40007420��ַ����DAC_DHR12RD
   ˫DAC��12λ�Ҷ������ݱ��ּĴ���
*/
uint32_t DAC_DHR12R2_Address = 0x40007414;	  //0x4000 7400 - 0x4000 77FFΪDAC��ַ��Χ������0x40007414ΪDAC2�Ҷ������ݱ��ּĴ���
//uint32_t DAC_DHR12RD_Address = 0x40007420;	  //0x4000 7400 - 0x4000 77FFΪDAC��ַ��Χ������0x40007420Ϊ˫ͨ��DAC�Ҷ������ݱ��ּĴ���

const uint16_t Sine12bit[32] = {   /*32���Ҳ���*/
                      2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
                      3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909, 
                      599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};

uint32_t DAC2Sine12bit[32];
const uint16_t frebit[24] = {   /*1,2,3,4,5,6,7,1Ƶ��*/
                     0,523,587,659,698,784,880,988,523*2,587*2,659*2,698*2,784*2,880*2,988*2,523*4,523*4,523/2,587/2,659/2,698/2,784/2,880/2,988/2};//1300, 1462, 1625, 1733, 1950, 2166, 2437, 2600,2600, 2437,2166, 1950, 1733,1625,1462,1300};

const uint16_t Redfrebit[ ] = {784*2,784*2,784*2,880*2,587*2,587*2,587*2,587*2,523*2,523*2,523*2,880,  587*2,587*2,587*2,587*2,
														 784*2,784*2,784*2,784*2,880*2,523*4,880*2,784*2,523*2,523*2,523*2,880,  587*2,587*2,587*2,587*2,
														 784*2,784*2,587*2,587*2,523*2,523*2,988,  880,  784,  784,  784*2,784*2,587*2,587*2,659*2,587*2,
														 523*2,523*2,523*2,880,  587*2,659*2,587*2,523*2,587*2,523*2,988,  880,  784,  784,  784,  784,  
														 784,  784,  784,784,
															};   /*1,2,3,4,5,6,7,1Ƶ��*/
//                              3     2    3      5     6     5     1'    6      5    3      5    5       5     5       6     6    (16)
const uint16_t  Jasmine[ ] = {659*2,587*2,659*2,784*2,880*2,784*2,523*4,880*2,784*2,659*2,784*2,784*2, 784*2,784*2, 880*2,880*2,
	//                   			   1'    1'    2'    3'	    2'    1'    6    1'     5    3      5     5      5      5       5     5 (16)
															523*4,523*4,587*4,659*4,587*4,523*4,880*2,523*4,784*2,659*2,784*2,784*2,  784*2,784*2, 784*2,784*2,														 
	//                   			   5     3      5    5	    5    5    6      6    1'   1'     2'    3'      6     6       5     5 (16)
															784*2,659*2,784*2,784*2,784*2,784*2,880*2,880*2,523*4,523*4,587*4,659*4,  880*2,880*2, 784*2,784*2,														 
	//                   			   5     5      2    2	    3    5    3      2    1      6.     1    1      1     1       1     1 (16)
															784*2,784*2,587*2,587*2,659*2,784*2,659*2,587*2,523*2,880,523*2,523*2,  523*2,523*2, 523*2,523*2, 														 
	//                   			   3     2      1    1	    2    2    2      3    5      5     6    1'      6     6       5     5 (16)
															659*2,587*2,523*2,523*2,587*2,587*2,587*2,659*2,784*2,784*2,880*2,523*4,  880*2,880*2, 784*2,784*2, 														 
	//                   			   5     5      2    2	    3    5    3      2    1      2     6.    6.'      6.   6.      1     1 (16)
															784*2,784*2,587*2,587*2,659*2,784*2,659*2,587*2,523*2,587*2,880, 880,   880,   880,   523*2, 523*2, 														 
//															 2     2      2    3	    1    2    1      6.    1     6.     5.    5.'      5.   5.      5.     5. (16)
															587*2,587*2,587*2,659*2,523*2,587*2,523*2,880,523*2,  880,   784, 784,       784,  784,  784, 784, 														 
															};   /*1,2,3,4,5,6,7,1Ƶ��*/
const uint16_t  dolaimi[ ] = {523/2,523,523*3,523*4,523*5,880*2,784*2,523*3,880*2,784*2,659*2,784*2,784*2, 784*2,784*2, 784*2,784*2,784*2,880*2,
	//                   			   1'    1'    2'    3'	    2'    1'    6    1'     5    3      5     5      5      5       5     5    5     5 (18)
															523*3,523*3,587*3,659*3,587*3,523*3,880*2,523*3,784*2,784*2,784*2,7840,  784*2,784*2, 784*2,784*2, 784*2,784*2, 587*2,587*2,587*2,587*2,
														 784*2,784*2,587*2,587*2,523*2,523*2,988,  880,  784,  784,  784*2,784*2,587*2,587*2,659*2,587*2,
														 523*2,523*2,523*2,880,  587*2,659*2,587*2,523*2,587*2,523*2,988,  880,  784,  784,  784,  784,  
														 784,  784,  784,784,
															};   /*1,2,3,4,5,6,7,1Ƶ��*/

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#define times 2     //����LED�������ʾʱ��
uint32_t segNum;			//�����Ҫ��ʾ������
uint8_t seg[] ={0x41,0xE7,0x52,0x62,0xE4,0x68,0x48,0xE3,0x40,0x60,};	   //���������
uint8_t LEDDataSeg1[] ={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xa1,0x86,0x8E,0x8c,0xC1,0x91,0x7f,0xbf,0xC7,0x89,0xce,0xcf,0xaf,0xf1,0x9c,0xc8,0xff,};	   //���������
//                       0   1     2    3   4    5     6    7    8    9    A   b     C   d    E    F     P    U   Y		.	 -  L    H    T    I     r    J   o    N   ����ʾ
//                       0   1     2    3   4    5     6    7    8    9    10  11    12  13   14   15    16   17  18   19    20	 21	 22	  23   24    25   26  27   28   29
uint8_t LEDDataSeg[5] ={0x99,0xC7,0x86,0xa1,0xC7};	   //���������
//                     4     L   E    d     H   
/**									 _
	*���ƣ�void Delay(void)
	*��������
	*���أ���
	*���ܣ�����ʱ
**/

void Delayus(uint32_t k)
{
	int i,j;
	for(i=k;i>0;i--)
		for(j=1000;j>0;j--);

}
void Delay(void)
{
	int i,j;
	for(i=1000;i>0;i--)
		for(j=1000;j>0;j--);

}
extern u8  INDEX;
char str1[8];
uint8_t strd[8],strhw[8],SoundEnable;
extern u8 		USART_RX_BUF[10];		
uint8_t BeepFlag=0,ConnectBitPt100=0,ConnectBitWater=0,ConnectBitCSB=0;
uint8_t res,LimitBit=0,ReadFlag;
	u16 valued,Vdata1,Vdata2,Vdata3,Vdata5,Vdata,VdataBCD;   				/*����������������*/			
	float ADC_Value=0.00,ADC_ValueV=0.00,ADC_ValuePt100=0.00,ADC_ValueWater=0.00;       /*ʵ��������ֵ*/                  
uint8_t LEDCODE;
int main(void)
 {
//	 RTC_DateTimeTypeDef RTC_DateTimeStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	float temp;
	char DisNum[16];
uint8_t KEY=0;//,iRTC=0;
uint8_t Idx,i;
//uint16_t f=523;
//uint8_t str[40];
	SystemInit();
	Delay_Init();               //ʹ��SysTickʵ�־�ȷ��ʱ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_11|GPIO_Pin_12);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  	GPIO_Configuration();
	 LED5(1);
	USART123_Configuration();
BIT1(1);BIT2(1);BIT3(1);BIT4(1);
	PCout(4)=1;	//�̵���������	
	PCout(15)=1;//�̵���������		
	  

  USART2_SendString("\r\n �Ϻ�Ƕ��ʽϵͳ�ۺ�ʵ�鿪��ƽ̨��ʾϵͳ��ӭ����ʹ�ã�\r\n");

  USART2_SendString("\r\n ������ʾ��������ʾ��ʽչʾ��LED/LCD/OLED�������öŰ�������PA1��GND������ֱ�Ӳ���TFT����\r\n");
  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	      /*RTC�ж�ʹ��*/

if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))//�̽�PA1��GND�ɲ��Ը��ַ�TFT����Ĭ��LED��KEY1��OCWJ4X8C,KEY2:
{   
  USART2_SendString("\r\n ������ʾ��ʽչʾ��\r\n");
  USART2_SendString("\r\n ��1��4λLED�����������ʾ��KEY1������OCWJ4X8C LCD��\r\n");
	
	 while (1)
	{  //4λ LED���ԣ�
		
	if (!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11)) break;	//KEY1
	 for (i=0;i<=3;i++)
	 {
	 	switch(i)
		{
		case 3: 
			BIT1(0);BIT2(1);BIT3(1);BIT4(1);GPIO_Write(GPIOE,((GPIO_ReadOutputData(GPIOE)&0xFF00)|LEDDataSeg[i]));break;
		case 2: 
			BIT1(1);BIT2(0);BIT3(1);BIT4(1);GPIO_Write(GPIOE,((GPIO_ReadOutputData(GPIOE)&0xFF00)|LEDDataSeg[i]));break;
		case 1: 
			BIT1(1);BIT2(1);BIT3(0);BIT4(1);GPIO_Write(GPIOE,((GPIO_ReadOutputData(GPIOE)&0xFF00)|LEDDataSeg[i]));break;
		case 0: 
			BIT1(1);BIT2(1);BIT3(1);BIT4(0);GPIO_Write(GPIOE,((GPIO_ReadOutputData(GPIOE)&0xFF00)|LEDDataSeg[i]));break;
		}
		Delayus(times);
	 	
	 }
		Delayus(times);
 }
	LCD_Configuration();    //OCWJ4X8CҺ����
	LCD_Init_OCWJ4X8C();			
	displayInformation();
  USART2_SendString("\r\n ��2��ͼ�ε���LCDģ��OCWJ4X8CҺ������ʾ��KEY2��HJ12864-COG-1����ͼ�ε���LCD��\r\n");
 while(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12));//KEY2:HJ12864
	
	LCD_HJ12864Configuration();
	LCD_HJ12864_Init();			/* HJ12864 LCD��ʼ�� */
	clear_screenHJ();
  displayInformationHJ();//��ʼ��Ϣ��ʾ
   USART2_SendString("\r\n ��3��HJ12864-COG-1����ͼ�ε���LCD��ʾ��KEY3��1.3��128X64 OLED��\r\n");
while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13));	//KEY3:OLED	
		OLED_Init();		
		OLED_Clear()  	; 
		//for (i=0;i<8;i++) OLED_ShowCHinese(i*16,0,i+8);	//��ӭʹ�ñ�������8
			OLED_ShowString(0,0,"NH-SQ1005 CM3Kit");  
		for (i=0;i<8;i++) OLED_ShowCHinese(i*16,02,i+8);	//��ӭʹ�ñ�������
			OLED_ShowString(0,4,"NUAA_CS MWH OLED");  
		for (i=0;i<8;i++) OLED_ShowCHinese(i*16,6,i);	//�Ͼ����պ����ѧ  
   USART2_SendString("\r\n ��4��1.3��128X64 OLED����ʾ��KEY4��3.2�� 320X240 ���LCD��\r\n");
 while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));	//KEY4:TFT	
}	

  LCD_Init();					//���LCD��ʼ��
	GPIO_Configuration();

	UART4_Configuration();

USART2_SendString("\r\n ��ʼ����չʾ����ͼƬ�����ֵ���ʾ���õ�RTCʱ�䡢��ʱ����DAC�Լ�GPIO�������\r\n");
USART2_SendString("\r\n ����ʱ��ʱ��PA8��GND�̽ӣ�KEY3ѡ����˸λ��KEY1,KEY2�Ӽ�ʱ�䣬KEY4��ȷ����\r\n");

USART2_SendString("\r\n �ڳ�ʼ�����У�����DAC�������Ҳ�ͨ�����ʷŴ��ƶ����ȷ�������������������Ҳ�ɲ��������ת��\r\n");
USART2_SendString("\r\n ��KEY1������������������\r\n");
USART2_SendString("\r\n ��KEY2���ð���ֱ�������ת��\r\n");
USART2_SendString("\r\n ��KEY3���ð���ֱ�����ֹͣ��ת��ֹͣ�������֡�\r\n");
USART2_SendString("\r\n ��KEY4����ʼ�ۺ�ʵ�����ʾ��������һ�����档\r\n");

	Gui_Drawbmp16(0,0,gImage_WEEEDK);
	Gui_StrCenter(1,16,YELLOW,WHITE,"�Ϻ� NH-SQ1005 KIT",16,1);//������ʾ
	Gui_StrCenter(0,15,RED,WHITE,"�Ϻ� NH-SQ1005 KIT",16,1);//������ʾ
	if (USE_HORIZONTAL==1)	Show_Str(45,40,WHITE,BLACK,"Ƕ��ʽϵͳ�ۺ�ʵ��",24,1);
	else					Show_Str(10,40,WHITE,BLACK,"Ƕ��ʽϵͳ�ۺ�ʵ��",24,1);
	if (USE_HORIZONTAL==1)	Show_Str(47,42,BLUE,BLACK,"Ƕ��ʽϵͳ�ۺ�ʵ��",24,1);
	else					Show_Str(12,42,BLUE,BLACK,"Ƕ��ʽϵͳ�ۺ�ʵ��",24,1);

if (USE_HORIZONTAL==1)	Show_Str(28,98,RED,BLACK,"��ӭ��",64,1);
	else					Show_Str(28,98,RED,BLACK,"��ӭ��",64,1);
if (USE_HORIZONTAL==1)	Show_Str(25,95,YELLOW,BLACK,"��ӭ��",64,1);
	else					Show_Str(25,95,YELLOW,BLACK,"��ӭ��",64,1);
//Gui_StrCenter(0,46,RED,BLUE,"Ƕ��ʽϵͳԭ��Ӧ������ʵ���",16,1);//������ʾ
Gui_StrCenter(0,230,WHITE,BLACK, " KEY1:Music  KEY3:Stop!  ",16,0);//������ʾ	
Gui_StrCenter(0,250,WHITE,BLACK, " KEY2:Moving KEY4:Start! ",16,0);//������ʾ	
	Gui_StrCenter(1,270,WHITE,BLACK,"    ������4��KEY4��ʼ    ",16,0);//������ʾ	
	//Gui_StrCenter(0,294,YELLOW,BLACK,"�Ϻ�NH-SQ�ۺ�ʵ��ƽ̨��ʾϵͳ",16,0);//������ʾ	
	Gui_StrCenter(2,296,WHITE,BLACK,"�Ϻ�NH-SQ�ۺ�ʵ��ƽ̨��ʾϵͳ",16,0);//������ʾ	
	for (Idx = 0; Idx < 32; Idx++)
	{
		DAC2Sine12bit[Idx] = (Sine12bit[Idx] << 16) + (Sine12bit[Idx]);	  //˫����
	}
//	LED1(0);LED2(0);LED3(0);LED4(0);
	POINT_COLOR=WHITE;BACK_COLOR=BLACK;
	REMOTE_Init();
	NVIC_Configuration();		
	EXTI_Configuration();		
	DMA_Configuration();	   //DMA��ʼ��
	TIM_Configuration(frebit[3]);	   //TIM3��ʼ��
	DAC_Configuration();	   //DAC2��ʼ��
	Delay_ms(300);
	TIM_Configuration(frebit[1]);	   //TIM3��ʼ��
	Delay_ms(300);
	  	TIM_Cmd(TIM3, DISABLE);	//û�����������ʹ�ܶ�ʱ��������DAC�����
if((GPIOB->IDR&(1<<12))==0)//PB12Ϊ�Ƿ������ж�����
	SoundEnable=1;
else SoundEnable=0;
 	if (SoundEnable)
	{SetVolume(25);//ѡ���������30
	 Delay_ms(50);
	 SendSoundInf(20);//
  }
	USART1_SendString("\r\n USART1(RS-232C)��KEY4����ʼ�ۺ�ʵ��! \r\n");
  //USART2_SendString("\r\n USART2(USBת����)��KEY4����ʼ�ۺ�ʵ��! \r\n");
	// ADC1_Mode_Config ();	

//ADC1 ��ʼ������
//for (i=0;i<11;i++) 		 {SendSoundInf(i);	Delay_ms(500);}//Ϊ
			Delay_ms(350);
KEY=0;//iRTC=0;
	Shan=0;State=1;
	RTC_Init(1);
	
while(1)
	 {
		 
	if (SetTimeFlag==0)   //PA8�̽�GND
	{
	KEY=KEY_Scan(0);
	if(KEY!=0)    					 
	{
		switch (KEY)
		{
			case 1:IncTime();/*����ʱ��*/break;
			case 2:DecTime();/*����ʱ��*/break;
			case 3:SelectTime();/*ѡ��ʱ��*/break;			
			case 4:SetTime(); /*����ʱ�䣬д��RTC*/break;			
		}	
		KEY=0;
	}		
		Delay_ms(200);        /*����ʱ��˸ʱ��*/                    
		RTC_ITConfig(RTC_IT_SEC, DISABLE);	      /*��ֹRTC�ж�,��������*/
		GPIOD->ODR&=~(1<<2);/*��PD2��0��LED1����*/
		Shan++;
		DisplayInfSet();		/*ʱ����Ϣ����*/

	switch(State)  /*��������ʱ��״̬��˸��ͬλ��*/
		{
		case 1:if (Shan&1)/* LCD_ShowNum(28,180,RTC_DateTimeStructure.Year+2000,4,16);else */Show_Str(28,180,YELLOW,RED,"    ",16,0);break;
		case 2:if (Shan&1) /*LCD_ShowNum(20+16*6-8,180,RTC_DateTimeStructure.Month,2,16);else	*/Show_Str(20+16*6-8,180,YELLOW,RED,"  ",16,0);break;
		case 3:if (Shan&1) /*LCD_ShowNum(20+16*10-8,180,RTC_DateTimeStructure.Date,2,16);else */Show_Str(20+16*10-8,180,YELLOW,RED,"  ",16,0);break;
		case 4:if (Shan&1) /*LCD_ShowNum(28+16*3,200,RTC_DateTimeStructure.Hour,2,16);else */Show_Str(28+16*3,200,YELLOW,RED,"  ",16,0);break;
		case 5:if (Shan&1) /*LCD_ShowNum(28+16*6,200,RTC_DateTimeStructure.Minute,2,16);else */Show_Str(28+16*6,200,YELLOW,RED,"  ",16,0);break;
		case 6:if (Shan&1) /*LCD_ShowNum(28+16*9,200,RTC_DateTimeStructure.Second,2,16);else */Show_Str(28+16*9,200,YELLOW,RED,"  ",16,0);break;
		}	
	}
	else
	{	
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	      /*RTC�ж�ʹ��*/
    if (TimeDisplay == 1)										/*RTC���жϷ����������λ,��ʾʱ�䵽*/
    {
		 DisplayInf();													/*��ʾʱ������*/
     TimeDisplay = 0;												/*��ʱ�䵽��־�����ⲻ��ʱ��Ƶ����LCD������ʾ����ʵ��ʱ��û�б仯����ʾû������*/
    }
	KEY=KEY_Scan(0);
	}
		
loopmain:
			TIM_Cmd(TIM3, DISABLE);	//û�����������ʹ�ܶ�ʱ��������DAC���		
if(KEY!=0)	
{	if(KEY==1||KEY==2||KEY==3) {BEEP(1);Delay_ms(200);BEEP(0);}
  if(KEY==4) {BEEP(1);Delay_ms(200);BEEP(0);break;}
	switch (KEY)
		{		
		case 1:
			
	USART2_SendString("\r\n ����KEY1��������������������\r\n");
			
		for (i=0;i<112;i++)
			{
		KEY=KEY_Scan(0);
		if(KEY==2||KEY==3||KEY==4) {
		BEEP(1);Delay_ms(200);BEEP(0);goto loopmain;}
		TIM_Configuration(Jasmine[i]);	   //TIM2��ֵ�ı�Ƶ��
	Delay_ms(200);
		
			}	
			KEY=0;
			TIM_Cmd(TIM3, DISABLE);	//û�����������ʹ�ܶ�ʱ��������DAC�����			
			break;
		case 2:
	USART2_SendString("\r\n ����KEY2������ֹͣ���õ��ת����\r\n");
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);	
	  TIM_Cmd(TIM3, DISABLE);	//û�����������ʹ�ܶ�ʱ��������DAC�����
		KEY=0;
		break;
		case 3:
	USART2_SendString("\r\n ����KEY3������ֹͣ���õ��ֹͣת����\r\n");
		{
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);	
	  TIM_Cmd(TIM3, DISABLE);	//û�����������ʹ�ܶ�ʱ��������DAC�����
			KEY=0;
		}
		break;
		case 4:break;
	}
	}	
}
	  USART2_SendString("\r\n ����KEY4����ʼ�����ۺ�ʵ����ʾ��\r\n");
TIM_Cmd(TIM3, DISABLE);	//û�����������ʹ�ܶ�ʱ��������DAC�����
		 ADC1_Mode_Config ();		                            //ADC1 ��ʼ������
Ultran_Init();
	Welcome(); 		//��ʾ������
			GPIOC->ODR|=(1<<15);//�̵����Ͽ�
			GPIOC->ODR|=(1<<4);			
	if(SoundEnable)	
	{SendSoundInf(28);//��ʼʵ��
	//			Delay_ms(1400);
  			Delay_ms(400);
  }
	USART2_SendString("\r\n ���ۺ�ʵ����ʾ���棬չʾͨ��ADC����¶ȡ�ˮλ���ȣ�ͨ����������������������ȡ�\r\n");
	USART2_SendString("\r\n չʾPWM��������λ����λ��أ����Ƶ������ݣ�������PA1��GND,��ת��λ������ʾ�仯���۲����ٶȻ�������ȱ仯��\r\n");
	USART2_SendString("\r\n ����ͨ������������ң�������������ҲҪ����ͨ���ֻ��������������豸����ͨ������������������\r\n");
	USART2_SendString("\r\n �����������������GPIO��������������KEY1��ת��KEY2��ת��KEY3ֹͣ��\r\n");
	USART2_SendString("\r\n ͨ����������ң�������������ͨ����ʱ����GPIO�жϼ������������б��룩���Ϸ�����ת���·�����ת����Դ��ֹͣ��\r\n");
	USART2_SendString("\r\n ͨ���ֻ����������豸��ͨ��USART3��ȡ����ģ�鷢���������������豸�����ֻ�������������������12���������ɽ�����ز�����\r\n");
Init_PWM(10);
	USART2_SendString("\r\n =====================================\r\n");
	USART2_SendString("\r\n ע�⣺PB12�̽ӵ�GND��Ϊ�������Ų�����\r\n");
	USART2_SendString("\r\n =====================================\r\n");
	
while(1)
	{	
	Ultra_Ranging(&temp);
		Vdata5=temp*1000;
		if ((ConnectBitCSB==0)||(Vdata5>8000)) Show_Str(150,240,YELLOW,RED," δ��",16,0);
		else{				sprintf(DisNum,"%d",Vdata5);
			for (i=0;i<8;i++) strd[i]=DisNum[i];
		LCD_ShowNum(150,240,Vdata5,5,16);				/*��LCD������ʾ��ѹֵmV*/
		//LCD_ShowString(150,240,16,strd,0);LCD_ShowString(150+32,240,16," ",0);*/
			}
	//	LCD_ShowNum(150,240, valued,5,16);						/*��LCD������ʾ����ֵ*/
			
 		valued=ReadADCAverageValue(0);								/*��ȡ��λ��ADC_IN3,NO.1���Ϊ0����Ӧ����ֵ*/
		ADC_ValueV= (3.3*1000/4096)*valued;			    /*��ADֵת��Ϊ��ѹֵmV*/
		LCD_ShowNum(150,140,ADC_ValueV,5,16);				/*��LCD������ʾ��ѹֵmV*/
			if (!SelectPWMOut) Init_PWM(valued);//PB8=PWM OUT
			else 		GPIO_Configuration();
	
	Delay_ms(50);

		valued=ReadADCAverageValue(1);								/*��ȡPT100�����Ӧ�¶�ֵ��ADC_IN6,NO.2���Ϊ1*/
		if(valued>=0xFF0) 			{ConnectBitPt100=0;Show_Str(150,165,YELLOW,RED," δ��",16,0);}
		else{ConnectBitPt100=1;
		ADC_ValuePt100=(float)(Y1+(Ym1-Y1)*(valued-N1)/(Nm1-N1)-1.1);	  /*ͨ�����Ա�ȱ任�õ��¶�0~100��*/
			if (ADC_ValuePt100>120) {ConnectBitPt100=0;Show_Str(150,165,YELLOW,RED," δ��",16,0);}
		else{	
			ConnectBitPt100=1;
			LCD_ShowNum(150, 165, (u8)ADC_ValuePt100, 3,16);    /*��ʾ�¶���������	*/
			Show_Str(150+8*3,165,YELLOW,RED,".",16,0);
			ADC_Value-=(u8)ADC_ValuePt100;	  								/*��ȥ�������֣�ʣ��С������*/	
			LCD_ShowNum(150+8*4,165,ADC_ValuePt100*10,1,16);   /*��ʾ�¶�С	������*/	
				}
			}
		Delay_ms(50);

		valued=ReadADCAverageValue(2);								/*��ȡˮλ��������ƽ��ֵ��ADC_IN7 No.3 ���Ϊ2 */			
		ADC_ValueWater=(float)(Y2+(Ym2-Y2)*(valued-N2)/(Nm2-N2));	  /*ͨ�����Ա�ȱ任�õ�ˮλ*/
		if (valued<(N2-200.0)) {ConnectBitWater=0;Show_Str(150,190,YELLOW,RED," δ��",16,0);}
		else	{ConnectBitWater=1;
		if(ADC_ValueWater<=0) ADC_ValueWater=0;;LCD_ShowNum(150,190, ADC_ValueWater,5,16);}						/*��LCD������ʾ����ֵ*/
    
		if ((ADC_ValueWater<200.0)&&(valued>N2-200.0)&&(SoundEnable==1)) 
					{SendSoundInf(23);Delay_ms(600);SendSoundInf(21);LimitBit=1;Delay_ms(1800);Delay_ms(1800);}//ˮλ����0.2�ף���������ˮ
		else {if ((ADC_ValueWater>=250.0)|(valued<N2-200)) 
				  {LimitBit=0;}}
		
		Delay_ms(50);
			valued=ReadADCAverageValue(3);								/*��ȡ�ڲ��¶�ƽ��ֵ��ADC_IN16,NO.4���Ϊ3		*/	 	
			ADC_Value=(1.43-(3.2/4096)*valued)/0.0043+25;   /*�������ǰ�¶�ֵ,�μ���Ƕ��ʽϵͳԭ��Ӧ�ý̲ģ������棩����7��7.4.1�������*/	 
			if(ADC_Value<0)												  			/*�������¶�*/
			{
			Show_Str(150,215,WHITE,BLACK,"-",16,0);
			}					 
			else 
			Show_Str(150,215,WHITE,BLACK," ",16,0);
 		
			LCD_ShowNum(150, 215, (u8)ADC_Value, 2,16);    /*��ʾ�¶���������	*/
			Show_Str(150+8*2,215,WHITE,RED,".",16,0);
			ADC_Value-=(u8)ADC_Value;	  
			LCD_ShowNum(150+8*3,215,ADC_Value*100,2,16);   /*��ʾ�¶�С������*/	
		
		Delay_ms(50);
		valued=ReadADCAverageValue(4);								/*��ȡ�ڲ���׼��ѹ����ƽ��ֵ��ADC_IN17,NO.5���Ϊ4 */	
		ADC_Value = (3.3*1000/4096)*valued;			    /*��ADֵת��Ϊ��ѹֵmV*/
  if(SelectPWMOut)//����PWM�����PA11û�ж̽�GND)
	{	
		KEY=KEY_Scan(0);
	if(KEY!=0)
	{
		BEEP(1);Delay_ms(200);BEEP(0);
   switch (KEY)
	 {
		 case 1:
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);	
		LEDR(0);LEDG(1);
  	USART2_SendString("\r\n KEY1�õ����ת��");
	if(SoundEnable==1)
	{
		SendSoundInf(29);//���
				Delay_ms(900);
		SendSoundInf(30);//��ת
			Delay_ms(900);
	}		 
		 KEY=0;
		 break;
		 case 2:
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_SetBits(GPIOB, GPIO_Pin_9);	
  	USART2_SendString("\r\n KEY2�õ����ת��");
		LEDR(1);LEDG(0);
	if(SoundEnable==1)
	{
		SendSoundInf(29);//���
				Delay_ms(900);
		SendSoundInf(31);//��ת
			Delay_ms(900);
	}		 
		 KEY=0;
		 break;
		 case 3:
		LEDR(0);LEDG(0);
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);	
  	USART2_SendString("\r\n KEY3�õ��ֹͣ��");
	if(SoundEnable==1)
	{
		SendSoundInf(29);//���
				Delay_ms(900);
		SendSoundInf(37);//ֹͣ
			Delay_ms(900);
	}		 
		 KEY=0;
		 break;
		 case 4:
		LEDR(0);LEDG(0);
		 KEY=0;
		 break;
	 } 
	}	
if (Remote_Rdy)
{
  Remote_Rdy=0;
  sprintf(str1,"%08X",Remote_Odr);

	for (i=0;i<8;i++) strhw[i]=str1[i];

	 if(Remote_Odr!=0)
      switch (Remote_Odr)
    {

	case 0x00FDA05F://���ü���
					key=1;
					IA(0);IB(0);
  	USART2_SendString("\r\n ���յĺ�������ʹ���Ϊ����Դ��=ֹͣ  ����Ϊ��");
	USART2_SendString(strhw);
  	USART1_SendString("\r\n USART1���յĺ�������ʹ���Ϊ����Դ��=ֹͣ  ����Ϊ��");
	USART1_SendString(strhw);
	if(SoundEnable==1)
	{
		SendSoundInf(29);//���
				Delay_ms(900);
		SendSoundInf(37);//ֹͣ
			Delay_ms(900);
	}					key=0;
					LEDR(0);LEDG(0);
		break;
	case 0x00FDB04F://�ϼ���
					IA(1);IB(0);
					LEDR(1);LEDG(0);  //��������
  	USART2_SendString("\r\n ���յĺ�������ʹ���Ϊ���Ϸ���=��ת  ����Ϊ��");
	USART2_SendString(strhw);
  	USART1_SendString("\r\n USART1���յĺ�������ʹ���Ϊ���Ϸ���=��ת  ����Ϊ��");
	USART1_SendString(strhw);
	if(SoundEnable==1)
	{
		SendSoundInf(29);//���
				Delay_ms(900);
		SendSoundInf(30);//��ת
			Delay_ms(900);
	}		 				key=2;
		break;
	case 0x00FD8877://�¼���
					IA(0);IB(1);
					LEDR(0);LEDG(1);   //��������
  	USART2_SendString("\r\n ���յĺ�������ʹ���Ϊ���·���=��ת  ����Ϊ��");
	USART2_SendString(strhw);
  	USART1_SendString("\r\n USART1���յĺ�������ʹ���Ϊ���·���=��ת  ����Ϊ��");
	USART1_SendString(strhw);
if(SoundEnable==1)
	{
		SendSoundInf(29);//���
				Delay_ms(900);
		SendSoundInf(31);//��ת
			Delay_ms(900);
	}		 					key=3;
		break;
	default:
					IA(1);IB(1);	//ͬʱ��			
  	USART2_SendString("\r\n ���յĺ�������ʹ���Ϊ��������=��Ч  ����Ϊ��");
	USART2_SendString(strhw);
  	USART1_SendString("\r\n USART1���յĺ�������ʹ���Ϊ��������=��Ч  ����Ϊ��");
	USART1_SendString(strhw);
					key=0;
		break;

	}
 /* Delay_ms(100);
	BEEP(1);
	Delay_ms(100);
	BEEP(0);*/

}
}
if (BeepFlag)	   //��������Ч������һ��������
	{

		USART2_SendString("\r\n ��������������Ϊ��");
		
		USART1_SendString("\r\n USART1��������������Ϊ��");
		UART3_Operation();
  
		
	Delay_ms(200);
	BEEP(1);
	Delay_ms(200);
	BEEP(0);
		if(ReadFlag!=0)
	{	
		
		Vdata1=ADC_ValueV;
		Vdata2=ADC_ValuePt100*10;
		Vdata3=ADC_ValueWater;
	switch (ReadFlag)
  {
		case 1:
			sprintf(str1, "%d", Vdata1);
			for (i=0;i<8;i++) strhw[i]=str1[i];		
			USART3_SendString("\r\n V=");USART3_SendString(strhw);USART3_SendString("mV");//USART3_SendString("\r\n");
			USART2_SendString("����λ����ѹ\r\n ��λ����λֵ=");USART2_SendString(strhw);USART2_SendString("����\r\n");//USART3_SendString("\r\n");
	if(SoundEnable==1)
	{
	SendSoundInf(22);//��λ����ѹ
				Delay_ms(18500);
		SendSoundInf(19);//Ϊ
			Delay_ms(400);
		Vdata=Vdata1;
		SoundFormNum(0);
		SendSoundInf(15);	//����	
			Delay_ms(800);	  
	}
		break;
		case 2:
			if(ConnectBitPt100==1)
			{
			sprintf(str1, "%3.1f", ADC_ValuePt100);
			for (i=0;i<8;i++) strhw[i]=str1[i];		
			USART3_SendString("\r\n T=");USART3_SendString(strhw);USART3_SendString("du");//USART3_SendString("\r\n");
			USART2_SendString("��ȡ�¶�\r\n PT100�¶Ȳ���ֵ=");USART2_SendString(strhw);USART2_SendString("��\r\n");//USART3_SendString("\r\n");
			sprintf(str1, "%d", Vdata2);
			for (i=0;i<8;i++) strhw[i]=str1[i];		
		if(SoundEnable==1)
	{
		SendSoundInf(16);//Pt100
				Delay_ms(1000);
		SendSoundInf(13);//�¶�
				Delay_ms(600);
		SendSoundInf(19);//Ϊ
			Delay_ms(300);
		Vdata=Vdata2;
		SoundFormNum(1);
	SendSoundInf(14);	//��
			Delay_ms(300);	  
	}
			}
			else
				{
			USART3_SendString("\r\n Pt100 sensor not connected");//USART3_SendString("\r\n");
			USART2_SendString("��ȡ�¶�\r\n Pt100������û�����ӣ�\r\n");//USART3_SendString("\r\n");
	if(SoundEnable==1)
	{
	SendSoundInf(16);//Pt100
				Delay_ms(1000);
		SendSoundInf(13);//�¶�
				Delay_ms(600);
		SendSoundInf(24);//������
				Delay_ms(900);
		SendSoundInf(25);//δ����
				Delay_ms(900);				
	}
	}
		break;
		case 3:			
			if(ConnectBitWater==1)
			{	
			sprintf(str1, "%d", Vdata3);
			for (i=0;i<8;i++) strhw[i]=str1[i];		
			USART3_SendString("\r\n H=");USART3_SendString(strhw);USART3_SendString("mm");//USART3_SendString("\r\n");
			USART2_SendString("��ȡˮλ�߶�\r\n ˮλ�߶�=");USART2_SendString(strhw);USART2_SendString("����\r\n");//USART3_SendString("\r\n");
	if(SoundEnable==1)
	{
		SendSoundInf(23);//ˮλ
				Delay_ms(600);
		SendSoundInf(17);//�߶�
				Delay_ms(600);
		SendSoundInf(19);//Ϊ
			Delay_ms(200);
		Vdata=Vdata3;
		SoundFormNum(0);
	SendSoundInf(12);	//����
			Delay_ms(600);	  
	}
	}
		else
				{
		USART3_SendString("\r\nwater level sensor not connected");//USART3_SendString("\r\n");
		USART2_SendString("��ȡˮλ�߶�\r\n ˮλ������û������! \r\n");//USART3_SendString("\r\n");
	if(SoundEnable==1)
	{
		SendSoundInf(23);//ˮλ
				Delay_ms(600);
		SendSoundInf(24);//������
				Delay_ms(900);
		SendSoundInf(25);//δ����
				Delay_ms(900);				
	}
	}
				break;
		case 5:			
			if(ConnectBitCSB==1)
			{	
			//
			for (i=0;i<8;i++) strhw[i]=strd[i];		
			USART3_SendString("\r\n L=");USART3_SendString(strhw);USART3_SendString("mm");//USART3_SendString("\r\n");
			USART2_SendString("��ȡ�����������ľ���\r\n ����=");USART2_SendString(strhw);USART2_SendString("����\r\n");//USART3_SendString("\r\n");
	if(SoundEnable==1)
	{
		SendSoundInf(27);//������
				Delay_ms(900);
		SendSoundInf(26);//����
				Delay_ms(500);
		SendSoundInf(19);//Ϊ
			Delay_ms(300);
		Vdata=Vdata5;
		SoundFormNum(0);//
		SendSoundInf(12);	//����
			Delay_ms(600);	  
	}
	}
		else
				{
		USART3_SendString("\r\nwater level sensor not connected");//USART3_SendString("\r\n");
		USART2_SendString("��ȡˮλ�߶�\r\n ˮλ������û������! \r\n");//USART3_SendString("\r\n");
	if(SoundEnable==1)
	{
		SendSoundInf(27);//������
				Delay_ms(700);
		SendSoundInf(24);//������
				Delay_ms(900);
		SendSoundInf(25);//δ����
				Delay_ms(900);				
	}
	}
				break;  
 		case 211:			
	if(SoundEnable==1)
	{
		SendSoundInf(36);//LED
				Delay_ms(900);
		SendSoundInf(1);//1
				Delay_ms(500);
		SendSoundInf(34);//��
			Delay_ms(300);
	}
		break;
 		case 201:			
	if(SoundEnable==1)
	{
		SendSoundInf(36);//LED
				Delay_ms(1000);
		SendSoundInf(1);//1
				Delay_ms(300);
		SendSoundInf(35);//��
			Delay_ms(300);
	}
		break;
 		case 212:			
	if(SoundEnable==1)
	{
		SendSoundInf(36);//LED
				Delay_ms(1000);
		SendSoundInf(2);//2
				Delay_ms(300);
		SendSoundInf(34);//��
			Delay_ms(300);
	}
		break;
 		case 202:			
	if(SoundEnable==1)
	{
		SendSoundInf(36);//LED
				Delay_ms(1000);
		SendSoundInf(2);//2
				Delay_ms(300);
		SendSoundInf(35);//��
			Delay_ms(300);
	}
		break;
 		case 213:			
	if(SoundEnable==1)
	{
		SendSoundInf(36);//LED
				Delay_ms(1000);
		SendSoundInf(3);//3
				Delay_ms(300);
		SendSoundInf(34);//��
			Delay_ms(300);
	}
		break;
 		case 203:			
	if(SoundEnable==1)
	{
		SendSoundInf(36);//LED
				Delay_ms(1000);
		SendSoundInf(3);//3
				Delay_ms(300);
		SendSoundInf(35);//��
			Delay_ms(300);
	}
		break;
 		case 214:			
	if(SoundEnable==1)
	{
		SendSoundInf(36);//LED
				Delay_ms(1000);
		SendSoundInf(4);//4
				Delay_ms(300);
		SendSoundInf(34);//��
			Delay_ms(300);
	}
		break;
 		case 204:			
	if(SoundEnable==1)
	{
		SendSoundInf(36);//LED
				Delay_ms(1000);
		SendSoundInf(4);//4
				Delay_ms(300);
		SendSoundInf(35);//��
			Delay_ms(300);
	}
		break;
 		case 215:			
	if(SoundEnable==1)
	{
		SendSoundInf(32);//���
				Delay_ms(900);
		SendSoundInf(34);//��
			Delay_ms(300);
	}
		break;
 		case 205:			
	if(SoundEnable==1)
	{
		SendSoundInf(32);//���
				Delay_ms(900);
		SendSoundInf(35);//��
			Delay_ms(300);
	}
				break;  
 		case 216:			
	if(SoundEnable==1)
	{
		SendSoundInf(33);//�׵�
				Delay_ms(900);
		SendSoundInf(34);//��
			Delay_ms(300);
	}
		break;
 		case 206:			
	if(SoundEnable==1)
	{
		SendSoundInf(33);//�׵�
				Delay_ms(900);
		SendSoundInf(35);//��
			Delay_ms(300);
	}
				break;  
 		case 217:			
	if((SoundEnable==1)&&(SelectPWMOut==1))
	{
		SendSoundInf(29);//���
				Delay_ms(900);
		SendSoundInf(30);//��ת
			Delay_ms(900);
	}
		break;
 		case 227:			
	if((SoundEnable==1)&&(SelectPWMOut==1))
	{
		SendSoundInf(29);//���
				Delay_ms(900);
		SendSoundInf(31);//��ת
			Delay_ms(900);
	}
				break;  
 		case 207:			
	if((SoundEnable==1)&&(SelectPWMOut==1))
	{
		SendSoundInf(29);//���
				Delay_ms(900);
		SendSoundInf(37);//ֹͣ
			Delay_ms(900);
	}
				break;  
	
	}		
	ReadFlag=0;
	}
BeepFlag=0;INDEX=0;
	}
  	Delay_10ms(10);
   }
}

/********************************************************
* ��С����λ�����ֺ�С���㣬���������ų���
*��ڣ���1��������Vdata��
*			��2��Type=0��ʾû��С����Type=1��ʾ��С��Ϊһλ���������˵���������198��ʵ����Ϊ19.8
****************************************************/
void SoundFormNum(uint8_t Type)
{
if(Type==0)
{	
if(Vdata<10)
		{
		SendSoundInf(strhw[0]&0x0f);
			Delay_ms(400);
		}
else if(Vdata<100)
		{
		SendSoundInf(strhw[0]&0x0f);
			Delay_ms(400);
		if ((strhw[1]&0x0f)!=0) SendSoundInf(strhw[1]&0x0f);
			else SendSoundInf(10);
			Delay_ms(400);
		}
else if (Vdata<1000)
		{
		SendSoundInf(strhw[0]&0x0f);
			Delay_ms(400);
		if ((strhw[1]&0x0f)!=0) SendSoundInf(strhw[1]&0x0f);
			else SendSoundInf(10);
			Delay_ms(400);
		if ((strhw[2]&0x0f)!=0) SendSoundInf(strhw[2]&0x0f);
			else SendSoundInf(10);
			Delay_ms(400);
		}	
else
	{
		SendSoundInf(strhw[0]&0x0f);
			Delay_ms(400);
		if ((strhw[1]&0x0f)!=0) SendSoundInf(strhw[1]&0x0f);
			else SendSoundInf(10);		
			Delay_ms(400);
		if ((strhw[2]&0x0f)!=0) SendSoundInf(strhw[2]&0x0f);
			else SendSoundInf(10);		
			Delay_ms(400);
		if ((strhw[3]&0x0f)!=0) 
						SendSoundInf(strhw[3]&0x0f);
			else 
						SendSoundInf(10);		
			Delay_ms(400);
	}
}
else if(Type==1)
{
if(Vdata<10)
		{
		SendSoundInf(10);//0
			Delay_ms(400);
		SendSoundInf(11);//.
			Delay_ms(400);
		SendSoundInf(strhw[0]&0x0f);
			Delay_ms(400);
		}
else if(Vdata<100)
		{
		SendSoundInf(strhw[0]&0x0f);
			Delay_ms(400);
		SendSoundInf(11);//.
			Delay_ms(400);
		if ((strhw[1]&0x0f)!=0) SendSoundInf(strhw[1]&0x0f);
			else SendSoundInf(10);
			Delay_ms(400);
		}
else if (Vdata<1000)
		{
		SendSoundInf(strhw[0]&0x0f);
			Delay_ms(400);
		if ((strhw[1]&0x0f)!=0) SendSoundInf(strhw[1]&0x0f);
			else SendSoundInf(10);
			Delay_ms(400);
		SendSoundInf(11);//.
			Delay_ms(400);			
		if ((strhw[2]&0x0f)!=0) SendSoundInf(strhw[2]&0x0f);
			else SendSoundInf(10);
			Delay_ms(400);
		}	
else
	{
		SendSoundInf(strhw[0]&0x0f);
			Delay_ms(400);
		if ((strhw[1]&0x0f)!=0) SendSoundInf(strhw[1]&0x0f);
			else SendSoundInf(10);		
			Delay_ms(400);
		if ((strhw[2]&0x0f)!=0) SendSoundInf(strhw[2]&0x0f);
			else SendSoundInf(10);		
			Delay_ms(400);
		SendSoundInf(11);//.
			Delay_ms(400);
		if ((strhw[3]&0x0f)!=0) 
				SendSoundInf(strhw[3]&0x0f);
			else 
				SendSoundInf(10);		
			Delay_ms(400);
	}	
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
	USART_SendData(USART1, (uint8_t) ch);   
	
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
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
