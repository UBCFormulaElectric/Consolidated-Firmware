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
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
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

#include "App_InvWorld.h"
#include "App_SharedStateMachine.h"
#include "App_GateDrive.h"
#include "App_Motor.h"
#include "App_PowerStage.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"
#include "configs/App_HeartbeatMonitorConfig.h"

#include "Io_CanTx.h"
#include "Io_CanRx.h"
#include "Io_SoftwareWatchdog.h"
#include "Io_StackWaterMark.h"
#include "Io_SharedCan.h"
#include "Io_SharedErrorHandlerOverride.h"
#include "Io_SharedHardFaultHandler.h"
#include "Io_HeartbeatMonitor.h"
#include "Io_RgbLedSequence.h"
#include "Io_STGAP1AS.h"
#include "Io_TimerPwmGen.h"
#include "Io_AdcDac.h"
#include "Io_ECI1118.h"
#include "Io_PowerStage.h"
#include "control/App_ControlLoop.h"

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

CAN_HandleTypeDef hcan1;

DAC_HandleTypeDef hdac;

IWDG_HandleTypeDef hiwdg;

SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi4;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim8;

osThreadId          Task1HzHandle;
uint32_t            Task1HzTaskBuffer[512];
osStaticThreadDef_t Task1HzTaskControlBlock;
osThreadId          Task1kHzHandle;
uint32_t            TaskCan1kHzBuffer[512];
osStaticThreadDef_t TaskCan1kHzControlBlock;
osThreadId          TaskCanRxHandle;
uint32_t            TaskCanRxBuffer[512];
osStaticThreadDef_t TaskCanRxControlBlock;
osThreadId          TaskCanTxHandle;
uint32_t            TaskCanTxBuffer[512];
osStaticThreadDef_t TaskCanTxControlBlock;
osThreadId          Task100HzHandle;
uint32_t            Task100HzBuffer[512];
osStaticThreadDef_t Task100HzControlBlock;
/* USER CODE BEGIN PV */
struct InvWorld *         world;
struct StateMachine *     state_machine;
struct InvCanTxInterface *can_tx;
struct InvCanRxInterface *can_rx;
struct HeartbeatMonitor * heartbeat_monitor;
struct RgbLedSequence *   rgb_led_sequence;
struct ErrorTable *       error_table;
struct Clock *            clock;
struct PowerStage *       power_stage;
struct Motor *            motor;
struct GateDrive *        gate_drive;

uint32_t adc_buffer[4];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void        SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_CAN1_Init(void);
static void MX_DAC_Init(void);
static void MX_IWDG_Init(void);
static void MX_SPI2_Init(void);
static void MX_SPI4_Init(void);
static void MX_TIM8_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
void        RunTask1Hz(void const *argument);
void        RunTask1kHz(void const *argument);
void        RunTaskCanRx(void const *argument);
void        RunTaskCanTx(void const *argument);
void        RunTask100Hz(void const *argument);

