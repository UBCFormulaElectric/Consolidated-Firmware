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
#include "cmsis_os.h"
    /* USER CODE END Includes */

    /* Exported types ------------------------------------------------------------*/
    /* USER CODE BEGIN ET */

    /* USER CODE END ET */

    /* Exported constants --------------------------------------------------------*/
    /* USER CODE BEGIN EC */
    // extern ADC_HandleTypeDef hadc1;
    // extern ADC_HandleTypeDef hadc3;
    extern DMA_HandleTypeDef hdma_adc1;
    extern DMA_HandleTypeDef hdma_adc3;
    // extern CRC_HandleTypeDef   hcrc;
    extern FDCAN_HandleTypeDef hfdcan1;
    // extern FDCAN_HandleTypeDef hfdcan2;
    // extern SD_HandleTypeDef    hsd1;
    extern SPI_HandleTypeDef hspi2;
    extern TIM_HandleTypeDef htim1;
    extern TIM_HandleTypeDef htim3;
    extern TIM_HandleTypeDef htim5;
    // extern PCD_HandleTypeDef   hpcd_USB_OTG_HS;
    // extern IWDG_HandleTypeDef  hiwdg1;

    extern osThreadId_t Task1HzHandle;
    extern osThreadId_t Task100HzHandle;
    extern osThreadId_t Task1kHzHandle;
    extern osThreadId_t TaskCanRxHandle;
    extern osThreadId_t TaskCanTxHandle;

    extern const osThreadAttr_t Task100Hz_attributes;
    extern const osThreadAttr_t TaskCanRx_attributes;
    extern const osThreadAttr_t TaskCanTx_attributes;
    extern const osThreadAttr_t Task1kHz_attributes;
    extern const osThreadAttr_t Task1Hz_attributes;
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
#define SPI_CS_LS_Pin GPIO_PIN_12
#define SPI_CS_LS_GPIO_Port GPIOB
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
