/***********************************************************************
文件名称：can.C
功    能：CAN通信，波特率250K
编写时间：2016.11.22
编 写 人：NUAA
注    意：连接JP10到CAN2TX(C2TX)，JP11到CAN2RX(C2RX)
***********************************************************************/
#include "main.h"

uint8_t CAN1ID=0xaa;	
uint8_t CAN2ID=0xbb;	
uint8_t CAN1_Rec_Flag = 0;
uint8_t CAN1_DATA[]={0xC1,0x10,0x11,0x12,0x13,0x14,0x15,0x16};//等待CAN1发送的数据
uint8_t CAN2_Rec_Flag = 0;
uint8_t CAN2_DATA[]={0xC2,0x20,0x21,0x22,0x23,0x24,0x25,0x26};//等待CAN2发送的数据
uint8_t CAN1_Rec[20];
uint8_t CAN2_Rec[20];
//格式： 波特率  CAN_SJW 	CAN_BS1  CAN_BS2 CAN_Prescaler
const unsigned int CAN_baud_table[CAN_BAUD_NUM][5] = 
{
//波特率=36000/((SJW+BS1+BS2+3)*BPR) ，这里已经考虑加1了！ 
//波特率， CAN_SJW，   CAN_BS1，    CAN_BS2，CAN_Prescaler 
	{5,   CAN_SJW_1tq,CAN_BS1_13tq,CAN_BS2_2tq,450},		//5K			
	{10,  CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_2tq, 400},		//10K			
	{15,  CAN_SJW_1tq,CAN_BS1_13tq,CAN_BS2_2tq,150},		//15K
	{20,  CAN_SJW_1tq,CAN_BS1_6tq, CAN_BS2_2tq,200},		//20k
	{25,  CAN_SJW_1tq,CAN_BS1_13tq,CAN_BS2_2tq,90},			//25k
	{40,  CAN_SJW_1tq,CAN_BS1_6tq, CAN_BS2_2tq,100},		//40k
	{50,  CAN_SJW_1tq,CAN_BS1_13tq,CAN_BS2_2tq,45},			//50k
	{62,  CAN_SJW_1tq,CAN_BS1_13tq,CAN_BS2_2tq,36},			//62.5k
	{80,  CAN_SJW_1tq,CAN_BS1_6tq, CAN_BS2_2tq,50},			//80k
	{100, CAN_SJW_1tq,CAN_BS1_5tq, CAN_BS2_2tq,45},			//100K
	{125, CAN_SJW_1tq,CAN_BS1_13tq, CAN_BS2_2tq,18},		//125K
	{200, CAN_SJW_1tq,CAN_BS1_6tq, CAN_BS2_2tq,20},			//200K
	{250, CAN_SJW_1tq,CAN_BS1_13tq,CAN_BS2_2tq,9},		  //250k
/*******以下400K以上,如果不接匹配电阻，则通信不正常，接了匹配电阻很正常*********/
	{400, CAN_SJW_1tq,CAN_BS1_6tq, CAN_BS2_2tq,10},			//未能400K以上，通信不正常
	{500, CAN_SJW_1tq,CAN_BS1_5tq, CAN_BS2_2tq,9},			//未通
	{666, CAN_SJW_1tq,CAN_BS1_5tq, CAN_BS2_2tq,8},			//未通
	{800, CAN_SJW_1tq,CAN_BS1_6tq, CAN_BS2_2tq,5},			//未通800K
	{1000,CAN_SJW_1tq,CAN_BS1_6tq, CAN_BS2_2tq,4},			//未通1000K
};
void CAN_Configuration(void)
{
	
	CAN_InitTypeDef        CAN_InitStructure;
	
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	CAN_DeInit(CAN1);
	CAN_DeInit(CAN2);
	CAN_StructInit(&CAN_InitStructure);//CAN总线的默认值装入,默认装入的参数如下：
	/*
		CAN_TTCM=DISABLE，失能时间触发通讯模式
		CAN_ABOM=DISABLE，失能自动离线管理
		CAN_AWUM=DISABLE，失能自动唤醒模式
		CAN_NART=DISABLE，失能非自动重传输模式
		CAN_RFLM=DISABLE，失能接收 FIFO 锁定模式
		CAN_TXFP=DISABLE，失能发送 FIFO 优先级
		CAN_Mode=CAN_Mode_Normal，CAN 硬件工作在正常模式
		CAN_SJW=CAN_SJW_1tq，重新同步跳跃宽度 1 个时间单位
		CAN_BS1=CAN_BS1_4tq，时间段 1 为 4 个时间单位
		CAN_BS2=CAN_BS2_3tq，时间段 2 为3 个时间单位
		CAN_Prescaler=1，预分频为1
	以上SJW、BS1、BS2和Prescaler决定CAN通信波特率。波特率为：
		BAUD=36000K/((1+4+3)*1)=4500Kbps
	*/

	CAN_Baud_Process(250,&CAN_InitStructure);//波特率250K
	
	CAN_Init(CAN1,&CAN_InitStructure);//CAN1初始化
	CAN_Init(CAN2,&CAN_InitStructure);//CAN2初始化
	
	//CAN1滤波器设置
	CAN_FilterInitStructure.CAN_FilterNumber = 0;     ///* 过滤器组0~13有效 */
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;//CAN_FilterMode_IdList;//CAN_FilterMode_IdMask;  /* 屏敝模式 */
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; ///* 32位 */
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0;  
	CAN_FilterInitStructure.CAN_FilterIdLow = 0;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;  ///* 能够通过该过滤器的报文存到fifo0中 */
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	//CAN2滤波器设置
	CAN_FilterInitStructure.CAN_FilterNumber = 14;     ///* 过滤器组14~27有效,其它设置同CAN1 */
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);   /* 挂号中断, 进入中断后读fifo的报文函数释放报文清中断标志 */
	CAN_ITConfig(CAN2,CAN_IT_FMP0, ENABLE);   /* 挂号中断, 进入中断后读fifo的报文函数释放报文清中断标志 */
}

