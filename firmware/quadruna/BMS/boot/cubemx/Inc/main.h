/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#define nBOOT_EN_Pin GPIO_PIN_13
#define nBOOT_EN_GPIO_Port GPIOC
#define AIR_P_EN_Pin GPIO_PIN_2
#define AIR_P_EN_GPIO_Port GPIOC
#define PRE_CHARGE_END_Pin GPIO_PIN_3
#define PRE_CHARGE_END_GPIO_Port GPIOC
#define BSPD_TEST_EN_Pin GPIO_PIN_4
#define BSPD_TEST_EN_GPIO_Port GPIOA
#define nCHIMERA_Pin GPIO_PIN_5
#define nCHIMERA_GPIO_Port GPIOA
#define DEBUG_LED_Pin GPIO_PIN_7
#define DEBUG_LED_GPIO_Port GPIOE
#define SPI_CS_Pin GPIO_PIN_12
#define SPI_CS_GPIO_Port GPIOB
#define BMS_OK_Pin GPIO_PIN_9
#define BMS_OK_GPIO_Port GPIOD

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
