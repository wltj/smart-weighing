#include "system_params.h"
#include "easyflash.h"
#include "main_task.h"
#include <string.h>
#include <rtthread.h>
#include "weigh.h"
#include "sound.h"

#include <arpa/inet.h>
#include <netdev.h>       
#include <string.h>
#include <stdio.h>

#define DEV_NAME_KEY 					"dev_name"
#define DEV_CHEACK_RM_KEY 		"check_rm"
#define DEV_SOUND_KEY 				"sound"
#define DEV_TYPE_KEY 					"dev_type"
#define RED_CNT_KEY 					"red_cnt"
#define SERVER_ADDRESS_KEY 		"server_addres1"
#define WEIGH_TYPE_KEY 				"weigh_type"
#define PARAMS_VERSION_KEY		"pvk"
#define LOCATION_ERROR_KEY		"location_error"

#define RED_CNT								2    //红外对数量
#define PARAMS_UPDATA 				1	
#define PARAMS_VERSION				12//参数版本
//位置错误信息
//0  提示: 车辆未到位，请移动
//1  提示：车辆未到位，请向场内/场外方向移动
//2  提示：车辆未到位，请向场内/场外方向移动，与1不同的时，提示场内/场外的条件相反(遮挡红外条件)
//3  提示：车辆未到位，请朝东/西移动
//4  提示：车辆未到位，请朝东/西移动，与3不同的时，提示东西的条件相反(遮挡红外条件)
//5  提示：车辆未到位，请朝南/北移动
//6  提示：车辆未到位，请朝南/北移动，与5不同的时，提示南北的条件相反(遮挡红外条件) 
#define PARAMS_LOCATION_ERROR_INFO	1	

void initSystemParams(char *url)
{
	if(PARAMS_UPDATA==1)
	{
		unsigned char buff[1]={0};//check_rm
		ef_get_env_blob(PARAMS_VERSION_KEY, buff, sizeof(buff), RT_NULL);
		if(buff[0]!=PARAMS_VERSION)
		{
			buff[0]=PARAMS_VERSION;
			ef_set_env_blob(PARAMS_VERSION_KEY, buff,sizeof(buff));
//			ef_del_env(DEV_NAME_KEY);
//			ef_del_env(DEV_CHEACK_RM_KEY);
				saveCheckRM(1);
//			ef_del_env(DEV_SOUND_KEY);
				saveSound(1);
//			ef_del_env(DEV_TYPE_KEY);
				saveDevType(DEV_TYPE1);
//			ef_del_env(RED_CNT_KEY);
				saveREDCnt(RED_CNT);
//			ef_del_env(SERVER_ADDRESS_KEY);
				saveServerAddress(url);
//			ef_del_env(WEIGH_TYPE_KEY);
				saveWeighType(WEIGH_TYPE);
				saveLocationErrorInfo(1);
		}
	}
	rt_kprintf("//////////////////////PARAMS_VERSION: %d\n",PARAMS_VERSION);
	readDevName();
	readCheckRM();
	readSound();
	readDevType();
	readREDCnt();
	readServerAddress(url);
	readWeighType();
	readLocationErrorInfo();
}

typedef union tagIPADDR_HEX_U
{
    uint32_t ulAddr;
    struct {
        uint8_t ucHigh;
        uint8_t ucMediumH;
        uint8_t ucMediumL;
        uint8_t ucLow;
    } stAddr;
} IPADDR_U;

int netdev_to_get_ip(char * my_url, int url_len)
{
    struct netdev *netdev = RT_NULL;
    IPADDR_U unIp = {0};


    netdev = netdev_get_by_name("e0");
    if (netdev == RT_NULL)
    {
        rt_kprintf("not find network interface device name(\"e0\").\n");
        return -1;
    }


		unIp.ulAddr = netdev->ip_addr.addr;
    while(unIp.ulAddr == 0)
    {
        rt_thread_mdelay(1000);
        rt_kprintf("waiting to update ip_addr.\n");
        
       // lining();
        setSoundPayFun(lining);
        
        unIp.ulAddr = netdev->ip_addr.addr;
				
    }
      memset(systemStatus.ip, 0 , 64);
      sprintf(systemStatus.ip,"%d.%d.%d.%d",
                                    unIp.stAddr.ucHigh,
                                    unIp.stAddr.ucMediumH,
                                    unIp.stAddr.ucMediumL,
                                    unIp.stAddr.ucLow);

		rt_thread_mdelay(5000);


    rt_kprintf("ip:%d.%d.%d.%d\n", unIp.stAddr.ucHigh,
                                    unIp.stAddr.ucMediumH,
                                    unIp.stAddr.ucMediumL,
                                    unIp.stAddr.ucLow);
    memset(my_url, 0, url_len);
//   rt_sprintf(my_url, "tcp://%d.%d.%d.100:1883",
 //                                      unIp.stAddr.ucHigh,
  //                                      unIp.stAddr.ucMediumH,
   //                                     unIp.stAddr.ucMediumL);
		  //  rt_sprintf(my_url, "tcp://10.106.56.238:1883");
            rt_sprintf(my_url, "tcp://172.17.13.100:1883");
		rt_kprintf("\\\\\\\\\\\\\\\\\\my_url:%s\n",my_url);
    return 0;
}



