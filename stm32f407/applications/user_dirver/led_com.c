#include "led_com.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <string.h>
#include "rs485.h"
#include "TipsData.h"
#include "pcb_expand.h"
#include "uart_thread.h"
#include "main_task.h"
/*波特率*/
#define LED_UART_RATE 9600

led_com_pack_t led_com_pack;

/* 欢迎光临 */
unsigned short Text1[] = {0x6b22,0x8fce,0x5149,0x4e34};
/* 称重完成，请等待放行 */
unsigned char Text2[] = "79F091CD5B8C62108BF77B495F85653E884CFF01";
 
 

unsigned char Pack_check(unsigned char *data,unsigned int len)
{
    unsigned char crc = data[0];
    for(unsigned int i=1;i<len;i++)
    {
        crc = crc ^ data[i];
    }
    return crc;
}

int Led_Com_Send(unsigned char* data,unsigned int len)
{
	closeReadAdioFrequency();
	rt_thread_mdelay(10);
  int res=Rs485_Wirte(LED_485_CHANNLE,LED_485_NUM,data,len);
	openReadAdioFrequency();
	return res;
}
int Led_Com_Send2(unsigned char* data,unsigned int len)
{
	sendLedInfo(data,len);
	return 0;
}
 
void Led_Show_String_str2(unsigned char channle ,unsigned char *data,unsigned short len)
{
    memset(&led_com_pack,0,sizeof(led_com_pack));
    len = len/2;
    led_com_pack.head = PACK_HEAD;
    /* 广播地址 */
    led_com_pack.pack_head_and_id_high_4 = 0x90;
    led_com_pack.id_low_8 = channle; //行数，第几行
    
    led_com_pack.cmd = 0x50;
    led_com_pack.pack_length = (len>>8) | ((len & 0x00ff) << 8);
    
    /*填充报文内容*/
    String_To_Unicode_To_Hex(data,led_com_pack.data,len);
    
    /* crc */
    led_com_pack.data[len] = Pack_check(&led_com_pack.id_low_8,len+4);
    
    Led_Com_Send2((unsigned char *)&led_com_pack,len+7);
}

 

void Led_Show_String_8(unsigned char channle ,unsigned char *data,unsigned short len)
{
    memset(&led_com_pack,0,sizeof(led_com_pack));
    
    led_com_pack.head = PACK_HEAD;
    /* 广播地址 */
    led_com_pack.pack_head_and_id_high_4 = 0x90;
    led_com_pack.id_low_8 = channle; //行数，第几行
    
    led_com_pack.cmd = 0x50;
    led_com_pack.pack_length = (len>>8) | ((len & 0x00ff) << 8);
    
    /*填充报文内容*/
    Unicode_8_To_Hex(data,led_com_pack.data,len);
    
    /* crc */
    led_com_pack.data[len] = Pack_check(&led_com_pack.id_low_8,len+4);
    
    Led_Com_Send((unsigned char *)&led_com_pack,len+7);
}

void Led_Show_String_16(unsigned char channle ,unsigned short *data,unsigned short len)
{
    memset(&led_com_pack,0,sizeof(led_com_pack));
    len = len*2;
    led_com_pack.head = PACK_HEAD;
    /* 广播地址 */
    led_com_pack.pack_head_and_id_high_4 = 0x90;
    led_com_pack.id_low_8 = channle; //行数，第几行
    
    led_com_pack.cmd = 0x50;
    led_com_pack.pack_length = (len>>8) | ((len & 0x00ff) << 8);
    
    /*填充报文内容*/
    Unicode_16_To_Hex(data,led_com_pack.data,len);
    
    /* crc */
    led_com_pack.data[len] = Pack_check(&led_com_pack.id_low_8,len+4);
    
    Led_Com_Send((unsigned char *)&led_com_pack,len+7);
}

