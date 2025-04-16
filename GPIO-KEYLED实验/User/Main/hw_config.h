#ifndef __HW_CONFIG_H_
#define __HW_CONFIG_H_
#include "stm32f10x_conf.h"
/**
   *下面这部分是端口位带操作,参考Cortex-M3权威指南
   */ 
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
#define PFout(n)	*((volatile unsigned long *)(0x42000000+((GPIOF_ODR_Addr-0x40000000)<<5)+(n<<2)))

/*------------------------LED---------------------------*/
#define RCC_APB2Periph_LED	RCC_APB2Periph_GPIOD
#define LED_PORT			GPIOD
#define LED1_PIN			GPIO_Pin_2 /* PD2 */
#define LED2_PIN			GPIO_Pin_3 /* PD3 */
#define LED3_PIN			GPIO_Pin_4 /* PD4 */
#define LED4_PIN			GPIO_Pin_7 /* PD7 */
#define LED5_PIN			GPIO_Pin_13 /* PB13 */

#define KEY1_PIN			GPIO_Pin_11	/* PD11 */
#define KEY2_PIN			GPIO_Pin_12	/* PD12 */
#define KEY3_PIN			GPIO_Pin_13	/* PC13 */
#define KEY4_PIN			GPIO_Pin_0	/* PA0 */

#define LED1(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED1_PIN)) : (GPIO_ResetBits(LED_PORT, LED1_PIN)));
#define LED2(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED2_PIN)) : (GPIO_ResetBits(LED_PORT, LED2_PIN)));
#define LED3(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED3_PIN)) : (GPIO_ResetBits(LED_PORT, LED3_PIN)));
#define LED4(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED4_PIN)) : (GPIO_ResetBits(LED_PORT, LED4_PIN)));
#define LED5(x)   ((x) ? (GPIO_SetBits(GPIOB, LED5_PIN)) : (GPIO_ResetBits(GPIOB, LED5_PIN)));

#define BEEP_PIN			GPIO_Pin_0	/* PC0 */

#define BEEP(x)   ((x) ? (GPIO_SetBits(GPIOC, BEEP_PIN)) : (GPIO_ResetBits(GPIOC, BEEP_PIN)));

/*------------------------BUTTON-----------------------*/


/*----------------------------USART----------------------------*/
/*使用串口之前先打开相应时钟
 * USART		TX		RX
   USART1       PA9     PA10
   USART2		PD5 	PD6*/
#define USART_PORT		GPIOA
#define USARTx      	USART1
#define USART_TX_PIN	GPIO_Pin_9
#define USART_RX_PIN	GPIO_Pin_10


/* 函数声明 */
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void BUTTON_Configuration(void);
void EXTI_Configuration(void);
void Delay(uint16_t nTime);  //延时函数

#endif
