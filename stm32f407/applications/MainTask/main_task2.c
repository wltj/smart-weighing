//#include "main_task.h"
//#include <string.h>
//#include <rtthread.h>
//#include <mqtt_thread.h>
//#include "easyflash.h"
//#include "infrared.h"
//#include "bt.h"
//#include "weigh.h"
//#include "led_com.h"
//#include "pole.h"
//#include "sound.h"
//#include "printer.h"
//#include "uart_thread.h"
//#include "weigh_thread.h"  
//#include "rtdbg.h"  
//#include "cmd_handle.h"  
//extern rt_err_t get_mac_addr(unsigned char *mac);

//#define MACID_STRING_LENTH  12
//static char macid[MACID_STRING_LENTH+1];//保存MAC字符串
//#define DEVNAME_LEGHT						100 
//unsigned char deverName[DEVNAME_LEGHT];//保存设备Name的字符串
//SystemStatus_t systemStatus;		//系统状态参数


//SystemFun systemFun=NULL;

////局部函数
//static void systemOffLineFun(void);//系统离线
//static void systemErrorFun(void);//系统错误

//static void updataRoadGateStatus(void);
////清除上一次的二维码，射频信息、重量等信息
//static void 	clearLastStatus(unsigned char isBreak)	 ;

//static void changeSystemFun(SystemFun fun);
//static void waitProcessStart(void);//void //等待启动流程
//static void waitCarIntoFun(void);//等待汽车上称
//static void waitServerPass(void);//等待放行
//static void waitCarDepartureFun(void);//等待汽车离开

//static unsigned int  checkSystemStatus(void);
//static unsigned char cheackInfraredStatus(void);

//static void saveRQCode(void);
//void testPrintr();


//#define LIMIT_EXIST_CAR				120
//#define LIMIT_NO_EXIST_CAR		60

//void initMainTask(void)
//{
//    rt_memset((unsigned char *)&systemStatus,0,sizeof(SystemStatus_t));
//    //初始化MAC地址和MAC字符串
//    rt_memset(macid,0,sizeof(macid));
//		get_mac_addr(systemStatus.macid);
//    rt_sprintf(macid,"%02x%02x%02x%02x%02x%02x",
//                systemStatus.macid[0],systemStatus.macid[1],systemStatus.macid[2],
//                systemStatus.macid[3],systemStatus.macid[4],systemStatus.macid[5]);
//    systemStatus.macidstring = macid;
//		//初始化硬件、软件版本
//		systemStatus.hw=DEV_FW;
//		systemStatus.fw=DEV_HW;
//		//初始化设备类型.
//		//systemStatus.devType=DEV_TYPE;
//		readDevType();
//		//初始化设备名称
//		//saveDevName("YJ_02SL02B");//默认
//		readDevName();
//		systemStatus.hciStatus=HCI_INIT;
//		//systemStatus.devStatus.roadGate=OPEN;
//    systemStatus.status=SYSTME_OFF_LINE;
//		changeSystemStatus(SYSTME_FREE);
// 
//}
// 
//void mainTaskRun(void)
//{
//	if(checkSystemStatus()!=RT_EOK)
//	{
//		systemErrorFun();
//	}
//	else if((Get_Mqtt_Link_Status() != MQTT_LINKING)) 	//系统离线，切换离线状态
//	{
//		systemOffLineFun(); 
//	}
//	else
//	{
//		if(systemFun!=NULL)
//		{
//			systemFun();
//		}
//	} 
//}
////	static unsigned int lastTime=0;
////		if((rt_tick_get()-lastTime)>10000)
////	{
////		 systemStatus.rqDir=1;
////		testPrintr();
////		lastTime=rt_tick_get();
////	}

