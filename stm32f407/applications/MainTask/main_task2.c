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
//static char macid[MACID_STRING_LENTH+1];//����MAC�ַ���
//#define DEVNAME_LEGHT						100 
//unsigned char deverName[DEVNAME_LEGHT];//�����豸Name���ַ���
//SystemStatus_t systemStatus;		//ϵͳ״̬����


//SystemFun systemFun=NULL;

////�ֲ�����
//static void systemOffLineFun(void);//ϵͳ����
//static void systemErrorFun(void);//ϵͳ����

//static void updataRoadGateStatus(void);
////�����һ�εĶ�ά�룬��Ƶ��Ϣ����������Ϣ
//static void 	clearLastStatus(unsigned char isBreak)	 ;

//static void changeSystemFun(SystemFun fun);
//static void waitProcessStart(void);//void //�ȴ���������
//static void waitCarIntoFun(void);//�ȴ������ϳ�
//static void waitServerPass(void);//�ȴ�����
//static void waitCarDepartureFun(void);//�ȴ������뿪

//static unsigned int  checkSystemStatus(void);
//static unsigned char cheackInfraredStatus(void);

//static void saveRQCode(void);
//void testPrintr();


//#define LIMIT_EXIST_CAR				120
//#define LIMIT_NO_EXIST_CAR		60

//void initMainTask(void)
//{
//    rt_memset((unsigned char *)&systemStatus,0,sizeof(SystemStatus_t));
//    //��ʼ��MAC��ַ��MAC�ַ���
//    rt_memset(macid,0,sizeof(macid));
//		get_mac_addr(systemStatus.macid);
//    rt_sprintf(macid,"%02x%02x%02x%02x%02x%02x",
//                systemStatus.macid[0],systemStatus.macid[1],systemStatus.macid[2],
//                systemStatus.macid[3],systemStatus.macid[4],systemStatus.macid[5]);
//    systemStatus.macidstring = macid;
//		//��ʼ��Ӳ��������汾
//		systemStatus.hw=DEV_FW;
//		systemStatus.fw=DEV_HW;
//		//��ʼ���豸����.
//		//systemStatus.devType=DEV_TYPE;
//		readDevType();
//		//��ʼ���豸����
//		//saveDevName("YJ_02SL02B");//Ĭ��
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
//	else if((Get_Mqtt_Link_Status() != MQTT_LINKING)) 	//ϵͳ���ߣ��л�����״̬
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
//static void systemErrorFun(void)//ϵͳ����
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
//static void systemOffLineFun(void)//ϵͳ����
//{

////	//������������/LED��������/��Ļ��ʾ����/·բ��//	systemStatus.devStatus.speakerStatus=SPK_OFF_LINE;
////	systemStatus.devStatus.ledStatus=LED_OFF_LINE;
////	//-------------------
////	systemStatus.devStatus.roadGate=OPEN;
////	setSoundPayFun(payOffLine);//��ʾϵͳ����
//	systemStatus.hciStatus=HCI_SYS_OFF_LINE;
//	if((rt_tick_get()%3000)<200)
//	{
//		rt_kprintf("------system off line fun-------------\r\n");
//	}
//}

