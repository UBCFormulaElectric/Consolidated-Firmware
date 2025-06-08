/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tasks.h"
#include "hw_error.h"
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
FDCAN_HandleTypeDef hfdcan2;
FDCAN_HandleTypeDef hfdcan3;

/* Definitions for FDCanTx */
osThreadId_t         FDCanTxHandle;
uint32_t             FDCanTXBuffer[256];
osStaticThreadDef_t  FDCanTXControlBlock;
const osThreadAttr_t FDCanTx_attributes = {
    .name       = "FDCanTx",
    .cb_mem     = &FDCanTXControlBlock,
    .cb_size    = sizeof(FDCanTXControlBlock),
    .stack_mem  = &FDCanTXBuffer[0],
    .stack_size = sizeof(FDCanTXBuffer),
    .priority   = (osPriority_t)osPriorityNormal,
};
/* Definitions for CanRx */
osThreadId_t         CanRxHandle;
uint32_t             CanRxBuffer[512];
osStaticThreadDef_t  CanRxControlBlock;
const osThreadAttr_t CanRx_attributes = {
    .name       = "CanRx",
    .cb_mem     = &CanRxControlBlock,
    .cb_size    = sizeof(CanRxControlBlock),
    .stack_mem  = &CanRxBuffer[0],
    .stack_size = sizeof(CanRxBuffer),
    .priority   = (osPriority_t)osPriorityNormal,
};
/* Definitions for SxCanTxTask */
osThreadId_t         SxCanTxTaskHandle;
uint32_t             SxCanTxBuffer[256];
osStaticThreadDef_t  SxCanTxControlBlock;
const osThreadAttr_t SxCanTxTask_attributes = {
    .name       = "SxCanTxTask",
    .cb_mem     = &SxCanTxControlBlock,
    .cb_size    = sizeof(SxCanTxControlBlock),
    .stack_mem  = &SxCanTxBuffer[0],
    .stack_size = sizeof(SxCanTxBuffer),
    .priority   = (osPriority_t)osPriorityHigh,
};
/* Definitions for InvCanTxTask */
osThreadId_t         InvCanTxTaskHandle;
uint32_t             InvCanTxBuffer[256];
osStaticThreadDef_t  InvCanTxControlBlock;
const osThreadAttr_t InvCanTxTask_attributes = {
    .name       = "InvCanTxTask",
    .cb_mem     = &InvCanTxControlBlock,
    .cb_size    = sizeof(InvCanTxControlBlock),
    .stack_mem  = &InvCanTxBuffer[0],
    .stack_size = sizeof(InvCanTxBuffer),
    .priority   = (osPriority_t)osPriorityHigh,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void        SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_FDCAN1_Init(void);
static void MX_FDCAN2_Init(void);
static void MX_FDCAN3_Init(void);
void        StartFDCanTxTask(void *argument);
void        StartCanRxTask(void *argument);
void        StartSxCanTx(void *argument);
void        StartInvCanTx(void *argument);

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
    tasks_preInit();
    /* USER CODE END 1 */

    /* MPU Configuration--------------------------------------------------------*/
    MPU_Config();

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
    MX_FDCAN2_Init();
    MX_FDCAN3_Init();
    /* USER CODE BEGIN 2 */
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
    tasks_init();
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* creation of FDCanTx */
    FDCanTxHandle = osThreadNew(StartFDCanTxTask, NULL, &FDCanTx_attributes);

    /* creation of CanRx */
    CanRxHandle = osThreadNew(StartCanRxTask, NULL, &CanRx_attributes);

    /* creation of SxCanTxTask */
    SxCanTxTaskHandle = osThreadNew(StartSxCanTx, NULL, &SxCanTxTask_attributes);

    /* creation of InvCanTxTask */
    InvCanTxTaskHandle = osThreadNew(StartInvCanTx, NULL, &InvCanTxTask_attributes);

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
    RCC_OscInitStruct.PLL.PLLQ       = 2;
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
    hfdcan1.Init.FrameFormat          = FDCAN_FRAME_CLASSIC;
    hfdcan1.Init.Mode                 = FDCAN_MODE_NORMAL;
    hfdcan1.Init.AutoRetransmission   = DISABLE;
    hfdcan1.Init.TransmitPause        = DISABLE;
    hfdcan1.Init.ProtocolException    = DISABLE;
    hfdcan1.Init.NominalPrescaler     = 16;
    hfdcan1.Init.NominalSyncJumpWidth = 1;
    hfdcan1.Init.NominalTimeSeg1      = 2;
    hfdcan1.Init.NominalTimeSeg2      = 2;
    hfdcan1.Init.DataPrescaler        = 1;
    hfdcan1.Init.DataSyncJumpWidth    = 1;
    hfdcan1.Init.DataTimeSeg1         = 1;
    hfdcan1.Init.DataTimeSeg2         = 1;
    hfdcan1.Init.MessageRAMOffset     = 0;
    hfdcan1.Init.StdFiltersNbr        = 0;
    hfdcan1.Init.ExtFiltersNbr        = 0;
    hfdcan1.Init.RxFifo0ElmtsNbr      = 0;
    hfdcan1.Init.RxFifo0ElmtSize      = FDCAN_DATA_BYTES_8;
    hfdcan1.Init.RxFifo1ElmtsNbr      = 0;
    hfdcan1.Init.RxFifo1ElmtSize      = FDCAN_DATA_BYTES_8;
    hfdcan1.Init.RxBuffersNbr         = 0;
    hfdcan1.Init.RxBufferSize         = FDCAN_DATA_BYTES_8;
    hfdcan1.Init.TxEventsNbr          = 0;
    hfdcan1.Init.TxBuffersNbr         = 0;
    hfdcan1.Init.TxFifoQueueElmtsNbr  = 0;
    hfdcan1.Init.TxFifoQueueMode      = FDCAN_TX_FIFO_OPERATION;
    hfdcan1.Init.TxElmtSize           = FDCAN_DATA_BYTES_8;
    if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN FDCAN1_Init 2 */

    /* USER CODE END FDCAN1_Init 2 */
}

