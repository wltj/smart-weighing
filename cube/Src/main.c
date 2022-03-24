/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2020 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ETH_HandleTypeDef heth;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_UART4_Init(void);
static void MX_UART5_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_ETH_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
#include <math.h>

char const ascii_table_5x8[95][5]={
/*全体 ASCII 列表:5x8 点阵*/
0x00,0x00,0x00,0x00,0x00,//- - //space
0x00,0x00,0x4f,0x00,0x00,//-!-
0x00,0x07,0x00,0x07,0x00,//-"-
0x14,0x7f,0x14,0x7f,0x14,//-#-
0x24,0x2a,0x7f,0x2a,0x12,//-$-
0x23,0x13,0x08,0x64,0x62,//-%-
0x36,0x49,0x55,0x22,0x50,//-&-
0x00,0x05,0x07,0x00,0x00,//-'-
0x00,0x1c,0x22,0x41,0x00,//-(-
0x00,0x41,0x22,0x1c,0x00,//-)-
0x14,0x08,0x3e,0x08,0x14,//-*-
0x08,0x08,0x3e,0x08,0x08,//-+-
0x00,0x50,0x30,0x00,0x00,//-,-
0x08,0x08,0x08,0x08,0x08,//---
0x00,0x60,0x60,0x00,0x00,//-.-
0x20,0x10,0x08,0x04,0x02,//-/-
0x3e,0x51,0x49,0x45,0x3e,//-0-
0x00,0x42,0x7f,0x40,0x00,//-1-
0x42,0x61,0x51,0x49,0x46,//-2-
0x21,0x41,0x45,0x4b,0x31,//-3-
0x18,0x14,0x12,0x7f,0x10,//-4-
0x27,0x45,0x45,0x45,0x39,//-5-
0x3c,0x4a,0x49,0x49,0x30,//-6-
0x01,0x71,0x09,0x05,0x03,//-7-
0x36,0x49,0x49,0x49,0x36,//-8-
0x06,0x49,0x49,0x29,0x1e,//-9-
0x00,0x36,0x36,0x00,0x00,//-:-
0x00,0x56,0x36,0x00,0x00,//-;-
0x08,0x14,0x22,0x41,0x00,//-<-
0x14,0x14,0x14,0x14,0x14,//-=-
0x00,0x41,0x22,0x14,0x08,//->-
0x02,0x01,0x51,0x09,0x06,//-?-
0x32,0x49,0x79,0x41,0x3e,//-@-
0x7e,0x11,0x11,0x11,0x7e,//-A-
0x7f,0x49,0x49,0x49,0x36,//-B-
0x3e,0x41,0x41,0x41,0x22,//-C-
0x7f,0x41,0x41,0x22,0x1c,//-D-
0x7f,0x49,0x49,0x49,0x41,//-E-
0x7f,0x09,0x09,0x09,0x01,//-F-
0x3e,0x41,0x49,0x49,0x7a,//-G-
0x7f,0x08,0x08,0x08,0x7f,//-H-
0x00,0x41,0x7f,0x41,0x00,//-I-
0x20,0x40,0x41,0x3f,0x01,//-J-
0x7f,0x08,0x14,0x22,0x41,//-K-
0x7f,0x40,0x40,0x40,0x40,//-L-
0x7f,0x02,0x0c,0x02,0x7f,//-M-
0x7f,0x04,0x08,0x10,0x7f,//-N-
0x3e,0x41,0x41,0x41,0x3e,//-O-
0x7f,0x09,0x09,0x09,0x06,//-P-
0x3e,0x41,0x51,0x21,0x5e,//-Q-
0x7f,0x09,0x19,0x29,0x46,//-R-
0x46,0x49,0x49,0x49,0x31,//-S-
0x01,0x01,0x7f,0x01,0x01,//-T-
0x3f,0x40,0x40,0x40,0x3f,//-U-
0x1f,0x20,0x40,0x20,0x1f,//-V-
0x3f,0x40,0x38,0x40,0x3f,//-W-
0x63,0x14,0x08,0x14,0x63,//-X-
0x07,0x08,0x70,0x08,0x07,//-Y-
0x61,0x51,0x49,0x45,0x43,//-Z-
0x00,0x7f,0x41,0x41,0x00,//-[-
0x02,0x04,0x08,0x10,0x20,//-\-
0x00,0x41,0x41,0x7f,0x00,//-]-
0x04,0x02,0x01,0x02,0x04,//-^-
0x40,0x40,0x40,0x40,0x40,//-_-
0x01,0x02,0x04,0x00,0x00,//-`-
0x20,0x54,0x54,0x54,0x78,//-a-
0x7f,0x48,0x48,0x48,0x30,//-b-
0x38,0x44,0x44,0x44,0x44,//-c-
0x30,0x48,0x48,0x48,0x7f,//-d-
0x38,0x54,0x54,0x54,0x58,//-e-
0x00,0x08,0x7e,0x09,0x02,//-f-
0x48,0x54,0x54,0x54,0x3c,//-g-
0x7f,0x08,0x08,0x08,0x70,//-h-
0x00,0x00,0x7a,0x00,0x00,//-i-
0x20,0x40,0x40,0x3d,0x00,//-j-
0x7f,0x20,0x28,0x44,0x00,//-k-
0x00,0x41,0x7f,0x40,0x00,//-l-
0x7c,0x04,0x38,0x04,0x7c,//-m-
0x7c,0x08,0x04,0x04,0x78,//-n-
0x38,0x44,0x44,0x44,0x38,//-o-
0x7c,0x14,0x14,0x14,0x08,//-p-
0x08,0x14,0x14,0x14,0x7c,//-q-
0x7c,0x08,0x04,0x04,0x08,//-r-
0x48,0x54,0x54,0x54,0x24,//-s-
0x04,0x04,0x3f,0x44,0x24,//-t-
0x3c,0x40,0x40,0x40,0x3c,//-u-
0x1c,0x20,0x40,0x20,0x1c,//-v-
0x3c,0x40,0x30,0x40,0x3c,//-w-
0x44,0x28,0x10,0x28,0x44,//-x-
0x04,0x48,0x30,0x08,0x04,//-y-
0x44,0x64,0x54,0x4c,0x44,//-z-
0x08,0x36,0x41,0x41,0x00,//-{-
0x00,0x00,0x77,0x00,0x00,//-|-
0x00,0x41,0x41,0x36,0x08,//-}-
0x04,0x02,0x02,0x02,0x01,//-~-
};

