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
    extern ADC_HandleTypeDef   hadc1;
    extern FDCAN_HandleTypeDef hfdcan1;
    extern I2C_HandleTypeDef   hi2c2;
    extern SPI_HandleTypeDef   hspi3;
    extern TIM_HandleTypeDef   htim3;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BPS_R_3V3_Pin GPIO_PIN_2
#define BPS_R_3V3_GPIO_Port GPIOC
#define LC3_OUT_Pin GPIO_PIN_0
#define LC3_OUT_GPIO_Port GPIOA
#define nBSPD_BRAKE_PRESSED_3V3_Pin GPIO_PIN_2
#define nBSPD_BRAKE_PRESSED_3V3_GPIO_Port GPIOA
#define BRAKE_OCSC_OK_3V3_Pin GPIO_PIN_3
#define BRAKE_OCSC_OK_3V3_GPIO_Port GPIOA
#define RL_INT_3V3_SENS_Pin GPIO_PIN_4
#define RL_INT_3V3_SENS_GPIO_Port GPIOA
#define SUSP_TRAVEL_RL_3V3_Pin GPIO_PIN_5
#define SUSP_TRAVEL_RL_3V3_GPIO_Port GPIOA
#define SUSP_TRAVEL_RR_3V3_Pin GPIO_PIN_6
#define SUSP_TRAVEL_RR_3V3_GPIO_Port GPIOA
#define SUSP_TRAVEL_RL_OCSC_Pin GPIO_PIN_7
#define SUSP_TRAVEL_RL_OCSC_GPIO_Port GPIOA
#define SUSP_TRAVEL_RR_OCSC_Pin GPIO_PIN_4
#define SUSP_TRAVEL_RR_OCSC_GPIO_Port GPIOC
#define RPUMP_SCL_Pin GPIO_PIN_10
#define RPUMP_SCL_GPIO_Port GPIOB
#define RPUMP_SDA_Pin GPIO_PIN_12
#define RPUMP_SDA_GPIO_Port GPIOB
#define IMU_CS_Pin GPIO_PIN_6
#define IMU_CS_GPIO_Port GPIOC
#define IMU_INT_Pin GPIO_PIN_7
#define IMU_INT_GPIO_Port GPIOC
#define IMU_FSYNC_Pin GPIO_PIN_8
#define IMU_FSYNC_GPIO_Port GPIOC
#define FLOW_METER_5V5_Pin GPIO_PIN_8
#define FLOW_METER_5V5_GPIO_Port GPIOA
#define BRAKE_LIGHT_EN_3V3_Pin GPIO_PIN_9
#define BRAKE_LIGHT_EN_3V3_GPIO_Port GPIOA
#define IMU_SPC_Pin GPIO_PIN_10
#define IMU_SPC_GPIO_Port GPIOC
#define IMU_SDO_Pin GPIO_PIN_11
#define IMU_SDO_GPIO_Port GPIOC
#define IMU_SDI_Pin GPIO_PIN_12
#define IMU_SDI_GPIO_Port GPIOC
#define D_P_PULLUP_Pin GPIO_PIN_2
#define D_P_PULLUP_GPIO_Port GPIOD
#define LED_Pin GPIO_PIN_4
#define LED_GPIO_Port GPIOB
#define CANTX_Pin GPIO_PIN_7
#define CANTX_GPIO_Port GPIOB
#define CANRX_Pin GPIO_PIN_8
#define CANRX_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
