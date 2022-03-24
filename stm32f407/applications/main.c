/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 * 2018-11-19     flybreak     add stm32f407-atk-explorer bsp
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <string.h>
#include "com_to_server_thread.h"
#include "gateway.h"
#include "fal.h"
#include "easyflash.h"
#include "main.h"
#include "fal_cfg.h"
#include "gui_thread.h"
#include "rs485.h"
#include "led_com.h"
#include "rq_code_read.h"
#include "bt.h"
#include "main_task.h"
#include "hci.h"
#include "pole.h"
#include "printer.h"
#include "weigh.h"
#include "bt_thread.h"
#include "weigh_thread.h"
#include "uart_thread.h"
#include "infrared.h"
#include "system_params.h"
#include "mqtt_thread.h"
/* defined the LED0 pin: PC0 */
#define LED0_PIN GET_PIN(C, 0)
#define LED2_PIN GET_PIN(C, 3)
//#define LED_LCD_PIN GET_PIN(A, 15)//
#define LED_LCD_PIN GET_PIN(C, 2)

#define PARAMS_VERSION				23//参数版本
int main(void)
{

    /* initial easyflash */
    /* initial gui */
    rt_pin_mode(LED2_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_LCD_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(LED_LCD_PIN, PIN_HIGH);
    rt_pin_write(LED0_PIN, PIN_HIGH);
    rt_pin_write(LED2_PIN, PIN_HIGH);
	  fal_init();									//fal
	//	Gui_Run_Init();							//GUI
    easyflash_init();						//easyflash
	  rt_memset((unsigned char *)&systemStatus,0,sizeof(SystemStatus_t));  
    initMainTask();						 //主任务
    rt_kprintf("//////////////////////PARAMS_VERSION: %d\n",PARAMS_VERSION);
    uartThreadInit();//            //串口接收线程
    Init_Infrared();                   //红外
    hci_thread_start();            //LED&声音播报线程
		netdev_to_get_ip(systemStatus.my_url,64);
		initSystemParams(systemStatus.my_url);//系统参数
    mqtt_to_server_start();		 //MQTT

    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
		
    rt_kprintf(" The current version of APP firmware is %s\n", APP_VERSION);
		
		
    while (1)
    {
        if((rt_tick_get()%1000)<500)
        {
            rt_pin_write(LED0_PIN, PIN_HIGH);
        }
        else
        {
            rt_pin_write(LED0_PIN, PIN_LOW);
        }
        mainTaskRun();
        rt_thread_mdelay(200);
    }

    return RT_EOK;
}

static void show_time(int argc, char **argv)
{
    struct rt_device* sys_rtc;
    rt_uint32_t sys_times = 0;
    sys_rtc = (struct rt_device *)rt_device_find("rtc");
    if(sys_rtc == RT_NULL)
    {
        rt_kprintf("[rtc] rtc driver error!\n");
        return;
    }
    sys_rtc->control(sys_rtc,RT_DEVICE_CTRL_RTC_GET_TIME,&sys_times);
    char str[80];
    strftime(str,sizeof(str),"Date:%Y-%m-%d  Time:%H:%M:%S\n",localtime(&sys_times));
    rt_kprintf("[rtc] %s",str);
}
MSH_CMD_EXPORT(show_time, show sys time);
/**
* Function    ota_app_vtor_reconfig
* Description Set Vector Table base location to the start addr of app(RT_APP_PART_ADDR).
*/
static int ota_app_vtor_reconfig(void)
{
#define NVIC_VTOR_MASK 0x3FFFFF80
    /* Set the Vector Table base location by user application firmware definition */
    SCB->VTOR = RT_APP_PART_ADDR & NVIC_VTOR_MASK;

    return 0;
}
INIT_BOARD_EXPORT(ota_app_vtor_reconfig);
