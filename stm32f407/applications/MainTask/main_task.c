#include "main_task.h"
#include <string.h>
#include <rtthread.h>
#include <mqtt_thread.h>
#include "easyflash.h"
#include "infrared.h"
#include "bt.h"
#include "weigh.h"
#include "led_com.h"
#include "pole.h"
#include "sound.h"
#include "printer.h"
#include "uart_thread.h"
#include "weigh_thread.h"  
#include "rtdbg.h"  
#include "cmd_handle.h"  
#include "system_params.h"

extern rt_err_t get_mac_addr(unsigned char *mac);

SystemStatus_t systemStatus;		//ϵͳ״̬����


SystemFun systemFun=NULL;

//�ֲ�����
static void systemOffLineFun(void);//ϵͳ����
static void systemErrorFun(void);//ϵͳ����

//�����һ�εĶ�ά�룬��Ƶ��Ϣ����������Ϣ
static void clearLastStatus(unsigned char isBreak)	 ;

static void changeSystemFun(SystemFun fun);
static void waitProcessStart(void);//void //�ȴ���������
static void waitCarIntoFun(void);//�ȴ������ϳ�
static void waitServerPass(void);//�ȴ�����
static void waitCarDepartureFun(void);//�ȴ������뿪

static unsigned int  checkSystemStatus(void);
static unsigned char cheackInfraredStatus(void);
static 	void	clearCards(void);
static void saveRQCode(void);

static void checkSysStatusTimeOut(void);//���״̬�Ƿ�ʱ
#define LIMIT_EXIST_CAR				99
#define LIMIT_NO_EXIST_CAR		30

void initMainTask(void)
{
		unsigned char i=0;

    //��ʼ��MAC��ַ��MAC�ַ���
    rt_memset(getMACArray(),0,MACID_STRING_LENTH+1);
		get_mac_addr(systemStatus.macid);
    rt_sprintf(getMACArray(),"%02x%02x%02x%02x%02x%02x",
                systemStatus.macid[0],systemStatus.macid[1],systemStatus.macid[2],
                systemStatus.macid[3],systemStatus.macid[4],systemStatus.macid[5]);
    systemStatus.macidstring = getMACArray();
		//��ʼ��Ӳ��������汾
		systemStatus.hw=DEV_FW;
		systemStatus.fw=DEV_HW;
		//------------------------------
		for(i=0;i<MAX_CARD_COUNT;i++)
		{
			systemStatus.cards[i]=systemStatus.card[i];
		}
		systemStatus.hciStatus=HCI_INIT;
		systemStatus.sevType=TYPE_INIT;
    systemStatus.status=SYSTME_OFF_LINE;
		
		changeSystemStatus(SYSTME_FREE);
}
 
void mainTaskRun(void)
{
	if(checkSystemStatus()!=RT_EOK)
	{
		systemErrorFun();
	}
	else if((Get_Mqtt_Link_Status() != MQTT_LINKING)) 	//ϵͳ���ߣ��л�����״̬
	{
		systemOffLineFun(); 
	}
	else
	{
		//-------------����Ƿ��յ����������͵ĸ�λָ��-------------------
		if(systemStatus.breakType==1)
		{
			if(systemStatus.isPrint)
			{
				printReceiptInfo();
			}
			changeSystemStatus(SPEAKER_END_ING);
			systemStatus.breakType=0;
		} 
		else if(systemStatus.breakType==2)
		{
			clearLastStatus(0);
			changeSystemStatus(SPEAKER_BUYS1);
			systemStatus.breakType=0;
		}
		//--------------------------------------------------------------------------
		if(systemFun!=NULL)
		{
			systemFun();
		}
		checkSysStatusTimeOut();
	} 
}

