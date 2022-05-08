/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <assert.h>

#include "Io_CanTx.h"
#include "Io_CanRx.h"
#include "Io_SharedSoftwareWatchdog.h"
#include "Io_SharedCan.h"
#include "Io_SharedErrorTable.h"
#include "Io_SharedHardFaultHandler.h"
#include "Io_StackWaterMark.h"
#include "Io_SoftwareWatchdog.h"
#include "Io_Imd.h"
#include "Io_HeartbeatMonitor.h"
#include "Io_RgbLedSequence.h"
#include "Io_Charger.h"
#include "Io_OkStatuses.h"
#include "Io_LTC6813/Io_LTC6813Shared.h"
#include "Io_LTC6813/Io_LTC6813CellVoltages.h"
#include "Io_LTC6813/Io_LTC6813CellTemperatures.h"
#include "Io_Airs.h"
#include "Io_PreCharge.h"
#include "Io_Adc.h"
#include "Io_VoltageSense.h"

#include "App_BmsWorld.h"
#include "App_SharedMacros.h"
#include "App_SharedStateMachine.h"
#include "states/App_InitState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "configs/App_ImdConfig.h"
#include "configs/App_AccumulatorThresholds.h"
#include "configs/App_CellMonitorsThresholds.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
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
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;

CAN_HandleTypeDef hcan;

IWDG_HandleTypeDef hiwdg;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

osThreadId          Task1HzHandle;
uint32_t            Task1HzBuffer[TASK1HZ_STACK_SIZE];
osStaticThreadDef_t Task1HzControlBlock;
osThreadId          Task1kHzHandle;
uint32_t            Task1kHzBuffer[TASK1KHZ_STACK_SIZE];
osStaticThreadDef_t Task1kHzControlBlock;
osThreadId          TaskCanRxHandle;
uint32_t            TaskCanRxBuffer[TASKCANRX_STACK_SIZE];
osStaticThreadDef_t TaskCanRxControlBlock;
osThreadId          TaskCanTxHandle;
uint32_t            TaskCanTxBuffer[TASKCANTX_STACK_SIZE];
osStaticThreadDef_t TaskCanTxControlBlock;
osThreadId          Task100HzHandle;
uint32_t            Task100HzBuffer[TASK100HZ_STACK_SIZE];
osStaticThreadDef_t Task100HzControlBlock;
/* USER CODE BEGIN PV */
struct BmsWorld *         world;
struct StateMachine *     state_machine;
struct BmsCanTxInterface *can_tx;
struct BmsCanRxInterface *can_rx;
struct Imd *              imd;
struct HeartbeatMonitor * heartbeat_monitor;
struct RgbLedSequence *   rgb_led_sequence;
struct Charger *          charger;
struct OkStatus *         bms_ok;
struct OkStatus *         imd_ok;
struct OkStatus *         bspd_ok;
struct Accumulator *      accumulator;
struct CellMonitors *     cell_monitors;
struct Airs *             airs;
struct PrechargeRelay *   precharge_relay;
struct TractiveSystem *   ts;
struct ErrorTable *       error_table;
struct Clock *            clock;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void        SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_CAN_Init(void);
static void MX_ADC1_Init(void);
static void MX_IWDG_Init(void);
static void MX_TIM2_Init(void);
static void MX_ADC2_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM3_Init(void);
void        RunTask1Hz(void const *argument);
void        RunTask1kHz(void const *argument);
void        RunTaskCanRx(void const *argument);
void        RunTaskCanTx(void const *argument);
void        RunTask100Hz(void const *argument);

/* USER CODE BEGIN PFP */

static void CanRxQueueOverflowCallBack(size_t overflow_count);
static void CanTxQueueOverflowCallBack(size_t overflow_count);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void CanRxQueueOverflowCallBack(size_t overflow_count)
{
    App_CanTx_SetPeriodicSignal_RX_OVERFLOW_COUNT(can_tx, overflow_count);
}

