#ifndef COM_TO_SERVER_THREAD
#define COM_TO_SERVER_THREAD

 extern struct rt_mailbox mb;
 int mqtt_to_server_start(void);
 void mqtt_ack(unsigned short cmd,unsigned short err_status);
 unsigned short rec_mqtt_ack(unsigned short cmd,unsigned int timeout);
 
 #endif