//纵向取模，适合 ST7565P,ST7565R,ST7567,UC1701X,KS0108 等驱动 IC 的液晶模块使用
char const ascii_table_8x16[95][16]={
//粗体 8x16 点阵的 ASCII 码的点阵数据，从"JLX-GB2312"型号的字库 IC 中读出来的国标的。
0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, //- -(即“空格”）ASCII 码：0X20
0x00,0x00,0x38,0xFC, 0xFC,0x38,0x00,0x00, 0x00,0x00,0x00,0x0D, 0x0D,0x00,0x00,0x00, //-!- ASCII码：0X21
0x00,0x0E,0x1E,0x00, 0x00,0x1E,0x0E,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, //-"-
0x20,0xF8,0xF8,0x20, 0xF8,0xF8,0x20,0x00, 0x02,0x0F,0x0F,0x02, 0x0F,0x0F,0x02,0x00, //-#-
0x38,0x7C,0x44,0x47, 0x47,0xCC,0x98,0x00, 0x06,0x0C,0x08,0x38, 0x38,0x0F,0x07,0x00, //-$-
0x30,0x30,0x00,0x80, 0xC0,0x60,0x30,0x00, 0x0C,0x06,0x03,0x01, 0x00,0x0C,0x0C,0x00, //-%-
0x80,0xD8,0x7C,0xE4, 0xBC,0xD8,0x40,0x00, 0x07,0x0F,0x08,0x08, 0x07,0x0F,0x08,0x00, //-&-
0x00,0x10,0x1E,0x0E, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, //-'-
0x00,0x00,0xF0,0xF8, 0x0C,0x04,0x00,0x00, 0x00,0x00,0x03,0x07, 0x0C,0x08,0x00,0x00, //-(-
0x00,0x00,0x04,0x0C, 0xF8,0xF0,0x00,0x00, 0x00,0x00,0x08,0x0C, 0x07,0x03,0x00,0x00, //-)-
0x80,0xA0,0xE0,0xC0, 0xC0,0xE0,0xA0,0x80, 0x00,0x02,0x03,0x01, 0x01,0x03,0x02,0x00, //-*- ASCII码：0X2A
0x00,0x80,0x80,0xE0, 0xE0,0x80,0x80,0x00, 0x00,0x00,0x00,0x03, 0x03,0x00,0x00,0x00, //-+-
0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x10,0x1E, 0x0E,0x00,0x00,0x00, //-,-
0x80,0x80,0x80,0x80, 0x80,0x80,0x80,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, //---
0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x0C, 0x0C,0x00,0x00,0x00, //-.-
0x00,0x00,0x00,0x80, 0xC0,0x60,0x30,0x00, 0x0C,0x06,0x03,0x01, 0x00,0x00,0x00,0x00, //-/-
0xF8,0xF8,0x0C,0xC4, 0x0C,0xF8,0xF0,0x00, 0x03,0x07,0x0C,0x08, 0x0C,0x07,0x03,0x00, //-0- ASCII码：0X30
0x00,0x10,0x18,0xFC, 0xFC,0x00,0x00,0x00, 0x00,0x08,0x08,0x0F, 0x0F,0x08,0x08,0x00, //-1-
0x08,0x0C,0x84,0xC4, 0x64,0x3C,0x18,0x00, 0x0E,0x0F,0x09,0x08, 0x08,0x0C,0x0C,0x00, //-2-
0x08,0x0C,0x44,0x44, 0x44,0xFC,0xB8,0x00, 0x04,0x0C,0x08,0x08, 0x08,0x0F,0x07,0x00, //-3-
0xC0,0xE0,0xB0,0x98, 0xFC,0xFC,0x80,0x00, 0x00,0x00,0x00,0x08, 0x0F,0x0F,0x08,0x00, //-4- ASCII码：0X34
0x7C,0x7C,0x44,0x44, 0x44,0xC4,0x84,0x00, 0x04,0x0C,0x08,0x08, 0x08,0x0F,0x07,0x00, //-5-
0xF0,0xF8,0x4C,0x44, 0x44,0xC0,0x80,0x00, 0x07,0x0F,0x08,0x08, 0x08,0x0F,0x07,0x00, //-6-
0x0C,0x0C,0x04,0x84, 0xC4,0x7C,0x3C,0x00, 0x00,0x00,0x0F,0x0F, 0x00,0x00,0x00,0x00, //-7-
0xB8,0xFC,0x44,0x44, 0x44,0xFC,0xB8,0x00, 0x07,0x0F,0x08,0x08, 0x08,0x0F,0x07,0x00, //-8-
0x38,0x7C,0x44,0x44, 0x44,0xFC,0xF8,0x00, 0x00,0x08,0x08,0x08, 0x0C,0x07,0x03,0x00, //-9-
0x00,0x00,0x00,0x30, 0x30,0x00,0x00,0x00, 0x00,0x00,0x00,0x06, 0x06,0x00,0x00,0x00, //-:-
0x00,0x00,0x00,0x30, 0x30,0x00,0x00,0x00, 0x00,0x00,0x08,0x0E, 0x06,0x00,0x00,0x00, //-;-
0x00,0x80,0xC0,0x60, 0x30,0x18,0x08,0x00, 0x00,0x00,0x01,0x03, 0x06,0x0C,0x08,0x00, //-<-
0x00,0x20,0x20,0x20, 0x20,0x20,0x20,0x00, 0x00,0x01,0x01,0x01, 0x01,0x01,0x01,0x00, //-=-
0x00,0x08,0x18,0x30, 0x60,0xC0,0x80,0x00, 0x00,0x08,0x0C,0x06, 0x03,0x01,0x00,0x00, //->- ASCII码：0X3E
0x18,0x1C,0x04,0xC4, 0xE4,0x3C,0x18,0x00, 0x00,0x00,0x00,0x0D, 0x0D,0x00,0x00,0x00, //-?-
0xF0,0xF0,0x08,0xC8, 0xC8,0xF8,0xF0,0x00, 0x07,0x0F,0x08,0x0B, 0x0B,0x0B,0x01,0x00, //-@-
0xE0,0xF0,0x98,0x8C, 0x98,0xF0,0xE0,0x00, 0x0F,0x0F,0x00,0x00, 0x00,0x0F,0x0F,0x00, //-A- ASCII码：0X41
0x04,0xFC,0xFC,0x44, 0x44,0xFC,0xB8,0x00, 0x08,0x0F,0x0F,0x08, 0x08,0x0F,0x07,0x00, //-B-
0xF0,0xF8,0x0C,0x04, 0x04,0x0C,0x18,0x00, 0x03,0x07,0x0C,0x08, 0x08,0x0C,0x06,0x00, //-C-
0x04,0xFC,0xFC,0x04, 0x0C,0xF8,0xF0,0x00, 0x08,0x0F,0x0F,0x08, 0x0C,0x07,0x03,0x00, //-D-
0x04,0xFC,0xFC,0x44, 0xE4,0x0C,0x1C,0x00, 0x08,0x0F,0x0F,0x08, 0x08,0x0C,0x0E,0x00, //-E-
0x04,0xFC,0xFC,0x44, 0xE4,0x0C,0x1C,0x00, 0x08,0x0F,0x0F,0x08, 0x00,0x00,0x00,0x00, //-F-
0xF0,0xF8,0x0C,0x84, 0x84,0x8C,0x98,0x00, 0x03,0x07,0x0C,0x08, 0x08,0x07,0x0F,0x00, //-G-
0xFC,0xFC,0x40,0x40, 0x40,0xFC,0xFC,0x00, 0x0F,0x0F,0x00,0x00, 0x00,0x0F,0x0F,0x00, //-H- ASCII码：0X48
0x00,0x00,0x04,0xFC, 0xFC,0x04,0x00,0x00, 0x00,0x00,0x08,0x0F, 0x0F,0x08,0x00,0x00, //-I-
0x00,0x00,0x00,0x04, 0xFC,0xFC,0x04,0x00, 0x07,0x0F,0x08,0x08, 0x0F,0x07,0x00,0x00, //-J-
0x04,0xFC,0xFC,0xC0, 0xE0,0x3C,0x1C,0x00, 0x08,0x0F,0x0F,0x00, 0x01,0x0F,0x0E,0x00, //-K-
0x04,0xFC,0xFC,0x04, 0x00,0x00,0x00,0x00, 0x08,0x0F,0x0F,0x08, 0x08,0x0C,0x0E,0x00, //-L-
0xFC,0xFC,0x38,0x70, 0x38,0xFC,0xFC,0x00, 0x0F,0x0F,0x00,0x00, 0x00,0x0F,0x0F,0x00, //-M-
0xFC,0xFC,0x38,0x70, 0xE0,0xFC,0xFC,0x00, 0x0F,0x0F,0x00,0x00, 0x00,0x0F,0x0F,0x00, //-N-
0xF8,0xFC,0x04,0x04, 0x04,0xFC,0xF8,0x00, 0x07,0x0F,0x08,0x08, 0x08,0x0F,0x07,0x00, //-O-
0x04,0xFC,0xFC,0x44, 0x44,0x7C,0x38,0x00, 0x08,0x0F,0x0F,0x08, 0x00,0x00,0x00,0x00, //-P-
0xF8,0xFC,0x04,0x04, 0x04,0xFC,0xF8,0x00, 0x07,0x0F,0x08,0x0E, 0x3C,0x3F,0x27,0x00, //-Q-
0x04,0xFC,0xFC,0x44, 0xC4,0xFC,0x38,0x00, 0x08,0x0F,0x0F,0x00, 0x00,0x0F,0x0F,0x00, //-R-
0x18,0x3C,0x64,0x44, 0xC4,0x9C,0x18,0x00, 0x06,0x0E,0x08,0x08, 0x08,0x0F,0x07,0x00, //-S-
0x00,0x1C,0x0C,0xFC, 0xFC,0x0C,0x1C,0x00, 0x00,0x00,0x08,0x0F, 0x0F,0x08,0x00,0x00, //-T-
0xFC,0xFC,0x00,0x00, 0x00,0xFC,0xFC,0x00, 0x07,0x0F,0x08,0x08, 0x08,0x0F,0x07,0x00, //-U-
0xFC,0xFC,0x00,0x00, 0x00,0xFC,0xFC,0x00, 0x01,0x03,0x06,0x0C, 0x06,0x03,0x01,0x00, //-V-
0xFC,0xFC,0x00,0x00, 0x00,0xFC,0xFC,0x00, 0x07,0x0F,0x0E,0x03, 0x0E,0x0F,0x07,0x00, //-W-
0x0C,0x3C,0xF0,0xE0, 0xF0,0x3C,0x0C,0x00, 0x0C,0x0F,0x03,0x01, 0x03,0x0F,0x0C,0x00, //-X-
0x00,0x0C,0x7C,0xC0, 0xC0,0x7C,0x3C,0x00, 0x00,0x00,0x08,0x0F, 0x0F,0x08,0x00,0x00, //-Y-
0x1C,0x0C,0x84,0xC4, 0x64,0x3C,0x1C,0x00, 0x0E,0x0F,0x09,0x08, 0x08,0x0C,0x0E,0x00, //-Z-
0x00,0x00,0xFC,0xFC, 0x04,0x04,0x00,0x00, 0x00,0x00,0x0F,0x0F, 0x08,0x08,0x00,0x00, //-[-
0x38,0x70,0xE0,0xC0, 0x80,0x00,0x00,0x00, 0x00,0x00,0x00,0x01, 0x03,0x07,0x0E,0x00, //-\-
0x00,0x00,0x04,0x04, 0xFC,0xFC,0x00,0x00, 0x00,0x00,0x08,0x08, 0x0F,0x0F,0x00,0x00, //-]-
0x08,0x0C,0x06,0x03, 0x06,0x0C,0x08,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, //-^-
0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x20,0x20,0x20,0x20, 0x20,0x20,0x20,0x20, //-_-
0x00,0x00,0x03,0x07, 0x04,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, //-`-
0x00,0xA0,0xA0,0xA0, 0xE0,0xC0,0x00,0x00, 0x07,0x0F,0x08,0x08, 0x07,0x0F,0x08,0x00, //-a- ASCII码：0X61
0x04,0xFC,0xFC,0x20, 0x60,0xC0,0x80,0x00, 0x00,0x0F,0x0F,0x08, 0x08,0x0F,0x07,0x00, //-b-
0xC0,0xE0,0x20,0x20, 0x20,0x60,0x40,0x00, 0x07,0x0F,0x08,0x08, 0x08,0x0C,0x04,0x00, //-c-
0x80,0xC0,0x60,0x24, 0xFC,0xFC,0x00,0x00, 0x07,0x0F,0x08,0x08, 0x07,0x0F,0x08,0x00, //-d-
0xC0,0xE0,0xA0,0xA0, 0xA0,0xE0,0xC0,0x00, 0x07,0x0F,0x08,0x08, 0x08,0x0C,0x04,0x00, //-e-
0x40,0xF8,0xFC,0x44, 0x0C,0x18,0x00,0x00, 0x08,0x0F,0x0F,0x08, 0x00,0x00,0x00,0x00, //-f-
0xC0,0xE0,0x20,0x20, 0xC0,0xE0,0x20,0x00, 0x27,0x6F,0x48,0x48, 0x7F,0x3F,0x00,0x00, //-g-
0x04,0xFC,0xFC,0x40, 0x20,0xE0,0xC0,0x00, 0x08,0x0F,0x0F,0x00, 0x00,0x0F,0x0F,0x00, //-h-
0x00,0x00,0x20,0xEC, 0xEC,0x00,0x00,0x00, 0x00,0x00,0x08,0x0F, 0x0F,0x08,0x00,0x00, //-i-
0x00,0x00,0x00,0x00, 0x20,0xEC,0xEC,0x00, 0x00,0x30,0x70,0x40, 0x40,0x7F,0x3F,0x00, //-j-
0x04,0xFC,0xFC,0x80, 0xC0,0x60,0x20,0x00, 0x08,0x0F,0x0F,0x01, 0x03,0x0E,0x0C,0x00, //-k-
0x00,0x00,0x04,0xFC, 0xFC,0x00,0x00,0x00, 0x00,0x00,0x08,0x0F, 0x0F,0x08,0x00,0x00, //-l-
0xE0,0xE0,0x60,0xC0, 0x60,0xE0,0xC0,0x00, 0x0F,0x0F,0x00,0x07, 0x00,0x0F,0x0F,0x00, //-m-
0x20,0xE0,0xC0,0x20, 0x20,0xE0,0xC0,0x00, 0x00,0x0F,0x0F,0x00, 0x00,0x0F,0x0F,0x00, //-n-
0xC0,0xE0,0x20,0x20, 0x20,0xE0,0xC0,0x00, 0x07,0x0F,0x08,0x08, 0x08,0x0F,0x07,0x00, //-o-
0x20,0xE0,0xC0,0x20, 0x20,0xE0,0xC0,0x00, 0x40,0x7F,0x7F,0x48, 0x08,0x0F,0x07,0x00, //-p-
0xC0,0xE0,0x20,0x20, 0xC0,0xE0,0x20,0x00, 0x07,0x0F,0x08,0x48, 0x7F,0x7F,0x40,0x00, //-q-
0x20,0xE0,0xC0,0x60, 0x20,0xE0,0xC0,0x00, 0x08,0x0F,0x0F,0x08, 0x00,0x00,0x00,0x00, //-r-
0x40,0xE0,0xA0,0x20, 0x20,0x60,0x40,0x00, 0x04,0x0C,0x09,0x09, 0x0B,0x0E,0x04,0x00, //-s-
0x20,0x20,0xF8,0xFC, 0x20,0x20,0x00,0x00, 0x00,0x00,0x07,0x0F, 0x08,0x0C,0x04,0x00, //-t-
0xE0,0xE0,0x00,0x00, 0xE0,0xE0,0x00,0x00, 0x07,0x0F,0x08,0x08, 0x07,0x0F,0x08,0x00, //-u-
0x00,0xE0,0xE0,0x00, 0x00,0xE0,0xE0,0x00, 0x00,0x03,0x07,0x0C, 0x0C,0x07,0x03,0x00, //-v-
0xE0,0xE0,0x00,0x80, 0x00,0xE0,0xE0,0x00, 0x07,0x0F,0x0C,0x07, 0x0C,0x0F,0x07,0x00, //-w-
0x20,0x60,0xC0,0x80, 0xC0,0x60,0x20,0x00, 0x08,0x0C,0x07,0x03, 0x07,0x0C,0x08,0x00, //-x-
0xE0,0xE0,0x00,0x00, 0x00,0xE0,0xE0,0x00, 0x47,0x4F,0x48,0x48, 0x68,0x3F,0x1F,0x00, //-y-
0x60,0x60,0x20,0xA0, 0xE0,0x60,0x20,0x00, 0x0C,0x0E,0x0B,0x09, 0x08,0x0C,0x0C,0x00, //-z- //
0x00,0x40,0x40,0xF8, 0xBC,0x04,0x04,0x00, 0x00,0x00,0x00,0x07, 0x0F,0x08,0x08,0x00, //-{-
0x00,0x00,0x00,0xBC, 0xBC,0x00,0x00,0x00, 0x00,0x00,0x00,0x0F, 0x0F,0x00,0x00,0x00, //-|-
0x00,0x04,0x04,0xBC, 0xF8,0x40,0x40,0x00, 0x00,0x08,0x08,0x0F, 0x07,0x00,0x00,0x00, //-}-
0x08,0x0C,0x04,0x0C, 0x08,0x0C,0x04,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, //-~- ASCII 码：0X7E
};

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+0x14) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+0x14) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+0x14) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+0x14) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+0x14) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+0x14) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+0x14) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+0x10) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+0x10) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+0x10) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+0x10) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+0x10) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+0x10) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+0x10) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入
#define cs11 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_SET)
#define cs10 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_RESET)

