
/***********************************************************************
�ļ����ƣ�CAN.H
��    �ܣ�CAN���ݶ���
��дʱ�䣺2012.12.14
�� д �ˣ�
ע    �⣺
***********************************************************************/
#ifndef  _CAN_H_
#define  _CAN_H_

//#define CAN1_ID  0x18412345
//#define CAN2_ID  0x18467890

#define CAN_BAUD_NUM 20  //�����ʸ���

#define SendData1  0xaa  //��KEY1ʱCAN1���͵������ֽ�,
#define SendData2  0xbb  //��KEY2ʱCAN2���͵������ֽ�

extern uint8_t CAN1_Rec[20];
extern uint8_t CAN2_Rec[20];
extern uint8_t CAN1ID;
extern uint8_t CAN2ID;
extern uint8_t CAN1_Rec_Flag;
extern uint8_t CAN1_DATA[8];
extern uint8_t CAN2_Rec_Flag;
extern uint8_t CAN2_DATA[8];

void  CAN1_SEND(uint16_t ID);
void  CAN2_SEND(uint16_t ID);
void CAN_Configuration(void);
void CAN_Baud_Process(unsigned int Baud,CAN_InitTypeDef *CAN_InitStructure);

#endif