//static unsigned int checkSystemStatus(void)
//{
//	unsigned int res= RT_EOK;
//	//---------------------------------
//	if(weighing.warning_offline)
//	{
//		systemStatus.monitSta=5;
//		systemStatus.monitCH=0;
//		res=RT_ERROR;
//	}
//	else
//	{
//		if(weighing.warning!=0)
//		{	
//			switch(weighing.warningType)
//			{
//				case 0:
//					systemStatus.monitSta|=0x01;
//					systemStatus.monitCH=weighing.warningCh;
//					break;
//				case 1:
//					systemStatus.monitSta|=0x02;
//					systemStatus.monitCH=weighing.warningCh;
//					break;
//				case 2:
//					systemStatus.monitSta=5;
//					systemStatus.monitCH=0;
//					break;
//			}
//		}
//		else
//		{
//			systemStatus.monitSta=0;
//			systemStatus.monitCH=0;
//		}
//	}
//	if(weighing.warning_offline||weighing.pcb_offline||weighing.weigh_offline||weighing.vlpr_offline||weighing.radioFrequency_offline)
//	{
//		res=RT_ERROR;
//	}
//	if((rt_tick_get()%3000)<200)
//	{
//		if(weighing.warning_offline)
//		{
//			LOG_E("warning offline\r\n");
//		}
//		if(weighing.pcb_offline)
//		{
//			LOG_E("pcb offline\r\n");
//		}
//		if(weighing.weigh_offline)
//		{
//			LOG_E("weight offline\r\n");
//		}
//		if(weighing.vlpr_offline)
//		{
//			LOG_E("vlpr offline\r\n");
//		}
//		if(weighing.radioFrequency_offline)
//		{
//			LOG_E("radio Frequency offline\r\n");
//		}
//	}
//	return res;

//}
//static void systemErrorFun(void)//系统错误
//{

//	//---------------------------------
//	if((rt_tick_get()%3000)<200)
//	{
//		rt_kprintf("------system error fun-------------\r\n");
//	}
//	if(weighing.warning_offline)
//	{
////		 	systemStatus.devStatus.ledStatus=LED_ERROR1;
////			setSoundPayFun(payERROR1); 
//			systemStatus.hciStatus=HCI_WARNING_OFFLINE;
//	}
//	else if(weighing.pcb_offline)
//	{
////			systemStatus.devStatus.ledStatus=LED_ERROR2;
////			setSoundPayFun(payERROR2); 
//			systemStatus.hciStatus=HCI_PCB_OFFLINE;
//	}
//	else  if(weighing.weigh_offline)
//	{
////			systemStatus.devStatus.ledStatus=LED_ERROR3;
////			setSoundPayFun(payERROR3); 
//			systemStatus.hciStatus=HCI_WEIGH_OFFLINE;
//	}
//	else if(weighing.vlpr_offline)
//	{
////			systemStatus.devStatus.ledStatus=LED_ERROR3;
////			setSoundPayFun(payERROR3); 
//			systemStatus.hciStatus=HCI_VLPR_OFFLINE;
//	}
//	else if(weighing.radioFrequency_offline)
//	{
////			systemStatus.devStatus.ledStatus=LED_ERROR3;
////			setSoundPayFun(payERROR3); 
//			systemStatus.hciStatus=HCI_RADIOFREQUENCY_OFFLINE;
//	}
//}
////-----------------------------------------------------------------------------
//static void systemOffLineFun(void)//系统离线
//{

////	//语音报警离线/LED报警离线/屏幕显示离线/路闸关//	systemStatus.devStatus.speakerStatus=SPK_OFF_LINE;
////	systemStatus.devStatus.ledStatus=LED_OFF_LINE;
////	//-------------------
////	systemStatus.devStatus.roadGate=OPEN;
////	setSoundPayFun(payOffLine);//提示系统离线
//	systemStatus.hciStatus=HCI_SYS_OFF_LINE;
//	if((rt_tick_get()%3000)<200)
//	{
//		rt_kprintf("------system off line fun-------------\r\n");
//	}
//}