#define reset0 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET)
#define rs0 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET)
#define sclk0 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET)
#define sid0 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET)

#define reset1 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET)
#define rs1 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET)
#define sclk1 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET)
#define sid1 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET)



#define uchar unsigned char
#define uint unsigned int

void Delay_1us(uint us)
{
    for (uint16_t n = 0; n < 320; n++)
    {
        __asm volatile("nop");
    }
}

void Delay_us(uint us)
{
    for (uint16_t n = 0; n < us; n++)
    {
        Delay_1us(1);
    }
}

void Delay_ms(uint ms)
{
    for (uint16_t n = 0; n < ms; n++)
    {
        Delay_1us(1000);
    }
}

void transfer_command(int data1)
{
    char i;
    cs10;
    rs0;
    for(i=0;i<8;i++)
    {
        sclk0;
        if(data1&0x80) sid1;
        else sid0;
        sclk1;
        Delay_us(1);
        data1<<=1;
    }
    cs11;
}

void transfer_data(int data1)
{
    char i;
    cs10;
    rs1;
    for(i=0;i<8;i++)
    {
        sclk0;
        if(data1&0x80) sid1;
        else sid0;
        sclk1;
        Delay_us(1);
        data1<<=1;
    }
    cs11;
}

void lcd_address(unsigned char page,unsigned char column)
{
    column=column+3; //我们平常所说的第 1 列，在 LCD 驱动 IC 里是第 0 列。所以在这里减去1.
    page=page-1;
    transfer_command(0xb0+page); //设置页地址。每页是 8 行。一个画面的 64 行被分成 8 个页。我们平常所说的第 1 页，在 LCD 驱动 IC 里是第 0 页，所以在这里减去 1*/
    transfer_command(((column>>4)&0x0f)+0x10); //设置列地址的高 4 位
    transfer_command(column&0x0f); //设置列地址的低 4 位
}

