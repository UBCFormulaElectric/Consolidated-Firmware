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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tasks.h"
#include "hw_error.h"
#include "io_log.h"
#include "hw_sd.h"
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

CRC_HandleTypeDef hcrc;

FDCAN_HandleTypeDef hfdcan2;

I2C_HandleTypeDef hi2c1;

IWDG_HandleTypeDef hiwdg1;

SD_HandleTypeDef hsd1;

TIM_HandleTypeDef htim15;

UART_HandleTypeDef huart2;

/* Definitions for Task100Hz */
osThreadId_t         Task100HzHandle;
uint32_t             Task100HzBuffer[8096];
osStaticThreadDef_t  Task100HzControlBlock;
const osThreadAttr_t Task100Hz_attributes = {
    .name       = "Task100Hz",
    .cb_mem     = &Task100HzControlBlock,
    .cb_size    = sizeof(Task100HzControlBlock),
    .stack_mem  = &Task100HzBuffer[0],
    .stack_size = sizeof(Task100HzBuffer),
    .priority   = (osPriority_t)osPriorityHigh,
};
/* Definitions for TaskCanTx */
osThreadId_t         TaskCanTxHandle;
uint32_t             canTxTaskBuffer[512];
osStaticThreadDef_t  canTxTaskControlBlock;
const osThreadAttr_t TaskCanTx_attributes = {
    .name       = "TaskCanTx",
    .cb_mem     = &canTxTaskControlBlock,
    .cb_size    = sizeof(canTxTaskControlBlock),
    .stack_mem  = &canTxTaskBuffer[0],
    .stack_size = sizeof(canTxTaskBuffer),
    .priority   = (osPriority_t)osPriorityBelowNormal,
};
/* Definitions for TaskCanRx */
osThreadId_t         TaskCanRxHandle;
uint32_t             canRxTaskBuffer[512];
osStaticThreadDef_t  canRxTaskControlBlock;
const osThreadAttr_t TaskCanRx_attributes = {
    .name       = "TaskCanRx",
    .cb_mem     = &canRxTaskControlBlock,
    .cb_size    = sizeof(canRxTaskControlBlock),
    .stack_mem  = &canRxTaskBuffer[0],
    .stack_size = sizeof(canRxTaskBuffer),
    .priority   = (osPriority_t)osPriorityBelowNormal,
};
/* Definitions for Task1kHz */
osThreadId_t         Task1kHzHandle;
uint32_t             Task1kHzBuffer[512];
osStaticThreadDef_t  Task1kHzControlBlock;
const osThreadAttr_t Task1kHz_attributes = {
    .name       = "Task1kHz",
    .cb_mem     = &Task1kHzControlBlock,
    .cb_size    = sizeof(Task1kHzControlBlock),
    .stack_mem  = &Task1kHzBuffer[0],
    .stack_size = sizeof(Task1kHzBuffer),
    .priority   = (osPriority_t)osPriorityRealtime,
};
/* Definitions for Task1Hz */
osThreadId_t         Task1HzHandle;
uint32_t             Task1HzBuffer[512];
osStaticThreadDef_t  Task1HzControlBlock;
const osThreadAttr_t Task1Hz_attributes = {
    .name       = "Task1Hz",
    .cb_mem     = &Task1HzControlBlock,
    .cb_size    = sizeof(Task1HzControlBlock),
    .stack_mem  = &Task1HzBuffer[0],
    .stack_size = sizeof(Task1HzBuffer),
    .priority   = (osPriority_t)osPriorityAboveNormal,
};
/* Definitions for TaskLogging */
osThreadId_t         TaskLoggingHandle;
uint32_t             TaskLoggingBuffer[1024];
osStaticThreadDef_t  TaskLoggingControlBlock;
const osThreadAttr_t TaskLogging_attributes = {
    .name       = "TaskLogging",
    .cb_mem     = &TaskLoggingControlBlock,
    .cb_size    = sizeof(TaskLoggingControlBlock),
    .stack_mem  = &TaskLoggingBuffer[0],
    .stack_size = sizeof(TaskLoggingBuffer),
    .priority   = (osPriority_t)osPriorityLow,
};
/* Definitions for TaskTelem */
osThreadId_t         TaskTelemHandle;
uint32_t             TaskTelemBuffer[512];
osStaticThreadDef_t  TaskTelemControlBlock;
const osThreadAttr_t TaskTelem_attributes = {
    .name       = "TaskTelem",
    .cb_mem     = &TaskTelemControlBlock,
    .cb_size    = sizeof(TaskTelemControlBlock),
    .stack_mem  = &TaskTelemBuffer[0],
    .stack_size = sizeof(TaskTelemBuffer),
    .priority   = (osPriority_t)osPriorityNormal,
};
/* Definitions for TaskTelemRx */
osThreadId_t         TaskTelemRxHandle;
uint32_t             TaskTelemRxBuffer[512];
osStaticThreadDef_t  TaskTelemRxControlBlock;
const osThreadAttr_t TaskTelemRx_attributes = {
    .name       = "TaskTelemRx",
    .cb_mem     = &TaskTelemRxControlBlock,
    .cb_size    = sizeof(TaskTelemRxControlBlock),
    .stack_mem  = &TaskTelemRxBuffer[0],
    .stack_size = sizeof(TaskTelemRxBuffer),
    .priority   = (osPriority_t)osPriorityLow,
};

