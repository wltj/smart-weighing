#ifndef __PCB_EXPAND_H_
#define __PCB_EXPAND_H_
#include "rs485.h"
#define PCBEXPAND_485_CHANNLE 	2
#define PCBEXPAND_485_NUM				0
#define PCBEXPAND_UART_INDEX		CHANNLE3_UART_INDEX	
void queryPcbExpandStatus(void);
void pcbExpandInit(void);
int pcbExpandRXHandel(unsigned char * rqCodeBuf,unsigned int len,unsigned char * rq_updata_flag,unsigned char * vlprBuf,unsigned int vlprBuf_len,unsigned char * vlpr_updata_flag);
void sendLedInfo(unsigned char *buff,unsigned char len);
void sendPrinthInfo(unsigned char *buff,unsigned char len);
#endif
