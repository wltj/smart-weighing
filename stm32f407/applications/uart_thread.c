#include "uart_thread.h"
#include "uart_util.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "main_task.h"
#include "weigh.h" 
#include "rs485.h"
#include "uart_util.h"
#include "rq_code_rec.h"
#include "printer.h"
#include "pcb_expand.h"
#include "remote_check.h"
#include "cJSON.h"
#include "cJSON_util.h"
#include "vlpr.h"
#include "RadioFrequency.h"
#include "led_com.h"
weighing_t weighing;
static void uart_thread(void *param);
//����豸�Ƿ�����
static void cheackDevOffLine(void);
static void rs485ChannelFun(void);//ͨ��1
static void rs485Channe2Fun(void);//ͨ��2
static void rs485Channe3Fun(void);//ͨ��3
static void rs485Channe4Fun(void);//ͨ��4
static void uart1Fun(void);
int uartThreadInit(void)
{
 
	//485
	//Init_485();//
	//����ʶ��
	vlprInit();
	//��Ƶʶ��
	radioFrequencyInit();
	//��ά��ɨ��������Ƶģ��
	Rqcode_Init();
	//��չ��
	if(EN_EXPAND_PCB)
	{
		pcbExpandInit();
	}
	//��ң��
	temoteCheckInit();
	//��ӡ��
	Print_Init();
	//�ذ�
	Weigh_Init();

	rt_thread_t tid = RT_NULL;
	tid = rt_thread_create( "uart_handel",
													uart_thread, RT_NULL,       
													2048,   
													7, 10 );  
	if (tid)
	{
			return rt_thread_startup(tid);
	}
	return RT_ERROR;

}




 

//-------------------------------------------------
static void uart_thread(void *param)
{

	weighing.weigh_fresh_time= rt_tick_get();
	weighing.pcb_fresh_time= rt_tick_get();
	weighing.warning_fresh_time= rt_tick_get();
	weighing.vlpr_fresh_time= rt_tick_get();
	weighing.radioFrequency_fresh_time= rt_tick_get();
	while(1)
	{	
		rs485ChannelFun();
		rs485Channe3Fun();
		rs485Channe4Fun();
		rs485Channe2Fun();
		uart1Fun();
		cheackDevOffLine();
		//------------------------------------
		rt_thread_mdelay(20);			
	}
}
static void rs485ChannelFun(void)
{
	if(vlprRXHandel(weighing.vlpr_buf,&weighing.new_vlpr)==RT_EOK)
	{
			weighing.vlpr_fresh=1;
			weighing.vlpr_fresh_time= rt_tick_get();
			weighing.vlpr_offline=0;
	}
}


void checkVlpr(unsigned char * buff,unsigned char dir)
{
		
		if((weighing.new_vlpr)&&(weighing.vlpr_offline==0))
		{
			if(dir==0)
			{
				rt_memset(buff,0,VLPR_BUFF_LEN);
				rt_memcpy(buff,weighing.vlpr_buf,VLPR_BUFF_LEN);
			}
			weighing.new_vlpr=0;
		}
		if((weighing.pcb_vlpr_fresh)&&(weighing.pcb_offline==0))
		{
			if(dir==1)
			{
				rt_memset(buff,0,VLPR_BUFF_LEN);
				rt_memcpy(buff,weighing.pcb_vlpr_buf,VLPR_BUFF_LEN);
			}
			weighing.pcb_vlpr_fresh=0;		
		}
}

static unsigned char readRAdioFrequencyStatus=0;
void closeReadAdioFrequency(void)
{
	readRAdioFrequencyStatus=1;
}
void openReadAdioFrequency(void)
{
	readRAdioFrequencyStatus=0;
}
static unsigned char isQueryRadioFrequency=0;
void openQueryRadioFrequency(void)
{
	isQueryRadioFrequency=1;
}
void closeQueryRadioFrequency(void)
{
	isQueryRadioFrequency=0;
	//weighing.new_radioFrequency=0;
}
static void rs485Channe2Fun(void)
{
	static unsigned int lastTime=0;
	if(!readRAdioFrequencyStatus)
	{
		if(radioFrequencyRXHandel()==RT_EOK)
		{
				weighing.radioFrequency_fresh=1;
				weighing.radioFrequency_fresh_time= rt_tick_get();
				weighing.radioFrequency_offline=0;
		}
		if(isQueryRadioFrequency)
		{
			if((rt_tick_get()-lastTime)>=1000)	//ÿ��1S��һ��
			{
					sendQueryCardCmd1();
					lastTime=rt_tick_get();
			}
		} 
	}
}