void Led_Show_String_str(unsigned char channle ,unsigned char *data,unsigned short len)
{
    memset(&led_com_pack,0,sizeof(led_com_pack));
    len = len/2;
    led_com_pack.head = PACK_HEAD;
    /* 广播地址 */
    led_com_pack.pack_head_and_id_high_4 = 0x90;
    led_com_pack.id_low_8 = channle; //行数，第几行
    
    led_com_pack.cmd = 0x50;
    led_com_pack.pack_length = (len>>8) | ((len & 0x00ff) << 8);
    
    /*填充报文内容*/
    String_To_Unicode_To_Hex(data,led_com_pack.data,len);
    
    /* crc */
    led_com_pack.data[len] = Pack_check(&led_com_pack.id_low_8,len+4);
    
    Led_Com_Send((unsigned char *)&led_com_pack,len+7);
}

unsigned char test[]= {0x4f,0x60,0x59,0x7d,0xff,0x01,0x00,0x48,0x00,0x65,0x00,0x6c,0x00,0x6c,0x00,0x6f,0x00,0x21};
unsigned short test1[] = {0x4f60,0x597d,0xff01,0x0048,0x0065,0x006c,0x006c,0x006f,0x0021};
unsigned short test2[]={0x6b22,0x8fce,0x5149,0x4e34};
void Led_Init(void)
{
		Init_485();

		SoundPayData("005B0064005D005B007A0031005D005B006D0033005D4F60597D",strlen("005B0064005D005B007A0031005D005B006D0033005D4F60597D"));
		rt_thread_mdelay(200);	
		//Led_Show_String_str(1,Text2,sizeof(Text2)-1);
		//Led_Show_String_str(2,Text2,sizeof(Text2)-1);
}
//显示当前空闲
void LEDShowFree(void)
{
	Led_Show_String_str(1,(unsigned char *)TD_SYS_FREE,strlen(TD_SYS_FREE));
	Led_Show_String_str(2,(unsigned char *)TD_SYS_SYSTEM,strlen(TD_SYS_SYSTEM));
}
//显示系统离线
void LEDShowOffLine(void)
{
	Led_Show_String_str(1,(unsigned char *)TD_SYS_OFFLINE,strlen(TD_SYS_OFFLINE));
	Led_Show_String_str(2,(unsigned char *)TD_NULL,strlen(TD_NULL));
}
 
void LEDShowError1(void)
{
	Led_Show_String_str(1,(unsigned char *)TD_CHECK_REMOTE,strlen(TD_CHECK_REMOTE));
	Led_Show_String_str(2,(unsigned char *)TD_NULL,strlen(TD_NULL));
}
 
void LEDShowError2(void)
{
	Led_Show_String_str(1,(unsigned char *)TD_CHECK_EXPAND,strlen(TD_CHECK_EXPAND));
	Led_Show_String_str(2,(unsigned char *)TD_NULL,strlen(TD_NULL));
}

 
void LEDShowError3(void)
{
	Led_Show_String_str(1,(unsigned char *)TD_CHECK_WEIGH,strlen(TD_CHECK_WEIGH));
	Led_Show_String_str(2,(unsigned char *)TD_NULL,strlen(TD_NULL));
}
void LEDShowError4(void)
{
	Led_Show_String_str(1,(unsigned char *)TD_CHECK_CAMERA1,strlen(TD_CHECK_CAMERA1));
	Led_Show_String_str(2,(unsigned char *)TD_NULL,strlen(TD_NULL));
}
void LEDShowError5(void)
{
	Led_Show_String_str(1,(unsigned char *)TD_CHECK_RFID,strlen(TD_CHECK_RFID));
	Led_Show_String_str(2,(unsigned char *)TD_NULL,strlen(TD_NULL));
}

