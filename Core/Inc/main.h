/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "cmsis_os2.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern osMessageQueueId_t printer_qHandle;
extern osSemaphoreId_t lcd_semHandle;
extern osSemaphoreId_t printer_semHandle;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern SPI_HandleTypeDef hspi1;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void debug_printf(const char* format, ...);
void Start_printer(void *argument);
void StartGPSReceiver(void *argument);
void Start_Parser(void *argument);
void Start_lcd(void *argument);
void Start_lcd_idel(void *argument);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY_Pin GPIO_PIN_0
#define KEY_GPIO_Port GPIOA
#define DEBUG_TX_Pin GPIO_PIN_2
#define DEBUG_TX_GPIO_Port GPIOA
#define LCD_SCK_Pin GPIO_PIN_5
#define LCD_SCK_GPIO_Port GPIOA
#define LCD_MOSI_Pin GPIO_PIN_7
#define LCD_MOSI_GPIO_Port GPIOA
#define LCD_DC_Pin GPIO_PIN_0
#define LCD_DC_GPIO_Port GPIOB
#define LCD_RESET_Pin GPIO_PIN_1
#define LCD_RESET_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_2
#define LCD_CS_GPIO_Port GPIOB
#define DEBUG_PIN_Pin GPIO_PIN_15
#define DEBUG_PIN_GPIO_Port GPIOB
#define GPS_RX_Pin GPIO_PIN_10
#define GPS_RX_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
#define DEBUG_MSG_LEN 128
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
