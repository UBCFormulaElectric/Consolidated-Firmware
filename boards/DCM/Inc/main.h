/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "Io/Io_SharedErrorHandlerOverride.h"
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
#define TASK1HZ_STACK_SIZE 128
#define TASK1KHZ_STACK_SIZE 128
#define TASKCANRX_STACK_SIZE 128
#define TASKCANTX_STACK_SIZE 128
#define UNUSED_ANALOG_IN1_Pin GPIO_PIN_0
#define UNUSED_ANALOG_IN1_GPIO_Port GPIOA
#define UNUSED_ANALOG_IN2_Pin GPIO_PIN_1
#define UNUSED_ANALOG_IN2_GPIO_Port GPIOA
#define UNUSED_ANALOG_IN3_Pin GPIO_PIN_2
#define UNUSED_ANALOG_IN3_GPIO_Port GPIOA
#define UNUSED_ANALOG_OUT_Pin GPIO_PIN_4
#define UNUSED_ANALOG_OUT_GPIO_Port GPIOA
#define UNUSED_GPIO1_Pin GPIO_PIN_5
#define UNUSED_GPIO1_GPIO_Port GPIOA
#define UNUSED_GPIO2_Pin GPIO_PIN_6
#define UNUSED_GPIO2_GPIO_Port GPIOA
#define UNUSED_GPIO3_Pin GPIO_PIN_7
#define UNUSED_GPIO3_GPIO_Port GPIOA
#define UNUSED_GPIO4_Pin GPIO_PIN_0
#define UNUSED_GPIO4_GPIO_Port GPIOB
#define UNUSED_GPIO5_Pin GPIO_PIN_1
#define UNUSED_GPIO5_GPIO_Port GPIOB
#define UNUSED_GPIO6_Pin GPIO_PIN_2
#define UNUSED_GPIO6_GPIO_Port GPIOB
#define INVERTER_L_RUN_Pin GPIO_PIN_10
#define INVERTER_L_RUN_GPIO_Port GPIOB
#define INVERTER_R_RUN_Pin GPIO_PIN_11
#define INVERTER_R_RUN_GPIO_Port GPIOB
#define IMON_Pin GPIO_PIN_13
#define IMON_GPIO_Port GPIOB
#define IMU_INT_Pin GPIO_PIN_14
#define IMU_INT_GPIO_Port GPIOB
#define STATUS_R_Pin GPIO_PIN_8
#define STATUS_R_GPIO_Port GPIOA
#define STATUS_G_Pin GPIO_PIN_9
#define STATUS_G_GPIO_Port GPIOA
#define STATUS_B_Pin GPIO_PIN_10
#define STATUS_B_GPIO_Port GPIOA
#define BSPD_BRAKE_THRES_Pin GPIO_PIN_15
#define BSPD_BRAKE_THRES_GPIO_Port GPIOA
#define BUZZER_EN_Pin GPIO_PIN_4
#define BUZZER_EN_GPIO_Port GPIOB
#define BRAKE_LIGHT_EN_Pin GPIO_PIN_5
#define BRAKE_LIGHT_EN_GPIO_Port GPIOB
    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
