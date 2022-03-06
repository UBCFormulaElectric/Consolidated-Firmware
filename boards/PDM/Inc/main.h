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
#define IS_AUX1_AUX2_Pin GPIO_PIN_0
#define IS_AUX1_AUX2_GPIO_Port GPIOC
#define IS_AIR_SHDN_LV_PWR_Pin GPIO_PIN_1
#define IS_AIR_SHDN_LV_PWR_GPIO_Port GPIOC
#define IS_DI_BL_DI_BR_Pin GPIO_PIN_2
#define IS_DI_BL_DI_BR_GPIO_Port GPIOC
#define IS_DI_FL_DI_FR_Pin GPIO_PIN_3
#define IS_DI_FL_DI_FR_GPIO_Port GPIOC
#define VBAT_SENSE_Pin GPIO_PIN_0
#define VBAT_SENSE_GPIO_Port GPIOA
#define FR_STBY_DI_REAR_Pin GPIO_PIN_1
#define FR_STBY_DI_REAR_GPIO_Port GPIOA
#define CUR_SNS_DI_REAR_RHS_Pin GPIO_PIN_2
#define CUR_SNS_DI_REAR_RHS_GPIO_Port GPIOA
#define CUR_SNS_DI_REAR_LHS_Pin GPIO_PIN_3
#define CUR_SNS_DI_REAR_LHS_GPIO_Port GPIOA
#define IN_DI_FRONT_LHS_Pin GPIO_PIN_4
#define IN_DI_FRONT_LHS_GPIO_Port GPIOA
#define CUR_SNS_DI_FRONT_RHS_Pin GPIO_PIN_5
#define CUR_SNS_DI_FRONT_RHS_GPIO_Port GPIOA
#define CUR_SNS_DI_FRONT_LHS_Pin GPIO_PIN_6
#define CUR_SNS_DI_FRONT_LHS_GPIO_Port GPIOA
#define CUR_SNS_AUX2_Pin GPIO_PIN_7
#define CUR_SNS_AUX2_GPIO_Port GPIOA
#define CUR_SNS_AUX1_Pin GPIO_PIN_4
#define CUR_SNS_AUX1_GPIO_Port GPIOC
#define CUR_SNS_LVPWR_Pin GPIO_PIN_5
#define CUR_SNS_LVPWR_GPIO_Port GPIOC
#define CUR_SNS_AIR_Pin GPIO_PIN_2
#define CUR_SNS_AIR_GPIO_Port GPIOB
#define IN_AIR_Pin GPIO_PIN_10
#define IN_AIR_GPIO_Port GPIOB
#define FR_STBY_AIR_LVPWR_Pin GPIO_PIN_11
#define FR_STBY_AIR_LVPWR_GPIO_Port GPIOB
#define IN_LVPWR_Pin GPIO_PIN_12
#define IN_LVPWR_GPIO_Port GPIOB
#define IN_AUX1_Pin GPIO_PIN_13
#define IN_AUX1_GPIO_Port GPIOB
#define FR_STBY_AUX1_AUX2_Pin GPIO_PIN_14
#define FR_STBY_AUX1_AUX2_GPIO_Port GPIOB
#define IN_AUX2_Pin GPIO_PIN_15
#define IN_AUX2_GPIO_Port GPIOB
#define STATUS_R_Pin GPIO_PIN_6
#define STATUS_R_GPIO_Port GPIOC
#define STATUS_G_Pin GPIO_PIN_7
#define STATUS_G_GPIO_Port GPIOC
#define STATUS_B_Pin GPIO_PIN_8
#define STATUS_B_GPIO_Port GPIOC
#define GPIOB_4_Pin GPIO_PIN_9
#define GPIOB_4_GPIO_Port GPIOC
#define GPIOB_3_Pin GPIO_PIN_8
#define GPIOB_3_GPIO_Port GPIOA
#define GPIOB_2_Pin GPIO_PIN_9
#define GPIOB_2_GPIO_Port GPIOA
#define GPIOB_1_Pin GPIO_PIN_10
#define GPIOB_1_GPIO_Port GPIOA
#define CHRG_Pin GPIO_PIN_15
#define CHRG_GPIO_Port GPIOA
#define CHRG_FAULT_Pin GPIO_PIN_10
#define CHRG_FAULT_GPIO_Port GPIOC
#define PGOOD_Pin GPIO_PIN_11
#define PGOOD_GPIO_Port GPIOC
#define IN_DI_REAR_RHS_Pin GPIO_PIN_9
#define IN_DI_REAR_RHS_GPIO_Port GPIOB
    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