//------------------------------------------------------------------------------------------------------------
static unsigned char communicationStatus=0;//0������ͨ��  1ֻͨ����չ�� 2ֻͨ�ŷ�ң��ģ��
void setCommunicationStatus(unsigned char value)
{
	communicationStatus=value;
}
static void rs485Channe3Fun(void)
{
		static unsigned char status=0,lastStatus=1;//status=0����д��չ�壬status=1����д��ң��

		if(communicationStatus==1)
		{
			openQueryRadioFrequency();
			if(pcbExpandRXHandel(weighing.pcb_rq_buf,MAX_RQ_CODE_BUF,&weighing.pcb_rq_fresh,weighing.pcb_vlpr_buf,VLPR_BUFF_LEN,&weighing.pcb_vlpr_fresh)==RT_EOK)			//��չ����մ���
			{				
				weighing.pcb_fresh=1;
				weighing.pcb_fresh_time= rt_tick_get();
				weighing.pcb_offline=0;
			}
			weighing.warning_fresh_time= rt_tick_get();	
		}
		else if(communicationStatus==2)
		{
			openQueryRadioFrequency();
			//��ң�ؽ��մ���
			if(remoteCheckRXhandel(&weighing.warning,&weighing.warningType,&weighing.warningCh)==RT_EOK)
			{
				weighing.warning_fresh=1;
				weighing.warning_fresh_time= rt_tick_get();	
				weighing.warning_offline=0;
			}
			weighing.pcb_fresh_time= rt_tick_get();
		}
		else if(status==0)
		{
			if(EN_EXPAND_PCB)
			{
				closeQueryRadioFrequency();
				if(pcbExpandRXHandel(weighing.pcb_rq_buf,MAX_RQ_CODE_BUF,&weighing.pcb_rq_fresh,weighing.pcb_vlpr_buf,VLPR_BUFF_LEN,&weighing.pcb_vlpr_fresh)==RT_EOK)				//��չ����մ���
				{				
					weighing.pcb_fresh=1;
					weighing.pcb_fresh_time= rt_tick_get();
					weighing.pcb_offline=0;				
				}
				if(lastStatus!=status)
				{
					queryPcbExpandStatus();			//��ѯ��չ��
				}
			}
			else
			{
				weighing.pcb_fresh=1;
				weighing.pcb_fresh_time= rt_tick_get();
				weighing.pcb_offline=0;	
			}
		}
		else if(status==1)
		{
			openQueryRadioFrequency();
			if(remoteCheckRXhandel(&weighing.warning,&weighing.warningType,&weighing.warningCh)==RT_EOK)
			{
				weighing.warning_fresh=1;
				weighing.warning_fresh_time= rt_tick_get();		
				weighing.warning_offline=0;				
			}
			if(lastStatus!=status)
			{
				queryRemoteCheckStatus();		//��ѯ��ң��
			}
		}
		lastStatus=status;
		if((rt_tick_get()%2000)>1000)
		{
			status=1;
		}
		else
		{
			status=0;
		}
}
//------------------------------------------------------------------------------------------------------------
static void rs485Channe4Fun(void)
{
	//���ؽ��մ���
	if(weightRXhandel(&weighing.weight_value,&weighing.tare_value)==RT_EOK)
	{
		weighing.weigh_fresh=1;
		weighing.weigh_fresh_time= rt_tick_get();
		weighing.weigh_offline=0;
	}		
}

static void uart1Fun(void)
{
	//��ά����մ���
	if(rqRXhandel(weighing.rq_code_buf,sizeof(weighing.rq_code_buf))==RT_EOK)
	{
		if((weighing.rq_code_buf[0]=='A')&&(weighing.rq_code_buf[1]=='A')&&(weighing.rq_code_buf[2]=='A'))
		{
		
				
		}
		else
		{
			weighing.rq_code_fresh=1;
		}
	}	
}
unsigned char checkRQ(unsigned char * buff,unsigned char * dir)
{
		if(weighing.rq_code_fresh)
		{
			rt_memset(buff,0,MAX_RQ_CODE_BUF);
			rt_memcpy(buff,weighing.rq_code_buf,MAX_RQ_CODE_BUF);
			*dir=0;
			weighing.rq_code_fresh=0;
			return 1;
		}
		if((weighing.pcb_rq_fresh)&&(weighing.pcb_offline==0))
		{
			rt_memset(buff,0,MAX_RQ_CODE_BUF);
			rt_memcpy(buff,weighing.pcb_rq_buf,MAX_RQ_CODE_BUF);
			*dir=1;
			weighing.pcb_rq_fresh=0;
			return 1;
		}
		return 0;
}

//����豸�Ƿ�����
static void cheackDevOffLine(void)
{
	unsigned int time=rt_tick_get();
	//------------------------------------
	if((time-weighing.warning_fresh_time)>8000)
	{
			weighing.warning_offline=1;	//	weighing.warning_offline=0;//
	}
	if(((time-weighing.pcb_fresh_time)>8000)&&EN_EXPAND_PCB)
	{
			weighing.pcb_offline=1;	
	}
	if((time-weighing.weigh_fresh_time)>5000)
	{
			weighing.weigh_offline=1;	
	}
	if((time-weighing.vlpr_fresh_time)>71000)//1��10��û��ȡ��Ϣ
	{
			weighing.vlpr_offline=1;	
	}
	if((systemStatus.sevType==TYPE_FEED)&&((time-weighing.radioFrequency_fresh_time)>5000))//
	{
		  weighing.radioFrequency_offline=1;	
	}
}


