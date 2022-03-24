#include "sound.h"
 
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "uart_util.h"
#include <string.h>
#include "main_task.h"
#include "led_com.h"
#include "TipsData.h"
#define DATA_MAX    265


static SoundPayFun  soundPayFun=NULL;
static SoundPayFun  lastPayFun=NULL;
unsigned int lastPayTime=0;
unsigned short lastNum=0;
/*������*/
#define SOUND_UART_RATE 9600 
 
void playSound(void)
{
	if(systemStatus.sound!=1)
	{
		return;
	}
	if(soundPayFun!=NULL)
 {
	 if(((rt_tick_get()-lastPayTime)>10000)//
		 ||(lastPayFun!=soundPayFun))
	 {
			lastPayFun=soundPayFun;
		 if(soundPayFun!=NULL)
		 {
				soundPayFun();
		 }
		 lastPayTime=rt_tick_get();
		 //Sound_Car_Id("8C6B005200300050005900330035");
		 //Sound_Weight_Value(100);
	 }
 }
}
  
int Sound_Weight_Value(char * p, int weight);

 
void setSoundPayFun(SoundPayFun fun)
{
	soundPayFun=fun;
}	
 
 
 void lining(void)//ϵͳ�����У���ȴ�
{
		#define Lining  "7CFB7EDF8FDE63A54E2D00208BF77B495F85"
		SoundPayData(Lining,strlen(Lining));
		rt_kprintf("123456\n");
		rt_thread_delay(5000);
}
 

void payOffLine(void)//ϵͳ����,����
{
		#define SYS_OFF_LENE  "7CFB7EDF79BB7EBF002C8BF768C067E5"
		SoundPayData(SYS_OFF_LENE,strlen(SYS_OFF_LENE));
		rt_thread_delay(5000);
}
void payERROR1(void)/// ����������,����
{
		#define ERROR1 "62A58B664EEA79BB7EBF002C8BF768C067E5"
		SoundPayData(ERROR1,strlen(ERROR1));
		rt_thread_delay(5000);
}
void payERROR2(void)//��չ������,����
{
		#define ERROR2 "62695C55677F79BB7EBF002C8BF768C067E5"
		SoundPayData(ERROR2,strlen(ERROR2));
		rt_thread_delay(5000);
}
void payERROR3(void)/// �ذ�����,����
{
		#define ERROR3 "573078C579BB7EBF002C8BF768C067E5"
		SoundPayData(ERROR3,strlen(ERROR3));
		rt_thread_delay(5000);
}
void payERROR4(void)/// ����ͷ����,����
{
		#define ERROR4 "8BF768C067E54E3B63A7677F644450CF5934"
		SoundPayData(ERROR4,strlen(ERROR4));
		rt_thread_delay(5000);
}
void payERROR6(void)/// ����ͷ����,����
{
		#define ERROR "8BF768C067E562695C55677F644450CF5934"
		SoundPayData(ERROR,strlen(ERROR));
		rt_thread_delay(5000);
}
void payERROR5(void)//��Ƶ���ߣ�����
{
		#define ERROR5 "5C0498918BFB5361566879BB7EBF002C8BF768C067E5"
		SoundPayData(ERROR5,strlen(ERROR5));
		rt_thread_delay(5000);
}
void payERROR7(void)//�����쳣ң���ź�,����
{
		#define ERROR7 "5B5857285F025E38906563A74FE153F7002C8BF768C067E5"
		SoundPayData(ERROR7,strlen(ERROR7));
		rt_thread_delay(5000);
}
void paySevERROR(void)// 
{	
		if(systemStatus.errorInfo[0]!=0)
		{
			 SoundPayData((char *)systemStatus.errorInfo,strlen((char *)systemStatus.errorInfo));
		}
		else
		{
			#define SEV_ERROR "670D52A1566863D0793A5F025E38000D"
			SoundPayData(SEV_ERROR,strlen(SEV_ERROR));
		}
		rt_thread_delay(5000);
}

