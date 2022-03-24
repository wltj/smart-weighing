/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-03-05     Jason        first version
 * 
 */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <rtthread.h>

#define DBG_ENABLE
#define DBG_SECTION_NAME    "mqtt"
#define DBG_LEVEL           DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

#include "paho_mqtt.h"
#include "cmd_handle.h"
#include "mqtt_thread.h"
#include "com_to_server_cmd_list.h"
#include "main_task.h"
#include "system_params.h"
/**
 * MQTT URI farmat:
 * domain mode
 * tcp://iot.eclipse.org:1883
 *
 * ipv4 mode
 * tcp://192.168.10.1:1883
 * ssl://192.168.10.1:1884
 *
 * ipv6 mode
 * tcp://[fe80::20c:29ff:fe9a:a07e]:1883
 * ssl://[fe80::20c:29ff:fe9a:a07e]:1884
 */



#define MQTT_USERNAME           "admin"   
#define MQTT_PASSWORD           "My002714!@"
#define MQTT_SUBTOPIC_HEAD      "/zngb/servertogateway/"// ���ö�������
#define MQTT_PUBTOPIC_HEAD      "/zngb/gatewaytoserver/"// ������������
//#define MQTT_PUBTOPIC           "/test/1"                       
#define MQTT_WILLMSG            "Goodbye!"                     // ����������Ϣ
	

	

/* ���� MQTT �ͻ��˻����ṹ�� */
static MQTTClient client;
static int is_started = 0;

static char mqtt_sub_topic[50];
static char mqtt_pub_topic[50];
static char mqttlink,mqttlink_bck;

extern rt_err_t get_mac_addr(unsigned char *mac);



void Set_Mqtt_Status(unsigned char mqtt_status)
{
    switch(mqtt_status)
    {
        case MQTT_LINK_UP:
            mqttlink = 1;
            mqttlink_bck = 0;
        break;
        
        case MQTT_LINKING:
            mqttlink = 1;
            mqttlink_bck = 1;
        break;
        
        case MQTT_LINK_DOWN:
            mqttlink = 0;
            mqttlink_bck = 1;
        break;
        
        case MQTT_LINK_DOWNING:
            mqttlink = 0;
            mqttlink_bck = 0;
        break;
    }
}

unsigned char Get_Mqtt_Link_Status(void)
{
    if(mqttlink==1 && mqttlink_bck == 0)
        return MQTT_LINK_UP;
    
    else if(mqttlink==1 && mqttlink_bck == 1)
        return MQTT_LINKING;
    
    else if(mqttlink==0 && mqttlink_bck == 1)
        return MQTT_LINK_DOWN;
    
    else if(mqttlink==0 && mqttlink_bck == 0)
        return MQTT_LINK_DOWNING;
		return MQTT_LINK_DOWNING;
}

/* ����Ψһid*/
static void init_mqtt_para(void)
{

    /*��������*/
    rt_sprintf(mqtt_sub_topic,"%s%s",MQTT_SUBTOPIC_HEAD,systemStatus.macidstring);
    /*��������*/
    rt_sprintf(mqtt_pub_topic,"%s%s",MQTT_PUBTOPIC_HEAD,systemStatus.macidstring);
    LOG_D("mqtt_sub_topic  %s",mqtt_sub_topic);
    LOG_D("mqtt_pub_topic  %s",mqtt_pub_topic);
}



/* MQTT �����¼��Զ���ص����� */
static void mqtt_sub_callback(MQTTClient *c, MessageData *msg_data)
{
    //*((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
//    LOG_D("mqtt sub callback: %.*s %.*s",
//               msg_data->topicName->lenstring.len,
//               msg_data->topicName->lenstring.data,
//               msg_data->message->payloadlen,
//               (char *)msg_data->message->payload);
    //LOG_D("mqtt rec msg!");
    /*�������ָ��Ľ��պͷ���*/
    cmd_rec_handle((unsigned char *)msg_data->message->payload,msg_data->message->payloadlen);
}

