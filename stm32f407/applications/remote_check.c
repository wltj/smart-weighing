#include "remote_check.h"
#include "rs485.h"
#include "uart_util.h"

static void sendRemoteCheckCmd(unsigned char cmd); 
int remoteCheckRXhandel(unsigned char *pWarning,unsigned char *pWarningType,unsigned short * pWarningCh)
{
	//WEIGH_UART_INDEX
		#define RC_RX_TIME	100
		UART_RX_MSG * rxMsg= getUartStatus(REMOTE_CHECK_UART_INDEX);
		if(rxMsg->rxTime==0)
		{
			return RT_ERROR;
		}
		if(rxMsg->rx_len==0)
		{
			clearRXMsg(REMOTE_CHECK_UART_INDEX);
			return RT_ERROR;
		}
		if((rt_tick_get()-rxMsg->rxTime)<RC_RX_TIME)//RC_RX_TIME 毫秒未收到数据
		{
			return RT_ERROR;	
		}
		if(rxMsg->rx_len < 4)
		{
			clearRXMsg(REMOTE_CHECK_UART_INDEX);
			return RT_ERROR;	
		}
		unsigned char *weigh_read_buf=(unsigned char *)rxMsg->rx_buffer;
		int dataLen=rxMsg->rx_len;
		if(weigh_read_buf[0]!=0x3A)
		{
			clearRXMsg(REMOTE_CHECK_UART_INDEX);
			return RT_ERROR;	
		}
		if(dataLen<weigh_read_buf[1])
		{
			clearRXMsg(REMOTE_CHECK_UART_INDEX);
			return RT_ERROR;	
		}
		if(weigh_read_buf[2]!=0x81)
		{
			clearRXMsg(REMOTE_CHECK_UART_INDEX);
			return RT_ERROR;	
		}
	
		if((weigh_read_buf[3]==1)&&(weigh_read_buf[4]==1))//白名单
		{
			resetRemoteCheckState();
		}
		else
		{
			*pWarning=weigh_read_buf[3];
			if(weigh_read_buf[3]==1)
			{
				*pWarningType=weigh_read_buf[4];
				*pWarningCh=((weigh_read_buf[5]<<8)|weigh_read_buf[6])*10+weigh_read_buf[7];
			}
		}
	  clearRXMsg(REMOTE_CHECK_UART_INDEX);
		return RT_EOK;	
}

void queryRemoteCheckStatus(void)
{
	sendRemoteCheckCmd(0x01);
}

void resetRemoteCheckState(void)
{
	sendRemoteCheckCmd(0x02);
}
void temoteCheckInit(void)
{
	Init_485();
}
static void sendRemoteCheckCmd(unsigned char cmd)
{
	unsigned char buff[4]={0};
	unsigned char i=0;
	buff[i++]=0x3A;
	buff[i++]=0x04;
	buff[i++]=cmd;
	buff[i++]=orCheck(buff+1,2);
	Rs485_Wirte(REMOTECHECK_485_CHANNLE,REMOTECHECK_485_NUM,buff,i);
	Rs485_SetRead(REMOTECHECK_485_CHANNLE,REMOTECHECK_485_NUM);
}