/**
 * @brief FDCAN2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_FDCAN2_Init(void)
{
    /* USER CODE BEGIN FDCAN2_Init 0 */

    /* USER CODE END FDCAN2_Init 0 */

    /* USER CODE BEGIN FDCAN2_Init 1 */

    /* USER CODE END FDCAN2_Init 1 */
    hfdcan2.Instance                  = FDCAN2;
    hfdcan2.Init.FrameFormat          = FDCAN_FRAME_CLASSIC;
    hfdcan2.Init.Mode                 = FDCAN_MODE_NORMAL;
    hfdcan2.Init.AutoRetransmission   = DISABLE;
    hfdcan2.Init.TransmitPause        = DISABLE;
    hfdcan2.Init.ProtocolException    = DISABLE;
    hfdcan2.Init.NominalPrescaler     = 16;
    hfdcan2.Init.NominalSyncJumpWidth = 1;
    hfdcan2.Init.NominalTimeSeg1      = 2;
    hfdcan2.Init.NominalTimeSeg2      = 2;
    hfdcan2.Init.DataPrescaler        = 1;
    hfdcan2.Init.DataSyncJumpWidth    = 1;
    hfdcan2.Init.DataTimeSeg1         = 1;
    hfdcan2.Init.DataTimeSeg2         = 1;
    hfdcan2.Init.MessageRAMOffset     = 0;
    hfdcan2.Init.StdFiltersNbr        = 0;
    hfdcan2.Init.ExtFiltersNbr        = 0;
    hfdcan2.Init.RxFifo0ElmtsNbr      = 0;
    hfdcan2.Init.RxFifo0ElmtSize      = FDCAN_DATA_BYTES_8;
    hfdcan2.Init.RxFifo1ElmtsNbr      = 0;
    hfdcan2.Init.RxFifo1ElmtSize      = FDCAN_DATA_BYTES_8;
    hfdcan2.Init.RxBuffersNbr         = 0;
    hfdcan2.Init.RxBufferSize         = FDCAN_DATA_BYTES_8;
    hfdcan2.Init.TxEventsNbr          = 0;
    hfdcan2.Init.TxBuffersNbr         = 0;
    hfdcan2.Init.TxFifoQueueElmtsNbr  = 0;
    hfdcan2.Init.TxFifoQueueMode      = FDCAN_TX_FIFO_OPERATION;
    hfdcan2.Init.TxElmtSize           = FDCAN_DATA_BYTES_8;
    if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN FDCAN2_Init 2 */

    /* USER CODE END FDCAN2_Init 2 */
}