void clear_screen(void)
{
    unsigned char i,j;
    for(i=0;i<9;i++)
    {
        lcd_address(1+i,1);
        for(j=0;j<132;j++)
        {
            transfer_data(0x00);
        }
    }
}

void test_display(unsigned char data1,unsigned char data2)
{
    int i,j;
    for(j=0;j<8;j++)
    {
        lcd_address(j+1,0);
        for(i=0;i<128;i++)
        {
            transfer_data(data1);
            transfer_data(data2);
        }
    }
}

void display_graphic(uchar *dp)
{
    uchar i,j;
    for(j=0;j<8;j++)
    {
        lcd_address(j+1,1);
        for(i=0;i<128;i++)
        {
            transfer_data(*dp);
            dp++;
        }
    }
}
/*显示 32x32 点阵图像、汉字、生僻字或 32x32 点阵的其他图标*/
void display_graphic_32x32(uchar page,uchar column,uchar *dp)
{
    uchar i,j;
    for(j=0;j<4;j++)
    {
        lcd_address(page+j,column);
        for (i=0;i<31;i++)
        {
            transfer_data(*dp); /*写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1*/
            dp++;
        }
    }
}
/*显示 16x16 点阵图像、汉字、生僻字或 16x16 点阵的其他图标*/
void display_graphic_16x16(uchar page,uchar column,uchar reverse,uchar *dp)
{
    uchar i,j;
    for(j=0;j<2;j++)
    {
        lcd_address(page+j,column);
        for (i=0;i<16;i++)
        {
            if(reverse==1)
            {
                transfer_data(~*dp); /*写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1*/
            }
            else
                transfer_data(*dp);
            dp++;
        }
    }
}
/*显示 8x16 点阵图像、ASCII, 或 8x16 点阵的自造字符、其他图标*/
void display_graphic_8x16(uchar page,uchar column,uchar *dp)
{
    uchar i,j;
    for(j=0;j<2;j++)
    {
        lcd_address(page+j,column);
        for (i=0;i<8;i++)
        {
            transfer_data(*dp); /*写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1*/
            dp++;
        }
    }
}
//显示一串 8x16 点阵的字符串
//括号里的参数分别为（页，列，是否反显，数据指针）
void display_string_8x16(uint page,uint column,uchar reverse,uchar *text)
{
    uint i=0,j,k,n,data1;

    while(text[i]>0x00)
    {
        if((text[i]>=0x20)&&(text[i]<=0x7e))
        {
            j=text[i]-0x20;
            for(n=0;n<2;n++)
            {
                lcd_address(page+n,column);
                for(k=0;k<8;k++)
                {
                    if(reverse==1) data1=~ascii_table_8x16[j][k+8*n];
                    else data1=ascii_table_8x16[j][k+8*n];
                    transfer_data(data1);
                }
            }
            i++;
            column+=8;
        }
        else
        i++;
    }
}
//显示一串 5x8 点阵的字符串
//括号里的参数分别为（页，列，是否反显，数据指针）
void display_string_5x8(uint page,uint column,uchar reverse,uchar *text)
{
    uchar i=0,j,k,data1;
    while(text[i]>0x00)
    {   
        if((text[i]>=0x20)&&(text[i]<=0x7e))
        {
            j=text[i]-0x20;
            lcd_address(page,column);
            for(k=0;k<5;k++)
            {
                if(reverse==1) data1=~ascii_table_5x8[j][k];
                else data1=ascii_table_5x8[j][k];
                transfer_data(data1);
            }
            if(reverse==1) transfer_data(0xff);
            else transfer_data(0x00);
            i++;
            column+=6;
        }
        else
        i++;
    }
}

