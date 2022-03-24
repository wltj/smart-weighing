#ifndef COM_TO_SERVER_CMD_LIST_H
#define COM_TO_SERVER_CMD_LIST_H


int Gateway_Link_Server(void);
int Gateway_Register_To_Server(void);
int Common_Ack_Decode(unsigned char *pb,unsigned int len);
int Decode_Wireless_Para(unsigned char *pb,unsigned int len);
int Decode_Term_Sys_Para(unsigned char *pb,unsigned int len);
int Decode_Feed_Para(unsigned char *pb,unsigned int len);
int Send_Term_Report(void);
int Decode_Gateway_Upgread_Param(unsigned char *pb,unsigned int len);
int Decode_Term_Upgread_Param(unsigned char *pb,unsigned int len);
#endif
