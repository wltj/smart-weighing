#ifndef PRINTER_H
#define PRINTER_H
#include "rs485.h"
#define PRINTER_485_CHANNLE 	3
#define PRINTER_485_NUM				0
#define PRINTER_UART_INDEX		CHANNLE4_UART_INDEX	

#define PRINT_PACK_DATA_LEN_MAX   64
/*
    包结构定义
*/
typedef struct print_pack_type
{
    unsigned char data[PRINT_PACK_DATA_LEN_MAX];
    unsigned char print_cmd[2]; //打印并走纸
	  unsigned char distance;	//走纸距离
		
}
print_pack_t;

extern int Print_Init(void);
void printReceiptInfo(void);//打印小票
void Print_Number_String_Print_8(unsigned char *data,unsigned char len);
void Printer_Content_Print(unsigned char *data,unsigned char len);
void Print_Line(void);
//extern int Init_Printer(void);


#endif