/* USER CODE BEGIN PFP */

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

    /* Enable I-Cache---------------------------------------------------------*/
    SCB_EnableICache();

    /* Enable D-Cache---------------------------------------------------------*/
    SCB_EnableDCache();

    /* MCU
     * Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the
     * Systick. */
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
    MX_ADC1_Init();
    MX_ADC2_Init();
    MX_CAN1_Init();
    MX_DAC_Init();
    MX_IWDG_Init();
    MX_SPI2_Init();
    MX_SPI4_Init();
    MX_TIM8_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();
    /* USER CODE BEGIN 2 */

    __HAL_DBGMCU_FREEZE_IWDG();
    SEGGER_SYSVIEW_Conf();

    Io_SharedHardFaultHandler_Init();

    can_tx = App_CanTx_Create(
        Io_CanTx_EnqueueNonPeriodicMsg_INV_WATCHDOG_TIMEOUT,
        Io_CanTx_EnqueueNonPeriodicMsg_INV_AIR_SHUTDOWN_ERRORS);

    can_rx = App_CanRx_Create();

    heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
        Io_HeartbeatMonitor_GetCurrentMs, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS,
        HEARTBEAT_MONITOR_BOARDS_TO_CHECK, Io_HeartbeatMonitor_TimeoutCallback);

    rgb_led_sequence = App_SharedRgbLedSequence_Create(
        Io_RgbLedSequence_TurnOnRedLed, Io_RgbLedSequence_TurnOnBlueLed,
        Io_RgbLedSequence_TurnOnGreenLed);

    error_table = App_SharedErrorTable_Create();

    clock = App_SharedClock_Create();

    gate_drive = App_GateDrive_Create(
        Io_STGAP1AS_WriteConfiguration, Io_STGAP1AS_ResetStatus,
        Io_STGAP1AS_GlobalReset, Io_STGAP1AS_WriteRegister,
        Io_STGAP1AS_ReadRegister, Io_STGAP1AS_GetFaults, Io_STGAP1AS_Command,
        Io_STGAP1AS_SetShutdownPin, Io_STGAP1AS_GetShutdownPin,
        Io_TimerPwmGen_LoadPwm, Io_TimerPwmGen_StartPwm, Io_TimerPwmGen_StopPwm,
        Io_TimerPwmGen_SetSwitchingFreq, Io_TimerPwmGen_SetDeadTime,
        Io_STGAP1AS_GetPhaHiDiag, Io_STGAP1AS_GetPhaLoDiag,
        Io_STGAP1AS_GetPhbHiDiag, Io_STGAP1AS_GetPhbLoDiag,
        Io_STGAP1AS_GetPhcHiDiag, Io_STGAP1AS_GetPhcLoDiag);

    //        motor = App_Motor_Create(Io_ECI1118_GetTemperature,
    //        Io_ECI1118_GetRotorAngle);

    power_stage = App_PowerStage_Create(
        Io_AdcDac_AdcContModeInit, Io_AdcDac_AdcPwmSyncModeInit,
        Io_AdcDac_AdcStart, Io_AdcDac_AdcStop, Io_AdcDac_DacStart,
        Io_AdcDac_DacSetCurrentLim, Io_AdcDac_GetPhaseCurrents,
        Io_AdcDac_CorrectOffset, Io_AdcDac_GetBusVoltage,
        Io_AdcDac_GetPowerstageTemp, Io_PowerStage_GetPhaOCFault,
        Io_PowerStage_GetPhbOCFault, Io_PowerStage_GetPhcOCFault,
        Io_PowerStage_GetPowerStageOTFault);

    world = App_InvWorld_Create(
        can_tx, can_rx, heartbeat_monitor, rgb_led_sequence, error_table, clock,
        gate_drive, power_stage);

    state_machine = App_SharedStateMachine_Create(world, App_GetInitState());

    Io_StackWaterMark_Init(can_tx);
    Io_SoftwareWatchdog_Init(can_tx);

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
    osThreadStaticDef(
        Task1Hz, RunTask1Hz, osPriorityLow, 0, 512, Task1HzTaskBuffer,
        &Task1HzTaskControlBlock);
    Task1HzHandle = osThreadCreate(osThread(Task1Hz), NULL);

    /* definition and creation of Task1kHz */
    osThreadStaticDef(
        Task1kHz, RunTask1kHz, osPriorityAboveNormal, 0, 512, TaskCan1kHzBuffer,
        &TaskCan1kHzControlBlock);
    Task1kHzHandle = osThreadCreate(osThread(Task1kHz), NULL);

    /* definition and creation of TaskCanRx */
    osThreadStaticDef(
        TaskCanRx, RunTaskCanRx, osPriorityIdle, 0, 512, TaskCanRxBuffer,
        &TaskCanRxControlBlock);
    TaskCanRxHandle = osThreadCreate(osThread(TaskCanRx), NULL);

    /* definition and creation of TaskCanTx */
    osThreadStaticDef(
        TaskCanTx, RunTaskCanTx, osPriorityIdle, 0, 512, TaskCanTxBuffer,
        &TaskCanTxControlBlock);
    TaskCanTxHandle = osThreadCreate(osThread(TaskCanTx), NULL);

    /* definition and creation of Task100Hz */
    osThreadStaticDef(
        Task100Hz, RunTask100Hz, osPriorityIdle, 0, 512, Task100HzBuffer,
        &Task100HzControlBlock);
    Task100HzHandle = osThreadCreate(osThread(Task100Hz), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
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
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the CPU, AHB and APB busses clocks
     */
    RCC_OscInitStruct.OscillatorType =
        RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState      = RCC_HSE_ON;
    RCC_OscInitStruct.LSIState      = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState  = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM      = 4;
    RCC_OscInitStruct.PLL.PLLN      = 216;
    RCC_OscInitStruct.PLL.PLLP      = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ      = 9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Activate the Over-Drive mode
     */
    if (HAL_PWREx_EnableOverDrive() != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
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

    ADC_ChannelConfTypeDef sConfig = { 0 };

    /* USER CODE BEGIN ADC1_Init 1 */

    /* USER CODE END ADC1_Init 1 */
    /** Configure the global features of the ADC (Clock, Resolution, Data
     * Alignment and number of conversion)
     */
    hadc1.Instance                   = ADC1;
    hadc1.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode          = ADC_SCAN_ENABLE;
    hadc1.Init.ContinuousConvMode    = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc1.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T8_TRGO2;
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion       = 3;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.EOCSelection          = ADC_EOC_SEQ_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in
     * the sequencer and its sample time.
     */
    sConfig.Channel      = ADC_CHANNEL_0;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in
     * the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_6;
    sConfig.Rank    = ADC_REGULAR_RANK_2;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in
     * the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_14;
    sConfig.Rank    = ADC_REGULAR_RANK_3;
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
    /** Configure the global features of the ADC (Clock, Resolution, Data
     * Alignment and number of conversion)
     */
    hadc2.Instance                   = ADC2;
    hadc2.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc2.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc2.Init.ScanConvMode          = ADC_SCAN_ENABLE;
    hadc2.Init.ContinuousConvMode    = DISABLE;
    hadc2.Init.DiscontinuousConvMode = DISABLE;
    hadc2.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc2.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T8_TRGO2;
    hadc2.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc2.Init.NbrOfConversion       = 4;
    hadc2.Init.DMAContinuousRequests = ENABLE;
    hadc2.Init.EOCSelection          = ADC_EOC_SEQ_CONV;
    if (HAL_ADC_Init(&hadc2) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in
     * the sequencer and its sample time.
     */
    sConfig.Channel      = ADC_CHANNEL_1;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in
     * the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_3;
    sConfig.Rank    = ADC_REGULAR_RANK_2;
    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in
     * the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_15;
    sConfig.Rank    = ADC_REGULAR_RANK_3;
    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in
     * the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_2;
    sConfig.Rank    = ADC_REGULAR_RANK_4;
    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC2_Init 2 */

    /* USER CODE END ADC2_Init 2 */
}

/**
 * @brief CAN1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_CAN1_Init(void)
{
    /* USER CODE BEGIN CAN1_Init 0 */

    /* USER CODE END CAN1_Init 0 */

    /* USER CODE BEGIN CAN1_Init 1 */

    /* USER CODE END CAN1_Init 1 */
    hcan1.Instance                  = CAN1;
    hcan1.Init.Prescaler            = 6;
    hcan1.Init.Mode                 = CAN_MODE_NORMAL;
    hcan1.Init.SyncJumpWidth        = CAN_SJW_4TQ;
    hcan1.Init.TimeSeg1             = CAN_BS1_15TQ;
    hcan1.Init.TimeSeg2             = CAN_BS2_2TQ;
    hcan1.Init.TimeTriggeredMode    = DISABLE;
    hcan1.Init.AutoBusOff           = ENABLE;
    hcan1.Init.AutoWakeUp           = DISABLE;
    hcan1.Init.AutoRetransmission   = DISABLE;
    hcan1.Init.ReceiveFifoLocked    = ENABLE;
    hcan1.Init.TransmitFifoPriority = ENABLE;
    if (HAL_CAN_Init(&hcan1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN CAN1_Init 2 */
    Io_SharedCan_Init(
        &hcan1, CanTxQueueOverflowCallBack, CanRxQueueOverflowCallBack);
    /* USER CODE END CAN1_Init 2 */
}

/**
 * @brief DAC Initialization Function
 * @param None
 * @retval None
 */
static void MX_DAC_Init(void)
{
    /* USER CODE BEGIN DAC_Init 0 */

    /* USER CODE END DAC_Init 0 */

    DAC_ChannelConfTypeDef sConfig = { 0 };

    /* USER CODE BEGIN DAC_Init 1 */

    /* USER CODE END DAC_Init 1 */
    /** DAC Initialization
     */
    hdac.Instance = DAC;
    if (HAL_DAC_Init(&hdac) != HAL_OK)
    {
        Error_Handler();
    }
    /** DAC channel OUT1 config
     */
    sConfig.DAC_Trigger      = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
    if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    /** DAC channel OUT2 config
     */
    if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN DAC_Init 2 */

    /* USER CODE END DAC_Init 2 */
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
    hiwdg.Init.Reload = LSI_FREQUENCY / IWDG_PRESCALER / IWDG_RESET_FREQUENCY;
    if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN IWDG_Init 2 */
    Io_SharedSoftwareWatchdog_Init(
        Io_HardwareWatchdog_Refresh, Io_SoftwareWatchdog_TimeoutCallback);
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
    hspi2.Init.DataSize          = SPI_DATASIZE_16BIT;
    hspi2.Init.CLKPolarity       = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase          = SPI_PHASE_2EDGE;
    hspi2.Init.NSS               = SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
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
 * @brief SPI4 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI4_Init(void)
{
    /* USER CODE BEGIN SPI4_Init 0 */

    /* USER CODE END SPI4_Init 0 */

    /* USER CODE BEGIN SPI4_Init 1 */

    /* USER CODE END SPI4_Init 1 */
    /* SPI4 parameter configuration*/
    hspi4.Instance               = SPI4;
    hspi4.Init.Mode              = SPI_MODE_MASTER;
    hspi4.Init.Direction         = SPI_DIRECTION_2LINES;
    hspi4.Init.DataSize          = SPI_DATASIZE_16BIT;
    hspi4.Init.CLKPolarity       = SPI_POLARITY_LOW;
    hspi4.Init.CLKPhase          = SPI_PHASE_1EDGE;
    hspi4.Init.NSS               = SPI_NSS_HARD_OUTPUT;
    hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    hspi4.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    hspi4.Init.TIMode            = SPI_TIMODE_DISABLE;
    hspi4.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    hspi4.Init.CRCPolynomial     = 7;
    hspi4.Init.CRCLength         = SPI_CRC_LENGTH_DATASIZE;
    hspi4.Init.NSSPMode          = SPI_NSS_PULSE_ENABLE;
    if (HAL_SPI_Init(&hspi4) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI4_Init 2 */

    /* USER CODE END SPI4_Init 2 */
}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void)
{
    /* USER CODE BEGIN TIM1_Init 0 */

    /* USER CODE END TIM1_Init 0 */

    TIM_ClockConfigTypeDef         sClockSourceConfig   = { 0 };
    TIM_MasterConfigTypeDef        sMasterConfig        = { 0 };
    TIM_OC_InitTypeDef             sConfigOC            = { 0 };
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

    /* USER CODE BEGIN TIM1_Init 1 */

    /* USER CODE END TIM1_Init 1 */
    htim1.Instance               = TIM1;
    htim1.Init.Prescaler         = 108 - 1;
    htim1.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim1.Init.Period            = 8 - 1;
    htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_OC_Init(&htim1) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger  = TIM_TRGO_RESET;
    sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
    sMasterConfig.MasterSlaveMode      = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode       = TIM_OCMODE_ACTIVE;
    sConfigOC.Pulse        = 0;
    sConfigOC.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode   = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState  = TIM_OCIDLESTATE_SET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    sBreakDeadTimeConfig.OffStateRunMode  = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel        = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime         = 0;
    sBreakDeadTimeConfig.BreakState       = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity    = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.BreakFilter      = 0;
    sBreakDeadTimeConfig.Break2State      = TIM_BREAK2_DISABLE;
    sBreakDeadTimeConfig.Break2Polarity   = TIM_BREAK2POLARITY_HIGH;
    sBreakDeadTimeConfig.Break2Filter     = 0;
    sBreakDeadTimeConfig.AutomaticOutput  = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM1_Init 2 */

    /* USER CODE END TIM1_Init 2 */
    HAL_TIM_MspPostInit(&htim1);
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

    TIM_ClockConfigTypeDef  sClockSourceConfig = { 0 };
    TIM_MasterConfigTypeDef sMasterConfig      = { 0 };
    TIM_OC_InitTypeDef      sConfigOC          = { 0 };

    /* USER CODE BEGIN TIM2_Init 1 */

    /* USER CODE END TIM2_Init 1 */
    htim2.Instance               = TIM2;
    htim2.Init.Prescaler         = 1 - 1;
    htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim2.Init.Period            = 324000 - 1;
    htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode     = TIM_OCMODE_TIMING;
    sConfigOC.Pulse      = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM2_Init 2 */

    /* USER CODE END TIM2_Init 2 */
}

/**
 * @brief TIM8 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM8_Init(void)
{
    /* USER CODE BEGIN TIM8_Init 0 */

    /* USER CODE END TIM8_Init 0 */

    TIM_ClockConfigTypeDef         sClockSourceConfig   = { 0 };
    TIM_MasterConfigTypeDef        sMasterConfig        = { 0 };
    TIM_OC_InitTypeDef             sConfigOC            = { 0 };
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

    /* USER CODE BEGIN TIM8_Init 1 */

    /* USER CODE END TIM8_Init 1 */
    htim8.Instance               = TIM8;
    htim8.Init.Prescaler         = 1 - 1;
    htim8.Init.CounterMode       = TIM_COUNTERMODE_CENTERALIGNED1;
    htim8.Init.Period            = 10800 - 1;
    htim8.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim8.Init.RepetitionCounter = 0;
    htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_Init(&htim8) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger  = TIM_TRGO_RESET;
    sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_OC4REF_RISING_OC6REF_FALLING;
    sMasterConfig.MasterSlaveMode      = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode       = TIM_OCMODE_PWM1;
    sConfigOC.Pulse        = 0;
    sConfigOC.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode   = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState  = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM2;
    sConfigOC.Pulse  = 10;
    if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_6) != HAL_OK)
    {
        Error_Handler();
    }
    sBreakDeadTimeConfig.OffStateRunMode  = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel        = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime         = 1;
    sBreakDeadTimeConfig.BreakState       = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity    = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.BreakFilter      = 0;
    sBreakDeadTimeConfig.Break2State      = TIM_BREAK2_DISABLE;
    sBreakDeadTimeConfig.Break2Polarity   = TIM_BREAK2POLARITY_HIGH;
    sBreakDeadTimeConfig.Break2Filter     = 0;
    sBreakDeadTimeConfig.AutomaticOutput  = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM8_Init 2 */

    /* USER CODE END TIM8_Init 2 */
    HAL_TIM_MspPostInit(&htim8);
}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA2_Stream0_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
    /* DMA2_Stream2_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
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
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(
        GPIOE, FLASH_nWP_Pin | ENDAT_DATA_SEND_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(
        GPIOC, nSHDN_GDRV_MCU_Pin | ENDAT_DATA_TX_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(nSHDN_GDRV_GPIO_Port, nSHDN_GDRV_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GDRV_SPI_CS_GPIO_Port, GDRV_SPI_CS_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(ENDAT_CLK_EN_GPIO_Port, ENDAT_CLK_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD_1_GPIO_Port, GPIOD_1_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : FLASH_nWP_Pin ENDAT_DATA_SEND_Pin */
    GPIO_InitStruct.Pin   = FLASH_nWP_Pin | ENDAT_DATA_SEND_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : nDIAG_PHB_HS_Pin nDIAG_PHC_LS_Pin nDIAG_PHC_HS_Pin
     */
    GPIO_InitStruct.Pin =
        nDIAG_PHB_HS_Pin | nDIAG_PHC_LS_Pin | nDIAG_PHC_HS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : nSHDN_GDRV_MCU_Pin nSHDN_GDRV_Pin ENDAT_DATA_TX_Pin
     */
    GPIO_InitStruct.Pin =
        nSHDN_GDRV_MCU_Pin | nSHDN_GDRV_Pin | ENDAT_DATA_TX_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : PHA_LIN_IN_Pin PHA_HIN_IN_Pin PHC_HIN_IN_Pin
     * ENDAT_DATA_RX_Pin */
    GPIO_InitStruct.Pin =
        PHA_LIN_IN_Pin | PHA_HIN_IN_Pin | PHC_HIN_IN_Pin | ENDAT_DATA_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : nPHC_OC_ALARM_Pin */
    GPIO_InitStruct.Pin  = nPHC_OC_ALARM_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(nPHC_OC_ALARM_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : GDRV_SPI_CS_Pin */
    GPIO_InitStruct.Pin   = GDRV_SPI_CS_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GDRV_SPI_CS_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : nDIAG_PHA_LS_Pin nDIAG_PHA_HS_Pin */
    GPIO_InitStruct.Pin  = nDIAG_PHA_LS_Pin | nDIAG_PHA_HS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : nDIAG_PHB_LS_Pin GPIOD_2_Pin */
    GPIO_InitStruct.Pin  = nDIAG_PHB_LS_Pin | GPIOD_2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pin : nMOD_OT_ALARM_Pin */
    GPIO_InitStruct.Pin  = nMOD_OT_ALARM_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(nMOD_OT_ALARM_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : nMOTOR_OT_ALARM_Pin */
    GPIO_InitStruct.Pin  = nMOTOR_OT_ALARM_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(nMOTOR_OT_ALARM_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : USB_DM_Pin USB_DP_Pin */
    GPIO_InitStruct.Pin       = USB_DM_Pin | USB_DP_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : ENDAT_CLK_EN_Pin */
    GPIO_InitStruct.Pin   = ENDAT_CLK_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ENDAT_CLK_EN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : CHANGE_THIS_REVC_Pin */
    GPIO_InitStruct.Pin  = CHANGE_THIS_REVC_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(CHANGE_THIS_REVC_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : GPIOD_1_Pin */
    GPIO_InitStruct.Pin   = GPIOD_1_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD_1_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : PREV_LED_OUT_Pin PREV_LED_OUTB5_Pin
     * PREV_LED_OUTB6_Pin */
    GPIO_InitStruct.Pin =
        PREV_LED_OUT_Pin | PREV_LED_OUTB5_Pin | PREV_LED_OUTB6_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : nPHA_OC_ALARM_Pin nPHB_OC_ALARM_Pin */
    GPIO_InitStruct.Pin  = nPHA_OC_ALARM_Pin | nPHB_OC_ALARM_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
    }
    if (hadc->Instance == ADC2)
    {
        SEGGER_SYSVIEW_RecordEnterISR();
        App_ControlLoop_Run(world);
        SEGGER_SYSVIEW_RecordExitISR();
    }
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        Io_ECI1118_ClockRisingEdge();
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // TODO once board revision changes, commented pins will also be interrupt
    // triggered. Don't have the correct pins atm. asyncronously shut down the
    // gate drive if any fault pin interrupt is triggered
    App_GateDrive_Shutdown(gate_drive);
    if (GPIO_Pin == nPHA_OC_ALARM_Pin)
    {
        App_CanTx_SetPeriodicSignal_PHA_OC_ALARM(can_tx, 1);
    }
    else if (GPIO_Pin == nPHB_OC_ALARM_Pin)
    {
        App_CanTx_SetPeriodicSignal_PHB_OC_ALARM(can_tx, 1);
    }
    else if (GPIO_Pin == nPHC_OC_ALARM_Pin)
    {
        App_CanTx_SetPeriodicSignal_PHC_OC_ALARM(can_tx, 1);
    }
    //    else if(GPIO_Pin == nMOD_OT_ALARM_Pin)
    //    {
    //        App_CanTx_SetPeriodicSignal_PWRSTG_OT_ALARM(can_tx, 1);
    //    }
    //    else if(GPIO_Pin == nMOTOR_OT_ALARM_Pin)
    //    {
    //        App_CanTx_SetPeriodicSignal_MOTOR_OT_ALARM(can_tx, 1);
    //    }
    else if (GPIO_Pin == nDIAG_PHA_LS_Pin)
    {
        App_CanTx_SetPeriodicSignal_PHA_LO_DIAG(can_tx, 1);
    }
    else if (GPIO_Pin == nDIAG_PHA_HS_Pin)
    {
        App_CanTx_SetPeriodicSignal_PHA_HI_DIAG(can_tx, 1);
    }
    //    else if(GPIO_Pin == nDIAG_PHB_LS_Pin)
    //    {
    //        App_CanTx_SetPeriodicSignal_PHB_LO_DIAG(can_tx, 1);
    //    }
    else if (GPIO_Pin == nDIAG_PHB_HS_Pin)
    {
        App_CanTx_SetPeriodicSignal_PHB_HI_DIAG(can_tx, 1);
    }
    else if (GPIO_Pin == nDIAG_PHC_LS_Pin)
    {
        App_CanTx_SetPeriodicSignal_PHC_LO_DIAG(can_tx, 1);
    }
    else if (GPIO_Pin == nDIAG_PHC_HS_Pin)
    {
        App_CanTx_SetPeriodicSignal_PHC_HI_DIAG(can_tx, 1);
    }
}

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
    SoftwareWatchdogHandle_t watchdog =
        Io_SharedSoftwareWatchdog_AllocateWatchdog();
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
    SoftwareWatchdogHandle_t watchdog =
        Io_SharedSoftwareWatchdog_AllocateWatchdog();
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, "TASK_1KHZ", period_ms);
    for (;;)
    {
        const uint32_t current_time_ms = osKernelSysTick() * portTICK_PERIOD_MS;
        App_SharedClock_SetCurrentTimeInMilliseconds(clock, current_time_ms);
        Io_CanTx_EnqueuePeriodicMsgs(can_tx, current_time_ms);
        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        Io_SharedSoftwareWatchdog_CheckInWatchdog(watchdog);
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
        Io_CanRx_UpdateRxTableWithMessage(
            App_InvWorld_GetCanRx(world), &message);
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
    SoftwareWatchdogHandle_t watchdog =
        Io_SharedSoftwareWatchdog_AllocateWatchdog();
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
    if (htim->Instance == TIM1)
    {
        Io_ECI1118_ClockFallingEdge();
    }
    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM6)
    {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */
    if (htim->Instance == TIM8)
    {
        SEGGER_SYSVIEW_RecordEnterISR();

        SEGGER_SYSVIEW_RecordExitISR();
    }
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
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    __assert_func(file, line, "assert_failed", "assert_failed");
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
