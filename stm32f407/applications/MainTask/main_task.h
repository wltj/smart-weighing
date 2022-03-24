#ifndef __MAIN_TASK_H_
#define __MAIN_TASK_H_

#include "rq_code_rec.h"
#include "vlpr.h"
#include "RadioFrequency.h"
#define DEV_TYPE1								1
#define DEV_TYPE2								2
#define DEV_TYPE								DEV_TYPE1
#define DEV_HW									1
#define DEV_FW									1 



#define BLETAG_MAX_LENGHT				100
#define ERROR_MSG_LENGHT				100
#define AUTO_PROCESSID					0xFFFFFFFF

#pragma pack(1)
typedef void (* SystemFun)(void);//系统方法
//道闸状态
typedef enum  
{ 
		RG_INIT	=		-1,	
		CLOSE=0,
		OPEN=1
}RoadGateSta;
 	
////LED状态
//typedef enum  
//{ 
//		LED_INIT	=		-1,		
//		LED_OFF_LINE	=		0,						//离线 
//		LED_FREE			=		1,						//空闲 
//		LED_BE_BUSY1	=		2,						//忙碌1，重量还没有秤出来
//		LED_BE_BUSY2	=		3,						//忙碌2，重量已经秤出来
//		LED_BE_BUSY3	=		4,						//红外未到位
//		LED_BE_BUSY4	=		5,						//未扫描二维码
//		LED_ERROR1		=		6,
//		LED_ERROR2		=		7,
//		LED_ERROR3		=		8
//}LEDSta;	
typedef enum  
{ 
	TYPE_INIT=-1,
	TYPE_FEED=0,
	TYPE_SLAUGHTER
}SevType;	
//系统状态
typedef enum  
{ 
		 SYSTME_OFF_LINE=		-1,				//离线
		 SYSTME_FREE=				0,				//空闲
		 SPEAKER_BUYS1=			1,				//忙碌1-->车未上磅
		 SPEAKER_BUYS2=			2,				//忙碌2-->车已上磅，
		 SPEAKER_WAIT_END=	3,				//等待结束
		 SPEAKER_END_ING=		4,				//正在离开//	
		 SPEAKER_ERROR=			5,					//系统错误
		 SPEAKER_BUYS3=			6,				//忙碌3-->提示质检未通过
}SystemSta;	 
//typedef struct 
//{
//    RoadGateSta  roadGate;					//道闸状态，1开或者0关
//    LEDSta  		 ledStatus;					//LED状态
//}DevStatus_t;

typedef enum
{	
	HCI_INIT	=	-1,
	HCI_SYS_OFF_LINE=0,									//系统离线
	HCI_WARNING_OFFLINE,							//报警仪离线
	HCI_PCB_OFFLINE,									//PCB离线
	HCI_WEIGH_OFFLINE,								//地磅离线
	HCI_VLPR_OFFLINE1,								//摄像头1离线
	HCI_VLPR_OFFLINE2,								//摄像头2离线
	HCI_RADIOFREQUENCY_OFFLINE,			  //射频检测离线
	HCI_REMOTE_CHEAK_ERROR,			 				//存在异常遥控信号
	HCI_SYS_FREE,			 								//系统空闲
	HCI_START_PROCESS,			 					//开始流程
//	HCI_WAIT_CAR,											//等待汽车到位
	HCI_NO_IN_PLACE,									//车辆未到位
	HCI_START_WEIGHT,									//开始称重
	HCI_NO_IN_PLACE2,									//车辆未到位
	HCI_SACN_RQ,											//请扫描二维码
	HCI_NOT_CARNUM,										//未发现车牌
	HCI_NOT_CARD,										//未发现射频卡信息
	HCI_WEIGHT_ING,										//正在称重
	HCI_WAIT_END,											//等待结束
	HCI_PLAY_WEIGHT,									//播放重量
	HCI_WAIT_LEAVE,										//等待车离开
	HCI_SHOW_SEV_ERROR,								//显示服务器发送过来的错误
	QUALITYTESTINGFAIL								//质检失败
}HCIStatus;