/***********************************************************************
文件名称：CAN_Baud_Process
功    能：计算波特率，返回
编写时间：2013.4.25
编 写 人：
注    意：
CAN_SJW : CAN_SJW_1tq - CAN_SJW_4tq	  不能比任何一相位缓冲段长
CAN_BS1 : CAN_BS1_1tq - CAN_BS1_16tq
CAN_BS2 : CAN_BS2_1tq - CAN_BS2_8tq
CAN_Prescaler : 1 - 1024
	配置说明：
CAN_SJW + CAN_BS1 / (CAN_SJW + CAN_BS1 + CAN_BS2)
	0.75     baud > 800k
	0.80     baud > 500k
	0.875    baud <= 500k
	baud = 36000 / (CAN_SJW + CAN_BS1 + CAN_BS2) / CAN_Prescaler     （Kbps）
***********************************************************************/
void CAN_Baud_Process(unsigned int Baud,CAN_InitTypeDef *CAN_InitStructure)
{
	unsigned int i = 0;
	for(i = 0;i < CAN_BAUD_NUM;i ++)
	{
		if(Baud == CAN_baud_table[i][0])
		{
			CAN_InitStructure->CAN_SJW = CAN_baud_table[i][1];
			CAN_InitStructure->CAN_BS1 = CAN_baud_table[i][2];
			CAN_InitStructure->CAN_BS2 = CAN_baud_table[i][3];
			CAN_InitStructure->CAN_Prescaler = CAN_baud_table[i][4];
			break;	
		}
	}	
}

void  CAN1_SEND(uint16_t ID)
{
		
	int8_t i;
	CanTxMsg TxMessage;

	TxMessage.StdId=ID;	//标准标识符
  TxMessage.ExtId=0x0000; //扩展标识符0x0000
	TxMessage.IDE = CAN_ID_STD;//使用标准标识符	 CAN_ID_EXT为扩展标识符
	TxMessage.RTR = CAN_RTR_DATA; /* 设置为数据帧 */
	TxMessage.DLC = 8;            /* 数据长度, can报文规定最大的数据长度为8字节 */
	
	for(i = 0;i < 8; i ++)
	{
		TxMessage.Data[i] = CAN1_DATA[i];//发送数据
	}
   
	CAN_Transmit(CAN1,&TxMessage);  /* 返回这个信息请求发送的邮箱号0,1,2或没有邮箱申请发送no_box */	
}

//CAN1中断程序
void CAN1_RX0_IRQHandler(void)
{
uint8_t i;
	CanRxMsg RxMessage; 
	RxMessage.StdId=0x00;
  RxMessage.ExtId=0x00;
  RxMessage.IDE=0;
  RxMessage.DLC=0;
  RxMessage.FMI=0;
  RxMessage.Data[0]=0x00;     
	
	CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);  // 使用FIFO 0作为接收缓冲器，接收CAN总线上发到CAN1上的数据 
	
	if(RxMessage.StdId==CAN2ID)				  //判断是否有CAN2发送过来的
	{
	for (i=0;i<8;i++)   CAN1_Rec[i]=RxMessage.Data[i];//将接收到的数据存入CAN1_Rec指示的缓冲区，以便主函数处理     	
	CAN1_Rec_Flag = 1;
	}

	CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);  // 清除挂起中断
//	SEI();//打开总中断
}
void  CAN2_SEND(uint16_t ID)
{
		
	uint8_t i;
	CanTxMsg TxMessage;

	TxMessage.StdId=ID;	//标准标识符为0x00
  TxMessage.ExtId=0x0000; //扩展标识符0x0000
	TxMessage.IDE = CAN_ID_STD;//使用标准标识符
	TxMessage.RTR = CAN_RTR_DATA; /* 设置为数据帧 */
	TxMessage.DLC = 8;            /* 数据长度, can报文规定最大的数据长度为8字节 */
//	CAN2_DATA[0]=Data;
	for(i = 0;i < 8; i ++)
	{
		TxMessage.Data[i] = CAN2_DATA[i];//can_send_data[i];
	}
   
	CAN_Transmit(CAN2,&TxMessage);  // 返回这个信息请求发送的邮箱号0,1,2或没有邮箱申请发送no_box
}

//CAN2中断程序
void CAN2_RX0_IRQHandler(void)
{
uint8_t i;
	CanRxMsg RxMessage; 
/*	RxMessage.StdId=0x00;
  RxMessage.ExtId=0x00;
  RxMessage.IDE=0;
  RxMessage.DLC=0;
  RxMessage.FMI=0;
  RxMessage.Data[0]=0x00;  */   
	
	CAN_Receive(CAN2,CAN_FIFO0, &RxMessage); // 使用FIFO 0作为接收缓冲器，接收CAN总线上发到CAN1上的数据
	
	if(RxMessage.StdId==CAN1ID)		
	{
	for (i=0;i<8;i++)   CAN2_Rec[i]=RxMessage.Data[i];     	
	CAN2_Rec_Flag = 1;//判断是否有CAN1发送过来的
	}
	CAN_ClearITPendingBit(CAN2,CAN_IT_FMP0);  // 清除挂起中断 
}

