/************************************************************************************
**	  NUAA_CM3_107实验开发板（V1.0）
**	  I2C：24C02-EEPROM实验
**    版    本：V1.1
************************************************************************************/
#ifndef __I2C_EE_H
#define __I2C_EE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* The M24C02W contains 256 bytes */
/* EEPROM Addresses defines */
#define EEPROM_Block_ADDRESS 0xA0   /* Device Address */


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void I2C_EE_Init(void);
void I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr);
void I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite);
void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
void I2C_EE_WaitEepromStandbyState(void);

#endif /* __I2C_EE_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/