//static void waitProcessStart(void)//void //等待启动流程
//{
//	//屏幕显示空闲	//LED显示空闲	//喇叭不叫
////	systemStatus.devStatus.roadGate=OPEN;//路闸开
////	systemStatus.devStatus.ledStatus=LED_FREE;
////		setSoundPayFun(NULL);//不出声音 
//	systemStatus.hciStatus=HCI_SYS_FREE;
//	//------------------------------
//	if(systemStatus.processID!=0)
//	{
//		//提示某某车开始称重
//		setSoundPayFun(payStartWork);
//		rt_thread_delay(500);//确保提示信息播出
//		//清除旧标志位
//		clearLsatFreshFlag();
//		if(!systemStatus.processType)		
//		{
//			//由服务器发起的流程，车辆到位再扫码，所以清除
//			//由设备发起的流程，车辆已经扫码过了，保留扫码结果
//			rt_memset(systemStatus.rqCode,0,MAX_RQ_CODE_BUF);
//		}
//		changeSystemStatus(SPEAKER_BUYS);
//		rt_kprintf("processID!=0,change system status\r\n");
//	}
//	if((weighing.pcb_rq_fresh)||(weighing.rq_code_fresh))
//		//此时服务器没有发送排队信息，但是有新的二维码产生
//		//说明车没有在平台记录，直接跑过来称重了。
//		//此时通过用户主动扫描二维码触发称重流程。
//	{
//		if(weighing.weight_value<LIMIT_EXIST_CAR)
//		{
//			clearLsatFreshFlag();
//			return;
//		}
//		saveRQCode();
//		systemStatus.isOpenTask=1;
//	}
//	if((rt_tick_get()%3000)<200)
//	{
//		rt_kprintf("------wait Process Start fun-------------\r\n");
//	}
//}
//static void waitCarIntoFun(void)//等待汽车上秤-包括称重
//{
//	unsigned char infraredStatus=0;
////	//LED显示忙碌
////	systemStatus.devStatus.ledStatus=LED_BE_BUSY1;
////	//路闸开-------------------
////	systemStatus.devStatus.roadGate=OPEN;
//	
//	if((rt_tick_get()%3000)<200)
//	{
//		rt_kprintf("------wait car weight fun-------------\r\n");
//	}
//	//-------------检测是否收到服务器发送的复位指令-------------------
//	if(systemStatus.breakType==1)
//	{
//		if(systemStatus.isPrint)
//		{
//			printReceiptInfo();
//		}
//		changeSystemStatus(SPEAKER_END_ING);
//	} 
//	else if(systemStatus.breakType==2)
//	{
//		clearLastStatus(0);
//		changeSystemStatus(SPEAKER_BUYS);
//	}
//	//-----------------------------------------------
//	if(systemStatus.monitSta!=0)
//	{
//			//清除旧标志位
//			clearLsatFreshFlag();
//			rt_thread_mdelay(100);
//			rt_kprintf("systemStatus.monitSta!=0 \r\n");
//			return;
//	} 
//	if(weighing.weight_value<LIMIT_EXIST_CAR)
//	{
//			//LED显示"***请上磅称重"，
//		setSoundPayFun(plearOntheScale);
//		systemStatus.arrivalTime=0;
//		rt_thread_mdelay(100);
//		rt_kprintf("weighing.weight_value<100.0 \r\n");
//		return;
//	}
//	infraredStatus=cheackInfraredStatus();
//	if((infraredStatus!=2)&&(infraredStatus!=0))
//	{
//		rt_thread_mdelay(100);
//		infraredStatus=cheackInfraredStatus();
//		if((infraredStatus!=2)&&(infraredStatus!=0))
//		{
//			//车辆未到位，请移动"
//			setSoundPayFun(payWork1);//			 
//			systemStatus.devStatus.ledStatus=LED_BE_BUSY3;
//			systemStatus.arrivalTime=0;
//			rt_kprintf("infraredStatus!=0 \r\n");
//			return;
//		}
//	}
//	if(systemStatus.arrivalTime==0)
//	{
//		systemStatus.arrivalTime=rt_tick_get();
//	}
//	if((rt_tick_get()-systemStatus.arrivalTime)<5000)//稳定到位超过10秒，才开始提示检测二维码和语音等
//	{
//			systemStatus.devStatus.ledStatus=LED_BE_BUSY1;
//			setSoundPayFun(payStartWork);	//开始称重
//			//清除旧标志位
//			clearLsatFreshFlag();
//			return;
//	}
//	//获取二维码
//	if((systemStatus.rqCode[0]==0)&&(weighing.rq_code_fresh==0)&&(weighing.pcb_rq_fresh==0))
//	{
//		//语音定时播报"请扫描二维码"
//		setSoundPayFun(payWork3);					//systemStatus.devStatus.speakerStatus=SPK_WORK3;
//		systemStatus.devStatus.ledStatus=LED_BE_BUSY4;
//		rt_thread_mdelay(100);
//		rt_kprintf("wait rq_code_fresh \r\n");
//		return;
//	}
//	if(systemStatus.rqCode[0]==0)
//	{
//		saveRQCode(); 
//	}
//	systemStatus.devStatus.roadGate=CLOSE;//扫描到二维码，开始关
//	setSoundPayFun(payWork4);							//语音定时播报"请等待称重结束
//	//获取蓝牙
////	if((weighing.bt_fresh==0)&&(systemStatus.bleTag[0]==0))
////	{
////		Open_BtRead();
////		rt_thread_mdelay(100);
////		rt_kprintf("wait bt_fresh \r\n");
////		return;
////	}
////	Close_BtRead();
////	if(systemStatus.bleTag[0]==0)
////	{
////		weighing.bt_fresh=0;
////		rt_sprintf((char *)systemStatus.bleTag,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
////		weighing.uuid[0],weighing.uuid[1],weighing.uuid[2],weighing.uuid[3],//
////		weighing.uuid[4],weighing.uuid[5],weighing.uuid[6],weighing.uuid[7],//
////		weighing.uuid[8],weighing.uuid[9],weighing.uuid[10],weighing.uuid[11],//
////		weighing.uuid[12],weighing.uuid[13],weighing.uuid[14],weighing.uuid[15]);
////		//rt_memcpy(systemStatus.bleTag,weighing.uuid,sizeof(weighing.uuid));
////		//rt_thread_mdelay(1000);//等待1秒
////	}
////	rt_memset(systemStatus.bleTag,0,sizeof(systemStatus.bleTag));
////	rt_sprintf((char *)systemStatus.bleTag,"%s","123");
//	
//	if(	weighing.weigh_fresh==0)
//	{
//		rt_thread_mdelay(100);
//		rt_kprintf("wait weigh_fresh \r\n");
//		return;
//	}
//	systemStatus.weight=(int)(weighing.weight_value*10);
//	if(systemStatus.weight>=(LIMIT_EXIST_CAR*10))
//	{
//		systemStatus.isChangeCarNum&=(~0x02);
//		systemStatus.devStatus.ledStatus=LED_BE_BUSY2;
//		setSoundPayFun(payWork5);
//		rt_thread_mdelay(3000);//等待3秒
//		changeSystemStatus(SPEAKER_WAIT_END);
//		clearLsatFreshFlag();
//		weighing.weigh_fresh=0;
//	}
//	else
//	{
//		systemStatus.weight=0;
//		weighing.weigh_fresh=0;
//	}
//	return;
//	//Close_WeighRead();
//}
//static void waitServerPass(void)
//{
//	systemStatus.devStatus.roadGate=CLOSE;
//	if((rt_tick_get()%3000)<200)
//	{
//		rt_kprintf("------wait Server  pass fun-------------\r\n");
//	}
//	if(systemStatus.isChangeCarNum&0x02)
//	{
//		//平台修改了车牌。重新播报一次
//		setSoundPayFun(payWork5);
//		rt_thread_mdelay(3000);//等待3秒
//		systemStatus.isChangeCarNum&=(~0x02);
//	}
//	else
//	{
//		setSoundPayFun(payWork4);//	systemStatus.devStatus.speakerStatus=SPK_WORK5;
//	}
//	//------------------------------------------------------------------------------------------
//	if(systemStatus.breakType==1)
//	{
//		if(systemStatus.isPrint)
//		{
//			printReceiptInfo();
//		}
//		rt_kprintf("systemStatus.breakType==1 \r\n");
//		changeSystemStatus(SPEAKER_END_ING);
//	} 
//	else if(systemStatus.breakType==2)
//	{
//		rt_kprintf("systemStatus.breakType==2 \r\n");
//		clearLastStatus(0);
//		changeSystemStatus(SPEAKER_BUYS);
//	}
//}
//static void waitCarDepartureFun(void)//等待汽车离开
//{
//	static int lastTime=0;
//	//LED显示忙碌
//	systemStatus.devStatus.ledStatus=LED_BE_BUSY2;
//	//语音播报，请离开
//	setSoundPayFun(payWork6);
//	//路闸开
//	systemStatus.devStatus.roadGate=OPEN;
//	systemStatus.irStatus=cheackInfrared(); //
//	if((systemStatus.irStatus==0)&&(weighing.weight_value<LIMIT_NO_EXIST_CAR))//等待重量清0 和 红外清0
//	{
//		if(lastTime==0)
//		{
//			lastTime=rt_tick_get();
//		}
//	}
//	else
//	{
//		lastTime=0;
//	}
//	if((weighing.weight_value<LIMIT_NO_EXIST_CAR)&&(systemStatus.irStatus==0)&&((rt_tick_get()-lastTime)>=5000))//连续5S无车
//	{
//		//车已离开
//		rt_thread_mdelay(3000);//等待3秒
//		changeSystemStatus(SYSTME_FREE);
//		return;
//	}
//	if((rt_tick_get()%3000)<200)
//	{
//		rt_kprintf("------wait Car Departure  fun,weight=%d,irStatus=%d-------------\r\n",weighing.weight_value,systemStatus.irStatus);
//	}
//}

