#ifndef SOUND_H
#define SOUND_H



typedef void (* SoundPayFun)(void);// 
 
void playSound(void);
void payOffLine(void);///���� 
void payERROR1(void);
void payERROR2(void);
void payERROR3(void);
void payERROR4(void);
void payERROR5(void);
void payERROR6(void);
void payERROR7(void);//�����쳣ң���ź�,����


void paySevERROR(void);
void payStartWork(void);///��ʼ����
void payWork1(void);//��ĳĳ��ǰ�� 

void payWork2(void);//��ĳĳ������

void payWork3(void);//��ĳĳɨ���ά��

void payWork4(void);//��ȴ����ؽ���
void payWork5 (void);// 
void payWork6(void);//���뿪
void payWork7(void);
void payWork8(void);
void payWork9(void);//�ʼ�ʧ��
void plearOntheScale (void);///��MOUĳĳ���ϰ�

void setSoundPayFun(SoundPayFun fun);

#endif
