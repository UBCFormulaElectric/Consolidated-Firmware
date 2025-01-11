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
#include "stm32f4xx_hal.h"

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
#define SHDN_G_Pin GPIO_PIN_3
#define SHDN_G_GPIO_Port GPIOC
#define SHDN_R_Pin GPIO_PIN_0
#define SHDN_R_GPIO_Port GPIOA
#define BMS_B_Pin GPIO_PIN_1
#define BMS_B_GPIO_Port GPIOA
#define BMS_G_Pin GPIO_PIN_4
#define BMS_G_GPIO_Port GPIOA
#define BPSD_R_Pin GPIO_PIN_5
#define BPSD_R_GPIO_Port GPIOA
#define BMS_R_Pin GPIO_PIN_7
#define BMS_R_GPIO_Port GPIOA
#define VC_G_Pin GPIO_PIN_5
#define VC_G_GPIO_Port GPIOC
#define VC_R_Pin GPIO_PIN_0
#define VC_R_GPIO_Port GPIOB
#define AUX_DB_B_Pin GPIO_PIN_1
#define AUX_DB_B_GPIO_Port GPIOB
#define AUX_DB_G_Pin GPIO_PIN_2
#define AUX_DB_G_GPIO_Port GPIOB
#define AUX_DB_R_Pin GPIO_PIN_10
#define AUX_DB_R_GPIO_Port GPIOB
#define CRIT_DB_B_Pin GPIO_PIN_12
#define CRIT_DB_B_GPIO_Port GPIOB
#define CRIT_DB_G_Pin GPIO_PIN_13
#define CRIT_DB_G_GPIO_Port GPIOB
#define CRIT_DB_R_Pin GPIO_PIN_14
#define CRIT_DB_R_GPIO_Port GPIOB
#define RSM_B_Pin GPIO_PIN_15
#define RSM_B_GPIO_Port GPIOB
#define RSM_G_Pin GPIO_PIN_6
#define RSM_G_GPIO_Port GPIOC
#define RSM_R_Pin GPIO_PIN_7
#define RSM_R_GPIO_Port GPIOC
#define FSM_B_Pin GPIO_PIN_8
#define FSM_B_GPIO_Port GPIOC
#define FSM_G_Pin GPIO_PIN_9
#define FSM_G_GPIO_Port GPIOC
#define FSM_R_Pin GPIO_PIN_8
#define FSM_R_GPIO_Port GPIOA
#define nBOOT_EN_Pin GPIO_PIN_9
#define nBOOT_EN_GPIO_Port GPIOA
#define VC_B_Pin GPIO_PIN_10
#define VC_B_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_15
#define LED_GPIO_Port GPIOA
#define IMD_R_Pin GPIO_PIN_12
#define IMD_R_GPIO_Port GPIOC
#define AMS_R_Pin GPIO_PIN_2
#define AMS_R_GPIO_Port GPIOD
#define START_LED_Pin GPIO_PIN_4
#define START_LED_GPIO_Port GPIOB
#define TORQUE_VECTORING_LED_Pin GPIO_PIN_6
#define TORQUE_VECTORING_LED_GPIO_Port GPIOB
#define REGEN_LED_Pin GPIO_PIN_9
#define REGEN_LED_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
