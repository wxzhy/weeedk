//#include "hw_config.h"

//IO¿ÚµØÖ·Ó³Éä
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


#define DataToWrite(DATA)	GPIO_Write(GPIOE, DATA)  
#define DataToRead()	    GPIO_ReadInputData(GPIOE) 

#define CSB					GPIO_Pin_6	  //PC6
#define A0_RS				GPIO_Pin_13   //PD13
#define RSTB				GPIO_Pin_15	  //PD15
#define SDA					GPIO_Pin_7	  //PC7
#define SCL					GPIO_Pin_9	  //PC9
#define MISO				GPIO_Pin_11	  //PC11
#define MOSI				GPIO_Pin_12	  //PC12
#define SCLK				GPIO_Pin_10	  //PC10
#define nCS					GPIO_Pin_8	  //PC8

#define LCD_CSB(x)   ((x) ? (GPIO_SetBits(GPIOC, CSB)) : (GPIO_ResetBits(GPIOC, CSB)))
#define LCD_RS(x)    ((x) ? (GPIO_SetBits(GPIOD, A0_RS)) : (GPIO_ResetBits(GPIOD, A0_RS)))
#define LCD_RSTB(x)  ((x) ? (GPIO_SetBits(GPIOD, RSTB)) : (GPIO_ResetBits(GPIOD, RSTB)))
#define LCD_SDA(x)   ((x) ? (GPIO_SetBits(GPIOC, SDA)) : (GPIO_ResetBits(GPIOC, SDA)))
#define LCD_SCL(x)   ((x) ? (GPIO_SetBits(GPIOC, SCL)) : (GPIO_ResetBits(GPIOC, SCL)))
#define LCD_MOSI(x)  ((x) ? (GPIO_SetBits(GPIOC, MOSI)) : (GPIO_ResetBits(GPIOC, MOSI)))
#define LCD_SCLK(x)  ((x) ? (GPIO_SetBits(GPIOC, SCLK)) : (GPIO_ResetBits(GPIOC, SCLK)))
#define LCD_nCS(x)   ((x) ? (GPIO_SetBits(GPIOC, nCS)) : (GPIO_ResetBits(GPIOC, nCS)))

#define LCD_MISO  GPIO_ReadInputDataBit(GPIOC,MISO);


#define RCC_LCD_DATA_GPIO		RCC_APB2Periph_GPIOE
#define RCC_LCD_CONTROL_GPIO	RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
#define LCD_DATA_GPIO			GPIOE  
 



/*----------º¯ÊýÉùÃ÷------------*/
/* Ë¢ÆÁ */



void LCD_HJ12864Configuration(void);
void LCD_HJ12864_Init(void);
void clear_screenHJ(void);
void displayInformationHJ(void);
void display_bk(void);
void display_HJGB2312_string(uint8_t page,uint8_t column,uint8_t *text,uint8_t fb);




