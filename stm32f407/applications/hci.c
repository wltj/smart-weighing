#include "hci.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "uart_util.h"
#include <string.h>
#include "TipsData.h"
#include "main_task.h"
#include "led_com.h"
#include "pole.h"
#include "sound.h"
#include "traffic_light.h"
//----------------------------------------------
static void updataRoadGateStatus(void)
{
	static RoadGateSta  rgLastStatus=RG_INIT;
	if(rgLastStatus!=systemStatus.roadGate)
	{
		if(systemStatus.roadGate==CLOSE)//关闭推杆
		{
			closeRoadGate();
		}
		else if(systemStatus.roadGate==OPEN)//打开推杆
		{
			openRoadGate();
		}
		rgLastStatus=systemStatus.roadGate;
	}
}
//----------------------------------------------
static void  updataTrafficLightStatus(void)
{
	static RoadGateSta  redLastStatus=RG_INIT;
	if(redLastStatus!=systemStatus.redStatus)
	{
		if(systemStatus.redStatus==CLOSE)//关闭推杆
		{
			showPass();
		}
		else if(systemStatus.redStatus==OPEN)//打开推杆
		{
			showProhibit();
		}
		redLastStatus=systemStatus.redStatus;
	}
}
static HCIStatus realityHciStatus=HCI_INIT;
static HCIStatus hciStatus=HCI_INIT;
void updataHIC(void)
{
		 realityHciStatus=HCI_INIT;
	   hciStatus=HCI_INIT;
}
//----------------------------------------------
/* 
  声音播报线程
 */
 static void hci_thread(void *param)
 {
	 unsigned int lastTime=0;
		updataHIC();
    while(1)
    {	
			 if((hciStatus!=systemStatus.hciStatus)||((rt_tick_get()-lastTime)>3000)||(realityHciStatus!=systemStatus.hciStatus))	 
			 {
				 hciStatus=systemStatus.hciStatus;
				 systemStatus.redStatus=OPEN;
				 lastTime=rt_tick_get();
				switch(systemStatus.hciStatus)
				{
					case HCI_INIT:
						break;
					case HCI_SYS_OFF_LINE://系统离线
						setSoundPayFun(payOffLine);
						LEDShowOffLine();
						realityHciStatus=HCI_SYS_OFF_LINE;
						//systemStatus.roadGate=OPEN;
						break;
					case HCI_WARNING_OFFLINE://报警仪离线
						LEDShowError1();
						setSoundPayFun(payERROR1);
						realityHciStatus=HCI_WARNING_OFFLINE;
						//systemStatus.roadGate=OPEN;
						break;			
						case HCI_PCB_OFFLINE://PCB离线
						LEDShowError2();
						setSoundPayFun(payERROR2);
						realityHciStatus=HCI_PCB_OFFLINE;
						//systemStatus.roadGate=OPEN;
						break;
					case HCI_WEIGH_OFFLINE://地磅离线
						LEDShowError3();
						setSoundPayFun(payERROR3);
						realityHciStatus=HCI_WEIGH_OFFLINE;
						//systemStatus.roadGate=OPEN;
						break;
					case HCI_VLPR_OFFLINE1://摄像头离线
						LEDShowError4();
						setSoundPayFun(payERROR4);
						realityHciStatus=HCI_VLPR_OFFLINE1;
						//systemStatus.roadGate=OPEN;
						break;	
					case HCI_VLPR_OFFLINE2://摄像头离线
						LEDShowError6();
						setSoundPayFun(payERROR6);
						realityHciStatus=HCI_VLPR_OFFLINE2;
						//systemStatus.roadGate=OPEN;
						break;	
					case HCI_RADIOFREQUENCY_OFFLINE://射频检测离线
						LEDShowError5();
						setSoundPayFun(payERROR5);
						realityHciStatus=HCI_RADIOFREQUENCY_OFFLINE;
						//systemStatus.roadGate=OPEN;
						break;
					case HCI_REMOTE_CHEAK_ERROR://异常遥控信号
						LEDShowError7();
						setSoundPayFun(payERROR7);
						realityHciStatus=HCI_REMOTE_CHEAK_ERROR;
						break;
					case HCI_SYS_FREE://系统空闲
						LEDShowFree();
						setSoundPayFun(NULL);
						realityHciStatus=HCI_SYS_FREE;
						systemStatus.roadGate=OPEN;
						break;
					case HCI_START_PROCESS://开始流程
						LEDShowWeighedUp(systemStatus.carNum,systemStatus.nextNum);
						setSoundPayFun(plearOntheScale);
						realityHciStatus=HCI_START_PROCESS;
						systemStatus.roadGate=OPEN;	
						break;	
//					case HCI_WAIT_CAR://等待汽车到位
//						LEDShowWeighedUp(systemStatus.carNum,systemStatus.nextNum);
//						setSoundPayFun(payStartWork);
//						realityHciStatus=HCI_WAIT_CAR;
//						break;
					case HCI_NO_IN_PLACE://车辆未到位
						LEDShowWeighedUp3(systemStatus.carNum);
						setSoundPayFun(payWork1);
						realityHciStatus=HCI_NO_IN_PLACE;
						systemStatus.roadGate=OPEN;		
						break;
					case HCI_START_WEIGHT://开始称重
						LEDShowWeighedUp(systemStatus.carNum,systemStatus.nextNum);
						setSoundPayFun(payStartWork);
						realityHciStatus=HCI_START_WEIGHT;
						systemStatus.roadGate=OPEN;		
						break;	
					case HCI_NO_IN_PLACE2://车辆未到位
						LedShowLocationErrorInfo(systemStatus.carNum);
						setSoundPayFun(payWork1);
						realityHciStatus=HCI_NO_IN_PLACE2;
						systemStatus.roadGate=OPEN;
						break;
					case HCI_SACN_RQ://请扫描二维码
						LEDShowWeighedUp4(systemStatus.carNum);
						setSoundPayFun(payWork3);
						realityHciStatus=HCI_SACN_RQ;
						systemStatus.roadGate=OPEN;
						break;	
					case HCI_NOT_CARNUM:
						LEDShowWeighedUp5(systemStatus.carNum);
						setSoundPayFun(payWork7);
						realityHciStatus=HCI_NOT_CARNUM;
						systemStatus.roadGate=CLOSE;
						break;
					case HCI_NOT_CARD:
						LEDShowWeighedUp6(systemStatus.carNum);
						setSoundPayFun(payWork8);
						realityHciStatus=HCI_NOT_CARD;
						systemStatus.roadGate=CLOSE;
						break;
					case HCI_WEIGHT_ING://正在称重
						systemStatus.roadGate=CLOSE;
						LEDShowWeighedUp(systemStatus.carNum,systemStatus.nextNum);
						setSoundPayFun(payWork4);
						realityHciStatus=HCI_WEIGHT_ING;
						break;	
					case HCI_WAIT_END://等待结束
						systemStatus.roadGate=CLOSE;
						LEDShowWeighedUp(systemStatus.carNum,NULL);
						setSoundPayFun(payWork4);
						realityHciStatus=HCI_WAIT_END;
						break;	
					case HCI_PLAY_WEIGHT://播放重量
						systemStatus.roadGate=CLOSE;
						LEDShowWeighedUp2(systemStatus.carNum,systemStatus.weight,systemStatus.rqDir);
						setSoundPayFun(payWork5);
						realityHciStatus=HCI_PLAY_WEIGHT;
						systemStatus.redStatus=CLOSE;
						break;	
					case HCI_WAIT_LEAVE://等待车离开
						LEDShowWeighedUp2(systemStatus.carNum,systemStatus.weight,systemStatus.rqDir);
						setSoundPayFun(payWork6);
						realityHciStatus=HCI_WAIT_LEAVE;
						systemStatus.redStatus=CLOSE;
						systemStatus.roadGate=OPEN;	
						break;		
					case HCI_SHOW_SEV_ERROR:
						systemStatus.roadGate=CLOSE;
						LEDShowSevErrorInfo(systemStatus.carNum,systemStatus.errorInfo);
						setSoundPayFun(paySevERROR);
						realityHciStatus=HCI_SHOW_SEV_ERROR;
						break;
				}
			 }
			 playSound();
			 updataRoadGateStatus();
			 updataTrafficLightStatus();
       rt_thread_delay(100);
    }
 }

int hci_thread_start(void)
{

		poleInit();				//路闸初始化
		trafficLightInit();//红绿灯初始化
		Led_Init();				 //LED+语音
    rt_thread_t tid = RT_NULL;
    tid = rt_thread_create( "soundpay",
                            hci_thread, RT_NULL,      //  
                            1024,          // 
                            12, 10 );     //     
    if (tid)
    {
        rt_thread_startup(tid);
    }
    return RT_EOK;
}
