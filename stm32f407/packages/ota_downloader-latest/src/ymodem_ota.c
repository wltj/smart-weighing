/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-01-30     armink       the first version
 * 2018-08-27     Murphy       update log
 */

#include <rtthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <finsh.h>
#include <fal.h>
#include <ymodem.h>


#define DBG_ENABLE
#define DBG_SECTION_NAME               "ymodem"
#ifdef OTA_DOWNLOADER_DEBUG
#define DBG_LEVEL                      DBG_LOG
#else
#define DBG_LEVEL                      DBG_INFO
#endif
#define DBG_COLOR
#include <rtdbg.h>

#ifdef PKG_USING_YMODEM_OTA
#define DEFAULT_DOWNLOAD_PART "download"
#define DEFAULT_FONT_PART   "font"
#define DEFAULT_TERM_UPDATA_PART   "term_up"


static char* recv_partition = DEFAULT_DOWNLOAD_PART;
static size_t update_file_total_size, update_file_cur_size;
static const struct fal_partition * dl_part = RT_NULL;

static enum rym_code ymodem_on_begin(struct rym_ctx *ctx, rt_uint8_t *buf, rt_size_t len)
{
    char *file_name, *file_size;

    /* calculate and store file size */
    file_name = (char *)&buf[0];
    file_size = (char *)&buf[rt_strlen(file_name) + 1];
    update_file_total_size = atol(file_size);
    rt_kprintf("Ymodem file_size:%d\n", update_file_total_size);

    update_file_cur_size = 0;

    /* Get download partition information and erase download partition data */
    if ((dl_part = fal_partition_find(recv_partition)) == RT_NULL)
    {
        LOG_E("Firmware download failed! Partition (%s) find error!", recv_partition);
        return RYM_CODE_CAN;
    }

    if (update_file_total_size > dl_part->len)
    {
        LOG_E("Firmware is too large! File size (%d), '%s' partition size (%d)", update_file_total_size, recv_partition, dl_part->len);
        return RYM_CODE_CAN;
    }

    LOG_I("Start erase. Size (%d)", update_file_total_size);

    /* erase DL section */
    if (fal_partition_erase(dl_part, 0, update_file_total_size) < 0)
    {
        LOG_E("Firmware download failed! Partition (%s) erase error!", dl_part->name);
        return RYM_CODE_CAN;
    }

    return RYM_CODE_ACK;
}

static enum rym_code ymodem_on_data(struct rym_ctx *ctx, rt_uint8_t *buf, rt_size_t len)
{
    /* write data of application to DL partition  */
    if (fal_partition_write(dl_part, update_file_cur_size, buf, len) < 0)
    {
        LOG_E("Firmware download failed! Partition (%s) write data error!", dl_part->name);
        return RYM_CODE_CAN;
    }

    update_file_cur_size += len;

    return RYM_CODE_ACK;
}

void ymodem_ota(uint8_t argc, char **argv)
{
    struct rym_ctx rctx;

    if (argc < 2)
    {
        recv_partition = DEFAULT_DOWNLOAD_PART;
        rt_kprintf("Default save firmware on download partition.\n");
    }
    else
    {
        const char *operator = argv[1];
        if (!strcmp(operator, "-p")) {
            if (argc < 3) {
                rt_kprintf("Usage: ymodem_ota -p <partiton name>.\n");
                return;
            } else {
                /* change default partition to save firmware */
                recv_partition = argv[2];  
            }
        }else{
            rt_kprintf("Usage: ymodem_ota -p <partiton name>.\n");
            return;
        }
    }

    rt_kprintf("Warning: Ymodem has started! This operator will not recovery.\n");
    rt_kprintf("Please select the ota firmware file and use Ymodem to send.\n");

    if (!rym_recv_on_device(&rctx, rt_console_get_device(), RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX,
                            ymodem_on_begin, ymodem_on_data, NULL, RT_TICK_PER_SECOND))
    {
        rt_kprintf("Download firmware to flash success.\n");
        rt_kprintf("System now will restart...\r\n");

        /* wait some time for terminal response finish */
        rt_thread_delay(rt_tick_from_millisecond(200));

        /* Reset the device, Start new firmware */
        extern void rt_hw_cpu_reset(void);
        rt_hw_cpu_reset();
        /* wait some time for terminal response finish */
        rt_thread_delay(rt_tick_from_millisecond(200));
    }
    else
    {
        /* wait some time for terminal response finish */
        rt_thread_delay(RT_TICK_PER_SECOND);
        rt_kprintf("Update firmware fail.\n");
    }

    return;
}

