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
#ifndef MQTT_THREAD_H
#define MQTT_THREAD_H



#define MQTT_LINK_DOWNING   0      
#define MQTT_LINK_DOWN      1 
#define MQTT_LINK_UP        2
#define MQTT_LINKING        3 

//#define MQTT_REG_TOPIC          "/feedfactory/register" 

#define MQTT_REG_TOPIC          "/zngb/register"
#define URI_1									0	//���Է�������ַ 
#define URI_2									1	//���÷�����				4 DS
#define URI_3									2 //���ŷ�����--��� 	4 IND
#define URI_4									3 //��ʳ���ܹ�����������ַ
#define URI_5									4 //��ˮ������	2 IND
#define URI_6									5 //�ºӷ�����	2 DS
#define URI_7									6 //���µڶ���  2 IND
#define URI_8									7 //ƽ�����ϳ�	2 IND
#define URI_9									8 //�����ҵ԰	2 IND
#define URI_10								9 //�������ϳ�	4 DS
#define URI_11								10 //ͨ�ܷ�����	2 IND
#define URI_12								11 //�˶����ϳ�	2 IND
#define URI_13								12 //�ֵ顢��ԣ���ϳ�	2 IND
#define URI_14								13 //ũ�����ϳ�	2 IND
#define URI_15								14 //�������ϳ�	4 IND
#define URI_16								15 //�������ϳ�	2 IND
#define URI_17								16 //�������ϳ�	2 IND
#define URI_18								17 //�����صڶ����ϳ�	2 IND
#define URI_19								18 //��ˮ���ϳ�	2 IND
#define URI_20								19 //��ƽ�������ϳ�	2 IND
#define URI_21								20 //�������ϳ�	2 IND
#define URI_22								21 //˫�ɵڶ����ϳ�	4 IND
#define URI_23								22 //��Ұ�ڶ����ϳ�	2 IND
#define URI_24								23 //۰�ǵڶ����ϳ�	2 IND
#define URI_25								24 //������˰�����ϳ�	2 IND
#define URI_26								25 //��筵ڶ����ϳ�	4 DS
#define URI_27								26 //ͨ�����ϳ�	2 IND
#define URI_28								27 //ͭɽ�������ϳ�	2 IND
#define URI_29								28 //�������ϳ�	2 IND
#define URI_30								29 //ʯ�����ϳ�	2 IND
#define URI_31							  30 //�ƺӱϵ����ϳ�	4 IND
#define URI_32							  31 //ͭɽ�������ϳ�	2 IND
#define URI_33							  32 //��ԣ���ϳ�	    2 IND

#define URL_TYPE							URI_33

#if URL_TYPE==URI_1
	#define MQTT_URI                "tcp://10.106.11.37:11883"  // ���ò��Է�������ַ
#endif

#if URL_TYPE==URI_2								//���÷�����
	#define MQTT_URI                "tcp://192.168.0.100:1883"  // ���ò��Է�������ַ           // ����������Ϣ
#endif
#if URL_TYPE==URI_3								//���ŷ�����--���
	#define MQTT_URI                "tcp://192.168.10.100:1883"  // ���ò��Է�������ַ
#endif

#if URL_TYPE==URI_4								//��ʳ���ܹ�����������ַ
	#define MQTT_URI                "tcp://10.106.5.62:1883"  // ���ò��Է�������ַ
#endif 
#if URL_TYPE==URI_5								//��ˮ������
	#define MQTT_URI                "tcp://172.17.14.100:1883"  // ���ò��Է�������ַ
#endif
#if URL_TYPE==URI_6								//�ºӷ����� 
	#define MQTT_URI                "tcp://192.168.40.100:1883" // ���ò��Է�������ַ
#endif
#if URL_TYPE==URI_7								//���µڶ���
	#define MQTT_URI                "tcp://192.168.1.100:1883"  //
#endif
#if URL_TYPE==URI_8								//ƽ�����ϳ�
	#define MQTT_URI                "tcp://172.17.13.100:1883"  // 
#endif
#if URL_TYPE==URI_9								//�����ҵ԰
	#define MQTT_URI                "tcp://10.100.22.100:1883" 