/* Definitions for chimera */
osThreadId_t         TaskChimeraHandle;
uint32_t             TaskChimeraBuffer[512];
osStaticThreadDef_t  TaskChimeraControlBlock;
const osThreadAttr_t TaskChimera_attributes = {
    .name       = "TaskChimera",
    .cb_mem     = &TaskChimeraControlBlock,
    .cb_size    = sizeof(TaskChimeraControlBlock),
    .stack_mem  = &TaskChimeraBuffer[0],
    .stack_size = sizeof(TaskChimeraBuffer),
    .priority   = (osPriority_t)osPriorityAboveNormal,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void        SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SDMMC1_SD_Init(void);
static void MX_FDCAN2_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM15_Init(void);
static void MX_IWDG1_Init(void);
static void MX_CRC_Init(void);
void        RunChimera(void *argument);
void        RunTask100Hz(void *argument);
void        RunCanTxTask(void *argument);
void        RunCanRxTask(void *argument);
void        RunTask1kHz(void *argument);
void        RunTask1Hz(void *argument);
void        RunTaskLogging(void *argument);
void        RunTaskTelem(void *argument);
void        RunTaskTelemRx(void *argument);

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
    MX_SDMMC1_SD_Init();
    MX_FDCAN2_Init();
    MX_USART2_UART_Init();
    MX_I2C1_Init();
    MX_TIM15_Init();
    MX_IWDG1_Init();
    MX_CRC_Init();
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

    tasks_init();
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* creation of Task100Hz */
    // Task100HzHandle = osThreadNew(RunTask100Hz, NULL, &Task100Hz_attributes);
    TaskChimeraHandle = osThreadNew(RunChimera, NULL, &TaskChimera_attributes); // High priority level

    // const CanMsg dummyRadioMsg =
    // {
    //     .std_id = 1,
    //     .dlc = 64,
    //     .timestamp = 1,
    //     .data.data8 = {0},
    //     .bus = 1,
    //     .is_fd = false
    // };

    // for (int i = 0; i < 10; i++)
    // {
    //     bool status = io_telemMessageQueue_pushTx(&dummyRadioMsg);
    //     assert(status);
    //}

    /* creation of TaskCanTx */
    TaskCanTxHandle = osThreadNew(RunCanTxTask, NULL, &TaskCanTx_attributes);

    /* creation of TaskCanRx */
    TaskCanRxHandle = osThreadNew(RunCanRxTask, NULL, &TaskCanRx_attributes);

    /* creation of Task1kHz */
    Task1kHzHandle = osThreadNew(RunTask1kHz, NULL, &Task1kHz_attributes);

    /* creation of Task1Hz */
    Task1HzHandle = osThreadNew(RunTask1Hz, NULL, &Task1Hz_attributes);

    /* creation of TaskLogging */
    TaskLoggingHandle = osThreadNew(RunTaskLogging, NULL, &TaskLogging_attributes);

    /* creation of TaskTelem */
    TaskTelemHandle = osThreadNew(RunTaskTelem, NULL, &TaskTelem_attributes);

    /* creation of TaskTelemRx */
    TaskTelemRxHandle = osThreadNew(RunTaskTelemRx, NULL, &TaskTelemRx_attributes);

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
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.LSIState       = RCC_LSI_ON;
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
 * @brief CRC Initialization Function
 * @param None
 * @retval None
 */
static void MX_CRC_Init(void)
{
    /* USER CODE BEGIN CRC_Init 0 */

    /* USER CODE END CRC_Init 0 */

    /* USER CODE BEGIN CRC_Init 1 */

    /* USER CODE END CRC_Init 1 */
    hcrc.Instance                     = CRC;
    hcrc.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_ENABLE;
    hcrc.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_ENABLE;
    hcrc.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_BYTE;
    hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE;
    hcrc.InputDataFormat              = CRC_INPUTDATA_FORMAT_BYTES;
    if (HAL_CRC_Init(&hcrc) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN CRC_Init 2 */

    /* USER CODE END CRC_Init 2 */
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
    hfdcan2.Init.FrameFormat          = FDCAN_FRAME_FD_NO_BRS;
    hfdcan2.Init.Mode                 = FDCAN_MODE_NORMAL;
    hfdcan2.Init.AutoRetransmission   = ENABLE;
    hfdcan2.Init.TransmitPause        = DISABLE;
    hfdcan2.Init.ProtocolException    = DISABLE;
    hfdcan2.Init.NominalPrescaler     = 6;
    hfdcan2.Init.NominalSyncJumpWidth = 2;
    hfdcan2.Init.NominalTimeSeg1      = 13;
    hfdcan2.Init.NominalTimeSeg2      = 2;
    hfdcan2.Init.DataPrescaler        = 3;
    hfdcan2.Init.DataSyncJumpWidth    = 2;
    hfdcan2.Init.DataTimeSeg1         = 5;
    hfdcan2.Init.DataTimeSeg2         = 2;
    hfdcan2.Init.MessageRAMOffset     = 0;
    hfdcan2.Init.StdFiltersNbr        = 1;
    hfdcan2.Init.ExtFiltersNbr        = 1;
    hfdcan2.Init.RxFifo0ElmtsNbr      = 32;
    hfdcan2.Init.RxFifo0ElmtSize      = FDCAN_DATA_BYTES_64;
    hfdcan2.Init.RxFifo1ElmtsNbr      = 0;
    hfdcan2.Init.RxFifo1ElmtSize      = FDCAN_DATA_BYTES_64;
    hfdcan2.Init.RxBuffersNbr         = 0;
    hfdcan2.Init.RxBufferSize         = FDCAN_DATA_BYTES_64;
    hfdcan2.Init.TxEventsNbr          = 0;
    hfdcan2.Init.TxBuffersNbr         = 0;
    hfdcan2.Init.TxFifoQueueElmtsNbr  = 32;
    hfdcan2.Init.TxFifoQueueMode      = FDCAN_TX_FIFO_OPERATION;
    hfdcan2.Init.TxElmtSize           = FDCAN_DATA_BYTES_64;
    if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN FDCAN2_Init 2 */

    /* USER CODE END FDCAN2_Init 2 */
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void)
{
    /* USER CODE BEGIN I2C1_Init 0 */

    /* USER CODE END I2C1_Init 0 */

    /* USER CODE BEGIN I2C1_Init 1 */

    /* USER CODE END I2C1_Init 1 */
    hi2c1.Instance              = I2C1;
    hi2c1.Init.Timing           = 0x20A0C4DF;
    hi2c1.Init.OwnAddress1      = 0;
    hi2c1.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2      = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Analogue filter
     */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Digital filter
     */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C1_Init 2 */

    /* USER CODE END I2C1_Init 2 */
}

/**
 * @brief IWDG1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_IWDG1_Init(void)
{
    /* USER CODE BEGIN IWDG1_Init 0 */
    tasks_preInitWatchdog();
#ifndef WATCHDOG_DISABLED
    /* USER CODE END IWDG1_Init 0 */

    /* USER CODE BEGIN IWDG1_Init 1 */

    /* USER CODE END IWDG1_Init 1 */
    hiwdg1.Instance       = IWDG1;
    hiwdg1.Init.Prescaler = IWDG_PRESCALER_4;
    hiwdg1.Init.Window    = 4095;
    hiwdg1.Init.Reload    = LSI_FREQUENCY / IWDG_PRESCALER / IWDG_RESET_FREQUENCY;
    if (HAL_IWDG_Init(&hiwdg1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN IWDG1_Init 2 */
#endif
    /* USER CODE END IWDG1_Init 2 */
}

/**
 * @brief SDMMC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SDMMC1_SD_Init(void)
{
    /* USER CODE BEGIN SDMMC1_Init 0 */
    if (!hw_sd_present())
    {
        return;
    }
    /* USER CODE END SDMMC1_Init 0 */

    /* USER CODE BEGIN SDMMC1_Init 1 */

    /* USER CODE END SDMMC1_Init 1 */
    hsd1.Instance                 = SDMMC1;
    hsd1.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
    hsd1.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
    hsd1.Init.BusWide             = SDMMC_BUS_WIDE_4B;
    hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_ENABLE;
    hsd1.Init.ClockDiv            = 9;
    if (HAL_SD_Init(&hsd1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN SDMMC1_Init 2 */

    /* USER CODE END SDMMC1_Init 2 */
}

/**
 * @brief TIM15 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM15_Init(void)
{
    /* USER CODE BEGIN TIM15_Init 0 */

    /* USER CODE END TIM15_Init 0 */

    TIM_MasterConfigTypeDef sMasterConfig = { 0 };
    TIM_IC_InitTypeDef      sConfigIC     = { 0 };

    /* USER CODE BEGIN TIM15_Init 1 */

    /* USER CODE END TIM15_Init 1 */
    htim15.Instance               = TIM15;
    htim15.Init.Prescaler         = 0;
    htim15.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim15.Init.Period            = 65535;
    htim15.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim15.Init.RepetitionCounter = 0;
    htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_IC_Init(&htim15) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter    = 0;
    if (HAL_TIM_IC_ConfigChannel(&htim15, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM15_Init 2 */

    /* USER CODE END TIM15_Init 2 */
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{
    /* USER CODE BEGIN USART2_Init 0 */

    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    huart2.Instance                    = USART2;
    huart2.Init.BaudRate               = 57600;
    huart2.Init.WordLength             = UART_WORDLENGTH_8B;
    huart2.Init.StopBits               = UART_STOPBITS_1;
    huart2.Init.Parity                 = UART_PARITY_NONE;
    huart2.Init.Mode                   = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl              = UART_HWCONTROL_RTS_CTS;
    huart2.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart2.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART2_Init 2 */

    /* USER CODE END USART2_Init 2 */
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
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, LED_Pin | BOOT_LED_Pin | SD_FAIL_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(_900M_GPIO_GPIO_Port, _900M_GPIO_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOE, TELEM_PWR_EN_Pin | BUZZER_PWR_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(nTSIM_GRN_EN_GPIO_Port, nTSIM_GRN_EN_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(TSIM_RED_EN_GPIO_Port, TSIM_RED_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : LED_Pin BOOT_LED_Pin SD_FAIL_Pin */
    GPIO_InitStruct.Pin   = LED_Pin | BOOT_LED_Pin | SD_FAIL_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : _900M_GPIO_Pin */
    GPIO_InitStruct.Pin   = _900M_GPIO_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(_900M_GPIO_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : TELEM_PWR_EN_Pin BUZZER_PWR_EN_Pin */
    GPIO_InitStruct.Pin   = TELEM_PWR_EN_Pin | BUZZER_PWR_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : nTSIM_GRN_EN_Pin TSIM_RED_EN_Pin */
    GPIO_InitStruct.Pin   = nTSIM_GRN_EN_Pin | TSIM_RED_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : SPLITTER_SHDN_SENSE_Pin L_SHDN_SENSE_Pin */
    GPIO_InitStruct.Pin  = SPLITTER_SHDN_SENSE_Pin | L_SHDN_SENSE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : SD_CD_Pin */
    GPIO_InitStruct.Pin  = SD_CD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(SD_CD_GPIO_Port, &GPIO_InitStruct);

    /* USER CODE BEGIN MX_GPIO_Init_2 */
    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_RunTask100Hz */
/**
 * @brief  Function implementing the Task100Hz thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTask100Hz */
void RunTask100Hz(void *argument)
{
    /* init code for USB_DEVICE */
    MX_USB_DEVICE_Init();
    /* USER CODE BEGIN 5 */
    /* Infinite loop */
    tasks_run100Hz();
    /* USER CODE END 5 */
}

void RunChimera(void *argument)
{
    /* USER CODE BEGIN 5 */
    /* Infinite loop */
    tasks_runChimera();
    /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_RunCanTxTask */
/**
 * @brief Function implementing the TaskCanTx thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunCanTxTask */
void RunCanTxTask(void *argument)
{
    /* USER CODE BEGIN RunCanTxTask */
    /* Infinite loop */
    tasks_runCanTx();
    /* USER CODE END RunCanTxTask */
}

/* USER CODE BEGIN Header_RunCanRxTask */
/**
 * @brief Function implementing the TaskCanRx thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunCanRxTask */
void RunCanRxTask(void *argument)
{
    /* USER CODE BEGIN RunCanRxTask */
    /* Infinite loop */
    tasks_runCanRx();
    /* USER CODE END RunCanRxTask */
}

/* USER CODE BEGIN Header_RunTask1kHz */
/**
 * @brief Function implementing the Task1kHz thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTask1kHz */
void RunTask1kHz(void *argument)
{
    /* USER CODE BEGIN RunTask1kHz */
    /* Infinite loop */
    tasks_run1kHz();
    /* USER CODE END RunTask1kHz */
}

/* USER CODE BEGIN Header_RunTask1Hz */
/**
 * @brief Function implementing the Task1Hz thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTask1Hz */
void RunTask1Hz(void *argument)
{
    /* USER CODE BEGIN RunTask1Hz */
    /* Infinite loop */
    tasks_run1Hz();
    /* USER CODE END RunTask1Hz */
}

/* USER CODE BEGIN Header_RunTaskLogging */
/**
 * @brief Function implementing the TaskLogging thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTaskLogging */
void RunTaskLogging(void *argument)
{
    /* USER CODE BEGIN RunTaskLogging */
    /* Infinite loop */
    tasks_runLogging();
    /* USER CODE END RunTaskLogging */
}

/* USER CODE BEGIN Header_RunTaskTelem */
/**
 * @brief Function implementing the TaskTelem thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTaskTelem */
void RunTaskTelem(void *argument)
{
    /* USER CODE BEGIN RunTaskTelem */
    /* Infinite loop */
    tasks_runTelem();
    /* USER CODE END RunTaskTelem */
}

/* USER CODE BEGIN Header_RunTaskTelemRx */
/**
 * @brief Function implementing the TaskTelemRx thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTaskTelemRx */
void RunTaskTelemRx(void *argument)
{
    /* USER CODE BEGIN RunTaskTelemRx */
    /* Infinite loop */

    tasks_runTelemRx();

    /* USER CODE END RunTaskTelemRx */
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
