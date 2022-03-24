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
unsigned char Text[] = "Hello world";//需转化为16进制
/*牧原集团*/
unsigned char Chinese[] = "C4C1D4ADBCAFCDC5";//(GB18030)
/*开启汉字模式指令*/
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
    
    /*填充报文内容*/
		for(i = 0; i < len; i++)
		{
			print_pack.data[i] = data[i]; 
		}
		
		print_pack.data[len]= 0x1B;		//1B 64  n
		print_pack.data[len+1] = 0x4A;
		print_pack.data[len+2] = 0x50; //走纸距离可变
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
		print_pack.data[len+2] = 0x50; //走纸距离可变
		
		
    /*填充报文内容*/
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
	rt_thread_mdelay(500);//等待1秒
	//if()
	//标题
	#define SEPARATOR	"----------------------------"
	printLineStr(systemStatus.printInfo.title,rt_strlen((char *)(systemStatus.printInfo.title))); 
	printLineStr((unsigned char *)SEPARATOR,rt_strlen((char *)(SEPARATOR))); 
	//打印单号
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"检斤单号:%s",systemStatus.printInfo.sn);
	printLineStr(buff,rt_strlen((char *)buff)); 
	//打印车号
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"车号:%s",systemStatus.printInfo.car);
	printLineStr(buff,rt_strlen((char *)buff));
	//货物名
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"货物名:%s",systemStatus.printInfo.gn);
	printLineStr(buff,rt_strlen((char *)buff));
	//规格
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"规格:%s",systemStatus.printInfo.sp);
	printLineStr(buff,rt_strlen((char *)buff));
	//收货单位
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"收货单位:%s",systemStatus.printInfo.ru);
	printLineStr(buff,rt_strlen((char *)buff));
	//供应商
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"供应商:%s",systemStatus.printInfo.fu);
	printLineStr(buff,rt_strlen((char *)buff));
	//毛重
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"毛重(KG):%s",systemStatus.printInfo.gw);
	printLineStr(buff,rt_strlen((char *)buff)); 
	//皮重
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"皮重(KG):%s",systemStatus.printInfo.tw);
	printLineStr(buff,rt_strlen((char *)buff)); 
	//净重
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"净重(KG):%s",systemStatus.printInfo.nw);
	printLineStr(buff,rt_strlen((char *)buff)); 
	//毛重时间
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"毛重时间:%s",systemStatus.printInfo.gwt);
	printLineStr(buff,rt_strlen((char *)buff));
	//皮重时间
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"皮重时间:%s",systemStatus.printInfo.nwt);
	printLineStr(buff,rt_strlen((char *)buff));
	//司磅人
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf((char *)buff,"司磅人:%s",systemStatus.printInfo.sib);
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



