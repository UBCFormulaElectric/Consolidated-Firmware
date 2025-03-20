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
#define _900M_GPIO_Pin GPIO_PIN_4
#define _900M_GPIO_GPIO_Port GPIOA
#define TELEM_PWR_EN_Pin GPIO_PIN_9
#define TELEM_PWR_EN_GPIO_Port GPIOE
#define BUZZER_PWR_EN_Pin GPIO_PIN_15
#define BUZZER_PWR_EN_GPIO_Port GPIOE
#define TSIM_GRN_EN_Pin GPIO_PIN_13
#define TSIM_GRN_EN_GPIO_Port GPIOD
#define nTSIM_RED_EN_Pin GPIO_PIN_15
#define nTSIM_RED_EN_GPIO_Port GPIOD
#define R_SHDN_SENSE_Pin GPIO_PIN_6
#define R_SHDN_SENSE_GPIO_Port GPIOC
#define L_SHDN_SENSE_Pin GPIO_PIN_7
#define L_SHDN_SENSE_GPIO_Port GPIOC
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
