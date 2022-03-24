/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-03-11     Jason        first version
 * 
 */
#include "cmd_handle.h"
#include <string.h>
#include "crc.h"
#include <netdev_ipaddr.h>
#include "mqtt_thread.h"
#include "com_to_server_thread.h"
#include "cmd_table.h"
#include <rtthread.h>
#include "com_to_server_cmd_list.h"
#include "cJSON.h"
#include "cJSON_util.h"
#include "hci.h"
#include "main_task.h"
#include "system_params.h"
#include "uart_thread.h"
#define MIN_LENGTH  10

#define  IND_XXX			0	//IND系列 
#define  DS_XXX				1 //DS系列
#define  MATRIX_XXX		2 //MATRIX  II参数：SCALE1  250MS F10 9600 8Bits CR RS-485
#define  XK_XXX				3 //xk3102d1

static unsigned char send_handle_flag = 0;
static com_to_server_data_t com_rec_data_t,com_send_data_t;

static void setCarInfoCmdHandle(cJSON *pJson);
//本次流程放行，处理函数  
static void setThroughCmdHandle(cJSON *pJson);
//本次流程中断，处理函数  
static void setProcessBreadCmdHandle(cJSON *pJson);
//修改展示车牌信息，处理函数
static void changeCarNumCmdHandle(cJSON *pJson);
/*返回开启流程结果*/
static void  openTaskCmdHandle(cJSON *pJson);
/*修改红外状态*/
static void  changeInfraredHandle(cJSON *pJson);
static void  showSvrError(cJSON *pJson);

static void  changeDevName(cJSON *pJson);
static void  setDisableRMStatus(cJSON *pJson);
static void  setSoundStatus(cJSON *pJson);
static void  changeREDCnt(cJSON *pJson);
static void  changeWeighType(cJSON *pJson);

//请求开启新的流程
static void sendOpenTask(void);
static void sendOutTime(void);

cJSON *pSerJson=NULL;
void cmd_rec_handle(unsigned char * data,unsigned int lenth)
{
 
    memset((unsigned char*)&com_rec_data_t,0,sizeof(com_to_server_data_t));
    memcpy((unsigned char*)&com_rec_data_t,data,lenth);
    /*字节顺序转换*/
    com_rec_data_t.cmd = ntohs(com_rec_data_t.cmd);
    com_rec_data_t.crc = ntohl(com_rec_data_t.crc);
    com_rec_data_t.lenth = ntohl(com_rec_data_t.lenth);
    /* 校验长度*/
    if(lenth != com_rec_data_t.lenth)
        return ;
    
    if(com_rec_data_t.lenth < MIN_LENGTH)
        return;
    /* crc 校验*/
//    unsigned int crc = calculate_CRC32((unsigned char*)&com_rec_data_t.cmd,com_rec_data_t.lenth-8,0);
//    if(crc != com_rec_data_t.crc)
//        return;
		systemStatus.mqttRxTime=rt_tick_get();
    if(com_rec_data_t.lenth>MIN_LENGTH)
    {
			pSerJson=cJSON_Parse((char *)com_rec_data_t.jsonStr);
			if(pSerJson==NULL)
			{
					 return;
			}
    }
		// rt_kprintf("receive serves message,cmd=%d\n",com_rec_data_t.cmd);
    rt_kprintf("receive serves message,cmd=%d,len=%d,data=%s\n",com_rec_data_t.cmd,com_rec_data_t.lenth, com_rec_data_t.jsonStr);
    switch(com_rec_data_t.cmd)
    {
			
			case Cmd_DEV_REGISTER:
				
				if(pSerJson!=NULL)//systemStatus.devType=1;
				{
					int type=0;
					cJSON_item_get_number(pSerJson,"sev_type",&type);
					cJSON_Delete(pSerJson);
					pSerJson=NULL;
					if((type>=TYPE_FEED)&&(type<=TYPE_SLAUGHTER))
					{
						systemStatus.sevType=type;
					}
					else
					{
						systemStatus.sevType=TYPE_FEED;
					}
				}
				else
				{
					systemStatus.sevType=TYPE_FEED;
				}
				Set_Mqtt_Status(MQTT_LINKING);
				break;
			/*--------------------------------------------------------------------------------------------------------*/
			case CMD_SVR_QUERY_DEVSETTING:																	/*查询设备系统参数*/
				sendDevParams();
				break;
			case CMD_SVR_QUERY_DEV_STATUS:																	/*查询设备系统状态*/
				if(pSerJson!=NULL)//systemStatus.devType=1;
				{
					int processID=0;
					//-------------------		
					cJSON_item_get_number(pSerJson,"process_id",&processID);
					if(systemStatus.status!=SYSTME_FREE)//
					{
						systemStatus.processID=processID;
					}
					cJSON_Delete(pSerJson);
					pSerJson=NULL;
				}
			
			  if(systemStatus.isOpenTask)
				{
					sendOpenTask();
					systemStatus.isOutTime=0;
					systemStatus.isOpenTask=0;
				}
				else if(systemStatus.isOutTime)
				{
					sendOutTime();
					systemStatus.isOutTime=0;
				}
				else
				{
					sendDevStatus(CMD_DEV_UPLOAD_STATUS);
				}
				break;
			case CMD_SVR_SET_CARINFO:																				/*设置车辆信息*/
				setCarInfoCmdHandle(pSerJson);
				break;
			case CMD_SVR_SET_THROUGH:																				/*设置本次流程放行*/
				setThroughCmdHandle(pSerJson);
				break;
			case CMD_SVR_PROCESS_BREAK:																			/*设置本次流程中断*/
				setProcessBreadCmdHandle(pSerJson);
				break;
			case CMD_SVR_CHANGECARNUM:																			/*修改展示车牌信息*/
				rt_memset(systemStatus.carNum,0,rt_strlen((char *)systemStatus.carNum));
				changeCarNumCmdHandle(pSerJson);
				break;
			case CMD_SVR_OPEN_TASK:																					/*返回开启流程结果*/
				openTaskCmdHandle(pSerJson);
				break;
			case CMD_SVR_DISABLE_INFRARED:																	//禁止红外
				changeInfraredHandle(pSerJson);
				break;
			case CMD_SVR_SHOW_ERROR:
				showSvrError(pSerJson);
				break;
			case CMD_SVR_SET_DEVNAME:																				// 设置设备名称
				changeDevName(pSerJson);
									rt_kprintf("//////////////////////	changeDevName: %d\n",pSerJson);
				break;
			/*新增设置系统参数*/
			case CMD_SVR_SET_REDCNT :																				// 设置红外对数
				changeREDCnt(pSerJson);
						//rt_kprintf("//////////////////////	changeREDCnt: %d\n",cJSON_GetObjectItem(pSerJson,"count")->valueint);
				break;
			case CMD_SVR_SET_WEIGHTYPE:																				// 设置地磅型号
				
				changeWeighType(pSerJson);

				break;
			
			case CMD_SVR_DISABLE_RM:																				// 是否禁止遥控监测
				setDisableRMStatus(pSerJson);
				break;	
			case CMD_SVR_SOUND_STATUS:                                
				setSoundStatus(pSerJson);
				break;
        default:
        break;
    }  
		if(pSerJson!=NULL)
		{
			 cJSON_Delete(pSerJson);
			 pSerJson=NULL;
		}	
}
 

