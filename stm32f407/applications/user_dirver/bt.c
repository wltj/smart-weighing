//#include "bt.h"
//#include "rtthread.h"
//#include <rtdevice.h>
//#include <board.h>
//#include <string.h>
//#include "uart_util.h"

//#define BT_MIN_RSSI     170

//#define ROLE    2

//#define BT_ACK_TIMEOUT  50  //10ms

//#define BT_MODE_CHANGE_PIN  GET_PIN(C,9)

//#define BT_UART_RATE    19200
//#define GET_BT_ROLE     "AT+ROLE?"

//#define SCAN_ROUND      "AT+ADVINTV=16000"
//#define CLOSE_DELAY     "AT+DATDLY=0"

//#if ROLE == 2
//#define SET_BT_ROLE     "AT+ROLE=2"
//#elif ROLE == 0
//#define SET_BT_ROLE     "AT+ROLE=0"
//#endif

////unsigned char bt_rec_buf[256];

//unsigned char Bt_Set_Scan_Round(void)
//{
//    unsigned char buf[10];
//    memset(buf,0,sizeof(buf));
//    int ret = Bt_Send_Cmd((unsigned char *)SCAN_ROUND,sizeof(SCAN_ROUND)-1,buf);
//    return ret;
//}

//unsigned char Bt_Get_Role(void)
//{
//    unsigned char buf[10];
//    memset(buf,0,sizeof(buf));
//    Bt_Send_Cmd((unsigned char *)GET_BT_ROLE,sizeof(GET_BT_ROLE)-1,buf);
//    if(buf[0] == '0')
//        return 0;
//    if(buf[0] == '1')
//        return 1;
//    if(buf[0] == '2')
//        return 2;
//		return 0;
//}

//unsigned char Bt_Set_Role(void)
//{
//    unsigned char buf[10];
//    memset(buf,0,sizeof(buf));
//    int ret = Bt_Send_Cmd((unsigned char *)SET_BT_ROLE,sizeof(SET_BT_ROLE)-1,buf);
//    return ret;
//}

//unsigned char Bt_Close_Delay(void)
//{
//    unsigned char buf[10];
//    memset(buf,0,sizeof(buf));
//    int ret = Bt_Send_Cmd((unsigned char *)CLOSE_DELAY,sizeof(CLOSE_DELAY)-1,buf);
//    return ret;
//}

//void BT_Change_At_Mode(void)
//{
//    rt_pin_write(BT_MODE_CHANGE_PIN, PIN_HIGH);
//    rt_thread_mdelay(10);
//    rt_pin_write(BT_MODE_CHANGE_PIN, PIN_LOW);
//}

//void BT_Change_Tr_Mode(void)
//{
//    rt_pin_write(BT_MODE_CHANGE_PIN, PIN_LOW);
//    rt_thread_mdelay(10);
//    rt_pin_write(BT_MODE_CHANGE_PIN, PIN_HIGH);
//}

//void Bt_Write(unsigned char *data,unsigned int len)
//{
//    Uart4_Wirte(data,len);
//}
//int btRXhandel(unsigned char *uuid,unsigned char* rssi)
//{
//		#define BT_RX_TIME	20
//		UART_RX_MSG * rxMsg= getUartStatus(UART4_INDEX);
//		if(rxMsg->rxTime==0)
//		{
//			return RT_ERROR;
//		}
//		if(rxMsg->rx_len==0)
//		{
//			clearRXMsg(UART4_INDEX);
//			return RT_ERROR;
//		}
//		if((rt_tick_get()-rxMsg->rxTime)<BT_RX_TIME)//BT_RX_TIME 毫秒未收到数据
//		{
//			return RT_ERROR;	
//		}
//		if(rxMsg->rx_len < 7+UUID_LENGTH)
//		{
//			return RT_ERROR;	
//		}
//		int res= Get_Uuid(uuid,rssi);
//		clearRXMsg(UART4_INDEX);
//		return res;
//}
// 


//int Get_Uuid(unsigned char *uuid,unsigned char* rssi)
//{
//    unsigned int len = 0;
//	  unsigned char * bt_rec_buf=NULL;
//    //memset(bt_rec_buf,0,sizeof(bt_rec_buf));
////    int ret = Bt_Read(bt_rec_buf,&len);
////    if(ret != RT_EOK)
////        return RT_ERROR;
//		len=getUartStatus(UART4_INDEX)->rx_len;
//	  bt_rec_buf=(unsigned char *)getUartStatus(UART4_INDEX)->rx_buffer;
//    if(len >= 7+UUID_LENGTH)
//    {
//        for(int i=1;i<len;i++)
//        {
//            if(bt_rec_buf[i]==0xab&&bt_rec_buf[i+1]==0xcd&&bt_rec_buf[i+2]==0xef && (len - i >= 15))
//            {
//                *rssi = bt_rec_buf[i-1];
//                memcpy(uuid,&bt_rec_buf[i],UUID_LENGTH);
//                
//                rt_kprintf("uuid=");
//                for(int j=0;j<UUID_LENGTH;j++)
//                {
//                    rt_kprintf("%02x,",uuid[j]);
//                }
//                rt_kprintf(",rssi=%d",*rssi);
//                rt_kprintf("\n");
//                
//                if(*rssi > BT_MIN_RSSI)
//                    return RT_EOK;
//                else
//                    return RT_ERROR;
//            }
//        }  
//    }
//    return RT_ERROR;
//}

//void Bt_Init(void)
//{
//    rt_pin_mode(BT_MODE_CHANGE_PIN, PIN_MODE_OUTPUT);
//    BT_Change_At_Mode();
//    Uart4_Init(BT_UART_RATE);
//    
//    //if(Bt_Get_Role() != ROLE)
//    {
//        Bt_Set_Role();
//    }

//    BT_Change_Tr_Mode();

//}

//int Bt_Send_Cmd(unsigned char *data,unsigned int len,unsigned char* ack_param)
//{
//    unsigned int get_len = 0;
//		unsigned char *bt_rec_buf;
//    bt_rec_buf=(unsigned char *)getUartStatus(UART4_INDEX)->rx_buffer;
//    Bt_Write(data,len);
//    for(int i=0;i<BT_ACK_TIMEOUT;i++)
//    {
//				rt_thread_mdelay(20);
//				int len=getUartStatus(UART4_INDEX)->rx_len;
//        if(len > 0)
//        {
//            if(memcmp(bt_rec_buf,"+OK",3) == 0)
//            {
//                if(ack_param != NULL)
//                    memcpy(ack_param,&bt_rec_buf[4],get_len-4);
//                return RT_EOK;
//            }
//            else if(memcmp(bt_rec_buf,"+ERR",3) == 0)
//            {
//                return RT_ERROR;
//            }
//        }

//    }
//    return RT_ETIMEOUT;
//}

//void Open_BtRead(void)
//{
//	setReadClose(UART4_INDEX,0);
//}	
//void Close_BtRead(void)
//{
//	setReadClose(UART4_INDEX,1);
//}
// 
// 


