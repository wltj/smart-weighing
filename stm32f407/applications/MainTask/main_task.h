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
typedef void (* SystemFun)(void);//ϵͳ����
//��բ״̬
typedef enum  
{ 
		RG_INIT	=		-1,	
		CLOSE=0,
		OPEN=1
}RoadGateSta;
 	
////LED״̬
//typedef enum  
//{ 
//		LED_INIT	=		-1,		
//		LED_OFF_LINE	=		0,						//���� 
//		LED_FREE			=		1,						//���� 
//		LED_BE_BUSY1	=		2,						//æµ1��������û�гӳ���
//		LED_BE_BUSY2	=		3,						//æµ2�������Ѿ��ӳ���
//		LED_BE_BUSY3	=		4,						//����δ��λ
//		LED_BE_BUSY4	=		5,						//δɨ���ά��
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
//ϵͳ״̬
typedef enum  
{ 
		 SYSTME_OFF_LINE=		-1,				//����
		 SYSTME_FREE=				0,				//����
		 SPEAKER_BUYS1=			1,				//æµ1-->��δ�ϰ�
		 SPEAKER_BUYS2=			2,				//æµ2-->�����ϰ���
		 SPEAKER_WAIT_END=	3,				//�ȴ�����
		 SPEAKER_END_ING=		4,				//�����뿪//	
		 SPEAKER_ERROR=			5,					//ϵͳ����
		 SPEAKER_BUYS3=			6,				//æµ3-->��ʾ�ʼ�δͨ��
}SystemSta;	 
//typedef struct 
//{
//    RoadGateSta  roadGate;					//��բ״̬��1������0��
//    LEDSta  		 ledStatus;					//LED״̬
//}DevStatus_t;

typedef enum
{	
	HCI_INIT	=	-1,
	HCI_SYS_OFF_LINE=0,									//ϵͳ����
	HCI_WARNING_OFFLINE,							//����������
	HCI_PCB_OFFLINE,									//PCB����
	HCI_WEIGH_OFFLINE,								//�ذ�����
	HCI_VLPR_OFFLINE1,								//����ͷ1����
	HCI_VLPR_OFFLINE2,								//����ͷ2����
	HCI_RADIOFREQUENCY_OFFLINE,			  //��Ƶ�������
	HCI_REMOTE_CHEAK_ERROR,			 				//�����쳣ң���ź�
	HCI_SYS_FREE,			 								//ϵͳ����
	HCI_START_PROCESS,			 					//��ʼ����
//	HCI_WAIT_CAR,											//�ȴ�������λ
	HCI_NO_IN_PLACE,									//����δ��λ
	HCI_START_WEIGHT,									//��ʼ����
	HCI_NO_IN_PLACE2,									//����δ��λ
	HCI_SACN_RQ,											//��ɨ���ά��
	HCI_NOT_CARNUM,										//δ���ֳ���
	HCI_NOT_CARD,										//δ������Ƶ����Ϣ
	HCI_WEIGHT_ING,										//���ڳ���
	HCI_WAIT_END,											//�ȴ�����
	HCI_PLAY_WEIGHT,									//��������
	HCI_WAIT_LEAVE,										//�ȴ����뿪
	HCI_SHOW_SEV_ERROR,								//��ʾ���������͹����Ĵ���
	QUALITYTESTINGFAIL								//�ʼ�ʧ��
}HCIStatus;

typedef struct 
{
    unsigned char title[60];
		unsigned char car[30];//����
	  unsigned char sn[30];//����
	  unsigned char gn[20];//������
		unsigned char sp[20];//���
		unsigned char ru[60];//�ջ���λ
		unsigned char fu[60];//��Ӧ��
		unsigned char	gw[20];//ë��
		unsigned char tw[20];//Ƥ��
		unsigned char nw[20];//����
		unsigned char	gwt[22];//ë��ʱ��
		unsigned char nwt[22];//Ƥ��ʱ��
		unsigned char sib[20];//˾����
}PrintInfo_t;
typedef struct 
{
		SystemSta     status;				//״̬
		SevType			  sevType;			//ƽ̨����
		unsigned int mqttRxTime;
    unsigned char macid[6];			//MAC ID
    char 		*macidstring;				//MAC ID�ַ���
		unsigned char *devName;			//�豸����
		unsigned char	devType;			//�豸���� 
    unsigned char hw;						//Ӳ���汾
    unsigned char fw;						//����汾
    unsigned char ip[64];				//����IP
		unsigned int  funChangeTime;//FUN�޸�ʱ��
		unsigned char check_rm;			//�Ƿ���ң�� 0�������   1���
		unsigned char sound;
		unsigned char	weightType;			//
		unsigned char redCnt;					//��������
		unsigned char locationErrorInfo;//��ʾ��Ϣ����
		//----------------------------------------------
		unsigned int 	msgId;											//��ϢID
		unsigned int 	processID;									//����ID 0xffffffff�����Զ�����������
		unsigned char processType;								//�������� 0���ɷ�����������1�����豸����
	  unsigned char msgType;										//��Ϣ���ͣ�0���������� 1���ʼ�δͨ��
		unsigned char disableIr;									//�����Ƿ��ֹ����
		unsigned char isOpenTask;									//�Ƿ�������������
		unsigned char isOutTime;									//�Ƿ��Ѿ���ʱ
		unsigned int  arrivalTime;								//������λʱ��
		unsigned int  arrivalWeightTime;								//�����ȡ��Ч������ʱ��
		//----------------------------------------------
		//DevStatus_t		devStatus;		//�豸״̬
		unsigned char rqCode[MAX_RQ_CODE_BUF];	   //��ά����Ϣ
	  unsigned char current_num[VLPR_BUFF_LEN];  //ʶ�𵽵ĳ���
	  char * cards[MAX_CARD_COUNT];
	  char card[MAX_CARD_COUNT][RADIOFREQUENCY_BUFF_LEN]; //��ƵID
		unsigned char cardNum;
	  unsigned char cardIndex;								    //��ǰ�洢��λ��
		unsigned char rqDir;										    //ɨ�뷽��
		unsigned char telecontrolMonitor;					  //�Ƿ���ڼ���ң���źţ�0����  1������
		
		int 	weight;																//����
		unsigned char monitSta;											//������״̬���Ƿ񾯸�
		unsigned short monitCH;											//������������ͨ��
		RoadGateSta  roadGate;					//��բ״̬��1������0��
		RoadGateSta	 redStatus;					//���״̬
		//------------------------------------------------------------------
		HCIStatus  hciStatus;							//�˻�����״̬
		unsigned char printSta;											//��ӡ״̬
		unsigned char breakType;										//�Ƿ����
		unsigned char isPrint;											//�Ƿ��ӡ
		unsigned char informatio[512];			
		PrintInfo_t   printInfo;										//��ӡ��Ϣ
		//----------------------------------------------	
		unsigned int  irStatus;										//����״̬  0xx1�г���
		unsigned char isChangeCarNum;
		unsigned char carNum[35];								//����Ҫ���صĳ���
		unsigned char nextNum[35];								//��һ��Ҫ���صĳ���
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
