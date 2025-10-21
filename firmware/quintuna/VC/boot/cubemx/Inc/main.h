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
#include "hw/error.h"
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
#define LSI_FREQUENCY 32000
#define IWDG_PRESCALER 4
#define IWDG_RESET_FREQUENCY 4
#define SB_SHDN_3V3_OUT_Pin GPIO_PIN_0
#define SB_SHDN_3V3_OUT_GPIO_Port GPIOC
#define INTERIA_3V3_OUT_Pin GPIO_PIN_1
#define INTERIA_3V3_OUT_GPIO_Port GPIOC
#define FR_STBY_INV_Pin GPIO_PIN_7
#define FR_STBY_INV_GPIO_Port GPIOA
#define TSMS_3V3_OUT_Pin GPIO_PIN_2
#define TSMS_3V3_OUT_GPIO_Port GPIOB
#define RR_ILCK_3V3_OUT_Pin GPIO_PIN_7
#define RR_ILCK_3V3_OUT_GPIO_Port GPIOE
#define R_INV_EN_Pin GPIO_PIN_8
#define R_INV_EN_GPIO_Port GPIOE
#define LED_Pin GPIO_PIN_9
#define LED_GPIO_Port GPIOE
#define BOOT_LED_Pin GPIO_PIN_10
#define BOOT_LED_GPIO_Port GPIOE
#define BMS_EN_Pin GPIO_PIN_11
#define BMS_EN_GPIO_Port GPIOE
#define FR_STBY_FRONT_Pin GPIO_PIN_12
#define FR_STBY_FRONT_GPIO_Port GPIOE
#define PGOOD_Pin GPIO_PIN_13
#define PGOOD_GPIO_Port GPIOE
#define BAT_CHRG_MODE_Pin GPIO_PIN_14
#define BAT_CHRG_MODE_GPIO_Port GPIOE
#define BAT_CHRG_nSHDN_Pin GPIO_PIN_15
#define BAT_CHRG_nSHDN_GPIO_Port GPIOE
#define BAT_MTR_nALERT_Pin GPIO_PIN_12
#define BAT_MTR_nALERT_GPIO_Port GPIOB
#define FR_STBY_REAR_Pin GPIO_PIN_13
#define FR_STBY_REAR_GPIO_Port GPIOB
#define FRONT_EN_Pin GPIO_PIN_14
#define FRONT_EN_GPIO_Port GPIOB
#define RSM_EN_Pin GPIO_PIN_15
#define RSM_EN_GPIO_Port GPIOB
#define IMU_INT1_Pin GPIO_PIN_9
#define IMU_INT1_GPIO_Port GPIOD
#define IMU_INT2_Pin GPIO_PIN_10
#define IMU_INT2_GPIO_Port GPIOD
#define RAD_FAN_FR_STBY_Pin GPIO_PIN_6
#define RAD_FAN_FR_STBY_GPIO_Port GPIOC
#define RL_PUMP_PGOOD_Pin GPIO_PIN_8
#define RL_PUMP_PGOOD_GPIO_Port GPIOC
#define F_INV_EN_Pin GPIO_PIN_9
#define F_INV_EN_GPIO_Port GPIOA
#define DAM_EN_Pin GPIO_PIN_10
#define DAM_EN_GPIO_Port GPIOA
#define SWDIO_1_Pin GPIO_PIN_13
#define SWDIO_1_GPIO_Port GPIOA
#define SWCLK_1_Pin GPIO_PIN_14
#define SWCLK_1_GPIO_Port GPIOA
#define RR_PUMP_EN_Pin GPIO_PIN_12
#define RR_PUMP_EN_GPIO_Port GPIOC
#define CAN1_RX_Pin GPIO_PIN_0
#define CAN1_RX_GPIO_Port GPIOD
#define CAN1_TX_Pin GPIO_PIN_1
#define CAN1_TX_GPIO_Port GPIOD
#define RR_PUMP_PGOOD_Pin GPIO_PIN_2
#define RR_PUMP_PGOOD_GPIO_Port GPIOD
#define PCM_EN_Pin GPIO_PIN_3
#define PCM_EN_GPIO_Port GPIOD
#define F_PUMP_PGOOD_Pin GPIO_PIN_4
#define F_PUMP_PGOOD_GPIO_Port GPIOD
#define F_PUMP_EN_Pin GPIO_PIN_5
#define F_PUMP_EN_GPIO_Port GPIOD
#define R_RAD_FAN_EN_Pin GPIO_PIN_6
#define R_RAD_FAN_EN_GPIO_Port GPIOD
#define RL_PUMP_EN_Pin GPIO_PIN_7
#define RL_PUMP_EN_GPIO_Port GPIOD
#define SWO1_Pin GPIO_PIN_3
#define SWO1_GPIO_Port GPIOB
#define L_RAD_FAN_EN_Pin GPIO_PIN_7
#define L_RAD_FAN_EN_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