#endif
#if URL_TYPE==URI_10								//�������ϳ�
	#define MQTT_URI                "tcp://192.168.0.100:1883" 
#endif
#if URL_TYPE==URI_11								//ͨ�ܷ�����
	#define MQTT_URI                "tcp://192.168.0.100:1883" 
#endif
#if URL_TYPE==URI_12								//�˶����ϳ�
	#define MQTT_URI                "tcp://192.168.31.100:1883" 
#endif
#if URL_TYPE==URI_13								//�ֵ����ϳ�
	#define MQTT_URI                "tcp://192.168.20.100:1883" 
#endif
#if URL_TYPE==URI_14								//ũ�����ϳ�
	#define MQTT_URI                "tcp://192.168.3.100:1883" 
#endif

#if URL_TYPE==URI_15								//�������ϳ�
	#define MQTT_URI                "tcp://192.168.1.100:1883" 
#endif

#if URL_TYPE==URI_16								//�������ϳ�
	#define MQTT_URI                "tcp://192.168.50.100:1883" 
#endif

#if URL_TYPE==URI_17								//�������ϳ�
	#define MQTT_URI                "tcp://192.168.40.100:1883" 
#endif

#if URL_TYPE==URI_18								//�����صڶ����ϳ�
	#define MQTT_URI                "tcp://192.168.40.100:1883" 
#endif

#if URL_TYPE==URI_19								//��ˮ���ϳ�
	#define MQTT_URI                "tcp://172.17.15.100:1883" 
#endif

#if URL_TYPE==URI_20								//��ƽ�������ϳ�
	#define MQTT_URI                "tcp://172.17.5.100:1883" 
#endif

#if URL_TYPE==URI_21								//�������ϳ�
	#define MQTT_URI                "tcp://172.17.14.100:1883" 
#endif

#if URL_TYPE==URI_22								//˫�ɵڶ����ϳ�
	#define MQTT_URI                "tcp://192.168.13.100:1883" 
#endif

#if URL_TYPE==URI_23								//��Ұ�ڶ����ϳ�
	#define MQTT_URI                "tcp://172.17.13.100:1883" 
#endif

#if URL_TYPE==URI_24								//۰�ǵڶ����ϳ�
	#define MQTT_URI                "tcp://172.17.12.100:1883" 
#endif

#if URL_TYPE==URI_25								//������˰�����ϳ�
	#define MQTT_URI                "tcp://172.17.9.100:1883" 
#endif

#if URL_TYPE==URI_26								//��筵ڶ����ϳ�
	#define MQTT_URI                "tcp://192.168.10.100:1883" 
#endif

#if URL_TYPE==URI_27							//ͨ�����ϳ�
	#define MQTT_URI                "tcp://172.17.13.100:1883" 
#endif

#if URL_TYPE==URI_28							//ͨ�����ϳ�
	#define MQTT_URI                "tcp://192.168.1.100:1883" 
#endif

#if URL_TYPE==URI_29							//�������ϳ�
	#define MQTT_URI                "tcp://172.17.3.100:1883" 
#endif

#if URL_TYPE==URI_30							//ʯ�����ϳ�
	#define MQTT_URI                "tcp://192.168.0.100:1883" 
#endif

#if URL_TYPE==URI_31							//�ƺӱϵ����ϳ�
	#define MQTT_URI                "tcp://172.17.13.100:1883" 
#endif

#if URL_TYPE==URI_32							//ͭɽ�������ϳ�
	#define MQTT_URI                "tcp://192.168.100.100:1883" 
#endif

#if URL_TYPE==URI_33						  //��ԣ���ϳ�
	#define MQTT_URI                "tcp://192.168.10.100:1883" 
#endif

int user_mqtt_start(void);
int user_mqtt_publish_to_server(unsigned char *msg_str,unsigned int len);
int user_mqtt_register(void);
int mqtt_stop(void);
unsigned char get_mqtt_status(void);
unsigned char Get_Mqtt_Link_Status(void);
void Set_Mqtt_Status(unsigned char mqtt_status);
int user_mqtt_publish(char *topic,unsigned char *msg_str,unsigned int len);
#endif
