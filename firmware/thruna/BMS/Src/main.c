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
#include "Io_Imd.h"
#include "Io_SharedHeartbeatMonitor.h"
#include "Io_RgbLedSequence.h"
#include "Io_Charger.h"
#include "Io_CurrentSense.h"
#include "Io_OkStatuses.h"
#include "Io_LTC6813/Io_LTC6813Shared.h"
#include "Io_LTC6813/Io_LTC6813CellVoltages.h"
#include "Io_LTC6813/Io_LTC6813CellTemperatures.h"
#include "Io_Airs.h"
#include "Io_PreCharge.h"
#include "Io_Adc.h"
#include "Io_VoltageSense.h"
#include "Io_Eeprom.h"
#include "Io_LatchedFaults.h"
#include "Io_ThermistorReadings.h"

#include "App_CanUtils.h"
#include "App_CanAlerts.h"
#include "App_BmsWorld.h"
#include "states/App_AllStates.h"
#include "App_SharedMacros.h"
#include "App_SharedStateMachine.h"
#include "states/App_InitState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "configs/App_ImdConfig.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define I2C_RECOVER_NUM_CLOCKS 10U    /* # clock cycles for recovery  */
#define I2C_RECOVER_CLOCK_FREQ 50000U /* clock frequency for recovery */
#define I2C_RECOVER_CLOCK_DELAY_MS (1000U / (2U * I2C_RECOVER_CLOCK_FREQ))
#define I2C1_SCL_PIN GPIO_PIN_6
#define I2C1_SCL_PORT GPIOB

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

CAN_HandleTypeDef hcan1;

I2C_HandleTypeDef hi2c1;

IWDG_HandleTypeDef hiwdg;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim13;

osThreadId          Task100HzHandle;
uint32_t            Task100HzBuffer[TASK100HZ_STACK_SIZE];
osStaticThreadDef_t Task100HzControlBlock;
osThreadId          TaskCanRxHandle;
uint32_t            TaskCanRxBuffer[TASKCANRX_STACK_SIZE];
osStaticThreadDef_t TaskCanRxControlBlock;
osThreadId          TaskCanTxHandle;
uint32_t            TaskCanTxBuffer[TASKCANTX_STACK_SIZE];
osStaticThreadDef_t TaskCanTxControlBlock;
osThreadId          Task1kHzHandle;
uint32_t            Task1kHzBuffer[TASK1KHZ_STACK_SIZE];
osStaticThreadDef_t Task1kHzControlBlock;
osThreadId          Task1HzHandle;
uint32_t            Task1HzBuffer[TASK1HZ_STACK_SIZE];
osStaticThreadDef_t Task1HzControlBlock;
/* USER CODE BEGIN PV */
struct BmsWorld *        world;
struct StateMachine *    state_machine;
struct Imd *             imd;
struct HeartbeatMonitor *heartbeat_monitor;
struct RgbLedSequence *  rgb_led_sequence;
struct Charger *         charger;
struct OkStatus *        bms_ok;
struct OkStatus *        imd_ok;
struct OkStatus *        bspd_ok;
struct Accumulator *     accumulator;
struct CellMonitors *    cell_monitors;
struct Airs *            airs;
struct PrechargeRelay *  precharge_relay;
struct TractiveSystem *  ts;
struct Clock *           clock;
struct Eeprom *          eeprom;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void        SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_CAN1_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM13_Init(void);
static void MX_IWDG_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM3_Init(void);
void        RunTask100Hz(void const *argument);
void        RunTaskCanRx(void const *argument);
void        RunTaskCanTx(void const *argument);
void        RunTask1kHz(void const *argument);
void        RunTask1Hz(void const *argument);

/* USER CODE BEGIN PFP */

static void CanRxQueueOverflowCallBack(size_t overflow_count);
static void CanTxQueueOverflowCallBack(size_t overflow_count);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void CanRxQueueOverflowCallBack(size_t overflow_count)
{
    App_CanTx_BMS_AlertsContext_RxOverflowCount_Set(overflow_count);
    App_CanAlerts_SetWarning(BMS_WARNING_RX_OVERFLOW, true);
}

