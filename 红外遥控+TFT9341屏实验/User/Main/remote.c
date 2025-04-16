/******************************************************************
**	  NUAA_CS_107Kit 4.1������
**	  HS0038B�������
**
**	  ��    ̳��bbs.openmcu.com
**	  ��    ����www.openmcu.com
**	  ��    �䣺support@openmcu.com
**
**    ��    ����V1.1
**	  ��    �ߣ�openmcu
**	  �������:	2014.3.14
********************************************************************/
#include "remote.h"
#include "delay.h"
#include "stm32f10x_conf.h"
#include "hw_config.h"

u32 Remote_Odr=0;  	    //�����ݴ洦  
u8  Remote_Cnt=0;       //��������,�˴ΰ��¼��Ĵ��� 
u8  Remote_Rdy=0;       //������յ�����


/*********************************************
*�������ƣ�u8 Pulse_Width_Check(void)
*
*��ڲ�������
*
*���ڲ�����t����������Ϊt*20us(t=1~250)
*
*����˵�������������	�������Ϊ5ms
*  		   
**********************************************/	
u8 Pulse_Width_Check(void)
{
    u8 t=0;	 
    while(RDATA)
    {	 
		t++;Delay_us(20);					 
        if(t>=250)return t;     //��ʱ���
    }
    return t;
}

/*********************************************
*�������ƣ�void EXTI0_IRQHandler(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����������մ���
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
		    if(RDATA)                     //�и��������	 
		    {
		        res=Pulse_Width_Check();  //��ô˴θ�������
				
				       
		        if(res>=250)break;         //�������ź� 
		        if((res>=200)&&(res<250)) OK=1; //���ǰ��λ(4.5ms) 
		        else if(res>=85&&res<200)  //����������һ(2ms) 
		        {  							    		 
		            Remote_Rdy=1;          //���յ�����  
		            Remote_Cnt++;          //������������	
		            break;
		        }
		        else if((res>=50)&&(res<85)) RODATA=1;     // 1.5ms 
		        else if((res>=10)&&(res<50)) RODATA=0;     // 500us 
		        if(OK)
		        {
		            Remote_Odr<<=1;
		            Remote_Odr+=RODATA; 
		            Remote_Cnt=0;                     // ������������ 
		        }   
		    }			 						 
		} 	 	    
		EXTI_ClearITPendingBit(EXTI_Line0);			  //�����־λ
	}	
}


/*********************************************
*�������ƣ�u8 Remote_Process(void)
*
*��ڲ�������
*
*���ڲ��������ؼ�ֵ
*
*����˵��������������
*  		   
**********************************************/	
u8 Remote_Process(void)
{               
    u8 t1,t2;   
    t1=Remote_Odr>>24;                   //�õ���ַ�� 	
    t2=(Remote_Odr>>16)&0xff;            //�õ���ַ����
    Remote_Rdy=0;                        //������ 	      
    if((t1==(u8)~t2)&&(t1==REMOTE_ID))       //����ң��ʶ����(ID)����ַ 
    { 
        t1=Remote_Odr>>8;
        t2=Remote_Odr; 	
        if(t1==(u8)~t2)return t1;        //�����ֵ    
    }     
    return 0;
}					  
