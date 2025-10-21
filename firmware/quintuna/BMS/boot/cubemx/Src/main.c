/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hw/fdcan.h"
#include "hw/error.h"
#include "io/canQueue.h"

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

FDCAN_HandleTypeDef hfdcan1;

/* Definitions for interfaceTask */
osThreadId_t         interfaceTaskHandle;
uint32_t             interfaceTaskBuffer[512];
osStaticThreadDef_t  interfaceTaskControlBlock;
const osThreadAttr_t interfaceTask_attributes = {
    .name       = "interfaceTask",
    .cb_mem     = &interfaceTaskControlBlock,
    .cb_size    = sizeof(interfaceTaskControlBlock),
    .stack_mem  = &interfaceTaskBuffer[0],
    .stack_size = sizeof(interfaceTaskBuffer),
    .priority   = (osPriority_t)osPriorityRealtime,
};
/* Definitions for canTxTask */
osThreadId_t         canTxTaskHandle;
uint32_t             canTxTaskBuffer[512];
osStaticThreadDef_t  canTxTaskControlBlock;
const osThreadAttr_t canTxTask_attributes = {
    .name       = "canTxTask",
    .cb_mem     = &canTxTaskControlBlock,
    .cb_size    = sizeof(canTxTaskControlBlock),
    .stack_mem  = &canTxTaskBuffer[0],
    .stack_size = sizeof(canTxTaskBuffer),
    .priority   = (osPriority_t)osPriorityHigh,
};
/* Definitions for tickTask */
osThreadId_t         tickTaskHandle;
uint32_t             tickTaskBuffer[512];
osStaticThreadDef_t  tickTaskControlBlock;
const osThreadAttr_t tickTask_attributes = {
    .name       = "tickTask",
    .cb_mem     = &tickTaskControlBlock,
    .cb_size    = sizeof(tickTaskControlBlock),
    .stack_mem  = &tickTaskBuffer[0],
    .stack_size = sizeof(tickTaskBuffer),
    .priority   = (osPriority_t)osPriorityBelowNormal,
};
/* USER CODE BEGIN PV */
CanHandle can = { .bus_num = 0, .hcan = &hfdcan1, .receive_callback = io_canQueue_pushRx, .ready = false };
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void        SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FDCAN1_Init(void);
void        runInterfaceTask(void *argument);
void        runCanTxTask(void *argument);
void        runTickTask(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */
    bootloader_preInit();
    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_FDCAN1_Init();
    /* USER CODE BEGIN 2 */
    bootloader_init();
    /* USER CODE END 2 */

    /* Init scheduler */
    osKernelInitialize();

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

    /* Create the thread(s) */
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

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

    /** Supply configuration update enable
     */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

    /** Configure the main internal regulator output voltage
     */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
    {
    }

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = 1;
    RCC_OscInitStruct.PLL.PLLN       = 64;
    RCC_OscInitStruct.PLL.PLLP       = 1;
    RCC_OscInitStruct.PLL.PLLQ       = 4;
    RCC_OscInitStruct.PLL.PLLR       = 2;
    RCC_OscInitStruct.PLL.PLLRGE     = RCC_PLL1VCIRANGE_3;
    RCC_OscInitStruct.PLL.PLLVCOSEL  = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN   = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                  RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief FDCAN1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_FDCAN1_Init(void)
{
    /* USER CODE BEGIN FDCAN1_Init 0 */

    /* USER CODE END FDCAN1_Init 0 */

    /* USER CODE BEGIN FDCAN1_Init 1 */

    /* USER CODE END FDCAN1_Init 1 */
    hfdcan1.Instance                  = FDCAN1;
    hfdcan1.Init.FrameFormat          = FDCAN_FRAME_FD_NO_BRS;
    hfdcan1.Init.Mode                 = FDCAN_MODE_NORMAL;
    hfdcan1.Init.AutoRetransmission   = ENABLE;
    hfdcan1.Init.TransmitPause        = DISABLE;
    hfdcan1.Init.ProtocolException    = DISABLE;
    hfdcan1.Init.NominalPrescaler     = 6;
    hfdcan1.Init.NominalSyncJumpWidth = 2;
    hfdcan1.Init.NominalTimeSeg1      = 13;
    hfdcan1.Init.NominalTimeSeg2      = 2;
    hfdcan1.Init.DataPrescaler        = 1;
    hfdcan1.Init.DataSyncJumpWidth    = 6;
    hfdcan1.Init.DataTimeSeg1         = 17;
    hfdcan1.Init.DataTimeSeg2         = 6;
    hfdcan1.Init.MessageRAMOffset     = 0;
    hfdcan1.Init.StdFiltersNbr        = 1;
    hfdcan1.Init.ExtFiltersNbr        = 1;
    hfdcan1.Init.RxFifo0ElmtsNbr      = 32;
    hfdcan1.Init.RxFifo0ElmtSize      = FDCAN_DATA_BYTES_64;
    hfdcan1.Init.RxFifo1ElmtsNbr      = 0;
    hfdcan1.Init.RxFifo1ElmtSize      = FDCAN_DATA_BYTES_64;
    hfdcan1.Init.RxBuffersNbr         = 0;
    hfdcan1.Init.RxBufferSize         = FDCAN_DATA_BYTES_64;
    hfdcan1.Init.TxEventsNbr          = 0;
    hfdcan1.Init.TxBuffersNbr         = 0;
    hfdcan1.Init.TxFifoQueueElmtsNbr  = 32;
    hfdcan1.Init.TxFifoQueueMode      = FDCAN_TX_FIFO_OPERATION;
    hfdcan1.Init.TxElmtSize           = FDCAN_DATA_BYTES_64;
    if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN FDCAN1_Init 2 */

    /* USER CODE END FDCAN1_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    /* USER CODE BEGIN MX_GPIO_Init_1 */
    /* USER CODE END MX_GPIO_Init_1 */

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(
        GPIOE, IR_P_EN_Pin | SHDN_EN_Pin | PRE_CHARGE_EN_Pin | FB_STBY_SHDN_Pin | FAN_EN_Pin | BMS_OK_Pin,
        GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(SPI_CS_LS_GPIO_Port, SPI_CS_LS_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LEDB_GPIO_Port, LEDB_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, BSPD_TEST_EN_Pin | TSENSE_SEL0_Pin | TSENSE_SEL1_Pin | TSENSE_SEL2_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(SPI_CS_HS_GPIO_Port, SPI_CS_HS_Pin, GPIO_PIN_SET);

    /*Configure GPIO pins : IR_P_EN_Pin SHDN_EN_Pin FB_STBY_SHDN_Pin BMS_OK_Pin */
    GPIO_InitStruct.Pin   = IR_P_EN_Pin | SHDN_EN_Pin | FB_STBY_SHDN_Pin | BMS_OK_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : LED_Pin SPI_CS_LS_Pin */
    GPIO_InitStruct.Pin   = LED_Pin | SPI_CS_LS_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : MSD_SHDN_SNS_Pin HV_P_INTLCK_SNS_Pin HV_N_INTLCK_SNS_Pin IMD_LATCH_Pin
                             DIAG_Pin */
    GPIO_InitStruct.Pin  = MSD_SHDN_SNS_Pin | HV_P_INTLCK_SNS_Pin | HV_N_INTLCK_SNS_Pin | IMD_LATCH_Pin | DIAG_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : LEDB_Pin */
    GPIO_InitStruct.Pin   = LEDB_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LEDB_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : BSPD_OK_Pin ACCEL_BRAKE_OK_Pin nBRAKE_PRESS_3V3_Pin nHIGH_CURRENT_BSPD_Pin */
    GPIO_InitStruct.Pin  = BSPD_OK_Pin | ACCEL_BRAKE_OK_Pin | nBRAKE_PRESS_3V3_Pin | nHIGH_CURRENT_BSPD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : BMS_LATCH_Pin IMD_OK_Pin BSPD_LATCH_Pin FAN_PGOOD_Pin
                             nEVSE_I_LIM_GPIO_Pin */
    GPIO_InitStruct.Pin  = BMS_LATCH_Pin | IMD_OK_Pin | BSPD_LATCH_Pin | FAN_PGOOD_Pin | nEVSE_I_LIM_GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pin : PRE_CHARGE_EN_Pin */
    GPIO_InitStruct.Pin   = PRE_CHARGE_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(PRE_CHARGE_EN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : FAN_EN_Pin */
    GPIO_InitStruct.Pin   = FAN_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(FAN_EN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : BSPD_TEST_EN_Pin SPI_CS_HS_Pin TSENSE_SEL0_Pin TSENSE_SEL1_Pin
                             TSENSE_SEL2_Pin */
    GPIO_InitStruct.Pin   = BSPD_TEST_EN_Pin | SPI_CS_HS_Pin | TSENSE_SEL0_Pin | TSENSE_SEL1_Pin | TSENSE_SEL2_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pin : TS_ISENSE_OCSC_OK_3V3_Pin */
    GPIO_InitStruct.Pin  = TS_ISENSE_OCSC_OK_3V3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(TS_ISENSE_OCSC_OK_3V3_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : PB5 PB6 */
    GPIO_InitStruct.Pin       = GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USER CODE BEGIN MX_GPIO_Init_2 */
    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_runInterfaceTask */
/**
 * @brief  Function implementing the interfaceTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_runInterfaceTask */
void runInterfaceTask(void *argument)
{
    /* USER CODE BEGIN 5 */
    /* Infinite loop */
    bootloader_runInterfaceTask();
    /* USER CODE END 5 */
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
    /* USER CODE BEGIN runCanTxTask */
    /* Infinite loop */
    bootloader_runCanTxTask();
    /* USER CODE END runCanTxTask */
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
    /* USER CODE BEGIN runTickTask */
    /* Infinite loop */
    bootloader_runTickTask();
    /* USER CODE END runTickTask */
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM2 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM2)
    {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
