/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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

    /* USER CODE END EC */

    /* Exported macro ------------------------------------------------------------*/
    /* USER CODE BEGIN EM */

    /* USER CODE END EM */

    /* Exported functions prototypes ---------------------------------------------*/
    void Error_Handler(void);

    /* USER CODE BEGIN EFP */
    extern FDCAN_HandleTypeDef hfdcan1;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_BOOT_Pin GPIO_PIN_13
#define LED_BOOT_GPIO_Port GPIOC
#define _3V3_LED_SERIN_Pin GPIO_PIN_1
#define _3V3_LED_SERIN_GPIO_Port GPIOC
#define _3V3_LED_DIMMING_Pin GPIO_PIN_2
#define _3V3_LED_DIMMING_GPIO_Port GPIOC
#define _3V3_7SEG_RCK_Pin GPIO_PIN_3
#define _3V3_7SEG_RCK_GPIO_Port GPIOA
#define _3V3_7SEG_RCKA4_Pin GPIO_PIN_4
#define _3V3_7SEG_RCKA4_GPIO_Port GPIOA
#define _3V3_PWR_CHRG_CLK_Pin GPIO_PIN_5
#define _3V3_PWR_CHRG_CLK_GPIO_Port GPIOA
#define _3V3_CHRG_DATA_Pin GPIO_PIN_7
#define _3V3_CHRG_DATA_GPIO_Port GPIOA
#define _3V3_7SEG_SERIN_Pin GPIO_PIN_2
#define _3V3_7SEG_SERIN_GPIO_Port GPIOB
#define _3V3_LED_SRCK_Pin GPIO_PIN_10
#define _3V3_LED_SRCK_GPIO_Port GPIOB
#define _3V3_7SEG_DIMMING_Pin GPIO_PIN_6
#define _3V3_7SEG_DIMMING_GPIO_Port GPIOC
#define _3V3_7SEG_SRCK_Pin GPIO_PIN_10
#define _3V3_7SEG_SRCK_GPIO_Port GPIOC

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
