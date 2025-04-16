
/******************************************************************
**	  ����107�����壨V1.0��
**	  Ӳ�������ļ�
**
**	  ��    ̳��bbs.openmcu.com
**	  ��    ����www.openmcu.com
**	  ��    �䣺support@openmcu.com
**
**    ��    ����V1.1
**	  ��    �ߣ�openmcu
**	  �������:	2014.3.11
********************************************************************/
#include "hw_config.h" 

/**************************************************
*�������ƣ�void NVIC_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����NVIC����
***************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);   	  //ʹ�����ȼ�����0
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;       //ָ���ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    // ָ����Ӧ���ȼ���1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/**************************************************
*�������ƣ�void LED_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����LED��ʼ������
***************************************************/
void LED_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_LED, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(LED_PORT, &GPIO_InitStructure);

	LED1(0);   //��ʼ��״̬�ĸ�LEDȫOFF
	LED2(0);
	LED3(0);
	LED4(0);
}
/**************************************************
*�������ƣ�void TIMER_Configuration(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����TIM4��ʼ������
***************************************************/
void TIMER_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��

	/* Prescaler Ԥ��Ƶϵ��Ϊ36000-1������������ʱ��Ϊ72MHz/36000 = 2kHz
	   Period    ���ü��������С��ÿ��2000�����Ͳ���һ�������¼�
	   ������ʵ����1s��˸һ��
    */
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36000-1;                // ��Ƶֵ  ���65536

	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	  //���ϼ���ģʽ

	TIM_TimeBaseInitStructure.TIM_Period = 2000-1; 		              //ARR��ֵ ���65536

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

	TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);	    // �ж�ʹ��

 	TIM_Cmd(TIM4, ENABLE);                         //ʹ�ܶ�ʱ��
}