int cmd_send_handle(char *topic,unsigned short cmd,unsigned char *jsonStr,unsigned int json_lenth)
{
    if(send_handle_flag == 1)
        return 1;
    
    send_handle_flag = 1;//上锁
    
    memset((unsigned char *)&com_send_data_t.cmd,0,sizeof(com_to_server_data_t));
    
    com_send_data_t.cmd = cmd;
    memcpy(&(com_send_data_t.jsonStr[0]),jsonStr,json_lenth);
    com_send_data_t.lenth = json_lenth + MIN_LENGTH;
		    /* 字节序转换*/
    com_send_data_t.cmd = htons(com_send_data_t.cmd);
    com_send_data_t.crc = calculate_CRC32((unsigned char*)&com_send_data_t.cmd,com_send_data_t.lenth-8,0xffffffff);
    

    com_send_data_t.crc = htonl(com_send_data_t.crc);
    com_send_data_t.lenth = htonl(com_send_data_t.lenth);
    
    if(topic == NULL)
        user_mqtt_publish_to_server((unsigned char *)&com_send_data_t,json_lenth + MIN_LENGTH);
    else
        user_mqtt_publish(topic,(unsigned char *)&com_send_data_t,json_lenth + MIN_LENGTH);
    
    send_handle_flag = 0;//开锁
		  
//    if(topic == NULL)
//		{
//			rt_kprintf("send mqtt message:cmd=%d,jsonStr=%s\r\n",cmd,com_send_data_t.jsonStr);//
//		}
//		else
//		{
//			rt_kprintf("send mqtt message:topic=%s,cmd=%d,jsonStr=%s\n",topic,cmd,com_send_data_t.jsonStr);//
//		}
    return 0;
}



#define MAC_ID 			"mac_id"
#define DEV_NAME		 "driver_name"
#define RED_CNT      "red_cnt"
#define WEIGH_TYPE   "weigh_type"
#define IP					 "ip"
#define HW					 "hw"
#define FW					 "fw"
void sendRegisterMessage(void)//发送注册消息
{
    char  *cjson_str = NULL;
    cJSON * root =  cJSON_CreateObject(); 
		cJSON_AddStringToObject(root,MAC_ID,(char *)systemStatus.macidstring);//
		cJSON_AddStringToObject(root,IP,(char *)systemStatus.ip);//gateway.ip
		cJSON_AddStringToObject(root,DEV_NAME,(char *)systemStatus.devName);//gateway.ip
	    cJSON_AddNumberToObject(root,HW, systemStatus.hw);//
 		cJSON_AddNumberToObject(root,FW, systemStatus.fw);//
		cJSON_AddNumberToObject(root,RED_CNT, systemStatus.redCnt);//
 		cJSON_AddNumberToObject(root,WEIGH_TYPE,systemStatus.weightType);//
//	cJSON_AddStringToObject(root,WEIGH_TYPE,(char *)systemStatus.weightType);//gateway.ip
	
    cjson_str =cJSON_Print(root);
    rt_kprintf("send register message\r\n");
    cmd_send_handle(MQTT_REG_TOPIC,Cmd_DEV_REGISTER,(unsigned char *)cjson_str,strlen(cjson_str));
    cJSON_Delete(root);
    cJSON_free(cjson_str);
}
 
//-------------------------------------------------------------------------------------------------------------- 
//以下为新协议的发送
/**
* 发送设备参数
 *
 * @param  null
 * @return  null
 * @note		未实现
 */
void sendDevParams(void)
{
		char  *cjson_str = NULL;
    cJSON * root =  cJSON_CreateObject(); 

    cjson_str =cJSON_Print(root);
    cmd_send_handle(NULL,CMD_DEV_UPLOAD_SETTING,(unsigned char *)cjson_str,strlen(cjson_str));
    cJSON_Delete(root);
    cJSON_free(cjson_str);
}
//向服务器请求开启流程
static void sendOpenTask(void)
{
		#define KEY_RQ_CODE				"rq_code"
		char  *cjson_str = NULL;
    cJSON * root =  cJSON_CreateObject(); 
		if(root==NULL)
		{
			return;
		}
		cJSON_AddStringToObject(root,( char *) KEY_RQ_CODE,( char *)systemStatus.rqCode); 
    cjson_str =cJSON_Print(root);
		if(cjson_str!=NULL)
		{
			//rt_kprintf("send cmd=%d Dev Status message\n",CMD_DEV_OPEN_TASK);
			cmd_send_handle(NULL,CMD_DEV_OPEN_TASK,(unsigned char *)cjson_str,strlen(cjson_str));
		}
		else
		{
			rt_kprintf("send cmd=%d fail,cjson_str = null\n",CMD_DEV_OPEN_TASK);
		}
		cJSON_Delete(root);
		cJSON_free(cjson_str);
}
static void sendOutTime(void)
{
		#define KEY_SYS_STA				"sysSta"
		char  *cjson_str = NULL;
    cJSON * root =  cJSON_CreateObject(); 
		if(root==NULL)
		{
			return;
		}
		unsigned char status=0;
		switch(systemStatus.status)
		{
			case SYSTME_OFF_LINE:
			case SYSTME_FREE:
			case SPEAKER_BUYS1:
				status=systemStatus.status;
				break;
			default:
				status=systemStatus.status-1;
			break;
		}
		cJSON_AddNumberToObject(root,( char *) KEY_SYS_STA,status); 
    cjson_str =cJSON_Print(root);
		if(cjson_str!=NULL)
		{
		//	rt_kprintf("send cmd=%d Dev Status message\n",CMD_DEV_SEND_OUTTIME);
			cmd_send_handle(NULL,CMD_DEV_OPEN_TASK,(unsigned char *)cjson_str,strlen(cjson_str));
		}
		else
		{
			rt_kprintf("send cmd=%d fail,cjson_str = null\n",CMD_DEV_SEND_OUTTIME);
		}
		cJSON_Delete(root);
		cJSON_free(cjson_str);
}
/**
* 发送设备状态
 *
 * @param  null
 * @return  null
 */
