
/******************************************************************
**	 NUAA_CS_CM3_Kit WEEEDK4.1������
**	  W25Q16.c��д�ļ�
**	  �������:	2014.3.14
********************************************************************/
#include "w25q16.h"

#include "hw_config.h"

/**************************************************

*�������ƣ�u8 SPIx_ReadWriteByte(u8 TxData)
*
*��ڲ�����TxData Ҫд����ֽ�
*
*���ڲ�����RxData ��ȡ�����ֽ�
*
*����˵����
***************************************************/
u8 SPIx_ReadWriteByte(u8 TxData)
{		
	uint8_t RxData = 0;
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);	 //�ȴ����ͻ������� 
	SPI_I2S_SendData(SPI1, TxData);	                                 //��һ���ֽ�

    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);	 //�ȴ����ݽ���	 
	RxData = SPI_I2S_ReceiveData(SPI1);	                             //���ؽ��յ�������*/

    return (uint8_t)RxData;				    
}
/**************************************************

*�������ƣ�void SPI_FLASH_Write_Enable(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����SPI_FLASHдʹ��
***************************************************/
void SPI_FLASH_Write_Enable(void)   
{												 
	W25Q16_CS_LOW();                              //CS=0 
    SPIx_ReadWriteByte(W25X_WriteEnable);         //����дʹ��  
	W25Q16_CS_HIGH();                             //CS=1    	      
} 


/**************************************************

*�������ƣ�void SPI_Flash_Erase_Chip(void)   
*
*��ڲ�������
*
*���ڲ�������
*
*����˵������������оƬ����Ƭ����ʱ��:
*		   W25X16:25s  W25X32:40s  W25X64:40s 	 �ȴ�ʱ�䳬��...
*		      
***************************************************/
void SPI_Flash_Erase_Chip(void)   
{                                             
    SPI_FLASH_Write_Enable();                  //SET WEL 
    SPI_Flash_Wait_Busy();   
  	W25Q16_CS_LOW();                            //ʹ������   
    SPIx_ReadWriteByte(W25X_ChipErase);        //����Ƭ��������  
	W25Q16_CS_HIGH();                            //ȡ��Ƭѡ     	      
	SPI_Flash_Wait_Busy();   				   //�ȴ�оƬ��������
} 


/**************************************************

*�������ƣ�void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)
*
*��ڲ�����ReadAddr ��ʼ��ȡ�ĵ�ַ(24bit)
*		   NumByteToRead Ҫ��ȡ���ֽ���(���65535)
*
*���ڲ�����pBuffer ָ�������
*
*����˵������ָ����ַ��ʼ��ȡָ�����ȵ�����
***************************************************/
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;    												    
	W25Q16_CS_LOW();                            //ʹ������   
    SPIx_ReadWriteByte(W25X_ReadData);          //���Ͷ�ȡ����   
    SPIx_ReadWriteByte((u8)((ReadAddr)>>16));   //����24bit��ַ    
    SPIx_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPIx_ReadWriteByte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPIx_ReadWriteByte(0XFF);    //ѭ������  
    }
	W25Q16_CS_HIGH();                           //ȡ��Ƭѡ     	      
}

//��ȡSPI_FLASH��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
/**************************************************

*�������ƣ�uint8_t SPI_Flash_ReadSR(void) 
*
*��ڲ�������
*
*���ڲ�����byte
*
*����˵����
***************************************************/
uint8_t SPI_Flash_ReadSR(void)   
{  
	u8 byte=0;   
	W25Q16_CS_LOW();                      //ʹ������   
	SPIx_ReadWriteByte(0x05);            //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPIx_ReadWriteByte(0Xff);      //��ȡһ���ֽ�  
	W25Q16_CS_HIGH();                  //ȡ��Ƭѡ     
	return byte;   
} 

