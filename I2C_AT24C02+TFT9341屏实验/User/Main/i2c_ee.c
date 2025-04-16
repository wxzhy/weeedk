
/************************************************************************************
**	  NUAA_CM3_107实验开发板（V1.0）
**	  I2C：24C02-EEPROM实验
	  I2CSCL:PB8,I2CSDA:PB9  
	  没有使用系统默认的I2C引脚(PB6,PB7)，因此要重新映射I2C(PB8,PB9)，
	  使用函数：
	  GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);			   //I2C重映射，不然只能使用PB6、7,映射后可使用PB8，PB9

**    版    本：V1.1
********************************************************************/
#include "i2c_ee.h"
#include "hw_config.h"
#define I2C_Speed              400000	   //快速模式400k
#define I2C1_SLAVE_ADDRESS7    0xA0	      //从机地址
#define I2C_PageSize           8	     //页面大小


uint16_t EEPROM_ADDRESS;

void GPIO_Configuration(void);
void I2C_Configuration(void);
								  
/**************************************************
*函数名称：void GPIO_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：配置使用的I/ O端口引脚 
***************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
    GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);			 //I2C重映射，不然只能使用PB6、PB7,映射后可使用PB8，PB9，必须在配置引脚之前重映射
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;  //配置I2C1 SDA SCL Pin，也可以配置PB6和PB7为SCL和SDA
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //管脚频率为50MHZ
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;		   //模式为复用开漏输出，因此两个引脚要接上拉电阻
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**************************************************
*函数名称：void I2C_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：I2C Configuration
***************************************************/
void I2C_Configuration(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 					     //定义I2C初始化结构体I2C_InitStructure
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;		         //I2C模式
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;         //占空比50%
  I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDRESS7;	 //本地地址(0xA0)使能应答，2402EEPROM等存储器地址均为0xA0
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	//应答7位地址
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;			      //快速模式400k
  I2C_Init(I2C1, &I2C_InitStructure);                         //初始化I2C
  
  I2C_Cmd(I2C1, ENABLE);		                              //使能I2C1
}


/**************************************************
*函数名称：void I2C_EE_Init()
*
*入口参数：无
*
*出口参数：无
*
*功能说明：初始化用于I2C EEPROM驱动的外设
***************************************************/
void I2C_EE_Init()
{
  
  GPIO_Configuration();

  I2C_Configuration();

  EEPROM_ADDRESS = EEPROM_Block_ADDRESS;	 //EEPROM块地址
}

/**************************************************
*函数名称：void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
*
*入口参数：pBuffer指向写入的数据缓冲区，WriteAddr写入EEPROM的地址，
*		   NumByteToWrite写入EEPROM的字节数
*出口参数：无
*
*功能说明：把数据写入I2C EEPROM
***************************************************/
void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % I2C_PageSize;
  count = I2C_PageSize - Addr;
  NumOfPage =  NumByteToWrite / I2C_PageSize;
  NumOfSingle = NumByteToWrite % I2C_PageSize;
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 
    	I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;
      }

      if(NumOfSingle!=0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_PageSize;
      NumOfSingle = NumByteToWrite % I2C_PageSize;	
      
      if(count != 0)
      {  
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }  
}


/**************************************************
*函数名称：void I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr)
*
*入口参数：pBuffer指向写入的数据缓冲区，WriteAddr写入的地址
*
*出口参数：无
*
*功能说明：写一个字节到I2C EEPROM
***************************************************/
void I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr)
{
  
  I2C_GenerateSTART(I2C1, ENABLE);		//起始信号

  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));   //  测试和清除EV5

  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);	 //发送写入EEPROMd的地址
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));	   //  测试和清除EV6
      
  
  I2C_SendData(I2C1, WriteAddr);                                       //发送写入EEPROM的内部地址
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));	    //  测试和清除EV8

  I2C_SendData(I2C1, *pBuffer);                                         //发送要写入的数据
   
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));	    //  测试和清除EV8
  
  I2C_GenerateSTOP(I2C1, ENABLE);	                                    //停止信号
}

/**************************************************
*函数名称：void I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite)
*
*入口参数：pBuffer指向写入的数据缓冲区，WriteAddr写入EEPROM的地址，
*		   NumByteToWrite写入EEPROM的字节数
*
*出口参数：无
*
*功能说明：写入多个字节的EEPROM具有单个写周期。字节的数目不能超过EEPROM的页面大小。
***************************************************/
void I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite)
{
 
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));  //忙等待
  
  I2C_GenerateSTART(I2C1, ENABLE);				 //起始信号
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));   //  测试和清除EV5
  
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);	 //发送写EEPROM地址

  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  // 测试和清除EV6

  I2C_SendData(I2C1, WriteAddr);                                  //发送要写入EEPROM的内部地址

  while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));	  // 测试和清除EV8

  while(NumByteToWrite--)  
  {
    I2C_SendData(I2C1, *pBuffer); 	  //发送当前字节

    pBuffer++; 		            //下一个被写入的字节
  
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));	 // 测试和清除EV8
  }

  I2C_GenerateSTOP(I2C1, ENABLE);	  //停止信号
}


/**************************************************
*函数名称：void I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
*
*入口参数：pBuffer指向读取的数据缓冲区，ReadAddr读取的EEPROM地址，
*		   NumByteToRead读取EEPROM的字节数
*
*出口参数：无
*
*功能说明:读取EEPROM的页数据
***************************************************/
void I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
  
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));   //忙等待
  
  I2C_GenerateSTART(I2C1, ENABLE);		   //起始信号
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));	   // 测试和清除EV5
   
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);	 //发送写EEPROM地址

  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));		// 测试和清除EV6
  
  I2C_Cmd(I2C1, ENABLE);	     // 使能I2C1

  I2C_SendData(I2C1, ReadAddr);    //发送读EEPROM地址

  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));		// 测试和清除EV8
   
  I2C_GenerateSTART(I2C1, ENABLE);
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));			 // 测试和清除EV5
  
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Receiver);
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));	  // 测试和清除EV6
  
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      I2C_AcknowledgeConfig(I2C1, DISABLE);
      
      I2C_GenerateSTOP(I2C1, ENABLE);
    }

    if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  				// 测试和清除EV7
    {      
      *pBuffer = I2C_ReceiveData(I2C1);	   //从EEPROM读取一个字节

	   pBuffer++; 							   //下一个读取的数据
      
      NumByteToRead--;        				   //递减字节计数
    }   
  }

  I2C_AcknowledgeConfig(I2C1, ENABLE);
}

/**************************************************
*函数名称：void I2C_EE_WaitEepromStandbyState(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：等待EEPROM的待机状态
***************************************************/
void I2C_EE_WaitEepromStandbyState(void)      
{
  __IO uint16_t SR1_Tmp = 0;

  do
  {
    I2C_GenerateSTART(I2C1, ENABLE);		//起始信号
    			
    SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);	  //阅读I2C1 SR1寄存器
    				  
    I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter); //发送EEPROM写入地址
  }while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));
  
  I2C_ClearFlag(I2C1, I2C_FLAG_AF);	   //清除AF标志位
  
  I2C_GenerateSTOP(I2C1, ENABLE);  	 //停止信号
}