static void carNumExtend( char *buff,char * str)
{
	int i=0,j=0;
	int carNumLen=rt_strlen((char *)systemStatus.carNum);
	rt_sprintf(buff,"%s","005B007A0031005D");
	j=strlen(buff);
	for(i=0;i<carNumLen;)
	{
		buff[j++]=systemStatus.carNum[i++];
		buff[j++]=systemStatus.carNum[i++];
		buff[j++]=systemStatus.carNum[i++];
		buff[j++]=systemStatus.carNum[i++];
		buff[j++]='0';
		buff[j++]='0';
		buff[j++]='2';
		buff[j++]='A';
		buff[j++]='0';
		buff[j++]='0';
		buff[j++]='2';
		buff[j++]='3';
	}
	
	if(str!=NULL)
	{
		rt_sprintf(buff+j,"%s",str);
	}
//----------------------------------------
//	SoundPayData("005B006D0033005D",strlen("005B006D0033005D"));
//	SoundPayData("005B00730035005D",strlen("005B00730035005D"));
//	SoundPayData("005B00660030005D",strlen("005B00660030005D"));
//	if(str!=NULL)
//	{
//		rt_sprintf(buff,"005B00660031005D%s005B00660030005D%s",systemStatus.carNum,str);
//	}
//	else{
//		rt_sprintf(buff,"005B00660031005D%s005B00660030005D",systemStatus.carNum);
//	}
}

void plearOntheScale (void)///���ϰ�����
{
		#define STR1 "8BF74E0A78C579F091CD"//���ϰ����� 
//		static rt_uint32_t lastPlayTime=0;
//		if((rt_tick_get()-lastPlayTime)<100000)//
//		{
//			return;
//		}
//		lastPlayTime=rt_tick_get();
		if(systemStatus.carNum[0]!=0)
		{
			char buff[200]={0};
			carNumExtend(buff,STR1);
			SoundPayData(buff,strlen(buff));
		}
		else
		{
			SoundPayData(STR1,strlen(STR1));
		}
		rt_thread_delay(5000);
}

void payStartWork(void)///��ʼ����
{
		#define STR2 "5F0059CB79F091CD"
		if(systemStatus.carNum[0]!=0)
		{
			char buff[200]={0};
			carNumExtend(buff,STR2);
			SoundPayData(buff,strlen(buff));
		}
		else
		{
			SoundPayData(STR2,strlen(STR2));
		}
		rt_thread_delay(5000);
}

void payWork1(void)//��ĳĳ��ǰ�� 
{ 
		 char buff[100]={0};
		getLocationErrorStr((char *)buff,sizeof(buff),systemStatus.locationErrorInfo,systemStatus.irStatus); 
		SoundPayData(buff,strlen(buff));
		rt_thread_delay(5000);
}
void payWork2(void)//��ĳĳ������
{
		payWork1();
}
void payWork3(void)//��ĳĳɨ���ά��
{
		//#define STR4 "8F668F8652304F4DFF0C8BF7626B63CF4E8C7EF47801"
		#define STR4 "8F668F8652304F4D8BF77184706B540E626B63CF4E8C7EF47801"
		SoundPayData(STR4,strlen(STR4));
		rt_thread_delay(5000);
}
void payWork4(void)//��ȴ����ؽ���
{
		#define STR5 "8BF77B495F8579F091CD7ED3675F"
		SoundPayData(STR5,strlen(STR5));
		rt_thread_delay(5000);
}

 
void payWork5(void)//��������
{
		 char buff[300]={0};
			//"%s91CD%s";
		rt_memset(buff,0,sizeof(buff));
		if(systemStatus.carNum[0]!=0)
		{
			carNumExtend(buff,NULL);
		}
		SoundPayStop();//��ֹͣ����
		rt_thread_delay(200);
		//����ɨ��ķ����ж���ë�غ�Ƥ��
		rt_memcpy(&buff[strlen(buff)],"672C6B21517191CD",16);
		Sound_Weight_Value(buff,systemStatus.weight);
		SoundPayData(buff,strlen(buff));
		rt_thread_delay(6000);
}
void payWork6(void)//���뿪
{
		if(systemStatus.informatio[0]!=0)
		{
			SoundPayData((char *)systemStatus.informatio,strlen((char *)systemStatus.informatio));
		}
		else
		{
			#define STR7 "79F091CD7ED3675FFF0C8BF779BB5F00"
			SoundPayData(STR7,strlen(STR7));
		}
		rt_thread_delay(5000);
}

void payWork7(void)//δʶ����
{
		#define STR8 "8F66724C672A8BC6522B"
		SoundPayData(STR8,strlen(STR8));
		rt_thread_delay(5000);
}

void payWork8(void)//δʶ����Ƶ��
{
		#define STR9 "672A68C06D4B52305C0498915361"
		SoundPayData(STR9,strlen(STR9));
		rt_thread_delay(5000);
}
 
void payWork9(void)//�ʼ�ʧ��
{
		#define STR10 "8D2868C0672A901A8FC7002C8BF779BB5F00"
		if(systemStatus.carNum[0]!=0)
		{
			char buff[200]={0};
			carNumExtend(buff,STR10);
			SoundPayData(buff,strlen(buff));
		}
		else
		{
			SoundPayData(STR10,strlen(STR10));
		}
		rt_thread_delay(5000);
}
 

