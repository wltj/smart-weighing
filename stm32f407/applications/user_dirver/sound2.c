//#include "sound.h"
// 
//#include <rtthread.h>
//#include <rtdevice.h>
//#include <board.h>
//#include "uart_util.h"
//#include <string.h>
//#include "TipsData.h"
//#include "main_task.h"
//#define DATA_MAX    265


//static SoundPayFun  soundPayFun=NULL;
//static SoundPayFun  lastPayFun=NULL;
//unsigned int lastPayTime=0;
//unsigned short lastNum=0;
///*波特率*/
//#define SOUND_UART_RATE 9600 
// 

// 
//int Sound_Car_Id(unsigned char* carid);
//int Sound_Weight_Value( int weight);

//int Sound_Wirte(unsigned char* data,unsigned int len)
//{
//    return Uart1_Wirte(data,len); 
//}

// 
//int Sound_Init(void)
//{
//	 Uart1_Init(9600);
//   return RT_ERROR;
//}
//void play_sound(unsigned char num)
//{
//    //7E FF 06 0F 00 01 01 EF  	
//    unsigned char str[10]={0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x00, 0xEF };
//    str[6]=num;
//    /* 发送 */
//		Sound_Wirte(str,sizeof(str));
//}
// 
///*
//    播放音频
//*/
//int Sound_Put(unsigned short num)
//{
////    unsigned char data[2] = {0x00,0x00};
////    data[0] = num >> 8;
////    data[1] = num;
////    Sound_Pack_Send(0x03,0x01,data,2);
//		play_sound(num);
//		lastPayTime=rt_tick_get(); 
//		lastNum=num;
//    return RT_EOK;
//}

//void payOffLine(void)///离线 
//{
//		Sound_Put(SOUND_INDEX_7);
//		rt_thread_delay(2500);
//}
//void payERROR1(void)/// 
//{
//		Sound_Put(SOUND_ERROR_1);
//		rt_thread_delay(2500);
//}
//void payERROR2(void)/// 
//{
//		Sound_Put(SOUND_ERROR_2);
//		rt_thread_delay(2500);
//}
//void payERROR3(void)/// 
//{
//		Sound_Put(SOUND_ERROR_3);
//		rt_thread_delay(2500);
//}


//void plearOntheScale (void)///请上磅称重
//{
//		if(systemStatus.carNum[0]!=0)
//		{
//			Sound_Car_Id(systemStatus.carNum);
//		}
//		Sound_Put(SOUND_INDEX_18);
//		rt_thread_delay(2000);
//}

//void payStartWork(void)///开始称重
//{
//		if(systemStatus.carNum[0]!=0)
//		{
//			Sound_Car_Id(systemStatus.carNum);
//		}
//		Sound_Put(SOUND_INDEX_3);
//		rt_thread_delay(2000);
//}

//void payWork1(void)//请某某车前进 
//{
//	
//		Sound_Put(SOUND_INDEX_16);
//		rt_thread_delay(2000);
//}
//void payWork2(void)//请某某车后退
//{
//		Sound_Put(SOUND_INDEX_16);
//		rt_thread_delay(2000);
//}
//void payWork3(void)//请某某扫描二维码
//{
//		Sound_Put(SOUND_INDEX_1);
//		rt_thread_delay(2000);
//}
//void payWork4(void)//请等待称重结束
//{
//		Sound_Put(SOUND_INDEX_4);
//		rt_thread_delay(2000);
//}
//void payWork5(void)//播报重量
//{
//		if(systemStatus.carNum[0]!=0)
//		{
//			Sound_Car_Id(systemStatus.carNum);
//		}
//		Sound_Weight_Value(systemStatus.weight);
//		rt_thread_delay(2000);
//}
//void payWork6(void)//请离开
//{
//		if(systemStatus.carNum[0]!=0)
//		{
//			Sound_Car_Id(systemStatus.carNum);
//		}
//		Sound_Put(SOUND_INDEX_2);
//		rt_thread_delay(2000);
//}
//void setSoundPayFun(SoundPayFun fun)
//{
//	soundPayFun=fun;
//}	
///* 
//  声音播报线程
// */
// static void sound_pay_thread(void *param)
// {
//    while(1)
//    {	
//			 if(soundPayFun!=NULL)
//			 {
//				 if(((rt_tick_get()-lastPayTime)>5000)//8888
//					 ||(lastPayFun!=soundPayFun))
//				 {
//					  lastPayFun=soundPayFun;
//						soundPayFun();
//					 //Sound_Car_Id("8C6B005200300050005900330035");
//					 //Sound_Weight_Value(100);
//				 }
//			 }
//       rt_thread_delay(200);
//    }
// }
// 
// 
//int Sound_Pay_start(void)
//{

//    rt_thread_t tid = RT_NULL;
//		Sound_Init();
//    tid = rt_thread_create( "soundpay",
//                            sound_pay_thread, RT_NULL,      //  
//                            1024,          // 
//                            11, 10 );     //     
//    if (tid)
//    {
//        rt_thread_startup(tid);
//    }
//    return RT_EOK;
//}