//static void waitProcessStart(void)//void //�ȴ���������
//{
//	//��Ļ��ʾ����	//LED��ʾ����	//���Ȳ���
////	systemStatus.devStatus.roadGate=OPEN;//·բ��
////	systemStatus.devStatus.ledStatus=LED_FREE;
////		setSoundPayFun(NULL);//�������� 
//	systemStatus.hciStatus=HCI_SYS_FREE;
//	//------------------------------
//	if(systemStatus.processID!=0)
//	{
//		//��ʾĳĳ����ʼ����
//		setSoundPayFun(payStartWork);
//		rt_thread_delay(500);//ȷ����ʾ��Ϣ����
//		//����ɱ�־λ
//		clearLsatFreshFlag();
//		if(!systemStatus.processType)		
//		{
//			//�ɷ�������������̣�������λ��ɨ�룬�������
//			//���豸��������̣������Ѿ�ɨ����ˣ�����ɨ����
//			rt_memset(systemStatus.rqCode,0,MAX_RQ_CODE_BUF);
//		}
//		changeSystemStatus(SPEAKER_BUYS);
//		rt_kprintf("processID!=0,change system status\r\n");
//	}
//	if((weighing.pcb_rq_fresh)||(weighing.rq_code_fresh))
//		//��ʱ������û�з����Ŷ���Ϣ���������µĶ�ά�����
//		//˵����û����ƽ̨��¼��ֱ���ܹ��������ˡ�
//		//��ʱͨ���û�����ɨ���ά�봥���������̡�
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
//static void waitCarIntoFun(void)//�ȴ������ϳ�-��������
//{
//	unsigned char infraredStatus=0;
////	//LED��ʾæµ
////	systemStatus.devStatus.ledStatus=LED_BE_BUSY1;
////	//·բ��-------------------
////	systemStatus.devStatus.roadGate=OPEN;
//	
//	if((rt_tick_get()%3000)<200)
//	{
//		rt_kprintf("------wait car weight fun-------------\r\n");
//	}
//	//-------------����Ƿ��յ����������͵ĸ�λָ��-------------------
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
//			//����ɱ�־λ
//			clearLsatFreshFlag();
//			rt_thread_mdelay(100);
//			rt_kprintf("systemStatus.monitSta!=0 \r\n");
//			return;
//	} 
//	if(weighing.weight_value<LIMIT_EXIST_CAR)
//	{
//			//LED��ʾ"***���ϰ�����"��
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
//			//����δ��λ�����ƶ�"
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
//	if((rt_tick_get()-systemStatus.arrivalTime)<5000)//�ȶ���λ����10�룬�ſ�ʼ��ʾ����ά���������
//	{
//			systemStatus.devStatus.ledStatus=LED_BE_BUSY1;
//			setSoundPayFun(payStartWork);	//��ʼ����
//			//����ɱ�־λ
//			clearLsatFreshFlag();
//			return;
//	}
//	//��ȡ��ά��
//	if((systemStatus.rqCode[0]==0)&&(weighing.rq_code_fresh==0)&&(weighing.pcb_rq_fresh==0))
//	{
//		//������ʱ����"��ɨ���ά��"
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
//	systemStatus.devStatus.roadGate=CLOSE;//ɨ�赽��ά�룬��ʼ��
//	setSoundPayFun(payWork4);							//������ʱ����"��ȴ����ؽ���
//	//��ȡ����
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
////		//rt_thread_mdelay(1000);//�ȴ�1��
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
//		rt_thread_mdelay(3000);//�ȴ�3��
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
//		//ƽ̨�޸��˳��ơ����²���һ��
//		setSoundPayFun(payWork5);
//		rt_thread_mdelay(3000);//�ȴ�3��
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
//static void waitCarDepartureFun(void)//�ȴ������뿪
//{
//	static int lastTime=0;
//	//LED��ʾæµ
//	systemStatus.devStatus.ledStatus=LED_BE_BUSY2;
//	//�������������뿪
//	setSoundPayFun(payWork6);
//	//·բ��
//	systemStatus.devStatus.roadGate=OPEN;
//	systemStatus.irStatus=cheackInfrared(); //
//	if((systemStatus.irStatus==0)&&(weighing.weight_value<LIMIT_NO_EXIST_CAR))//�ȴ�������0 �� ������0
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
//	if((weighing.weight_value<LIMIT_NO_EXIST_CAR)&&(systemStatus.irStatus==0)&&((rt_tick_get()-lastTime)>=5000))//����5S�޳�
//	{
//		//�����뿪
//		rt_thread_mdelay(3000);//�ȴ�3��
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
//			case SYSTME_FREE:	//����
//				clearLastStatus(1);
//				changeSystemFun(waitProcessStart);
//				break;
//			case SPEAKER_BUYS:	//æµ
//				changeSystemFun(waitCarIntoFun);
//				break;
//			case SPEAKER_WAIT_END:	//�ȴ�����
//				changeSystemFun(waitServerPass);
//				break;
//			case SPEAKER_END_ING:	//�ȴ�����
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
////�޸�systemFun
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
//		//������
//	systemStatus.irStatus=cheackInfrared();
//	if(systemStatus.irStatus==0)//||(weighing.weight_value<1000))//û��⵽�κκ��⣬˵��û�����ϳ�//����С��1000��Ҳ��Ϊ����û��λ
//	{
//			return 0;
//	}
//	if((!systemStatus.disableIr)&&(systemStatus.irStatus&0x01))//����λ�ò���
//	{	
//			return 1;
//	}
//	if((!systemStatus.disableIr)&&(systemStatus.irStatus&0x04))//����λ�ó�ǰ
//	{
//		return 4;
//	}
//	return systemStatus.irStatus;
//}


////�����һ�εĶ�ά�룬��Ƶ��Ϣ����������Ϣ
////isBreak�Ƿ�������л���ֻ���¿�ʼ����
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
////��Flash�ж����豸����
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
////���豸���Ʊ��浽Flash��
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
////��Flash�ж����豸����
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
////���豸���Ʊ��浽Flash��
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
