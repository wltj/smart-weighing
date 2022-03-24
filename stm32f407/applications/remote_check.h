#ifndef __REMOTE_CHECK_H_
#define __REMOTE_CHECK_H_
#include "rs485.h"
//’Ê
#define REMOTECHECK_485_CHANNLE 	2
#define REMOTECHECK_485_NUM				1	
#define REMOTE_CHECK_UART_INDEX		CHANNLE3_UART_INDEX	
//≤‚ ‘
//#define REMOTECHECK_485_CHANNLE 	1
//#define REMOTECHECK_485_NUM				0	
//#define REMOTE_CHECK_UART_INDEX		CHANNLE2_UART_INDEX
int remoteCheckRXhandel(unsigned char *pWarning,unsigned char *WarningType,unsigned short * pWarningCh);
void queryRemoteCheckStatus(void);
void resetRemoteCheckState(void);

void temoteCheckInit(void);
#endif
