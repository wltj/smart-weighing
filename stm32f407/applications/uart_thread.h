#ifndef __UART_THREAD_H_
#define __UART_THREAD_H_
#include "bt.h"
#include "rq_code_rec.h"
#include "vlpr.h"
#include "RadioFrequency.h"

#define EN_EXPAND_PCB   1  //�Ƿ�ʹ����չPCB

typedef struct weighing_type
{
		//------------------------
    unsigned char rq_code_fresh;     							//rqcode ˢ��---��0
    unsigned char rq_code_buf[MAX_RQ_CODE_BUF]; 
		//------------------------
		unsigned char pcb_fresh;     							//rqcode ˢ��---��0
		unsigned int  pcb_fresh_time;
		unsigned char pcb_offline;
		unsigned char pcb_rq_fresh;  
    unsigned char pcb_rq_buf[MAX_RQ_CODE_BUF]; 
		unsigned char pcb_vlpr_fresh;  
    unsigned char pcb_vlpr_buf[VLPR_BUFF_LEN]; 
		unsigned char pcb_vlpr_offline;
		//------------------------
    unsigned char weigh_fresh;
		unsigned int  weigh_fresh_time;
		unsigned char weigh_offline;
    double weight_value;//15.2KG
    double tare_value;	
		unsigned char  sign ;//����
	
		//------------------------
		unsigned char warning;//�������� 0 �޾���  1�о���  2����
		unsigned char warningType;//�������� 0 �Զ�ģʽ���ǰ�����   1�Զ�ģʽ��������  2���ģʽ
		unsigned short warningCh;//�����źŵ�CH
		unsigned char warning_fresh;
		unsigned int  warning_fresh_time;
		unsigned char warning_offline;
		//------------------------
		unsigned char vlpr_fresh;
		unsigned int  vlpr_fresh_time;
		unsigned char vlpr_offline;
		unsigned char vlpr_buf[VLPR_BUFF_LEN]; 
		unsigned char new_vlpr;
		//--------------------------
		unsigned char radioFrequency_fresh;
		unsigned int  radioFrequency_fresh_time;
		unsigned char radioFrequency_offline;
		int zhongshu_w;

}
weighing_t;
extern weighing_t weighing;
int uartThreadInit(void);
void setCommunicationStatus(unsigned char value);
void closeReadAdioFrequency(void);
void openReadAdioFrequency(void);

unsigned char checkRQ(unsigned char * buff,unsigned char * dir);
void checkVlpr(unsigned char * buff,unsigned char dir);
//void checkRadioFrequency(unsigned char ** buff,unsigned char *num);
void openQueryRadioFrequency(void);
void closeQueryRadioFrequency(void);
#endif
