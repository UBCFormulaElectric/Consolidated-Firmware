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
#include <stm32h7xx_hal_gpio.h>
#include <stdbool.h>
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
ADC_HandleTypeDef hadc2;

FDCAN_HandleTypeDef hfdcan1;
FDCAN_HandleTypeDef hfdcan2;
FDCAN_HandleTypeDef hfdcan3;

I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;
I2C_HandleTypeDef hi2c4;
I2C_HandleTypeDef hi2c5;

IWDG_HandleTypeDef hiwdg1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart8;

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
/* Definitions for TaskCan1Tx */
osThreadId_t         TaskCan1TxHandle;
uint32_t             TaskCanTxBuffer[512];
osStaticThreadDef_t  TaskCanTxControlBlock;
const osThreadAttr_t TaskCan1Tx_attributes = {
    .name       = "TaskCan1Tx",
    .cb_mem     = &TaskCanTxControlBlock,
    .cb_size    = sizeof(TaskCanTxControlBlock),
    .stack_mem  = &TaskCanTxBuffer[0],
    .stack_size = sizeof(TaskCanTxBuffer),
    .priority   = (osPriority_t)osPriorityBelowNormal,
};
/* Definitions for TaskCanRx */
osThreadId_t         TaskCanRxHandle;
uint32_t             TaskCanRxBuffer[512];
osStaticThreadDef_t  TaskCanRxControlBlock;
const osThreadAttr_t TaskCanRx_attributes = {
    .name       = "TaskCanRx",
    .cb_mem     = &TaskCanRxControlBlock,
    .cb_size    = sizeof(TaskCanRxControlBlock),
    .stack_mem  = &TaskCanRxBuffer[0],
    .stack_size = sizeof(TaskCanRxBuffer),
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
/* Definitions for TaskBtrMonitor */
osThreadId_t         TaskBtrMonitorHandle;
uint32_t             TaskBatteryMoniBuffer[512];
osStaticThreadDef_t  TaskBatteryMoniControlBlock;
const osThreadAttr_t TaskBtrMonitor_attributes = {
    .name       = "TaskBtrMonitor",
    .cb_mem     = &TaskBatteryMoniControlBlock,
    .cb_size    = sizeof(TaskBatteryMoniControlBlock),
    .stack_mem  = &TaskBatteryMoniBuffer[0],
    .stack_size = sizeof(TaskBatteryMoniBuffer),
    .priority   = (osPriority_t)osPriorityLow,
};
/* Definitions for TaskChimera */
osThreadId_t         TaskChimeraHandle;
uint32_t             TaskChimeraBuffer[512];
osStaticThreadDef_t  TaskChimeraControlBlock;
const osThreadAttr_t TaskChimera_attributes = {
    .name       = "TaskChimera",
    .cb_mem     = &TaskChimeraControlBlock,
    .cb_size    = sizeof(TaskChimeraControlBlock),
    .stack_mem  = &TaskChimeraBuffer[0],
    .stack_size = sizeof(TaskChimeraBuffer),
    .priority   = (osPriority_t)osPriorityHigh,
};
/* Definitions for TaskCan2Tx */
osThreadId_t         TaskCan2TxHandle;
uint32_t             TaskCan2TxBuffer[512];
osStaticThreadDef_t  TaskCan2TxControlBlock;
const osThreadAttr_t TaskCan2Tx_attributes = {
    .name       = "TaskCan2Tx",
    .cb_mem     = &TaskCan2TxControlBlock,
    .cb_size    = sizeof(TaskCan2TxControlBlock),
    .stack_mem  = &TaskCan2TxBuffer[0],
    .stack_size = sizeof(TaskCan2TxBuffer),
    .priority   = (osPriority_t)osPriorityBelowNormal,
};
/* Definitions for TaskCan3Tx */
osThreadId_t         TaskCan3TxHandle;
uint32_t             TaskCan3TxBuffer[512];
osStaticThreadDef_t  TaskCan3TxControlBlock;
const osThreadAttr_t TaskCan3Tx_attributes = {
    .name       = "TaskCan3Tx",
    .cb_mem     = &TaskCan3TxControlBlock,
    .cb_size    = sizeof(TaskCan3TxControlBlock),
    .stack_mem  = &TaskCan3TxBuffer[0],
    .stack_size = sizeof(TaskCan3TxBuffer),
    .priority   = (osPriority_t)osPriorityBelowNormal,
};
/* Definitions for TaskPwrMontr */
osThreadId_t         TaskPwrMontrHandle;
uint32_t             TaskPwrMontrBuffer[512];
osStaticThreadDef_t  TaskPwrMontrControlBlock;
const osThreadAttr_t TaskPwrMontr_attributes = {
    .name       = "TaskPwrMontr",
    .cb_mem     = &TaskPwrMontrControlBlock,
    .cb_size    = sizeof(TaskPwrMontrControlBlock),
    .stack_mem  = &TaskPwrMontrBuffer[0],
    .stack_size = sizeof(TaskPwrMontrBuffer),
    .priority   = (osPriority_t)osPriorityAboveNormal2,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void        SystemClock_Config(void);
void        PeriphCommonClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_FDCAN1_Init(void);
static void MX_FDCAN2_Init(void);
static void MX_FDCAN3_Init(void);
static void MX_I2C2_Init(void);
static void MX_I2C3_Init(void);
static void MX_I2C4_Init(void);
static void MX_UART8_Init(void);
static void MX_I2C5_Init(void);
static void MX_TIM3_Init(void);
static void MX_ADC2_Init(void);
static void MX_IWDG1_Init(void);
void        RunTask100Hz(void *argument);
void        RunCan1TxTask(void *argument);
void        RunCanRxTask(void *argument);
void        RunTask1kHz(void *argument);
void        RunTask1Hz(void *argument);
void        RunTaskBtrMonitor(void *argument);
void        RunTaskChimera(void *argument);
void        RunCan2TxTask(void *argument);
void        RunTask3TxTask(void *argument);
void        RunPwrMontr(void *argument);

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
    MX_FDCAN2_Init();
    MX_FDCAN3_Init();
    MX_I2C2_Init();
    MX_I2C3_Init();
    MX_I2C4_Init();
    MX_UART8_Init();
    MX_I2C5_Init();
    MX_TIM3_Init();
    MX_ADC2_Init();
    MX_IWDG1_Init();
    /* USER CODE BEGIN 2 */
    HAL_GPIO_WritePin(BAT_CHRG_nSHDN_GPIO_Port, BAT_CHRG_nSHDN_Pin, false);
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
    /* creation of Task100Hz */
    Task100HzHandle = osThreadNew(RunTask100Hz, NULL, &Task100Hz_attributes);

    /* creation of TaskCan1Tx */
    TaskCan1TxHandle = osThreadNew(RunCan1TxTask, NULL, &TaskCan1Tx_attributes);

    /* creation of TaskCanRx */
    TaskCanRxHandle = osThreadNew(RunCanRxTask, NULL, &TaskCanRx_attributes);

    /* creation of Task1kHz */
    Task1kHzHandle = osThreadNew(RunTask1kHz, NULL, &Task1kHz_attributes);

    /* creation of Task1Hz */
    Task1HzHandle = osThreadNew(RunTask1Hz, NULL, &Task1Hz_attributes);

    /* creation of TaskBtrMonitor */
    TaskBtrMonitorHandle = osThreadNew(RunTaskBtrMonitor, NULL, &TaskBtrMonitor_attributes);

    /* creation of TaskChimera */
    TaskChimeraHandle = osThreadNew(RunTaskChimera, NULL, &TaskChimera_attributes);

    /* creation of TaskCan2Tx */
    TaskCan2TxHandle = osThreadNew(RunCan2TxTask, NULL, &TaskCan2Tx_attributes);

    /* creation of TaskCan3Tx */
    TaskCan3TxHandle = osThreadNew(RunTask3TxTask, NULL, &TaskCan3Tx_attributes);

    /* creation of TaskPwrMontr */
    TaskPwrMontrHandle = osThreadNew(RunPwrMontr, NULL, &TaskPwrMontr_attributes);

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
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
void PeriphCommonClock_Config(void)
{
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };

    /** Initializes the peripherals clock
     */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB | RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_FDCAN;
    PeriphClkInitStruct.PLL2.PLL2M           = 1;
    PeriphClkInitStruct.PLL2.PLL2N           = 24;
    PeriphClkInitStruct.PLL2.PLL2P           = 2;
    PeriphClkInitStruct.PLL2.PLL2Q           = 2;
    PeriphClkInitStruct.PLL2.PLL2R           = 2;
    PeriphClkInitStruct.PLL2.PLL2RGE         = RCC_PLL2VCIRANGE_3;
    PeriphClkInitStruct.PLL2.PLL2VCOSEL      = RCC_PLL2VCOWIDE;
    PeriphClkInitStruct.PLL2.PLL2FRACN       = 0;
    PeriphClkInitStruct.PLL3.PLL3M           = 1;
    PeriphClkInitStruct.PLL3.PLL3N           = 24;
    PeriphClkInitStruct.PLL3.PLL3P           = 2;
    PeriphClkInitStruct.PLL3.PLL3Q           = 4;
    PeriphClkInitStruct.PLL3.PLL3R           = 2;
    PeriphClkInitStruct.PLL3.PLL3RGE         = RCC_PLL3VCIRANGE_3;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL      = RCC_PLL3VCOWIDE;
    PeriphClkInitStruct.PLL3.PLL3FRACN       = 0;
    PeriphClkInitStruct.FdcanClockSelection  = RCC_FDCANCLKSOURCE_PLL2;
    PeriphClkInitStruct.UsbClockSelection    = RCC_USBCLKSOURCE_PLL3;
    PeriphClkInitStruct.AdcClockSelection    = RCC_ADCCLKSOURCE_PLL3;
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
    hadc1.Init.ScanConvMode             = ADC_SCAN_ENABLE;
    hadc1.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;
    hadc1.Init.LowPowerAutoWait         = DISABLE;
    hadc1.Init.ContinuousConvMode       = DISABLE;
    hadc1.Init.NbrOfConversion          = 6;
    hadc1.Init.DiscontinuousConvMode    = DISABLE;
    hadc1.Init.ExternalTrigConv         = ADC_EXTERNALTRIG_T3_TRGO;
    hadc1.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
    hadc1.Init.Overrun                  = ADC_OVR_DATA_PRESERVED;
    hadc1.Init.LeftBitShift             = ADC_LEFTBITSHIFT_NONE;
    hadc1.Init.OversamplingMode         = DISABLE;
    hadc1.Init.Oversampling.Ratio       = 1;
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
    sConfig.Channel                = ADC_CHANNEL_5;
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

    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank    = ADC_REGULAR_RANK_2;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Rank = ADC_REGULAR_RANK_3;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Rank = ADC_REGULAR_RANK_4;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Rank = ADC_REGULAR_RANK_5;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Rank = ADC_REGULAR_RANK_6;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */
}

/**
 * @brief ADC2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC2_Init(void)
{
    /* USER CODE BEGIN ADC2_Init 0 */

    /* USER CODE END ADC2_Init 0 */

    ADC_ChannelConfTypeDef sConfig = { 0 };

    /* USER CODE BEGIN ADC2_Init 1 */

    /* USER CODE END ADC2_Init 1 */

    /** Common config
     */
    hadc2.Instance                      = ADC2;
    hadc2.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV1;
    hadc2.Init.Resolution               = ADC_RESOLUTION_16B;
    hadc2.Init.ScanConvMode             = ADC_SCAN_ENABLE;
    hadc2.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;
    hadc2.Init.LowPowerAutoWait         = DISABLE;
    hadc2.Init.ContinuousConvMode       = DISABLE;
    hadc2.Init.NbrOfConversion          = 5;
    hadc2.Init.DiscontinuousConvMode    = DISABLE;
    hadc2.Init.ExternalTrigConv         = ADC_EXTERNALTRIG_T3_TRGO;
    hadc2.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc2.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
    hadc2.Init.Overrun                  = ADC_OVR_DATA_PRESERVED;
    hadc2.Init.LeftBitShift             = ADC_LEFTBITSHIFT_NONE;
    hadc2.Init.OversamplingMode         = DISABLE;
    hadc2.Init.Oversampling.Ratio       = 1;
    if (HAL_ADC_Init(&hadc2) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel                = ADC_CHANNEL_3;
    sConfig.Rank                   = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime           = ADC_SAMPLETIME_32CYCLES_5;
    sConfig.SingleDiff             = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber           = ADC_OFFSET_NONE;
    sConfig.Offset                 = 0;
    sConfig.OffsetSignedSaturation = DISABLE;
    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_8;
    sConfig.Rank    = ADC_REGULAR_RANK_2;
    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_9;
    sConfig.Rank    = ADC_REGULAR_RANK_3;
    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_18;
    sConfig.Rank    = ADC_REGULAR_RANK_4;
    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_19;
    sConfig.Rank    = ADC_REGULAR_RANK_5;
    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC2_Init 2 */

    /* USER CODE END ADC2_Init 2 */
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
    hfdcan1.Init.FrameFormat          = FDCAN_FRAME_FD_BRS;
    hfdcan1.Init.Mode                 = FDCAN_MODE_NORMAL;
    hfdcan1.Init.AutoRetransmission   = ENABLE;
    hfdcan1.Init.TransmitPause        = DISABLE;
    hfdcan1.Init.ProtocolException    = DISABLE;
    hfdcan1.Init.NominalPrescaler     = 6;
    hfdcan1.Init.NominalSyncJumpWidth = 2;
    hfdcan1.Init.NominalTimeSeg1      = 13;
    hfdcan1.Init.NominalTimeSeg2      = 2;
    hfdcan1.Init.DataPrescaler        = 3;
    hfdcan1.Init.DataSyncJumpWidth    = 2;
    hfdcan1.Init.DataTimeSeg1         = 5;
    hfdcan1.Init.DataTimeSeg2         = 2;
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
    hfdcan2.Init.AutoRetransmission   = ENABLE;
    hfdcan2.Init.TransmitPause        = DISABLE;
    hfdcan2.Init.ProtocolException    = DISABLE;
    hfdcan2.Init.NominalPrescaler     = 12;
    hfdcan2.Init.NominalSyncJumpWidth = 3;
    hfdcan2.Init.NominalTimeSeg1      = 12;
    hfdcan2.Init.NominalTimeSeg2      = 3;
    hfdcan2.Init.DataPrescaler        = 1;
    hfdcan2.Init.DataSyncJumpWidth    = 1;
    hfdcan2.Init.DataTimeSeg1         = 1;
    hfdcan2.Init.DataTimeSeg2         = 1;
    hfdcan2.Init.MessageRAMOffset     = 1280;
    hfdcan2.Init.StdFiltersNbr        = 1;
    hfdcan2.Init.ExtFiltersNbr        = 1;
    hfdcan2.Init.RxFifo0ElmtsNbr      = 32;
    hfdcan2.Init.RxFifo0ElmtSize      = FDCAN_DATA_BYTES_8;
    hfdcan2.Init.RxFifo1ElmtsNbr      = 0;
    hfdcan2.Init.RxFifo1ElmtSize      = FDCAN_DATA_BYTES_8;
    hfdcan2.Init.RxBuffersNbr         = 0;
    hfdcan2.Init.RxBufferSize         = FDCAN_DATA_BYTES_8;
    hfdcan2.Init.TxEventsNbr          = 0;
    hfdcan2.Init.TxBuffersNbr         = 0;
    hfdcan2.Init.TxFifoQueueElmtsNbr  = 32;
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
    hfdcan3.Init.AutoRetransmission   = ENABLE;
    hfdcan3.Init.TransmitPause        = DISABLE;
    hfdcan3.Init.ProtocolException    = DISABLE;
    hfdcan3.Init.NominalPrescaler     = 6;
    hfdcan3.Init.NominalSyncJumpWidth = 3;
    hfdcan3.Init.NominalTimeSeg1      = 12;
    hfdcan3.Init.NominalTimeSeg2      = 3;
    hfdcan3.Init.DataPrescaler        = 1;
    hfdcan3.Init.DataSyncJumpWidth    = 1;
    hfdcan3.Init.DataTimeSeg1         = 1;
    hfdcan3.Init.DataTimeSeg2         = 1;
    hfdcan3.Init.MessageRAMOffset     = 1920;
    hfdcan3.Init.StdFiltersNbr        = 1;
    hfdcan3.Init.ExtFiltersNbr        = 1;
    hfdcan3.Init.RxFifo0ElmtsNbr      = 32;
    hfdcan3.Init.RxFifo0ElmtSize      = FDCAN_DATA_BYTES_8;
    hfdcan3.Init.RxFifo1ElmtsNbr      = 0;
    hfdcan3.Init.RxFifo1ElmtSize      = FDCAN_DATA_BYTES_8;
    hfdcan3.Init.RxBuffersNbr         = 0;
    hfdcan3.Init.RxBufferSize         = FDCAN_DATA_BYTES_8;
    hfdcan3.Init.TxEventsNbr          = 0;
    hfdcan3.Init.TxBuffersNbr         = 0;
    hfdcan3.Init.TxFifoQueueElmtsNbr  = 32;
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
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void)
{
    /* USER CODE BEGIN I2C2_Init 0 */

    /* USER CODE END I2C2_Init 0 */

    /* USER CODE BEGIN I2C2_Init 1 */

    /* USER CODE END I2C2_Init 1 */
    hi2c2.Instance              = I2C2;
    hi2c2.Init.Timing           = 0x20A0C4DF;
    hi2c2.Init.OwnAddress1      = 0;
    hi2c2.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    hi2c2.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    hi2c2.Init.OwnAddress2      = 0;
    hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c2.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    hi2c2.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c2) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Analogue filter
     */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Digital filter
     */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C2_Init 2 */

    /* USER CODE END I2C2_Init 2 */
}