static void CanTxQueueOverflowCallBack(size_t overflow_count)
{
    App_CanTx_SetPeriodicSignal_TX_OVERFLOW_COUNT(can_tx, overflow_count);
}

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

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_CAN_Init();
    MX_ADC1_Init();
    MX_IWDG_Init();
    MX_TIM2_Init();
    MX_ADC2_Init();
    MX_SPI2_Init();
    MX_TIM3_Init();
    /* USER CODE BEGIN 2 */
    __HAL_DBGMCU_FREEZE_IWDG();

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)Io_Adc_GetRawAdc1Values(), hadc1.Init.NbrOfConversion);
    HAL_ADC_Start_DMA(&hadc2, (uint32_t *)Io_Adc_GetRawAdc2Values(), hadc2.Init.NbrOfConversion);
    HAL_TIM_Base_Start(&htim3);

    Io_SharedHardFaultHandler_Init();

    Io_Imd_Init();
    imd = App_Imd_Create(Io_Imd_GetFrequency, IMD_FREQUENCY_TOLERANCE, Io_Imd_GetDutyCycle, Io_Imd_GetTimeSincePowerOn);

    can_tx = App_CanTx_Create(
        Io_CanTx_EnqueueNonPeriodicMsg_BMS_STARTUP, Io_CanTx_EnqueueNonPeriodicMsg_BMS_WATCHDOG_TIMEOUT,
        Io_CanTx_EnqueueNonPeriodicMsg_BMS_CHARGING_STATUS);

    can_rx = App_CanRx_Create();

    heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
        Io_HeartbeatMonitor_GetCurrentMs, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK,
        Io_HeartbeatMonitor_TimeoutCallback);

    rgb_led_sequence = App_SharedRgbLedSequence_Create(
        Io_RgbLedSequence_TurnOnRedLed, Io_RgbLedSequence_TurnOnBlueLed, Io_RgbLedSequence_TurnOnGreenLed);

    charger = App_Charger_Create(Io_Charger_Enable, Io_Charger_Disable, Io_Charger_IsConnected, Io_Charger_HasFaulted);
        Io_Charger_Enable, Io_Charger_Disable, Io_Charger_IsConnected,
        Io_Charger_HasFaulted);

    bms_ok = App_OkStatus_Create(Io_OkStatuses_EnableBmsOk, Io_OkStatuses_DisableBmsOk, Io_OkStatuses_IsBmsOkEnabled);

    imd_ok = App_OkStatus_Create(Io_OkStatuses_EnableImdOk, Io_OkStatuses_DisableImdOk, Io_OkStatuses_IsImdOkEnabled);

    bspd_ok =
        App_OkStatus_Create(Io_OkStatuses_EnableBspdOk, Io_OkStatuses_DisableBspdOk, Io_OkStatuses_IsBspdOkEnabled);

    Io_LTC6813Shared_InitSpiHandle(&hspi2);
    accumulator = App_Accumulator_Create(
        Io_LTC6813Shared_SetCfgRegsToDefaultSettings, Io_LTC6813Shared_WriteConfigurationRegisters,
        Io_LTC6813CellVoltages_StartAdcConversion, Io_LTC6813CellVoltages_ReadVoltages,
        Io_LTC6813CellVoltages_GetMinCellVoltage, Io_LTC6813CellVoltages_GetMaxCellVoltage,
        Io_LTC6813CellVoltages_GetSegmentVoltage, Io_LTC6813CellVoltages_GetPackVoltage,
        Io_LTC6813CellVoltages_GetAverageCellVoltage, Io_LTC6813CellTemperatures_StartAdcConversion,
        Io_LTC6813CellTemperatures_ReadTemperatures);

    ts = App_TractiveSystem_Create(Io_Adc_GetAdc1Channel3Voltage, Io_VoltageSense_GetTractiveSystemVoltage);

    airs = App_Airs_Create(
        Io_Airs_IsAirPositiveClosed, Io_Airs_IsAirNegativeClosed, Io_Airs_CloseAirPositive, Io_Airs_OpenAirPositive);

    precharge_relay = App_PrechargeRelay_Create(Io_PreCharge_Enable, Io_PreCharge_Disable);

    error_table = App_SharedErrorTable_Create();

    clock = App_SharedClock_Create();

    world = App_BmsWorld_Create(
        can_tx, can_rx, imd, heartbeat_monitor, rgb_led_sequence, charger, bms_ok, imd_ok, bspd_ok, accumulator, airs,
        precharge_relay, ts, error_table, clock);

    Io_StackWaterMark_Init(can_tx);
    Io_SoftwareWatchdog_Init(can_tx);

    state_machine = App_SharedStateMachine_Create(world, App_GetInitState());

    struct CanMsgs_bms_startup_t payload = { .dummy = 0 };
    App_CanTx_SendNonPeriodicMsg_BMS_STARTUP(can_tx, &payload);
    /* USER CODE END 2 */

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
    /* definition and creation of Task1Hz */
    osThreadStaticDef(Task1Hz, RunTask1Hz, osPriorityLow, 0, TASK1HZ_STACK_SIZE, Task1HzBuffer, &Task1HzControlBlock);
    Task1HzHandle = osThreadCreate(osThread(Task1Hz), NULL);

    /* definition and creation of Task1kHz */
    osThreadStaticDef(
        Task1kHz, RunTask1kHz, osPriorityAboveNormal, 0, TASK1KHZ_STACK_SIZE, Task1kHzBuffer, &Task1kHzControlBlock);
    Task1kHzHandle = osThreadCreate(osThread(Task1kHz), NULL);

    /* definition and creation of TaskCanRx */
    osThreadStaticDef(
        TaskCanRx, RunTaskCanRx, osPriorityIdle, 0, TASKCANRX_STACK_SIZE, TaskCanRxBuffer, &TaskCanRxControlBlock);
    TaskCanRxHandle = osThreadCreate(osThread(TaskCanRx), NULL);

    /* definition and creation of TaskCanTx */
    osThreadStaticDef(
        TaskCanTx, RunTaskCanTx, osPriorityIdle, 0, TASKCANTX_STACK_SIZE, TaskCanTxBuffer, &TaskCanTxControlBlock);
    TaskCanTxHandle = osThreadCreate(osThread(TaskCanTx), NULL);

    /* definition and creation of Task100Hz */
    osThreadStaticDef(
        Task100Hz, RunTask100Hz, osPriorityBelowNormal, 0, TASK100HZ_STACK_SIZE, Task100HzBuffer,
        &Task100HzControlBlock);
    Task100HzHandle = osThreadCreate(osThread(Task100Hz), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
/* add threads, ... */
// According to Percpio documentation, vTraceEnable() should be the last
// function call before the scheduler starts.
#if (configUSE_TRACE_FACILITY == 1)
    vTraceEnable(TRC_INIT);
#endif
    /* USER CODE END RTOS_THREADS */

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
    RCC_OscInitTypeDef       RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef       RCC_ClkInitStruct = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInit     = { 0 };

    /** Initializes the CPU, AHB and APB busses clocks
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
    RCC_OscInitStruct.LSIState       = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC12;
    PeriphClkInit.Adc12ClockSelection  = RCC_ADC12PLLCLK_DIV1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
    hadc1.Instance                   = ADC1;
    hadc1.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;
    hadc1.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode          = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode    = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc1.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T3_TRGO;
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion       = 1;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    hadc1.Init.LowPowerAutoWait      = DISABLE;
    hadc1.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
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
    sConfig.Channel      = ADC_CHANNEL_3;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SingleDiff   = ADC_DIFFERENTIAL_ENDED;
    sConfig.SamplingTime = ADC_SAMPLETIME_19CYCLES_5;
    sConfig.OffsetNumber = ADC_OFFSET_3;
    sConfig.Offset       = TS_ADC_OFFSET;
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
    hadc2.Instance                   = ADC2;
    hadc2.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;
    hadc2.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc2.Init.ScanConvMode          = ADC_SCAN_ENABLE;
    hadc2.Init.ContinuousConvMode    = DISABLE;
    hadc2.Init.DiscontinuousConvMode = DISABLE;
    hadc2.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc2.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T3_TRGO;
    hadc2.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc2.Init.NbrOfConversion       = 3;
    hadc2.Init.DMAContinuousRequests = ENABLE;
    hadc2.Init.EOCSelection          = ADC_EOC_SEQ_CONV;
    hadc2.Init.LowPowerAutoWait      = DISABLE;
    hadc2.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
    if (HAL_ADC_Init(&hadc2) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure Regular Channel
     */
    sConfig.Channel      = ADC_CHANNEL_1;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SingleDiff   = ADC_SINGLE_ENDED;
    sConfig.SamplingTime = ADC_SAMPLETIME_61CYCLES_5;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset       = 0;
    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_3;
    sConfig.Rank    = ADC_REGULAR_RANK_2;
    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank    = ADC_REGULAR_RANK_3;
    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC2_Init 2 */

    /* USER CODE END ADC2_Init 2 */
}