static unsigned int checkSystemStatus(void)
{
	unsigned int res= RT_EOK;
	//---------------------------------
	if(systemStatus.check_rm)
	{
		if(weighing.warning_offline)
		{
			systemStatus.monitSta=5;
			systemStatus.monitCH=0;
			res=RT_ERROR;
		}
		else
		{
			if(weighing.warning!=0)
			{	
				switch(weighing.warningType)
				{
					case 0:
						systemStatus.monitSta|=0x01;
						systemStatus.monitCH=weighing.warningCh;
						break;
					case 1:
						systemStatus.monitSta|=0x02;
						systemStatus.monitCH=weighing.warningCh;
						break;
					case 2:
						systemStatus.monitSta=5;
						systemStatus.monitCH=0;
						break;
				}
				systemStatus.hciStatus=HCI_REMOTE_CHEAK_ERROR;
			}
			else
			{
				systemStatus.monitSta=0;
				systemStatus.monitCH=0;
			}
		}
	}
	else
	{
			systemStatus.monitSta=0;
			systemStatus.monitCH=0;
	}
	if(systemStatus.check_rm&&weighing.warning_offline)
	{
		res=RT_ERROR;
	}
	if((systemStatus.sevType==TYPE_FEED)&&(weighing.radioFrequency_offline))
	{
		res=RT_ERROR;
	}
	if(weighing.pcb_offline||weighing.weigh_offline||weighing.vlpr_offline||weighing.pcb_vlpr_offline)
	{
		res=RT_ERROR;
	}
	if((rt_tick_get()%3000)<200)
	{
		if(systemStatus.check_rm&&weighing.warning_offline)
		{
			LOG_E("warning offline\r\n");
		}
		if(weighing.pcb_offline)
		{
			LOG_E("pcb offline\r\n");
		}
		if(weighing.weigh_offline)
		{
			LOG_E("weight offline\r\n");
		}
		if(weighing.vlpr_offline)
		{
			LOG_E("vlpr offline\r\n");
		}
		if(weighing.pcb_vlpr_offline)
		{
			LOG_E("vlpr pcb vlpr offline\r\n");
		}
		if((systemStatus.sevType==TYPE_FEED)&&(weighing.radioFrequency_offline))
		{
			LOG_E("radio Frequency offline\r\n");
		}
	}
	return res;

}
static void systemErrorFun(void)//ϵͳ����
{
	//---------------------------------
	if((rt_tick_get()%3000)<200)
	{
		rt_kprintf("------system error fun-------------\r\n");
	}
	if(systemStatus.check_rm&&weighing.warning_offline)
	{
			systemStatus.hciStatus=HCI_WARNING_OFFLINE;
	}
	else if(weighing.pcb_offline)
	{
			systemStatus.hciStatus=HCI_PCB_OFFLINE;
	}
	else  if(weighing.weigh_offline)
	{
			systemStatus.hciStatus=HCI_WEIGH_OFFLINE;
	}
	else if(weighing.vlpr_offline)
	{
			systemStatus.hciStatus=HCI_VLPR_OFFLINE1;
	}
	else if(weighing.pcb_vlpr_offline)
	{
			systemStatus.hciStatus=HCI_VLPR_OFFLINE2;
	}
	else if((systemStatus.sevType==TYPE_FEED)&&weighing.radioFrequency_offline)
	{
			systemStatus.hciStatus=HCI_RADIOFREQUENCY_OFFLINE;
	}
}
//-----------------------------------------------------------------------------
static void systemOffLineFun(void)//ϵͳ����
{
	systemStatus.hciStatus=HCI_SYS_OFF_LINE;
	if((rt_tick_get()%3000)<200)
	{
		rt_kprintf("------system off line fun-------------\r\n");
	}
}

