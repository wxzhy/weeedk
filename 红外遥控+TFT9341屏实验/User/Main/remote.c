/******************************************************************
**	  NUAA_CS_107Kit 4.1开发板
**	  HS0038B红外解码
**
**	  论    坛：bbs.openmcu.com
**	  旺    宝：www.openmcu.com
**	  邮    箱：support@openmcu.com
**
**    版    本：V1.1
**	  作    者：openmcu
**	  完成日期:	2014.3.14
********************************************************************/
#include "remote.h"
#include "delay.h"
#include "stm32f10x_conf.h"
#include "hw_config.h"

u32 Remote_Odr=0;  	    //命令暂存处  
u8  Remote_Cnt=0;       //按键次数,此次按下键的次数 
u8  Remote_Rdy=0;       //红外接收到数据


/*********************************************
*函数名称：u8 Pulse_Width_Check(void)
*
*入口参数：无
*
*出口参数：t，代表脉宽为t*20us(t=1~250)
*
*功能说明：检测脉冲宽度	，最长脉宽为5ms
*  		   
**********************************************/	
u8 Pulse_Width_Check(void)
{
    u8 t=0;	 
    while(RDATA)
    {	 
		t++;Delay_us(20);					 
        if(t>=250)return t;     //超时溢出
    }
    return t;
}

/*********************************************
*函数名称：void EXTI0_IRQHandler(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：红外接收触发
*  		   
**********************************************/	
void EXTI0_IRQHandler(void)
{
	u8 res=0;
    u8 OK=0; 
    u8 RODATA=0;


	if(EXTI_GetITStatus(EXTI_Line0))
	{
		while(1)
		{        
		    if(RDATA)                     //有高脉冲出现	 
		    {
		        res=Pulse_Width_Check();  //获得此次高脉冲宽度
				
				       
		        if(res>=250)break;         //非有用信号 
		        if((res>=200)&&(res<250)) OK=1; //获得前导位(4.5ms) 
		        else if(res>=85&&res<200)  //按键次数加一(2ms) 
		        {  							    		 
		            Remote_Rdy=1;          //接收到数据  
		            Remote_Cnt++;          //按键次数增加	
		            break;
		        }
		        else if((res>=50)&&(res<85)) RODATA=1;     // 1.5ms 
		        else if((res>=10)&&(res<50)) RODATA=0;     // 500us 
		        if(OK)
		        {
		            Remote_Odr<<=1;
		            Remote_Odr+=RODATA; 
		            Remote_Cnt=0;                     // 按键次数清零 
		        }   
		    }			 						 
		} 	 	    
		EXTI_ClearITPendingBit(EXTI_Line0);			  //清除标志位
	}	
}


/*********************************************
*函数名称：u8 Remote_Process(void)
*
*入口参数：无
*
*出口参数：返回键值
*
*功能说明：处理红外键盘
*  		   
**********************************************/	
u8 Remote_Process(void)
{               
    u8 t1,t2;   
    t1=Remote_Odr>>24;                   //得到地址码 	
    t2=(Remote_Odr>>16)&0xff;            //得到地址反码
    Remote_Rdy=0;                        //清除标记 	      
    if((t1==(u8)~t2)&&(t1==REMOTE_ID))       //检验遥控识别码(ID)及地址 
    { 
        t1=Remote_Odr>>8;
        t2=Remote_Odr; 	
        if(t1==(u8)~t2)return t1;        //处理键值    
    }     
    return 0;
}					  