uchar read_data (void)
{
//    uchar ret_data=0;
//    unsigned char i=0;
//    rs=1;
//    cs1=0;
//    
//    for(;i<8;i++)
//    {
//        sclk=0;
//        sclk=1;
//        Delay_us(1);
//        ret_data |= sidin << i;
//    }
//    cs1=1;
//    return(ret_data);
}

//===开始 “读取-修改-写入” 模式====
void Start_Read_Modify_Write()
{
    transfer_command(0xe0);
}
//===结束 “读取-修改-写入” 模式====
void End_Read_Modify_Write()
{
    transfer_command(0xee);
}

//从液晶屏驱动 IC 中读取数据,含一次空读和一次正式读取
uchar read_data_twice()
{
    uchar ret_data=0;
//    cs1=0;
//    rs=1;
//    //一次空读----------------
//    sclk=0;
//    sclk=1;
//    Delay_us(1);
//    //---一次正式读------------
//    sclk=0;
//    sclk=1;
//    Delay_us(1);
//    cs1=1;
//    //------------------------
//    ret_data=read_data();
    return(ret_data);
}
//画点函数。括号里的参数分别为：坐标(column,row),row 为行,共 64 行，最小值为 1，最大值为 64; column 为列,共 128 列,最小值为 1，最大值为 128.
void draw_point(uint column,uint row)
{
    uchar i,dat,page;
    row--;
    column--; //我们平常的“第 1 行”在液晶屏里叫“第 0 行”，所以减 1，同理,列也如此
    page=row/8;
    page++;
    lcd_address(page,column); //设置地址
    Start_Read_Modify_Write(); //开始 “读取-修改-写入” 模式
    i=row%8; //行位置分成了第几页和这一页内的第几位,i=y%8 即是这个第几位
    dat=1;
    dat<<=i; //用移位的方法,根据这个“第几位”算出这个字节的数值来，这个就是我们要画的“点”的一个字节数据
    dat|=read_data_twice(); //要画点的数据和刚读到的原来液晶屏的位置上的数据进行“或” 运算， 以免覆盖原来的数据。
    transfer_data(dat);
    End_Read_Modify_Write(); //结束 “读取-修改-写入” 模式
}