static void waitProcessStart(void)//void //�ȴ���������
{
	systemStatus.hciStatus=HCI_SYS_FREE;
	//------------------------------
	if((weighing.pcb_rq_fresh)||(weighing.rq_code_fresh))//��ʱ������û�з����Ŷ���Ϣ���������µĶ�ά�����
	{
		if(weighing.weight_value<LIMIT_EXIST_CAR)
		{
			weighing.pcb_rq_fresh=0;
			weighing.rq_code_fresh=0;
			return;
		}
		saveRQCode();
		systemStatus.isOpenTask=1;
		rt_kprintf("rx rq data,isOpenTask=1:%s\r\n");
	}
	//------------------------------
	if(systemStatus.processID!=0)
	{
		//���֮ǰ���ܴ��ڵĶ�ά�롢��Ƶ��Ϣ
		clearCards();
		if(systemStatus.msgType)
		{
				changeSystemStatus(SPEAKER_BUYS3);
				rt_kprintf("processID!=0 and msgType!=0,change system status\r\n");
				return ;
		}
		//----------------------------------------
		if(!systemStatus.processType)					//���豸��������̣������Ѿ�ɨ����ˣ�����ɨ����
		{
			checkRQ(systemStatus.rqCode,&systemStatus.rqDir);
			rt_memset(systemStatus.rqCode,0,sizeof(systemStatus.rqCode));
		}
		//----------------------------------------
		changeSystemStatus(SPEAKER_BUYS1);
		rt_kprintf("processID!=0,change system status\r\n");
	}
	if((rt_tick_get()%3000)<200)
	{
		rt_kprintf("------wait Process Start fun-------------\r\n");
	}
}
static void waitCarIntoFun(void)//�ȴ������ϳ� 
{
	unsigned char infraredStatus=0;
	if((rt_tick_get()%3000)<200)
	{
		rt_kprintf("------wait car weight fun-------------\r\n");
	}
	//-----------------------------------------------
	checkRQ(systemStatus.rqCode,&systemStatus.rqDir);
	if(systemStatus.rqCode[0]!=0)//ɨ���ά��֮��ȷ���������ݷ���ȥ�������ͷ
	{
		checkVlpr(systemStatus.current_num,systemStatus.rqDir);
	}
	//-----------------------------------------------
	if(systemStatus.monitSta!=0)
	{
			rt_thread_mdelay(100);
			rt_kprintf("systemStatus.monitSta!=0 \r\n");
			systemStatus.hciStatus=HCI_REMOTE_CHEAK_ERROR;
			return;
	} 
	infraredStatus=cheackInfraredStatus();
	if((weighing.weight_value<LIMIT_EXIST_CAR))//��������������û�У���ʾ����ĳĳ���ϳ�
	{
		systemStatus.hciStatus=HCI_START_PROCESS;
		systemStatus.arrivalTime=0;
		rt_thread_mdelay(100);
		rt_kprintf("weighing.weight_value<100.0 \r\n");
		return;
	}
	if((infraredStatus!=2)&&(infraredStatus!=0))
	{
		rt_thread_mdelay(200);
		infraredStatus=cheackInfraredStatus();
		if((infraredStatus!=2)&&(infraredStatus!=0))			//����δ��λ�����ƶ�"
		{
			systemStatus.hciStatus=HCI_NO_IN_PLACE;
			systemStatus.arrivalTime=0;
			rt_kprintf("infraredStatus!=0 \r\n");
			return;
		}
	}
	systemStatus.hciStatus=HCI_START_WEIGHT;
	if(systemStatus.arrivalTime==0)
	{
		systemStatus.arrivalTime=rt_tick_get();
	}
	if((rt_tick_get()-systemStatus.arrivalTime)<10000)//�ȶ���λ����10�룬�ſ�ʼ��ʾ����ά���������
	{
			systemStatus.arrivalTime=0;
			changeSystemStatus(SPEAKER_BUYS2);
			return;
	}
}
static void checkDataFun(void)//��������������
{
	unsigned char infraredStatus=0;
	if((rt_tick_get()%3000)<200)
	{
		rt_kprintf("------wait car weight fun-------------\r\n");
	}
	//-----------------------------------------------
	checkRQ(systemStatus.rqCode,&systemStatus.rqDir);
	if(systemStatus.rqCode[0]!=0)//ɨ���ά��֮��ȷ���������ݷ���ȥ�������ͷ
	{
		checkVlpr(systemStatus.current_num,systemStatus.rqDir);
	}
	//-----------------------------------------------
	if(systemStatus.monitSta!=0)
	{
			rt_thread_mdelay(100);
			rt_kprintf("systemStatus.monitSta!=0 \r\n");
			systemStatus.hciStatus=HCI_REMOTE_CHEAK_ERROR;
			systemStatus.arrivalWeightTime=0;
			return;
	} 
	infraredStatus=cheackInfraredStatus();
	if(((infraredStatus!=2)&&(infraredStatus!=0))||(weighing.weight_value<LIMIT_EXIST_CAR))			//����δ��λ�����ƶ�"
	{
		systemStatus.hciStatus=HCI_NO_IN_PLACE2;
		systemStatus.arrivalTime=0;
		systemStatus.arrivalWeightTime=0;
		rt_kprintf("infraredStatus!=0 \r\n");
		return;
	}
	if(systemStatus.arrivalTime==0)
	{
		systemStatus.arrivalTime=rt_tick_get();
	}
	if((rt_tick_get()-systemStatus.arrivalTime)<2000)//�ȶ���λ����1�룬��ʾ����ά���������
	{
		systemStatus.arrivalWeightTime=0;
			return;
	}
	systemStatus.weight=(int)(weighing.weight_value*10);
		//��ȡ��ά��
	if((systemStatus.rqCode[0]==0)||(systemStatus.rqCode[1]==0))//&&(weighing.rq_code_fresh==0)&&(weighing.pcb_rq_fresh==0)
	{
		//������ʱ����"��ɨ���ά��"
		systemStatus.hciStatus=HCI_SACN_RQ;
		systemStatus.arrivalWeightTime=0;
		rt_thread_mdelay(100);
		rt_kprintf("wait rq_code_fresh \r\n");
		return;
	}
	//��ȡ����
	if((systemStatus.current_num[0]==0)||(systemStatus.current_num[1]==0))
	{
			//������ʱ����"δʶ����"
			systemStatus.hciStatus=HCI_NOT_CARNUM;
			rt_thread_mdelay(100);
			rt_kprintf("wait car num fresh \r\n");
		  if((rt_tick_get()-systemStatus.arrivalTime)<=40000)
			{
					systemStatus.arrivalWeightTime=0;
					return;
			}
			rt_sprintf((char *)systemStatus.current_num,"%s","AAAAAAAA");
	}
	
	//��ȡ������Ϣ
	if(weighing.weigh_fresh==0)
	{
		rt_thread_mdelay(100);
		systemStatus.hciStatus=HCI_WEIGHT_ING;
		rt_kprintf("wait weigh_fresh \r\n");
		return;
	}
	//���������������������,��ֹ�ϴ���������
	if(systemStatus.arrivalWeightTime==0)
	{
		systemStatus.arrivalWeightTime=rt_tick_get();
	}
	if(weighing.weigh_fresh_time<systemStatus.arrivalWeightTime)
	{
		return;
	}
	if((weighing.weigh_fresh_time-systemStatus.arrivalWeightTime)<500)
	{
		return;
	}
	//-------------------------------------
	systemStatus.weight=(int)(weighing.weight_value*10);
	if(systemStatus.weight>=(LIMIT_EXIST_CAR*10))
	{
		systemStatus.isChangeCarNum&=(~0x02);
	//	systemStatus.devStatus.ledStatus=LED_BE_BUSY2;
		systemStatus.hciStatus=HCI_PLAY_WEIGHT;
		rt_thread_mdelay(10000);//�ȴ�5��
		systemStatus.weight=(int)(weighing.weight_value*10);
		if(systemStatus.weight>=(LIMIT_EXIST_CAR*10))
		{
			systemStatus.shorError=0;
			systemStatus.arrivalTime=0;
			changeSystemStatus(SPEAKER_WAIT_END);
		}
	}
	//��ȡ��Ƶ��Ϣ
	if((systemStatus.sevType==TYPE_FEED)&&(systemStatus.cardNum==0))
	{
		//������ʱ����"δʶ����Ƶ��"
		systemStatus.hciStatus=HCI_NOT_CARD;
		rt_thread_mdelay(100);
		rt_kprintf("wait card fresh \r\n");
		//return;
	}
	return;
}
static void waitServerPass(void)
{
	if((rt_tick_get()%3000)<200)
	{
		rt_kprintf("------wait Server  pass fun-------------\r\n");
	}
	if(checkRQ(systemStatus.rqCode,&systemStatus.rqDir))
		//�������ɨ�赽��ά�룬����Ϊ����һ���������Ƚ��봫�����������
	{
			rt_memset(systemStatus.current_num,0,sizeof(systemStatus.current_num));
			clearCards();
			changeSystemStatus(SPEAKER_BUYS2);
			return;
	}
	checkVlpr(systemStatus.current_num,systemStatus.rqDir);
	//��ȡ��Ƶ��Ϣ
	if((systemStatus.sevType==TYPE_FEED)&&(systemStatus.cardNum==0))
	{
		//������ʱ����"δʶ����Ƶ��"
		systemStatus.hciStatus=HCI_NOT_CARD;
		rt_thread_mdelay(100);
		rt_kprintf("wait card fresh \r\n");
		//return;
	}
	if(systemStatus.isChangeCarNum&0x02)
	{
		//ƽ̨�޸��˳��ơ����²���һ��
		systemStatus.hciStatus=HCI_PLAY_WEIGHT;;
		rt_thread_mdelay(3000);//�ȴ�3��
		systemStatus.isChangeCarNum&=(~0x02);
	}
	else
	{
		if(systemStatus.shorError==1)
		{
			systemStatus.hciStatus=HCI_SHOW_SEV_ERROR;
		}
		else
		{
			systemStatus.hciStatus=HCI_WAIT_END;;
		}
	}
}
static void waitCarDepartureFun(void)//�ȴ������뿪
{
	static int lastTime=0;
	systemStatus.hciStatus=HCI_WAIT_LEAVE;
	systemStatus.irStatus=cheackInfrared(); //
		systemStatus.irStatus=0;
	if((systemStatus.irStatus==0)&&(weighing.weight_value<LIMIT_NO_EXIST_CAR))//�ȴ�������0 �� ������0
	{
		if(lastTime==0)
		{
			lastTime=rt_tick_get();
		}
	}
	else
	{
		lastTime=0;
	}
	if((weighing.weight_value<LIMIT_NO_EXIST_CAR)&&(systemStatus.irStatus==0)&&((rt_tick_get()-lastTime)>=2000))//����5S�޳�
	{
		//�����뿪
		rt_thread_mdelay(1000);//�ȴ�1��
		changeSystemStatus(SYSTME_FREE);
		return;
	}
	if((rt_tick_get()%3000)<200)
	{
		rt_kprintf("------wait Car Departure  fun,weight=%d,irStatus=%d-------------\r\n",weighing.weight_value,systemStatus.irStatus);
	}
}
static void qualityTestingFail(void)//��ʾ�ʼ�δͨ��
{
		systemStatus.hciStatus=QUALITYTESTINGFAIL;
		if((rt_tick_get()-systemStatus.funChangeTime)>=(30*1000))
		{
			clearLastStatus(1);
			changeSystemStatus(SYSTME_FREE); 
		}
		
}
void changeSystemStatus(SystemSta sysStatus)
{
	if(systemStatus.status!=sysStatus)
	{
		systemStatus.status=sysStatus;
		switch(systemStatus.status)
		{
			case SYSTME_FREE:	//����
				clearLastStatus(1);
				changeSystemFun(waitProcessStart);
				break;
			case SPEAKER_BUYS1:	//æµ
				changeSystemFun(waitCarIntoFun);
				break;
			case SPEAKER_BUYS2:
				changeSystemFun(checkDataFun);
				break;
			case SPEAKER_BUYS3:
				changeSystemFun(qualityTestingFail);
				break;
			case SPEAKER_WAIT_END:	//�ȴ�����
				changeSystemFun(waitServerPass);
				break;
			case SPEAKER_END_ING:	//�ȴ�����
				changeSystemFun(waitCarDepartureFun);
				break;
			default:
				break;
		}
	}
} 	 
static void checkSysStatusTimeOut(void)//���״̬�Ƿ�ʱ
{
	if((systemStatus.status==SPEAKER_BUYS1)||(systemStatus.status==SPEAKER_BUYS2))
	{
		if(systemStatus.rqCode[0]!=0)//ɨ���ά��֮ǰ������20���ӣ�����Ϊ�˴γ��أ�����һֱû���ϳ�
		{
			if((rt_tick_get()-systemStatus.funChangeTime)>=(20*60*1000))
			{
					changeSystemStatus(SYSTME_FREE);
					systemStatus.isOutTime=1;
			}
		}
	}
}