void sendDevStatus(unsigned int cmd)
{
		
		#define KEY_IR_STA				"irSta"
		#define KEY_BARGATE_STA		"barGateSta"
		#define KEY_WEIGH					"weigh"
		#define KEY_RQCODE_STA		"rqcode"
		#define KEY_CARD_STA			"cards"
		#define KEY_MONIT_STA			"monitSta"
		#define KEY_PRINT_STA			"printSta"
		#define KEY_MSG_ID				"msg_id"
		#define KEY_FLOW_ID				"process_id"
		#define KEY_CAR_NUM				"car_num"
		#define KEY_CURRENT_NUM		"current_num"
		#define KEY_SIGN					"sign"
		#define KEY_REAL_WEIGHT		"real_weight"

		char  *cjson_str = NULL;
		int real_weight=0;
		if(pSerJson!=NULL)
		{
			cJSON_Delete(pSerJson);
			pSerJson=NULL;
		}
    cJSON * root =  cJSON_CreateObject(); 
		unsigned char status=0;
		switch(systemStatus.status)
		{
			case SYSTME_OFF_LINE:
			case SYSTME_FREE:
			case SPEAKER_BUYS1:
				status=systemStatus.status;
				break;
			case SPEAKER_BUYS3:
				status=1;
				break;
			default:
				status=systemStatus.status-1;
			break;
		}
		cJSON_AddNumberToObject(root,( char *) KEY_SYS_STA,status); 
		cJSON_AddNumberToObject(root,( char *) KEY_IR_STA,systemStatus.irStatus); 
		cJSON_AddNumberToObject(root,( char *) KEY_BARGATE_STA,systemStatus.roadGate); 
		cJSON_AddNumberToObject(root,( char *) KEY_WEIGH,(systemStatus.weight/10)*10); 
		cJSON_AddNumberToObject(root,( char *) KEY_MONIT_STA,systemStatus.monitSta); 
		cJSON_AddNumberToObject(root,( char *) KEY_PRINT_STA,systemStatus.printSta); 
	
		cJSON_AddNumberToObject(root,( char *) KEY_MSG_ID,(systemStatus.msgId++)); 
		cJSON_AddNumberToObject(root,( char *) KEY_FLOW_ID,systemStatus.processID); 
		cJSON_AddNumberToObject(root,( char *) KEY_SIGN,weighing.sign);
		real_weight=weighing.weight_value*10;
		cJSON_AddNumberToObject(root,( char *) KEY_REAL_WEIGHT,real_weight); 
		
		cJSON_AddStringToObject(root,( char *) KEY_RQCODE_STA,(char *)systemStatus.rqCode); 
		
//		cJSON *pJsonArry=cJSON_CreateStringArray((const char **)systemStatus.cards,(int)systemStatus.cardNum);	
//		cJSON_AddItemToObject(root,( char *) KEY_CARD_STA,pJsonArry);
		if((systemStatus.cardNum!=0)&&(status!=SYSTME_FREE))
		{
			char buff[100]={0};
			for(int i=0;i<systemStatus.cardNum;i++)
			{
				if(systemStatus.cards[i][0]!=0)
				{
					rt_sprintf(&buff[strlen(buff)],"%s,",systemStatus.cards[i]);
				}
			}
			buff[strlen(buff)-1]=0;
			cJSON_AddStringToObject(root,( char *) KEY_CARD_STA,(char *)buff); 
		}
		cJSON_AddStringToObject(root,( char *) KEY_CURRENT_NUM,(char *)systemStatus.current_num); 
		cJSON_AddStringToObject(root,( char *) KEY_CAR_NUM,(char *)systemStatus.carNum); 
    cjson_str =cJSON_Print(root);
		if(cjson_str!=NULL)
		{
			//rt_kprintf("send cmd=%d Dev Status message\n",cmd);
			cmd_send_handle(NULL,cmd,(unsigned char *)cjson_str,strlen(cjson_str));
		}
		else
		{
			rt_kprintf("send cmd=%d fail,cjson_str = null\n",cmd);
		}
		cJSON_Delete(root);
		cJSON_free(cjson_str);
}
static void sendResultCmd(unsigned int cmd,unsigned char result)
{
		char  *cjson_str = NULL;
		if(pSerJson!=NULL)
		{
			cJSON_Delete(pSerJson);
			pSerJson=NULL;
		}
    cJSON * root =  cJSON_CreateObject(); 
		cJSON_AddNumberToObject(root,( char *) "res",result);
    cjson_str =cJSON_Print(root);
		if(cjson_str!=NULL)
		{
			//rt_kprintf("send cmd=%d Dev Status message\n",cmd);
			cmd_send_handle(NULL,cmd,(unsigned char *)cjson_str,strlen(cjson_str));
		}
		else
		{
			rt_kprintf("send cmd=%d fail,cjson_str = null\n",cmd);
		}
		cJSON_Delete(root);
		cJSON_free(cjson_str);
}
//--------------------------------------------------------------------------------------------------------------
//接收处理函数
//-------------------------------------------------------------------------------------------------------------- 
//设置流程开始（车辆信息），处理函数  
static void setCarInfoCmdHandle(cJSON *pJson)
{
		char* carNum=NULL;
		char* nextCarNum=NULL;
	  int processID=0;
		int disableInfrared=0;
		int msgType=0;
		if(pJson==NULL)
		{
			return;
		}
		carNum=( char *)cJSON_item_get_string(pJson,"carNum");
		nextCarNum=( char *)cJSON_item_get_string(pJson,"nextNum");
		cJSON_item_get_number(pJson,"process_id",&processID);
		cJSON_item_get_number(pJson,"type",&msgType);
		cJSON_item_get_number(pJson,"disable_infrared",&disableInfrared);
		//rt_kprintf("processID=%d,disableInfrared=%d,carNum=%s,nextNum=%s\n",processID,disableInfrared,carNum,nextCarNum);
		if((processID==0)||(systemStatus.status!=SYSTME_FREE))//
		{
			sendDevStatus(CMD_DEV_UPLOAD_CARINFO);
			return;
		}
		systemStatus.processID=processID;
		systemStatus.disableIr=disableInfrared;
		systemStatus.msgType=msgType;
		systemStatus.processType=0;
		if(carNum!=NULL)
		{ 
			rt_memcpy(systemStatus.carNum,carNum,strlen(carNum));
		}
		if(nextCarNum!=NULL)
		{ 
			rt_memcpy(systemStatus.nextNum,nextCarNum,strlen(nextCarNum));
		}  
		sendDevStatus(CMD_DEV_UPLOAD_CARINFO);
}
//本次流程放行，处理函数  
static void setThroughCmdHandle(cJSON *pJson)
{
	int processID=0;
	int isPrint=0;
	int b_type=0;
	int len1=0;
	int len2=0;

	 char *p=NULL;
	if(pJson==NULL)
	{
		return;
	}
		cJSON_item_get_number(pJson,"process_id",&processID);
		systemStatus.processID=processID;
		cJSON_item_get_number(pJson,"isPrint",&isPrint);
		cJSON_item_get_number(pJson,"b_type",&b_type);
		if(b_type==1)
		{
				systemStatus.breakType=2;//==2代表回到重新称重
		}
		else
		{
				systemStatus.breakType=1;//真实放行
				rt_memset(systemStatus.informatio,0,sizeof(systemStatus.informatio));
			  p=(char *)cJSON_item_get_string(pJson,"informatio");
			  if(p!=NULL)
				{
					len1=strlen(p);
					if(len1>10)
					{
						rt_memcpy(systemStatus.informatio,p,len1);
					}
				}
				len1=0;
				p=NULL;
		}
		if(isPrint)//如果需要打印信息
		{
			systemStatus.isPrint=1;
			rt_memset(&systemStatus.printInfo,0,sizeof(PrintInfo_t));
			//--------------------
			p=NULL;
			p=(char *)cJSON_item_get_string(pJson,"title");
			if(p!=NULL)
			{
				len1=strlen(p);
				len2=sizeof(systemStatus.printInfo.title);
				len1=len1>len2?len2-1:len1;
				rt_memcpy(systemStatus.printInfo.title,p,len1);
			}
			p=NULL;
			p=(char *)cJSON_item_get_string(pJson,"car");
			if(p!=NULL)
			{
				len1=strlen(p);
				len2=sizeof(systemStatus.printInfo.car);
				len1=len1>len2?len2-1:len1;
				rt_memcpy(systemStatus.printInfo.car,p,len1);
			}
			//--------------------
			p=NULL;
			p=(char *)cJSON_item_get_string(pJson,"SN");
			if(p!=NULL)
			{
				len1=strlen(p);
				len2=sizeof(systemStatus.printInfo.sn);
				len1=len1>len2?len2-1:len1;
				rt_memcpy(systemStatus.printInfo.sn,p,len1); 
			}
			//--------------------
			p=NULL;
			p=(char *)cJSON_item_get_string(pJson,"GN");
			if(p!=NULL)
			{
				len1=strlen(p);
				len2=sizeof(systemStatus.printInfo.gn);
				len1=len1>len2?len2-1:len1;
				rt_memcpy(systemStatus.printInfo.gn,p,len1);  
			}
			//--------------------
			p=NULL;
			p=(char *)cJSON_item_get_string(pJson,"SP");
			if(p!=NULL)
			{
				len1=strlen(p);
				len2=sizeof(systemStatus.printInfo.sp);
				len1=len1>len2?len2-1:len1;
				rt_memcpy(systemStatus.printInfo.sp,p,len1);   
			}
			//--------------------
			p=NULL;
			p=(char *)cJSON_item_get_string(pJson,"RU");
			if(p!=NULL)
			{
				len1=strlen(p);
				len2=sizeof(systemStatus.printInfo.ru);
				len1=len1>len2?len2-1:len1;
				rt_memcpy(systemStatus.printInfo.ru,p,len1);  
			}
			//--------------------
			p=NULL;
			p=(char *)cJSON_item_get_string(pJson,"FU");
			if(p!=NULL)
			{
				len1=strlen(p);
				len2=sizeof(systemStatus.printInfo.fu);
				len1=len1>len2?len2-1:len1;
				rt_memcpy(systemStatus.printInfo.fu,p,len1);  
			}
			//--------------------
			p=NULL;
			p=(char *)cJSON_item_get_string(pJson,"GW");
			if(p!=NULL)
			{
				len1=strlen(p);
				len2=sizeof(systemStatus.printInfo.gw);
				len1=len1>len2?len2-1:len1;
				rt_memcpy(systemStatus.printInfo.gw,p,len1);   
			}
			//--------------------
			p=NULL;
			p=(char *)cJSON_item_get_string(pJson,"TW");
			if(p!=NULL)
			{
				len1=strlen(p);
				len2=sizeof(systemStatus.printInfo.tw);
				len1=len1>len2?len2-1:len1;
				rt_memcpy(systemStatus.printInfo.tw,p,len1);  
			}
			//--------------------
			p=NULL;
			p=(char *)cJSON_item_get_string(pJson,"NW");
			if(p!=NULL)
			{
				len1=strlen(p);
				len2=sizeof(systemStatus.printInfo.nw);
				len1=len1>len2?len2-1:len1;
				rt_memcpy(systemStatus.printInfo.nw,p,len1);   
			} 
			//--------------------
			p=NULL;
			p=(char *)cJSON_item_get_string(pJson,"GWT");
			if(p!=NULL)
			{
				len1=strlen(p);
				len2=sizeof(systemStatus.printInfo.gwt);
				len1=len1>len2?len2-1:len1;
				rt_memcpy(systemStatus.printInfo.gwt,p,len1);  
			}
			//--------------------
			p=NULL;
			p=(char *)cJSON_item_get_string(pJson,"NWT");
			if(p!=NULL)
			{
				len1=strlen(p);
				len2=sizeof(systemStatus.printInfo.nwt);
				len1=len1>len2?len2-1:len1;
				rt_memcpy(systemStatus.printInfo.nwt,p,len1);   
			}
			//--------------------
			p=NULL;
			p=(char *)cJSON_item_get_string(pJson,"SIB");
			if(p!=NULL)
			{
				len1=strlen(p);
				len2=sizeof(systemStatus.printInfo.sib);
				len1=len1>len2?len2-1:len1;
				rt_memcpy(systemStatus.printInfo.sib,p,len1);  
			}
		}
	sendDevStatus(CMD_DEV_UPLOAD_THROUGH); 
}
static void setProcessBreadCmdHandle(cJSON *pJson)
{ 
		int processID=0;
		if(pJson==NULL)
		{
			return;
		}
		cJSON_item_get_number(pJson,"process_id",&processID);
		systemStatus.processID=processID;
		changeSystemStatus(SYSTME_FREE);
		sendDevStatus(CMD_DEV_PROCESS_BREAK);
}
static void changeCarNumCmdHandle(cJSON *pJson)
{
		if(pJson==NULL)
		{
			return;
		}
		char * p=NULL;
		p=(char *)cJSON_item_get_string(pJson,"new_num");
		if(p!=NULL)
		{
			rt_memcpy(systemStatus.carNum,p,strlen(p));
			systemStatus.isChangeCarNum=0x01|0x02;
		}
		sendDevStatus(CMD_DEV_CHANGECARNUM);
}
static void  openTaskCmdHandle(cJSON *pJson)
{
		int result=0;
		int msgType=0;
		if(pJson==NULL)
		{
			return;
		}
		cJSON_item_get_number(pJson,"result",&result);
		if(result)
		{
			char* carNum=NULL;
			int processID=0;
			int disableInfrared=0;
			carNum=( char *)cJSON_item_get_string(pJson,"car_num");
			cJSON_item_get_number(pJson,"process_id",&processID);
			cJSON_item_get_number(pJson,"type",&msgType);
			cJSON_item_get_number(pJson,"disable_infrared",&disableInfrared);
			//rt_kprintf("processID=%d,carNum=%s,nextNum=%s\n",processID,carNum);
			if((processID==0)||(systemStatus.status!=SYSTME_FREE))//
			{
				sendDevStatus(CMD_DEV_UPLOAD_CARINFO);
				return;
			}
			systemStatus.msgType=msgType;
			systemStatus.processID=processID;
			systemStatus.processType=1;
			systemStatus.disableIr=disableInfrared;
			if(carNum!=NULL)
			{ 
				rt_memcpy(systemStatus.carNum,carNum,strlen(carNum));
			}
		}
}
/*改变红外状态*/
static void  changeInfraredHandle(cJSON *pJson)
{
		int disableIr=0;
		if(pJson==NULL)
		{
			return;
		}
		cJSON_item_get_number(pJson,"disable_infrared",&disableIr);
		systemStatus.disableIr=disableIr;
		sendDevStatus(CMD_DEV_DISABLE_INFRARED);
}
static void  showSvrError(cJSON *pJson)
{
	 char *p=NULL;
		if(pJson==NULL)
		{
			return;
		}
		p=(char *)cJSON_item_get_string(pJson,"error_msg");
		if(p!=NULL)
		{
			int len=rt_strlen(p);
			systemStatus.shorError=1;
			if(strcmp((char *)systemStatus.errorInfo,p)!=0)
			{
				rt_memset(systemStatus.errorInfo,0,ERROR_MSG_LENGHT);
				rt_memcpy(systemStatus.errorInfo,p,ERROR_MSG_LENGHT<=len?(ERROR_MSG_LENGHT-1):len);
				updataHIC();
			}
		}
		
		sendDevStatus(CMD_DEV_SHOW_ERROR);
}
/*修改地磅名称*/
static void  changeDevName(cJSON *pJson)    
{
		 char *p=NULL;
		if(pJson==NULL)
		{
			return;
		}
		p=(char *)cJSON_item_get_string(pJson,"driver_name");
		if(p!=NULL)
		{
			saveDevName(p);
			    rt_kprintf("//////////////////////////////saveDevName: %s\n",p);
			sendResultCmd(CMD_DEV_SET_DEVNAME,0);
		}
		else
		{
			sendResultCmd(CMD_DEV_SET_DEVNAME,1);
		}
}
/*设置红外数量*/
static void  changeREDCnt(cJSON *pJson)     
{

		int num = 0;
		if(pJson==NULL)
		{
			return;
		}
		//cJSON_item_get_number(pJson,"count",&num);
		num = cJSON_GetObjectItem(pJson,"count")->valueint;
		rt_kprintf("//////////////////////////////REDCnt: %d\n",num);
		if(num!=0)
		{
			saveREDCnt(num);
    
			sendResultCmd(CMD_DEV_SET_REDCNT,0);
			rt_kprintf("//////////////////////////////CMD_DEV_SET_REDCNT = 0\n");
		}
		else
		{
			sendResultCmd(CMD_DEV_SET_REDCNT,1);
			rt_kprintf("//////////////////////////////CMD_DEV_SET_REDCNT = 1\n");		
		}
}
/*设置仪表类型*/
static void  changeWeighType(cJSON *pJson)
{
		char *p=NULL;
		int type = -1;
		if(pJson==NULL)
		{
			return;
		}
		//cJSON_item_get_number(pJson,"type",&type);
		type = cJSON_GetObjectItem(pJson,"type")->valueint;
		if(type != -1 )
		{
			switch(type){
			case IND_XXX:
				saveWeighType(IND_XXX);
			  rt_kprintf("//////////////////////////////saveWeighType(IND_XXX)");
				break;
			case DS_XXX:
				saveWeighType(DS_XXX);
				  rt_kprintf("//////////////////////////////saveWeighType(DS_XXX)");
				break;
			case MATRIX_XXX:
				saveWeighType(MATRIX_XXX);
				  rt_kprintf("//////////////////////////////saveWeighType(MATRIX_XXX)");
				break;
			case XK_XXX:
				saveWeighType(XK_XXX);
				  rt_kprintf("//////////////////////////////saveWeighType(XK_XXX)");
				break;
			default:
        break;
			};
			
			sendResultCmd(CMD_DEV_SET_WEIGHTYPE,0);
		}
		else
		{
			sendResultCmd(CMD_DEV_SET_WEIGHTYPE,1);
		}
}