//“画直线”函数的注意事项：
//直线的斜率 k=(y2-y1)/(x2-x1)，k\x1\y1\x2\y2 必须是浮点型的数据，除此之外，由于终点坐标可能小于起点坐标，所以斜率还有可能是负数的。
//由于 k=(y2-y1)/(x2-x1)，所以在 x2=x1 时，导致除数为 0，数学上这是行不通的，所以另外想办法。同理 k_y=(x2-x1)/(y2-y1)当 y2=y1 时也是行不通的。
//另外 int 是 16 位的整数，uchar 却只有 8 位.
//
void draw_line(float x1,float y1,float x2, float y2)
{
    int i;float k,k_y,x,z;
    if( (y2-y1)==0&&(x2-x1)!=0 ) //如果 y2-y1=0 且 x2-x1 不等于 0，则画一条横线。
    {
        if(x2<x1) //如果 x2<x1，则两个坐标互换。画横线时，从左到右与从右到左是一样的结果。
        {
            z=x2; x2=x1;x1=z;
        }
        for(i=0;i<=(x2-x1);i++)
        {
            draw_point((x1+i),y1);
        }
    }
    else if( ((x2-x1)==0)&&((y2-y1)!=0 ))//如果 x2-x1=0 且 y2-y1 不等于 0，则画一条竖线。
    {
        if(y2<y1) //如果 y2<y1，则两个坐标互换。画竖线时，从上到下与从下到上是一样的结果。
        {
            z=y2; y2=y1;y1=z;
        }
        for(i=0;i<=(y2-y1);i++)
        {
            draw_point(x1,(y1+i));
        }
    }
    else if((x2-x1)==0&&(y2-y1)==0) //如果 (x2=x1)且(y2=y1)，画一个点即可
    {
        draw_point(x1,y2);
    }
    else //否则，画斜线
    {
        if(x2<x1) //如果 x2<x1，则两个坐标互换。（从起点画到终点）与（从终点画到起点）结果是一样的。
        {
            z=y2; y2=y1;y1=z;
            z=x2; x2=x1;x1=z;
        }
        else; //
        if(fabs(y2-y1)<=fabs(x2-x1)) //如果(y2-y1)的绝对值小于等于（x2-x1）的绝对值，就启动方案一：x 逐点扫描，y 按斜率计算，然后画点（x+i，y）
        {
            k=(y2-y1)/(x2-x1); //k 是斜率
            for(i=0;i<=(x2-x1);i++)
            {
                draw_point((x1+i),(y1+k*i));
            }
        }
        else //如果(y2-y1)的绝对值大于(x2-x1)的绝对值，就启动方案二：y 逐点扫描，x 按斜率计算，然后画点（x,y+i）或（x,y-i)
        {
            k_y=fabs((x2-x1)/(y2-y1)); //k_y 是反斜率（即 x 除以 y）。fabs 是浮点型数据的绝对值
            for(i=0;i<=fabs(y2-y1);i++)
            {
                x=x1+k_y*i;
                if((y2-y1)>0)
                {
                    draw_point(x,(y1+i));
                }
                else
                {
                    draw_point(x,(y1-i));
                }
            }
        }
    }
}
void sleep()
{
    transfer_command(0xac);/*静态图标关闭*/
    transfer_command(0x00);/*静态图标寄存器设置：关闭。此指令与上述指令一起完成静态图标关闭*/
    transfer_command(0xae);/*显示：关*/
    transfer_command(0xa5);/*全屏显示：开*/
}
void wake()
{
    transfer_command(0xa4);/*全屏显示：关。进入正常模式*/
    transfer_command(0xad);/*静态图标开启*/
    transfer_command(0x03);/*静态图标寄存器设置：开。此指令与上述指令一起完成静态图标开启*/
    transfer_command(0xaf);/*显示：开*/
}

