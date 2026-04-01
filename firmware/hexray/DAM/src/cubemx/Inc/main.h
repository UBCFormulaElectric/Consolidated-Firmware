/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal.h"

    /* Private includes ----------------------------------------------------------*/
    /* USER CODE BEGIN Includes */

    /* USER CODE END Includes */

    /* Exported types ------------------------------------------------------------*/
    /* USER CODE BEGIN ET */

    /* USER CODE END ET */

    /* Exported constants --------------------------------------------------------*/
    /* USER CODE BEGIN EC */
    extern CRC_HandleTypeDef   hcrc;
    extern FDCAN_HandleTypeDef hfdcan1;
    extern IWDG_HandleTypeDef  hiwdg;
    extern RTC_HandleTypeDef   hrtc;
    extern SD_HandleTypeDef    hsd1;
    extern UART_HandleTypeDef  huart2;
    extern PCD_HandleTypeDef   hpcd_USB_DRD_FS;
    /* USER CODE END EC */

    /* Exported macro ------------------------------------------------------------*/
    /* USER CODE BEGIN EM */

    /* USER CODE END EM */

    /* Exported functions prototypes ---------------------------------------------*/
    void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IWDG_RESET_FREQUENCY 5
#define IWDG_PRESCALER 4
#define LSI_FREQUENCY 32000
#define NTP_Pin GPIO_PIN_0
#define NTP_GPIO_Port GPIOC
#define LED_Pin GPIO_PIN_1
#define LED_GPIO_Port GPIOC
#define BOOT_Pin GPIO_PIN_2
#define BOOT_GPIO_Port GPIOC
#define SD_FAIL_Pin GPIO_PIN_3
#define SD_FAIL_GPIO_Port GPIOC
#define FROM_900M_CTS_Pin GPIO_PIN_0
#define FROM_900M_CTS_GPIO_Port GPIOA
#define TO_900M_RTS_Pin GPIO_PIN_1
#define TO_900M_RTS_GPIO_Port GPIOA
#define TO_900M_TX_Pin GPIO_PIN_2
#define TO_900M_TX_GPIO_Port GPIOA
#define FROM_900M_RX_Pin GPIO_PIN_3
#define FROM_900M_RX_GPIO_Port GPIOA
#define _900M_GPIO_Pin GPIO_PIN_4
#define _900M_GPIO_GPIO_Port GPIOA
#define L_SHDN_SENSE_Pin GPIO_PIN_6
#define L_SHDN_SENSE_GPIO_Port GPIOA
#define R_SHDN_SENSE_Pin GPIO_PIN_7
#define R_SHDN_SENSE_GPIO_Port GPIOA
#define BUZZER_PWR_EN_Pin GPIO_PIN_5
#define BUZZER_PWR_EN_GPIO_Port GPIOC
#define SD_CD_Pin GPIO_PIN_7
#define SD_CD_GPIO_Port GPIOC
#define USB_D_N_Pin GPIO_PIN_11
#define USB_D_N_GPIO_Port GPIOA
#define USB_D_P_Pin GPIO_PIN_12
#define USB_D_P_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define D_P_PULLUP_Pin GPIO_PIN_2
#define D_P_PULLUP_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define nTSIM_GRN_EN_Pin GPIO_PIN_5
#define nTSIM_GRN_EN_GPIO_Port GPIOB
#define TSIM_RED_EN_Pin GPIO_PIN_6
#define TSIM_RED_EN_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
