#ifndef __HW_CONFIG_H_
#define __HW_CONFIG_H_
#include "stm32f10x_conf.h"

/*使用位带操作，对单个IO灵活操作*/
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

/* LED */
#define RCC_APB2Periph_LED	RCC_APB2Periph_GPIOD
#define LED_PORT	GPIOD
#define LED1_PIN	GPIO_Pin_2
#define LED2_PIN	GPIO_Pin_3
#define LED3_PIN	GPIO_Pin_4
#define LED4_PIN	GPIO_Pin_7

#define LED1(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED1_PIN)) : (GPIO_ResetBits(LED_PORT, LED1_PIN)));
#define LED2(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED2_PIN)) : (GPIO_ResetBits(LED_PORT, LED2_PIN)));
#define LED3(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED3_PIN)) : (GPIO_ResetBits(LED_PORT, LED3_PIN)));
#define LED4(x)   ((x) ? (GPIO_SetBits(LED_PORT, LED4_PIN)) : (GPIO_ResetBits(LED_PORT, LED4_PIN)));
#define KEY1  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)	//读取按键KEY1
#define KEY2  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)//读取按键KEY2
#define KEY3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)	//读取按键KEY3 
#define KEY4  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//读取按键KEY4 


/* BUTTON */
#define RCC_APB2Periph_BUTTON	RCC_APB2Periph_GPIOD
#define GPIO_ButtonPort	GPIOD
#define BUTTON1	GPIO_Pin_8


/* 函数声明 */
void LED_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);
void SysTick_Configuration(void);
void USART2_SendString(uint8_t *ch);
void Delay_ms(uint16_t N);


#endif
