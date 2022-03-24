#include "rq_code_rec.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "uart_util.h"
#include "string.h"
#include "system_params.h"
/*波特率*/
#define RQ_CODE_UART_RATE 9600 

static rt_device_t rqccode_serial;


int Rqcode_Wirte(unsigned char* data,unsigned int len)
{
    return Uart1_Wirte(data,len);
}
int rqRXhandel(unsigned char *buff,unsigned short len)
{
		#define RQ_RX_TIME	200
		UART_RX_MSG * rxMsg= getUartStatus(UART1_INDEX);
		if(rxMsg->rxTime==0)
		{
			return RT_ERROR;
		}
		if(rxMsg->rx_len==0)
		{
			clearRXMsg(UART1_INDEX);
			return RT_ERROR;
		}
		if((rt_tick_get()-rxMsg->rxTime)<RQ_RX_TIME)//RQ_RX_TIME 毫秒未收到数据
		{
			return RT_ERROR;	
		}
//		if((rxMsg->rx_buffer[0]!='A')&&(rxMsg->rx_buffer[1]=='A'))
//		{
//			if(strchr(rxMsg->rx_buffer,'{')==NULL)
//			{
//				clearRXMsg(UART1_INDEX);
//				return RT_ERROR;
//			}
//			if(strchr(rxMsg->rx_buffer,'}')==NULL)
//			{
//				clearRXMsg(UART1_INDEX);
//				return RT_ERROR;
//			}
//		}
		if(rxMsg->rx_len>200)
		{
			clearRXMsg(UART1_INDEX);
			return RT_ERROR;
		}
		rt_memset(buff,0,len);
			//888888888888888888888888888888888888888888888
		if((rxMsg->rx_buffer[0]=='A')&&(rxMsg->rx_buffer[1]=='A')&&(rxMsg->rx_buffer[2]=='A'))//AAA开头代表配置指令
		{
			 setParamWithRQData((unsigned char *)rxMsg->rx_buffer);
		}
		else
		{
			char * index1=strchr((char *)rxMsg->rx_buffer,'{');
			char * index2=strchr((char *)rxMsg->rx_buffer,'}');
			if((index1==NULL)||(index2==NULL)||(index1>index2)||((index2-index1)<10))
			{
				clearRXMsg(UART1_INDEX);
				return RT_ERROR;
			}
			rt_memcpy(buff,index1,index2-index1+2);
		}
		//888888888888888888888888888888888888888888888
//		if(rxMsg->rx_len<len)
//		{
//			rt_memcpy(buff,rxMsg->rx_buffer,rxMsg->rx_len);
//		}
//		else
//		{
//			rt_memcpy(buff,rxMsg->rx_buffer,len);
//		}
//		
		
		rt_kprintf("rx rqbuff:%s\r\n",buff);
		clearRXMsg(UART1_INDEX);
		return RT_EOK;	
}
 

int Rqcode_Init(void)
{
    return Uart1_Init(RQ_CODE_UART_RATE);
    return RT_ERROR;
}


