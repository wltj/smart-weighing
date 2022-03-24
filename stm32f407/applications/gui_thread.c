#include "gui_thread.h"
#include <stdio.h>
#include <string.h>
#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>
#include "menu.h"
#include <fal.h>

ALIGN(RT_ALIGN_SIZE)
static char gui_Run_stack[1024];
static struct rt_thread gui_run;
extern void Lcd_12864_Init(u8g2_t * u8g2);

u8g2_t u8g2;

#define OLED_PIN_SCL                    GET_PIN(D,14)       // PD14
#define OLED_PIN_SDA                    GET_PIN(C,8)        // PC8
#define OLED_PIN_RST                    GET_PIN(D,12)       // pd12
#define OLED_PIN_CS                     GET_PIN(D,11)       // PD11
#define OLED_PIN_DC                     GET_PIN(D,13)       // pd13

#define KEY_SELECT                      GET_PIN(E,4)
#define EEY_HOME                        GET_PIN(E,2)
#define KEY_NEXT                        GET_PIN(E,5)
#define KEY_PREV                        GET_PIN(E,6)
#define KEY_UP                          GET_PIN(C,13)
#define KEY_DOWN                        GET_PIN(E,3)

#define EN_FONTS    u8g2_font_t0_11_t_all

#define TEST "hello"
unsigned char * FONTS;    
unsigned char CN_FONTS[22];
static const struct fal_partition * dl_part = RT_NULL;
void Lcd_12864_Init(u8g2_t * u8g2)
{
    // Initialization
    u8g2_Setup_st7567_jlx12864_f( u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8x8_rt_gpio_and_delay);
    
    u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_SPI_CLOCK, OLED_PIN_SCL);
    u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_SPI_DATA, OLED_PIN_SDA); 
    u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_RESET, OLED_PIN_RST); 
    u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_CS, OLED_PIN_CS);
    u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_DC, OLED_PIN_DC);
    
    u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_MENU_SELECT, KEY_SELECT);
    u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_MENU_NEXT, KEY_NEXT); 
    u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_MENU_PREV, KEY_PREV); 
    u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_MENU_HOME, EEY_HOME);
    u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_MENU_UP, KEY_UP);
    u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_MENU_DOWN, KEY_DOWN);
    
    u8g2_InitDisplay(u8g2);
    u8g2_SetPowerSave(u8g2, 0);
    FONTS = (unsigned char *)EN_FONTS;
    
    dl_part = fal_partition_find("font");
    if(dl_part == RT_NULL)
    {
        FONTS = (unsigned char *)EN_FONTS;
    }
    else
    {
        fal_partition_read(dl_part,0,CN_FONTS,sizeof(CN_FONTS));
        FONTS = (unsigned char *)EN_FONTS;//CN_FONTS;
    }
    
    // Draw Graphics
    /* full buffer example, setup procedure ends in _f */
    u8g2_ClearBuffer(u8g2);
    u8g2_SetFont(u8g2, FONTS);
    u8g2_DrawStr(u8g2, 36, 36, "sys init...");
    u8g2_SendBuffer(u8g2);
}

void Show_Rssi(unsigned char rssi,unsigned short addr,unsigned char bcrssi)
{
    char rssi_str[30];
    memset(rssi_str,0,sizeof(rssi_str));
    sprintf(rssi_str,"num=000,rssi=000,000");
    //sprintf(rssi_str,"rssi = %3d,%3d,",rssi,bcrssi);
	
	rssi_str[4] = (addr%1000/100) + '0';
	rssi_str[5] = (addr%100/10) + '0';
	rssi_str[6] = (addr%10) + '0';
	
	rssi_str[13] = (rssi%1000/100) + '0';
	rssi_str[14] = (rssi%100/10) + '0';
	rssi_str[15] = (rssi%10) + '0';
	
	rssi_str[17] = (bcrssi%1000/100) + '0';
	rssi_str[18] = (bcrssi%100/10) + '0';
	rssi_str[19] = (bcrssi%10) + '0';
		
    u8g2_DrawStr(&u8g2, 1, 18, rssi_str);
    u8g2_SendBuffer(&u8g2);
}

/*
    u8g2_UserInterfaceInputValue  输出值
    u8g2_UserInterfaceMessage     多行描述
    
*/
menu_t* current_menu;
static void Gui_Run_Entry(void *parameter)
{
    unsigned char index = 0;
    current_menu = &menu_main;
    u8g2_SetFont(&u8g2,FONTS);
    while (1)
    {
        index = u8g2_UserInterfaceSelectionList(&u8g2,current_menu->title,current_menu->selected,current_menu->menuItems);
        
        /* 主页或者取消按键返回0,选项返回选项数字,对应的功能函数 */
        if(current_menu->func != RT_NULL)
            current_menu->func(index,&current_menu,RT_NULL);
        
				rt_thread_mdelay(100);
    }
}


int Gui_Run_Init(void)
{

    rt_err_t result_run;
    Lcd_12864_Init(&u8g2);
    Menu_Init();
    result_run = rt_thread_init(&gui_run,
                                "gui_run",
                                Gui_Run_Entry,
                                RT_NULL,
                                &gui_Run_stack[0], sizeof(gui_Run_stack), 25, 5);
    if (result_run == RT_EOK)
    {
        rt_thread_startup(&gui_run);
    }
    else
    {
        rt_kprintf("gui_run Init Faild.\n");
    }
    return result_run;
}

