#ifndef MENU_H
#define MENU_H

typedef struct menu_type
{
    unsigned char index; //页面索引
    char title[20]; //菜单名
    
    unsigned char selected; //当前选项
    char menuItems[100];
    
    struct menu_type * prev_menu; //前页面索引
    
    void (*func)(unsigned char index,struct menu_type** cu_menu,void * param);  //选择相应项后执行函数
}
menu_t;

extern menu_t menu_main;
//extern menu_t* menu_list[];

int Gui_Run_Init(void);
void Menu_Init(void);
#endif
