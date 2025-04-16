#include "hw_config.h"

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


#define DataToWrite(DATA)	GPIO_Write(GPIOE, DATA)  
#define DataToRead()	    GPIO_ReadInputData(GPIOE) 

#define E				GPIO_Pin_6   //PC6
#define RS				GPIO_Pin_13   //PD13
#define RW				GPIO_Pin_14	  //PD14
#define RSTB			GPIO_Pin_15	  //PD15

#define LCD_E(x)     ((x) ? (GPIO_SetBits(GPIOC, E)) : (GPIO_ResetBits(GPIOC, E)))
#define LCD_RS(x)    ((x) ? (GPIO_SetBits(GPIOD, RS)) : (GPIO_ResetBits(GPIOD, RS)))
#define LCD_RSTB(x)  ((x) ? (GPIO_SetBits(GPIOD, RSTB)) : (GPIO_ResetBits(GPIOD, RSTB)))
#define LCD_RW(x)    ((x) ? (GPIO_SetBits(GPIOD, RW)) : (GPIO_ResetBits(GPIOD, RW)))


#define RCC_LCD_DATA_GPIO		RCC_APB2Periph_GPIOE
#define RCC_LCD_CONTROL_GPIO	RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
#define LCD_DATA_GPIO			GPIOE  
 

/*Private define-------------------------*/
#define POINT_COLOR RED
#define BACK_COLOR	0xFFFF
/*--------16位颜色值---------------*/
#define WHITE        0xFFFF
#define BLACK        0x0000	  
#define BLUE         0x001F  
#define RED          0xF800
#define MAGENTA      0xF81F
#define GREEN        0x07E0
#define CYAN         0x7FFF
#define YELLOW       0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define LGRAY 			 0XC618 //浅灰色



/*----------函数声明------------*/

void LCD_Configuration(void);
void LCD_Init(void);
void clear_screen(void);
void displayInformation(void);
void display_bk(void);


