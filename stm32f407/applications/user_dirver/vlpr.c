#include "vlpr.h"
#include "string.h"
#include "rs485.h"

#define CMD_CAR_INFO						0X01				//������Ϣ
#define CMD_CAR_INFO_CONFIRM		0X02				//������Ϣȷ��
#define CMD_HEARTBEAT						0X03				//����
#define CMD_HEARTBEAT_CONFIRM		0X04				//����ȷ��
#define CMD_POWER_ON						0X07				//�����ϱ�

static unsigned char VFDCalcCRC(unsigned char *data, unsigned int size);
static void sendVlprMsg(unsigned char cmd,unsigned char streamNum,unsigned char *data,unsigned short len);


void vlprInit(void)
{
	Init_485();
	Rs485_SetRead(VLPR_485_CHANNLE,VLPR_485_NUM);
}

int vlprRXHandel(unsigned char * carNumBuff,unsigned char *newVlpr)
{
		#define PE_RX_TIME	100
		UART_RX_MSG * rxMsg= getUartStatus(VLPR_UART_INDEX);
		if(rxMsg->rxTime==0)
		{
			return RT_ERROR;
		}
		if(rxMsg->rx_len==0)
		{
			clearRXMsg(VLPR_UART_INDEX);
			return RT_ERROR;
		}
		if((rt_tick_get()-rxMsg->rxTime)<PE_RX_TIME)//PE_RX_TIME ����δ�յ�����
		{
			return RT_ERROR;	
		}
		if(rxMsg->rx_len < 15)
		{
			clearRXMsg(VLPR_UART_INDEX);
			return RT_ERROR;	
		}
		unsigned char *read_buf=(unsigned char *)rxMsg->rx_buffer;
		//--------���ͷ--------
		if(read_buf[0]!=0x8a)
		{
			clearRXMsg(VLPR_UART_INDEX);
			return RT_ERROR;	
		}
		//--------���Ŀ���ַ--------
		if((read_buf[1]!=0)||(read_buf[2]!=0)||(read_buf[3]!=0)||(read_buf[4]!=0))//
		{
			clearRXMsg(VLPR_UART_INDEX);
			return RT_ERROR;	
		}
		//--------���Դ��ַ--------
		if((read_buf[5]!=0x64)||(read_buf[6]!=0)||(read_buf[7]!=0xf3)||(read_buf[8]!=0x02))
		{
			clearRXMsg(VLPR_UART_INDEX);
			return RT_ERROR;	
		}
		//��ȡ����
		int dataLen=(read_buf[11] | (read_buf[12]<<8));
		if((15+dataLen)>rxMsg->rx_len)
		{
			return RT_ERROR;
		}
		//���֡β
		if(read_buf[14+dataLen]!=0x7F)
		{
			clearRXMsg(VLPR_UART_INDEX);
			return RT_ERROR;
		}
		switch(read_buf[9])
		{
			case CMD_CAR_INFO://������Ϣ
				for(int i=0;i<12;i++)
				{
					if(read_buf[13+i]==0)
					{
						break;
					}
				}
				rt_memset(carNumBuff,0,VLPR_BUFF_LEN);
				rt_memcpy(carNumBuff,&read_buf[13],16);//���Ƴ�16
				sendVlprMsg(CMD_CAR_INFO_CONFIRM,read_buf[10],NULL,0);	
				*newVlpr=1;
				rt_kprintf("rx car num:%s\r\n",carNumBuff);
				break;
			case CMD_POWER_ON://��������---����Ҫ����
				break;
			case CMD_HEARTBEAT://����
				sendVlprMsg(CMD_CAR_INFO_CONFIRM,read_buf[10],read_buf+13,4);	
				break;
			default:
			break;
		}
		clearRXMsg(VLPR_UART_INDEX);
		return RT_EOK;	
}

/**
*������Ϣ
*streamNum����ˮ��
*data������
*len:���ݳ���
**/
static void sendVlprMsg(unsigned char cmd,unsigned char streamNum,unsigned char *data,unsigned short len)
{
	unsigned char index=0;
	unsigned char buff[30];//�������������͵���Ϣ���ᳬ��30�ֽ�,�ӻ����Ͱ����ᳬ��80
	
	//----��Ϣͷ---------
	buff[index++]=0x8A;
	//----���ֽ�Ŀ���ַ64 00 f3 02---------
	buff[index++]=0x64;
	buff[index++]=0x00;
	buff[index++]=0xf3;
	buff[index++]=0x02;
	//----���ֽ�Դ��ַ---------
	buff[index++]=0;
	buff[index++]=0;
	buff[index++]=0;
	buff[index++]=0;
	//----һ�ֽ�Ψһ��ʶ-------
	buff[index++]=cmd;
	//----һ�ֽ���Ϣ֡��ˮ��-------
	buff[index++]=streamNum;
	//----���ֽ����ݳ���-------
	if((data==NULL)||(len==0))
	{
		buff[index++]=0;
		buff[index++]=0;
	}
	else
	{
		buff[index++]=len&0xff;
		buff[index++]=len>>8;
		//----���ֽ�����-------
		unsigned short i=0;
		for(i=0;i<len;i++)
		{
			buff[index++]=data[i];
		}
	}
	//----һ�ֽ�У��λ-------
	buff[index+1]=VFDCalcCRC(buff,index);
	index++;
	//----һ�ֽ�֡β-------
	buff[index++]=0x7f;
	//ִ�з��ͺ���
	Rs485_Wirte(VLPR_485_CHANNLE,VLPR_485_NUM,buff,index);
	Rs485_SetRead(VLPR_485_CHANNLE,VLPR_485_NUM);
}


static unsigned char VFDCalcCRC(unsigned char *data, unsigned int size)
{ 
	unsigned int  i;
	unsigned char crc = 0; 
	for (i = 0; i < size; i++) 
		crc = crc^data[i];
	return crc;
}
