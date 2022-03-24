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




//���������͵�ָ��
typedef enum  
{
//    CMD_SVR_BREAK_TASK				=		503,		//�������жϱ�������	
//    CMD_SVR_CONTROL_POLE			=		504,		//���������ƹ�����	
//    CMD_SVR_DISABLE_IR				=		505,		//��ʱ�رպ���
		//----Э��2--------------
		CMD_SVR_QUERY_DEVSETTING	=		1001,		//��ѯ�豸ϵͳ����
		CMD_SVR_QUERY_DEV_STATUS	=		1002,		//��ѯ�豸ϵͳ״̬
		CMD_SVR_SET_CARINFO				=		1003,		//���̿���ָ��
		CMD_SVR_SET_THROUGH				=		1004,		//���ñ������̷���
		CMD_SVR_PROCESS_BREAK			=		1005,		//�����ж�ָ��
		CMD_SVR_CHANGECARNUM			=		1006,		//[�޸�չʾ����]
		CMD_SVR_OPEN_TASK					=		1007,		//�������̿������
		CMD_SVR_DISABLE_INFRARED	=		1008,		//��;��ֹ���ߴ򿪺�������
		CMD_SVR_SHOW_ERROR				=		1009,		//��ʾ������Ϣ
		CMD_SVR_SET_DEVNAME				=		1050,		// �����豸����
		CMD_SVR_DISABLE_RM				=		1051,		// �Ƿ��ֹң�ؼ��
		CMD_SVR_SOUND_STATUS			=		1052,		// �Ƿ��ֹң�ؼ��	 
		CMD_SVR_PUSH_UPGRADE			=		3000,		//����������Ϣ

}TYPE_SVR_CMD;
//���ư巢�͵�ָ��
typedef enum  
{
		//--
		Cmd_DEV_REGISTER					=		100,			//ע��
		//----Э��2--------------
		CMD_DEV_UPLOAD_SETTING		=		2001,		//�豸�ϱ�ϵͳ����
		CMD_DEV_UPLOAD_STATUS			=		2002,		//�豸�ϱ�ϵͳ״̬
		CMD_DEV_UPLOAD_CARINFO		=		2003,		//�豸�ϱ�[���̿�ʼ]���
		CMD_DEV_UPLOAD_THROUGH		=		2004,		//�豸�ϱ�[����ָ��]���
		CMD_DEV_PROCESS_BREAK	  	=		2005,		//�豸�ϱ�[�����ж�]���
		CMD_DEV_CHANGECARNUM			=		2006,		//�豸�ϱ�[�޸�չʾ����]���
		CMD_DEV_OPEN_TASK					=		2007,		//�������̿���
		CMD_DEV_DISABLE_INFRARED	=		2008,		//��;��ֹ���ߴ򿪺�������
		CMD_DEV_SHOW_ERROR				=		2009,		//������ʾ����
		CMD_DEV_SEND_OUTTIME			=		2010,		//��ʱ
		CMD_DEV_SET_DEVNAME				=		2050,		// �����豸����
		CMD_DEV_DISABLE_RM				=		2051,		// �Ƿ��ֹң�ؼ��
		CMD_DEV_SOUND_STATUS			=		2052,		// �Ƿ��ֹң�ؼ��	 
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

void sendRegisterMessage(void);//����ע����Ϣ
//���ͳɹ���Ϣ
void sendSuccessMessage(unsigned short cmd);
//����ʧ����Ϣ
void sendErrorMessage(unsigned short cmd,int errorCode,unsigned char * errorMsg);
//���ͽ��������Ϣ
void sendResultMessage(unsigned short cmd,int resultCode);
//����ֻ��һ���ַ�����������Ϣ 
void sendCMDMessageWithStrParam(unsigned short cmd,unsigned char * paramName,unsigned char * strValue);
//����ֻ��һ��int��������Ϣ
void sendCMDMessageWithIntParam(unsigned short cmd,unsigned char * paramName, int intValue);
//-------------------------------------------------------------------------------------------------------------- 
//���߱�ǩֵ�ϴ�
void  sendUploadCard(unsigned char * cardValue);
//��ά��ֵ�ϴ�	
void  sendUploadRQConde(unsigned char * rqValue);
//��������	
void  sendUploadWeigh(unsigned int weighValue);
//��������	
void  sendRequestPicture(void);
//�������
void  sendRequestProcessOver(void);
//�ϴ�СƱ��ӡ״̬
void  sendUploadTicketSta(int ticketSta);
//�������ٶ�ά��
void  sendRequestDesRQConde(unsigned char * rqValue);

//-------------------------------------------------------------------------------------------------------------- 
//����Ϊ��Э��ķ���
//�����豸������δʵ��
void sendDevParams(void);
//�����豸״̬
void sendDevStatus(unsigned int cmd);
#endif
