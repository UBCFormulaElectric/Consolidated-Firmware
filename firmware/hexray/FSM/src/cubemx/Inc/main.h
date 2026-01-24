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
<<<<<<< HEAD

/* USER CODE BEGIN EFP */
=======
    // void FSM_StartAllTasks(void);
    void tasks_init(void);
    /* USER CODE BEGIN EFP */
>>>>>>> master

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BOTS_3V3_Pin GPIO_PIN_0
#define BOTS_3V3_GPIO_Port GPIOC
#define COCKPIT_SHDN_3V3_Pin GPIO_PIN_1
#define COCKPIT_SHDN_3V3_GPIO_Port GPIOC
#define FR_INT_3V3_Pin GPIO_PIN_2
#define FR_INT_3V3_GPIO_Port GPIOC
#define FL_INT_3V3_Pin GPIO_PIN_3
#define FL_INT_3V3_GPIO_Port GPIOC
#define nBPS_F_OCSC_Pin GPIO_PIN_2
#define nBPS_F_OCSC_GPIO_Port GPIOA
#define nSTR_ANGLE_OCSC_Pin GPIO_PIN_3
#define nSTR_ANGLE_OCSC_GPIO_Port GPIOA
#define IMU_CS_Pin GPIO_PIN_4
#define IMU_CS_GPIO_Port GPIOA
#define IMU_SPC_Pin GPIO_PIN_5
#define IMU_SPC_GPIO_Port GPIOA
#define IMU_SDO_Pin GPIO_PIN_6
#define IMU_SDO_GPIO_Port GPIOA
#define IMU_SDI_Pin GPIO_PIN_7
#define IMU_SDI_GPIO_Port GPIOA
#define nSUSP_FL_OCSC_Pin GPIO_PIN_10
#define nSUSP_FL_OCSC_GPIO_Port GPIOC
#define nSUSP_FR_OCSC_Pin GPIO_PIN_11
#define nSUSP_FR_OCSC_GPIO_Port GPIOC
#define DEBUG_LED_Pin GPIO_PIN_4
#define DEBUG_LED_GPIO_Port GPIOB
#define BOOT_LED_Pin GPIO_PIN_5
#define BOOT_LED_GPIO_Port GPIOB
#define IMU_FSYNC_Pin GPIO_PIN_6
#define IMU_FSYNC_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
