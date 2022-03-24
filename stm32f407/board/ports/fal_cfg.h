/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-5      SummerGift   first version
 */

#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#include <rtthread.h>
#include <board.h>

#define RT_APP_PART_ADDR        0x08020000

#define MAX_ON_FLASH    8*128*1024

#define FLASH_SIZE_GRANULARITY_16K   (4 * 16 * 1024)
#define FLASH_SIZE_GRANULARITY_64K   (64 * 1024)
#define FLASH_SIZE_GRANULARITY_128K  (MAX_ON_FLASH - FLASH_SIZE_GRANULARITY_16K - FLASH_SIZE_GRANULARITY_64K)

#define STM32_FLASH_START_ADRESS_16K  STM32_FLASH_START_ADRESS
#define STM32_FLASH_START_ADRESS_64K  (STM32_FLASH_START_ADRESS_16K + FLASH_SIZE_GRANULARITY_16K)
#define STM32_FLASH_START_ADRESS_128K (STM32_FLASH_START_ADRESS_64K + FLASH_SIZE_GRANULARITY_64K)

extern const struct fal_flash_dev stm32_onchip_flash_16k;
extern const struct fal_flash_dev stm32_onchip_flash_64k;
extern const struct fal_flash_dev stm32_onchip_flash_128k;


/* ===================== Flash device Configuration ========================= */
extern const struct fal_flash_dev stm32_onchip_flash_bl;
extern const struct fal_flash_dev stm32_onchip_flash_app;
extern struct fal_flash_dev nor_flash0;

/* flash device table */
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &nor_flash0,                                                     \
    &stm32_onchip_flash_128k,                                        \
}
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG

/* partition table */
/*{FAL_PART_MAGIC_WORD,        "bl",     "stm32_onchip_bl",   STM32_FLASH_START_ADRESS_BL,   FLASH_SIZE_GRANULARITY_BL, 0}, \*/
#define FAL_PART_TABLE                                                               \
{                                                                                    \
    {FAL_PART_MAGIC_WROD, "app",        "onchip_flash_128k", 0 , FLASH_SIZE_GRANULARITY_128K, 0}, \
    {FAL_PART_MAGIC_WORD, "easyflash", FAL_USING_NOR_FLASH_DEV_NAME,         0, 4*1024*1024, 0}, \
    {FAL_PART_MAGIC_WORD,  "download", FAL_USING_NOR_FLASH_DEV_NAME, 4*1024*1024, 1024*1024, 0}, \
    {FAL_PART_MAGIC_WORD,  "factory", FAL_USING_NOR_FLASH_DEV_NAME,  5*1024*1024, 1024*1024, 0}, \
    {FAL_PART_MAGIC_WORD,  "term_up", FAL_USING_NOR_FLASH_DEV_NAME,     6*1024*1024, 1024*1024, 0},\
}

#endif /* FAL_PART_HAS_TABLE_CFG */
#endif /* _FAL_CFG_H_ */
