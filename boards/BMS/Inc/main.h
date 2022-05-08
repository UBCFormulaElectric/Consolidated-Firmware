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
#define IWDG_PRESCALER 4
#define IWDG_RESET_FREQUENCY 5
#define LSI_FREQUENCY 40000
#define TASK1HZ_STACK_SIZE 512
#define TASK100HZ_STACK_SIZE 512
#define TASK1KHZ_STACK_SIZE 512
#define TASKCANTX_STACK_SIZE 512
#define TASKCANRX_STACK_SIZE 512
#define TIM2_FREQUENCY 72000000
#define TIM2_AUTO_RELOAD_REG 0xFFFF
#define TIM2_PWM_MINIMUM_FREQUENCY 1
#define TIM2_PRESCALER (TIM2_FREQUENCY / TIM2_AUTO_RELOAD_REG / TIM2_PWM_MINIMUM_FREQUENCY)
#define TIMx_FREQUENCY 72000000
#define TIM3_PRESCALER 72
#define ADC1_ADC2_FREQUENCY 1000
#define TS_ADC_OFFSET 1983
#define IMD_OK_Pin GPIO_PIN_13
#define IMD_OK_GPIO_Port GPIOC
#define BRUSA_PON_Pin GPIO_PIN_14
#define BRUSA_PON_GPIO_Port GPIOC
#define TSAL_EN_Pin GPIO_PIN_15
#define TSAL_EN_GPIO_Port GPIOC
#define IMD_M_HS_Pin GPIO_PIN_1
#define IMD_M_HS_GPIO_Port GPIOA
#define TS_VSENSE_P_Pin GPIO_PIN_2
#define TS_VSENSE_P_GPIO_Port GPIOA
#define TS_VSENSE_N_Pin GPIO_PIN_3
#define TS_VSENSE_N_GPIO_Port GPIOA
#define MAIN_ISENSE_1_Pin GPIO_PIN_4
#define MAIN_ISENSE_1_GPIO_Port GPIOA
#define ACCEL_BRAKE_OK_Pin GPIO_PIN_5
#define ACCEL_BRAKE_OK_GPIO_Port GPIOA
#define MAIN_ISENSE_2_Pin GPIO_PIN_6
#define MAIN_ISENSE_2_GPIO_Port GPIOA
#define AIR_TOTAL_ISENSE_Pin GPIO_PIN_7
#define AIR_TOTAL_ISENSE_GPIO_Port GPIOA
#define AIR_POWER_STATUS_Pin GPIO_PIN_0
#define AIR_POWER_STATUS_GPIO_Port GPIOB
#define PRE_CHARGE_EN_Pin GPIO_PIN_1
#define PRE_CHARGE_EN_GPIO_Port GPIOB
#define AIR_EN_Pin GPIO_PIN_2
#define AIR_EN_GPIO_Port GPIOB
#define BMS_OK_Pin GPIO_PIN_10
#define BMS_OK_GPIO_Port GPIOB
#define MCU_LATCH_RESET_Pin GPIO_PIN_11
#define MCU_LATCH_RESET_GPIO_Port GPIOB
#define SPI2_NSS_Pin GPIO_PIN_12
#define SPI2_NSS_GPIO_Port GPIOB
#define STATUS_R_Pin GPIO_PIN_8
#define STATUS_R_GPIO_Port GPIOA
#define STATUS_G_Pin GPIO_PIN_9
#define STATUS_G_GPIO_Port GPIOA
#define STATUS_B_Pin GPIO_PIN_10
#define STATUS_B_GPIO_Port GPIOA
#define BRUSA_FLT_Pin GPIO_PIN_11
#define BRUSA_FLT_GPIO_Port GPIOA
#define GPIO2_Pin GPIO_PIN_12
#define GPIO2_GPIO_Port GPIOA
#define CHARGE_STATE_Pin GPIO_PIN_15
#define CHARGE_STATE_GPIO_Port GPIOA
#define OC_SC_OK_Pin GPIO_PIN_4
#define OC_SC_OK_GPIO_Port GPIOB
#define BSPD_OK_Pin GPIO_PIN_5
#define BSPD_OK_GPIO_Port GPIOB
    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
