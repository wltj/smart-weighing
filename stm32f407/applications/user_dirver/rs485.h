#ifndef RS485_H_H
#define RS485_H_H

#include "uart_util.h"

#define RS485_ENABLE    1
#define RS485_DISABLE   0

#define RS485_READ  0
#define RS485_WRITE 1


#define CHANNLE1_UART_INDEX		UART6_INDEX
#define CHANNLE2_UART_INDEX		UART3_INDEX
#define CHANNLE3_UART_INDEX		UART4_INDEX
#define CHANNLE4_UART_INDEX		UART5_INDEX

int Rs485_Wirte(unsigned char channle,unsigned char num,unsigned char* data,unsigned int len);
UART_RX_MSG * Rs485_SetRead(unsigned char channle,unsigned char num);
UART_RX_MSG * Rs485_GetUartStatus(unsigned char channle);
int Init_485(void); 
int Get_Rs485_Init_Flag(void);
void test485(void);
#endif
