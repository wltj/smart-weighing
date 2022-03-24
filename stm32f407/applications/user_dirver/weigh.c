#include "weigh.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "rs485.h"
#include <string.h>
#include "uart_util.h"
#include <math.h>
#include "system_params.h"
#include "main_task.h"
#include "uart_thread.h"
#include "util_filter.h"
/*波特率*/
#define WEIGT_UART_RATE 9600 
#define ARRAY_SIZE(a)                   (sizeof(a)/sizeof(a[0]))
 


int Weigh_Wirte(unsigned char* data,unsigned int len)
{
     return Rs485_Wirte(WEIGH_485_CHANNLE,WEIGH_485_NUM,data,len);
}

//static unsigned char weigh_read_buf[30];
int Get_Weigh_Value(unsigned char *data,double* weight_value,double* tare_value);

void setWeighRead(void)
{
	Rs485_SetRead(WEIGH_485_CHANNLE,WEIGH_485_NUM);
}
#define BUFFLEN 20
int weightBuff[BUFFLEN]={0};
unsigned char buffIndex=0;
unsigned char isInit=0;
double getAverageWeight() {
//		double zhongshu = weightBuff[0];
//		unsigned char  len = 0;
//		unsigned char  count = 0;
//		len = isInit ? BUFFLEN: buffIndex;
//		for (int i = 1; i < len; i++) {
//				if (weightBuff[i] == zhongshu) {
//						count++;
//				} else {
//						count--;
//				}
//				if (count == 0) {
//						count = 1;
//						zhongshu = weightBuff[i];
//				}
//		}
		double ret;
		int value[ARRAY_SIZE(weightBuff)];
		int cnt;
	//	centre_average_filter(weightBuff,&value,BUFFLEN,3);
	  //   majority_fiter(weightBuff,&value,BUFFLEN);
			majority_fiter_2(weightBuff,BUFFLEN, value, &cnt);
			ret = value[0];
		return (double)ret*0.1;
}

int weightRXhandel(double* weighte,double* tare_value)					//称重接收处理
{
		#define WG_RX_TIME	30
	  double weight_value=0;
		UART_RX_MSG * rxMsg= getUartStatus(WEIGH_UART_INDEX);
		if(rxMsg->rxTime==0)
		{
			return RT_ERROR;
		}
		if(rxMsg->rx_len==0)
		{
			clearRXMsg(WEIGH_UART_INDEX);
			return RT_ERROR;
		}
		if((rt_tick_get()-rxMsg->rxTime)<WG_RX_TIME)//WG_RX_TIME 毫秒未收到数据
		{
			return RT_ERROR;	
		}
		if(rxMsg->rx_len < 10)
		{
			return RT_ERROR;	
		}
		unsigned char *weigh_read_buf=(unsigned char *)rxMsg->rx_buffer;
		int len=rxMsg->rx_len;
		int index=0;
		int res=RT_ERROR;
		//int w=0;
		weighing.zhongshu_w = 0;
		for(index=0;index<len;index++)
		{
			if(weigh_read_buf[index]==0x02)
			{
				if((len>=(index+12))||((systemStatus.weightType==MATRIX_XXX)&&(len>=(index+10))))
				{
					  res= Get_Weigh_Value(&weigh_read_buf[index],&weight_value,tare_value);
						if(RT_EOK==res)
						{
							if(weight_value>=120000.0)//如果超过15万KG，则认为是错误数据
							{
								weight_value=0;
							}
							else
							{
								weightBuff[buffIndex++]=(int)(weight_value*10);
								buffIndex%=BUFFLEN;
								if(buffIndex==0)
								{
									isInit=1;
								}
							}
							if(*tare_value>=120000.0)//如果超过15万KG，则认为是错误数据
							{
								*tare_value=0;
							}
						}
						weighing.zhongshu_w = getAverageWeight();
					if((weight_value+20 >= weighing.zhongshu_w) && (weighing.zhongshu_w+20 >= weight_value))
					{
 						*weighte = ((int)weight_value/10)*10;
						
						}
			else
						{
							*weighte = -1;
						}
						break;
				}
			}
			
		} 
		clearRXMsg(WEIGH_UART_INDEX);
		return res;	
}
 

