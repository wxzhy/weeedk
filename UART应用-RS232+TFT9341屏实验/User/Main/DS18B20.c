/******************************************************************
**	  NUAA_CM3_107ʵ�鿪���壨V2.0��
**	  ��ȡ18b20�¶�ֵ������3.2��������ʾ
**	  ��    �ߣ�NUAA
**	  �������:	2016.8.25
********************************************************************/
#include "DS18B20.h"
#include "delay.h"

/**************************************************
*�������ƣ�void DS18B20_Rst(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵������λDS18B20	(ÿ�ζ�д��Ҫ�ȸ�λ)
***************************************************/
void DS18B20_Rst(void)	   
{                 
	DS18B20_DQ_OUT_LOW();	     // ����DQ 
    Delay_us(750);               // ��ʱ750us 
	DS18B20_DQ_OUT_HIGH();       // ����DQ	
	Delay_us(15);                // ��ʱ15US  
}


/**************************************************
*�������ƣ�uint8_t DS18B20_Init(void)
*
*��ڲ�������
*
*���ڲ�����1��δ�쵽DS18B20�Ĵ��� 0������
*
*����˵�����ȴ�DS18B20�Ļ�Ӧ
***************************************************/
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
    while (DS18B20_DQ_IN()&&retry<200)
	{
		retry++;
		Delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN()&&retry<240)
	{
		retry++;
		Delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}


/**************************************************
*�������ƣ�uint8_t DS18B20_Init(void)
*
*��ڲ�������
*
*���ڲ�����1��δ�쵽DS18B20�Ĵ��� 0������
*
*����˵������ʼ��18b20
***************************************************/
uint8_t DS18B20_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //�ܽ�Ƶ��50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  //��©��� 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	DS18B20_Rst();		                              //��λ 
	return DS18B20_Check();							  //0 ����; 1 δ��⵽DS18B20�Ĵ��� 
}
/**************************************************
*�������ƣ�void DS18B20_Write_Byte(u8 dat)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����дһ���ֽ�
***************************************************/
void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
			DS18B20_DQ_OUT_LOW();
            Delay_us(2);                            
			DS18B20_DQ_OUT_HIGH();
            Delay_us(60);             
        }
        else 
        {
			DS18B20_DQ_OUT_LOW();
            Delay_us(60);             
			DS18B20_DQ_OUT_HIGH();
            Delay_us(2);                          
        }
    }
}


/**************************************************
*�������ƣ�void DS18B20_Start(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵������ʼ�¶�ת��
***************************************************/
void DS18B20_Start(void)
{   						               
    DS18B20_Rst();	   				//��λDS18B20
	DS18B20_Check();	 			//�ȴ�DS18B20�Ļ�Ӧ
	//Delay_ms(500); 
    DS18B20_Write_Byte(0xcc);       //����ROM
    DS18B20_Write_Byte(0x44);       //���Ϳ�ʼת������
} 


/**************************************************
*�������ƣ�u8 DS18B20_Read_Bit(void)
*
*��ڲ�������
*
*���ڲ�����1/0
*
*����˵������DS18B20��ȡһ��λ
***************************************************/
u8 DS18B20_Read_Bit(void) 			
{
    u8 data;
    DS18B20_DQ_OUT_LOW(); 
	Delay_us(2);
    DS18B20_DQ_OUT_HIGH(); 
	Delay_us(12);
	if(DS18B20_DQ_IN())data=1;
    else data=0;	 
    Delay_us(50);           
    return data;
}


/**************************************************
*�������ƣ�u8 DS18B20_Read_Bit(void)
*
*��ڲ�������
*
*���ڲ�������ȡ������dat
*
*����˵������DS18B20��ȡһ���ֽ�
***************************************************/
u8 DS18B20_Read_Byte(void)    
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}
																											

/**************************************************
*�������ƣ�short DS18B20_Get_Temp(void)
*
*��ڲ�������
*
*���ڲ������¶�ֵ��-550~1250�� 
*
*����˵������ds18b20��ȡ�¶�ֵ ����0.1C
***************************************************/
short DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
	short tem;
    DS18B20_Start ();                    // ��ʼת��
    DS18B20_Rst();						 //��λ18b20
    DS18B20_Check();	 				 //���18b20�Ƿ����
    DS18B20_Write_Byte(0xcc);            //����ROM
    DS18B20_Write_Byte(0xbe);	 //���Ĵ����������ֽڣ�ǰ���ֽ�Ϊת��ֵ 
	
    TL=DS18B20_Read_Byte();               // LSB   
    TH=DS18B20_Read_Byte();               // MSB  
	    	  
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;                          //�¶�Ϊ��  
    }else temp=1;                        //�¶�Ϊ��	  	  
    tem=TH;                              //��ø߰�λ
    tem<<=8;    
    tem+=TL;                            //��õͰ�λ
    tem=(float)tem*0.625;               //ת��     
	if(temp)return tem;                 //�����¶�ֵ
	else return -tem;    
} 