/*
    加载字库
*/
void load_font(uint8_t argc, char **argv)
{
    struct rym_ctx rctx;

    if (argc < 2)
    {
        recv_partition = DEFAULT_FONT_PART;
        rt_kprintf("Default save firmware on download partition.\n");
    }
    else
    {
        const char *operator = argv[1];
        if (!strcmp(operator, "-p")) {
            if (argc < 3) {
                rt_kprintf("Usage: ymodem_ota -p <partiton name>.\n");
                return;
            } else {
                /* change default partition to save firmware */
                recv_partition = argv[2];  
            }
        }else{
            rt_kprintf("Usage: ymodem_ota -p <partiton name>.\n");
            return;
        }
    }

    rt_kprintf("Warning: Ymodem has started! This operator will not recovery.\n");
    rt_kprintf("Please select the font file and use Ymodem to send.\n");

    if (!rym_recv_on_device(&rctx, rt_console_get_device(), RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX,
                            ymodem_on_begin, ymodem_on_data, NULL, RT_TICK_PER_SECOND))
    {
        rt_kprintf("Download font to flash success.\n");
    }
    else
    {
        /* wait some time for terminal response finish */
        rt_thread_delay(RT_TICK_PER_SECOND);
        rt_kprintf("Download font fail.\n");
    }

    return;
}

/*
    加载终端升级文件
*/
void load_term_updata_file(uint8_t argc, char **argv)
{
    struct rym_ctx rctx;


//    term_updata_param_t term_updata_param;
//    /* 测试升级，需要手动修改对应的 */
//    term_updata_param.app_version = 11;
//    term_updata_param.crc32 = 2603890173;
//    term_updata_param.size = 20060;
//    /* 测试升级，需要手动修改对应的 */
//    
//    term_updata_param.total_pack = term_updata_param.size/UPDATA_PACK_LENTH;
//    if(term_updata_param.size % UPDATA_PACK_LENTH != 0)
//    {
//        term_updata_param.total_pack++;
//    }
    
    recv_partition = DEFAULT_TERM_UPDATA_PART;
       
    rt_kprintf("Warning: Ymodem has started! This operator will not recovery.\n");
    rt_kprintf("Please select the updata file and use Ymodem to send.\n");

    if (!rym_recv_on_device(&rctx, rt_console_get_device(), RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX,
                            ymodem_on_begin, ymodem_on_data, NULL, RT_TICK_PER_SECOND))
    {
//        if(Save_Term_Upgread_Param(term_updata_param) == RT_EOK)
//        {
//            unsigned char upgread_flag;
//            upgread_flag = 1;
//            rt_kprintf("[upgread term]get apgread file success,start upgread term.\n");
//        }
//        else
//        {
//            rt_kprintf("[upgread term]save term upgread param error.\n");
//        }
        
    }
    else
    {
        /* wait some time for terminal response finish */
        rt_thread_delay(RT_TICK_PER_SECOND);
        rt_kprintf("Download updata file fail.\n");
    }

    return;
}

/**
 * msh />ymodem_ota
*/
MSH_CMD_EXPORT(load_term_updata_file, Use Y-MODEM to download the term updata firmware);
MSH_CMD_EXPORT(ymodem_ota, Use Y-MODEM to download the firmware);
MSH_CMD_EXPORT(load_font, Use Y-MODEM to download the font);
#endif /* PKG_USING_YMODEM_OTA */
