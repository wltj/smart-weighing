#ifndef __HCI_H
#define __HCI_H

//typedef void (* SoundPayFun)(void);// 

//int Sound_Pay_start(void);
//void payOffLine(void);///离线 
//void payERROR1(void);
//void payERROR2(void);
//void payERROR3(void);
//void payStartWork(void);///开始称重
//void payWork1(void);//请某某车前进 

//void payWork2(void);//请某某车后退

//void payWork3(void);//请某某扫描二维码

//void payWork4(void);//请等待称重结束
//void payWork5 (void);// 
//void payWork6(void);//请离开
//void payWork7(void);
//void payWork8(void);
//void plearOntheScale (void);///请MOU某某车上磅

//void setSoundPayFun(SoundPayFun fun);
int hci_thread_start(void);
void updataHIC(void);
#endif
