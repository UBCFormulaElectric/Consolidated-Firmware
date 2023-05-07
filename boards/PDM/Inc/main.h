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
extern "C" {
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
#define LSI_FREQUENCY 32000
#define IWDG_PRESCALER 4
#define IWDG_RESET_FREQUENCY 5
#define TIMx_FREQUENCY 96000000
#define TIM3_PRESCALER 96
#define ADC_FREQUENCY 1000
#define SENSE_VBAT_Pin GPIO_PIN_0
#define SENSE_VBAT_GPIO_Port GPIOC
#define SENSE_24V_ACC_Pin GPIO_PIN_1
#define SENSE_24V_ACC_GPIO_Port GPIOC
#define SENSE_22V_BOOST_Pin GPIO_PIN_2
#define SENSE_22V_BOOST_GPIO_Port GPIOC
#define EN_DI_RHS_Pin GPIO_PIN_1
#define EN_DI_RHS_GPIO_Port GPIOA
#define FR_STBY_DIS_Pin GPIO_PIN_2
#define FR_STBY_DIS_GPIO_Port GPIOA
#define EN_DI_LHS_Pin GPIO_PIN_3
#define EN_DI_LHS_GPIO_Port GPIOA
#define CUR_SNS_DI_RHS_Pin GPIO_PIN_4
#define CUR_SNS_DI_RHS_GPIO_Port GPIOA
#define CUR_SNS_DI_LHS_Pin GPIO_PIN_5
#define CUR_SNS_DI_LHS_GPIO_Port GPIOA
#define CUR_SNS_FAN_Pin GPIO_PIN_6
#define CUR_SNS_FAN_GPIO_Port GPIOA
#define CUR_SNS_DRS_Pin GPIO_PIN_7
#define CUR_SNS_DRS_GPIO_Port GPIOA
#define CUR_SNS_AUX_Pin GPIO_PIN_4
#define CUR_SNS_AUX_GPIO_Port GPIOC
#define CUR_SNS_EMETER_Pin GPIO_PIN_5
#define CUR_SNS_EMETER_GPIO_Port GPIOC
#define CUR_SNS_LVPWR_Pin GPIO_PIN_0
#define CUR_SNS_LVPWR_GPIO_Port GPIOB
#define CUR_SNS_AIR_Pin GPIO_PIN_1
#define CUR_SNS_AIR_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_2
#define LED_GPIO_Port GPIOB
#define EN_AIR_Pin GPIO_PIN_12
#define EN_AIR_GPIO_Port GPIOB
#define FR_STBY_AIR_LVPWR_Pin GPIO_PIN_13
#define FR_STBY_AIR_LVPWR_GPIO_Port GPIOB
#define EN_LVPWR_Pin GPIO_PIN_14
#define EN_LVPWR_GPIO_Port GPIOB
#define EN_EMETER_Pin GPIO_PIN_15
#define EN_EMETER_GPIO_Port GPIOB
#define FR_STBY_EMETER_AUX_Pin GPIO_PIN_6
#define FR_STBY_EMETER_AUX_GPIO_Port GPIOC
#define EN_AUX_Pin GPIO_PIN_7
#define EN_AUX_GPIO_Port GPIOC
#define EN_DRS_Pin GPIO_PIN_8
#define EN_DRS_GPIO_Port GPIOC
#define FR_STBY_DRS_FAN_Pin GPIO_PIN_9
#define FR_STBY_DRS_FAN_GPIO_Port GPIOC
#define EN_FAN_Pin GPIO_PIN_8
#define EN_FAN_GPIO_Port GPIOA
#define N_CHRG_FAULT_Pin GPIO_PIN_15
#define N_CHRG_FAULT_GPIO_Port GPIOA
#define N_CHRG_Pin GPIO_PIN_10
#define N_CHRG_GPIO_Port GPIOC
#define PGOOD_Pin GPIO_PIN_11
#define PGOOD_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