//void changeSystemStatus(SystemSta sysStatus)
//{
//	if(systemStatus.status!=sysStatus)
//	{
//		systemStatus.status=sysStatus;
//		switch(systemStatus.status)
//		{
//			case SYSTME_FREE:	//空闲
//				clearLastStatus(1);
//				changeSystemFun(waitProcessStart);
//				break;
//			case SPEAKER_BUYS:	//忙碌
//				changeSystemFun(waitCarIntoFun);
//				break;
//			case SPEAKER_WAIT_END:	//等待放行
//				changeSystemFun(waitServerPass);
//				break;
//			case SPEAKER_END_ING:	//等待结束
//				changeSystemFun(waitCarDepartureFun);
//				break;
//			default:
//				break;
//		}
//		
//	
//	}
//	
//}
////修改systemFun
//static void changeSystemFun(SystemFun fun)
//{
//	if(fun!=systemFun)
//	{
//		systemFun=fun;
//		systemStatus.funChangeTime=rt_tick_get();
//	}
//}
// void clearLsatFreshFlag(void)
//{
// 
//		weighing.weigh_fresh=0;
//		weighing.rq_code_fresh=0;
//		weighing.pcb_rq_fresh=0;
//}
//static unsigned char cheackInfraredStatus(void)
//{
//		//检测红外
//	systemStatus.irStatus=cheackInfrared();
//	if(systemStatus.irStatus==0)//||(weighing.weight_value<1000))//没检测到任何红外，说明没车辆上秤//重量小于1000，也认为车辆没到位
//	{
//			return 0;
//	}
//	if((!systemStatus.disableIr)&&(systemStatus.irStatus&0x01))//车辆位置不够
//	{	
//			return 1;
//	}
//	if((!systemStatus.disableIr)&&(systemStatus.irStatus&0x04))//车辆位置超前
//	{
//		return 4;
//	}
//	return systemStatus.irStatus;
//}