static void CanTxQueueOverflowCallBack(size_t overflow_count)
{
    App_CanTx_BMS_AlertsContext_TxOverflowCount_Set(overflow_count);
    App_CanAlerts_SetWarning(BMS_WARNING_TX_OVERFLOW, true);
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
    HAL_Delay(1000);
    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_CAN1_Init();
    MX_SPI1_Init();
    MX_TIM1_Init();
    MX_TIM13_Init();
    MX_IWDG_Init();
    MX_I2C1_Init();
    MX_TIM3_Init();
    /* USER CODE BEGIN 2 */
    __HAL_DBGMCU_FREEZE_IWDG();

    // Configure and initialize SEGGER SystemView.
    SEGGER_SYSVIEW_Conf();

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)Io_Adc_GetRawAdcValues(), hadc1.Init.NbrOfConversion);
    HAL_TIM_Base_Start(&htim13);

    Io_SharedHardFaultHandler_Init();
    Io_SharedSoftwareWatchdog_Init(Io_HardwareWatchdog_Refresh, Io_SoftwareWatchdog_TimeoutCallback);
    Io_SharedCan_Init(&hcan1, CanTxQueueOverflowCallBack, CanRxQueueOverflowCallBack);
    Io_CanTx_EnableMode(CAN_MODE_DEFAULT, true);

    App_CanTx_Init();
    App_CanRx_Init();

    Io_Imd_Init();
    imd = App_Imd_Create(Io_Imd_GetFrequency, IMD_FREQUENCY_TOLERANCE, Io_Imd_GetDutyCycle, Io_Imd_GetTimeSincePowerOn);

    heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
        Io_SharedHeartbeatMonitor_GetCurrentMs, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);

    rgb_led_sequence = App_SharedRgbLedSequence_Create(
        Io_RgbLedSequence_TurnOnRedLed, Io_RgbLedSequence_TurnOnBlueLed, Io_RgbLedSequence_TurnOnGreenLed);

    charger = App_Charger_Create(Io_Charger_Enable, Io_Charger_Disable, Io_Charger_IsConnected, Io_Charger_HasFaulted);

    bms_ok = App_OkStatus_Create(Io_OkStatuses_EnableBmsOk, Io_OkStatuses_DisableBmsOk, Io_OkStatuses_IsBmsOkEnabled);

    imd_ok = App_OkStatus_Create(Io_OkStatuses_EnableImdOk, Io_OkStatuses_DisableImdOk, Io_OkStatuses_IsImdOkEnabled);

    bspd_ok =
        App_OkStatus_Create(Io_OkStatuses_EnableBspdOk, Io_OkStatuses_DisableBspdOk, Io_OkStatuses_IsBspdOkEnabled);

    Io_LTC6813Shared_InitSpiHandle(&hspi1);
    accumulator = App_Accumulator_Create(
        Io_LTC6813Shared_SetCfgRegsToDefaultSettings, Io_LTC6813Shared_WriteConfigurationRegisters,
        Io_LTC6813CellVoltages_StartAdcConversion, Io_LTC6813CellVoltages_ReadVoltages,
        Io_LTC6813CellTemperatures_StartAdcConversion, Io_LTC6813CellTemperatures_ReadTemperatures,
        Io_LTC6813CellTemperatures_GetMinTempDegC, Io_LTC6813CellTemperatures_GetMaxTempDegC,
        Io_LTC6813CellTemperatures_GetAverageTempDegC, Io_LTC6813Shared_EnableBalance, Io_LTC6813Shared_DisableBalance,
        Io_LatchedFaults_CheckImdLatchedFault, Io_LatchedFaults_CheckBspdLatchedFault,
        Io_LatchedFaults_CheckBmsLatchedFault, Io_ThermistorReadings_MuxSelect, Io_ThermistorReadings_ReadSelectedTemp);

    ts = App_TractiveSystem_Create(
        Io_VoltageSense_GetTractiveSystemVoltage, Io_CurrentSense_GetHighResolutionMainCurrent,
        Io_CurrentSense_GetLowResolutionMainCurrent);

    airs = App_Airs_Create(
        Io_Airs_IsAirPositiveClosed, Io_Airs_IsAirNegativeClosed, Io_Airs_CloseAirPositive, Io_Airs_OpenAirPositive);

    precharge_relay = App_PrechargeRelay_Create(Io_PreCharge_Enable, Io_PreCharge_Disable);

    clock = App_SharedClock_Create();

    eeprom = App_Eeprom_Create(Io_Eeprom_WritePage, Io_Eeprom_ReadPage, Io_Eeprom_PageErase);

    world = App_BmsWorld_Create(
        imd, heartbeat_monitor, rgb_led_sequence, charger, bms_ok, imd_ok, bspd_ok, accumulator, airs, precharge_relay,
        ts, clock, eeprom);

    state_machine = App_SharedStateMachine_Create(world, App_GetInitState());
    App_AllStates_Init();

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
    /* definition and creation of Task100Hz */
    osThreadStaticDef(
        Task100Hz, RunTask100Hz, osPriorityBelowNormal, 0, TASK100HZ_STACK_SIZE, Task100HzBuffer,
        &Task100HzControlBlock);
    Task100HzHandle = osThreadCreate(osThread(Task100Hz), NULL);

    /* definition and creation of TaskCanRx */
    osThreadStaticDef(
        TaskCanRx, RunTaskCanRx, osPriorityIdle, 0, TASKCANRX_STACK_SIZE, TaskCanRxBuffer, &TaskCanRxControlBlock);
    TaskCanRxHandle = osThreadCreate(osThread(TaskCanRx), NULL);

    /* definition and creation of TaskCanTx */
    osThreadStaticDef(
        TaskCanTx, RunTaskCanTx, osPriorityIdle, 0, TASKCANTX_STACK_SIZE, TaskCanTxBuffer, &TaskCanTxControlBlock);
    TaskCanTxHandle = osThreadCreate(osThread(TaskCanTx), NULL);

    /* definition and creation of Task1kHz */
    osThreadStaticDef(
        Task1kHz, RunTask1kHz, osPriorityNormal, 0, TASK1KHZ_STACK_SIZE, Task1kHzBuffer, &Task1kHzControlBlock);
    Task1kHzHandle = osThreadCreate(osThread(Task1kHz), NULL);

    /* definition and creation of Task1Hz */
    osThreadStaticDef(Task1Hz, RunTask1Hz, osPriorityLow, 0, TASK1HZ_STACK_SIZE, Task1HzBuffer, &Task1HzControlBlock);
    Task1HzHandle = osThreadCreate(osThread(Task1Hz), NULL);

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
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.LSIState       = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = 8;
    RCC_OscInitStruct.PLL.PLLN       = 192;
    RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ       = 2;
    RCC_OscInitStruct.PLL.PLLR       = 2;
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

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
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
    /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
     */
    hadc1.Instance                   = ADC1;
    hadc1.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode          = ENABLE;
    hadc1.Init.ContinuousConvMode    = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion       = 7;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel      = ADC_CHANNEL_3;
    sConfig.Rank         = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_7;
    sConfig.Rank    = 2;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_8;
    sConfig.Rank    = 3;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_9;
    sConfig.Rank    = 4;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_10;
    sConfig.Rank    = 5;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_11;
    sConfig.Rank    = 6;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_14;
    sConfig.Rank    = 7;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */
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
    hcan1.Init.Prescaler            = 12;
    hcan1.Init.Mode                 = CAN_MODE_NORMAL;
    hcan1.Init.SyncJumpWidth        = CAN_SJW_4TQ;
    hcan1.Init.TimeSeg1             = CAN_BS1_6TQ;
    hcan1.Init.TimeSeg2             = CAN_BS2_1TQ;
    hcan1.Init.TimeTriggeredMode    = DISABLE;
    hcan1.Init.AutoBusOff           = ENABLE;
    hcan1.Init.AutoWakeUp           = DISABLE;
    hcan1.Init.AutoRetransmission   = ENABLE;
    hcan1.Init.ReceiveFifoLocked    = ENABLE;
    hcan1.Init.TransmitFifoPriority = ENABLE;
    if (HAL_CAN_Init(&hcan1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN CAN1_Init 2 */
    /* USER CODE END CAN1_Init 2 */
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

    // Manually pulse the I2C clock signal 10 times, this can recover the I2C bus from a locked state after a reflash or
    // reset

    // To manually pulse must first set I2C pins to GPIO General Purpose Output Open-Drain
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin = I2C1_SCL_PIN;
    HAL_GPIO_Init(I2C1_SCL_PORT, &GPIO_InitStruct);

    for (uint8_t i = 0U; i < I2C_RECOVER_NUM_CLOCKS; ++i)
    {
        HAL_Delay(I2C_RECOVER_CLOCK_DELAY_MS);

        HAL_GPIO_WritePin(I2C1_SCL_PORT, I2C1_SCL_PIN, GPIO_PIN_RESET);

        HAL_Delay(I2C_RECOVER_CLOCK_DELAY_MS);

        HAL_GPIO_WritePin(I2C1_SCL_PORT, I2C1_SCL_PIN, GPIO_PIN_RESET);
    }

    // Set pins back to GPIO Alternate Function

    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;

    GPIO_InitStruct.Pin = I2C1_SCL_PIN;
    HAL_GPIO_Init(I2C1_SCL_PORT, &GPIO_InitStruct);

    /* USER CODE END I2C1_Init 1 */
    hi2c1.Instance             = I2C1;
    hi2c1.Init.ClockSpeed      = 400000;
    hi2c1.Init.DutyCycle       = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1     = 0;
    hi2c1.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2     = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C1_Init 2 */

    /* USER CODE END I2C1_Init 2 */
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
    hiwdg.Init.Reload    = LSI_FREQUENCY / IWDG_PRESCALER / IWDG_RESET_FREQUENCY;
    if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN IWDG_Init 2 */

    /* USER CODE END IWDG_Init 2 */
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void)
{
    /* USER CODE BEGIN SPI1_Init 0 */

    /* USER CODE END SPI1_Init 0 */

    /* USER CODE BEGIN SPI1_Init 1 */

    /* USER CODE END SPI1_Init 1 */
    /* SPI1 parameter configuration*/
    hspi1.Instance               = SPI1;
    hspi1.Init.Mode              = SPI_MODE_MASTER;
    hspi1.Init.Direction         = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize          = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    hspi1.Init.CLKPhase          = SPI_PHASE_2EDGE;
    hspi1.Init.NSS               = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
    hspi1.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode            = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial     = 10;
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI1_Init 2 */

    /* USER CODE END SPI1_Init 2 */
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

    TIM_SlaveConfigTypeDef  sSlaveConfig  = { 0 };
    TIM_MasterConfigTypeDef sMasterConfig = { 0 };
    TIM_IC_InitTypeDef      sConfigIC     = { 0 };

    /* USER CODE BEGIN TIM1_Init 1 */

    /* USER CODE END TIM1_Init 1 */
    htim1.Instance               = TIM1;
    htim1.Init.Prescaler         = TIM1_PRESCALER;
    htim1.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim1.Init.Period            = TIM1_AUTO_RELOAD_REG;
    htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
    {
        Error_Handler();
    }
    sSlaveConfig.SlaveMode       = TIM_SLAVEMODE_RESET;
    sSlaveConfig.InputTrigger    = TIM_TS_TI2FP2;
    sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sSlaveConfig.TriggerFilter   = 0;
    if (HAL_TIM_SlaveConfigSynchro(&htim1, &sSlaveConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_FALLING;
    sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter    = 0;
    if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM1_Init 2 */
    /* USER CODE END TIM1_Init 2 */
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
    htim3.Init.Period            = (TIMx_FREQUENCY / TIM3_PRESCALER / ADC_FREQUENCY) - 1;
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
 * @brief TIM13 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM13_Init(void)
{
    /* USER CODE BEGIN TIM13_Init 0 */

    /* USER CODE END TIM13_Init 0 */

    TIM_IC_InitTypeDef sConfigIC = { 0 };

    /* USER CODE BEGIN TIM13_Init 1 */

    /* USER CODE END TIM13_Init 1 */
    htim13.Instance               = TIM13;
    htim13.Init.Prescaler         = TIM1_PRESCALER;
    htim13.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim13.Init.Period            = TIM1_AUTO_RELOAD_REG;
    htim13.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim13.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim13) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_IC_Init(&htim13) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_FALLING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter    = 0;
    if (HAL_TIM_IC_ConfigChannel(&htim13, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM13_Init 2 */

    /* USER CODE END TIM13_Init 2 */
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
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(
        GPIOC, AIR_EN_Pin | PRE_CHARGE_EN_Pin | AUX_TSENSE_MUX1_Pin | AUX_TSENSE_MUX2_Pin | AUX_TSENSE_MUX3_Pin,
        GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, BMS_OK_3V3_Pin | LED_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, CHRG_EN_3V3_Pin | AUX_TSENSE_MUX0_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : AIR_PWR_STATUS_Pin TS_ISENSE_OCSC_OK_5V_Pin ACCEL_BRAKE_OK_5V_Pin BSPD_OK_3V3_Pin
                             CHRG_STATE_3V3_Pin CHRG_FLT_3V3_Pin */
    GPIO_InitStruct.Pin = AIR_PWR_STATUS_Pin | TS_ISENSE_OCSC_OK_5V_Pin | ACCEL_BRAKE_OK_5V_Pin | BSPD_OK_3V3_Pin |
                          CHRG_STATE_3V3_Pin | CHRG_FLT_3V3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : AIR_EN_Pin PRE_CHARGE_EN_Pin AUX_TSENSE_MUX1_Pin AUX_TSENSE_MUX2_Pin
                             AUX_TSENSE_MUX3_Pin */
    GPIO_InitStruct.Pin =
        AIR_EN_Pin | PRE_CHARGE_EN_Pin | AUX_TSENSE_MUX1_Pin | AUX_TSENSE_MUX2_Pin | AUX_TSENSE_MUX3_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : TSAL_RED_Pin */
    GPIO_InitStruct.Pin       = TSAL_RED_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(TSAL_RED_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : TSAL_GRN_Pin IMD_OK_3V3_Pin */
    GPIO_InitStruct.Pin  = TSAL_GRN_Pin | IMD_OK_3V3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : SPI1_NSS_Pin */
    GPIO_InitStruct.Pin   = SPI1_NSS_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SPI1_NSS_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : BMS_OK_3V3_Pin LED_Pin */
    GPIO_InitStruct.Pin   = BMS_OK_3V3_Pin | LED_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : BMS_LATCH_Pin IMD_LATCH_Pin BSPD_LATCH_Pin BSPD_I_TRIP_5V_Pin */
    GPIO_InitStruct.Pin  = BMS_LATCH_Pin | IMD_LATCH_Pin | BSPD_LATCH_Pin | BSPD_I_TRIP_5V_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : CHRG_EN_3V3_Pin AUX_TSENSE_MUX0_Pin */
    GPIO_InitStruct.Pin   = CHRG_EN_3V3_Pin | AUX_TSENSE_MUX0_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
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
void RunTask100Hz(void const *argument)
{
    /* USER CODE BEGIN 5 */
    UNUSED(argument);
    uint32_t                 PreviousWakeTime = osKernelSysTick();
    static const TickType_t  period_ms        = 10;
    SoftwareWatchdogHandle_t watchdog         = Io_SharedSoftwareWatchdog_AllocateWatchdog();
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, RTOS_TASK_100HZ, period_ms);

    /* Infinite loop */
    for (;;)
    {
        App_SharedStateMachine_Tick100Hz(state_machine);
        Io_CanTx_Enqueue100HzMsgs();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        Io_SharedSoftwareWatchdog_CheckInWatchdog(watchdog);
        osDelayUntil(&PreviousWakeTime, period_ms);
    }
    /* USER CODE END 5 */
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

    /* Infinite loop */
    for (;;)
    {
        struct CanMsg message;
        Io_SharedCan_DequeueCanRxMessage(&message);
        Io_CanRx_UpdateRxTableWithMessage(&message);
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

    /* Infinite loop */
    for (;;)
    {
        Io_SharedCan_TransmitEnqueuedCanTxMessagesFromTask();
    }
    /* USER CODE END RunTaskCanTx */
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
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, RTOS_TASK_1KHZ, period_ms);

    /* Infinite loop */
    for (;;)
    {
        // ADC wasn't reading any voltages when triggered by TIM3 like on other boards
        // But worked fine when starting the conversion via software as below
        // TODO: Figure out why
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)Io_Adc_GetRawAdcValues(), hadc1.Init.NbrOfConversion);

        // Check in for timeouts for all RTOS tasks
        Io_SharedSoftwareWatchdog_CheckForTimeouts();
        const uint32_t task_start_ms = TICK_TO_MS(osKernelSysTick());

        App_SharedClock_SetCurrentTimeInMilliseconds(clock, task_start_ms);
        Io_CanTx_EnqueueOtherPeriodicMsgs(task_start_ms);

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

/* USER CODE BEGIN Header_RunTask1Hz */
/**
 * @brief Function implementing the Task1Hz thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTask1Hz */
void RunTask1Hz(void const *argument)
{
    /* USER CODE BEGIN RunTask1Hz */
    UNUSED(argument);
    uint32_t                 PreviousWakeTime = osKernelSysTick();
    static const TickType_t  period_ms        = 1000U;
    SoftwareWatchdogHandle_t watchdog         = Io_SharedSoftwareWatchdog_AllocateWatchdog();
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, RTOS_TASK_1HZ, period_ms);

    /* Infinite loop */
    for (;;)
    {
        Io_StackWaterMark_Check();
        App_SharedStateMachine_Tick1Hz(state_machine);

        const bool debug_mode_enabled = App_CanRx_Debug_CanModes_EnableDebugMode_Get();
        Io_CanTx_EnableMode(CAN_MODE_DEBUG, debug_mode_enabled);
        Io_CanTx_Enqueue1HzMsgs();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        Io_SharedSoftwareWatchdog_CheckInWatchdog(watchdog);
        osDelayUntil(&PreviousWakeTime, period_ms);
    }
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
    __assert_func(__FILE__, __LINE__, "Error_Handler", "Error_Handler");
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
