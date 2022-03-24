#ifndef __UART_UTIL_H_
#define __UART_UTIL_H_
#include <board.h>
#define UART1_INDEX	0
#define UART3_INDEX	1
#define UART4_INDEX	2
#define UART5_INDEX	3
#define UART6_INDEX	4
/* 串口接收消息结构*/
typedef struct rx_msg
{
		char 				*rx_buffer;
		unsigned short rx_len;
		unsigned int 	 rxTime;
		//unsigned char  rxFlag;	
}UART_RX_MSG;

int Uart1_Init(unsigned int rate);
int Uart1_Wirte(unsigned char* data,unsigned int len);
int Uart3_Init(unsigned int rate);
int Uart3_Wirte(unsigned char* data,unsigned int len);
int Uart4_Init(unsigned int rate);
int Uart4_Wirte(unsigned char* data,unsigned int len);
int Uart5_Init(unsigned int rate);
int Uart5_Wirte(unsigned char* data,unsigned int len); 
int Uart6_Init(unsigned int rate);
int Uart6_Wirte(unsigned char* data,unsigned int len); 

UART_RX_MSG * getUartStatus(int index);
void clearRXMsg(int index);
int Uart_Init(int index,unsigned int rate);
int Uart_Wirte(int index,unsigned char* data,unsigned int len);
rt_device_t getUartDevice(int index);
 unsigned char orCheck(unsigned char *buff,unsigned char len);
void setReadClose(int index,unsigned char closeSwitch);
#endif
