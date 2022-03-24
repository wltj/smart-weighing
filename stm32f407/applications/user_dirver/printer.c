#include "printer.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <string.h>
#include "TipsData.h"
#include "uart_util.h"
#include "pcb_expand.h"
#include "main_task.h"
#include "uart_thread.h"
#include "weigh.h"
#define SAMPLE_UART_232_NAME    "uart5"
print_pack_t print_pack;

unsigned char Number[] = {0x30,0x31,0x32,0x33,0x33,0x33,0x33,0x33,0x33};
unsigned char Text[] = "Hello world";//��ת��Ϊ16����
/*��ԭ����*/
unsigned char Chinese[] = "C4C1D4ADBCAFCDC5";//(GB18030)
/*��������ģʽָ��*/
unsigned char ModeSwitchOn[] = {0x1C,0x26};
unsigned char ModeSwitchOff[] = {0x1C,0x2E};


void String_To_Hex(unsigned char *str ,unsigned char *data_hex,unsigned char len)
{
    unsigned int i = 0;
    for(;i<len;i++)
    {
        data_hex[i] = Char_To_Hex(&str[i*2]);
    }
}


int Print_Send(unsigned char* data,unsigned int len)
{
	return	Rs485_Wirte(PRINTER_485_CHANNLE,PRINTER_485_NUM,data,len);
}

int Print_Send2(unsigned char* data,unsigned int len)
{
	sendPrinthInfo(data,len);
	return 0;
}

void Print_Number_String_Print_8(unsigned char *data,unsigned char len)
{
    memset(&print_pack,0,sizeof(print_pack));
    if(data[0]==0)
		{
			return;
		}
		int i = 0;
    
    /*��䱨������*/
		for(i = 0; i < len; i++)
		{
			print_pack.data[i] = data[i]; 
		}
		
		print_pack.data[len]= 0x1B;		//1B 64  n
		print_pack.data[len+1] = 0x4A;
		print_pack.data[len+2] = 0x50; //��ֽ����ɱ�
    if(systemStatus.rqDir)
		{
			Print_Send((unsigned char *)&print_pack,len+3);
		}
		else
		{	
			Print_Send((unsigned char *)&print_pack,len+3);
		}
		//
		
}
void Print_Line(void)
{
    memset(&print_pack,0,sizeof(print_pack));
		print_pack.data[0]= 0x0A;		
		//Print_Send((unsigned char *)&print_pack,len+3);
		if(systemStatus.rqDir)
		{
			Print_Send((unsigned char *)&print_pack,1);
		}
		else
		{	
			Print_Send((unsigned char *)&print_pack,1);
		}
}
void Printer_Content_Print(unsigned char *data,unsigned char len)
{
		Print_Send(ModeSwitchOn,2);
		len = len/2;
    memset(&print_pack,0,sizeof(print_pack));
		print_pack.data[len]= 0x1B;
		print_pack.data[len+1] = 0x4A;
		print_pack.data[len+2] = 0x50; //��ֽ����ɱ�
		
		
    /*��䱨������*/
    String_To_Hex(data,print_pack.data,len);
    
		if(systemStatus.rqDir)
		{
			Print_Send((unsigned char *)&print_pack,1);
		}
		else
		{	
			Print_Send((unsigned char *)&print_pack,1);
		}
}

int Print_Init(void)
{
    
    return Uart5_Init(BAUD_RATE_9600);
}

static void printLineStr(unsigned char * data,unsigned int len)
{
	#define DELAY_TIME 	500
	Print_Number_String_Print_8(data,len);
	rt_thread_mdelay(DELAY_TIME);// 
}
 void printReceiptInfo(void)
{

	unsigned char buff[100]={0};
	if(systemStatus.rqDir==1)//
	{
		setCommunicationStatus(1);
	}
	rt_thread_mdelay(500);//�ȴ�1��
	//if()
	//����
	#define SEPARATOR	"----------------------------"
	printLineStr(systemStatus.printInfo.title,rt_strlen((char *)(systemStatus.printInfo.title))); 
	printLineStr((unsigned char *)SEPARATOR,rt_strlen((char *)(SEPARATOR))); 
	//��ӡ����
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"��ﵥ��:%s",systemStatus.printInfo.sn);
	printLineStr(buff,rt_strlen((char *)buff)); 
	//��ӡ����
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"����:%s",systemStatus.printInfo.car);
	printLineStr(buff,rt_strlen((char *)buff));
	//������
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"������:%s",systemStatus.printInfo.gn);
	printLineStr(buff,rt_strlen((char *)buff));
	//���
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"���:%s",systemStatus.printInfo.sp);
	printLineStr(buff,rt_strlen((char *)buff));
	//�ջ���λ
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"�ջ���λ:%s",systemStatus.printInfo.ru);
	printLineStr(buff,rt_strlen((char *)buff));
	//��Ӧ��
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"��Ӧ��:%s",systemStatus.printInfo.fu);
	printLineStr(buff,rt_strlen((char *)buff));
	//ë��
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"ë��(KG):%s",systemStatus.printInfo.gw);
	printLineStr(buff,rt_strlen((char *)buff)); 
	//Ƥ��
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"Ƥ��(KG):%s",systemStatus.printInfo.tw);
	printLineStr(buff,rt_strlen((char *)buff)); 
	//����
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"����(KG):%s",systemStatus.printInfo.nw);
	printLineStr(buff,rt_strlen((char *)buff)); 
	//ë��ʱ��
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"ë��ʱ��:%s",systemStatus.printInfo.gwt);
	printLineStr(buff,rt_strlen((char *)buff));
	//Ƥ��ʱ��
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"Ƥ��ʱ��:%s",systemStatus.printInfo.nwt);
	printLineStr(buff,rt_strlen((char *)buff));
	//˾����
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"˾����:%s",systemStatus.printInfo.sib);
	printLineStr(buff,rt_strlen((char *)buff));
	systemStatus.isPrint=0;
	rt_thread_mdelay(DELAY_TIME);// 
	Print_Line();
	rt_thread_mdelay(DELAY_TIME);// 
	Print_Line();
	rt_thread_mdelay(DELAY_TIME);// 
	setCommunicationStatus(0);
	setWeighRead();
	rt_kprintf("------print Info-------------\r\n");
}



