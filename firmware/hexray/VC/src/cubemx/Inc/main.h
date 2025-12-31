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
#define R_INV_SNS_Pin GPIO_PIN_1
#define R_INV_SNS_GPIO_Port GPIOA
#define F_INV_SNS_Pin GPIO_PIN_2
#define F_INV_SNS_GPIO_Port GPIOA
#define BMS_SNS_s__Pin GPIO_PIN_3
#define BMS_SNS_s__GPIO_Port GPIOA
#define RSM_SNS_Pin GPIO_PIN_4
#define RSM_SNS_GPIO_Port GPIOA
#define DAM_SNS_Pin GPIO_PIN_4
#define DAM_SNS_GPIO_Port GPIOC
#define RR_PUMP_I_SNS_Pin GPIO_PIN_5
#define RR_PUMP_I_SNS_GPIO_Port GPIOC
#define RL_PUMP_I_SNS_Pin GPIO_PIN_0
#define RL_PUMP_I_SNS_GPIO_Port GPIOB
#define R_RAD_FAN_I_SNS_Pin GPIO_PIN_1
#define R_RAD_FAN_I_SNS_GPIO_Port GPIOB
#define L_RAD_FAN_I_SNS_Pin GPIO_PIN_11
#define L_RAD_FAN_I_SNS_GPIO_Port GPIOB
#define CAN_VEH_RX_Pin GPIO_PIN_0
#define CAN_VEH_RX_GPIO_Port GPIOD
#define CAN_VEH_TX_Pin GPIO_PIN_1
#define CAN_VEH_TX_GPIO_Port GPIOD

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