void LEDShowError6(void)
{
	Led_Show_String_str(1,(unsigned char *)TD_CHECK_CAMERA2,strlen(TD_CHECK_CAMERA2));
	Led_Show_String_str(2,(unsigned char *)TD_NULL,strlen(TD_NULL));
}
void LEDShowError7(void)
{
	Led_Show_String_str(1,(unsigned char *)TD_REMOTE_CHECK_ERROR,strlen(TD_REMOTE_CHECK_ERROR));
	Led_Show_String_str(2,(unsigned char *)TD_NULL,strlen(TD_NULL));
}
//XX正在称重
void LEDShowSevErrorInfo(unsigned char * carNum,unsigned char *errorInfo)
{
	char buff[60]={0};
	rt_memset(buff,0,sizeof(buff));
	if((carNum!=0)&&(carNum[0]!=0))
	{
		rt_sprintf(buff,TD_WEIGHED_UP,carNum);
	}
	else
	{
		rt_sprintf(buff,TD_WEIGHED_UP2);
	}
	Led_Show_String_str(1,(unsigned char *)buff,strlen(buff));
	
	rt_memset(buff,0,sizeof(buff));
	if((errorInfo!=0)&&(errorInfo[0]!=0))
	{
		if(rt_strlen((char *)errorInfo)<sizeof(buff))
		{
			rt_sprintf(buff,"%s",errorInfo);
		}
	}
	if(buff[0]==0)
	{
		rt_sprintf(buff,TD_SEV_ERROR);
	}
	Led_Show_String_str(2,(unsigned char *)buff,strlen(buff));
} 
//XX正在称重
void LEDShowWeighedUp(unsigned char * carNum,unsigned char *nextCarNum)
{
	char buff[60]={0};
	rt_memset(buff,0,sizeof(buff));
	if((carNum!=0)&&(carNum[0]!=0))
	{
		rt_sprintf(buff,TD_WEIGHED_UP,carNum);
	}
	else
	{
		rt_sprintf(buff,TD_WEIGHED_UP2);
	}
	Led_Show_String_str(1,(unsigned char *)buff,strlen(buff));
	
	rt_memset(buff,0,sizeof(buff));
	if((nextCarNum!=0)&&(nextCarNum[0]!=0))
	{
		rt_sprintf(buff,TD_NEXT_CAR,nextCarNum);
	}
	else
	{
		rt_sprintf(buff,TD_WAIT);
	}
	Led_Show_String_str(2,(unsigned char *)buff,strlen(buff));
} 
//XX正在称重，第二行显示重量
void LEDShowWeighedUp2(unsigned char * carNum, int  weight,unsigned char dir)
{
	char buff[512]={0};
	char buff2[100]={0};
	char buff3[10]={0};
	int hundred_thousand=0,ten_thousand=0,thousand=0,hundred=0,ten=0,unit=0;
	rt_memset(buff,0,sizeof(buff));
	if(systemStatus.informatio[0]!=0)
	{
		rt_sprintf(buff,(char *)systemStatus.informatio);
	}
	else if((carNum!=0)&&(carNum[0]!=0))
	{
		rt_sprintf(buff,TD_WEIGHED_UP,carNum);
	}
	else
	{
		rt_sprintf(buff,TD_WEIGHED_UP2);
	}
	Led_Show_String_str(1,(unsigned char *)buff,strlen(buff));
	rt_memset(buff,0,sizeof(buff));
	rt_memset(buff2,0,sizeof(buff2));
	rt_memset(buff3,0,sizeof(buff3));
	if(weight<0)
	{
		weight=0;
	}
	hundred_thousand=weight/1000000+'0';
	ten_thousand=(weight%1000000)/100000+'0';
	thousand=(weight%100000)/10000+'0';
	hundred=(weight%10000)/1000+'0';
	ten=(weight%1000)/100+'0';
	unit=(weight%100)/10+'0';
	//		//根据扫码的方向，判断是毛重和皮重
	//	rt_memcpy(&buff[strlen(buff)],systemStatus.rqDir?"76AE91CD":"6BDB91CD",8);
	rt_sprintf(buff2,"%s","672C6B21517191CD",16);
	char *buff4=&buff2[strlen(buff2)];
	//数字转字符串
	if(weight>=1000000)
	{
		rt_sprintf(buff4,"%04x%04x%04x%04x%04x%04x",hundred_thousand,ten_thousand,thousand,hundred,ten,unit);
	}
	else if(weight>=100000)
	{
		rt_sprintf(buff4,"%04x%04x%04x%04x%04x",ten_thousand,thousand,hundred,ten,unit);
	}
	else  if(weight>=10000)
	{
		rt_sprintf(buff4,"%04x%04x%04x%04x",thousand,hundred,ten,unit);
	}
	else  if(weight>=1000)
	{
		rt_sprintf(buff4,"%04x%04x%04x",hundred,ten,unit);
	}
	else  if(weight>=100)
	{
		rt_sprintf(buff4,"%04x%04x",ten,unit);
	}
	else  if(weight>=100)
	{
		rt_sprintf(buff4,"%04x",unit);
	}
	rt_sprintf(buff3,"%04x",(weight%10)+'0');
	
	rt_sprintf(buff,TD_WEIGHED,buff2,buff3);
	Led_Show_String_str(2,(unsigned char *)buff,strlen(buff));

} 

