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
#include "hw_error.h"
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
#define TIM3_PRESCALER 10
#define ADC_FREQUENCY 1000
#define TASKCANTX_STACK_SIZE 512
#define TIM1_AUTO_RELOAD_REG 0xFFFF
#define TIMx_FREQUENCY 550000000
#define IWDG_RESET_FREQUENCY 5
#define TASK1KHZ_STACK_SIZE 512
#define TIM1_FREQUENCY 275000000
#define TIM3_FREQUENCY 275000000
#define TASK100HZ_STACK_SIZE 512
#define TASK1HZ_STACK_SIZE 512
#define IWDG_WINDOW_DISABLE_VALUE 4095
#define IWDG_PRESCALER 4
#define TASKCANRX_STACK_SIZE 512
#define TIM1_PWM_MIN_FREQUENCY 1
#define LSI_FREQUENCY 32000
#define TIM1_PRESCALER (TIM1_FREQUENCY / TIM1_AUTO_RELOAD_REG / TIM1_PWM_MIN_FREQUENCY)
#define TIM5_FREQUENCY 275000000
#define TIM5_AUTO_RELOAD_REG 0xFFFF
#define TIM5_PWM_MIN_FREQUENCY 1
#define TIM5_PRESCALER (TIM5_FREQUENCY / TIM5_AUTO_RELOAD_REG / TIM5_PWM_MIN_FREQUENCY)
#define IR_P_EN_Pin GPIO_PIN_3
#define IR_P_EN_GPIO_Port GPIOE
#define SHDN_EN_Pin GPIO_PIN_4
#define SHDN_EN_GPIO_Port GPIOE
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define SPI_CS_LS_Pin GPIO_PIN_14
#define SPI_CS_LS_GPIO_Port GPIOC
#define MSD_SHDN_SNS_Pin GPIO_PIN_0
#define MSD_SHDN_SNS_GPIO_Port GPIOC
#define HV_P_INTLCK_SNS_Pin GPIO_PIN_1
#define HV_P_INTLCK_SNS_GPIO_Port GPIOC
#define HV_N_INTLCK_SNS_Pin GPIO_PIN_2
#define HV_N_INTLCK_SNS_GPIO_Port GPIOC
#define BOOT_LED_Pin GPIO_PIN_0
#define BOOT_LED_GPIO_Port GPIOA
#define IMD_LATCH_Pin GPIO_PIN_4
#define IMD_LATCH_GPIO_Port GPIOC
#define DIAG_Pin GPIO_PIN_5
#define DIAG_GPIO_Port GPIOC
#define BSPD_OK_Pin GPIO_PIN_2
#define BSPD_OK_GPIO_Port GPIOB
#define BMS_LATCH_Pin GPIO_PIN_7
#define BMS_LATCH_GPIO_Port GPIOE
#define IMD_OK_Pin GPIO_PIN_8
#define IMD_OK_GPIO_Port GPIOE
#define BSPD_LATCH_Pin GPIO_PIN_9
#define BSPD_LATCH_GPIO_Port GPIOE
#define PRE_CHARGE_EN_Pin GPIO_PIN_11
#define PRE_CHARGE_EN_GPIO_Port GPIOE
#define FB_STBY_SHDN_Pin GPIO_PIN_13
#define FB_STBY_SHDN_GPIO_Port GPIOE
#define FAN_EN_Pin GPIO_PIN_14
#define FAN_EN_GPIO_Port GPIOE
#define FAN_PGOOD_Pin GPIO_PIN_15
#define FAN_PGOOD_GPIO_Port GPIOE
#define ACCEL_BRAKE_OK_Pin GPIO_PIN_12
#define ACCEL_BRAKE_OK_GPIO_Port GPIOB
#define nBRAKE_PRESS_3V3_Pin GPIO_PIN_13
#define nBRAKE_PRESS_3V3_GPIO_Port GPIOB
#define nHIGH_CURRENT_BSPD_Pin GPIO_PIN_14
#define nHIGH_CURRENT_BSPD_GPIO_Port GPIOB
#define BSPD_TEST_EN_Pin GPIO_PIN_9
#define BSPD_TEST_EN_GPIO_Port GPIOD
#define TS_ISENSE_OCSC_OK_3V3_Pin GPIO_PIN_13
#define TS_ISENSE_OCSC_OK_3V3_GPIO_Port GPIOD
#define SPI_CS_HS_Pin GPIO_PIN_14
#define SPI_CS_HS_GPIO_Port GPIOD
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define TSENSE_SEL0_Pin GPIO_PIN_5
#define TSENSE_SEL0_GPIO_Port GPIOD
#define TSENSE_SEL1_Pin GPIO_PIN_6
#define TSENSE_SEL1_GPIO_Port GPIOD
#define TSENSE_SEL2_Pin GPIO_PIN_7
#define TSENSE_SEL2_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define BMS_OK_Pin GPIO_PIN_0
#define BMS_OK_GPIO_Port GPIOE
#define nEVSE_I_LIM_GPIO_Pin GPIO_PIN_1
#define nEVSE_I_LIM_GPIO_GPIO_Port GPIOE

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