/* MQTT �����¼�Ĭ�ϻص����� */
static void mqtt_sub_default_callback(MQTTClient *c, MessageData *msg_data)
{
    *((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
    LOG_D("mqtt sub default callback: %.*s %.*s",
               msg_data->topicName->lenstring.len,
               msg_data->topicName->lenstring.data,
               msg_data->message->payloadlen,
               (char *)msg_data->message->payload);
}

/* MQTT �����¼��ص����� */
static void mqtt_connect_callback(MQTTClient *c)
{
    LOG_D("inter mqtt_connect_callback!");
}

/* MQTT �����¼��ص����� */
static void mqtt_online_callback(MQTTClient *c)
{
    /* �������� */
    Set_Mqtt_Status(MQTT_LINK_UP);
    //LOG_D("inter mqtt_online_callback!");
}

/* MQTT �����¼��ص����� */
static void mqtt_offline_callback(MQTTClient *c)
{
    Set_Mqtt_Status(MQTT_LINK_DOWN);
    //LOG_D("inter mqtt_offline_callback!");
}

/*
    ��ɶ�/feed/servertogateway/MACID���ģ�������Ϣ����
*/
int user_mqtt_start(void)
{
    /* ʹ�� MQTTPacket_connectData_initializer ��ʼ�� condata ���� */
    MQTTPacket_connectData condata = MQTTPacket_connectData_initializer;
    static char cid[30] = { 0 };
    if (is_started)
    {
        LOG_E("mqtt client is already connected.");
        return -1;
    }
     /* ���� MQTT �ṹ�����ݲ��� */
    {
        init_mqtt_para();
        
        client.isconnected = 0;
        //client.uri = getServerAddress();
				client.uri = systemStatus.my_url;
			rt_kprintf("\\\\\\\\\\\\\\\\\\\\\\\\client.uri:%s\n",client.uri);
        /* ��������Ŀͻ��� ID */
        memset(cid,0,sizeof(cid));
        rt_sprintf(cid,"feed_%s",systemStatus.macidstring);
        //rt_snprintf(cid, sizeof(cid), "rtthread%d", rt_tick_get());
        /* �������Ӳ��� */
        memcpy(&client.condata, &condata, sizeof(condata));
        client.condata.clientID.cstring = cid;
        client.condata.keepAliveInterval = 20;
        client.condata.cleansession = 1;
        client.condata.username.cstring = MQTT_USERNAME;
        client.condata.password.cstring = MQTT_PASSWORD;

        /* ���� MQTT ���Բ��� */
        client.condata.willFlag = 1;
        client.condata.will.qos = 1;
        client.condata.will.retained = 0;
        client.condata.will.topicName.cstring = mqtt_pub_topic;
        client.condata.will.message.cstring = MQTT_WILLMSG;

         /* ���仺���� */
        client.buf_size = client.readbuf_size = 1024;
        client.buf = rt_calloc(1, client.buf_size);
        client.readbuf = rt_calloc(1, client.readbuf_size);
        if (!(client.buf && client.readbuf))
        {
            LOG_E("no memory for MQTT client buffer!");
            return -1;
        }

        /* �����¼��ص����� */
        client.connect_callback = mqtt_connect_callback;
        client.online_callback = mqtt_online_callback;
        client.offline_callback = mqtt_offline_callback;

        /* ���ö��ı���¼��ص�����*/
        client.messageHandlers[0].topicFilter = rt_strdup(mqtt_sub_topic);
        client.messageHandlers[0].callback = mqtt_sub_callback;
        client.messageHandlers[0].qos = QOS1;

        /* ����Ĭ�ϵĶ�������*/
        client.defaultMessageHandler = mqtt_sub_default_callback;
    }

    /* ���� MQTT �ͻ��� */
    paho_mqtt_start(&client);
    is_started = 1;

    return 0;
}

/* �ú�������ֹͣ MQTT �ͻ��˲��ͷ��ڴ�ռ� */
int mqtt_stop(void)
{

    rt_kprintf("mqtt_stop    --stop mqtt worker thread and free mqtt client object.\n");

    is_started = 0;

    return paho_mqtt_stop(&client);
}

/* �ú������ڷ������ݵ�ָ�� topic */
int user_mqtt_publish(char *topic,unsigned char *msg_str,unsigned int len)
{
    if (is_started == 0)
    {
        LOG_E("mqtt client is not connected.");
        return -1;
    }

    if (topic && len)
    {
        user_paho_mqtt_publish(&client, QOS1, topic, msg_str,len);
    }
    else
    {
        rt_kprintf("topic error or len error\n");
        return -1;
    }

    return 0;
}

/* ��������������Ϣ����*/
int user_mqtt_publish_to_server(unsigned char *msg_str,unsigned int len)
{
		//rt_kprintf("topic=%s\n",mqtt_pub_topic);
    return user_mqtt_publish(mqtt_pub_topic,msg_str,len);
}

/* ����ע�ắ��*/
int user_mqtt_register(void)
{
    return 0;
}

/* MQTT �µĶ����¼��Զ���ص����� */
static void mqtt_new_sub_callback(MQTTClient *client, MessageData *msg_data)
{
    *((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
    LOG_D("mqtt new subscribe callback: %.*s %.*s",
               msg_data->topicName->lenstring.len,
               msg_data->topicName->lenstring.data,
               msg_data->message->payloadlen,
               (char *)msg_data->message->payload);
}

/* �ú������ڶ����µ� Topic */
static int user_mqtt_subscribe(char *topic)
{
    if (topic == RT_NULL)
    {
        rt_kprintf("mqtt_subscribe [topic]  --send an mqtt subscribe packet and wait for suback before returning.\n");
        return -1;
    }
	
	if (is_started == 0)
    {
        LOG_E("mqtt client is not connected.");
        return -1;
    }

    return paho_mqtt_subscribe(&client, QOS1, topic, mqtt_new_sub_callback);
}

/* �ú�������ȡ������ָ���� Topic */
static int user_mqtt_unsubscribe(char *topic)
{
    if (topic == RT_NULL)
    {
        rt_kprintf("mqtt_unsubscribe [topic]  --send an mqtt unsubscribe packet and wait for suback before returning.\n");
        return -1;
    }
	
	if (is_started == 0)
    {
        LOG_E("mqtt client is not connected.");
        return -1;
    }

    return paho_mqtt_unsubscribe(&client, topic);
}

/* ��ȡmqtt����״̬*/
unsigned char get_mqtt_status(void)
{
    return is_started;
}



