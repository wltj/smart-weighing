#include "RadioFrequency.h"
#include "rs485.h"
#include "uart_thread.h"
#include "main_task.h"
#define CMD_QUERY_CARD1					0X01				//��ѯCARD 
#define CMD_QUERY_CARD2					0x50				//��ѯCARD 
static void sendRadioFrequencyMsg(unsigned char cmd,unsigned char *data,unsigned char dataLen);
static unsigned int uiCrc16Cal(unsigned char const  * pucY, unsigned char ucX);

void radioFrequencyInit(void)
{
	Init_485();
}
	
#define 	TID_LEN		6
int radioFrequencyRXHandel(void)
{
	#define PE_RX_TIME	100
		UART_RX_MSG * rxMsg= getUartStatus(RADIOFREQUENCY_UART_INDEX);
		if(rxMsg->rxTime==0)
		{
			return RT_ERROR;
		}
		if(rxMsg->rx_len==0)
		{
			clearRXMsg(RADIOFREQUENCY_UART_INDEX);
			return RT_ERROR;
		}
		if((rt_tick_get()-rxMsg->rxTime)<PE_RX_TIME)//PE_RX_TIME ����δ�յ�����
		{
			return RT_ERROR;	
		}
		if(rxMsg->rx_len < (1+rxMsg->rx_buffer[0]))
		{
			clearRXMsg(RADIOFREQUENCY_UART_INDEX);
			return RT_ERROR;	
		}
		unsigned char *read_buf=(unsigned char *)rxMsg->rx_buffer;
 
		//--------����ַ--------
		if((read_buf[1]!=0))//
		{
			clearRXMsg(RADIOFREQUENCY_UART_INDEX);
			return RT_ERROR;	
		} 
		//���CRC
		unsigned short crc=uiCrc16Cal(read_buf,read_buf[1]-2);
		unsigned short crc2=(read_buf[read_buf[1]-1]<<8)|read_buf[read_buf[1]-2];
//		if(crc!=crc2)
//		{
//			clearRXMsg(RADIOFREQUENCY_UART_INDEX);
//			return RT_ERROR;	
//		}
		unsigned char cmd=read_buf[2];
		unsigned char status=read_buf[3];
		unsigned char *dataBuff=read_buf+4;
		unsigned char num=0;
		switch(cmd)
		{
			case CMD_QUERY_CARD1:
				num=dataBuff[0];//��ȡ���ݵ�����
				if((status<=0x04)&&(num!=0))
				{
					 rxCardData(dataBuff);
				}	
				break;
		}
		clearRXMsg(RADIOFREQUENCY_UART_INDEX);
		return RT_EOK;	
}

