#ifndef __TIPS_DATA_H_
#define __TIPS_DATA_H_

//NULL
#define TD_NULL 					""
//系统离线
#define TD_SYS_OFFLINE 		"7CFB7EDF79BB7EBF002C8BF768C067E57F517EDC"	

//--------------LED显示-------------------------------
//当前空闲
#define TD_SYS_FREE 			"5F53524D7A7A95F2FF01"
#define TD_SYS_SYSTEM 			"667A80FD8FC778C57CFB7EDF"
//XX正在称重
#define TD_WEIGHED_UP 		"%s6B63572879F091CD003A"	
//正在称重
#define TD_WEIGHED_UP2 		"6B63572879F091CD003A"	
//请准备
#define TD_NEXT_CAR 			"%s8BF751C65907"	
//.KG		
#define TD_WEIGHED 			"00200020%s002E%s004B0047"
//车辆未到位，请移动
#define TD_MOVE				"8F668F86672A52304F4DFF0C8BF779FB52A8"
//车辆未到位，请向场内移动
#define TD_MOVE1				"8F668F86672A52304F4DFF0C8BF75411573A518579FB52A8"
//车辆未到位，请向场外移动
#define TD_MOVE2				"8F668F86672A52304F4DFF0C8BF75411573A591679FB52A8"
//车辆未到位，请向东方移动
#define TD_MOVE3				"8F668F86672A52304F4DFF0C8BF754114E1C65B979FB52A8"
//车辆未到位，请向西方移动
#define TD_MOVE4				"8F668F86672A52304F4DFF0C8BF75411897F65B979FB52A8"
//车辆未到位，请向南方移动
#define TD_MOVE5				"8F668F86672A52304F4DFF0C8BF75411535765B979FB52A8"
//车辆未到位，请向北方移动
#define TD_MOVE6				"8F668F86672A52304F4DFF0C8BF75411531765B979FB52A8"

//请扫描二维码
#define TD_SCAN_RQ			"8BF7626B63CF4E8C7EF47801"
//未识别车牌
#define TD_NO_CAR_NUM		"8F66724C672A8BC6522B"
//未识别射频卡
#define TD_NO_CARD			"672A68C06D4B52305C0498915361"
//等待结束
#define TD_WAIT					"7B495F8579F091CD7ED3675F"
//服务器提示异常
#define TD_SEV_ERROR			"670D52A1566863D0793A5F025E38000D"
//检测遥控监测仪离线
#define TD_CHECK_REMOTE					"8BF768C067E5906563A776D16D4B4EEA"
//检测扩展板
#define TD_CHECK_EXPAND					"8BF768C067E562695C55677F"
//检测地磅
#define TD_CHECK_WEIGH					"8BF768C067E5573078C5"
//检测摄像头
#define TD_CHECK_CAMERA1					"8BF768C067E563A75236677F644450CF5934"
//检测摄像头
#define TD_CHECK_CAMERA2					"8BF768C067E562695C55677F644450CF5934"
//检测摄像头
#define TD_CHECK_RFID					"8BF768C067E55C0498918BFB53615668"
//XX质检失败
#define TD_QUALITYTESTINGFAIL 		"%s8D2868C0672A901A8FC7002C8BF779BB5F00"	
//XX质检失败
#define TD_QUALITYTESTINGFAIL2 		"8D2868C0672A901A8FC7002C8BF779BB5F00"	
//存在异常遥控信号
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
