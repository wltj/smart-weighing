#include "pcb_expand.h"
#include "rs485.h"
#include "uart_thread.h"
#include "RadioFrequency.h"
#include "system_params.h"
#include "string.h"
#define CMD_QUERY				0X01			//查询指令
#define CMD_LED					0X02			//LED
#define CMD_PRINTH			0X03			//打印
#define CMD_ANSWER			0X80			//应答
#define CMD_ANSWER1			0X81			//应答
#define CMD_ANSWER2			0X82			//应答
#define CMD_ANSWER3			0X83			//应答车牌
static void sendPcbExpandCmd(unsigned char cmd,unsigned char *data,unsigned char len);
int pcbExpandRXHandel(unsigned char * rqCodeBuf,unsigned int len,unsigned char * rq_updata_flag,unsigned char * vlprBuf,unsigned int vlprBuf_len,unsigned char * vlpr_updata_flag)
{
		#define PE_RX_TIME	100
		UART_RX_MSG * rxMsg= getUartStatus(PCBEXPAND_UART_INDEX);
		char * index1=0;
		char * index2=0;
		if(rxMsg->rxTime==0)
		{
			return RT_ERROR;
		}
		if(rxMsg->rx_len==0)
		{
			clearRXMsg(PCBEXPAND_UART_INDEX);
			return RT_ERROR;
		}
		if((rt_tick_get()-rxMsg->rxTime)<PE_RX_TIME)//PE_RX_TIME 毫秒未收到数据
		{
			return RT_ERROR;	
		}
		if(rxMsg->rx_len < 4)
		{
			clearRXMsg(PCBEXPAND_UART_INDEX);
			return RT_ERROR;	
		}
		unsigned char *read_buf=(unsigned char *)rxMsg->rx_buffer;
		 
		if(read_buf[0]!=0x3b)
		{
			clearRXMsg(PCBEXPAND_UART_INDEX);
			return RT_ERROR;	
		}
		if(rxMsg->rx_len<read_buf[1])
		{
			clearRXMsg(PCBEXPAND_UART_INDEX);
			return RT_ERROR;	
		}
		int dLen=0;
		switch(read_buf[2])
		{
			case CMD_ANSWER:
				weighing.pcb_vlpr_offline=0;
				break;
			case CMD_ANSWER1://收到二维码数据
				weighing.pcb_vlpr_offline=0;
				dLen=read_buf[1]-5;
				if(dLen!=0)
				{
					if((read_buf[4]=='A')&&(read_buf[5]=='A')&&(read_buf[6]=='A'))
					{
							setParamWithRQData(&read_buf[4]);
					}
					else
					{
						//888888888888888888888888888
						rt_memset(rqCodeBuf,0,len);
						//rt_memcpy(rqCodeBuf,read_buf+4,dLen<len?dLen:len);
						index1=strchr((char *)(&read_buf[4]),'{');
						index2=strchr((char *)(&read_buf[4]),'}');
						if((index1==NULL)||(index2==NULL)||(index1>index2)||((index2-index1)<10))
						{
							clearRXMsg(PCBEXPAND_UART_INDEX);
							return RT_ERROR;
						}
						rt_memcpy(rqCodeBuf,index1,index2-index1+2);
						rt_kprintf("rx pcb rq:%s\r\n",rqCodeBuf);
						*rq_updata_flag=1;
					}
				}
				break;
			case CMD_ANSWER2://收到车牌数据
				dLen=read_buf[1]-5;
				if(dLen!=0)
				{
					if(read_buf[3]=='1')
					{
						for(int i=0;i<12;i++)
						{
							if(read_buf[4+i]==0)
							{
								break;
							}
						}
						rt_memset(vlprBuf,0,vlprBuf_len);
						rt_memcpy(vlprBuf,read_buf+4,dLen<(vlprBuf_len-1)?dLen:(vlprBuf_len-1));
						rt_kprintf("rx pcb vlpr:%s\r\n",vlprBuf);
						*vlpr_updata_flag=1;
						weighing.pcb_vlpr_offline=0;
					}
					else
					{//摄像头离线
						weighing.pcb_vlpr_offline=1;
					}
				}
				break;
			case CMD_ANSWER3://收到射频信息
				weighing.pcb_vlpr_offline=0;
//				if(dLen!=0)
//				{
						if(read_buf[3]=='1')
						{
							rxCardData(read_buf+4);
						}
//				}
				break;
		}
		clearRXMsg(PCBEXPAND_UART_INDEX);
		return RT_EOK;	
}

void queryPcbExpandStatus(void)
{
	sendPcbExpandCmd(CMD_QUERY,NULL,0);
}

void sendLedInfo(unsigned char *buff,unsigned char len)
{
	sendPcbExpandCmd(CMD_LED,buff,len);			
}
void sendPrinthInfo(unsigned char *buff,unsigned char len)
{
	sendPcbExpandCmd(CMD_PRINTH,buff,len);		
}

void pcbExpandInit(void)
{
		Init_485();
}

static void sendPcbExpandCmd(unsigned char cmd,unsigned char *data,unsigned char len)
{
	unsigned char buff[256]={0};
	unsigned char i=0,j=0;
	unsigned char check;
	buff[i++]=0x3B;
	buff[i++]=0x04;
	buff[i++]=cmd;	
	for(j=0;j<len;j++)
	{
		buff[i++]=data[j];
	}
	buff[1]=i+1;
	check=orCheck(buff+1,i-1);
	buff[i++]=check;
	Rs485_Wirte(PCBEXPAND_485_CHANNLE,PCBEXPAND_485_NUM,buff,i);
	Rs485_SetRead(PCBEXPAND_485_CHANNLE,PCBEXPAND_485_NUM);
}
