#ifndef __TIPS_DATA_H_
#define __TIPS_DATA_H_

//NULL
#define TD_NULL 					""
//ϵͳ����
#define TD_SYS_OFFLINE 		"7CFB7EDF79BB7EBF002C8BF768C067E57F517EDC"	

//--------------LED��ʾ-------------------------------
//��ǰ����
#define TD_SYS_FREE 			"5F53524D7A7A95F2FF01"
#define TD_SYS_SYSTEM 			"667A80FD8FC778C57CFB7EDF"
//XX���ڳ���
#define TD_WEIGHED_UP 		"%s6B63572879F091CD003A"	
//���ڳ���
#define TD_WEIGHED_UP2 		"6B63572879F091CD003A"	
//��׼��
#define TD_NEXT_CAR 			"%s8BF751C65907"	
//.KG		
#define TD_WEIGHED 			"00200020%s002E%s004B0047"
//����δ��λ�����ƶ�
#define TD_MOVE				"8F668F86672A52304F4DFF0C8BF779FB52A8"
//����δ��λ���������ƶ�
#define TD_MOVE1				"8F668F86672A52304F4DFF0C8BF75411573A518579FB52A8"
//����δ��λ���������ƶ�
#define TD_MOVE2				"8F668F86672A52304F4DFF0C8BF75411573A591679FB52A8"
//����δ��λ�����򶫷��ƶ�
#define TD_MOVE3				"8F668F86672A52304F4DFF0C8BF754114E1C65B979FB52A8"
//����δ��λ�����������ƶ�
#define TD_MOVE4				"8F668F86672A52304F4DFF0C8BF75411897F65B979FB52A8"
//����δ��λ�������Ϸ��ƶ�
#define TD_MOVE5				"8F668F86672A52304F4DFF0C8BF75411535765B979FB52A8"
//����δ��λ�����򱱷��ƶ�
#define TD_MOVE6				"8F668F86672A52304F4DFF0C8BF75411531765B979FB52A8"

//��ɨ���ά��
#define TD_SCAN_RQ			"8BF7626B63CF4E8C7EF47801"
//δʶ����
#define TD_NO_CAR_NUM		"8F66724C672A8BC6522B"
//δʶ����Ƶ��
#define TD_NO_CARD			"672A68C06D4B52305C0498915361"
//�ȴ�����
#define TD_WAIT					"7B495F8579F091CD7ED3675F"
//��������ʾ�쳣
#define TD_SEV_ERROR			"670D52A1566863D0793A5F025E38000D"
//���ң�ؼ��������
#define TD_CHECK_REMOTE					"8BF768C067E5906563A776D16D4B4EEA"
//�����չ��
#define TD_CHECK_EXPAND					"8BF768C067E562695C55677F"
//���ذ�
#define TD_CHECK_WEIGH					"8BF768C067E5573078C5"
//�������ͷ
#define TD_CHECK_CAMERA1					"8BF768C067E563A75236677F644450CF5934"
//�������ͷ
#define TD_CHECK_CAMERA2					"8BF768C067E562695C55677F644450CF5934"
//�������ͷ
#define TD_CHECK_RFID					"8BF768C067E55C0498918BFB53615668"
//XX�ʼ�ʧ��
#define TD_QUALITYTESTINGFAIL 		"%s8D2868C0672A901A8FC7002C8BF779BB5F00"	
//XX�ʼ�ʧ��
#define TD_QUALITYTESTINGFAIL2 		"8D2868C0672A901A8FC7002C8BF779BB5F00"	
//�����쳣ң���ź�
#define TD_REMOTE_CHECK_ERROR				"5B5857285F025E38906563A74FE153F7"
unsigned short charToShortHex(unsigned char * chars); 
char Char_To_Hex(unsigned char * chars);
void String_To_Unicode_To_Hex(unsigned char *str ,unsigned char *data_hex,unsigned short len);
void Unicode_8_To_Hex(unsigned char *data_unicode,unsigned char *data_hex,unsigned int len);
void Unicode_16_To_Hex(unsigned short *data_unicode,unsigned char *data_hex,unsigned int len);

unsigned char getProvinceIndex(unsigned char * data);
unsigned char getLetterIndex(unsigned char * data);
unsigned char getNumberIndex(unsigned char * data);
void getLocationErrorStr( char *buff,unsigned long len,unsigned char type,unsigned char status);
#endif
