
/***********************************************************************
文件名称：CAN.H
功    能：CAN数据定义
编写时间：2012.12.14
编 写 人：
注    意：
***********************************************************************/
#ifndef  _CAN_H_
#define  _CAN_H_

//#define CAN1_ID  0x18412345
//#define CAN2_ID  0x18467890

#define CAN_BAUD_NUM 20  //波特率个数

#define SendData1  0xaa  //按KEY1时CAN1发送的数据字节,
#define SendData2  0xbb  //按KEY2时CAN2发送的数据字节

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
