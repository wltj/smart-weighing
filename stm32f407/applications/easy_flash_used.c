/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-03-05     Jason        first version
 * 
 */
 #include <rtthread.h>
 #include <rtdevice.h>
 #include <board.h>
 #include "fal.h"
 #include "easyflash.h"
 
 /*
    参数存储表在EasyFlask/ef_fal_port.c中定义。称为环境变量。
    
    使用easy_flash 需要提前初始化fal和easy_flash.
    
    fal_init();
    
    easyflash_init();
    
    api 函数参照以下网址
    https://www.bookstack.cn/read/EasyFlash-v4.0/docs-zh-api.md
 */
 
 /*easy_flash 使用例子*/
 void easy_flash_exp(void)
 {
    unsigned int _fw = 0;
     
    /*初始化fal*/
    fal_init();
     
     /*初始化easy_flash*/
    if(easyflash_init() == EF_NO_ERR)
    {
        /*获取fw值*/
        ef_get_env_blob("fw",&_fw,sizeof(_fw),NULL);
    }
    
    _fw++;
    /*保存fw值*/
    ef_set_env_blob("fw",&_fw,sizeof(_fw));
 } 
 