#ifndef LED_COM_H
#define LED_COM_H


#include "rs485.h"

 


#define PACK_DATA_LEN_MAX   1018
#define PACK_HEAD           0xa0

#define LED_485_CHANNLE 	1
#define LED_485_NUM				1		
#define LED_UART_INDEX		CHANNLE2_UART_INDEX	
/*
    ���ṹ����
*/
typedef struct led_com_pack_type
{
    unsigned char head;
    unsigned char pack_head_and_id_high_4;
    unsigned char id_low_8;
    unsigned char cmd;
    unsigned short pack_length; //���ֽ���ǰ ���������Ŀ�ʼ�� 6 ���ֽں�����У���ֽ�
    unsigned char data[PACK_DATA_LEN_MAX];//�����λΪУ��λ
}
led_com_pack_t;


void Led_Show_String_8(unsigned char channle ,unsigned char *data,unsigned short len);
void Led_Show_String_16(unsigned char channle ,unsigned short *data,unsigned short len);
void Led_Show_String_str(unsigned char channle ,unsigned char *data,unsigned short len);
void Led_Init(void);

//��ʾ��ǰ����
void LEDShowFree(void);
//��ʾϵͳ����
void LEDShowOffLine(void);
void LEDShowError1(void);
void LEDShowError2(void);
void LEDShowError3(void);
void LEDShowError4(void);
void LEDShowError5(void);
void LEDShowError6(void);
void LEDShowError7(void);
void LEDShowSevErrorInfo(unsigned char * carNum,unsigned char *errorInfo);
//XX���ڳ���
void LEDShowWeighedUp(unsigned char * carNum,unsigned char *nextCarNum);
void LEDShowWeighedUp2(unsigned char * carNum, int  weight,unsigned char dir);
void LEDShowWeighedUp3(unsigned char * carNum);
void LedShowLocationErrorInfo(unsigned char * carNum);
void LEDShowWeighedUp4(unsigned char * carNum);
void LEDShowWeighedUp5(unsigned char * carNum);
void LEDShowWeighedUp6(unsigned char * carNum);
void LEDShowQualityTestingFail(unsigned char * carNum);
void SoundPayData( char * payData,unsigned short len);
void SoundPayStop( void);
#endif