static void  setDisableRMStatus(cJSON *pJson)
{
		int rm_swicth;
		if(pJson==NULL)
		{
			return;
		}
		cJSON_item_get_number(pJson,"switch",&rm_swicth);
		systemStatus.check_rm=rm_swicth;
		saveCheckRM(systemStatus.check_rm);
		sendResultCmd(CMD_DEV_DISABLE_RM,0);
}
static void  setSoundStatus(cJSON *pJson)
{
		int sound;
		if(pJson==NULL)
		{
			return;
		}
		cJSON_item_get_number(pJson,"sound",&sound);
		systemStatus.sound=sound;
		saveSound(systemStatus.sound);
		sendResultCmd(CMD_DEV_SOUND_STATUS,0);
}

///*
// * Copyright (c) 2006-2018, RT-Thread Development Team
// *
// * SPDX-License-Identifier: Apache-2.0
// *
// * Change Logs:
// * Date           Author       Notes
// * 2020-03-11     Jason        first version
// * 
// */
//#include "cmd_handle.h"
//#include <string.h>
//#include "crc.h"
//#include <netdev_ipaddr.h>
//#include "mqtt_thread.h"
//#include "com_to_server_thread.h"
//#include "cmd_table.h"
//#include <rtthread.h>
//#include "com_to_server_cmd_list.h"
//#include "cJSON.h"
//#include "cJSON_util.h"
//#include "main_task.h"
//#define MIN_LENGTH  10
//static unsigned char send_handle_flag = 0;
//static com_to_server_data_t com_rec_data_t,com_send_data_t;