/**
 * @brief I2C3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C3_Init(void)
{
    /* USER CODE BEGIN I2C3_Init 0 */

    /* USER CODE END I2C3_Init 0 */

    /* USER CODE BEGIN I2C3_Init 1 */

    /* USER CODE END I2C3_Init 1 */
    hi2c3.Instance              = I2C3;
    hi2c3.Init.Timing           = 0x20A0C4DF;
    hi2c3.Init.OwnAddress1      = 0;
    hi2c3.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    hi2c3.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    hi2c3.Init.OwnAddress2      = 0;
    hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c3.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    hi2c3.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c3) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Analogue filter
     */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Digital filter
     */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C3_Init 2 */

    /* USER CODE END I2C3_Init 2 */
}

/**
 * @brief I2C4 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C4_Init(void)
{
    /* USER CODE BEGIN I2C4_Init 0 */

    /* USER CODE END I2C4_Init 0 */

    /* USER CODE BEGIN I2C4_Init 1 */

    /* USER CODE END I2C4_Init 1 */
    hi2c4.Instance              = I2C4;
    hi2c4.Init.Timing           = 0x20A0C4DF;
    hi2c4.Init.OwnAddress1      = 0;
    hi2c4.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    hi2c4.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    hi2c4.Init.OwnAddress2      = 0;
    hi2c4.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c4.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    hi2c4.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c4) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Analogue filter
     */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c4, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Digital filter
     */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c4, 0) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C4_Init 2 */

    /* USER CODE END I2C4_Init 2 */
}