void rxCardData(unsigned char *dataBuff)
{
		char t_buff[RADIOFREQUENCY_BUFF_LEN]={0}; 
		unsigned int index=0;
		unsigned char num=0;
		//unsigned char isExist=0;
		num=dataBuff[index++];//��ȡ���ݵ�����
		if(num!=0)
		{
			num=num>5?5:num;
			for(int i=0;i<num;i++)
			{
					rt_memset(t_buff,0,sizeof(t_buff));
					int epcLen=dataBuff[index++];//��֡EPC�ĳ���
					if((epcLen<=10)||(epcLen>=15))
					{
						continue;
					}
					if((dataBuff[index]==0)&&(dataBuff[index+1]==0))
					{
						return;
					}
					for(int j=0;j<epcLen;j++)
					{
						rt_sprintf(t_buff+rt_strlen(t_buff),"%02X",dataBuff[index++]);
					}
					rt_kprintf("rx pcb radio Frequency %d:%s\r\n",i,t_buff);
					//�ж���Щepc�����Ƿ��Ѵ���
					if((systemStatus.cardNum>=1)&&(rt_strncmp((char *)systemStatus.cards[0],t_buff,epcLen)==0))
					{
						continue;
					}
					if((systemStatus.cardNum>=2)&&(rt_strncmp((char *)systemStatus.cards[1],t_buff,epcLen)==0))
					{
						continue;
					}
					if((systemStatus.cardNum>=3)&&(rt_strncmp((char *)systemStatus.cards[2],t_buff,epcLen)==0))
					{
						continue;
					}
					if((systemStatus.cardNum>=4)&&(rt_strncmp((char *)systemStatus.cards[3],t_buff,epcLen)==0))
					{
						continue;
					}
					if((systemStatus.cardNum>=5)&&(rt_strncmp((char *)systemStatus.cards[4],t_buff,epcLen)==0))
					{
						continue;
					}
					//-------------------------------------------------------------------------------------------
					unsigned char isHas=0;
					for(int j=0;j< systemStatus.cardNum;j++)
					{
						if(rt_strncmp((char *)systemStatus.cards[j],t_buff,epcLen)==0)
						{
							 isHas=1;
							 break;
						}
					}
					if(!isHas)
					{
						rt_memset(systemStatus.cards[systemStatus.cardIndex],0,RADIOFREQUENCY_BUFF_LEN);
						rt_memcpy(systemStatus.cards[systemStatus.cardIndex],t_buff,RADIOFREQUENCY_BUFF_LEN);
						systemStatus.cardIndex++;
						systemStatus.cardIndex%=MAX_CARD_COUNT;
						if(systemStatus.cardNum<MAX_CARD_COUNT)
						{
							systemStatus.cardNum++;
						}
					}
					isHas=0;
			}
		}	
		 
}
void sendQueryCardCmd1(void)
{
//	unsigned char data[2]={0};
//	data[0]=0x00;
//	data[1]=TID_LEN;
//	sendRadioFrequencyMsg(CMD_QUERY_CARD1,data,sizeof(data));
	
	sendRadioFrequencyMsg(CMD_QUERY_CARD1,NULL,0);
	
}
//void sendQueryCardCmd2(void)
//{
//	sendRadioFrequencyMsg(CMD_QUERY_CARD2,NULL,0);
//}

/**
*������Ϣ
*streamNum����ˮ��
*data������
*len:���ݳ���
**/
static void sendRadioFrequencyMsg(unsigned char cmd,unsigned char *data,unsigned char dataLen)
{
	unsigned char index=0;
	unsigned char buff[30]; 
	//----����---------
	buff[index++]=4+dataLen;
	//----��ַ---------
	buff[index++]=0x00;
	//----ָ��-------
	buff[index++]=cmd;
	//����
	if((data!=NULL)&&(dataLen!=0))
	{
		unsigned short i=0;
		for(i=0;i<dataLen;i++)
		{
			buff[index++]=data[i];
		}
	}
	unsigned int crc=0;
	crc=uiCrc16Cal(buff,index);
	buff[index++]=crc&0xff;
	buff[index++]=(crc>>8)&0xff;
	//ִ�з��ͺ���
	Rs485_Wirte(RADIOFREQUENCY_485_CHANNLE,RADIOFREQUENCY_485_NUM,buff,index);
	Rs485_SetRead(RADIOFREQUENCY_485_CHANNLE,RADIOFREQUENCY_485_NUM);
}



#define PRESET_VALUE 0xFFFF
#define POLYNOMIAL  0x8408
static unsigned int uiCrc16Cal(unsigned char const  * pucY, unsigned char ucX)
{
	unsigned char ucI,ucJ;
	unsigned short int  uiCrcValue = PRESET_VALUE;

	for(ucI = 0; ucI < ucX; ucI++)
	{
		 uiCrcValue = uiCrcValue ^ *(pucY + ucI);
		 for(ucJ = 0; ucJ < 8; ucJ++)
		 {
				if(uiCrcValue & 0x0001)
				{
					uiCrcValue = (uiCrcValue >> 1) ^ POLYNOMIAL;
				}
				else
				{
					uiCrcValue = (uiCrcValue >> 1);
				}
		 }
 	}
	return uiCrcValue;
}