//�޸�systemFun
static void changeSystemFun(SystemFun fun)
{
	if(fun!=systemFun)
	{
		systemFun=fun;
		systemStatus.funChangeTime=rt_tick_get();
	}
}
static unsigned char cheackInfraredStatus(void)
{
	unsigned char status=0;
		//������
	status=cheackInfrared();
	if((status==0)||(systemStatus.disableIr))//||(weighing.weight_value<1000))//û��⵽�κκ��⣬˵��û�����ϳ�//����С��1000��Ҳ��Ϊ����û��λ
	{
			systemStatus.irStatus=0;
			return 0;
	}
	if((!systemStatus.disableIr)&&(status&0x01))//����λ�ò���
	{	
			systemStatus.irStatus=1;
			return 1;
	}
	if((!systemStatus.disableIr)&&(status&0x04))//����λ�ó�ǰ
	{
			systemStatus.irStatus=4;
			return 4;
	}
	return systemStatus.irStatus;
}


//�����һ�εĶ�ά�룬��Ƶ��Ϣ����������Ϣ
//isBreak�Ƿ�������л���ֻ���¿�ʼ����
static void clearLastStatus(unsigned char isBreak)
{
	rt_memset(systemStatus.rqCode,0,sizeof(systemStatus.rqCode));
	rt_memset(systemStatus.carNum,0,sizeof(systemStatus.carNum));
	rt_memset(systemStatus.nextNum,0,sizeof(systemStatus.nextNum));
	clearCards();
	rt_memset(&systemStatus.printInfo,0,sizeof(PrintInfo_t));
	systemStatus.weight=0;
	systemStatus.arrivalTime=0;
	systemStatus.breakType=0;
	systemStatus.isPrint=0;	
	
	weighing.weigh_fresh=0;
	weighing.rq_code_fresh=0;
	weighing.pcb_rq_fresh=0;
	weighing.radioFrequency_fresh=0;
	weighing.new_vlpr=0;
	weighing.pcb_vlpr_fresh=0;
	systemStatus.processType=0;
	systemStatus.arrivalWeightTime=0;
	systemStatus.shorError=0;
	if(isBreak)
	{
		systemStatus.processID=0;
		systemStatus.disableIr=0;
		weighing.new_vlpr=0;
		weighing.pcb_vlpr_fresh=0;
		rt_memset(systemStatus.current_num,0,sizeof(systemStatus.current_num));
		rt_memset(systemStatus.informatio,0,sizeof(systemStatus.informatio));
		if(systemStatus.sound==2)
		{
			systemStatus.sound=1;
			saveSound(systemStatus.sound);
		} 
	}
	rt_kprintf("------clear Last Status-------------\r\n");
}
static void saveRQCode(void)
{
	if(weighing.rq_code_fresh)
	{
		systemStatus.rqDir=0;
		rt_memcpy(systemStatus.rqCode,weighing.rq_code_buf,MAX_RQ_CODE_BUF);
		weighing.rq_code_fresh=0;
	}
	else if(weighing.pcb_rq_fresh)
	{
		systemStatus.rqDir=1;
		rt_memcpy(systemStatus.rqCode,weighing.pcb_rq_buf,MAX_RQ_CODE_BUF);
		weighing.pcb_rq_fresh=0;
	}
}
static 	void	clearCards(void)
{
	unsigned char i=0;
	for(i=0;i<MAX_CARD_COUNT;i++)
	{
		rt_memset(systemStatus.cards[i],0,RADIOFREQUENCY_BUFF_LEN);
	}
	systemStatus.cardNum=0;
	systemStatus.cardIndex=0;
}

