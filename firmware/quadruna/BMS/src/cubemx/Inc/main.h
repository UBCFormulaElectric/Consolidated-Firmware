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
#define TIM3_PRESCALER 8
#define ADC_FREQUENCY 1000
#define TIM1_AUTO_RELOAD_REG 0xFFFF
#define TIMx_FREQUENCY 256000000
#define IWDG_RESET_FREQUENCY 5
#define TASK1KHZ_STACK_SIZE 512
#define TIM1_FREQUENCY 256000000
#define TASK100HZ_STACK_SIZE 512
#define TASK1HZ_STACK_SIZE 512
#define IWDG_WINDOW_DISABLE_VALUE 4095
#define IWDG_PRESCALER 4
#define TASKCANRX_STACK_SIZE 512
#define TIM1_PWM_MIN_FREQUENCY 1
#define LSI_FREQUENCY 32000
#define TIM1_PRESCALER (TIM1_FREQUENCY / TIM1_AUTO_RELOAD_REG / TIM1_PWM_MIN_FREQUENCY)
#define nPROGRAM_3V3_Pin GPIO_PIN_13
#define nPROGRAM_3V3_GPIO_Port GPIOC
#define TS_VSENSE_P_Pin GPIO_PIN_0
#define TS_VSENSE_P_GPIO_Port GPIOC
#define TS_VSENSE_N_Pin GPIO_PIN_1
#define TS_VSENSE_N_GPIO_Port GPIOC
#define AIR_P_EN_Pin GPIO_PIN_2
#define AIR_P_EN_GPIO_Port GPIOC
#define PRE_CHARGE_EN_Pin GPIO_PIN_3
#define PRE_CHARGE_EN_GPIO_Port GPIOC
#define HVD_SHDN_OK_Pin GPIO_PIN_0
#define HVD_SHDN_OK_GPIO_Port GPIOA
#define TS_ILCK_SHDN_OK_Pin GPIO_PIN_1
#define TS_ILCK_SHDN_OK_GPIO_Port GPIOA
#define nHIGH_CURRENT_BSPD_3V3_Pin GPIO_PIN_2
#define nHIGH_CURRENT_BSPD_3V3_GPIO_Port GPIOA
#define EVSE_I_LIM_PWM_Pin GPIO_PIN_3
#define EVSE_I_LIM_PWM_GPIO_Port GPIOA
#define BSPD_TEST_EN_Pin GPIO_PIN_4
#define BSPD_TEST_EN_GPIO_Port GPIOA
#define nCHIMERA_Pin GPIO_PIN_5
#define nCHIMERA_GPIO_Port GPIOA
#define AUX_TSENSE_Pin GPIO_PIN_4
#define AUX_TSENSE_GPIO_Port GPIOC
#define TS_ISENSE_OCSC_OK_3V3_Pin GPIO_PIN_5
#define TS_ISENSE_OCSC_OK_3V3_GPIO_Port GPIOC
#define TS_ISENSE_400A_Pin GPIO_PIN_0
#define TS_ISENSE_400A_GPIO_Port GPIOB
#define TS_ISENSE_50A_Pin GPIO_PIN_1
#define TS_ISENSE_50A_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_7
#define LED_GPIO_Port GPIOE
#define SPI_CS_Pin GPIO_PIN_12
#define SPI_CS_GPIO_Port GPIOB
#define BMS_OK_3V3_Pin GPIO_PIN_9
#define BMS_OK_3V3_GPIO_Port GPIOD
#define BSPD_OK_3V3_Pin GPIO_PIN_10
#define BSPD_OK_3V3_GPIO_Port GPIOD
#define BSPD_LATCH_Pin GPIO_PIN_11
#define BSPD_LATCH_GPIO_Port GPIOD
#define IMD_LATCH_Pin GPIO_PIN_12
#define IMD_LATCH_GPIO_Port GPIOD
#define BMS_LATCH_Pin GPIO_PIN_13
#define BMS_LATCH_GPIO_Port GPIOD
#define ACCEL_BRAKE_OK_3V3_Pin GPIO_PIN_6
#define ACCEL_BRAKE_OK_3V3_GPIO_Port GPIOC
#define SD_CD_Pin GPIO_PIN_7
#define SD_CD_GPIO_Port GPIOC
#define IMD_M_HS_3V3_Pin GPIO_PIN_8
#define IMD_M_HS_3V3_GPIO_Port GPIOA
#define IMD_OK_3V3_Pin GPIO_PIN_9
#define IMD_OK_3V3_GPIO_Port GPIOA
#define AUX_TSENSE_MUX0_Pin GPIO_PIN_3
#define AUX_TSENSE_MUX0_GPIO_Port GPIOD
#define AUX_TSENSE_MUX1_Pin GPIO_PIN_4
#define AUX_TSENSE_MUX1_GPIO_Port GPIOD
#define AUX_TSENSE_MUX2_Pin GPIO_PIN_5
#define AUX_TSENSE_MUX2_GPIO_Port GPIOD
#define AUX_TSENSE_MUX3_Pin GPIO_PIN_6
#define AUX_TSENSE_MUX3_GPIO_Port GPIOD

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
