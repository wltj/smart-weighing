#ifndef __VLPR_H_
#define __VLPR_H_
#include "rs485.h"

#define VLPR_485_CHANNLE 		0
#define VLPR_485_NUM				0
#define VLPR_UART_INDEX		CHANNLE1_UART_INDEX	

#define VLPR_BUFF_LEN 40
void vlprInit(void); 
int vlprRXHandel(unsigned char * carNumBuff,unsigned char *newVlpr);
#endif

