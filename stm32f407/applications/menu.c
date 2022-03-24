#include <stdio.h>
#include <string.h>
#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>
#include <gui_thread.h>
#include "menu.h"
#include <fal.h>
#include "gateway.h"
#include "easyflash.h"
#include "pole.h"
/*
    ���˵�
    unit
    wireless
    term
    contorl
    test
    error
*/
menu_t menu_main =
    {
        /* main ��ҳ */

        0,                                                                   /* ҳ������ */
        "Main",                                                              /* ҳ������ */
        1,                                                                   /* ��ǰѡ�� */
        "About\nSetting\nTest\nError", /* ѡ�� */
        0,                                                                   /* ǰ��ҳ�� */
};

/*
    About �˵�
    unit type
    unit num
    term num
*/
menu_t menu_about =
    {
        1,
        "ABOUT",
        1,
        "Unit Type\nUnit Num\nTerm Num",
        &menu_main,
};


menu_t menu_Test =
    {
        3,
        "TEST",
        1,
        "Open\nClose\nStop\nNet",
        &menu_main,
};

menu_t menu_Setting =
    {
        2,
        "SETTING",
        1,
        "Main_Board\nOutput_Board",
        &menu_main,
};

//menu_t* menu_list[10] = {&menu_main,&menu_unit,&menu_wireless,&menu_Term,&menu_Control,&menu_Test};

void Menu_Main_Fun(unsigned char index, menu_t **cu_menu, void *param);
void Menu_Test_Fun(unsigned char index, menu_t **cu_menu, void *param);
void Menu_Setting_Fun(unsigned char index, menu_t **cu_menu, void *param);
void Menu_Init(void)
{
    menu_main.func = Menu_Main_Fun;
    //menu_about.func = Menu_About_Fun;
    menu_Setting.func = Menu_Setting_Fun;
    menu_Test.func = Menu_Test_Fun;
}

void Menu_Main_Fun(unsigned char index, menu_t **cu_menu, void *param)
{
    menu_main.selected = index;
    switch (menu_main.selected)
    {
    case 0:
        break;

    case 1:
        //*cu_menu = &menu_unit;
        break;

    case 2:
        *cu_menu = &menu_Setting;
        break;

    case 3:
        *cu_menu = &menu_Test;
        break;

    default:
        break;
    }
}

void Menu_Test_Fun(unsigned char index, menu_t **cu_menu, void *param)
{
    switch (index)
    {
    case 0:
        *cu_menu = menu_Test.prev_menu;
        break;

    /* open */
    case 1:
        openRoadGate();
        break;
    
    /* close */
    case 2:
        closeRoadGate();
        break;
    
    /* stop */
    case 3:
        stopRoadGate();
        break;
    }
}


void Menu_Setting_Fun(unsigned char index, menu_t **cu_menu, void *param)
{
    switch (index)
    {
    case 0:
        *cu_menu = menu_Setting.prev_menu;
        break;

    /* ���� */
    case 1:
        if (u8g2_UserInterfaceMessage(&u8g2, "", "", "Change Board type to main board!!!", "Yes\nCancel") == 1)
        {
            
        }
        break;
    /* ����� */    
    case 2:
        if (u8g2_UserInterfaceMessage(&u8g2, "", "", "Change Board type to output board!!!", "Yes\nCancel") == 1)
        {
            
        }
        break;
    }
}