////清除上一次的二维码，射频信息、重量等信息
////isBreak是否清除所有还是只重新开始称重
//static void clearLastStatus(unsigned char isBreak)	// 
//{
//	rt_memset(systemStatus.rqCode,0,sizeof(systemStatus.rqCode));
//	rt_memset(systemStatus.carNum,0,sizeof(systemStatus.carNum));
//	rt_memset(systemStatus.nextNum,0,sizeof(systemStatus.nextNum));
//	rt_memset(systemStatus.current_num,0,sizeof(systemStatus.current_num));
//	rt_memset(systemStatus.card,0,sizeof(systemStatus.card));
//	systemStatus.weight=0;
//	systemStatus.arrivalTime=0;
//	systemStatus.breakType=0;
//	systemStatus.isPrint=0;	 
//	weighing.weigh_fresh=0;
//	weighing.rq_code_fresh=0;
//	weighing.vlpr_fresh=0;
//	weighing.radioFrequency_fresh=0;
//	rt_memset(&systemStatus.printInfo,0,sizeof(PrintInfo_t));
//	if(isBreak)
//	{
//		systemStatus.processID=0;
//		systemStatus.disableIr=0;
//	}
//	rt_kprintf("------clear Last Status-------------\r\n");
//}
//static void saveRQCode(void)
//{
//	if(weighing.rq_code_fresh)
//	{
//		systemStatus.rqDir=0;
//		rt_memcpy(systemStatus.rqCode,weighing.rq_code_buf,MAX_RQ_CODE_BUF);
//	}
//	else if(weighing.pcb_rq_fresh)
//	{
//		systemStatus.rqDir=1;
//		rt_memcpy(systemStatus.rqCode,weighing.pcb_rq_buf,MAX_RQ_CODE_BUF);
//	}
//}