//////服务器中断本次流程指令，处理函数
////static void breadTaskHandle(cJSON *pJson);
//////控制过道杆指令，处理函数
////static void breadTaskCmdHandle(cJSON *pJson);
//////临时关闭红外指令，处理函数  
////static void disableIRCmdHandle(cJSON *pJson);
//////请求放行结果，处理函数  
////static void throughResultCmdHandle(cJSON *pJson);
//static void setCarInfoCmdHandle(cJSON *pJson);
////本次流程放行，处理函数  
//static void setThroughCmdHandle(cJSON *pJson);
////本次流程中断，处理函数  
//static void setProcessBreadCmdHandle(cJSON *pJson);
//cJSON *pSerJson=NULL;
//void cmd_rec_handle(unsigned char * data,unsigned int lenth)
//{
////    unsigned short ack_status = 0;
//		
//    memset((unsigned char*)&com_rec_data_t,0,sizeof(com_to_server_data_t));
//    memcpy((unsigned char*)&com_rec_data_t,data,lenth);
//    /*字节顺序转换*/
//    com_rec_data_t.cmd = ntohs(com_rec_data_t.cmd);
//    com_rec_data_t.crc = ntohl(com_rec_data_t.crc);
//    com_rec_data_t.lenth = ntohl(com_rec_data_t.lenth);
//    /* 校验长度*/
//    if(lenth != com_rec_data_t.lenth)
//        return ;
//    
//    if(com_rec_data_t.lenth < MIN_LENGTH)
//        return;
//   
//    /* crc 校验*/
////    unsigned int crc = calculate_CRC32((unsigned char*)&com_rec_data_t.cmd,com_rec_data_t.lenth-8,0);
////    if(crc != com_rec_data_t.crc)
////        return;
//    if(com_rec_data_t.lenth>MIN_LENGTH)
//    {
//        pSerJson=cJSON_Parse((char *)com_rec_data_t.jsonStr);
//        if(pSerJson==NULL)
//        {
//             return;
//        }
//    }
//		systemStatus.mqttRxTime=rt_tick_get();
//    rt_kprintf("receive serves message,cmd=%d,len=%d,data=%s\n",com_rec_data_t.cmd,com_rec_data_t.lenth, com_rec_data_t.jsonStr);
//    switch(com_rec_data_t.cmd)
//    {
//			
//			case Cmd_DEV_REGISTER:
//				Set_Mqtt_Status(MQTT_LINKING);
//				break;
////			case CMD_SVR_BREAK_TASK:																				/*服务器中断本次流程*/
////				breadTaskHandle(pJson);
////				break;
////			case CMD_SVR_CONTROL_POLE:																			/*服务器控制过道杆*/
////				breadTaskCmdHandle(pJson);
////				break;
////			case CMD_SVR_DISABLE_IR:																				/*临时关闭红外*/
////				disableIRCmdHandle(pJson);
////				break;
////			case CMD_DEV_REQUEST_THROUGH:																		/*设备请求放行，服务器是否放行*/
////				throughResultCmdHandle(pJson);
////				break;
//			/*--------------------------------------------------------------------------------------------------------*/
//			/*--------------------------------------------------------------------------------------------------------*/
//			case CMD_SVR_QUERY_DEVSETTING:																	/*查询设备系统参数*/
//				sendDevParams();
//				break;
//			case CMD_SVR_QUERY_DEV_STATUS:																	/*查询设备系统状态*/
//				sendDevStatus(CMD_DEV_UPLOAD_STATUS);
//				break;
//			case CMD_SVR_SET_CARINFO:																				/*设置车辆信息*/
//				setCarInfoCmdHandle(pSerJson);
//				break;
//			case CMD_SVR_SET_THROUGH:																				/*设置本次流程放行*/
//				setThroughCmdHandle(pSerJson);
//				break;
//			case CMD_DEV_PROCESS_BREAK:																			/*设置本次流程中断*/
//				setProcessBreadCmdHandle(pSerJson);
//				break;
//        default:
//        break;
//    }  
//		if(pSerJson!=NULL)
//		{
//			 cJSON_Delete(pSerJson);
//			 pSerJson=NULL;
//		}
//			
//		
//}
// 