typedef struct 
{
    unsigned char title[60];
		unsigned char car[30];//车牌
	  unsigned char sn[30];//单号
	  unsigned char gn[20];//货物名
		unsigned char sp[20];//规格
		unsigned char ru[60];//收货单位
		unsigned char fu[60];//供应商
		unsigned char	gw[20];//毛重
		unsigned char tw[20];//皮重
		unsigned char nw[20];//净重
		unsigned char	gwt[22];//毛重时间
		unsigned char nwt[22];//皮重时间
		unsigned char sib[20];//司磅人
}PrintInfo_t;
typedef struct 
{
		SystemSta     status;				//状态
		SevType			  sevType;			//平台类型
		unsigned int mqttRxTime;
    unsigned char macid[6];			//MAC ID
    char 		*macidstring;				//MAC ID字符串
		unsigned char *devName;			//设备名称
		unsigned char	devType;			//设备类型 
    unsigned char hw;						//硬件版本
    unsigned char fw;						//软件版本
    unsigned char ip[64];				//网络IP
		unsigned int  funChangeTime;//FUN修改时间
		unsigned char check_rm;			//是否检测遥控 0：不检查   1检查
		unsigned char sound;
		unsigned char	weightType;			//
		unsigned char redCnt;					//红外数量
		unsigned char locationErrorInfo;//提示信息类型
		//----------------------------------------------
		unsigned int 	msgId;											//消息ID
		unsigned int 	processID;									//流程ID 0xffffffff代表自动开启的流程
		unsigned char processType;								//流程类型 0：由服务器开启，1：由设备开启
	  unsigned char msgType;										//消息类型：0：正常过磅 1：质检未通过
		unsigned char disableIr;									//本次是否禁止红外
		unsigned char isOpenTask;									//是否主动开启任务
		unsigned char isOutTime;									//是否已经超时
		unsigned int  arrivalTime;								//车辆到位时间
		unsigned int  arrivalWeightTime;								//允许获取有效重量的时间
		//----------------------------------------------
		//DevStatus_t		devStatus;		//设备状态
		unsigned char rqCode[MAX_RQ_CODE_BUF];	   //二维码信息
	  unsigned char current_num[VLPR_BUFF_LEN];  //识别到的车牌
	  char * cards[MAX_CARD_COUNT];
	  char card[MAX_CARD_COUNT][RADIOFREQUENCY_BUFF_LEN]; //射频ID
		unsigned char cardNum;
	  unsigned char cardIndex;								    //当前存储的位置
		unsigned char rqDir;										    //扫码方向
		unsigned char telecontrolMonitor;					  //是否存在监视遥控信号，0存在  1不存在
		
		int 	weight;																//重量
		unsigned char monitSta;											//监视器状态：是否警告
		unsigned short monitCH;											//监视器反馈的通道
		RoadGateSta  roadGate;					//道闸状态，1开或者0关
		RoadGateSta	 redStatus;					//红灯状态
		//------------------------------------------------------------------
		HCIStatus  hciStatus;							//人机交互状态
		unsigned char printSta;											//打印状态
		unsigned char breakType;										//是否放行
		unsigned char isPrint;											//是否打印
		unsigned char informatio[512];			
		PrintInfo_t   printInfo;										//打印信息
		//----------------------------------------------	
		unsigned int  irStatus;										//红外状态  0xx1有车进
		unsigned char isChangeCarNum;
		unsigned char carNum[35];								//本次要称重的车牌
		unsigned char nextNum[35];								//下一辆要称重的车牌
		//--------------------------------
		unsigned char errorInfo[ERROR_MSG_LENGHT];
		unsigned char shorError;
		char my_url[64];
}SystemStatus_t;


#pragma pack()

extern SystemStatus_t systemStatus;

void initMainTask(void);
void mainTaskRun(void);
void changeSystemStatus(SystemSta sysStatus);



#endif
