#ifndef __REMOTE_H
#define __REMOTE_H
#include "stm32f10x_conf.h" 

#define RDATA GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)

/**	红外遥控识别码(ID),每款遥控器的该值基本都不一样,但也有一样的.
	我们选用的遥控器识别码为0 **/
#define REMOTE_ID 0

extern u8 Remote_Cnt;         /*按键次数,此次按下键的次数 */
extern u8 Remote_Rdy;         /*红外接收到数据 */
extern u32 Remote_Odr;        /*命令暂存处	 */
void Remote_Init(void);       /*红外传感器接收头引脚初始化 */
u8 Remote_Process(void);      /*红外接收到数据处理 */
u8 Pulse_Width_Check(void);   /*检查脉宽   */
void delay_init(u8 SYSCLK);

#endif