//int cmd_send_handle(char *topic,unsigned short cmd,unsigned char *jsonStr,unsigned int json_lenth)
//{
//    if(send_handle_flag == 1)
//        return 1;
//    
//    send_handle_flag = 1;//上锁
//    
//    memset((unsigned char *)&com_send_data_t.cmd,0,sizeof(com_to_server_data_t));
//    
//    com_send_data_t.cmd = cmd;
//    memcpy(&(com_send_data_t.jsonStr[0]),jsonStr,json_lenth);
//    com_send_data_t.lenth = json_lenth + MIN_LENGTH;
//		    /* 字节序转换*/
//    com_send_data_t.cmd = htons(com_send_data_t.cmd);
//    com_send_data_t.crc = calculate_CRC32((unsigned char*)&com_send_data_t.cmd,com_send_data_t.lenth-8,0xffffffff);
//    

//    com_send_data_t.crc = htonl(com_send_data_t.crc);
//    com_send_data_t.lenth = htonl(com_send_data_t.lenth);
//    
//    if(topic == NULL)
//        user_mqtt_publish_to_server((unsigned char *)&com_send_data_t,json_lenth + MIN_LENGTH);
//    else
//        user_mqtt_publish(topic,(unsigned char *)&com_send_data_t,json_lenth + MIN_LENGTH);
//    
//    send_handle_flag = 0;//开锁
//		  
//    if(topic == NULL)
//		{
//			rt_kprintf("send mqtt message:cmd=%d,jsonStr=%s\r\n",cmd,com_send_data_t.jsonStr);//
//		}
//		else
//		{
//			rt_kprintf("send mqtt message:topic=%s,cmd=%d,jsonStr=%s\n",topic,cmd,com_send_data_t.jsonStr);//
//		}
//    return 0;
//}



//#define MAC_ID 			"mac_id"
//#define DEV_NAME		 "driver_name"
//#define IP					 "ip"
//#define HW					 "hw"
//#define FW					 "fw"
//void sendRegisterMessage(void)//发送注册消息
//{
//    char  *cjson_str = NULL;
//    cJSON * root =  cJSON_CreateObject(); 
//		cJSON_AddStringToObject(root,MAC_ID,(char *)systemStatus.macidstring);//
//		cJSON_AddStringToObject(root,IP,(char *)systemStatus.ip);//gateway.ip
//		cJSON_AddStringToObject(root,DEV_NAME,(char *)systemStatus.devName);//gateway.ip
//		cJSON_AddNumberToObject(root,HW, systemStatus.hw);//
// 		cJSON_AddNumberToObject(root,FW, systemStatus.fw);//
//	
//    cjson_str =cJSON_Print(root);
//    rt_kprintf("send register message\r\n");
//    cmd_send_handle(MQTT_REG_TOPIC,Cmd_DEV_REGISTER,(unsigned char *)cjson_str,strlen(cjson_str));
//    cJSON_Delete(root);
//    cJSON_free(cjson_str);
//}
////#define KEY_ERROR  			"error"
////#define KEY_MSG  				"msg"
////#define KEY_RESULT 			"resCode"
////#define VALUE_OK  				"ok"
////#define VALUE_SUCCESS  		0
//////发送成功消息
////void sendSuccessMessage(unsigned short cmd)
////{
////		char  *cjson_str = NULL;
////    cJSON * root =  cJSON_CreateObject(); 
////		cJSON_AddNumberToObject(root,KEY_ERROR,VALUE_SUCCESS); //"error":0
////		cJSON_AddStringToObject(root,KEY_MSG,VALUE_OK);//"msg":"ok"
////    cjson_str =cJSON_Print(root);
////		rt_kprintf("send Success message\n");
////		cmd_send_handle(NULL,cmd,(unsigned char *)cjson_str,strlen(cjson_str));
////		cJSON_Delete(root);
////		rt_free(cjson_str);
////}
//////发送失败消息
////void sendErrorMessage(unsigned short cmd,int errorCode,unsigned char * errorMsg)
////{
////		char  *cjson_str = NULL;
////    cJSON * root =  cJSON_CreateObject(); 
////		cJSON_AddNumberToObject(root, KEY_ERROR,errorCode); //"error":0
////		cJSON_AddStringToObject(root,KEY_MSG,(char *)errorMsg);//"msg":"ok"
////    cjson_str =cJSON_Print(root);
////		rt_kprintf("send error message\n");
////		cmd_send_handle(NULL,cmd,(unsigned char *)cjson_str,strlen(cjson_str));
////		cJSON_Delete(root);
////		rt_free(cjson_str);
////}
//////发送结果反馈消息
////void sendResultMessage(unsigned short cmd,int resultCode)
////{
////		char  *cjson_str = NULL;
////    cJSON * root =  cJSON_CreateObject(); 
////		cJSON_AddNumberToObject(root, KEY_RESULT,resultCode);  
////		cJSON_AddNumberToObject(root,KEY_ERROR,VALUE_SUCCESS); //"error":0
////		cJSON_AddStringToObject(root,KEY_MSG,VALUE_OK);//"msg":"ok"	
////    cjson_str =cJSON_Print(root);
////		rt_kprintf("send result message\n");
////		cmd_send_handle(NULL,cmd,(unsigned char *)cjson_str,strlen(cjson_str));
////		cJSON_Delete(root);
////		rt_free(cjson_str);
////}
//////发送只有一个字符串参数的消息
////void sendCMDMessageWithStrParam(unsigned short cmd,unsigned char * paramName,unsigned char * strValue)
////{
////		char  *cjson_str = NULL;
////    cJSON * root =  cJSON_CreateObject(); 
////		cJSON_AddStringToObject(root,(char * )paramName,(char *)strValue); 
////    cjson_str =cJSON_Print(root);
////		rt_kprintf("send cmd=%d message\n",cmd);
////		cmd_send_handle(NULL,cmd,(unsigned char *)cjson_str,strlen(cjson_str));
////		cJSON_Delete(root);
////		rt_free(cjson_str);
////} 
//////发送只有一个int参数的消息
////void sendCMDMessageWithIntParam(unsigned short cmd,unsigned char * paramName, int intValue)
////{
////		char  *cjson_str = NULL;
////    cJSON * root =  cJSON_CreateObject(); 
////		cJSON_AddNumberToObject(root,( char *) paramName,intValue); //"error":0 
////    cjson_str =cJSON_Print(root);
////		rt_kprintf("send cmd=%d message\n",cmd);
////		cmd_send_handle(NULL,cmd,(unsigned char *)cjson_str,strlen(cjson_str));
////		cJSON_Delete(root);
////		rt_free(cjson_str);
////} 
//////无线标签值上传  禁用
////void  sendUploadCard(unsigned char * cardValue)
////{
////	#define KEY_CARD	"card"
////	rt_kprintf("send upload card message\n");
////	sendCMDMessageWithStrParam(CMD_DEV_UPLOAD_CARD,(unsigned char *)KEY_CARD,cardValue);