void Init_Lcd(void)
{
    reset0; /*低电平复位*/
    Delay_ms(200);
    reset1; /*复位完毕*/
    Delay_ms(50);
    transfer_command(0xe2); /*软复位*/
    Delay_ms(5);
    transfer_command(0x2c); /*升压步聚 1*/
    Delay_ms(5);
    transfer_command(0x2e); /*升压步聚 2*/
    Delay_ms(5);
    transfer_command(0x2f); /*升压步聚 3*/
    Delay_ms(5);
    transfer_command(0x24); /*粗调对比度，可设置范围 0x20～0x27*/
    transfer_command(0x81); /*微调对比度*/
    transfer_command(0x0d); /*0x22,微调对比度的值，可设置范围 0x00～0x3f*/
    transfer_command(0xa2); /*1/9 偏压比（bias）*/
    transfer_command(0xc0); /*行扫描顺序：从上到下*/
    transfer_command(0xa1); /*列扫描顺序：从左到右*/
    transfer_command(0x40); /*起始行：第一行开始*/
    transfer_command(0xaf); /*开显示*/
    test_display(0xaa,0x11);
    clear_screen();
    display_string_5x8(1,1,0,"Select>>>>");
    display_string_5x8(3,64,0,"1.Graphic ");
    display_string_5x8(4,64,0,"2.Chinese " );
    display_string_5x8(5,64,0,"3.Movie ");
    display_string_5x8(6,64,0,"4.Contrast ");
    display_string_5x8(7,64,0,"5.Mirror ");
    display_string_5x8(8,1,0,"PRE USER DEL NEW ");
}
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  MX_ETH_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);
  Init_Lcd();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /**Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.LSEState = RCC_LSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

   uint8_t MACAddr[6] ;

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  heth.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
  heth.Init.PhyAddress = LAN8742A_PHY_ADDRESS;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.RxMode = ETH_RXPOLLING_MODE;
  heth.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
  heth.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;

  /* USER CODE BEGIN MACADDRESS */
    
  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ETH_Init 2 */

  /* USER CODE END ETH_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /**Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, CTR_485_1_Pin|IN_1_Pin|SPI1_CS_Pin|RELAY_1_Pin 
                          |RELAY_2_Pin|RELAY_3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, IN_2_Pin|CTR_485_2_Pin|IN_3_Pin|CTR_485_3_Pin 
                          |IN_4_Pin|CTR_485_4_Pin|LORA_CS_Pin|LORA_RST_Pin 
                          |RELAY_4_Pin|RELAY_5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, ETH_RST_Pin|LCD_CS_Pin|LCD_RST_Pin|LCD_RS_Pin 
                          |LCD_SCK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_DIO_GPIO_Port, LCD_DIO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, WIFI_RST_Pin|LCD_BCK_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : KEY_6_Pin KEY_5_Pin KEY_4_Pin KEY_3_Pin 
                           KEY_2_Pin */
  GPIO_InitStruct.Pin = KEY_6_Pin|KEY_5_Pin|KEY_4_Pin|KEY_3_Pin 
                          |KEY_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY_1_Pin */
  GPIO_InitStruct.Pin = KEY_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(KEY_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DC2_IN_EXT_Pin DC3_IN_EXT_Pin DC4_IN_EXT_Pin DC5_IN_EXT_Pin */
  GPIO_InitStruct.Pin = DC2_IN_EXT_Pin|DC3_IN_EXT_Pin|DC4_IN_EXT_Pin|DC5_IN_EXT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : DC6_IN_EXT_Pin */
  GPIO_InitStruct.Pin = DC6_IN_EXT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DC6_IN_EXT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : CTR_485_1_Pin IN_1_Pin SPI1_CS_Pin RELAY_1_Pin 
                           RELAY_2_Pin RELAY_3_Pin */
  GPIO_InitStruct.Pin = CTR_485_1_Pin|IN_1_Pin|SPI1_CS_Pin|RELAY_1_Pin 
                          |RELAY_2_Pin|RELAY_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : IN_2_Pin CTR_485_2_Pin IN_3_Pin CTR_485_3_Pin 
                           IN_4_Pin CTR_485_4_Pin LORA_CS_Pin LORA_RST_Pin 
                           RELAY_4_Pin RELAY_5_Pin */
  GPIO_InitStruct.Pin = IN_2_Pin|CTR_485_2_Pin|IN_3_Pin|CTR_485_3_Pin 
                          |IN_4_Pin|CTR_485_4_Pin|LORA_CS_Pin|LORA_RST_Pin 
                          |RELAY_4_Pin|RELAY_5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : LORA_INT_Pin */
  GPIO_InitStruct.Pin = LORA_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LORA_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ETH_RST_Pin LCD_CS_Pin LCD_RST_Pin LCD_RS_Pin 
                           LCD_SCK_Pin */
  GPIO_InitStruct.Pin = ETH_RST_Pin|LCD_CS_Pin|LCD_RST_Pin|LCD_RS_Pin 
                          |LCD_SCK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_DIO_Pin */
  GPIO_InitStruct.Pin = LCD_DIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_DIO_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : WIFI_RST_Pin LCD_BCK_LED_Pin */
  GPIO_InitStruct.Pin = WIFI_RST_Pin|LCD_BCK_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
