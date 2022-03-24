//#include <stdio.h>
//#include <string.h>
//#include <rtthread.h>
//#include <rthw.h>
//#include <rtdevice.h>
//#include <gui_thread.h>
//#include "menu.h"
//#include <fal.h>
//#include "gateway.h"
//#include "easyflash.h"
///*
//    主菜单
//    unit
//    wireless
//    term
//    contorl
//    test
//    error
//*/
//menu_t menu_main =
//    {
//        /* main 主页 */

//        0,                                                                   /* 页面索引 */
//        "Main",                                                              /* 页面名称 */
//        1,                                                                   /* 当前选项 */
//        "Unit\nWireless\nTerm\nContol\nTest\nLora Pair\nSys Setting\nError", /* 选项 */
//        0,                                                                   /* 前级页面 */
//};

///*
//    Unit菜单
//    unit type
//    unit num
//    term num
//*/
//menu_t menu_unit =
//    {
//        1,
//        "UNIT",
//        1,
//        "Unit Type\nUnit Num\nTerm Num",
//        &menu_main,
//};

//menu_t menu_wireless =
//    {
//        2,
//        "WIRELESS",
//        1,
//        "Net Id\nWireless Channle\nto register\nlora send test\nWireless power set",
//        &menu_main,
//};

//menu_t menu_Term =
//    {
//        3,
//        "TERM",
//        1,
//        "Feed Speed\nTail Value\nFlow Pulse",
//        &menu_main,
//};

//menu_t menu_Control =
//    {
//        4,
//        "CONTROL",
//        1,
//        "Feed\nWater\nClean Food\nSetting",
//        &menu_main,
//};

//menu_t menu_Control_Feed =
//    {
//        4,
//        "CONTROL FEED",
//        1,
//        "Feed 10 sec\nFeed 1 min\nMotor running",
//        &menu_Control,
//};

//menu_t menu_Test =
//    {
//        5,
//        "TEST",
//        1,
//        "Feed\nWater\nWireless\nNet",
//        &menu_main,
//};

//menu_t menu_Setting =
//    {
//        5,
//        "SETTING",
//        1,
//        "ResetEnv\nnull",
//        &menu_main,
//};

////menu_t* menu_list[10] = {&menu_main,&menu_unit,&menu_wireless,&menu_Term,&menu_Control,&menu_Test};

//void Menu_Main_Fun(unsigned char index, menu_t **cu_menu, void *param);
//void Menu_Unit_Fun(unsigned char index, menu_t **cu_menu, void *param);
//void Menu_Wireless_Fun(unsigned char index, menu_t **cu_menu, void *param);
//void Menu_Term_Fun(unsigned char index, menu_t **cu_menu, void *param);
//void Menu_Setting_Fun(unsigned char index, menu_t **cu_menu, void *param);
//void Menu_Init(void)
//{
//    menu_main.func = Menu_Main_Fun;
//    menu_unit.func = Menu_Unit_Fun;
//    menu_wireless.func = Menu_Wireless_Fun;
//    menu_Term.func = Menu_Term_Fun;
//    menu_Setting.func = Menu_Setting_Fun;
//}

//void Menu_Main_Fun(unsigned char index, menu_t **cu_menu, void *param)
//{
//    menu_main.selected = index;
//    switch (menu_main.selected)
//    {
//    case 0:
//        break;

//    case 1:
//        *cu_menu = &menu_unit;
//        break;

//    case 2:
//        *cu_menu = &menu_wireless;
//        break;

//    case 3:
//        *cu_menu = &menu_Term;
//        break;

//    case 4:
//        //*cu_menu = &menu_Control;
//        break;

//    case 5:
//        //*cu_menu = &menu_Test;
//        break;

//    case 6:
//        rt_kprintf("[lora] LoRa send test mode!\n");
//        //Lora_To_Test_Mode();
//        u8g2_UserInterfaceMessage(&u8g2, "", "", "lora send test!!!", "Cancel");
//        //Lora_Out_Test_Mode();
//        rt_kprintf("[lora] LoRa out test mode!\n");
//        break;

//    case 7:
//        *cu_menu = &menu_Setting;
//        break;

//    default:
//        break;
//    }
//}
//void resetenv(uint8_t argc, char **argv);
//void Menu_Unit_Fun(unsigned char index, menu_t **cu_menu, void *param)
//{
//    menu_unit.selected = index;

//    switch (index)
//    {
//    case 0:
//        *cu_menu = menu_unit.prev_menu;
//        break;