static char macid[MACID_STRING_LENTH+1];//保存MAC字符串

#define DEVNAME_LEGHT						100 
unsigned char deverName[DEVNAME_LEGHT];//保存设备Name的字符串

unsigned char *getNameArray(void)
{
	return deverName;
}
 char *getMACArray(void)
{
	return macid;
}

//-----------------------------------------------------------------------------

//从Flash中读出设备名称
unsigned char * readDevName(void)
{
		ef_get_env_blob(DEV_NAME_KEY, deverName, DEVNAME_LEGHT, RT_NULL);
		if(deverName[0]==0)
		{
			rt_sprintf((char *)deverName,"%s_%s","ZNGB",macid);
			saveDevName((char *)deverName);
		}
		systemStatus.devName=deverName;
		return 	systemStatus.devName;
}
//将设备名称保存到Flash中
void saveDevName( char *s)
{
		int len=rt_strlen(s);
		if(len>=DEVNAME_LEGHT)
		{
			s[DEVNAME_LEGHT]=0;
			len=DEVNAME_LEGHT;
		}
		rt_memcpy(deverName,s,len);
		deverName[len]=0;
		systemStatus.devName=deverName;
		ef_set_env_blob(DEV_NAME_KEY, deverName,DEVNAME_LEGHT);
}
//------------------------------------------

#define DEV_CHEACK_RM_FLAG 		0x56
unsigned char  readCheckRM(void)
{
	unsigned char buff[2]={0};//check_rm
		ef_get_env_blob(DEV_CHEACK_RM_KEY, buff, sizeof(buff), RT_NULL);
		if(buff[0]!=DEV_CHEACK_RM_FLAG)
		{
			buff[1]=1;
			saveCheckRM(buff[1]);
		}
		systemStatus.check_rm=buff[1];
		return systemStatus.check_rm;
}
void saveCheckRM(unsigned char checkRm)
{
		unsigned char buff[2]={0};//check_rm
		buff[0]=DEV_CHEACK_RM_FLAG;
		buff[1]=checkRm;
		systemStatus.check_rm=checkRm;
		ef_set_env_blob(DEV_CHEACK_RM_KEY, buff,sizeof(buff));
}
//------------------------------------------

#define DEV_SOUND_FLAG 		0x56
unsigned char  readSound(void)
{
	unsigned char buff[2]={0};//check_rm
		ef_get_env_blob(DEV_SOUND_KEY, buff, sizeof(buff), RT_NULL);
		if(buff[0]!=DEV_SOUND_FLAG)
		{
			buff[1]=1;
			saveSound(buff[1]);
		}
		systemStatus.sound=buff[1];
		return systemStatus.sound;
}
void saveSound(unsigned char sound)
{
		unsigned char buff[2]={0};//check_rm
		buff[0]=DEV_SOUND_FLAG;
		buff[1]=sound;
		systemStatus.sound=sound;
		ef_set_env_blob(DEV_SOUND_KEY, buff,sizeof(buff));
}

//-----------------------------------------------------------------------------

unsigned char * readDevType(void)
{
		ef_get_env_blob(DEV_TYPE_KEY, &systemStatus.devType, 1, RT_NULL);
		if((systemStatus.devType!=DEV_TYPE1) && (systemStatus.devType!=DEV_TYPE2))
		{
			systemStatus.devType=DEV_TYPE1;
			saveDevType(systemStatus.devType);
		} 
		return &systemStatus.devType;
}
void saveDevType(unsigned char type)
{
	ef_set_env_blob(DEV_TYPE_KEY, &type,1);	
	systemStatus.devType=type;
}
//-----------------------------------------------------------------------------