////}
//////二维码值上传	 禁用
////void  sendUploadRQConde(unsigned char * rqValue)
////{
////	#define KEY_CRQCODE	"rqcode"
////	rt_kprintf("send upload RQCond message\n");
////	sendCMDMessageWithStrParam(CMD_DEV_UPLOAD_RQCODE,(unsigned char *)KEY_CRQCODE,rqValue);
////}
//////称重数据	     禁用
////void  sendUploadWeigh(unsigned int weighValue)
////{
////	#define KEY_CWEIGH	"weigh"
////	rt_kprintf("send upload Weigh message\n");
////	sendCMDMessageWithIntParam(CMD_DEV_UPLOAD_WEIGH,(unsigned char *)KEY_CWEIGH,weighValue);
////}
//////请求拍照	    禁用
////void  sendRequestPicture(void)//unsigned int pictureValue
////{
////	#define KEY_CPICTURE	"picture"
////	rt_kprintf("send Request Picture message\n");
////	sendCMDMessageWithIntParam(CMD_DEV_REQUEST_PICTURE,(unsigned char *)KEY_CPICTURE,1);
////}
//////请求放行     禁用
////void  sendRequestProcessOver(void)
////{
////	#define KEY_CPROCESS_OVER	"process_over"
////	rt_kprintf("send Request Process Over message\n");
////	sendCMDMessageWithIntParam(CMD_DEV_REQUEST_THROUGH,(unsigned char *)KEY_CPROCESS_OVER,1);
////} 	 
//////上传小票打印状态  禁用
////void  sendUploadTicketSta(int ticketSta)
////{
////	#define KEY_CTICKET_STA	"ticketSta"
////	rt_kprintf("send Upload Ticket status message\n");
////	sendCMDMessageWithIntParam(CMD_DEV_UPLOAD_TICKET,(unsigned char *)KEY_CTICKET_STA,ticketSta);
////} 	 
//////请求销毁二维码   禁用
////void  sendRequestDesRQConde(unsigned char * rqValue)
////{
////	#define KEY_CRQCODE	"rqcode"
////	rt_kprintf("send Request Des RQConde message\n");
////	sendCMDMessageWithStrParam(CMD_DEV_REQUEST_DES_RQ,(unsigned char *)KEY_CRQCODE,rqValue);
////} 	 
////-------------------------------------------------------------------------------------------------------------- 
////以下为新协议的发送
///**
//* 发送设备参数
// *
// * @param  null
// * @return  null
// * @note		未实现
// */
//void sendDevParams(void)
//{
//		char  *cjson_str = NULL;
//    cJSON * root =  cJSON_CreateObject(); 

//    cjson_str =cJSON_Print(root);
//    cmd_send_handle(NULL,CMD_DEV_UPLOAD_SETTING,(unsigned char *)cjson_str,strlen(cjson_str));
//    cJSON_Delete(root);
//    cJSON_free(cjson_str);
//}
///**
//* 发送设备状态
// *
// * @param  null
// * @return  null
// */
//void sendDevStatus(unsigned int cmd)
//{
//		#define KEY_SYS_STA				"sysSta"
//		#define KEY_IR_STA				"irSta"
//		#define KEY_BARGATE_STA		"barGateSta"
//		#define KEY_WEIGH					"weigh"
//		#define KEY_RQCODE_STA		"rqcode"
//		#define KEY_CARD_STA			"card"
//		#define KEY_MONIT_STA			"monitSta"
//		#define KEY_PRINT_STA			"printSta"
//		#define KEY_MSG_ID				"msg_id"
//		#define KEY_FLOW_ID				"process_id"
//		char  *cjson_str = NULL;
//		if(pSerJson!=NULL)
//		{
//			cJSON_Delete(pSerJson);
//			pSerJson=NULL;
//		}
//    cJSON * root =  cJSON_CreateObject(); 
//	
//		cJSON_AddNumberToObject(root,( char *) KEY_SYS_STA,systemStatus.status); 
//		cJSON_AddNumberToObject(root,( char *) KEY_IR_STA,systemStatus.irStatus); 
//		cJSON_AddNumberToObject(root,( char *) KEY_BARGATE_STA,systemStatus.devStatus.roadGate); 
//		cJSON_AddNumberToObject(root,( char *) KEY_WEIGH,systemStatus.weight); 
//		cJSON_AddNumberToObject(root,( char *) KEY_MONIT_STA,systemStatus.monitSta); 
//		cJSON_AddNumberToObject(root,( char *) KEY_PRINT_STA,systemStatus.printSta); 
//	
//		cJSON_AddNumberToObject(root,( char *) KEY_MSG_ID,(systemStatus.msgId++)); 
//		cJSON_AddNumberToObject(root,( char *) KEY_FLOW_ID,systemStatus.processID); 
//	
//		cJSON_AddStringToObject(root,( char *) KEY_RQCODE_STA,(char *)systemStatus.rqCode); 
//		cJSON_AddStringToObject(root,( char *) KEY_CARD_STA,(char *)systemStatus.bleTag); 

//    cjson_str =cJSON_Print(root);
//		if(cjson_str!=NULL)
//		{
//			rt_kprintf("send cmd=%d Dev Status message\n",cmd);
//			cmd_send_handle(NULL,cmd,(unsigned char *)cjson_str,strlen(cjson_str));
//		}
//		else
//		{
//			rt_kprintf("send cmd=%d fail,cjson_str = null\n",cmd);
//		}
//		cJSON_Delete(root);
//		cJSON_free(cjson_str);
//}

