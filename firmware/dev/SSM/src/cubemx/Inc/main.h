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
    extern SPI_HandleTypeDef hspi1;
    extern SPI_HandleTypeDef hspi2;
    /* USER CODE END EC */

    /* Exported macro ------------------------------------------------------------*/
    /* USER CODE BEGIN EM */

    /* USER CODE END EM */

    /* Exported functions prototypes ---------------------------------------------*/
    void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CS_LS_Pin GPIO_PIN_13
#define CS_LS_GPIO_Port GPIOC
#define CS_HS_Pin GPIO_PIN_1
#define CS_HS_GPIO_Port GPIOC
#define isoSPI_MISO_Pin GPIO_PIN_2
#define isoSPI_MISO_GPIO_Port GPIOC
#define isoSPI_MOSI_Pin GPIO_PIN_3
#define isoSPI_MOSI_GPIO_Port GPIOC
#define INT3_Pin GPIO_PIN_1
#define INT3_GPIO_Port GPIOA
#define INT4_Pin GPIO_PIN_2
#define INT4_GPIO_Port GPIOA
#define nCLR_Pin GPIO_PIN_3
#define nCLR_GPIO_Port GPIOA
#define DAC_SCLK_Pin GPIO_PIN_5
#define DAC_SCLK_GPIO_Port GPIOA
#define DAC_MISO_Pin GPIO_PIN_6
#define DAC_MISO_GPIO_Port GPIOA
#define DAC_MOSI_Pin GPIO_PIN_7
#define DAC_MOSI_GPIO_Port GPIOA
#define INT1_Pin GPIO_PIN_1
#define INT1_GPIO_Port GPIOB
#define INT2_Pin GPIO_PIN_2
#define INT2_GPIO_Port GPIOB
#define isoSPI_SCLK_Pin GPIO_PIN_10
#define isoSPI_SCLK_GPIO_Port GPIOB
#define DOUT3_Pin GPIO_PIN_6
#define DOUT3_GPIO_Port GPIOC
#define DOUT4_Pin GPIO_PIN_7
#define DOUT4_GPIO_Port GPIOC
#define Boot_LED_Pin GPIO_PIN_8
#define Boot_LED_GPIO_Port GPIOC
#define Debug_LED_Pin GPIO_PIN_9
#define Debug_LED_GPIO_Port GPIOC
#define DOUT1_Pin GPIO_PIN_8
#define DOUT1_GPIO_Port GPIOA
#define DOUT2_Pin GPIO_PIN_9
#define DOUT2_GPIO_Port GPIOA
#define USB_D__Pin GPIO_PIN_11
#define USB_D__GPIO_Port GPIOA
#define USB_D_A12_Pin GPIO_PIN_12
#define USB_D_A12_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define INDICATOR1_Pin GPIO_PIN_10
#define INDICATOR1_GPIO_Port GPIOC
#define INDICATOR2_Pin GPIO_PIN_11
#define INDICATOR2_GPIO_Port GPIOC
#define INDICATOR3_Pin GPIO_PIN_12
#define INDICATOR3_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define CS_LD_Pin GPIO_PIN_4
#define CS_LD_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
