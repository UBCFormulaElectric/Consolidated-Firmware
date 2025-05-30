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
    extern ADC_HandleTypeDef  hadc1;
    extern DMA_HandleTypeDef  hdma_adc1;
    extern CAN_HandleTypeDef  hcan1;
    extern IWDG_HandleTypeDef hiwdg;
    extern TIM_HandleTypeDef  htim3;
    extern TIM_HandleTypeDef  htim12;
    extern UART_HandleTypeDef huart1;

    extern osThreadId_t Task1HzHandle;
    extern osThreadId_t Task100HzHandle;
    extern osThreadId_t Task1kHzHandle;
    extern osThreadId_t TaskCanRxHandle;
    extern osThreadId_t TaskCanTxHandle;

    extern const osThreadAttr_t Task1Hz_attributes;
    extern const osThreadAttr_t Task100Hz_attributes;
    extern const osThreadAttr_t Task1kHz_attributes;
    extern const osThreadAttr_t TaskCanRx_attributes;
    extern const osThreadAttr_t TaskCanTx_attributes;
    /* USER CODE END EC */

    /* Exported macro ------------------------------------------------------------*/
    /* USER CODE BEGIN EM */

    /* USER CODE END EM */

    /* Exported functions prototypes ---------------------------------------------*/
    void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TASK_100HZ_STACK_SIZE 512
#define TASK_CANRX_STACK_SIZE 512
#define TASK_CANTX_STACK_SIZE 512
#define TIM3_PRESCALER 8
#define TIM12_PWM_MINIMUM_FREQUENCY 1
#define IWDG_RESET_FREQUENCY 5
#define ADC_FREQUENCY 1000
#define TIMx_FREQUENCY 96000000
#define TIM12_PRESCALER (TIMx_FREQUENCY / TIM12_AUTO_RELOAD_REG / TIM12_PWM_MINIMUM_FREQUENCY)
#define TASK_1HZ_STACK_SIZE 512
#define TASK_1KHZ_STACK_SIZE 512
#define TIM12_AUTO_RELOAD_REG 0xFFFF
#define IWDG_PRESCALER 4
#define IWDG_WINDOW_DISABLE_VALUE 4095
#define LSI_FREQUENCY 32000
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define SteeringAngle_3V3_Pin GPIO_PIN_1
#define SteeringAngle_3V3_GPIO_Port GPIOC
#define APPS1_3V3_Pin GPIO_PIN_2
#define APPS1_3V3_GPIO_Port GPIOC
#define LOAD_CELL_1_3V3_Pin GPIO_PIN_3
#define LOAD_CELL_1_3V3_GPIO_Port GPIOC
#define LOAD_CELL_2_3V3_Pin GPIO_PIN_1
#define LOAD_CELL_2_3V3_GPIO_Port GPIOA
#define BRAKE_OCSC_OK_3V3_Pin GPIO_PIN_2
#define BRAKE_OCSC_OK_3V3_GPIO_Port GPIOA
#define APPS2_3V3_Pin GPIO_PIN_5
#define APPS2_3V3_GPIO_Port GPIOA
#define BPS_F_3V3_Pin GPIO_PIN_7
#define BPS_F_3V3_GPIO_Port GPIOA
#define NBSPD_BRAKE_PRESSED_3V3_Pin GPIO_PIN_4
#define NBSPD_BRAKE_PRESSED_3V3_GPIO_Port GPIOC
#define BPS_B_3V3_Pin GPIO_PIN_5
#define BPS_B_3V3_GPIO_Port GPIOC
#define SUSP_TRAVEL_FL_3V3_Pin GPIO_PIN_0
#define SUSP_TRAVEL_FL_3V3_GPIO_Port GPIOB
#define SUSP_TRAVEL_FR_3V3_Pin GPIO_PIN_1
#define SUSP_TRAVEL_FR_3V3_GPIO_Port GPIOB
#define WheelSpeed_L_5V_Pin GPIO_PIN_14
#define WheelSpeed_L_5V_GPIO_Port GPIOB
#define WheelSpeed_R_5V_Pin GPIO_PIN_15
#define WheelSpeed_R_5V_GPIO_Port GPIOB
#define FSM_SHDN_Pin GPIO_PIN_6
#define FSM_SHDN_GPIO_Port GPIOC
#define UART4_RX_Pin GPIO_PIN_9
#define UART4_RX_GPIO_Port GPIOA
#define UART4_TX_Pin GPIO_PIN_10
#define UART4_TX_GPIO_Port GPIOA
#define NPROGRAM_3V3_Pin GPIO_PIN_15
#define NPROGRAM_3V3_GPIO_Port GPIOA
#define NCHIMERA_Pin GPIO_PIN_6
#define NCHIMERA_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