////--------------------------------------------------------------------------------------------------------------
////接收处理函数
////-------------------------------------------------------------------------------------------------------------- 
////服务器中断本次流程指令，处理函数
////static void breadTaskHandle(cJSON *pJson)
////{
////		int breakProcess=0;//是否打断流程
////		if(pJson==NULL)
////		{
////			return;
////		}
////		cJSON_item_get_number(pJson,"breakProcess",&breakProcess);
////		if(breakProcess)//打断本次流程
////		{
////		
////		}
////		sendSuccessMessage(CMD_SVR_BREAK_TASK);
////}
////控制过道杆指令，处理函数
////static void breadTaskCmdHandle(cJSON *pJson)
////{
////		int controlPole=0;
////		if(pJson==NULL)
////		{
////			return;
////		}
////		cJSON_item_get_number(pJson,"controlPole",&controlPole);
////		if(controlPole)//打开过道杆		
////		{
////		
////		}
////		else					//关闭过道杆
////		{
////		
////		}
////		sendSuccessMessage(CMD_SVR_CONTROL_POLE);
////}
////临时关闭红外指令，处理函数  
////static void disableIRCmdHandle(cJSON *pJson)
////{
////		int disableInfrared=0;
////		if(pJson==NULL)
////		{
////			return;
////		}
////		cJSON_item_get_number(pJson,"disableInfrared",&disableInfrared);
////		if(disableInfrared)
////		{//本次停用红外
////		
////		}
////		else
////		{//不停用红外
////		
////		}
////		sendSuccessMessage(CMD_SVR_DISABLE_IR);
////}
////请求放行结果，处理函数  
////static void throughResultCmdHandle(cJSON *pJson)
////{
////		int enThrough=0;
////		if(pJson==NULL)
////		{
////			return;
////		}
////		cJSON_item_get_number(pJson,"enThrough",&enThrough);
////		if(enThrough)
////		{//允许放行
////		
////		}
////		else
////		{//不允许放行
////		
////		} 

////}
////-------------------------------------------------------------------------------------------------------------- 
////以下为新协议的处理
////设置流程开始（车辆信息），处理函数  
//static void setCarInfoCmdHandle(cJSON *pJson)
//{
//		char* carNum=NULL;
//		char* nextCarNum=NULL;
//	  int processID=0;
//		int disableInfrared=0;
//		if(pJson==NULL)
//		{
//			return;
//		}
//		carNum=( char *)cJSON_item_get_string(pJson,"carNum");
//		nextCarNum=( char *)cJSON_item_get_string(pJson,"nextNum");
//		cJSON_item_get_number(pJson,"process_id",&processID);
//		cJSON_item_get_number(pJson,"disable_infrared",&disableInfrared);
//		rt_kprintf("processID=%d,disableInfrared=%d,carNum=%s,nextNum=%s\n",processID,disableInfrared,carNum,nextCarNum);
//		if((processID==0)||(systemStatus.status!=SYSTME_FREE))//
//		{
//			sendDevStatus(CMD_DEV_UPLOAD_CARINFO);
//			return;
//		}
//		systemStatus.processID=processID;
//		systemStatus.disableIr=disableInfrared;
//		if(carNum!=NULL)
//		{ 
//			rt_memcpy(systemStatus.carNum,carNum,strlen(carNum));
//		}
//		if(nextCarNum!=NULL)
//		{ 
//			rt_memcpy(systemStatus.nextNum,nextCarNum,strlen(nextCarNum));
//		} 
////		changeSystemStatus(SPEAKER_BUYS);
//		//sendResultMessage(CMD_DEV_UPLOAD_CARINFO,VALUE_SUCCESS);
//		sendDevStatus(CMD_DEV_UPLOAD_CARINFO);
//}
////本次流程放行，处理函数  
//static void setThroughCmdHandle(cJSON *pJson)
//{
//	int processID=0;
//	int isPrint=0;
//	 char *p=NULL;
//	if(pJson==NULL)
//	{
//		return;
//	}
//	cJSON_item_get_number(pJson,"process_id",&processID);
//	if(processID==systemStatus.processID)
//	{
//		//changeSystemStatus(SPEAKER_END_ING);
//		systemStatus.isThrough=1;
//		cJSON_item_get_number(pJson,"isPrint",&isPrint);
//		if(isPrint)//如果需要打印信息
//		{
//			systemStatus.isPrint=1;
//			rt_memset(&systemStatus.printInfo,0,sizeof(PrintInfo_t));
//			//--------------------
//			p=NULL;
//			p=(char *)cJSON_item_get_string(pJson,"title");
//			if(p!=NULL)
//			{
//				rt_memcpy(systemStatus.printInfo.title,p,strlen(p));
//			}
//			//--------------------
//			p=NULL;
//			p=(char *)cJSON_item_get_string(pJson,"SN");
//			if(p!=NULL)
//			{
//				rt_memcpy(systemStatus.printInfo.sn,p,strlen(p));
//			}
//			//--------------------
//			p=NULL;
//			p=(char *)cJSON_item_get_string(pJson,"GN");
//			if(p!=NULL)
//			{
//				rt_memcpy(systemStatus.printInfo.gn,p,strlen(p));
//			}
//			//--------------------
//			p=NULL;
//			p=(char *)cJSON_item_get_string(pJson,"SP");
//			if(p!=NULL)
//			{
//				rt_memcpy(systemStatus.printInfo.sp,p,strlen(p));
//			}
//			//--------------------
//			p=NULL;
//			p=(char *)cJSON_item_get_string(pJson,"RU");
//			if(p!=NULL)
//			{
//				rt_memcpy(systemStatus.printInfo.ru,p,strlen(p));
//			}
//			//--------------------
//			p=NULL;
//			p=(char *)cJSON_item_get_string(pJson,"FU");
//			if(p!=NULL)
//			{
//				rt_memcpy(systemStatus.printInfo.fu,p,strlen(p));
//			}
//			//--------------------
//			p=NULL;
//			p=(char *)cJSON_item_get_string(pJson,"GW");
//			if(p!=NULL)
//			{
//				rt_memcpy(systemStatus.printInfo.gw,p,strlen(p));
//			}
//			//--------------------
//			p=NULL;
//			p=(char *)cJSON_item_get_string(pJson,"TW");
//			if(p!=NULL)
//			{
//				rt_memcpy(systemStatus.printInfo.tw,p,strlen(p));
//			}
//			//--------------------
//			p=NULL;
//			p=(char *)cJSON_item_get_string(pJson,"NW");
//			if(p!=NULL)
//			{
//				rt_memcpy(systemStatus.printInfo.nw,p,strlen(p));
//			} 
//			//--------------------
//			p=NULL;
//			p=(char *)cJSON_item_get_string(pJson,"GWT");
//			if(p!=NULL)
//			{
//				rt_memcpy(systemStatus.printInfo.gwt,p,strlen(p));
//			}
//			//--------------------
//			p=NULL;
//			p=(char *)cJSON_item_get_string(pJson,"NWT");
//			if(p!=NULL)
//			{
//				rt_memcpy(systemStatus.printInfo.nwt,p,strlen(p));
//			}
//			//--------------------
//			p=NULL;
//			p=(char *)cJSON_item_get_string(pJson,"SIB");
//			if(p!=NULL)
//			{
//				rt_memcpy(systemStatus.printInfo.sib,p,strlen(p));
//			}
//		}
//	}
//	sendDevStatus(CMD_DEV_UPLOAD_THROUGH);
//	// sendResultMessage(CMD_DEV_UPLOAD_THROUGH,VALUE_SUCCESS);
//}
//static void setProcessBreadCmdHandle(cJSON *pJson)
//{ 
//		int processID=0;
//		if(pJson==NULL)
//		{
//			return;
//		}
//		cJSON_item_get_number(pJson,"process_id",&processID);
//		if(processID==systemStatus.processID)
//		{
//			changeSystemStatus(SYSTME_FREE);
//		}
//		sendDevStatus(CMD_DEV_PROCESS_BREAK);
//}
