#ifndef __HW_CONFIG_H_
#define __HW_CONFIG_H_

#include "stm32f10x_conf.h"

#define SetTimeFlag  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)//PA8接GND，则为设置时钟


#define SelectPWMOut  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)//PA11连接GND为PWM  PB8输出
#define KEY1  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)//读取按键1
#define KEY1  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)//读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)//读取按键2
#define KEY3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)  //读取按键3
#define KEY4  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)  //读取按键4

//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08

#define PAout(n)	*((volatile unsigned long *)(0x42000000+((GPIOA_ODR_Addr-0x40000000)<<5)+(n<<2)))
#define PBout(n)	*((volatile unsigned long *)(0x42000000+((GPIOB_ODR_Addr-0x40000000)<<5)+(n<<2)))
#define PCout(n)	*((volatile unsigned long *)(0x42000000+((GPIOC_ODR_Addr-0x40000000)<<5)+(n<<2)))
#define PDout(n)	*((volatile unsigned long *)(0x42000000+((GPIOD_ODR_Addr-0x40000000)<<5)+(n<<2)))
#define PEout(n)	*((volatile unsigned long *)(0x42000000+((GPIOE_ODR_Addr-0x40000000)<<5)+(n<<2))) 
 

//IO口操作,只对单一的IO口!
//确保n的值小于16!
//#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
//#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

//#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

//#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

//#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

//#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入
 
   
/*------------------------LED---------------------------*/
#define RCC_APB2Periph_LED	RCC_APB2Periph_GPIOD
#define LED_PORT			GPIOD
#define LED1_PIN			GPIO_Pin_2	  //PD2
#define LED2_PIN			GPIO_Pin_3	  //PD3
#define LED3_PIN			GPIO_Pin_4	  //PD4
#define LED4_PIN			GPIO_Pin_7	  //PD7
#define LED5_PIN			GPIO_Pin_13   //PB13


#define LEDR(x)   ((x) ? (GPIO_SetBits(GPIOB, GPIO_Pin_1)) : (GPIO_ResetBits(GPIOB, GPIO_Pin_1)));
#define LEDG(x)   ((x) ? (GPIO_SetBits(GPIOB, GPIO_Pin_2)) : (GPIO_ResetBits(GPIOB, GPIO_Pin_2)));

#define LED1(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED1_PIN)) : (GPIO_ResetBits(LED_PORT, LED1_PIN)));
#define LED2(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED2_PIN)) : (GPIO_ResetBits(LED_PORT, LED2_PIN)));
#define LED3(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED3_PIN)) : (GPIO_ResetBits(LED_PORT, LED3_PIN)));
#define LED4(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED4_PIN)) : (GPIO_ResetBits(LED_PORT, LED4_PIN)));
#define LED5(x)   ((x) ? (GPIO_SetBits(GPIOB, LED5_PIN)) : (GPIO_ResetBits(GPIOB, LED5_PIN)));
#define Motor_PORT		GPIOB
#define IA_PIN			GPIO_Pin_8    //PB8
#define IB_PIN			GPIO_Pin_9    //PB9
#define IA(x)   ((x) ? (GPIO_SetBits(Motor_PORT, IA_PIN)) : (GPIO_ResetBits(Motor_PORT, IA_PIN)))
#define IB(x)   ((x) ? (GPIO_SetBits(Motor_PORT, IB_PIN)) : (GPIO_ResetBits(Motor_PORT, IB_PIN)))
#define BEEP(x)   ((x) ? (GPIO_SetBits(GPIOC, BEEP_PIN)) : (GPIO_ResetBits(GPIOC, BEEP_PIN)));
#define BEEP_PIN			GPIO_Pin_0	/* PC0 */

/*----------------------------USART----------------------------*/
/*使用串口之前先打开相应时钟
 * USART		TX		RX
   USART1       PA9     PA10
   USART2		PA2		PA3*/
#define USART_PORT		GPIOA
#define USARTx			USART1
#define USART_TX		GPIO_Pin_9
#define USART_RX		GPIO_Pin_10

//#define TRIG_Send(x)   ((x) ? (GPIO_SetBits(GPIOA, GPIO_Pin_2)) : (GPIO_ResetBits({GPIOA, GPIO_Pin_2)))//PAout(2)
//#define ECHO_Reci(x)   ((x) ? (GPIO_SetBits(GPIOC, GPIO_Pin_1)) : (GPIO_ResetBits(GPIOC, GPIO_Pin_1)))//PCin(1)

/*------------------------八段LED数码管---------------------------*/
#define BIT1_PIN			GPIO_Pin_11	  //PC11
#define BIT2_PIN			GPIO_Pin_12	  //PC12
#define BIT3_PIN			GPIO_Pin_13	  //PD13
#define BIT4_PIN			GPIO_Pin_14	  //PD14

#define BIT1(x)   ((x) ? (GPIO_SetBits(GPIOC, BIT1_PIN)) : (GPIO_ResetBits(GPIOC, BIT1_PIN)))
#define BIT2(x)   ((x) ? (GPIO_SetBits(GPIOC, BIT2_PIN)) : (GPIO_ResetBits(GPIOC, BIT2_PIN)))
#define BIT3(x)   ((x) ? (GPIO_SetBits(GPIOD, BIT3_PIN)) : (GPIO_ResetBits(GPIOD, BIT3_PIN)))
#define BIT4(x)   ((x) ? (GPIO_SetBits(GPIOD, BIT4_PIN)) : (GPIO_ResetBits(GPIOD, BIT4_PIN)))

//---------------------------------------------------------------------------------------------------------------------------------------------
void Ultran_Init(void);
void Ultra_Ranging(float *p);



/* 函数声明 */
void GPIO_Configuration(void);
void USART123_Configuration(void);
void UART4_Configuration(void);
void NVIC_Configuration(void);
void EXTI_Configuration(void);
void Delay_Init(void);
void Delay_ms(u16 nms);
void Delay_us(u32 nus);
void ADC1_Mode_Config(void);
void REMOTE_Init(void);

void USART1_SendString(uint8_t *ch);
void USART2_SendString(uint8_t *ch);
void USART3_SendString(uint8_t *ch);
void Delay_10ms(uint32_t nCount);
void DAC_Configuration(void);
void TIM_Configuration(uint16_t f);
void DMA_Configuration(void);

void MotorStop(void);
void MotorCCW(void);
void MotorCW(void);
int ReadADCAverageValue(uint16_t Channel);
void SetVolume(uint8_t Num);   //0B 01 1E 2A   设置音量为最大30(1E)
void SendSoundInf(uint8_t Num);   
void SoundFormNum(uint8_t Type);
void UART3_Operation(void);	
void  LCD_Init_OCWJ4X8C(void);     
void displayInformation(void);		//显示信息（设置状态除外）
void LCD_Configuration(void);
void OLED_Configuration(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowString(u8 x,u8 y,u8 *chr);
void OLED_ShowCHineseMotor(u8 x,u8 y,u8 no);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void RTC_Init(u8 select);
void LCD_ShowNumSelMode(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);
void DisplayInf(void);
u8 KEY_Scan(u8 mode);
void IncTime(void);
void DecTime(void);
void Time_Regulate(void);
void SelectTime(void);
void SetTime(void);
void DisplayInfSet(void);
void RTC_Init0(void);
void Init_PWM(uint16_t Dutyfactor);
void PWM_GPIO_Init(void);


#endif