/*
    ��������
*/
//#define CAR_ID_SOUND_DELAY  450  //ms
int Sound_Weight_Value(char * p, int weight)
{
		int hundred_thousand,ten_thousand=0,thousand=0,hundred=0,ten=0,unit=0;
		if(weight<0)
		{
			weight=0;
		}
		hundred_thousand=	 weight/1000000;
		ten_thousand=			(weight%1000000)/100000;
		thousand=					(weight%100000)/10000;
		hundred= 					(weight%10000) /1000;
		ten=							(weight%1000)/100;
		unit=							(weight%100)/10;
		rt_sprintf(&p[strlen(p)],"%s","005B006E0032005D");  
		if(weight>=1000000)																					//ʮ��
		{
				rt_sprintf(&p[strlen(p)],"003%d",hundred_thousand);     			
		}
		
		if(weight>=100000)																					//��
		{
				rt_sprintf(&p[strlen(p)],"003%d",ten_thousand);     			
		}
		if(weight>=10000)
		{
				rt_sprintf(&p[strlen(p)],"003%d",thousand);		
		}
		if(weight>=1000)//��λ
		{
				rt_sprintf(&p[strlen(p)],"003%d",hundred);				
		}
		if(weight>=100)//ʮλ
		{
				rt_sprintf(&p[strlen(p)],"003%d",ten);
		}
		rt_sprintf(&p[strlen(p)],"003%d",unit); 
		rt_sprintf(&p[strlen(p)],"%s","516C65A4"); 
//		hundred_thousand=	 weight/1000000;
//		ten_thousand=			(weight%1000000)/100000;
//		thousand=					(weight%100000)/10000;
//		hundred= 					(weight%10000) /1000;
//		ten=							(weight%1000)/100;
//		unit=							(weight%100)/10;
//		///////////////////////////////////////////////////////
//		ten_thousand_mantissa=weight%1000000;
//		thousand_mantissa=weight%100000;
//		hundred_mantissa=weight%10000;
//		ten_mantissa=weight%1000;
//		unit_mantissa=weight%100;
//		
//		if(weight>=1000000)																					//ʮ��
//		{
//				rt_sprintf(&p[strlen(p)],"003%d",hundred_thousand);     			
//				rt_memcpy(&p[strlen(p)],"53414E07",8);
//				if(ten_thousand_mantissa==0)
//				{
//					rt_sprintf(&p[strlen(p)],"%s","516C65A4"); 
//					return 1;
//				}
//		}
//		
//		if(weight>=100000)																					//��
//		{
//				if(ten_thousand!=0)
//				{
//					rt_sprintf(&p[strlen(p)],"003%d",ten_thousand);     			
//				}
//				rt_memcpy(&p[strlen(p)],"4E07",4);
//				if(thousand_mantissa==0)
//				{
//					rt_sprintf(&p[strlen(p)],"%s","516C65A4"); 
//					return 1;
//				}
//		}
//		if(weight>=10000)
//		{
//				if(!((ten_thousand==0)&&(thousand==0)))
//				{
//					rt_sprintf(&p[strlen(p)],"003%d",thousand);
//				}
//				if(thousand!=0)
//				{
//					rt_memcpy(&p[strlen(p)],"5343",4);
//				}
//				if(hundred_mantissa==0)
//				{
//					rt_sprintf(&p[strlen(p)],"%s","516C65A4"); 
//					return 1;
//				}				
//		}
//		if(weight>=1000)//��λ
//		{
//				if(!((thousand==0)&&(hundred==0)))
//				{
//					rt_sprintf(&p[strlen(p)],"003%d",hundred);
//				}
//				if(hundred!=0)
//				{
//					rt_memcpy(&p[strlen(p)],"767E",4);
//				}
//				if(ten_mantissa==0)
//				{
//					rt_sprintf(&p[strlen(p)],"%s","516C65A4"); 
//					return 1;
//				}					
//		}
//		if(weight>=100)//ʮλ
//		{
//				if(!((hundred==0)&&(ten==0)&&(unit==0)))
//				{
//					rt_sprintf(&p[strlen(p)],"003%d",ten);
//				}
//				if(ten!=0)
//				{
//					rt_memcpy(&p[strlen(p)],"5341",4);
//				} 
//				if(ten_mantissa==0)
//				{
//					rt_sprintf(&p[strlen(p)],"%s","516C65A4"); 
//					return 1;
//				}
//		}
//		if(unit!=0)
//		{
//			rt_sprintf(&p[strlen(p)],"003%d",unit); 
//		}
//		rt_sprintf(&p[strlen(p)],"%s","516C65A4"); 
		return 0;
}