/**
 * @brief I2C5 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C5_Init(void)
{
    /* USER CODE BEGIN I2C5_Init 0 */

    /* USER CODE END I2C5_Init 0 */

    /* USER CODE BEGIN I2C5_Init 1 */

    /* USER CODE END I2C5_Init 1 */
    hi2c5.Instance              = I2C5;
    hi2c5.Init.Timing           = 0x20A0C4DF;
    hi2c5.Init.OwnAddress1      = 0;
    hi2c5.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    hi2c5.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    hi2c5.Init.OwnAddress2      = 0;
    hi2c5.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c5.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    hi2c5.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c5) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Analogue filter
     */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c5, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Digital filter
     */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c5, 0) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C5_Init 2 */

    /* USER CODE END I2C5_Init 2 */
}

/**
 * @brief IWDG1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_IWDG1_Init(void)
{
    /* USER CODE BEGIN IWDG1_Init 0 */
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
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void)
{
    /* USER CODE BEGIN TIM3_Init 0 */

    /* USER CODE END TIM3_Init 0 */

    TIM_MasterConfigTypeDef sMasterConfig = { 0 };
    TIM_OC_InitTypeDef      sConfigOC     = { 0 };

    /* USER CODE BEGIN TIM3_Init 1 */

    /* USER CODE END TIM3_Init 1 */
    htim3.Instance               = TIM3;
    htim3.Init.Prescaler         = 0;
    htim3.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim3.Init.Period            = 65535;
    htim3.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_OC_Init(&htim3) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode     = TIM_OCMODE_TIMING;
    sConfigOC.Pulse      = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM3_Init 2 */

    /* USER CODE END TIM3_Init 2 */
}

