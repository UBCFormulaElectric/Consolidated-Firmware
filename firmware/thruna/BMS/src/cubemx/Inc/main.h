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
#include "stm32f4xx_hal.h"

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
#define LSI_FREQUENCY 32000
#define TASK100HZ_STACK_SIZE 512
#define TASK1KHZ_STACK_SIZE 512
#define TASK1HZ_STACK_SIZE 512
#define TASKCANTX_STACK_SIZE 512
#define TASKCANRX_STACK_SIZE 512
#define TIM1_FREQUENCY 96000000
#define TIM1_AUTO_RELOAD_REG 0xFFFF
#define TIM1_PWM_MINIMUM_FREQUENCY 1
#define TIM1_PRESCALER (TIM1_FREQUENCY / TIM1_AUTO_RELOAD_REG / TIM1_PWM_MINIMUM_FREQUENCY)
#define TIMx_FREQUENCY 96000000
#define TIM3_PRESCALER 96
#define ADC_FREQUENCY 1000
#define TS_ADC_OFFSET 1983
#define AIR_N_STATUS_Pin GPIO_PIN_13
#define AIR_N_STATUS_GPIO_Port GPIOC
#define TS_VSENSE_P_Pin GPIO_PIN_0
#define TS_VSENSE_P_GPIO_Port GPIOC
#define TS_VSENSE_N_Pin GPIO_PIN_1
#define TS_VSENSE_N_GPIO_Port GPIOC
#define AIR_P_EN_Pin GPIO_PIN_2
#define AIR_P_EN_GPIO_Port GPIOC
#define PRECHARGE_EN_Pin GPIO_PIN_3
#define PRECHARGE_EN_GPIO_Port GPIOC
#define TSAL_RED_Pin GPIO_PIN_0
#define TSAL_RED_GPIO_Port GPIOA
#define TSAL_GRN_Pin GPIO_PIN_1
#define TSAL_GRN_GPIO_Port GPIOA
#define SHDN_ISENSE_Pin GPIO_PIN_3
#define SHDN_ISENSE_GPIO_Port GPIOA
#define SPI1_NSS_Pin GPIO_PIN_4
#define SPI1_NSS_GPIO_Port GPIOA
#define EVSE_I_LIM_PWM_Pin GPIO_PIN_6
#define EVSE_I_LIM_PWM_GPIO_Port GPIOA
#define EVSE_PROX_PILOT_Pin GPIO_PIN_7
#define EVSE_PROX_PILOT_GPIO_Port GPIOA
#define TEMP_SENSE_Pin GPIO_PIN_4
#define TEMP_SENSE_GPIO_Port GPIOC
#define TS_ISENSE_OCSC_OK_5V_Pin GPIO_PIN_5
#define TS_ISENSE_OCSC_OK_5V_GPIO_Port GPIOC
#define TS_ISENSE_50A_Pin GPIO_PIN_0
#define TS_ISENSE_50A_GPIO_Port GPIOB
#define TS_ISENSE_300A_Pin GPIO_PIN_1
#define TS_ISENSE_300A_GPIO_Port GPIOB
#define BMS_OK_3V3_Pin GPIO_PIN_10
#define BMS_OK_3V3_GPIO_Port GPIOB
#define BMS_LATCH_Pin GPIO_PIN_12
#define BMS_LATCH_GPIO_Port GPIOB
#define IMD_LATCH_Pin GPIO_PIN_13
#define IMD_LATCH_GPIO_Port GPIOB
#define BSPD_LATCH_Pin GPIO_PIN_14
#define BSPD_LATCH_GPIO_Port GPIOB
#define BSPD_I_TRIP_5V_Pin GPIO_PIN_15
#define BSPD_I_TRIP_5V_GPIO_Port GPIOB
#define ACCEL_BRAKE_OK_5V_Pin GPIO_PIN_6
#define ACCEL_BRAKE_OK_5V_GPIO_Port GPIOC
#define BSPD_OK_3V3_Pin GPIO_PIN_7
#define BSPD_OK_3V3_GPIO_Port GPIOC
#define CHRG_STATE_3V3_Pin GPIO_PIN_8
#define CHRG_STATE_3V3_GPIO_Port GPIOC
#define CHRG_FLT_3V3_Pin GPIO_PIN_9
#define CHRG_FLT_3V3_GPIO_Port GPIOC
#define CHRG_EN_3V3_Pin GPIO_PIN_8
#define CHRG_EN_3V3_GPIO_Port GPIOA
#define IMD_M_HS_3V3_Pin GPIO_PIN_9
#define IMD_M_HS_3V3_GPIO_Port GPIOA
#define IMD_OK_3V3_Pin GPIO_PIN_10
#define IMD_OK_3V3_GPIO_Port GPIOA
#define AUX_TSENSE_MUX0_Pin GPIO_PIN_15
#define AUX_TSENSE_MUX0_GPIO_Port GPIOA
#define AUX_TSENSE_MUX1_Pin GPIO_PIN_10
#define AUX_TSENSE_MUX1_GPIO_Port GPIOC
#define AUX_TSENSE_MUX2_Pin GPIO_PIN_11
#define AUX_TSENSE_MUX2_GPIO_Port GPIOC
#define AUX_TSENSE_MUX3_Pin GPIO_PIN_12
#define AUX_TSENSE_MUX3_GPIO_Port GPIOC
#define LED_Pin GPIO_PIN_8
#define LED_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