//    case 1:
////        if (u8g2_UserInterfaceInputValue(&u8g2, "\n1.lactating\n2.gestating", "type = ", (uint8_t *)&gateway.unit_type, 1, 10, 2, ""))
////        {
////            Save_Gateway_Param();
////        }

//        break;

//    case 2:

////        if (u8g2_UserInterfaceInputValue_u16(&u8g2, "\nset unit num", "uit num = ", &gateway.unit_num, 0, 9999, 4, ""))
////        {
////            Save_Gateway_Param();
////        }
//        break;

//    case 3:
////        if (u8g2_UserInterfaceInputValue_u16(&u8g2, "\nset term num", "term num = ", &gateway.term_num, 0, 9999, 4, ""))
////        {
////            Save_Gateway_Param();
////            resetenv(0, RT_NULL);
////            //Term_List_Init();
////        }

//        break;

//    default:
//        break;
//    }
//}

//void Menu_Wireless_Fun(unsigned char index, menu_t **cu_menu, void *param)
//{
//    menu_wireless.selected = index;

//    unsigned char str[20];
//    memset(str, 0, 20);
//    switch (index)
//    {
//    case 0:
//        *cu_menu = menu_wireless.prev_menu;
//        break;

//    case 1:
//        
//        break;

//    case 2:
////        if (u8g2_UserInterfaceInputValue(&u8g2, "\nset wireless channle", "channle = ", (uint8_t *)&gateway.wireless_param.wireless_channle, 0, 20, 2, ""))
////        {
////            lora.lora_channle_switch = 1;
////        }

//        break;

//    case 3:
//        rt_kprintf("[lora] LoRa in register mode!\n");
//        //Lora_To_Register();
//        u8g2_UserInterfaceMessage(&u8g2, "", "", "lora register!!!", "Cancel");
//        //Lora_Back_To_Normal();
//        rt_kprintf("[lora] LoRa out register mode!\n");
//        break;

//    case 4:
//        rt_kprintf("[lora] LoRa send test mode!\n");
//        //Lora_To_Test_Mode();
//        u8g2_UserInterfaceMessage(&u8g2, "", "", "lora send test!!!", "Cancel");
//        //Lora_Out_Test_Mode();
//        rt_kprintf("[lora] LoRa out test mode!\n");
//        break;

//    case 5: //信号强度
////        if (u8g2_UserInterfaceInputValue(&u8g2, "\nset wireless power\npower from 0 to 15", "power = ", (uint8_t *)&gateway.wireless_power, 0, 15, 2, ""))
////        {
////            //Lora_Set_Power(gateway.wireless_power);
////            //Save_Gateway_Param();
////            //Lora_Back_To_Normal();
////        }
//        break;

//    default:
//        break;
//    }
//}

//void Menu_Term_Fun(unsigned char index, menu_t **cu_menu, void *param)
//{
//    switch (index)
//    {
//    case 0:
//        *cu_menu = menu_Term.prev_menu;
//        break;

//    case 1:
////        if (u8g2_UserInterfaceInputValue(&u8g2, "\nset feed speed", "feed speed = ", (uint8_t *)&term_sys_param.Feed_Speed, 0, 255, 3, ""))
////        {
////            //Set_Term_Sys_Param();
////        }
//        break;

//    case 2:
////        if (u8g2_UserInterfaceInputValue(&u8g2, "\nset tail value", "tail value = ", (uint8_t *)&term_sys_param.Tail_Value, 0, 255, 3, ""))
////        {
////            //Set_Term_Sys_Param();
////        }
//        break;

//    case 3:
////        if (u8g2_UserInterfaceInputValue(&u8g2, "\nset flow pulse", "flow pulse = ", (uint8_t *)&term_sys_param.Flow_Unit_Pulse, 0, 255, 3, ""))
////        {
////            //Set_Term_Sys_Param();
////        }
//        break;
//    }
//}

//void Menu_Control_Fun(unsigned char index, menu_t **cu_menu, void *param)
//{
//    switch (index)
//    {
//    case 0:
//        *cu_menu = menu_Control.prev_menu;
//        break;

//    /* feed */
//    case 1:
//        *cu_menu = &menu_Control_Feed;
//        break;
//    }
//}

//void Menu_Setting_Fun(unsigned char index, menu_t **cu_menu, void *param)
//{
//    switch (index)
//    {
//    case 0:
//        *cu_menu = menu_Setting.prev_menu;
//        break;

//    /* feed */
//    case 1:
//        if (u8g2_UserInterfaceMessage(&u8g2, "", "", "reset env!!!", "Yes\nCancel") == 1)
//            ef_env_set_default();
//        break;
//    }
//}