//从Flash中读出红外数量
unsigned char * readREDCnt(void)
{
		if(ef_get_env_blob(RED_CNT_KEY, &systemStatus.redCnt, 1, RT_NULL)==0)
		{
			systemStatus.redCnt=RED_CNT;
			saveREDCnt(systemStatus.redCnt);
		} 
		return &systemStatus.redCnt;
}
//将红外数量保存到Flash中
void saveREDCnt(unsigned char cnt)
{
		ef_set_env_blob(RED_CNT_KEY, &cnt,1);	
		systemStatus.redCnt=cnt;
}
//-----------------------------------------------------------------------------


#define SERVER_ADDRESS_LEGHT		40 
static char serverAddress[SERVER_ADDRESS_LEGHT]={0};// 

char * readServerAddress( char * defaultUrl)
{
		memset(serverAddress,0,sizeof(serverAddress));
	
		ef_get_env_blob(SERVER_ADDRESS_KEY, serverAddress, SERVER_ADDRESS_LEGHT, RT_NULL);
		if(serverAddress[0]==0)
		{
			rt_sprintf((char *)serverAddress,"%s",defaultUrl);
			saveServerAddress((char *)serverAddress);
		}
		return 	serverAddress;
}
 char * getServerAddress(void)
{
			return 	serverAddress;
}

void saveServerAddress( char *s)
{
		int len=rt_strlen(s);
		if(len>=SERVER_ADDRESS_LEGHT)
		{
			s[SERVER_ADDRESS_LEGHT]=0;
			len=SERVER_ADDRESS_LEGHT;
		}
		rt_memcpy(serverAddress,s,len);
		serverAddress[len]=0;
		ef_set_env_blob(SERVER_ADDRESS_KEY, serverAddress,SERVER_ADDRESS_LEGHT);
}
//-----------------------------------------------------------------------------

//从Flash中读出地磅类型
unsigned char * readWeighType(void)
{
		if(ef_get_env_blob(WEIGH_TYPE_KEY, &systemStatus.weightType, 1, RT_NULL)==NULL)
		{
			systemStatus.weightType=WEIGH_TYPE;
			saveWeighType(systemStatus.weightType);
		} 
		return &systemStatus.weightType;
}
//将地磅类型保存到Flash中
void saveWeighType(unsigned char type)
{
		ef_set_env_blob(WEIGH_TYPE_KEY, &type,1);	
		systemStatus.weightType=type;
}

//-----------------------------------------------------------------------------
unsigned char * readLocationErrorInfo(void)
{
		if(ef_get_env_blob(LOCATION_ERROR_KEY, &systemStatus.locationErrorInfo, 1, RT_NULL)==NULL)
		{
			systemStatus.locationErrorInfo=1;
			saveLocationErrorInfo(systemStatus.locationErrorInfo);
		} 
		return &systemStatus.locationErrorInfo;
}
void saveLocationErrorInfo(unsigned char type)
{
		ef_set_env_blob(LOCATION_ERROR_KEY, &type,1);	
		systemStatus.locationErrorInfo=type;
}
//------------------------------------------------
void setParamWithRQData(unsigned char *data)
{
	unsigned char dataBuff[100];
	unsigned char len=0;
	memset(dataBuff,0,sizeof(dataBuff));
	if((data[0]=='A')&&(data[1]=='A')&&(data[2]=='A'))
	{
		switch(data[3])
		{
			case '0'://设置设备名称
				len=(data[4]-'0')*10+data[5]-'0';
				memcpy(dataBuff,&data[6],len);
				saveDevName((char *)dataBuff);
				break;
		  case '1'://设置是否检测红外
				saveCheckRM(data[4]-'0');
				break;
			case '2'://设置设备类型
				saveDevType(data[4]-'0');
				break;
			case '3'://设置是否播报声音
				saveSound(data[4]-'0');
				break;
			case '4'://设置平台IP
				len=(data[4]-'0')*10+data[5]-'0';
				memcpy(dataBuff,&data[6],len);
				saveServerAddress((char *)dataBuff);
				__set_FAULTMASK(1);
			  HAL_NVIC_SystemReset();
			  NVIC_SystemReset();
				break;
			case '5'://地磅类型
				saveWeighType(data[4]-'0');
				break;
			case '6':
				saveREDCnt(data[4]-'0');
				break;
			case '7':
				saveLocationErrorInfo(data[4]-'0');
				break;
		}
	}
}
