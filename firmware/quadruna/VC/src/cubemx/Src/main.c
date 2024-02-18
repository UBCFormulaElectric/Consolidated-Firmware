/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "string.h"
#include "tasks.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
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
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc3;

FDCAN_HandleTypeDef hfdcan1;

UART_HandleTypeDef huart7;

/* Definitions for Task100Hz */
osThreadId_t         Task100HzHandle;
uint32_t             Task100HzBuffer[512];
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
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void        SystemClock_Config(void);
void        PeriphCommonClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_FDCAN1_Init(void);
static void MX_ADC3_Init(void);
static void MX_UART7_Init(void);
void        RunTask100Hz(void *argument);
void        RunCanTxTask(void *argument);
void        RunCanRxTask(void *argument);
void        RunTask1kHz(void *argument);
void        RunTask1Hz(void *argument);

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

    /* Configure the peripherals common clocks */
    PeriphCommonClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_FDCAN1_Init();
    MX_ADC3_Init();
    MX_UART7_Init();
    /* USER CODE BEGIN 2 */
    tasks_init();
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
    /* creation of Task100Hz */
    Task100HzHandle = osThreadNew(RunTask100Hz, NULL, &Task100Hz_attributes);

    /* creation of TaskCanTx */
    TaskCanTxHandle = osThreadNew(RunCanTxTask, NULL, &TaskCanTx_attributes);

    /* creation of TaskCanRx */
    TaskCanRxHandle = osThreadNew(RunCanRxTask, NULL, &TaskCanRx_attributes);

    /* creation of Task1kHz */
    Task1kHzHandle = osThreadNew(RunTask1kHz, NULL, &Task1kHz_attributes);

    /* creation of Task1Hz */
    Task1HzHandle = osThreadNew(RunTask1Hz, NULL, &Task1Hz_attributes);

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
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
void PeriphCommonClock_Config(void)
{
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };

    /** Initializes the peripherals clock
     */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInitStruct.PLL2.PLL2M           = 1;
    PeriphClkInitStruct.PLL2.PLL2N           = 24;
    PeriphClkInitStruct.PLL2.PLL2P           = 2;
    PeriphClkInitStruct.PLL2.PLL2Q           = 2;
    PeriphClkInitStruct.PLL2.PLL2R           = 2;
    PeriphClkInitStruct.PLL2.PLL2RGE         = RCC_PLL2VCIRANGE_3;
    PeriphClkInitStruct.PLL2.PLL2VCOSEL      = RCC_PLL2VCOWIDE;
    PeriphClkInitStruct.PLL2.PLL2FRACN       = 0;
    PeriphClkInitStruct.AdcClockSelection    = RCC_ADCCLKSOURCE_PLL2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void)
{
    /* USER CODE BEGIN ADC1_Init 0 */

    /* USER CODE END ADC1_Init 0 */

    ADC_MultiModeTypeDef   multimode = { 0 };
    ADC_ChannelConfTypeDef sConfig   = { 0 };

    /* USER CODE BEGIN ADC1_Init 1 */

    /* USER CODE END ADC1_Init 1 */

    /** Common config
     */
    hadc1.Instance                      = ADC1;
    hadc1.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV1;
    hadc1.Init.Resolution               = ADC_RESOLUTION_16B;
    hadc1.Init.ScanConvMode             = ADC_SCAN_DISABLE;
    hadc1.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;
    hadc1.Init.LowPowerAutoWait         = DISABLE;
    hadc1.Init.ContinuousConvMode       = DISABLE;
    hadc1.Init.NbrOfConversion          = 1;
    hadc1.Init.DiscontinuousConvMode    = DISABLE;
    hadc1.Init.ExternalTrigConv         = ADC_SOFTWARE_START;
    hadc1.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
    hadc1.Init.Overrun                  = ADC_OVR_DATA_PRESERVED;
    hadc1.Init.LeftBitShift             = ADC_LEFTBITSHIFT_NONE;
    hadc1.Init.OversamplingMode         = DISABLE;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure the ADC multi-mode
     */
    multimode.Mode = ADC_MODE_INDEPENDENT;
    if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel                = ADC_CHANNEL_7;
    sConfig.Rank                   = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime           = ADC_SAMPLETIME_1CYCLE_5;
    sConfig.SingleDiff             = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber           = ADC_OFFSET_NONE;
    sConfig.Offset                 = 0;
    sConfig.OffsetSignedSaturation = DISABLE;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */
}

