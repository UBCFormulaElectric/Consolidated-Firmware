/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : app_freertos.c
 * Description        : FreeRTOS applicative file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_freertos.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bootloader.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for interfaceTask */
osThreadId_t         interfaceTaskHandle;
uint32_t             interfaceTaskBuffer[512];
osStaticThreadDef_t  interfaceTaskControlBlock;
const osThreadAttr_t interfaceTask_attributes = {
    .name       = "interfaceTask",
    .stack_mem  = &interfaceTaskBuffer[0],
    .stack_size = sizeof(interfaceTaskBuffer),
    .cb_mem     = &interfaceTaskControlBlock,
    .cb_size    = sizeof(interfaceTaskControlBlock),
    .priority   = (osPriority_t)osPriorityRealtime,
};
/* Definitions for canTxTask */
osThreadId_t         canTxTaskHandle;
uint32_t             canTxTaskBuffer[512];
osStaticThreadDef_t  canTxTaskControlBlock;
const osThreadAttr_t canTxTask_attributes = {
    .name       = "canTxTask",
    .stack_mem  = &canTxTaskBuffer[0],
    .stack_size = sizeof(canTxTaskBuffer),
    .cb_mem     = &canTxTaskControlBlock,
    .cb_size    = sizeof(canTxTaskControlBlock),
    .priority   = (osPriority_t)osPriorityHigh,
};
/* Definitions for tickTask */
osThreadId_t         tickTaskHandle;
uint32_t             tickTaskBuffer[512];
osStaticThreadDef_t  tickTaskControlBlock;
const osThreadAttr_t tickTask_attributes = {
    .name       = "tickTask",
    .stack_mem  = &tickTaskBuffer[0],
    .stack_size = sizeof(tickTaskBuffer),
    .cb_mem     = &tickTaskControlBlock,
    .cb_size    = sizeof(tickTaskControlBlock),
    .priority   = (osPriority_t)osPriorityBelowNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */
    /* creation of interfaceTask */
    interfaceTaskHandle = osThreadNew(runInterfaceTask, NULL, &interfaceTask_attributes);

    /* creation of canTxTask */
    canTxTaskHandle = osThreadNew(runCanTxTask, NULL, &canTxTask_attributes);

    /* creation of tickTask */
    tickTaskHandle = osThreadNew(runTickTask, NULL, &tickTask_attributes);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */
}
/* USER CODE BEGIN Header_runInterfaceTask */
/**
 * @brief Function implementing the interfaceTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_runInterfaceTask */
void runInterfaceTask(void *argument)
{
    /* USER CODE BEGIN interfaceTask */
    /* Infinite loop */
    bootloader_runInterfaceTask();
    /* USER CODE END interfaceTask */
}

/* USER CODE BEGIN Header_runCanTxTask */
/**
 * @brief Function implementing the canTxTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_runCanTxTask */
void runCanTxTask(void *argument)
{
    /* USER CODE BEGIN canTxTask */
    /* Infinite loop */
    bootloader_runCanTxTask();
    /* USER CODE END canTxTask */
}

/* USER CODE BEGIN Header_runTickTask */
/**
 * @brief Function implementing the tickTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_runTickTask */
void runTickTask(void *argument)
{
    /* USER CODE BEGIN tickTask */
    /* Infinite loop */
    bootloader_runTickTask();
    /* USER CODE END tickTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
