/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
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
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Io_SharedErrorHandlerOverride.h"
    /* USER CODE END Includes */

    /* Exported types
     * ------------------------------------------------------------*/
    /* USER CODE BEGIN ET */

    /* USER CODE END ET */

    /* Exported constants
     * --------------------------------------------------------*/
    /* USER CODE BEGIN EC */

    /* USER CODE END EC */

    /* Exported macro
     * ------------------------------------------------------------*/
    /* USER CODE BEGIN EM */

    /* USER CODE END EM */

    /* Exported functions prototypes
     * ---------------------------------------------*/
    void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IWDG_WINDOW_DISABLE_VALUE 4095
#define LSI_FREQUENCY 40000
#define IWDG_PRESCALER 4
#define IWDG_RESET_FREQUENCY 5
#define TASK1HZ_STACK_SIZE 512
#define TASK100HZ_STACK_SIZE 512
#define TASK1KHZ_STACK_SIZE 512
#define TASKCANRX_STACK_SIZE 512
#define TASKCANTX_STACK_SIZE 512
#define VBAT_SENSE_Pin GPIO_PIN_1
#define VBAT_SENSE_GPIO_Port GPIOC
#define _24V_AUX_SENSE_Pin GPIO_PIN_2
#define _24V_AUX_SENSE_GPIO_Port GPIOC
#define _24V_ACC_SENSE_Pin GPIO_PIN_3
#define _24V_ACC_SENSE_GPIO_Port GPIOC
#define CUR_SYNC_DI_L_DI_R_Pin GPIO_PIN_0
#define CUR_SYNC_DI_L_DI_R_GPIO_Port GPIOA
#define IS_DI_L_DI_R_Pin GPIO_PIN_1
#define IS_DI_L_DI_R_GPIO_Port GPIOA
#define PIN_DI_R_Pin GPIO_PIN_2
#define PIN_DI_R_GPIO_Port GPIOA
#define PIN_DI_L_Pin GPIO_PIN_3
#define PIN_DI_L_GPIO_Port GPIOA
#define CUR_SYNC_GLV_AIR_SHDN_Pin GPIO_PIN_4
#define CUR_SYNC_GLV_AIR_SHDN_GPIO_Port GPIOA
#define PIN_AIR_SHDN_Pin GPIO_PIN_5
#define PIN_AIR_SHDN_GPIO_Port GPIOA
#define PIN_GLV_Pin GPIO_PIN_6
#define PIN_GLV_GPIO_Port GPIOA
#define CSB_DI_L_DI_R_Pin GPIO_PIN_4
#define CSB_DI_L_DI_R_GPIO_Port GPIOC
#define CSB_GLV_AIR_SHDN_Pin GPIO_PIN_5
#define CSB_GLV_AIR_SHDN_GPIO_Port GPIOC
#define PIN_AUX2_Pin GPIO_PIN_2
#define PIN_AUX2_GPIO_Port GPIOB
#define OBSOLETE_IS_AUX1_AUX2_Pin GPIO_PIN_10
#define OBSOLETE_IS_AUX1_AUX2_GPIO_Port GPIOB
#define PIN_AUX1_Pin GPIO_PIN_12
#define PIN_AUX1_GPIO_Port GPIOB
#define SCK_Pin GPIO_PIN_13
#define SCK_GPIO_Port GPIOB
#define MISO_Pin GPIO_PIN_14
#define MISO_GPIO_Port GPIOB
#define MOSI_Pin GPIO_PIN_15
#define MOSI_GPIO_Port GPIOB
#define CUR_SYNC_AUX1_AUX2_Pin GPIO_PIN_6
#define CUR_SYNC_AUX1_AUX2_GPIO_Port GPIOC
#define CSB_AUX1_AUX2_Pin GPIO_PIN_7
#define CSB_AUX1_AUX2_GPIO_Port GPIOC
#define OBSOLETE_IS_EM_PUMP_Pin GPIO_PIN_8
#define OBSOLETE_IS_EM_PUMP_GPIO_Port GPIOC
#define PIN_COOLING_Pin GPIO_PIN_9
#define PIN_COOLING_GPIO_Port GPIOC
#define PIN_EM_Pin GPIO_PIN_8
#define PIN_EM_GPIO_Port GPIOA
#define CSB_EM_PUMP_Pin GPIO_PIN_9
#define CSB_EM_PUMP_GPIO_Port GPIOA
#define CUR_SYNC_EM_PUMP_Pin GPIO_PIN_10
#define CUR_SYNC_EM_PUMP_GPIO_Port GPIOA
#define OBSOLETE_CHRG_FAULT_Pin GPIO_PIN_10
#define OBSOLETE_CHRG_FAULT_GPIO_Port GPIOC
#define CHRG_LED_Pin GPIO_PIN_11
#define CHRG_LED_GPIO_Port GPIOC
#define PGOOD_Pin GPIO_PIN_12
#define PGOOD_GPIO_Port GPIOC
#define OV_FAULT_MCU_Pin GPIO_PIN_2
#define OV_FAULT_MCU_GPIO_Port GPIOD
#define CHRG_FAULT_Pin GPIO_PIN_8
#define CHRG_FAULT_GPIO_Port GPIOB
#define UNUSED_GPIOD_1_Pin GPIO_PIN_9
#define UNUSED_GPIOD_1_GPIO_Port GPIOB
    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
