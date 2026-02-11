/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32h5xx_hal.h"

    /* Private includes ----------------------------------------------------------*/
    /* USER CODE BEGIN Includes */

    /* USER CODE END Includes */

    /* Exported types ------------------------------------------------------------*/
    /* USER CODE BEGIN ET */
    extern FDCAN_HandleTypeDef hfdcan1;
    extern IWDG_HandleTypeDef  hiwdg;
    extern RTC_HandleTypeDef   hrtc;
    extern SPI_HandleTypeDef   hspi1;
    extern PCD_HandleTypeDef   hpcd_USB_DRD_FS;
    extern ADC_HandleTypeDef   hadc1;
    extern DMA_NodeTypeDef     Node_GPDMA1_Channel0;
    extern DMA_QListTypeDef    List_GPDMA1_Channel0;
    extern DMA_HandleTypeDef   handle_GPDMA1_Channel0;
    extern TIM_HandleTypeDef   htim3;
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
#define MASTER_NSS_Pin GPIO_PIN_4
#define MASTER_NSS_GPIO_Port GPIOA
#define MASTER_CLK_Pin GPIO_PIN_5
#define MASTER_CLK_GPIO_Port GPIOA
#define MASTER_MISO_Pin GPIO_PIN_6
#define MASTER_MISO_GPIO_Port GPIOA
#define MASTER_MOSI_Pin GPIO_PIN_7
#define MASTER_MOSI_GPIO_Port GPIOA
#define DIAG_EN_Pin GPIO_PIN_14
#define DIAG_EN_GPIO_Port GPIOB
#define EFUSE_EN_Pin GPIO_PIN_15
#define EFUSE_EN_GPIO_Port GPIOB
#define SLAVE_CLK_Pin GPIO_PIN_9
#define SLAVE_CLK_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_4
#define LED_GPIO_Port GPIOB
#define BOOT_Pin GPIO_PIN_5
#define BOOT_GPIO_Port GPIOB

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
