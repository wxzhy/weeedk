#ifndef __REMOTE_H
#define __REMOTE_H
#include "stm32f10x_conf.h" 

#define RDATA GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)

/**	����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
	����ѡ�õ�ң����ʶ����Ϊ0 **/
#define REMOTE_ID 0

extern u8 Remote_Cnt;         /*��������,�˴ΰ��¼��Ĵ��� */
extern u8 Remote_Rdy;         /*������յ����� */
extern u32 Remote_Odr;        /*�����ݴ洦	 */
void Remote_Init(void);       /*���⴫��������ͷ���ų�ʼ�� */
u8 Remote_Process(void);      /*������յ����ݴ��� */
u8 Pulse_Width_Check(void);   /*�������   */
void delay_init(u8 SYSCLK);

#endif
