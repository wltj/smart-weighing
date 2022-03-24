#ifndef __SYSTEM_PARAMS_H_
#define __SYSTEM_PARAMS_H_
 
#define MACID_STRING_LENTH  12
unsigned char *getNameArray(void);
char *getMACArray(void);


//��Flash�ж����豸����
unsigned char * readDevName(void);
//���豸���Ʊ��浽Flash��
void saveDevName( char *s);

unsigned char  readCheckRM(void);
void saveCheckRM(unsigned char checkRm);

unsigned char * readDevType(void); 
void saveDevType(unsigned char s);


unsigned char  readSound(void);
void saveSound(unsigned char sound);


char * readServerAddress( char * defaultUrl);
char * getServerAddress(void);
void saveServerAddress( char *s);

unsigned char * readWeighType(void);
//���ذ����ͱ��浽Flash��
void saveWeighType(unsigned char type);

void setParamWithRQData(unsigned char *data);

int netdev_to_get_ip(char * my_url, int url_len);

unsigned char * readREDCnt(void);
void saveREDCnt(unsigned char cnt);

unsigned char * readLocationErrorInfo(void);
void saveLocationErrorInfo(unsigned char type);
void initSystemParams( char *url);
#endif
