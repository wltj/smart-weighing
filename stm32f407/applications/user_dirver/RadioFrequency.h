#ifndef __RADIOFREQUENCY_H_
#define __RADIOFREQUENCY_H_
#include "rs485.h"

#define RADIOFREQUENCY_485_CHANNLE 		1
#define RADIOFREQUENCY_485_NUM				0
#define RADIOFREQUENCY_UART_INDEX		CHANNLE2_UART_INDEX	
//≤‚ ‘
#define MAX_CARD_COUNT					5
#define RADIOFREQUENCY_BUFF_LEN		30

void radioFrequencyInit(void);
int radioFrequencyRXHandel(void);

void sendQueryCardCmd1(void);
//void sendQueryCardCmd2(void);

void rxCardData(unsigned char *buff);
#endif