////-----------------------------------------------------------------------------
//#define DEV_NAME_KEY 		"dev_name"
////从Flash中读出设备名称
//unsigned char * readDevName(void)
//{
//		ef_get_env_blob(DEV_NAME_KEY, deverName, DEVNAME_LEGHT, RT_NULL);
//		if(deverName[0]==0)
//		{
//			rt_sprintf((char *)deverName,"%s_%s","ZNGB",macid);
//			saveDevName((char *)deverName);
//		}
//		systemStatus.devName=deverName;
//		return 	systemStatus.devName;
//}
////将设备名称保存到Flash中
//void saveDevName( char *s)
//{
//		int len=rt_strlen(s);
//		if(len>=DEVNAME_LEGHT)
//		{
//			s[DEVNAME_LEGHT]=0;
//		}
//		rt_memcpy(deverName,s,len);
//		deverName[len]=0;
//		systemStatus.devName=deverName;
//		ef_set_env_blob(DEV_NAME_KEY, deverName,DEVNAME_LEGHT);
//}

////-----------------------------------------------------------------------------
//#define DEV_TYPE_KEY 		"dev_type"
////从Flash中读出设备名称
//unsigned char * readDevType(void)
//{
//		ef_get_env_blob(DEV_TYPE_KEY, &systemStatus.devType, 1, RT_NULL);
//		if((systemStatus.devType!=DEV_TYPE1)&&(systemStatus.devType!=DEV_TYPE2))
//		{
//			systemStatus.devType=DEV_TYPE1;
//			saveDevType(systemStatus.devType);
//		} 
//		return &systemStatus.devType;
//}
////将设备名称保存到Flash中
//void saveDevType(unsigned char type)
//{
//		ef_set_env_blob(DEV_TYPE_KEY, &type,1);	
//}
////---------------------------------------------------------------------------
//void testPrintr()
//{
//		rt_memset((char *)&systemStatus.printInfo,0,sizeof(PrintInfo_t));
//		rt_sprintf((char *)systemStatus.printInfo.title,"%s","ABCDADSDASDADASSD");
//		rt_sprintf((char *)systemStatus.printInfo.sn,"%s","12312312");
//		rt_sprintf((char *)systemStatus.printInfo.gn,"%s","12312312");
//		rt_sprintf((char *)systemStatus.printInfo.sp,"%s","asdsadsa");
//		rt_sprintf((char *)systemStatus.printInfo.ru,"%s","sadsasdas");
//		rt_sprintf((char *)systemStatus.printInfo.fu,"%s","adsdasdas");
//		rt_sprintf((char *)systemStatus.printInfo.gw,"%s","asdasd");
//		rt_sprintf((char *)systemStatus.printInfo.tw,"%s","asdasd");
//		rt_sprintf((char *)systemStatus.printInfo.nw,"%s","asdasdas");
//		rt_sprintf((char *)systemStatus.printInfo.gwt,"%s","ABCDADSDASDADASSD");
//		rt_sprintf((char *)systemStatus.printInfo.nwt,"%s","ABCDADSDASDADASSD");
//		rt_sprintf((char *)systemStatus.printInfo.sib,"%s","ABCDADSDASDADASSD"); 
//		rt_sprintf((char *)systemStatus.printInfo.car,"%s","ABCDA"); 
//		printReceiptInfo();
//}
