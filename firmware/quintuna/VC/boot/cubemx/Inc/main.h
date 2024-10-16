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
#define INV_R_PWR_EN_Pin GPIO_PIN_2
#define INV_R_PWR_EN_GPIO_Port GPIOE
#define INV_R_PROGRA_Pin GPIO_PIN_3
#define INV_R_PROGRA_GPIO_Port GPIOE
#define INV_L_PROGR_Pin GPIO_PIN_4
#define INV_L_PROGR_GPIO_Port GPIOE
#define SHDN_PWR_EN_Pin GPIO_PIN_6
#define SHDN_PWR_EN_GPIO_Port GPIOE
#define FR_STBY3_Pin GPIO_PIN_13
#define FR_STBY3_GPIO_Port GPIOC
#define LV_PWR_EN_Pin GPIO_PIN_14
#define LV_PWR_EN_GPIO_Port GPIOC
#define BUZZER_PWR_EN_Pin GPIO_PIN_0
#define BUZZER_PWR_EN_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_3
#define LED_GPIO_Port GPIOA
#define nPCM_EN_Pin GPIO_PIN_12
#define nPCM_EN_GPIO_Port GPIOB
#define PUMP_PWR_E_Pin GPIO_PIN_10
#define PUMP_PWR_E_GPIO_Port GPIOD
#define _900K_GPI_Pin GPIO_PIN_13
#define _900K_GPI_GPIO_Port GPIOD
#define FR_STBY2_Pin GPIO_PIN_7
#define FR_STBY2_GPIO_Port GPIOC
#define TELEM_PWR_EN_Pin GPIO_PIN_15
#define TELEM_PWR_EN_GPIO_Port GPIOA
#define AUX_PWR_EN_Pin GPIO_PIN_1
#define AUX_PWR_EN_GPIO_Port GPIOD
#define nBOOT_EN_Pin GPIO_PIN_5
#define nBOOT_EN_GPIO_Port GPIOB
#define INV_L_PWR_EN_Pin GPIO_PIN_0
#define INV_L_PWR_EN_GPIO_Port GPIOE
#define FR_STBY3E1_Pin GPIO_PIN_1
#define FR_STBY3E1_GPIO_Port GPIOE

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