int Weigh_Init(void)
{
    Init_485();
		Rs485_SetRead(WEIGH_485_CHANNLE,WEIGH_485_NUM);
    return RT_EOK;
}

/*
    获取重量和皮重
    data：接收到榜头的数据
    weight_value：解析出的重量
    tare_value：解析出的皮重
*/
int Get_Weigh_Value_IND880(unsigned char *data,double* weight_value,double* tare_value)
{
    int i = 0;
//    unsigned char crc = 0;
    if(data[i] != 0x02)
    {
        rt_kprintf("[Get_Weigh_Value]head error\n");
        return RT_ERROR;
    }
    
    unsigned char stx = data[i++];
    unsigned char swa = data[i++];
    unsigned char swb = data[i++];
    unsigned char swc = data[i++];
    
    unsigned char weight[6],tare[6];
    memcpy(weight,&data[i],6);
    i+=6;
    memcpy(tare,&data[i],6);
    i+=6;
    if(data[i++] != 0x0d)
    {
        rt_kprintf("[Get_Weigh_Value]cr error\n");
        return RT_ERROR;
    }
    
//    crc = data[i];
    
    /* 和校验 */
    
    
    /* 重量计算 */
    /* 低三位代表小数点位置 */
    double point = 10.0 / pow(10.0,(double)(swa&0x07));
    
    unsigned int _weight_value = 0, _tare_value = 0;
    
    for(int i=0;i<6;i++)
    {
				if( weight[5-i]==' ')
				{
					 weight[5-i]='0';
				}
        _weight_value += (weight[5-i]-'0') * pow(10,i);
    }
    
    for(int i=0;i<6;i++)
    {
				if( weight[5-i]==' ')
				{
					 weight[5-i]='0';
				}
        _tare_value += (tare[5-i]-'0') * pow(10,i);
    }
    
//    *weight_value = (double)_weight_value*point;
//    *tare_value = (double)_tare_value*point;
	
    *weight_value = (double)_weight_value*point;
    *tare_value = (double)_tare_value*point;
		weighing.sign=data[2]&0x02;
    return RT_EOK;
}

