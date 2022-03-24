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
#ifndef CMD_HANDLE
#define CMD_HANDLE
#define COM_BUF_LENGTH  2000




//服务器发送的指令
typedef enum  
{
//    CMD_SVR_BREAK_TASK				=		503,		//服务器中断本次流程	
//    CMD_SVR_CONTROL_POLE			=		504,		//服务器控制过道杆	
//    CMD_SVR_DISABLE_IR				=		505,		//临时关闭红外
		//----协议2--------------
		CMD_SVR_QUERY_DEVSETTING	=		1001,		//查询设备系统参数
		CMD_SVR_QUERY_DEV_STATUS	=		1002,		//查询设备系统状态
		CMD_SVR_SET_CARINFO				=		1003,		//流程开启指令
		CMD_SVR_SET_THROUGH				=		1004,		//设置本次流程放行
		CMD_SVR_PROCESS_BREAK			=		1005,		//流程中断指令
		CMD_SVR_CHANGECARNUM			=		1006,		//[修改展示车牌]
		CMD_SVR_OPEN_TASK					=		1007,		//返回流程开启结果
		CMD_SVR_DISABLE_INFRARED	=		1008,		//中途禁止或者打开红外限制
		CMD_SVR_SHOW_ERROR				=		1009,		//显示错误信息
		CMD_SVR_SET_DEVNAME				=		1050,		// 设置设备名称
		CMD_SVR_DISABLE_RM				=		1051,		// 是否禁止遥控监测
		CMD_SVR_SOUND_STATUS			=		1052,		// 是否禁止遥控监测	 
		CMD_SVR_PUSH_UPGRADE			=		3000,		//推送升级信息

}TYPE_SVR_CMD;
//控制板发送的指令
typedef enum  
{
		//--
		Cmd_DEV_REGISTER					=		100,			//注册
		//----协议2--------------
		CMD_DEV_UPLOAD_SETTING		=		2001,		//设备上报系统参数
		CMD_DEV_UPLOAD_STATUS			=		2002,		//设备上报系统状态
		CMD_DEV_UPLOAD_CARINFO		=		2003,		//设备上报[流程开始]结果
		CMD_DEV_UPLOAD_THROUGH		=		2004,		//设备上报[放行指令]结果
		CMD_DEV_PROCESS_BREAK	  	=		2005,		//设备上报[流程中断]结果
		CMD_DEV_CHANGECARNUM			=		2006,		//设备上报[修改展示车牌]结果
		CMD_DEV_OPEN_TASK					=		2007,		//请求流程开启
		CMD_DEV_DISABLE_INFRARED	=		2008,		//中途禁止或者打开红外限制
		CMD_DEV_SHOW_ERROR				=		2009,		//返回显示错误
		CMD_DEV_SEND_OUTTIME			=		2010,		//超时
		CMD_DEV_SET_DEVNAME				=		2050,		// 设置设备名称
		CMD_DEV_DISABLE_RM				=		2051,		// 是否禁止遥控监测
		CMD_DEV_SOUND_STATUS			=		2052,		// 是否禁止遥控监测	 
}TYPE_DEV_CMD;
 




#pragma pack(1)
typedef struct com_to_server_data_type
{
    unsigned int lenth;
    unsigned int crc;
    unsigned short cmd;
    unsigned char  jsonStr[COM_BUF_LENGTH];
}com_to_server_data_t;
#pragma pack()
void cmd_rec_handle(unsigned char * data,unsigned int lenth);
int cmd_send_handle(char *topic,unsigned short cmd,unsigned char *jsonStr,unsigned int json_lenth);

void sendRegisterMessage(void);//发送注册消息
//发送成功消息
void sendSuccessMessage(unsigned short cmd);
//发送失败消息
void sendErrorMessage(unsigned short cmd,int errorCode,unsigned char * errorMsg);
//发送结果反馈消息
void sendResultMessage(unsigned short cmd,int resultCode);
//发送只有一个字符串参数的消息 
void sendCMDMessageWithStrParam(unsigned short cmd,unsigned char * paramName,unsigned char * strValue);
//发送只有一个int参数的消息
void sendCMDMessageWithIntParam(unsigned short cmd,unsigned char * paramName, int intValue);
//-------------------------------------------------------------------------------------------------------------- 
//无线标签值上传
void  sendUploadCard(unsigned char * cardValue);
//二维码值上传	
void  sendUploadRQConde(unsigned char * rqValue);
//称重数据	
void  sendUploadWeigh(unsigned int weighValue);
//请求拍照	
void  sendRequestPicture(void);
//请求放行
void  sendRequestProcessOver(void);
//上传小票打印状态
void  sendUploadTicketSta(int ticketSta);
//请求销毁二维码
void  sendRequestDesRQConde(unsigned char * rqValue);

//-------------------------------------------------------------------------------------------------------------- 
//以下为新协议的发送
//发送设备参数：未实现
void sendDevParams(void);
//发送设备状态
void sendDevStatus(unsigned int cmd);
#endif