/**************************************************

*�������ƣ�uint8_t SPI_Flash_WriteSR(uint16_t data)
*
*��ڲ�����data
*
*���ڲ�����0
*
*����˵����
***************************************************/
uint8_t SPI_Flash_WriteSR(uint16_t data)
{
	W25Q16_CS_LOW();                           //ʹ������   
	SPIx_ReadWriteByte(0x01);                  //����д״̬�Ĵ�������    
	SPIx_ReadWriteByte(data&0x00ff);           //����һ���ֽ�  
	SPIx_ReadWriteByte(data>>8);               //����һ���ֽ�  
	W25Q16_CS_HIGH();                          //ȡ��Ƭѡ     
	return 0;   
}
/**************************************************

*�������ƣ�void SPI_Flash_Wait_Busy(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵�����ȴ�����
***************************************************/
void SPI_Flash_Wait_Busy(void)   
{   
	while ((SPI_Flash_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}
//����һ������
//Dst_Addr:������ַ 0~511 for w25x16
//����һ������������ʱ��:150ms
void SPI_Flash_Erase_Sector(u32 Dst_Addr)   
{   
	Dst_Addr*=4096;
    SPI_FLASH_Write_Enable();                  //SET WEL 	 
    SPI_Flash_Wait_Busy();   
	W25Q16_CS_LOW();                           //ʹ������   
    SPIx_ReadWriteByte(W25X_SectorErase);      //������������ָ�� 
    SPIx_ReadWriteByte((u8)((Dst_Addr)>>16));  //����24bit��ַ    
    SPIx_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPIx_ReadWriteByte((u8)Dst_Addr);  
	W25Q16_CS_HIGH();                          //ȡ��Ƭѡ     
    SPI_Flash_Wait_Busy();   				   //�ȴ��������
}  


 



/**************************************************
*�������ƣ�void SPI_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
*
*��ڲ�����pBuffer: ���ݴ洢��  ��  WriteAddr: ��ʼд��ĵ�ַ	 ��
*	       NumByteToWrite: Ҫд����ֽ���(���256)��������Ӧ�ó�����ҳ��ʣ���ֽ���������
*���ڲ�������
*	    
*����˵������ָ����ַ��ʼд�����256�ֽڵ�����
*	       SPI��һҳ(0~65535)��д������256���ֽڵ�����
***************************************************/					
void SPI_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
    SPI_FLASH_Write_Enable();                  //SET WEL 
		W25Q16_CS_LOW();                           //ʹ������   
    SPIx_ReadWriteByte(W25X_PageProgram);      //����дҳ����   
    SPIx_ReadWriteByte((u8)((WriteAddr)>>16)); //����24bit��ַ    
    SPIx_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPIx_ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++) SPIx_ReadWriteByte(pBuffer[i]);//ѭ��д��  
	W25Q16_CS_HIGH();                            //ȡ��Ƭѡ 
	SPI_Flash_Wait_Busy();					   //�ȴ�д�����
} 


/**************************************************

*�������ƣ�u16 SPI_Flash_ReadID(void)
*
*��ڲ�������
*
*���ڲ�����Temp
*
*����˵������ȡW25Q16��ID  W25X16��ID:0XEF14
***************************************************/
u16 SPI_Flash_ReadID(void)
{
	u16 Temp = 0;	  
	W25Q16_CS_LOW();
	SPIx_ReadWriteByte(0x90);     //���Ͷ�ȡID����	    
	SPIx_ReadWriteByte(0x00); 	    
	SPIx_ReadWriteByte(0x00); 	    
	SPIx_ReadWriteByte(0x00); 	 			   
	Temp|=SPIx_ReadWriteByte(0xFF)<<8;  
	Temp|=SPIx_ReadWriteByte(0xFF);	 
	W25Q16_CS_HIGH();				    
	return Temp;
}

/**************************************************

*�������ƣ�void W25Q16_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵�����˿ں�SPI������
***************************************************/
static void W25Q16_Configuration(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO | RCC_APB2Periph_SPI1, ENABLE);

	/*-----------------�˿�����-------------------*/
	/* SPI1_SCK SPI1_MISO SPI1_MOSI*/
    GPIO_InitStructure.GPIO_Pin = W25Q16_SCK_PIN | W25Q16_MISO_PIN | W25Q16_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(W25Q16_PORT, &GPIO_InitStructure);

	/* FLASH_CS */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_9);   /* ƬѡCS=1��PB9=1��*/

	/*-----------------SPI����--------------------*/
	/* ��ʼ�� */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;	/*����ʱ���ȶ�״̬�Ǹߵ�ƽ*/
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;/* ʱ�ӵڶ������ؿ�ʼ*/
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; /*SPI������Ԥ��Ƶ*/
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; /*��λ��ǰ*/ 
	SPI_InitStructure.SPI_CRCPolynomial = 7; /*the polynomial used for the CRC calculation*/
	SPI_Init(W25Q16_SPI, &SPI_InitStructure);

	/* SPIʹ�� */
	SPI_Cmd(W25Q16_SPI, ENABLE);     
}

/**************************************************

*�������ƣ�uint8_t W25Q16_Init(void)
*
*��ڲ�������
*
*���ڲ�����0����ʼ���ɹ���1����ʼ��ʧ��
*
*����˵������ʼ��W25Q16
***************************************************/
uint8_t W25Q16_Init(void)
{
	uint16_t temp;
	W25Q16_Configuration();		        // SPI�������ü���ʼ��
	temp = SPI_Flash_ReadID();
	if(temp==W25Q16_ID)	//�ж�ID�Ƿ�Ϊ0XEF14
	{
		printf("\r\n W25Q16 ID is 0x%x \r\n", temp);
		return 0;
	}
	else return 1;
}
