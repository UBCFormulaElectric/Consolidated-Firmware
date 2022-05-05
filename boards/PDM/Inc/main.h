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
#define IWDG_WINDOW_DISABLE_VALUE 4095
#define LSI_FREQUENCY 40000
#define IWDG_PRESCALER 4
#define IWDG_RESET_FREQUENCY 5
#define TASK1HZ_STACK_SIZE 512
#define TASK100HZ_STACK_SIZE 512
#define TASK1KHZ_STACK_SIZE 512
#define TASKCANRX_STACK_SIZE 512
#define TASKCANTX_STACK_SIZE 512
#define STATUS_R_Pin GPIO_PIN_13
#define STATUS_R_GPIO_Port GPIOC
#define STATUS_G_Pin GPIO_PIN_14
#define STATUS_G_GPIO_Port GPIOC
#define STATUS_B_Pin GPIO_PIN_15
#define STATUS_B_GPIO_Port GPIOC
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
#define _24V_ACC_SENSE_Pin GPIO_PIN_1
#define _24V_ACC_SENSE_GPIO_Port GPIOA
#define _24V_BOOST_OUT_SENSE_Pin GPIO_PIN_2
#define _24V_BOOST_OUT_SENSE_GPIO_Port GPIOA
#define PIN_AUX1_Pin GPIO_PIN_3
#define PIN_AUX1_GPIO_Port GPIOA
#define PIN_AUX2_Pin GPIO_PIN_4
#define PIN_AUX2_GPIO_Port GPIOF
#define CUR_SYNC_AUX1_AUX2_Pin GPIO_PIN_4
#define CUR_SYNC_AUX1_AUX2_GPIO_Port GPIOA
#define CSB_AUX1_AUX2_Pin GPIO_PIN_5
#define CSB_AUX1_AUX2_GPIO_Port GPIOA
#define FSB_AUX1_AUX2_Pin GPIO_PIN_6
#define FSB_AUX1_AUX2_GPIO_Port GPIOA
#define FSOB_AUX1_AUX2_Pin GPIO_PIN_7
#define FSOB_AUX1_AUX2_GPIO_Port GPIOA
#define PIN_AIR_SHDN_Pin GPIO_PIN_4
#define PIN_AIR_SHDN_GPIO_Port GPIOC
#define PIN_LV_PWR_Pin GPIO_PIN_5
#define PIN_LV_PWR_GPIO_Port GPIOC
#define CUR_SYNC_AIR_SHDN_LV_PWR_Pin GPIO_PIN_0
#define CUR_SYNC_AIR_SHDN_LV_PWR_GPIO_Port GPIOB
#define CSB_AIR_SHDN_LV_PWR_Pin GPIO_PIN_1
#define CSB_AIR_SHDN_LV_PWR_GPIO_Port GPIOB
#define FSB_AIR_SHDN_LV_PWR_Pin GPIO_PIN_2
#define FSB_AIR_SHDN_LV_PWR_GPIO_Port GPIOB
#define FSOB_AIR_SHDN_LV_PWR_Pin GPIO_PIN_10
#define FSOB_AIR_SHDN_LV_PWR_GPIO_Port GPIOB
#define PIN_DI_BL_Pin GPIO_PIN_11
#define PIN_DI_BL_GPIO_Port GPIOB
#define PIN_DI_BR_Pin GPIO_PIN_12
#define PIN_DI_BR_GPIO_Port GPIOB
#define SCK_Pin GPIO_PIN_13
#define SCK_GPIO_Port GPIOB
#define MISO_Pin GPIO_PIN_14
#define MISO_GPIO_Port GPIOB
#define MOSI_Pin GPIO_PIN_15
#define MOSI_GPIO_Port GPIOB
#define CUR_SYNC_DI_BL_DI_BR_Pin GPIO_PIN_6
#define CUR_SYNC_DI_BL_DI_BR_GPIO_Port GPIOC
#define CSB_DI_BL_DI_BR_Pin GPIO_PIN_7
#define CSB_DI_BL_DI_BR_GPIO_Port GPIOC
#define FSB_DI_BL_DI_BR_Pin GPIO_PIN_8
#define FSB_DI_BL_DI_BR_GPIO_Port GPIOC
#define FSOB_DI_BL_DI_BR_Pin GPIO_PIN_9
#define FSOB_DI_BL_DI_BR_GPIO_Port GPIOC
#define PIN_DI_FL_Pin GPIO_PIN_8
#define PIN_DI_FL_GPIO_Port GPIOA
#define PIN_DI_FR_Pin GPIO_PIN_9
#define PIN_DI_FR_GPIO_Port GPIOA
#define CUR_SYNC_DI_FL_DI_FR_Pin GPIO_PIN_10
#define CUR_SYNC_DI_FL_DI_FR_GPIO_Port GPIOA
#define CSB_DI_FL_DI_FR_Pin GPIO_PIN_15
#define CSB_DI_FL_DI_FR_GPIO_Port GPIOA
#define FSB_DI_FL_DI_FR_Pin GPIO_PIN_10
#define FSB_DI_FL_DI_FR_GPIO_Port GPIOC
#define FSOB_DI_FL_DI_FR_Pin GPIO_PIN_11
#define FSOB_DI_FL_DI_FR_GPIO_Port GPIOC
#define PGOOD_Pin GPIO_PIN_12
#define PGOOD_GPIO_Port GPIOC
#define CHRG_Pin GPIO_PIN_2
#define CHRG_GPIO_Port GPIOD
#define CHRG_FAULT_Pin GPIO_PIN_4
#define CHRG_FAULT_GPIO_Port GPIOB
#define GPIOB_4_Pin GPIO_PIN_6
#define GPIOB_4_GPIO_Port GPIOB
#define GPIOB_3_Pin GPIO_PIN_7
#define GPIOB_3_GPIO_Port GPIOB
#define GPIOB_2_Pin GPIO_PIN_8
#define GPIOB_2_GPIO_Port GPIOB
#define GPIOB_1_Pin GPIO_PIN_9
#define GPIOB_1_GPIO_Port GPIOB
    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