/**
 * @brief ADC3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC3_Init(void)
{
    /* USER CODE BEGIN ADC3_Init 0 */

    /* USER CODE END ADC3_Init 0 */

    ADC_ChannelConfTypeDef sConfig = { 0 };

    /* USER CODE BEGIN ADC3_Init 1 */

    /* USER CODE END ADC3_Init 1 */

    /** Common config
     */
    hadc3.Instance                      = ADC3;
    hadc3.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV1;
    hadc3.Init.Resolution               = ADC_RESOLUTION_12B;
    hadc3.Init.DataAlign                = ADC3_DATAALIGN_RIGHT;
    hadc3.Init.ScanConvMode             = ADC_SCAN_DISABLE;
    hadc3.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;
    hadc3.Init.LowPowerAutoWait         = DISABLE;
    hadc3.Init.ContinuousConvMode       = DISABLE;
    hadc3.Init.NbrOfConversion          = 1;
    hadc3.Init.DiscontinuousConvMode    = DISABLE;
    hadc3.Init.ExternalTrigConv         = ADC_SOFTWARE_START;
    hadc3.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc3.Init.DMAContinuousRequests    = DISABLE;
    hadc3.Init.SamplingMode             = ADC_SAMPLING_MODE_NORMAL;
    hadc3.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
    hadc3.Init.Overrun                  = ADC_OVR_DATA_PRESERVED;
    hadc3.Init.LeftBitShift             = ADC_LEFTBITSHIFT_NONE;
    hadc3.Init.OversamplingMode         = DISABLE;
    if (HAL_ADC_Init(&hadc3) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel      = ADC_CHANNEL_0;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC3_SAMPLETIME_2CYCLES_5;
    sConfig.SingleDiff   = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset       = 0;
    sConfig.OffsetSign   = ADC3_OFFSET_SIGN_NEGATIVE;
    if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC3_Init 2 */

    /* USER CODE END ADC3_Init 2 */
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
    hfdcan1.Init.AutoRetransmission   = ENABLE;
    hfdcan1.Init.TransmitPause        = DISABLE;
    hfdcan1.Init.ProtocolException    = DISABLE;
    hfdcan1.Init.NominalPrescaler     = 16;
    hfdcan1.Init.NominalSyncJumpWidth = 4;
    hfdcan1.Init.NominalTimeSeg1      = 13;
    hfdcan1.Init.NominalTimeSeg2      = 2;
    hfdcan1.Init.DataPrescaler        = 1;
    hfdcan1.Init.DataSyncJumpWidth    = 1;
    hfdcan1.Init.DataTimeSeg1         = 1;
    hfdcan1.Init.DataTimeSeg2         = 1;
    hfdcan1.Init.MessageRAMOffset     = 0;
    hfdcan1.Init.StdFiltersNbr        = 1;
    hfdcan1.Init.ExtFiltersNbr        = 0;
    hfdcan1.Init.RxFifo0ElmtsNbr      = 1;
    hfdcan1.Init.RxFifo0ElmtSize      = FDCAN_DATA_BYTES_8;
    hfdcan1.Init.RxFifo1ElmtsNbr      = 0;
    hfdcan1.Init.RxFifo1ElmtSize      = FDCAN_DATA_BYTES_8;
    hfdcan1.Init.RxBuffersNbr         = 0;
    hfdcan1.Init.RxBufferSize         = FDCAN_DATA_BYTES_8;
    hfdcan1.Init.TxEventsNbr          = 0;
    hfdcan1.Init.TxBuffersNbr         = 0;
    hfdcan1.Init.TxFifoQueueElmtsNbr  = 1;
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
 * @brief UART7 Initialization Function
 * @param None
 * @retval None
 */
static void MX_UART7_Init(void)
{
    /* USER CODE BEGIN UART7_Init 0 */

    /* USER CODE END UART7_Init 0 */

    /* USER CODE BEGIN UART7_Init 1 */

    /* USER CODE END UART7_Init 1 */
    huart7.Instance                    = UART7;
    huart7.Init.BaudRate               = 115200;
    huart7.Init.WordLength             = UART_WORDLENGTH_8B;
    huart7.Init.StopBits               = UART_STOPBITS_1;
    huart7.Init.Parity                 = UART_PARITY_NONE;
    huart7.Init.Mode                   = UART_MODE_TX_RX;
    huart7.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart7.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart7.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart7.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    huart7.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart7) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart7, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart7, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&huart7) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN UART7_Init 2 */

    /* USER CODE END UART7_Init 2 */
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
        GPIOE,
        INV_R_PWR_EN_Pin | INV_R_PROGRAM_Pin | INV_L_PROGRAM_Pin | GPIO_PIN_5 | SHDN_PWR_EN_Pin | GPIO_PIN_10 |
            INV_L_PWR_EN_Pin | FR_STBY3_Pin,
        GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, FR_STBY1_Pin | LV_PWR_EN_Pin | FR_STBY2_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, BUZZER_PWR_EN_Pin | LED_Pin | TELEM_PWR_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(NPCM_EN_GPIO_Port, NPCM_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, _900K_GPIO_Pin | AUX_PWR_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : INV_R_PWR_EN_Pin INV_R_PROGRAM_Pin INV_L_PROGRAM_Pin PE5
                             SHDN_PWR_EN_Pin PE10 INV_L_PWR_EN_Pin FR_STBY3_Pin */
    GPIO_InitStruct.Pin = INV_R_PWR_EN_Pin | INV_R_PROGRAM_Pin | INV_L_PROGRAM_Pin | GPIO_PIN_5 | SHDN_PWR_EN_Pin |
                          GPIO_PIN_10 | INV_L_PWR_EN_Pin | FR_STBY3_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : FR_STBY1_Pin LV_PWR_EN_Pin FR_STBY2_Pin */
    GPIO_InitStruct.Pin   = FR_STBY1_Pin | LV_PWR_EN_Pin | FR_STBY2_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : BUZZER_PWR_EN_Pin LED_Pin TELEM_PWR_EN_Pin */
    GPIO_InitStruct.Pin   = BUZZER_PWR_EN_Pin | LED_Pin | TELEM_PWR_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : BAT_I_SNS_NFLT_Pin PA8 */
    GPIO_InitStruct.Pin  = BAT_I_SNS_NFLT_Pin | GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : R_SHDN_SNS_Pin PGOOD_Pin */
    GPIO_InitStruct.Pin  = R_SHDN_SNS_Pin | PGOOD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : TSMS_SHDN_SNS_Pin SB_ILCK_SHDN_SNS_Pin ACC_I_SENSE_NFLT_Pin NPROGRAM_3V3_Pin */
    GPIO_InitStruct.Pin  = TSMS_SHDN_SNS_Pin | SB_ILCK_SHDN_SNS_Pin | ACC_I_SENSE_NFLT_Pin | NPROGRAM_3V3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : L_SHDN_SNS_Pin PE14 PE15 */
    GPIO_InitStruct.Pin  = L_SHDN_SNS_Pin | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : PB10 PB11 */
    GPIO_InitStruct.Pin       = GPIO_PIN_10 | GPIO_PIN_11;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : NPCM_EN_Pin */
    GPIO_InitStruct.Pin   = NPCM_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(NPCM_EN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PB13 */
    GPIO_InitStruct.Pin       = GPIO_PIN_13;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : PB15 */
    GPIO_InitStruct.Pin  = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : PD8 PD9 PD12 */
    GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_12;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : PUMP_PWR_EN_Pin NCHIMERA_Pin NCHRG_FAULT_Pin NCHRG_Pin */
    GPIO_InitStruct.Pin  = PUMP_PWR_EN_Pin | NCHIMERA_Pin | NCHRG_FAULT_Pin | NCHRG_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : _900K_GPIO_Pin AUX_PWR_EN_Pin */
    GPIO_InitStruct.Pin   = _900K_GPIO_Pin | AUX_PWR_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : PC8 PC9 PC10 PC11
                             PC12 */
    GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : PA9 PA10 PA11 PA12 */
    GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : PD2 */
    GPIO_InitStruct.Pin       = GPIO_PIN_2;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : PD5 PD6 */
    GPIO_InitStruct.Pin       = GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : PB6 PB7 */
    GPIO_InitStruct.Pin       = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
    /* USER CODE BEGIN 5 */
    tasks_run100Hz();
    /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_RunCanTxTask */
/**
 * @brief Function implementing the CanTxTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunCanTxTask */
void RunCanTxTask(void *argument)
{
    /* USER CODE BEGIN RunCanTxTask */
    tasks_runCanTx();
    /* USER CODE END RunCanTxTask */
}

/* USER CODE BEGIN Header_RunCanRxTask */
/**
 * @brief Function implementing the CanRxTask thread.
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
    tasks_run1Hz();
    /* USER CODE END RunTask1Hz */
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
