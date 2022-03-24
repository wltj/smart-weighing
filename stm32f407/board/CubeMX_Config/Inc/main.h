/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <rtthread.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
//void Error_Handler(void);

/* USER CODE BEGIN EFP */
#define APP_VERSION "2.0.0"
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */
#define KEY_6_Pin GPIO_PIN_2
#define KEY_6_GPIO_Port GPIOE
#define KEY_5_Pin GPIO_PIN_3
#define KEY_5_GPIO_Port GPIOE
#define KEY_4_Pin GPIO_PIN_4
#define KEY_4_GPIO_Port GPIOE
#define KEY_3_Pin GPIO_PIN_5
#define KEY_3_GPIO_Port GPIOE
#define KEY_2_Pin GPIO_PIN_6
#define KEY_2_GPIO_Port GPIOE
#define KEY_1_Pin GPIO_PIN_13
#define KEY_1_GPIO_Port GPIOC
#define DC2_IN_EXT_Pin GPIO_PIN_3
#define DC2_IN_EXT_GPIO_Port GPIOA
#define DC3_IN_EXT_Pin GPIO_PIN_4
#define DC3_IN_EXT_GPIO_Port GPIOA
#define DC4_IN_EXT_Pin GPIO_PIN_5
#define DC4_IN_EXT_GPIO_Port GPIOA
#define DC5_IN_EXT_Pin GPIO_PIN_6
#define DC5_IN_EXT_GPIO_Port GPIOA
#define DC6_IN_EXT_Pin GPIO_PIN_0
#define DC6_IN_EXT_GPIO_Port GPIOB
#define CTR_485_1_Pin GPIO_PIN_1
#define CTR_485_1_GPIO_Port GPIOB
#define IN_1_Pin GPIO_PIN_2
#define IN_1_GPIO_Port GPIOB
#define IN_2_Pin GPIO_PIN_7
#define IN_2_GPIO_Port GPIOE
#define CTR_485_2_Pin GPIO_PIN_8
#define CTR_485_2_GPIO_Port GPIOE
#define IN_3_Pin GPIO_PIN_9
#define IN_3_GPIO_Port GPIOE
#define CTR_485_3_Pin GPIO_PIN_10
#define CTR_485_3_GPIO_Port GPIOE
#define IN_4_Pin GPIO_PIN_11
#define IN_4_GPIO_Port GPIOE
#define CTR_485_4_Pin GPIO_PIN_12
#define CTR_485_4_GPIO_Port GPIOE
#define LORA_CS_Pin GPIO_PIN_13
#define LORA_CS_GPIO_Port GPIOE
#define LORA_RST_Pin GPIO_PIN_14
#define LORA_RST_GPIO_Port GPIOE
#define LORA_INT_Pin GPIO_PIN_15
#define LORA_INT_GPIO_Port GPIOE
#define RS485_3_4_TX_Pin GPIO_PIN_8
#define RS485_3_4_TX_GPIO_Port GPIOD
#define RS485_3_4_RX_Pin GPIO_PIN_9
#define RS485_3_4_RX_GPIO_Port GPIOD
#define ETH_RST_Pin GPIO_PIN_10
#define ETH_RST_GPIO_Port GPIOD
#define LCD_CS_Pin GPIO_PIN_11
#define LCD_CS_GPIO_Port GPIOD
#define LCD_RST_Pin GPIO_PIN_12
#define LCD_RST_GPIO_Port GPIOD
#define LCD_RS_Pin GPIO_PIN_13
#define LCD_RS_GPIO_Port GPIOD
#define LCD_SCK_Pin GPIO_PIN_14
#define LCD_SCK_GPIO_Port GPIOD
#define RS485_1_2_TX_Pin GPIO_PIN_6
#define RS485_1_2_TX_GPIO_Port GPIOC
#define RS485_1_2_RX_Pin GPIO_PIN_7
#define RS485_1_2_RX_GPIO_Port GPIOC
#define LCD_DIO_Pin GPIO_PIN_8
#define LCD_DIO_GPIO_Port GPIOC
#define WIFI_TX_Pin GPIO_PIN_9
#define WIFI_TX_GPIO_Port GPIOA
#define WIFI_RX_Pin GPIO_PIN_10
#define WIFI_RX_GPIO_Port GPIOA
#define WIFI_RST_Pin GPIO_PIN_11
#define WIFI_RST_GPIO_Port GPIOA
#define LCD_BCK_LED_Pin GPIO_PIN_15
#define LCD_BCK_LED_GPIO_Port GPIOA
#define BT_TX_Pin GPIO_PIN_10
#define BT_TX_GPIO_Port GPIOC
#define BT_RX_Pin GPIO_PIN_11
#define BT_RX_GPIO_Port GPIOC
#define RS232_TX_Pin GPIO_PIN_12
#define RS232_TX_GPIO_Port GPIOC
#define RS232_RX_Pin GPIO_PIN_2
#define RS232_RX_GPIO_Port GPIOD
#define SPI1_CS_Pin GPIO_PIN_6
#define SPI1_CS_GPIO_Port GPIOB
#define RELAY_1_Pin GPIO_PIN_7
#define RELAY_1_GPIO_Port GPIOB
#define RELAY_2_Pin GPIO_PIN_8
#define RELAY_2_GPIO_Port GPIOB
#define RELAY_3_Pin GPIO_PIN_9
#define RELAY_3_GPIO_Port GPIOB
#define RELAY_4_Pin GPIO_PIN_0
#define RELAY_4_GPIO_Port GPIOE
#define RELAY_5_Pin GPIO_PIN_1
#define RELAY_5_GPIO_Port GPIOE
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
