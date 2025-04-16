/************************************************
*  ���ֲ����㷨ʵ��
*************************************************/

#include "stdlib.h"
#define A 10       //Aֵ�ɸ���ʵ���������
#include "stm32f10x_conf.h"
#include "hw_config.h"

/**********************��λֵ�˲�****************/
//#define N 11       //Nֵ�ɸ���ʵ���������
#define N 12
#define a   0.25
u16 value;        //valueΪ����ֵ
u16 value_buf[N];
u8 i=0;
int sum=0;
u8 const jq[N]={1,2,3,4,5,6,7,8,9,10,11,12};//��Ȩϵ����
u8 const sum_jq=1+2+3+4+5+6+7+8+9+10+11+12;

int filter_ZWZ(void)
{
u16 count,i,j,temp;
for (count=0;count<N;count++)
{
value_buf[count]=Read_ADC1_MultiChannel(ADC_Channel_3);   //��ȡ����ֵ
//delay();
}
for (j=0;j<N-1;j++)    //����ֵ��С�������У��������ð�ݷ�
{
for (i=0;i<N-j;i++)
{
if(value_buf[i]>value_buf[i+1])
{
temp=value_buf[i];
value_buf[i]=value_buf[i+1];
value_buf[i+1]=temp;
}
}
}
return value_buf[(N-1)/2];    //ȡ�м�ֵ
}
/*****************����ƽ���˲�******************/
int filter_SSPJ()
{
int sum=0,count=0;
for(count=0;count<N;count++)
{
sum+=Read_ADC1_MultiChannel(ADC_Channel_3);
//delay();
}
return (u16)(sum/N);
}

/****************ȥ��ֵƽ���˲� ************************/
int filter_QJZQPJ()
{
u16 count,i,j,temp;
for (count=0;count<N;count++)
{
value_buf[count]=Read_ADC1_MultiChannel(ADC_Channel_3);     //��ȡ����ֵ
//delay();
}
for (j=0;j<N-1;j++)   //����ֵ��С�������У��������ð�ݷ�
{
for (i=0;i<N-j;i++)
{
if(value_buf[i]>value_buf[i+1])
{
temp=value_buf[i];
value_buf[i]=value_buf[i+1];
value_buf[i+1]=temp;
}
}
}
sum=0;
for(count=1;count<(N-1);count++)     /*ȥ����һ����ĩһ����*/
{
sum+=value_buf[count];
//delay();
}
return (u16)(sum/(N-2));
}
/***************�ƶ�ƽ���˲�������ƽ���˲���*************/
int filter_YDPJ()
{
u8 count;
int sum=0;
value_buf[i++]=Read_ADC1_MultiChannel(ADC_Channel_3);
if(i==N) i=0;
for (count=0;count<N;count++)
sum+=value_buf[count];
return (u16)(sum/N);
}

/*************��Ȩƽ���˲�**********************/
int filter_JQPJ()
{
u8 count;
int sum=0;
for (count=0;count<N;count++)
{value_buf[count]=Read_ADC1_MultiChannel(ADC_Channel_3);     //��ȡ����ֵ
//delay();
}
for (count=0;count<N;count++)
sum+=value_buf[count]*jq[count];
return (u16)(sum/sum_jq);
}

/**********************��ͨ�˲�**********************/
int filter_DT(void)
{
u16 new_value;    //new valueΪ��ǰ����ֵ
new_value=Read_ADC1_MultiChannel(ADC_Channel_3);
return (a*new_value+(1-a)*value);
}
