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
#define URI_1									0	//测试服务器地址 
#define URI_2									1	//永济服务器				4 DS
#define URI_3									2 //灌张服务器--新绛 	4 IND
#define URI_4									3 //肉食智能过磅服务器地址
#define URI_5									4 //白水服务器	2 IND
#define URI_6									5 //新河服务器	2 DS
#define URI_7									6 //阜新第二磅  2 IND
#define URI_8									7 //平舆饲料厂	2 IND
#define URI_9									8 //内乡产业园	2 IND
#define URI_10								9 //海兴饲料厂	4 DS
#define URI_11								10 //通榆服务器	2 IND
#define URI_12								11 //克东饲料厂	2 IND
#define URI_13								12 //林甸、富裕饲料厂	2 IND
#define URI_14								13 //农安饲料厂	2 IND
#define URI_15								14 //测试饲料厂	4 IND
#define URI_16								15 //凉州饲料厂	2 IND
#define URI_17								16 //枣阳饲料厂	2 IND
#define URI_18								17 //乌拉特第二饲料厂	2 IND
#define URI_19								18 //明水饲料厂	2 IND
#define URI_20								19 //邹平第三饲料厂	2 IND
#define URI_21								20 //兰西饲料厂	2 IND
#define URI_22								21 //双辽第二饲料厂	4 IND
#define URI_23								22 //新野第二饲料厂	2 IND
#define URI_24								23 //郯城第二饲料厂	2 IND
#define URI_25								24 //南阳保税区饲料厂	2 IND
#define URI_26								25 //新绛第二饲料厂	4 DS
#define URI_27								26 //通榆饲料厂	2 IND
#define URI_28								27 //铜山第四饲料厂	2 IND
#define URI_29								28 //繁峙饲料厂	2 IND
#define URI_30								29 //石首饲料厂	2 IND
#define URI_31							  30 //唐河毕店饲料厂	4 IND
#define URI_32							  31 //铜山第四饲料厂	2 IND
#define URI_33							  32 //富裕饲料厂	    2 IND

#define URL_TYPE							URI_33

#if URL_TYPE==URI_1
	#define MQTT_URI                "tcp://10.106.11.37:11883"  // 配置测试服务器地址
#endif

#if URL_TYPE==URI_2								//永济服务器
	#define MQTT_URI                "tcp://192.168.0.100:1883"  // 配置测试服务器地址           // 设置遗言消息
#endif
#if URL_TYPE==URI_3								//灌张服务器--新绛
	#define MQTT_URI                "tcp://192.168.10.100:1883"  // 配置测试服务器地址
#endif

#if URL_TYPE==URI_4								//肉食智能过磅服务器地址
	#define MQTT_URI                "tcp://10.106.5.62:1883"  // 配置测试服务器地址
#endif 
#if URL_TYPE==URI_5								//白水服务器
	#define MQTT_URI                "tcp://172.17.14.100:1883"  // 配置测试服务器地址
#endif
#if URL_TYPE==URI_6								//新河服务器 
	#define MQTT_URI                "tcp://192.168.40.100:1883" // 配置测试服务器地址
#endif
#if URL_TYPE==URI_7								//阜新第二磅
	#define MQTT_URI                "tcp://192.168.1.100:1883"  //
#endif
#if URL_TYPE==URI_8								//平舆饲料厂
	#define MQTT_URI                "tcp://172.17.13.100:1883"  // 
#endif
#if URL_TYPE==URI_9								//内乡产业园
	#define MQTT_URI                "tcp://10.100.22.100:1883" 
#endif
#if URL_TYPE==URI_10								//海兴饲料厂
	#define MQTT_URI                "tcp://192.168.0.100:1883" 
#endif
#if URL_TYPE==URI_11								//通榆服务器
	#define MQTT_URI                "tcp://192.168.0.100:1883" 
#endif
#if URL_TYPE==URI_12								//克东饲料厂
	#define MQTT_URI                "tcp://192.168.31.100:1883" 
#endif
#if URL_TYPE==URI_13								//林甸饲料厂
	#define MQTT_URI                "tcp://192.168.20.100:1883" 
#endif
#if URL_TYPE==URI_14								//农安饲料厂
	#define MQTT_URI                "tcp://192.168.3.100:1883" 
#endif

#if URL_TYPE==URI_15								//测试饲料厂
	#define MQTT_URI                "tcp://192.168.1.100:1883" 
#endif

#if URL_TYPE==URI_16								//凉州饲料厂
	#define MQTT_URI                "tcp://192.168.50.100:1883" 
#endif

#if URL_TYPE==URI_17								//凉州饲料厂
	#define MQTT_URI                "tcp://192.168.40.100:1883" 
#endif

#if URL_TYPE==URI_18								//乌拉特第二饲料厂
	#define MQTT_URI                "tcp://192.168.40.100:1883" 
#endif

#if URL_TYPE==URI_19								//明水饲料厂
	#define MQTT_URI                "tcp://172.17.15.100:1883" 
#endif

#if URL_TYPE==URI_20								//邹平第三饲料厂
	#define MQTT_URI                "tcp://172.17.5.100:1883" 
#endif

#if URL_TYPE==URI_21								//兰西饲料厂
	#define MQTT_URI                "tcp://172.17.14.100:1883" 
#endif

#if URL_TYPE==URI_22								//双辽第二饲料厂
	#define MQTT_URI                "tcp://192.168.13.100:1883" 
#endif

#if URL_TYPE==URI_23								//新野第二饲料厂
	#define MQTT_URI                "tcp://172.17.13.100:1883" 
#endif

#if URL_TYPE==URI_24								//郯城第二饲料厂
	#define MQTT_URI                "tcp://172.17.12.100:1883" 
#endif

#if URL_TYPE==URI_25								//南阳保税区饲料厂
	#define MQTT_URI                "tcp://172.17.9.100:1883" 
#endif

#if URL_TYPE==URI_26								//新绛第二饲料厂
	#define MQTT_URI                "tcp://192.168.10.100:1883" 
#endif

#if URL_TYPE==URI_27							//通榆饲料厂
	#define MQTT_URI                "tcp://172.17.13.100:1883" 
#endif

#if URL_TYPE==URI_28							//通榆饲料厂
	#define MQTT_URI                "tcp://192.168.1.100:1883" 
#endif

#if URL_TYPE==URI_29							//繁峙饲料厂
	#define MQTT_URI                "tcp://172.17.3.100:1883" 
#endif

#if URL_TYPE==URI_30							//石首饲料厂
	#define MQTT_URI                "tcp://192.168.0.100:1883" 
#endif

#if URL_TYPE==URI_31							//唐河毕店饲料厂
	#define MQTT_URI                "tcp://172.17.13.100:1883" 
#endif

#if URL_TYPE==URI_32							//铜山第四饲料厂
	#define MQTT_URI                "tcp://192.168.100.100:1883" 
#endif

#if URL_TYPE==URI_33						  //富裕饲料厂
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
