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
#define R_INV_EN_Pin GPIO_PIN_0
#define R_INV_EN_GPIO_Port GPIOC
#define BMS_EN_Pin GPIO_PIN_1
#define BMS_EN_GPIO_Port GPIOC
#define R_INV_SNS_Pin GPIO_PIN_1
#define R_INV_SNS_GPIO_Port GPIOA
#define F_INV_SNS_Pin GPIO_PIN_2
#define F_INV_SNS_GPIO_Port GPIOA
#define BMS_SNS_Pin GPIO_PIN_3
#define BMS_SNS_GPIO_Port GPIOA
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
#define BMS_PG_Pin GPIO_PIN_8
#define BMS_PG_GPIO_Port GPIOE
#define LED_Pin GPIO_PIN_9
#define LED_GPIO_Port GPIOE
#define BOOT_Pin GPIO_PIN_10
#define BOOT_GPIO_Port GPIOE
#define BOOST_PG_Pin GPIO_PIN_13
#define BOOST_PG_GPIO_Port GPIOE
#define L_RAD_FAN_I_SNS_Pin GPIO_PIN_11
#define L_RAD_FAN_I_SNS_GPIO_Port GPIOB
#define R_INV_PG_Pin GPIO_PIN_8
#define R_INV_PG_GPIO_Port GPIOD
#define F_INV_EN_Pin GPIO_PIN_9
#define F_INV_EN_GPIO_Port GPIOD
#define F_INV_PG_Pin GPIO_PIN_10
#define F_INV_PG_GPIO_Port GPIOD
#define CAN_INV_RX_Pin GPIO_PIN_12
#define CAN_INV_RX_GPIO_Port GPIOD
#define CAN_INV_TX_Pin GPIO_PIN_13
#define CAN_INV_TX_GPIO_Port GPIOD
#define RSM_EN_Pin GPIO_PIN_14
#define RSM_EN_GPIO_Port GPIOD
#define RSM_PG_Pin GPIO_PIN_7
#define RSM_PG_GPIO_Port GPIOC
#define DAM_EN_Pin GPIO_PIN_8
#define DAM_EN_GPIO_Port GPIOC
#define DAM_PG_Pin GPIO_PIN_9
#define DAM_PG_GPIO_Port GPIOC
#define RR_PUMP_EN_Pin GPIO_PIN_8
#define RR_PUMP_EN_GPIO_Port GPIOA
#define RR_PUMP_PGOOD_Pin GPIO_PIN_9
#define RR_PUMP_PGOOD_GPIO_Port GPIOA
#define PCM_EN_Pin GPIO_PIN_12
#define PCM_EN_GPIO_Port GPIOC
#define CAN_VEH_RX_Pin GPIO_PIN_0
#define CAN_VEH_RX_GPIO_Port GPIOD
#define CAN_VEH_TX_Pin GPIO_PIN_1
#define CAN_VEH_TX_GPIO_Port GPIOD
#define MISC_FUSE_PG_Pin GPIO_PIN_2
#define MISC_FUSE_PG_GPIO_Port GPIOD
#define MISC_FUSE_EN_Pin GPIO_PIN_3
#define MISC_FUSE_EN_GPIO_Port GPIOD
#define L_RAD_FAN_PG_Pin GPIO_PIN_4
#define L_RAD_FAN_PG_GPIO_Port GPIOD
#define R_RAD_FAN_EN_Pin GPIO_PIN_5
#define R_RAD_FAN_EN_GPIO_Port GPIOD
#define L_RAD_FAN_EN_Pin GPIO_PIN_6
#define L_RAD_FAN_EN_GPIO_Port GPIOD
#define R_RAD_FAN_PG_Pin GPIO_PIN_7
#define R_RAD_FAN_PG_GPIO_Port GPIOD
#define FRONT_PG_Pin GPIO_PIN_4
#define FRONT_PG_GPIO_Port GPIOB
#define FRONT_EN_Pin GPIO_PIN_5
#define FRONT_EN_GPIO_Port GPIOB
#define RL_PUMP_PGOOD_Pin GPIO_PIN_6
#define RL_PUMP_PGOOD_GPIO_Port GPIOB
#define RL_PUMP_EN_Pin GPIO_PIN_7
#define RL_PUMP_EN_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