/**
 * @brief CAN Initialization Function
 * @param None
 * @retval None
 */
static void MX_CAN_Init(void)
{
    /* USER CODE BEGIN CAN_Init 0 */

    /* USER CODE END CAN_Init 0 */

    /* USER CODE BEGIN CAN_Init 1 */

    /* USER CODE END CAN_Init 1 */
    hcan.Instance                  = CAN;
    hcan.Init.Prescaler            = 9;
    hcan.Init.Mode                 = CAN_MODE_NORMAL;
    hcan.Init.SyncJumpWidth        = CAN_SJW_4TQ;
    hcan.Init.TimeSeg1             = CAN_BS1_6TQ;
    hcan.Init.TimeSeg2             = CAN_BS2_1TQ;
    hcan.Init.TimeTriggeredMode    = DISABLE;
    hcan.Init.AutoBusOff           = ENABLE;
    hcan.Init.AutoWakeUp           = DISABLE;
    hcan.Init.AutoRetransmission   = ENABLE;
    hcan.Init.ReceiveFifoLocked    = ENABLE;
    hcan.Init.TransmitFifoPriority = ENABLE;
    if (HAL_CAN_Init(&hcan) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN CAN_Init 2 */
    Io_SharedCan_Init(&hcan, CanTxQueueOverflowCallBack, CanRxQueueOverflowCallBack);
    /* USER CODE END CAN_Init 2 */
}

/**
 * @brief IWDG Initialization Function
 * @param None
 * @retval None
 */
static void MX_IWDG_Init(void)
{
    /* USER CODE BEGIN IWDG_Init 0 */

    /* USER CODE END IWDG_Init 0 */

    /* USER CODE BEGIN IWDG_Init 1 */

    /* USER CODE END IWDG_Init 1 */
    hiwdg.Instance       = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
    hiwdg.Init.Window    = IWDG_WINDOW_DISABLE_VALUE;
    hiwdg.Init.Reload    = LSI_FREQUENCY / IWDG_PRESCALER / IWDG_RESET_FREQUENCY;
    if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN IWDG_Init 2 */
    Io_SharedSoftwareWatchdog_Init(Io_HardwareWatchdog_Refresh, Io_SoftwareWatchdog_TimeoutCallback);
    /* USER CODE END IWDG_Init 2 */
}

/**
 * @brief SPI2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI2_Init(void)
{
    /* USER CODE BEGIN SPI2_Init 0 */

    /* USER CODE END SPI2_Init 0 */

    /* USER CODE BEGIN SPI2_Init 1 */

    /* USER CODE END SPI2_Init 1 */
    /* SPI2 parameter configuration*/
    hspi2.Instance               = SPI2;
    hspi2.Init.Mode              = SPI_MODE_MASTER;
    hspi2.Init.Direction         = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize          = SPI_DATASIZE_8BIT;
    hspi2.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    hspi2.Init.CLKPhase          = SPI_PHASE_2EDGE;
    hspi2.Init.NSS               = SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    hspi2.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode            = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial     = 7;
    hspi2.Init.CRCLength         = SPI_CRC_LENGTH_DATASIZE;
    hspi2.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
    if (HAL_SPI_Init(&hspi2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI2_Init 2 */

    /* USER CODE END SPI2_Init 2 */
}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void)
{
    /* USER CODE BEGIN TIM2_Init 0 */

    /* USER CODE END TIM2_Init 0 */

    TIM_SlaveConfigTypeDef  sSlaveConfig  = { 0 };
    TIM_MasterConfigTypeDef sMasterConfig = { 0 };
    TIM_IC_InitTypeDef      sConfigIC     = { 0 };

    /* USER CODE BEGIN TIM2_Init 1 */

    /* USER CODE END TIM2_Init 1 */
    htim2.Instance               = TIM2;
    htim2.Init.Prescaler         = TIM2_PRESCALER;
    htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim2.Init.Period            = TIM2_AUTO_RELOAD_REG;
    htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }
    sSlaveConfig.SlaveMode       = TIM_SLAVEMODE_RESET;
    sSlaveConfig.InputTrigger    = TIM_TS_TI2FP2;
    sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sSlaveConfig.TriggerFilter   = 0;
    if (HAL_TIM_SlaveConfigSynchronization(&htim2, &sSlaveConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_FALLING;
    sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter    = 0;
    if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM2_Init 2 */

    /* USER CODE END TIM2_Init 2 */
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

    TIM_ClockConfigTypeDef  sClockSourceConfig = { 0 };
    TIM_MasterConfigTypeDef sMasterConfig      = { 0 };

    /* USER CODE BEGIN TIM3_Init 1 */

    /* USER CODE END TIM3_Init 1 */
    htim3.Instance               = TIM3;
    htim3.Init.Prescaler         = TIM3_PRESCALER - 1;
    htim3.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim3.Init.Period            = (TIMx_FREQUENCY / TIM3_PRESCALER) / ADC1_ADC2_FREQUENCY - 1;
    htim3.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM3_Init 2 */

    /* USER CODE END TIM3_Init 2 */
}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA2_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Channel1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    /* DMA2_Channel1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Channel1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA2_Channel1_IRQn);
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, BRUSA_PON_Pin | TSAL_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, PRE_CHARGE_EN_Pin | AIR_EN_Pin | BMS_OK_Pin | MCU_LATCH_RESET_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, STATUS_R_Pin | STATUS_G_Pin | STATUS_B_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin : IMD_OK_Pin */
    GPIO_InitStruct.Pin  = IMD_OK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(IMD_OK_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : BRUSA_PON_Pin TSAL_EN_Pin */
    GPIO_InitStruct.Pin   = BRUSA_PON_Pin | TSAL_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : PA0 */
    GPIO_InitStruct.Pin  = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : ACCEL_BRAKE_OK_Pin BRUSA_FLT_Pin GPIO2_Pin CHARGE_STATE_Pin */
    GPIO_InitStruct.Pin  = ACCEL_BRAKE_OK_Pin | BRUSA_FLT_Pin | GPIO2_Pin | CHARGE_STATE_Pin;
    GPIO_InitStruct.Pin =
        ACCEL_BRAKE_OK_Pin | BRUSA_FLT_Pin | GPIO2_Pin | CHARGE_STATE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : AIR_POWER_STATUS_Pin OC_SC_OK_Pin BSPD_OK_Pin */
    GPIO_InitStruct.Pin  = AIR_POWER_STATUS_Pin | OC_SC_OK_Pin | BSPD_OK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : PRE_CHARGE_EN_Pin AIR_EN_Pin BMS_OK_Pin MCU_LATCH_RESET_Pin */
    GPIO_InitStruct.Pin   = PRE_CHARGE_EN_Pin | AIR_EN_Pin | BMS_OK_Pin | MCU_LATCH_RESET_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : SPI2_NSS_Pin */
    GPIO_InitStruct.Pin   = SPI2_NSS_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SPI2_NSS_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : STATUS_R_Pin STATUS_G_Pin STATUS_B_Pin */
    GPIO_InitStruct.Pin   = STATUS_R_Pin | STATUS_G_Pin | STATUS_B_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : PB6 PB7 */
    GPIO_InitStruct.Pin       = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_RunTask1Hz */
/**
 * @brief  Function implementing the Task1Hz thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTask1Hz */
void RunTask1Hz(void const *argument)
{
    /* USER CODE BEGIN 5 */
    UNUSED(argument);
    uint32_t                 PreviousWakeTime = osKernelSysTick();
    static const TickType_t  period_ms        = 1000U;
    SoftwareWatchdogHandle_t watchdog         = Io_SharedSoftwareWatchdog_AllocateWatchdog();
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, "TASK_1HZ", period_ms);

    for (;;)
    {
        App_SharedStateMachine_Tick1Hz(state_machine);
        Io_StackWaterMark_Check();
        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        Io_SharedSoftwareWatchdog_CheckInWatchdog(watchdog);
        osDelayUntil(&PreviousWakeTime, period_ms);
    }
    /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_RunTask1kHz */
/**
 * @brief Function implementing the Task1kHz thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTask1kHz */
void RunTask1kHz(void const *argument)
{
    /* USER CODE BEGIN RunTask1kHz */
    UNUSED(argument);
    uint32_t                 PreviousWakeTime = osKernelSysTick();
    static const TickType_t  period_ms        = 1;
    SoftwareWatchdogHandle_t watchdog         = Io_SharedSoftwareWatchdog_AllocateWatchdog();
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, "TASK_1KHZ", period_ms);

    for (;;)
    {
        Io_SharedSoftwareWatchdog_CheckForTimeouts();
        const uint32_t task_start_ms = TICK_TO_MS(osKernelSysTick());

        App_SharedClock_SetCurrentTimeInMilliseconds(clock, task_start_ms);
        Io_CanTx_EnqueuePeriodicMsgs(can_tx, task_start_ms);

        // Watchdog check-in must be the last function called before putting the
        // task to sleep. Prevent check in if the elapsed period is greater or
        // equal to the period ms
        if ((TICK_TO_MS(osKernelSysTick()) - task_start_ms) <= period_ms)
        {
            Io_SharedSoftwareWatchdog_CheckInWatchdog(watchdog);
        }

        osDelayUntil(&PreviousWakeTime, period_ms);
    }
    /* USER CODE END RunTask1kHz */
}

/* USER CODE BEGIN Header_RunTaskCanRx */
/**
 * @brief Function implementing the TaskCanRx thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTaskCanRx */
void RunTaskCanRx(void const *argument)
{
    /* USER CODE BEGIN RunTaskCanRx */
    UNUSED(argument);

    for (;;)
    {
        struct CanMsg message;
        Io_SharedCan_DequeueCanRxMessage(&message);
        Io_CanRx_UpdateRxTableWithMessage(App_BmsWorld_GetCanRx(world), &message);
        Io_SharedErrorTable_SetErrorsFromCanMsg(error_table, &message);
    }
    /* USER CODE END RunTaskCanRx */
}

/* USER CODE BEGIN Header_RunTaskCanTx */
/**
 * @brief Function implementing the TaskCanTx thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTaskCanTx */
void RunTaskCanTx(void const *argument)
{
    /* USER CODE BEGIN RunTaskCanTx */
    UNUSED(argument);

    for (;;)
    {
        Io_SharedCan_TransmitEnqueuedCanTxMessagesFromTask();
    }
    /* USER CODE END RunTaskCanTx */
}

/* USER CODE BEGIN Header_RunTask100Hz */
/**
 * @brief Function implementing the Task100Hz thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTask100Hz */
void RunTask100Hz(void const *argument)
{
    /* USER CODE BEGIN RunTask100Hz */
    UNUSED(argument);
    uint32_t                 PreviousWakeTime = osKernelSysTick();
    static const TickType_t  period_ms        = 10;
    SoftwareWatchdogHandle_t watchdog         = Io_SharedSoftwareWatchdog_AllocateWatchdog();
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, "TASK_100HZ", period_ms);

    /* Infinite loop */
    for (;;)
    {
        App_SharedStateMachine_Tick100Hz(state_machine);

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        Io_SharedSoftwareWatchdog_CheckInWatchdog(watchdog);
        osDelayUntil(&PreviousWakeTime, period_ms);
    }
    /* USER CODE END RunTask100Hz */
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
    __assert_func(file, line, "Error_Handler", "Error_Handler");
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
void assert_failed(char *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    __assert_func(file, line, "assert_failed", "assert_failed");
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
