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
#include "hw_error.h"
#include "cmsis_os.h"
    /* USER CODE END Includes */

    /* Exported types ------------------------------------------------------------*/
    /* USER CODE BEGIN ET */

    /* USER CODE END ET */

    /* Exported constants --------------------------------------------------------*/
    /* USER CODE BEGIN EC */
    extern ADC_HandleTypeDef  hadc1;
    extern DMA_HandleTypeDef  hdma_adc1;
    extern TIM_HandleTypeDef  htim2;
    extern TIM_HandleTypeDef  htim4;
    extern TIM_HandleTypeDef  htim7;
    extern CAN_HandleTypeDef  hcan2;
    extern I2C_HandleTypeDef  hi2c1;
    extern I2C_HandleTypeDef  hi2c3;
    extern IWDG_HandleTypeDef hiwdg;
    /* USER CODE END EC */

    /* Exported macro ------------------------------------------------------------*/
    /* USER CODE BEGIN EM */

    /* USER CODE END EM */

    /* Exported functions prototypes ---------------------------------------------*/
    void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IWDG_RESET_FREQUENCY 5
#define TIM4_PRESCALER 256
#define ADC_FREQUENCY 1000
#define TIM2_ARR TIMx_FREQUENCY / (TIM2_PRESCALER * TIM2_FREQUENCY)
#define TIM2_PRESCALER 4
#define TIM4_ARR TIMx_FREQUENCY / (TIM4_PRESCALER * TIM4_FREQUENCY)
#define TIM4_FREQUENCY 10
#define IWDG_PRESCALER 4
#define LSI_FREQUENCY 32000
#define TIM2_FREQUENCY 100
#define TIMx_FREQUENCY 96000000
#define BPS_R_3V3_Pin GPIO_PIN_2
#define BPS_R_3V3_GPIO_Port GPIOC
#define LC3_OUT_Pin GPIO_PIN_0
#define LC3_OUT_GPIO_Port GPIOA
#define RL_INT_3V3_SENS_Pin GPIO_PIN_4
#define RL_INT_3V3_SENS_GPIO_Port GPIOA
#define SUSP_TRAVEL_RL_3V3_Pin GPIO_PIN_5
#define SUSP_TRAVEL_RL_3V3_GPIO_Port GPIOA
#define SUSP_TRAVEL_RR_3V3_Pin GPIO_PIN_6
#define SUSP_TRAVEL_RR_3V3_GPIO_Port GPIOA
#define SUSP_TRAVEL_RL_OCSC_Pin GPIO_PIN_7
#define SUSP_TRAVEL_RL_OCSC_GPIO_Port GPIOA
#define SUSP_TRAVEL_RR_OCSC_Pin GPIO_PIN_4
#define SUSP_TRAVEL_RR_OCSC_GPIO_Port GPIOC
#define BOOT_Pin GPIO_PIN_0
#define BOOT_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_1
#define LED_GPIO_Port GPIOB
#define R_PUMP_SDA_Pin GPIO_PIN_9
#define R_PUMP_SDA_GPIO_Port GPIOC
#define R_PUMP_SCL_Pin GPIO_PIN_8
#define R_PUMP_SCL_GPIO_Port GPIOA
#define USB_D_N_Pin GPIO_PIN_11
#define USB_D_N_GPIO_Port GPIOA
#define USB_D_P_Pin GPIO_PIN_12
#define USB_D_P_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define IMU_INT1_Pin GPIO_PIN_11
#define IMU_INT1_GPIO_Port GPIOC
#define IMU_INT1_EXTI_IRQn EXTI15_10_IRQn
#define IMU_INT2_Pin GPIO_PIN_12
#define IMU_INT2_GPIO_Port GPIOC
#define IMU_INT2_EXTI_IRQn EXTI15_10_IRQn
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define BRAKE_LIGHT_EN_Pin GPIO_PIN_5
#define BRAKE_LIGHT_EN_GPIO_Port GPIOB
#define IMU_SCL_Pin GPIO_PIN_6
#define IMU_SCL_GPIO_Port GPIOB
#define IMU_SDA_Pin GPIO_PIN_7
#define IMU_SDA_GPIO_Port GPIOB
#define FLOW_METER_5V_Pin GPIO_PIN_8
#define FLOW_METER_5V_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
