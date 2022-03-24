#ifndef MENU_H
#define MENU_H

typedef struct menu_type
{
    unsigned char index; //ҳ������
    char title[20]; //�˵���
    
    unsigned char selected; //��ǰѡ��
    char menuItems[100];
    
    struct menu_type * prev_menu; //ǰҳ������
    
    void (*func)(unsigned char index,struct menu_type** cu_menu,void * param);  //ѡ����Ӧ���ִ�к���
}
menu_t;

extern menu_t menu_main;
//extern menu_t* menu_list[];

int Gui_Run_Init(void);
void Menu_Init(void);
#endif
