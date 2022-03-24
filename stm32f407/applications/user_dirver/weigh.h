#ifndef WEIGH_H_H
#define WEIGH_H_H
#include "rs485.h"

#define  IND_XXX			0	//INDϵ�� 
#define  DS_XXX				1 //DSϵ��
#define  MATRIX_XXX		2 //MATRIX  II������SCALE1  250MS F10 9600 8Bits CR RS-485
#define  XK_XXX				3 //xk3102d1


#define WEIGH_TYPE    IND_XXX

#define WEIGH_485_CHANNLE 	3
#define WEIGH_485_NUM				1	
 
#define WEIGH_UART_INDEX		CHANNLE4_UART_INDEX	

int Weigh_Init(void);
int weightRXhandel(double* weight_value,double* tare_value);					//���ؽ��մ���
//int Weigh_Read(double* weight_value,double* tare_value);
int Weigh_Wirte(unsigned char* data,unsigned int len);
void setWeighRead(void);
 
#endif