///* 
//播放车牌号 
//*/
//#define CAR_ID_SOUND_DELAY  450  //ms
//int Sound_Car_Id(unsigned char* carid)
//{
//    unsigned char sound_table[7];
//		unsigned char index=0;
//		index=getProvinceIndex(carid);
//		if(index==0xff)
//		{
//			return RT_ERROR;
//		}
//		sound_table[0]=index+PROVINCES_INDEX_BASE;
//    
//    for(int i=1;i<7;i++)
//    {
//        index=getLetterIndex(carid+(4*i));
//				if(index!=0xff)
//				{
//					 sound_table[i]=index+LETTERS_INDEX_BASE;
//					 continue;
//				}
//				index=getNumberIndex(carid+(4*i));
//				if(index!=0xff)
//				{
//					 sound_table[i]=index+NUMBER_INDEX_BASE;
//					 continue;
//				}
//				return RT_ERROR;
//    }

//    for(int i=0;i<7;i++)
//    {
//        Sound_Put((unsigned short)sound_table[i]);      
//        rt_thread_mdelay(CAR_ID_SOUND_DELAY);
//    }
//    return RT_EOK;
//}

///*
//    重量播报
//*/
//#define CAR_ID_SOUND_DELAY2  300  //ms
//int Sound_Weight_Value( int weight)
//{
//		int hundred_thousand,ten_thousand=0,thousand=0,hundred=0,ten=0,unit=0;
//		if(weight<0)
//		{
//			weight=0;
//		}
//		hundred_thousand=	 weight/1000000;
//		ten_thousand=			(weight%1000000)/100000;
//		thousand=					(weight%100000)/10000;
//		hundred= 					(weight%10000) /1000;
//		ten=							(weight%1000)/100;
//		unit=							(weight%100)/10;
//	
//		Sound_Put(SOUND_INDEX_8);      					//共重
//		rt_thread_mdelay(CAR_ID_SOUND_DELAY);
//		rt_thread_mdelay(CAR_ID_SOUND_DELAY);
//		if(weight>=1000000)																					//十万
//		{
//				Sound_Put(hundred_thousand+NUMBER_INDEX_BASE);      			
//				rt_thread_mdelay(CAR_ID_SOUND_DELAY);
//				Sound_Put(SOUND_INDEX_11);      								 
//				rt_thread_mdelay(CAR_ID_SOUND_DELAY);
//		}
//		if(weight>=100000)																					//万
//		{
//				if(ten_thousand!=0)
//				{
//					Sound_Put(ten_thousand+NUMBER_INDEX_BASE);      			
//					rt_thread_mdelay(CAR_ID_SOUND_DELAY);
//				}
//				Sound_Put(SOUND_INDEX_14);      								 
//				rt_thread_mdelay(CAR_ID_SOUND_DELAY);
//		}
//		if(weight>=10000)
//		{
//				Sound_Put(thousand+NUMBER_INDEX_BASE);      					//千
//				rt_thread_mdelay(CAR_ID_SOUND_DELAY);
//				if(thousand!=0)
//				{
//					Sound_Put(SOUND_INDEX_13);      											 
//					rt_thread_mdelay(CAR_ID_SOUND_DELAY);
//				} 
//		}
//		if(weight>=1000)//百位
//		{
//				Sound_Put(hundred+NUMBER_INDEX_BASE);      				 
//				rt_thread_mdelay(CAR_ID_SOUND_DELAY);
//				if(hundred!=0)
//				{
//					Sound_Put(SOUND_INDEX_12);      												 
//					rt_thread_mdelay(CAR_ID_SOUND_DELAY);
//				}  
//		}
//		if(weight>=100)//十位
//		{
//				Sound_Put(ten+NUMBER_INDEX_BASE);      					 
//				rt_thread_mdelay(CAR_ID_SOUND_DELAY);
//				if(ten!=0)
//				{
//					Sound_Put(SOUND_INDEX_11);      													 
//					rt_thread_mdelay(CAR_ID_SOUND_DELAY);
//				}  
//		}
//		if(unit!=0)
//		{
//			Sound_Put(unit+NUMBER_INDEX_BASE);    //个位	
//			rt_thread_mdelay(CAR_ID_SOUND_DELAY); 
//		}
////		//小数点后面
////		if((weight%10)!=0)
////		{
////			Sound_Put(SOUND_INDEX_10);     			//显示点 					 
////			rt_thread_mdelay(CAR_ID_SOUND_DELAY); 
////			Sound_Put((weight%10)+NUMBER_INDEX_BASE); 
////			rt_thread_mdelay(CAR_ID_SOUND_DELAY); 
////		}
//		//单位  KG
//		Sound_Put(SOUND_INDEX_9); 
//		rt_thread_mdelay(CAR_ID_SOUND_DELAY); 
//		rt_thread_mdelay(CAR_ID_SOUND_DELAY); 
//		return 0;
//}
