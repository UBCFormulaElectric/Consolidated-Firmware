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
#include "Io_SharedHardFaultHandler.h"
#include "Io_StackWaterMark.h"
#include "Io_SoftwareWatchdog.h"
#include "Io_VoltageSense.h"
#include "Io_CurrentSense.h"
#include "Io_HeartbeatMonitor.h"
#include "Io_RgbLedSequence.h"
#include "Io_LT3650.h"
#include "Io_LTC3786.h"

#include "App_PdmWorld.h"
#include "App_SharedMacros.h"
#include "App_SharedConstants.h"
#include "App_SharedStateMachine.h"
#include "states/App_InitState.h"
#include "configs/App_CurrentLimits.h"
#include "configs/App_VoltageLimits.h"
#include "configs/App_HeartbeatMonitorConfig.h"
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

CAN_HandleTypeDef hcan;

IWDG_HandleTypeDef hiwdg;

SPI_HandleTypeDef hspi2;

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
struct PdmWorld *         world;
struct StateMachine *     state_machine;
struct PdmCanTxInterface *can_tx;
struct PdmCanRxInterface *can_rx;
struct InRangeCheck *     vbat_voltage_in_range_check;
struct InRangeCheck *     _24v_aux_voltage_in_range_check;
struct InRangeCheck *     _24v_acc_voltage_in_range_check;
struct InRangeCheck *     aux1_current_in_range_check;
struct InRangeCheck *     aux2_current_in_range_check;
struct InRangeCheck *     left_inverter_current_in_range_check;
struct InRangeCheck *     right_inverter_current_in_range_check;
struct InRangeCheck *     energy_meter_current_in_range_check;
struct InRangeCheck *     can_current_in_range_check;
struct InRangeCheck *     air_shutdown_current_in_range_check;
struct HeartbeatMonitor * heartbeat_monitor;
struct RgbLedSequence *   rgb_led_sequence;
struct LowVoltageBattery *low_voltage_battery;
struct Clock *            clock;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void        SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
static void MX_SPI2_Init(void);
static void MX_ADC1_Init(void);
static void MX_IWDG_Init(void);
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
    MX_CAN_Init();
    MX_SPI2_Init();
    MX_ADC1_Init();
    MX_IWDG_Init();
    /* USER CODE BEGIN 2 */
    __HAL_DBGMCU_FREEZE_IWDG();
    Io_SharedHardFaultHandler_Init();

    can_tx = App_CanTx_Create(
        Io_CanTx_EnqueueNonPeriodicMsg_PDM_STARTUP, Io_CanTx_EnqueueNonPeriodicMsg_PDM_WATCHDOG_TIMEOUT);

    can_rx = App_CanRx_Create();

    vbat_voltage_in_range_check =
        App_InRangeCheck_Create(Io_VoltageSense_GetVbatVoltage, VBAT_MIN_VOLTAGE, VBAT_MAX_VOLTAGE);

    _24v_aux_voltage_in_range_check =
        App_InRangeCheck_Create(Io_VoltageSense_Get24vAuxVoltage, _24V_AUX_MIN_VOLTAGE, _24V_AUX_MAX_VOLTAGE);

    _24v_acc_voltage_in_range_check =
        App_InRangeCheck_Create(Io_VoltageSense_Get24vAccVoltage, _24V_ACC_MIN_VOLTAGE, _24V_ACC_MAX_VOLTAGE);

    aux1_current_in_range_check =
        App_InRangeCheck_Create(Io_CurrentSense_GetAux1Current, AUX1_MIN_CURRENT, AUX1_MAX_CURRENT);

    aux2_current_in_range_check =
        App_InRangeCheck_Create(Io_CurrentSense_GetAux1Current, AUX2_MIN_CURRENT, AUX2_MAX_CURRENT);

    left_inverter_current_in_range_check = App_InRangeCheck_Create(
        Io_CurrentSense_GetLeftInverterCurrent, LEFT_INVERTER_MIN_CURRENT, LEFT_INVERTER_MAX_CURRENT);

    right_inverter_current_in_range_check = App_InRangeCheck_Create(
        Io_CurrentSense_GetRightInverterCurrent, RIGHT_INVERTER_MIN_CURRENT, RIGHT_INVERTER_MAX_CURRENT);

    energy_meter_current_in_range_check = App_InRangeCheck_Create(
        Io_CurrentSense_GetEnergyMeterCurrent, ENERGY_METER_MIN_CURRENT, ENERGY_METER_MAX_CURRENT);

    can_current_in_range_check =
        App_InRangeCheck_Create(Io_CurrentSense_GetCanCurrent, CAN_MIN_CURRENT, CAN_MAX_CURRENT);

    air_shutdown_current_in_range_check = App_InRangeCheck_Create(
        Io_CurrentSense_GetAirShutdownCurrent, AIR_SHUTDOWN_MIN_CURRENT, AIR_SHUTDOWN_MAX_CURRENT);

    heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
        Io_HeartbeatMonitor_GetCurrentMs, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK,
        Io_HeartbeatMonitor_TimeoutCallback);

    rgb_led_sequence = App_SharedRgbLedSequence_Create(
        Io_RgbLedSequence_TurnOnRedLed, Io_RgbLedSequence_TurnOnBlueLed, Io_RgbLedSequence_TurnOnGreenLed);

    low_voltage_battery = App_LowVoltageBattery_Create(Io_LT3650_HasFault, Io_LTC3786_HasFault);

    clock = App_SharedClock_Create();

    world = App_PdmWorld_Create(
        can_tx, can_rx, vbat_voltage_in_range_check, _24v_aux_voltage_in_range_check, _24v_acc_voltage_in_range_check,
        aux1_current_in_range_check, aux2_current_in_range_check, left_inverter_current_in_range_check,
        right_inverter_current_in_range_check, energy_meter_current_in_range_check, can_current_in_range_check,
        air_shutdown_current_in_range_check, heartbeat_monitor, rgb_led_sequence, low_voltage_battery, clock);

    state_machine = App_SharedStateMachine_Create(world, App_GetInitState());

    Io_SoftwareWatchdog_Init(can_tx);
    Io_StackWaterMark_Init(can_tx);

    struct CanMsgs_pdm_startup_t payload = { .dummy = 0 };
    App_CanTx_SendNonPeriodicMsg_PDM_STARTUP(can_tx, &payload);
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
    hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion       = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
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
    sConfig.Channel      = ADC_CHANNEL_1;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SingleDiff   = ADC_SINGLE_ENDED;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset       = 0;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */
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
    hspi2.Init.DataSize          = SPI_DATASIZE_4BIT;
    hspi2.Init.CLKPolarity       = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase          = SPI_PHASE_1EDGE;
    hspi2.Init.NSS               = SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi2.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode            = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial     = 7;
    hspi2.Init.CRCLength         = SPI_CRC_LENGTH_DATASIZE;
    hspi2.Init.NSSPMode          = SPI_NSS_PULSE_ENABLE;
    if (HAL_SPI_Init(&hspi2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI2_Init 2 */

    /* USER CODE END SPI2_Init 2 */
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
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, STATUS_R_Pin | STATUS_G_Pin | STATUS_B_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, PIN_AUX1_Pin | CSB_AUX1_AUX2_Pin | PIN_DI_FL_Pin | CSB_DI_FL_DI_FR_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(PIN_AUX2_GPIO_Port, PIN_AUX2_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, PIN_AIR_SHDN_Pin | PIN_LV_PWR_Pin | CSB_DI_BL_DI_BR_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, CSB_AIR_SHDN_LV_PWR_Pin | PIN_DI_BL_Pin | PIN_DI_BR_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(PIN_DI_FR_GPIO_Port, PIN_DI_FR_Pin, GPIO_PIN_SET);

    /*Configure GPIO pins : STATUS_R_Pin STATUS_G_Pin STATUS_B_Pin */
    GPIO_InitStruct.Pin   = STATUS_R_Pin | STATUS_G_Pin | STATUS_B_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : PIN_AUX1_Pin PIN_DI_FL_Pin */
    GPIO_InitStruct.Pin   = PIN_AUX1_Pin | PIN_DI_FL_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : PIN_AUX2_Pin */
    GPIO_InitStruct.Pin   = PIN_AUX2_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(PIN_AUX2_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : CUR_SYNC_AUX1_AUX2_Pin FSB_AUX1_AUX2_Pin FSOB_AUX1_AUX2_Pin CUR_SYNC_DI_FL_DI_FR_Pin */
    GPIO_InitStruct.Pin  = CUR_SYNC_AUX1_AUX2_Pin | FSB_AUX1_AUX2_Pin | FSOB_AUX1_AUX2_Pin | CUR_SYNC_DI_FL_DI_FR_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : CSB_AUX1_AUX2_Pin CSB_DI_FL_DI_FR_Pin */
    GPIO_InitStruct.Pin   = CSB_AUX1_AUX2_Pin | CSB_DI_FL_DI_FR_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : PIN_AIR_SHDN_Pin PIN_LV_PWR_Pin CSB_DI_BL_DI_BR_Pin */
    GPIO_InitStruct.Pin   = PIN_AIR_SHDN_Pin | PIN_LV_PWR_Pin | CSB_DI_BL_DI_BR_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : CUR_SYNC_AIR_SHDN_LV_PWR_Pin FSB_AIR_SHDN_LV_PWR_Pin FSOB_AIR_SHDN_LV_PWR_Pin CHRG_FAULT_Pin
                             GPIOB_4_Pin GPIOB_3_Pin GPIOB_2_Pin GPIOB_1_Pin */
    GPIO_InitStruct.Pin = CUR_SYNC_AIR_SHDN_LV_PWR_Pin | FSB_AIR_SHDN_LV_PWR_Pin | FSOB_AIR_SHDN_LV_PWR_Pin |
                          CHRG_FAULT_Pin | GPIOB_4_Pin | GPIOB_3_Pin | GPIOB_2_Pin | GPIOB_1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : CSB_AIR_SHDN_LV_PWR_Pin PIN_DI_BL_Pin */
    GPIO_InitStruct.Pin   = CSB_AIR_SHDN_LV_PWR_Pin | PIN_DI_BL_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : PIN_DI_BR_Pin */
    GPIO_InitStruct.Pin   = PIN_DI_BR_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(PIN_DI_BR_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : CUR_SYNC_DI_BL_DI_BR_Pin FSB_DI_BL_DI_BR_Pin FSOB_DI_BL_DI_BR_Pin FSB_DI_FL_DI_FR_Pin
                             FSOB_DI_FL_DI_FR_Pin PGOOD_Pin */
    GPIO_InitStruct.Pin = CUR_SYNC_DI_BL_DI_BR_Pin | FSB_DI_BL_DI_BR_Pin | FSOB_DI_BL_DI_BR_Pin | FSB_DI_FL_DI_FR_Pin |
                          FSOB_DI_FL_DI_FR_Pin | PGOOD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : PIN_DI_FR_Pin */
    GPIO_InitStruct.Pin   = PIN_DI_FR_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(PIN_DI_FR_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : CHRG_Pin */
    GPIO_InitStruct.Pin  = CHRG_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(CHRG_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PB5 */
    GPIO_InitStruct.Pin  = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
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
        Io_StackWaterMark_Check();
        App_SharedStateMachine_Tick1Hz(state_machine);

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
    static const TickType_t  period_ms        = 1U;
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
        Io_CanRx_UpdateRxTableWithMessage(can_rx, &message);
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
 * @note   This function is called  when TIM1 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM1)
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
