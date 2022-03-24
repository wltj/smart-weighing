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

SystemStatus_t systemStatus;		//系统状态参数


SystemFun systemFun=NULL;

//局部函数
static void systemOffLineFun(void);//系统离线
static void systemErrorFun(void);//系统错误

//清除上一次的二维码，射频信息、重量等信息
static void clearLastStatus(unsigned char isBreak)	 ;

static void changeSystemFun(SystemFun fun);
static void waitProcessStart(void);//void //等待启动流程
static void waitCarIntoFun(void);//等待汽车上称
static void waitServerPass(void);//等待放行
static void waitCarDepartureFun(void);//等待汽车离开

static unsigned int  checkSystemStatus(void);
static unsigned char cheackInfraredStatus(void);
static 	void	clearCards(void);
static void saveRQCode(void);

static void checkSysStatusTimeOut(void);//检测状态是否超时
#define LIMIT_EXIST_CAR				99
#define LIMIT_NO_EXIST_CAR		30

void initMainTask(void)
{
		unsigned char i=0;

    //初始化MAC地址和MAC字符串
    rt_memset(getMACArray(),0,MACID_STRING_LENTH+1);
		get_mac_addr(systemStatus.macid);
    rt_sprintf(getMACArray(),"%02x%02x%02x%02x%02x%02x",
                systemStatus.macid[0],systemStatus.macid[1],systemStatus.macid[2],
                systemStatus.macid[3],systemStatus.macid[4],systemStatus.macid[5]);
    systemStatus.macidstring = getMACArray();
		//初始化硬件、软件版本
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
	else if((Get_Mqtt_Link_Status() != MQTT_LINKING)) 	//系统离线，切换离线状态
	{
		systemOffLineFun(); 
	}
	else
	{
		//-------------检测是否收到服务器发送的复位指令-------------------
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
static void systemErrorFun(void)//系统错误
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
static void systemOffLineFun(void)//系统离线
{
	systemStatus.hciStatus=HCI_SYS_OFF_LINE;
	if((rt_tick_get()%3000)<200)
	{
		rt_kprintf("------system off line fun-------------\r\n");
	}
}

static void waitProcessStart(void)//void //等待启动流程
{
	systemStatus.hciStatus=HCI_SYS_FREE;
	//------------------------------
	if((weighing.pcb_rq_fresh)||(weighing.rq_code_fresh))//此时服务器没有发送排队信息，但是有新的二维码产生
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
		//清除之前可能存在的二维码、射频信息
		clearCards();
		if(systemStatus.msgType)
		{
				changeSystemStatus(SPEAKER_BUYS3);
				rt_kprintf("processID!=0 and msgType!=0,change system status\r\n");
				return ;
		}
		//----------------------------------------
		if(!systemStatus.processType)					//由设备发起的流程，车辆已经扫码过了，保留扫码结果
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
static void waitCarIntoFun(void)//等待汽车上秤 
{
	unsigned char infraredStatus=0;
	if((rt_tick_get()%3000)<200)
	{
		rt_kprintf("------wait car weight fun-------------\r\n");
	}
	//-----------------------------------------------
	checkRQ(systemStatus.rqCode,&systemStatus.rqDir);
	if(systemStatus.rqCode[0]!=0)//扫描二维码之后，确定方向。依据方向去检测摄像头
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
	if((weighing.weight_value<LIMIT_EXIST_CAR))//重量不到，红外没有，提示：请某某车上秤
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
		if((infraredStatus!=2)&&(infraredStatus!=0))			//车辆未到位，请移动"
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
	if((rt_tick_get()-systemStatus.arrivalTime)<10000)//稳定到位超过10秒，才开始提示检测二维码和语音等
	{
			systemStatus.arrivalTime=0;
			changeSystemStatus(SPEAKER_BUYS2);
			return;
	}
}
static void checkDataFun(void)//检测各传感器数据
{
	unsigned char infraredStatus=0;
	if((rt_tick_get()%3000)<200)
	{
		rt_kprintf("------wait car weight fun-------------\r\n");
	}
	//-----------------------------------------------
	checkRQ(systemStatus.rqCode,&systemStatus.rqDir);
	if(systemStatus.rqCode[0]!=0)//扫描二维码之后，确定方向。依据方向去检测摄像头
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
	if(((infraredStatus!=2)&&(infraredStatus!=0))||(weighing.weight_value<LIMIT_EXIST_CAR))			//车辆未到位，请移动"
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
	if((rt_tick_get()-systemStatus.arrivalTime)<2000)//稳定到位超过1秒，提示检测二维码和语音等
	{
		systemStatus.arrivalWeightTime=0;
			return;
	}
	systemStatus.weight=(int)(weighing.weight_value*10);
		//获取二维码
	if((systemStatus.rqCode[0]==0)||(systemStatus.rqCode[1]==0))//&&(weighing.rq_code_fresh==0)&&(weighing.pcb_rq_fresh==0)
	{
		//语音定时播报"请扫描二维码"
		systemStatus.hciStatus=HCI_SACN_RQ;
		systemStatus.arrivalWeightTime=0;
		rt_thread_mdelay(100);
		rt_kprintf("wait rq_code_fresh \r\n");
		return;
	}
	//获取车牌
	if((systemStatus.current_num[0]==0)||(systemStatus.current_num[1]==0))
	{
			//语音定时播报"未识别车牌"
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
	
	//获取重量信息
	if(weighing.weigh_fresh==0)
	{
		rt_thread_mdelay(100);
		systemStatus.hciStatus=HCI_WEIGHT_ING;
		rt_kprintf("wait weigh_fresh \r\n");
		return;
	}
	//满足条件后，重量必须更新,防止上传错误数据
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
		rt_thread_mdelay(10000);//等待5秒
		systemStatus.weight=(int)(weighing.weight_value*10);
		if(systemStatus.weight>=(LIMIT_EXIST_CAR*10))
		{
			systemStatus.shorError=0;
			systemStatus.arrivalTime=0;
			changeSystemStatus(SPEAKER_WAIT_END);
		}
	}
	//获取射频信息
	if((systemStatus.sevType==TYPE_FEED)&&(systemStatus.cardNum==0))
	{
		//语音定时播报"未识别射频卡"
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
		//如果重新扫描到二维码，则认为换了一辆车，重先进入传感器检测流程
	{
			rt_memset(systemStatus.current_num,0,sizeof(systemStatus.current_num));
			clearCards();
			changeSystemStatus(SPEAKER_BUYS2);
			return;
	}
	checkVlpr(systemStatus.current_num,systemStatus.rqDir);
	//获取射频信息
	if((systemStatus.sevType==TYPE_FEED)&&(systemStatus.cardNum==0))
	{
		//语音定时播报"未识别射频卡"
		systemStatus.hciStatus=HCI_NOT_CARD;
		rt_thread_mdelay(100);
		rt_kprintf("wait card fresh \r\n");
		//return;
	}
	if(systemStatus.isChangeCarNum&0x02)
	{
		//平台修改了车牌。重新播报一次
		systemStatus.hciStatus=HCI_PLAY_WEIGHT;;
		rt_thread_mdelay(3000);//等待3秒
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
static void waitCarDepartureFun(void)//等待汽车离开
{
	static int lastTime=0;
	systemStatus.hciStatus=HCI_WAIT_LEAVE;
	systemStatus.irStatus=cheackInfrared(); //
		systemStatus.irStatus=0;
	if((systemStatus.irStatus==0)&&(weighing.weight_value<LIMIT_NO_EXIST_CAR))//等待重量清0 和 红外清0
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
	if((weighing.weight_value<LIMIT_NO_EXIST_CAR)&&(systemStatus.irStatus==0)&&((rt_tick_get()-lastTime)>=2000))//连续5S无车
	{
		//车已离开
		rt_thread_mdelay(1000);//等待1秒
		changeSystemStatus(SYSTME_FREE);
		return;
	}
	if((rt_tick_get()%3000)<200)
	{
		rt_kprintf("------wait Car Departure  fun,weight=%d,irStatus=%d-------------\r\n",weighing.weight_value,systemStatus.irStatus);
	}
}
static void qualityTestingFail(void)//提示质检未通过
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
			case SYSTME_FREE:	//空闲
				clearLastStatus(1);
				changeSystemFun(waitProcessStart);
				break;
			case SPEAKER_BUYS1:	//忙碌
				changeSystemFun(waitCarIntoFun);
				break;
			case SPEAKER_BUYS2:
				changeSystemFun(checkDataFun);
				break;
			case SPEAKER_BUYS3:
				changeSystemFun(qualityTestingFail);
				break;
			case SPEAKER_WAIT_END:	//等待放行
				changeSystemFun(waitServerPass);
				break;
			case SPEAKER_END_ING:	//等待结束
				changeSystemFun(waitCarDepartureFun);
				break;
			default:
				break;
		}
	}
} 	 
static void checkSysStatusTimeOut(void)//检测状态是否超时
{
	if((systemStatus.status==SPEAKER_BUYS1)||(systemStatus.status==SPEAKER_BUYS2))
	{
		if(systemStatus.rqCode[0]!=0)//扫描二维码之前，超过20分钟，则认为此次称重，车辆一直没有上称
		{
			if((rt_tick_get()-systemStatus.funChangeTime)>=(20*60*1000))
			{
					changeSystemStatus(SYSTME_FREE);
					systemStatus.isOutTime=1;
			}
		}
	}
}

//修改systemFun
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
		//检测红外
	status=cheackInfrared();
	if((status==0)||(systemStatus.disableIr))//||(weighing.weight_value<1000))//没检测到任何红外，说明没车辆上秤//重量小于1000，也认为车辆没到位
	{
			systemStatus.irStatus=0;
			return 0;
	}
	if((!systemStatus.disableIr)&&(status&0x01))//车辆位置不够
	{	
			systemStatus.irStatus=1;
			return 1;
	}
	if((!systemStatus.disableIr)&&(status&0x04))//车辆位置超前
	{
			systemStatus.irStatus=4;
			return 4;
	}
	return systemStatus.irStatus;
}


//清除上一次的二维码，射频信息、重量等信息
//isBreak是否清除所有还是只重新开始称重
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

