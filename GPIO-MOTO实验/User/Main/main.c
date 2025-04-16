
/************************************************************************************
**	  WEEEDK NUAA_CM3_107ʵ�鿪���壨V4.1��
**	  GPIOӦ��:�������ʵ��
**    �������������س������к�
			������KEY1��ʱ��˫ɫ�����˸�ҵ����ת��������KEY2��ʱ��˫ɫ�̵���˸�ҵ����ת��������KEY3��ʱ��˫ɫ���𣬵��ֹͣ
**    ��    ����V4.1
**	  ��    �ߣ�NUAA
**	  �������:	2017.3.19
************************************************************************************/
#include "stm32f10x_conf.h"

#define KEY1	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)
#define KEY2	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)
#define KEY3	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)
#define IA(x)   	((x) ? (GPIO_SetBits(GPIOB, GPIO_Pin_8)) : (GPIO_ResetBits(GPIOB, GPIO_Pin_8)))
#define IB(x)   	((x) ? (GPIO_SetBits(GPIOB, GPIO_Pin_9)) : (GPIO_ResetBits(GPIOB, GPIO_Pin_9)))
#define LEDR(x)   ((x) ? (GPIO_SetBits(GPIOB, GPIO_Pin_1)) : (GPIO_ResetBits(GPIOB, GPIO_Pin_1)))
#define LEDG(x)   ((x) ? (GPIO_SetBits(GPIOB, GPIO_Pin_2)) : (GPIO_ResetBits(GPIOB, GPIO_Pin_2)))
/*********************************************
*�������ƣ�void Delayms(void)
*
*��ڲ�����N  ms
*
*���ڲ�������
*
*����˵��������ʱN ms
**********************************************/
void Delayms(uint16_t N)
{
	uint32_t i;
	for (i=0;i<(8000*N);i++);
}

/*********************************************
*�������ƣ�void GPIO_MOTO_Init(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵������ʼ��GPIO��LED��MOTO��KEY��
**********************************************/
void GPIO_MOTO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);  /*��ʼ��GPIOD�˿�PD11=KEY1,PD12=KEY2�������룬10MHz*/

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);  /*��ʼ��GPIOC�˿�PC13=KEY3�������룬10MHz*/

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);  /*��ʼ��GPIOD�˿�PD2��3��4��7 10MHz�������*/
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);  /*��ʼ��GPIOB�˿�PB1=RED,PB2=GREEN,PB8=IA,PB9=IB 10MHz�������*/
	
	/*---------��ʼ��״ָ̬ʾ����OFF------------*/
	IA(0);IB(0);					/*  ���ֹͣ  */
	LEDR(0);LEDG(0);			/*  ˫ɫָʾ��ȫ��  */	
	GPIO_SetBits(GPIOD,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7);/*  4ָֻʾ���� */
}

#define  n 200    /*��ʱʱ�䳣��*/
int main(void)
{
u8 KEY=0;	
	SystemInit();     		/*ϵͳ��ʼ��*/
	GPIO_MOTO_Init();			/*GPIO��ʼ��*/
	while (1)
	{if (KEY1==0) 		KEY=1;
	else{if (KEY2==0) KEY=2;
	else{if (KEY3==0) KEY=3;}}	
		switch(KEY)
			{
			case 1:		/* �����ת  */
					IA(1);			/*  PB8=IA=1  			*/
					IB(0);			/*  PB9=IB=0  			*/
					LEDR(1);		/*  PB1=1 �����˸ 	*/
					Delayms(n);
					LEDR(0);
					Delayms(n);
					LEDG(0);		/*  PB2=1 �̵���  */
					break;
			case 2:		/* �����ת  */
					IA(0);			/*  PB9=IB=1  			*/
					IB(1);			/*  PB8=IA=0  			*/
					LEDR(0);		/*  PB1=0 �����  	*/
					LEDG(1);		/*  PB2=1 �̵���˸  */
					Delayms(n);
					LEDG(0);
					Delayms(n);
					break;
			case 3:		/* ���ֹͣ  */
					IA(0);			/*  PB9=IB=0  			*/
					IB(0);			/*  PB8=IA=0  			*/
					LEDR(0);		/*  PB1=0 �����  	*/
					LEDG(0);		/*  PB2=1 �̵���  	*/
					break;
			}	
	}
}
										  






