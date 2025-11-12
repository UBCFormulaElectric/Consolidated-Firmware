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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hw_error.h"
    /* USER CODE END Includes */

    /* Exported types ------------------------------------------------------------*/
    /* USER CODE BEGIN ET */

    /* USER CODE END ET */

    /* Exported constants --------------------------------------------------------*/
    /* USER CODE BEGIN EC */
    extern FDCAN_HandleTypeDef hfdcan1;
    extern FDCAN_HandleTypeDef hfdcan2;
    extern FDCAN_HandleTypeDef hfdcan3;
    extern IWDG_HandleTypeDef  hiwdg1;
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
#define BOOT_Pin GPIO_PIN_6
#define BOOT_GPIO_Port GPIOE
#define LED_Pin GPIO_PIN_0
#define LED_GPIO_Port GPIOA
#define SWDIO2_Pin GPIO_PIN_13
#define SWDIO2_GPIO_Port GPIOA
#define SWCLK2_Pin GPIO_PIN_14
#define SWCLK2_GPIO_Port GPIOA
#define SWO2_Pin GPIO_PIN_3
#define SWO2_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

    void VCR_StartAllTasks(void);
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
