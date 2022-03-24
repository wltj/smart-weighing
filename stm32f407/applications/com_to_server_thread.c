/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-03-11     Jason        first version
 * 
 * 
 */
 #include <rtthread.h>
 #include <rtdbg.h>
 #include "mqtt_thread.h"
 #include "com_to_server_thread.h"
 #include "cmd_handle.h"
 #include "cmd_table.h"
 #include "gateway.h"
 #include "com_to_server_cmd_list.h"
#include <board.h>
 #include "main_task.h"
#define LED_ON  PIN_LOW
#define LED_OFF PIN_HIGH
 /* 指令返回超时时间 */
 #define CMD_ACK_TIMEOUT    1000     //1s
#define NET_LED_PIN    GET_PIN(C, 2)

 /* 邮箱控制块 */
struct rt_mailbox mb;
/* 用于放邮件的内存池 */
static char mb_pool[128];


void Net_Led_Status(unsigned char status)
{
    
    rt_pin_write(NET_LED_PIN, status);
}

void mqtt_ack(unsigned short cmd,unsigned short err_status)
{
    unsigned msg_int = ((unsigned int)cmd << 16) + err_status;
    
    rt_mb_send(&mb, (rt_uint32_t)msg_int);
}

unsigned short rec_mqtt_ack(unsigned short cmd,unsigned int timeout)
{
    unsigned short msg_cmd = 0;
    unsigned short msg_err = 0;
    unsigned int msg_int = 0;
    if(rt_mb_recv(&mb, (rt_ubase_t *)&msg_int, timeout) == RT_EOK)
    {
        msg_cmd = msg_int >> 16;
        msg_err = msg_int;
        if(cmd != msg_cmd )
            return RT_ERROR;
        
        return msg_err;
    }
    else
    {
        return RT_ERROR;
    }
}

 /* 
    mqtt收发
 */
 static void mqtt_rec_and_send_thread(void *param)
 {
    unsigned char  mqtt_status;
    while(1)
    {
        mqtt_status = Get_Mqtt_Link_Status();
				
        /* 刚刚上线 */
        if (mqtt_status == MQTT_LINK_UP)
        {
            Net_Led_Status(LED_ON);
						sendRegisterMessage();
						systemStatus.mqttRxTime=rt_tick_get();
					  rt_thread_delay(1000);
//						if(rec_mqtt_ack(Cmd_DEV_REGISTER,CMD_ACK_TIMEOUT) == RT_EOK)
//						{
//								 /* 设置mqtt状态为已经连接上 */
//							Set_Mqtt_Status(MQTT_LINKING);
//							
//						}
//            /* 未注册 */
//            if(gateway.gateway_register_status == GATEWAY_NO_REGISTER)
//            {
//                sendRegisterMessage();
//                if(rec_mqtt_ack(Cmd_DEV_REGISTER,CMD_ACK_TIMEOUT) == RT_EOK)
//                {
//                    /* 注册成功，修改注册状态并存储 */
//                    gateway.gateway_register_status = GATEWAY_REGISTERED;
//                    Save_Gateway_Param();
//                }
//                /* 注册失败 */
//                else
//                {
//                    rt_thread_mdelay(10000);
//                }
//            }
//            /* 已注册,发送连接 */
//            else
//            {
//                //Gateway_Link_Server();
//                if(rec_mqtt_ack(Cmd_DEV_REGISTER,CMD_ACK_TIMEOUT) == RT_EOK)
//                {
//                    /* 设置mqtt状态为已经连接上 */
//                    Set_Mqtt_Status(MQTT_LINKING);
//                }
//                /* 连接失败 */
//                else
//                {
//                    rt_thread_mdelay(60000);
//                }
//            }
        }
        else if(mqtt_status == MQTT_LINKING)
        {
            Net_Led_Status(LED_ON);
					  unsigned int time=0;
						time=rt_tick_get();
//            /* 业务数据收发 */
//						sendDevStatus();//发送设备状态
						if((time-systemStatus.mqttRxTime)>=30000) 
						{
							 //Set_Mqtt_Status(MQTT_LINK_DOWN);
							 LOG_E("mqtt client outtime,link down.");
							 __set_FAULTMASK(1);
							 HAL_NVIC_SystemReset();
							 NVIC_SystemReset();
						} 
        }
        else
        {
            Net_Led_Status(LED_OFF);
        }
        
        rt_thread_delay(30);
    }

 }
 
 
int mqtt_to_server_start(void)
{
    rt_thread_t tid = RT_NULL;

    rt_err_t result;
    rt_pin_mode(NET_LED_PIN,PIN_MODE_OUTPUT);
    Net_Led_Status(LED_OFF);
    /* 初始化一个 mailbox */
    result = rt_mb_init(&mb,
                        "mbt",                      /* 名称是 mbt */
                        &mb_pool[0],                /* 邮箱用到的内存池是 mb_pool */
                        sizeof(mb_pool) / 4,        /* 邮箱中的邮件数目，因为一封邮件占 4 字节 */
                        RT_IPC_FLAG_FIFO);          /* 采用 FIFO 方式进行线程等待 */
    if (result != RT_EOK)
    {
        rt_kprintf("init mqtt mailbox failed.\n");
        return -1;
    }
    
    if(get_mqtt_status() == 0)
    {
        /* 开启mqtt线程 */
        user_mqtt_start();
    }
    
    tid = rt_thread_create( "mqttloop",
                            mqtt_rec_and_send_thread, RT_NULL,      // fun, parameter
                            2048,          // stack size
                            8, 10 );         //priority, tick      
    if (tid)
    {
        rt_thread_startup(tid);
    }

    return RT_EOK;
}