/**
 * @brief FDCAN3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_FDCAN3_Init(void)
{
    /* USER CODE BEGIN FDCAN3_Init 0 */

    /* USER CODE END FDCAN3_Init 0 */

    /* USER CODE BEGIN FDCAN3_Init 1 */

    /* USER CODE END FDCAN3_Init 1 */
    hfdcan3.Instance                  = FDCAN3;
    hfdcan3.Init.FrameFormat          = FDCAN_FRAME_CLASSIC;
    hfdcan3.Init.Mode                 = FDCAN_MODE_NORMAL;
    hfdcan3.Init.AutoRetransmission   = DISABLE;
    hfdcan3.Init.TransmitPause        = DISABLE;
    hfdcan3.Init.ProtocolException    = DISABLE;
    hfdcan3.Init.NominalPrescaler     = 16;
    hfdcan3.Init.NominalSyncJumpWidth = 1;
    hfdcan3.Init.NominalTimeSeg1      = 2;
    hfdcan3.Init.NominalTimeSeg2      = 2;
    hfdcan3.Init.DataPrescaler        = 1;
    hfdcan3.Init.DataSyncJumpWidth    = 1;
    hfdcan3.Init.DataTimeSeg1         = 1;
    hfdcan3.Init.DataTimeSeg2         = 1;
    hfdcan3.Init.MessageRAMOffset     = 0;
    hfdcan3.Init.StdFiltersNbr        = 0;
    hfdcan3.Init.ExtFiltersNbr        = 0;
    hfdcan3.Init.RxFifo0ElmtsNbr      = 0;
    hfdcan3.Init.RxFifo0ElmtSize      = FDCAN_DATA_BYTES_8;
    hfdcan3.Init.RxFifo1ElmtsNbr      = 0;
    hfdcan3.Init.RxFifo1ElmtSize      = FDCAN_DATA_BYTES_8;
    hfdcan3.Init.RxBuffersNbr         = 0;
    hfdcan3.Init.RxBufferSize         = FDCAN_DATA_BYTES_8;
    hfdcan3.Init.TxEventsNbr          = 0;
    hfdcan3.Init.TxBuffersNbr         = 0;
    hfdcan3.Init.TxFifoQueueElmtsNbr  = 0;
    hfdcan3.Init.TxFifoQueueMode      = FDCAN_TX_FIFO_OPERATION;
    hfdcan3.Init.TxElmtSize           = FDCAN_DATA_BYTES_8;
    if (HAL_FDCAN_Init(&hfdcan3) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN FDCAN3_Init 2 */

    /* USER CODE END FDCAN3_Init 2 */
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
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(BOOT_GPIO_Port, BOOT_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : BOOT_Pin */
    GPIO_InitStruct.Pin   = BOOT_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BOOT_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : LED_Pin */
    GPIO_InitStruct.Pin   = LED_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

    /* USER CODE BEGIN MX_GPIO_Init_2 */
    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartFDCanTxTask */
/**
 * @brief  Function implementing the FDCanTx thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartFDCanTxTask */
void StartFDCanTxTask(void *argument)
{
    /* USER CODE BEGIN 5 */
    tasks_runCanFDTx();
    /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartCanRxTask */
/**
 * @brief Function implementing the CanRx thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartCanRxTask */
void StartCanRxTask(void *argument)
{
    /* USER CODE BEGIN StartCanRxTask */
    tasks_runcanRx();
    /* USER CODE END StartCanRxTask */
}

/* USER CODE BEGIN Header_StartSxCanTx */
/**
 * @brief Function implementing the SxCanTx thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSxCanTx */
void StartSxCanTx(void *argument)
{
    /* USER CODE BEGIN StartSxCanTx */
    /* Infinite loop */
    tasks_runCanSxTx();
    /* USER CODE END StartSxCanTx */
}

/* USER CODE BEGIN Header_StartInvCanTx */
/**
 * @brief Function implementing the InvCanTx thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartInvCanTx */
void StartInvCanTx(void *argument)
{
    /* USER CODE BEGIN StartInvCanTx */
    /* Infinite loop */
    tasks_runCanInvTx();
    /* USER CODE END StartInvCanTx */
}

/* MPU Configuration */

void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct = { 0 };

    /* Disables the MPU */
    HAL_MPU_Disable();

    /** Initializes and configures the Region and the memory to be protected
     */
    MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
    MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
    MPU_InitStruct.BaseAddress      = 0x0;
    MPU_InitStruct.Size             = MPU_REGION_SIZE_4GB;
    MPU_InitStruct.SubRegionDisable = 0x87;
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
    MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
    MPU_InitStruct.IsShareable      = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    /* Enables the MPU */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM6)
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
