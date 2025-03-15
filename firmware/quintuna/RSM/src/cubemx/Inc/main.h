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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
    /* USER CODE END Includes */

    /* Exported types ------------------------------------------------------------*/
    /* USER CODE BEGIN ET */

    /* USER CODE END ET */

    /* Exported constants --------------------------------------------------------*/
    /* USER CODE BEGIN EC */
    extern ADC_HandleTypeDef hadc1;
    extern DMA_HandleTypeDef hdma_adc1;
    extern TIM_HandleTypeDef htim3;
    extern TIM_HandleTypeDef htim4;
    extern CAN_HandleTypeDef hcan2;
    extern I2C_HandleTypeDef hi2c1;
    extern I2C_HandleTypeDef hi2c3;
    extern CAN_HandleTypeDef hcan2;
    /* USER CODE END EC */

    /* Exported macro ------------------------------------------------------------*/
    /* USER CODE BEGIN EM */

    /* USER CODE END EM */

    void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

    /* Exported functions prototypes ---------------------------------------------*/
    void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IWDG_RESET_FREQUENCY 5
#define TIM4_PRESCALER (TIMx_FREQUENCY / TIM4_AUTO_RELOAD_REG / TIM4_PWM_MINIMUM_FREQUENCY)
#define TIM4_AUTO_RELOAD_REG 0xFFFF
#define ADC_FREQUENCY 1000
#define TIM4_PWM_MINIMUM_FREQUENCY 1
#define TIMx_FREQUENCY 96000000
#define TIM3_PRESCALER 8
#define BPS_R_3V3_Pin GPIO_PIN_2
#define BPS_R_3V3_GPIO_Port GPIOC
#define SUSP_TRAVEL_RR_OCSC_Pin GPIO_PIN_4
#define SUSP_TRAVEL_RR_OCSC_GPIO_Port GPIOC
#define BOOT_Pin GPIO_PIN_0
#define BOOT_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_1
#define LED_GPIO_Port GPIOB
#define R_PUMP_SDA_Pin GPIO_PIN_9
#define R_PUMP_SDA_GPIO_Port GPIOC
#define IMU_INT1_Pin GPIO_PIN_11
#define IMU_INT1_GPIO_Port GPIOC
#define IMU_INT2_Pin GPIO_PIN_12
#define IMU_INT2_GPIO_Port GPIOC
#define LC3_OUT_Pin GPIO_PIN_7
#define LC3_OUT_GPIO_Port GPIOB
#define FLOW_METER_5V_Pin GPIO_PIN_8
#define FLOW_METER_5V_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