void LEDShowWeighedUp3(unsigned char * carNum)
{
		char buff[60]={0};
	rt_memset(buff,0,sizeof(buff));
	if((carNum!=0)&&(carNum[0]!=0))
	{
		rt_sprintf(buff,TD_WEIGHED_UP,carNum);
	}
	else
	{
		rt_sprintf(buff,TD_WEIGHED_UP2);
	}
	Led_Show_String_str(1,(unsigned char *)buff,strlen(buff));
	
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf(buff,TD_MOVE);
	Led_Show_String_str(2,(unsigned char *)buff,strlen(buff));
}

void LedShowLocationErrorInfo(unsigned char * carNum)
{
		char buff[60]={0};
		char buff2[100]={0};
		getLocationErrorStr((char *)buff2,sizeof(buff2),systemStatus.locationErrorInfo,systemStatus.irStatus);
		rt_memset(buff,0,sizeof(buff));
		if((carNum!=0)&&(carNum[0]!=0))
		{
			rt_sprintf(buff,TD_WEIGHED_UP,carNum);
		}
		else
		{
			rt_sprintf(buff,TD_WEIGHED_UP2);
		}
		Led_Show_String_str(1,(unsigned char *)buff,strlen(buff));
		
		rt_memset(buff,0,sizeof(buff));
		rt_sprintf(buff,buff2);
		Led_Show_String_str(2,(unsigned char *)buff,strlen(buff));
}
void LEDShowWeighedUp4(unsigned char * carNum)
{
	char buff[60]={0};
	rt_memset(buff,0,sizeof(buff));
	if((carNum!=0)&&(carNum[0]!=0))
	{
		rt_sprintf(buff,TD_WEIGHED_UP,carNum);
	}
	else
	{
		rt_sprintf(buff,TD_WEIGHED_UP2);
	}
	Led_Show_String_str(1,(unsigned char *)buff,strlen(buff));
	
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf(buff,TD_SCAN_RQ_FLAMEOUT);
	//rt_sprintf(buff,TD_SCAN_RQ);
	Led_Show_String_str(2,(unsigned char *)buff,strlen(buff));
}
void LEDShowWeighedUp5(unsigned char * carNum)
{
	char buff[60]={0};
	rt_memset(buff,0,sizeof(buff));
	if((carNum!=0)&&(carNum[0]!=0))
	{
		rt_sprintf(buff,TD_WEIGHED_UP,carNum);
	}
	else
	{
		rt_sprintf(buff,TD_WEIGHED_UP2);
	}
	Led_Show_String_str(1,(unsigned char *)buff,strlen(buff));
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf(buff,TD_NO_CAR_NUM);
	Led_Show_String_str(2,(unsigned char *)buff,strlen(buff));
}
void LEDShowWeighedUp6(unsigned char * carNum)
{
	char buff[60]={0};
	rt_memset(buff,0,sizeof(buff));
	if((carNum!=0)&&(carNum[0]!=0))
	{
		rt_sprintf(buff,TD_WEIGHED_UP,carNum);
	}
	else
	{
		rt_sprintf(buff,TD_WEIGHED_UP2);
	}
	Led_Show_String_str(1,(unsigned char *)buff,strlen(buff));
	
	rt_memset(buff,0,sizeof(buff));
	rt_sprintf(buff,TD_NO_CARD);
	Led_Show_String_str(2,(unsigned char *)buff,strlen(buff));
}
 //				LEDShowWeighedUp2(systemStatus.carNum,systemStatus.weight);
