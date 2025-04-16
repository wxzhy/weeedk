/**************NUAA_CM3_107ʵ�鿪����*********************************
������ Descriptions:        һЩ�������ʼ������																	 
*********************************************************************************/ 

#include "hw_config.h"

uint8_t phasecw[4] ={0x08,0x04,0x02,0x01};//��ת �����ͨ���� D-C-B-A
uint8_t phaseccw[4]={0x01,0x02,0x04,0x08};//��ת �����ͨ���� A-B-C-D

uint8_t MotorData=0;
uint8_t BeepFlag=0;

/*********************************************
*�������ƣ�void Delayms(void)
*
*��ڲ�����N  ms
*
*���ڲ�������
*
*����˵��������ʱN ms
**********************************************/
void Delay_ms(uint16_t N)
{
	uint32_t i;
	for (i=0;i<(8000*N);i++);
}


//˳ʱ��ת��
void MotorCW(void)
{
 uint8_t i;
 uint16_t ReadOD=0,temp=0;
 for(i=0;i<4;i++)
  {
   MotorData=phasecw[i];
   ReadOD=GPIO_ReadOutputData(GPIOD);
   temp=MotorData<<2;		//0,1,2,3->PD2PD3PD4PD7
   if (MotorData&(1<<3)) temp|=(1<<7);
   else 				 temp&=~(1<<7);
   ReadOD&=~(0x9C);			//��2,3,4,7λ����
   temp=ReadOD+temp;
   GPIO_Write(GPIOD, temp);
   Delay_ms(10);//ת�ٵ���
  }
}
//��ʱ��ת��
void MotorCCW(void)
{
 uint8_t i;
 uint16_t ReadOD=0,temp=0;
 for(i=0;i<4;i++)
  {
   MotorData=phaseccw[i];
   ReadOD=GPIO_ReadOutputData(GPIOD);
   temp=MotorData<<2;		//0,1,2,3->PD2PD3PD4PD7
   if (MotorData&(1<<3)) temp|=(1<<7);
   else 				 temp&=~(1<<7);
   ReadOD&=~(0x9C);			//��2,3,4,7λ����
   temp=ReadOD+temp;
   GPIO_Write(GPIOD, temp);
   Delay_ms(10);//ת�ٵ���
  }
}
//ֹͣת��
void MotorStop(void)
{
 uint16_t ReadOD=0,temp=0;
   ReadOD=GPIO_ReadOutputData(GPIOD);
   ReadOD&=~(0x9C);			//��2,3,4,7λ����,����λ����
   temp=ReadOD;
   GPIO_Write(GPIOD, temp);
}


/*********************************************************************************
	*�������ƣ�void LED_Configuration(void)
	*
	*��ڲ�������
	*
	*���ڲ�������
	*
	*����˵����LED��ʼ������
*********************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED5_PIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2 ;//PB1,PB2˫ɫLEDָʾ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*---------��ʼ��״̬���LEDȫOFF------------*/
//	LED1=1;
//	LED2=1;
//	LED3=1;
//	LED4=1;
//	LED5=1;		
}

