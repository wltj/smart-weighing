#ifndef WEIGH_H_H
#define WEIGH_H_H
#include "rs485.h"

#define  IND_XXX			0	//IND系列 
#define  DS_XXX				1 //DS系列
#define  MATRIX_XXX		2 //MATRIX  II参数：SCALE1  250MS F10 9600 8Bits CR RS-485
#define  XK_XXX				3 //xk3102d1


#define WEIGH_TYPE    IND_XXX

#define WEIGH_485_CHANNLE 	3
#define WEIGH_485_NUM				1	
 
#define WEIGH_UART_INDEX		CHANNLE4_UART_INDEX	

int Weigh_Init(void);
int weightRXhandel(double* weight_value,double* tare_value);					//称重接收处理
//int Weigh_Read(double* weight_value,double* tare_value);
int Weigh_Wirte(unsigned char* data,unsigned int len);
void setWeighRead(void);
 
#endif
