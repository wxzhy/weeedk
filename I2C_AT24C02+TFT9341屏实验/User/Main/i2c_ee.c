
/************************************************************************************
**	  NUAA_CM3_107ʵ�鿪���壨V1.0��
**	  I2C��24C02-EEPROMʵ��
	  I2CSCL:PB8,I2CSDA:PB9  
	  û��ʹ��ϵͳĬ�ϵ�I2C����(PB6,PB7)�����Ҫ����ӳ��I2C(PB8,PB9)��
	  ʹ�ú�����
	  GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);			   //I2C��ӳ�䣬��Ȼֻ��ʹ��PB6��7,ӳ����ʹ��PB8��PB9

**    ��    ����V1.1
********************************************************************/
#include "i2c_ee.h"
#include "hw_config.h"
#define I2C_Speed              400000	   //����ģʽ400k
#define I2C1_SLAVE_ADDRESS7    0xA0	      //�ӻ���ַ
#define I2C_PageSize           8	     //ҳ���С


uint16_t EEPROM_ADDRESS;

void GPIO_Configuration(void);
void I2C_Configuration(void);
								  
/**************************************************
*�������ƣ�void GPIO_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵��������ʹ�õ�I/ O�˿����� 
***************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
    GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);			 //I2C��ӳ�䣬��Ȼֻ��ʹ��PB6��PB7,ӳ����ʹ��PB8��PB9����������������֮ǰ��ӳ��
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;  //����I2C1 SDA SCL Pin��Ҳ��������PB6��PB7ΪSCL��SDA
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //�ܽ�Ƶ��Ϊ50MHZ
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;		   //ģʽΪ���ÿ�©����������������Ҫ����������
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**************************************************
*�������ƣ�void I2C_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����I2C Configuration
***************************************************/
void I2C_Configuration(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 					     //����I2C��ʼ���ṹ��I2C_InitStructure
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;		         //I2Cģʽ
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;         //ռ�ձ�50%
  I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDRESS7;	 //���ص�ַ(0xA0)ʹ��Ӧ��2402EEPROM�ȴ洢����ַ��Ϊ0xA0
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	//Ӧ��7λ��ַ
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;			      //����ģʽ400k
  I2C_Init(I2C1, &I2C_InitStructure);                         //��ʼ��I2C
  
  I2C_Cmd(I2C1, ENABLE);		                              //ʹ��I2C1
}


/**************************************************
*�������ƣ�void I2C_EE_Init()
*
*��ڲ�������
*
*���ڲ�������
*
*����˵������ʼ������I2C EEPROM����������
***************************************************/
void I2C_EE_Init()
{
  
  GPIO_Configuration();

  I2C_Configuration();

  EEPROM_ADDRESS = EEPROM_Block_ADDRESS;	 //EEPROM���ַ
}

/**************************************************
*�������ƣ�void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
*
*��ڲ�����pBufferָ��д������ݻ�������WriteAddrд��EEPROM�ĵ�ַ��
*		   NumByteToWriteд��EEPROM���ֽ���
*���ڲ�������
*
*����˵����������д��I2C EEPROM
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
*�������ƣ�void I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr)
*
*��ڲ�����pBufferָ��д������ݻ�������WriteAddrд��ĵ�ַ
*
*���ڲ�������
*
*����˵����дһ���ֽڵ�I2C EEPROM
***************************************************/
void I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr)
{
  
  I2C_GenerateSTART(I2C1, ENABLE);		//��ʼ�ź�

  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));   //  ���Ժ����EV5

  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);	 //����д��EEPROMd�ĵ�ַ
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));	   //  ���Ժ����EV6
      
  
  I2C_SendData(I2C1, WriteAddr);                                       //����д��EEPROM���ڲ���ַ
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));	    //  ���Ժ����EV8

  I2C_SendData(I2C1, *pBuffer);                                         //����Ҫд�������
   
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));	    //  ���Ժ����EV8
  
  I2C_GenerateSTOP(I2C1, ENABLE);	                                    //ֹͣ�ź�
}

/**************************************************
*�������ƣ�void I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite)
*
*��ڲ�����pBufferָ��д������ݻ�������WriteAddrд��EEPROM�ĵ�ַ��
*		   NumByteToWriteд��EEPROM���ֽ���
*
*���ڲ�������
*
*����˵����д�����ֽڵ�EEPROM���е���д���ڡ��ֽڵ���Ŀ���ܳ���EEPROM��ҳ���С��
***************************************************/
void I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite)
{
 
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));  //æ�ȴ�
  
  I2C_GenerateSTART(I2C1, ENABLE);				 //��ʼ�ź�
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));   //  ���Ժ����EV5
  
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);	 //����дEEPROM��ַ

  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  // ���Ժ����EV6

  I2C_SendData(I2C1, WriteAddr);                                  //����Ҫд��EEPROM���ڲ���ַ

  while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));	  // ���Ժ����EV8

  while(NumByteToWrite--)  
  {
    I2C_SendData(I2C1, *pBuffer); 	  //���͵�ǰ�ֽ�

    pBuffer++; 		            //��һ����д����ֽ�
  
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));	 // ���Ժ����EV8
  }

  I2C_GenerateSTOP(I2C1, ENABLE);	  //ֹͣ�ź�
}


/**************************************************
*�������ƣ�void I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
*
*��ڲ�����pBufferָ���ȡ�����ݻ�������ReadAddr��ȡ��EEPROM��ַ��
*		   NumByteToRead��ȡEEPROM���ֽ���
*
*���ڲ�������
*
*����˵��:��ȡEEPROM��ҳ����
***************************************************/
void I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
  
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));   //æ�ȴ�
  
  I2C_GenerateSTART(I2C1, ENABLE);		   //��ʼ�ź�
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));	   // ���Ժ����EV5
   
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);	 //����дEEPROM��ַ

  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));		// ���Ժ����EV6
  
  I2C_Cmd(I2C1, ENABLE);	     // ʹ��I2C1

  I2C_SendData(I2C1, ReadAddr);    //���Ͷ�EEPROM��ַ

  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));		// ���Ժ����EV8
   
  I2C_GenerateSTART(I2C1, ENABLE);
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));			 // ���Ժ����EV5
  
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Receiver);
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));	  // ���Ժ����EV6
  
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      I2C_AcknowledgeConfig(I2C1, DISABLE);
      
      I2C_GenerateSTOP(I2C1, ENABLE);
    }

    if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  				// ���Ժ����EV7
    {      
      *pBuffer = I2C_ReceiveData(I2C1);	   //��EEPROM��ȡһ���ֽ�

	   pBuffer++; 							   //��һ����ȡ������
      
      NumByteToRead--;        				   //�ݼ��ֽڼ���
    }   
  }

  I2C_AcknowledgeConfig(I2C1, ENABLE);
}

/**************************************************
*�������ƣ�void I2C_EE_WaitEepromStandbyState(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵�����ȴ�EEPROM�Ĵ���״̬
***************************************************/
void I2C_EE_WaitEepromStandbyState(void)      
{
  __IO uint16_t SR1_Tmp = 0;

  do
  {
    I2C_GenerateSTART(I2C1, ENABLE);		//��ʼ�ź�
    			
    SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);	  //�Ķ�I2C1 SR1�Ĵ���
    				  
    I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter); //����EEPROMд���ַ
  }while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));
  
  I2C_ClearFlag(I2C1, I2C_FLAG_AF);	   //���AF��־λ
  
  I2C_GenerateSTOP(I2C1, ENABLE);  	 //ֹͣ�ź�
}