void LEDShowQualityTestingFail(unsigned char * carNum)
{
	char buff[60]={0};
	rt_memset(buff,0,sizeof(buff));
	if((carNum!=0)&&(carNum[0]!=0))
	{
		rt_sprintf(buff,TD_QUALITYTESTINGFAIL,carNum);
	}
	else
	{
		rt_sprintf(buff,TD_QUALITYTESTINGFAIL2);
	}
	Led_Show_String_str(1,(unsigned char *)buff,strlen(buff)); 
	Led_Show_String_str(2,(unsigned char *)TD_NULL,strlen(TD_NULL));
}
//扩展板显示排队信息
void LEDShowLineUp(unsigned char * carNum1,unsigned char * carNum2,unsigned char * carNum3,unsigned char * carNum4,unsigned char * carNum5)
{

//	Led_Show_String_str2(1,2);
}


void sendAudioCmd(unsigned char *data,unsigned short len)
{
    memset(&led_com_pack,0,sizeof(led_com_pack));
    //len = len/2;
    led_com_pack.head = PACK_HEAD;
    /* 广播地址 */
    led_com_pack.pack_head_and_id_high_4 = 0x90;
    led_com_pack.id_low_8 = 1; //行数，第几行
    
    led_com_pack.cmd = 0x51;
    led_com_pack.pack_length = (len>>8) | ((len & 0x00ff) << 8);
    
    /*填充报文内容*/
    //String_To_Unicode_To_Hex(data,led_com_pack.data,len);
    rt_memcpy(led_com_pack.data,data,len);
    /* crc */
    led_com_pack.data[len] = Pack_check(&led_com_pack.id_low_8,len+4);
    
    Led_Com_Send((unsigned char *)&led_com_pack,len+7);
}
#define CMD_SPEECH_SYNTHESIS	0x01	//语音合成命令
#define CMD_SPEECH_STOP				0x02	//停止合成命令
#define ENCODED_TYPE					0x03	//0x00-GB2312 0x01-GBK 0x02-BIG5 0x03-UNICODE

void SoundPayStop( void)
{
	unsigned char index=0;
	unsigned char buff[50];//  
	unsigned short dataLen=1;
	unsigned int i=0;
	
 
	//----消息头---------
	buff[index++]=0xfd;
	//----二字节长度-------
	buff[index++]=dataLen>>8;
	buff[index++]=dataLen&0xff;
	//----命令-------------
	buff[index++]=CMD_SPEECH_STOP;
	//----编码格式-------------	 
	//buff[index++]=ENCODED_TYPE;
	 
	sendAudioCmd(buff,index);
}


void SoundPayData( char * payData,unsigned short len)
{
	unsigned char index=0;
	unsigned char buff[256];// 
	unsigned char buff2[128];// 
	unsigned short dataLen=len/2+2;
	unsigned int i=0;
	
	String_To_Unicode_To_Hex((unsigned char *)payData,buff2,len);
	//----消息头---------
	buff[index++]=0xfd;
	//----二字节长度-------
	buff[index++]=dataLen>>8;
	buff[index++]=dataLen&0xff;
	//----命令-------------
	buff[index++]=CMD_SPEECH_SYNTHESIS;
	//----编码格式-------------	 
	buff[index++]=ENCODED_TYPE;
	//语音数据
	for(i=0;i<dataLen;i++)
	{
		buff[index++]=buff2[i];
	}
	sendAudioCmd(buff,index);
}