/**
 * @brief UART8 Initialization Function
 * @param None
 * @retval None
 */
static void MX_UART8_Init(void)
{
    /* USER CODE BEGIN UART8_Init 0 */

    /* USER CODE END UART8_Init 0 */

    /* USER CODE BEGIN UART8_Init 1 */

    /* USER CODE END UART8_Init 1 */
    huart8.Instance                    = UART8;
    huart8.Init.BaudRate               = 115200;
    huart8.Init.WordLength             = UART_WORDLENGTH_8B;
    huart8.Init.StopBits               = UART_STOPBITS_1;
    huart8.Init.Parity                 = UART_PARITY_NONE;
    huart8.Init.Mode                   = UART_MODE_TX_RX;
    huart8.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart8.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart8.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart8.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    huart8.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart8, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart8, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&huart8) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN UART8_Init 2 */

    /* USER CODE END UART8_Init 2 */
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
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, FR_STBY_INV_Pin | F_INV_EN_Pin | DAM_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOE, R_INV_EN_Pin | LED_Pin | BOOT_LED_Pin | BMS_EN_Pin | FR_STBY_FRONT_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(BAT_CHRG_nSHDN_GPIO_Port, BAT_CHRG_nSHDN_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, FR_STBY_REAR_Pin | FRONT_EN_Pin | RSM_EN_Pin | L_RAD_FAN_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(RAD_FAN_FR_STBY_GPIO_Port, RAD_FAN_FR_STBY_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(RR_PUMP_EN_GPIO_Port, RR_PUMP_EN_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, PCM_EN_Pin | R_RAD_FAN_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, F_PUMP_EN_Pin | RL_PUMP_EN_Pin, GPIO_PIN_SET);

    /*Configure GPIO pins : SB_SHDN_3V3_OUT_Pin INTERIA_3V3_OUT_Pin RL_PUMP_PGOOD_Pin */
    GPIO_InitStruct.Pin  = SB_SHDN_3V3_OUT_Pin | INTERIA_3V3_OUT_Pin | RL_PUMP_PGOOD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : FR_STBY_INV_Pin F_INV_EN_Pin DAM_EN_Pin */
    GPIO_InitStruct.Pin   = FR_STBY_INV_Pin | F_INV_EN_Pin | DAM_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : TSMS_3V3_OUT_Pin */
    GPIO_InitStruct.Pin  = TSMS_3V3_OUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(TSMS_3V3_OUT_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : RR_ILCK_3V3_OUT_Pin PGOOD_Pin BAT_CHRG_MODE_Pin */
    GPIO_InitStruct.Pin  = RR_ILCK_3V3_OUT_Pin | PGOOD_Pin | BAT_CHRG_MODE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : R_INV_EN_Pin LED_Pin BOOT_LED_Pin BMS_EN_Pin
                             FR_STBY_FRONT_Pin BAT_CHRG_nSHDN_Pin */
    GPIO_InitStruct.Pin   = R_INV_EN_Pin | LED_Pin | BOOT_LED_Pin | BMS_EN_Pin | FR_STBY_FRONT_Pin | BAT_CHRG_nSHDN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pin : BAT_MTR_nALERT_Pin */
    GPIO_InitStruct.Pin  = BAT_MTR_nALERT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BAT_MTR_nALERT_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : FR_STBY_REAR_Pin FRONT_EN_Pin RSM_EN_Pin L_RAD_FAN_EN_Pin */
    GPIO_InitStruct.Pin   = FR_STBY_REAR_Pin | FRONT_EN_Pin | RSM_EN_Pin | L_RAD_FAN_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : IMU_INT1_Pin IMU_INT2_Pin */
    GPIO_InitStruct.Pin  = IMU_INT1_Pin | IMU_INT2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pin : RAD_FAN_FR_STBY_Pin */
    GPIO_InitStruct.Pin   = RAD_FAN_FR_STBY_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RAD_FAN_FR_STBY_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PWR_MTR_nALERT_Pin */
    GPIO_InitStruct.Pin  = PWR_MTR_nALERT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(PWR_MTR_nALERT_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : RR_PUMP_EN_Pin */
    GPIO_InitStruct.Pin   = RR_PUMP_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RR_PUMP_EN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : RR_PUMP_PGOOD_Pin F_PUMP_PGOOD_Pin */
    GPIO_InitStruct.Pin  = RR_PUMP_PGOOD_Pin | F_PUMP_PGOOD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : PCM_EN_Pin R_RAD_FAN_EN_Pin */
    GPIO_InitStruct.Pin   = PCM_EN_Pin | R_RAD_FAN_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : F_PUMP_EN_Pin RL_PUMP_EN_Pin */
    GPIO_InitStruct.Pin   = F_PUMP_EN_Pin | RL_PUMP_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

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
    tasks_run100Hz();
    /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_RunCan1TxTask */
/**
 * @brief Function implementing the TaskCan1Tx thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunCan1TxTask */
void RunCan1TxTask(void *argument)
{
    /* USER CODE BEGIN RunCan1TxTask */
    tasks_runCan1Tx();
    /* USER CODE END RunCan1TxTask */
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

/* USER CODE BEGIN Header_RunTaskBtrMonitor */
/**
 * @brief Function implementing the TaskBtrMonitor thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTaskBtrMonitor */
void RunTaskBtrMonitor(void *argument)
{
    /* USER CODE BEGIN RunTaskBtrMonitor */
    tasks_batteryMonitoring();
    /* USER CODE END RunTaskBtrMonitor */
}

/* USER CODE BEGIN Header_RunTaskChimera */
/**
 * @brief Function implementing the TaskChimera thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTaskChimera */
void RunTaskChimera(void *argument)
{
    /* USER CODE BEGIN RunTaskChimera */
    tasks_runChimera();
    /* USER CODE END RunTaskChimera */
}

/* USER CODE BEGIN Header_RunCan2TxTask */
/**
 * @brief Function implementing the TaskCan2Tx thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunCan2TxTask */
void RunCan2TxTask(void *argument)
{
    /* USER CODE BEGIN RunCan2TxTask */
    tasks_runCan2Tx();
    /* USER CODE END RunCan2TxTask */
}

/* USER CODE BEGIN Header_RunTask3TxTask */
/**
 * @brief Function implementing the TaskCan3Tx thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTask3TxTask */
void RunTask3TxTask(void *argument)
{
    /* USER CODE BEGIN RunTask3TxTask */
    tasks_runCan3Tx();
    /* USER CODE END RunTask3TxTask */
}

/* USER CODE BEGIN Header_RunPwrMontr */
/**
 * @brief Function implementing the TaskPwrMontr thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunPwrMontr */
void RunPwrMontr(void *argument)
{
    /* USER CODE BEGIN RunPwrMontr */
    /* Infinite loop */
    for (;;)
    {
        tasks_powerMonitoring();
    }
    /* USER CODE END RunPwrMontr */
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