int Get_Weigh_Value_DS8(unsigned char *data,double* weight_value,double* tare_value)
{
    int i = 0;
//    unsigned char crc = 0;
    if(data[i++] != 0x02)
    {
        rt_kprintf("[Get_Weigh_Value]head error\n");
        return RT_ERROR;
    }
    
    unsigned char sign = data[i++];
    unsigned char w_3 = data[i++] - '0';
    unsigned char w_4 = data[i++] - '0';
    unsigned char w_5 = data[i++] - '0';
    unsigned char w_6 = data[i++] - '0';
    unsigned char w_7 = data[i++] - '0';
    unsigned char w_8 = data[i++] - '0';
    
    unsigned char s_n = data[i++]- '0'; // 小数位
    unsigned char crc_h = data[i++];
    unsigned char crc_l = data[i++];
    
    unsigned char end = data[i++];
    
    if(end != 0x03)
    {
        rt_kprintf("[Get_Weigh_Value]end error\n");
        return RT_ERROR;
    }
    weighing.sign=((sign=='-') || (sign==1));
    *weight_value = (double)w_3*100000 + (double)w_4*10000  + (double)w_5*1000 +
                    (double)w_6*100 + (double)w_7*10 + (double)w_8 ;
    
    *weight_value = *weight_value / pow(10,s_n);
    if(sign=='-')
		{
			*weight_value =-1*(*weight_value);
		}
		
    * tare_value = 0;
    
    return RT_EOK;
}
int Get_Weigh_Value_Matrix(unsigned char *data,double* weight_value,double* tare_value)
{
    int i = 0;
//    unsigned char crc = 0;
    if(data[i++] != 0x02)
    {
        rt_kprintf("[Get_Weigh_Value]head error\n");
        return RT_ERROR;
    }
    
    unsigned char sign = data[i++];
    unsigned char w_3 = data[i++] - '0';
    unsigned char w_4 = data[i++] - '0';
    unsigned char w_5 = data[i++] - '0';
    unsigned char w_6 = data[i++] - '0';
    unsigned char w_7 = data[i++] - '0';
    unsigned char w_8 = data[i++] - '0';
    unsigned char w_9 = data[i++] - '0';
 
    
    unsigned char end = data[i++];
    
    if((end != 0x0D))
    {
        rt_kprintf("[Get_Weigh_Value]end error\n");
        return RT_ERROR;
    }
    
    *weight_value = (double)w_3*1000000 + (double)w_4*100000  + (double)w_5*10000 +
                    (double)w_6*1000 + (double)w_7*100 + (double)w_8*10 + (double)w_9;
     
    if(sign=='-')
		{
			*weight_value =-1*(*weight_value);
		}
		
    * tare_value = 0;
    
    return RT_EOK;
}
int Get_Weigh_Value_Xkxxx(unsigned char *data,double* weight_value,double* tare_value)
{
	 int i = 0;
		if(getUartStatus(WEIGH_UART_INDEX)->rx_len<18)
		{
				rt_kprintf("[Get_Weigh_Value]end error\n");
				return RT_ERROR;
		}
		
	 if((data[0]<0x43)||(data[0]>0x5a))
	 {
		 		rt_kprintf("[Get_Weigh_Value]end error\n");
				return RT_ERROR;
	 }
	  if(data[2]!=0x2c)
	 {
		 		rt_kprintf("[Get_Weigh_Value]end error\n");
				return RT_ERROR;
	 }
	  if((data[3]!=0x47)||(data[4]!=0x53))// 47 53 毛重  4E 54 净重 54 52 皮重
	 {
		 		rt_kprintf("[Get_Weigh_Value]end error\n");
				return RT_ERROR;
	 }
	 if(data[5]!=0x2c)
	 {
		 		rt_kprintf("[Get_Weigh_Value]end error\n");
				return RT_ERROR;
	 } 
	if((data[16]!=0x0d)||(data[17]!=0x0a))
	 {
		 		rt_kprintf("[Get_Weigh_Value]end error\n");
				return RT_ERROR;
	 }
	 if(data[6]==0x2d)//负数
	 { 
		 weight_value=0;
		 tare_value=0;
		 	return RT_EOK;
	 }
	 if(data[6]!=0x2b)//既不是正，又不是负数 
	 {
			return RT_ERROR;
	 }
	 unsigned char w_1=0,w_2=0,w_3=0,w_4=0,w_5=0,w_6=0,w_7=0;
	 if(data[7]==0x20)//无小数点
	 {
			w_1=data[8]-'0';
		  w_2=data[9]-'0';
		  w_3=data[10]-'0';
		  w_4=data[11]-'0';
		  w_5=data[12]-'0';
		  w_6=data[13]-'0';
	 }
	 else	//有小数点
	 {
			w_1=data[7]-'0';
		  w_2=data[8]-'0';
		  w_3=data[9]-'0';
		  w_4=data[10]-'0';
		  w_5=data[11]-'0';
		  w_6=data[12]-'0';
		 	w_7=data[12]-'0';
	 } 
	 *weight_value=w_1*100000+w_2*10000.0+w_3*1000.0+w_4*100.0+w_5*10.0+w_6*1.0+w_7/10.0;
	 return RT_EOK;
}
int Get_Weigh_Value(unsigned char *data,double* weight_value,double* tare_value)
{
		switch(systemStatus.weightType)
		{
			case IND_XXX:
				return Get_Weigh_Value_IND880(data,weight_value,tare_value);
			case DS_XXX:
				return Get_Weigh_Value_DS8(data,weight_value,tare_value);
			case MATRIX_XXX:
				return Get_Weigh_Value_Matrix(data,weight_value,tare_value);
			case XK_XXX:
				return Get_Weigh_Value_Xkxxx(data,weight_value,tare_value);
			default:
				return Get_Weigh_Value_IND880(data,weight_value,tare_value);
		}
}
 

