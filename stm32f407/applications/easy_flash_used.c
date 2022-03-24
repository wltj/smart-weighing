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
    �����洢����EasyFlask/ef_fal_port.c�ж��塣��Ϊ����������
    
    ʹ��easy_flash ��Ҫ��ǰ��ʼ��fal��easy_flash.
    
    fal_init();
    
    easyflash_init();
    
    api ��������������ַ
    https://www.bookstack.cn/read/EasyFlash-v4.0/docs-zh-api.md
 */
 
 /*easy_flash ʹ������*/
 void easy_flash_exp(void)
 {
    unsigned int _fw = 0;
     
    /*��ʼ��fal*/
    fal_init();
     
     /*��ʼ��easy_flash*/
    if(easyflash_init() == EF_NO_ERR)
    {
        /*��ȡfwֵ*/
        ef_get_env_blob("fw",&_fw,sizeof(_fw),NULL);
    }
    
    _fw++;
    /*����fwֵ*/
    ef_set_env_blob("fw",&_fw,sizeof(_fw));
 } 
 