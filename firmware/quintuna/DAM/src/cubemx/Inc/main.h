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
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

    /* Private includes ----------------------------------------------------------*/
    /* USER CODE BEGIN Includes */

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

    /* Exported functions prototypes ---------------------------------------------*/
    void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_0
#define LED_GPIO_Port GPIOC
#define BOOT_Pin GPIO_PIN_1
#define BOOT_GPIO_Port GPIOC
#define SD_FAIL_Pin GPIO_PIN_3
#define SD_FAIL_GPIO_Port GPIOC
#define FROM_900M_CTS_Pin GPIO_PIN_0
#define FROM_900M_CTS_GPIO_Port GPIOA
#define TO_900M_RTS_Pin GPIO_PIN_1
#define TO_900M_RTS_GPIO_Port GPIOA
#define TO_900M_TX_Pin GPIO_PIN_2
#define TO_900M_TX_GPIO_Port GPIOA
#define FROM_9000M_RX_Pin GPIO_PIN_3
#define FROM_9000M_RX_GPIO_Port GPIOA
#define INOUT_900M_GPIO_Pin GPIO_PIN_4
#define INOUT_900M_GPIO_GPIO_Port GPIOA
#define TELEM_PWR_Pin GPIO_PIN_9
#define TELEM_PWR_GPIO_Port GPIOE
#define BUZZER_PWR_EN_Pin GPIO_PIN_15
#define BUZZER_PWR_EN_GPIO_Port GPIOE
#define nTSLM_GRN_EN_Pin GPIO_PIN_13
#define nTSLM_GRN_EN_GPIO_Port GPIOD
#define TSLM_RED_EN_Pin GPIO_PIN_15
#define TSLM_RED_EN_GPIO_Port GPIOD
#define L_SHDN_SENSE_Pin GPIO_PIN_6
#define L_SHDN_SENSE_GPIO_Port GPIOC
#define SPLITTER_SHDN_SENSE_Pin GPIO_PIN_7
#define SPLITTER_SHDN_SENSE_GPIO_Port GPIOC
#define SD_CD_Pin GPIO_PIN_8
#define SD_CD_GPIO_Port GPIOA

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
