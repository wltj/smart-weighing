//#include "com485.h"
//#include "rs485.h"
//#include <string.h>
//#include "crc.h"
//#include <rtthread.h>
//#include <rtdevice.h>
//#include <board.h>
//#include "led_com.h"
///* 用于双机通信 */
//#define COM485_RE_SEND  3
//#define COM485_BUF_MAX 1024

///* 指令表 */
//#define LED_CTR     1   //led控制
//#define SOUND_CTR   2   //语音
//#define PRINTER_CTR 3   //打印机
//#define POLE_CTR    4   //过道杆



//unsigned char com485_channle[2] = {0,1};
//unsigned char com485_send_buf[COM485_BUF_MAX];
//unsigned char com485_rec_buf[COM485_BUF_MAX];

//unsigned char send_cmd_old = 0;
//void Init_Com485(void)
//{
//    if(Get_Rs485_Init_Flag() != 1)
//        Init_485();
//}

///*
//    485组包发送
//*/
//void Com485_Pack_Send(unsigned char cmd,unsigned char* data,unsigned short len,unsigned char ack)
//{
//    int i = 0;
//    com485_send_buf[i++] = 0xaa;
//    if(ack == 1)
//        com485_send_buf[i++] = 0x02; //回复
//    else
//        com485_send_buf[i++] = 0x01; //发送
//    
//    com485_send_buf[i++] = cmd;
//    
//    com485_send_buf[i++] = len>>8; //高位在前
//    com485_send_buf[i++] = len;
//    
//    memcpy(com485_send_buf,data,len);
//    i+=len;
//    
//    int crc = calculate_CRC32(com485_send_buf,i,0xffffffffL);
//    
//    com485_send_buf[i++] = crc>>24;
//    com485_send_buf[i++] = crc>>16;
//    com485_send_buf[i++] = crc>>8;
//    com485_send_buf[i++] = crc;
//    
//    Rs485_Wirte(com485_channle[0],com485_channle[1],com485_send_buf,i);
//}

///*
//    485解包
//*/
//int Com485_Unack(unsigned char* cmd,unsigned char** data,unsigned short* len,unsigned char* ack)
//{
//    int i = 0;
//    if(com485_rec_buf[i++] != 0xaa)
//        return RT_ERROR;
//    
//    if(com485_rec_buf[i++] == 0x02) //回复
//        *ack = 1;
//    else if(com485_rec_buf[i++] != 0x01) //接收
//        return RT_ERROR;
//    *cmd = com485_rec_buf[i++]; 
//    
//    *len = (unsigned short)com485_rec_buf[i++] << 8; //高位在前
//    *len = *len + com485_rec_buf[i++]; 
//    
//    int crc = calculate_CRC32(com485_rec_buf,*len+i,0xffffffffL);
//    
//    int crc2 = (unsigned int)com485_rec_buf[i+*len] << 24 | 
//               (unsigned int)com485_rec_buf[i+*len] << 16 | 
//               (unsigned int)com485_rec_buf[i+*len] << 8 |
//               (unsigned int)com485_rec_buf[i+*len] ;
//                
//    if(crc != crc2)
//        return RT_ERROR;
//    
//    *data = &com485_rec_buf[i];
//    
//    return RT_EOK;
//}


//int Com485_Send_Handle(unsigned char cmd,unsigned char* data,unsigned short len)
//{
//    for(int i=0;i<COM485_RE_SEND;i++)
//    {
//        Com485_Pack_Send(cmd,data,len,0);
//        
//        /* 接收返回 */
//        if(Com485_Rec_Handle() == RT_ETIMEOUT)
//            return RT_EOK;
//    }
//    
//    return RT_ERROR;
//}

//int Com485_Rec_Handle(void)
//{
//    unsigned int len = 0;
//    unsigned char cmd = 0;
//    unsigned char *data_buf = NULL;
//    unsigned char ack = 0;
//    unsigned int ret = Rs485_Read(com485_channle[0],com485_channle[1],com485_rec_buf,&len);
//    
//    if(ret != RT_EOK)
//        return RT_ERROR;
//    if(len > 0)
//    {
//        unsigned short data_len = 0;
//        /* 有数据 */
//        int ret = Com485_Unack(&cmd,&data_buf,&data_len,&ack);
//        
//        if(ret == RT_ERROR)
//            return RT_ERROR;
//        /* 不是应答 */  
//        if(ack == 0)
//        {
//            switch(cmd)
//            {
//                /* LED 控制 */
//                case LED_CTR:
//                    Led_Show_String_str(data_buf[0],&data_buf[1],data_len-1);
//                    /* 应答 */
//                    Com485_Pack_Send(cmd,NULL,0,1);
//                    break;
//                
//                /* 声音播报 */
//                case SOUND_CTR:
//                    
//                    Com485_Pack_Send(cmd,NULL,0,1);
//                    break;
//                
//                /* 打印机控制 */
//                case PRINTER_CTR:
//                    
//                    Com485_Pack_Send(cmd,NULL,0,1);
//                    break;
//                
//                /* 过道杆控制 */
//                case POLE_CTR:
//                    
//                    Com485_Pack_Send(cmd,NULL,0,1);
//                    break;
//            }
//            return RT_EOK;
//        }
//        else
//        {
//            if(send_cmd_old == cmd)
//            {
//                return RT_ETIMEOUT;
//            }
//            else
//            {
//                return RT_ERROR;
//            }
//        }
//    }
//    return RT_ERROR;
//}

