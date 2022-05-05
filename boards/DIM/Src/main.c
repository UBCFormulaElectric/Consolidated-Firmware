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

#include "App_DimWorld.h"
#include "App_SharedMacros.h"
#include "App_SevenSegDisplay.h"
#include "App_SharedStateMachine.h"
#include "states/App_DriveState.h"
#include "configs/App_RotarySwitchConfig.h"
#include "configs/App_RegenPaddleConfig.h"
#include "configs/App_HeartbeatMonitorConfig.h"

#include "Io_CanTx.h"
#include "Io_CanRx.h"
#include "Io_SoftwareWatchdog.h"
#include "Io_StackWaterMark.h"
#include "Io_SevenSegDisplays.h"
#include "Io_SharedCan.h"
#include "Io_SharedErrorHandlerOverride.h"
#include "Io_SharedHardFaultHandler.h"
#include "Io_HeartbeatMonitor.h"
#include "Io_RegenPaddle.h"
#include "Io_RgbLedSequence.h"
#include "Io_DriveModeSwitch.h"
#include "Io_Leds.h"
#include "Io_Switches.h"
#include "Io_Adc.h"
#include "Io_RgbLeds.h"
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
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc2;

CAN_HandleTypeDef hcan;

IWDG_HandleTypeDef hiwdg;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;

osThreadId          Task100HzHandle;
uint32_t            Task100HzTaskBuffer[TASK100HZ_STACK_SIZE];
osStaticThreadDef_t Task100HzTaskControlBlock;
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
struct DimWorld *         world;
struct StateMachine *     state_machine;
struct DimCanTxInterface *can_tx;
struct DimCanRxInterface *can_rx;
struct SevenSegDisplay *  left_seven_seg_display;
struct SevenSegDisplay *  middle_seven_seg_display;
struct SevenSegDisplay *  right_seven_seg_display;
struct SevenSegDisplays * seven_seg_displays;
struct HeartbeatMonitor * heartbeat_monitor;
struct RegenPaddle *      regen_paddle;
struct RgbLedSequence *   rgb_led_sequence;
struct RotarySwitch *     drive_mode_switch;
struct Led *              imd_led;
struct Led *              bspd_led;
struct BinarySwitch *     start_switch;
struct BinarySwitch *     traction_control_switch;
struct BinarySwitch *     torque_vectoring_switch;
struct ErrorTable *       error_table;
struct RgbLed *           bms_status_led;
struct RgbLed *           dcm_status_led;
struct RgbLed *           dim_status_led;
struct RgbLed *           fsm_status_led;
struct RgbLed *           pdm_status_led;
struct Clock *            clock;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void        SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_CAN_Init(void);
static void MX_ADC2_Init(void);
static void MX_IWDG_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM2_Init(void);
void        RunTask100Hz(void const *argument);
void        RunTaskCanRx(void const *argument);
void        RunTaskCanTx(void const *argument);
void        RunTask1kHz(void const *argument);
void        RunTask1Hz(void const *argument);

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
    MX_ADC2_Init();
    MX_IWDG_Init();
    MX_SPI2_Init();
    MX_TIM2_Init();
    /* USER CODE BEGIN 2 */
    __HAL_DBGMCU_FREEZE_IWDG();

    HAL_ADC_Start_DMA(&hadc2, (uint32_t *)Io_Adc_GetRawAdcValues(), hadc2.Init.NbrOfConversion);
    HAL_TIM_Base_Start(&htim2);

    Io_SharedHardFaultHandler_Init();

    Io_SevenSegDisplays_Init(&hspi2);

    left_seven_seg_display   = App_SevenSegDisplay_Create(Io_SevenSegDisplays_SetLeftHexDigit);
    middle_seven_seg_display = App_SevenSegDisplay_Create(Io_SevenSegDisplays_SetMiddleHexDigit);
    right_seven_seg_display  = App_SevenSegDisplay_Create(Io_SevenSegDisplays_SetRightHexDigit);

    seven_seg_displays = App_SevenSegDisplays_Create(
        left_seven_seg_display, middle_seven_seg_display, right_seven_seg_display, Io_SevenSegDisplays_WriteCommands);

    can_tx = App_CanTx_Create(
        Io_CanTx_EnqueueNonPeriodicMsg_DIM_STARTUP, Io_CanTx_EnqueueNonPeriodicMsg_DIM_WATCHDOG_TIMEOUT);

    can_rx = App_CanRx_Create();

    heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
        Io_HeartbeatMonitor_GetCurrentMs, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK,
        Io_HeartbeatMonitor_TimeoutCallback);

    regen_paddle = App_RegenPaddle_Create(
        Io_RegenPaddle_GetPaddlePosition, REGEN_PADDLE_LOWER_DEADZONE, REGEN_PADDLE_UPPER_DEADZONE);

    rgb_led_sequence = App_SharedRgbLedSequence_Create(
        Io_RgbLedSequence_TurnOnRedLed, Io_RgbLedSequence_TurnOnBlueLed, Io_RgbLedSequence_TurnOnGreenLed);

    drive_mode_switch = App_RotarySwitch_Create(Io_DriveModeSwitch_GetPosition, NUM_DRIVE_MODE_SWITCH_POSITIONS);

    imd_led = App_Led_Create(Io_Leds_TurnOnImdLed, Io_Leds_TurnOffImdLed);

    bspd_led = App_Led_Create(Io_Leds_TurnOnBspdLed, Io_Leds_TurnOffBspdLed);

    start_switch = App_BinarySwitch_Create(Io_Switches_StartSwitchIsTurnedOn);

    traction_control_switch = App_BinarySwitch_Create(Io_Switches_TractionControlSwitchIsTurnedOn);

    torque_vectoring_switch = App_BinarySwitch_Create(Io_Switches_TorqueVectoringSwitchIsTurnedOn);

    error_table = App_SharedErrorTable_Create();

    bms_status_led = App_SharedRgbLed_Create(
        Io_RgbLeds_TurnBmsStatusLedRed, Io_RgbLeds_TurnBmsStatusLedGreen, Io_RgbLeds_TurnBmsStatusLedBlue,
        Io_RgbLeds_TurnOffBmsStatusLed);

    dcm_status_led = App_SharedRgbLed_Create(
        Io_RgbLeds_TurnDcmStatusLedRed, Io_RgbLeds_TurnDcmStatusLedGreen, Io_RgbLeds_TurnDcmStatusLedBlue,
        Io_RgbLeds_TurnOffDcmStatusLed);

    dim_status_led = App_SharedRgbLed_Create(
        Io_RgbLeds_TurnDimStatusLedRed, Io_RgbLeds_TurnDimStatusLedGreen, Io_RgbLeds_TurnDimStatusLedBlue,
        Io_RgbLeds_TurnOffDimStatusLed);

    fsm_status_led = App_SharedRgbLed_Create(
        Io_RgbLeds_TurnFsmStatusLedRed, Io_RgbLeds_TurnFsmStatusLedGreen, Io_RgbLeds_TurnFsmStatusLedBlue,
        Io_RgbLeds_TurnOffFsmStatusLed);

    pdm_status_led = App_SharedRgbLed_Create(
        Io_RgbLeds_TurnPdmStatusLedRed, Io_RgbLeds_TurnPdmStatusLedGreen, Io_RgbLeds_TurnPdmStatusLedBlue,
        Io_RgbLeds_TurnOffPdmStatusLed);

    clock = App_SharedClock_Create();

    world = App_DimWorld_Create(
        can_tx, can_rx, seven_seg_displays, heartbeat_monitor, regen_paddle, rgb_led_sequence, drive_mode_switch,
        imd_led, bspd_led, start_switch, traction_control_switch, torque_vectoring_switch, error_table, bms_status_led,
        dcm_status_led, dim_status_led, fsm_status_led, pdm_status_led, clock);

    state_machine = App_SharedStateMachine_Create(world, App_GetDriveState());

    Io_StackWaterMark_Init(can_tx);
    Io_SoftwareWatchdog_Init(can_tx);

    struct CanMsgs_dim_startup_t payload = { .dummy = 0 };
    App_CanTx_SendNonPeriodicMsg_DIM_STARTUP(can_tx, &payload);
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
        Task100Hz, RunTask100Hz, osPriorityBelowNormal, 0, TASK100HZ_STACK_SIZE, Task100HzTaskBuffer,
        &Task100HzTaskControlBlock);
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
    hadc2.Init.ScanConvMode          = ADC_SCAN_DISABLE;
    hadc2.Init.ContinuousConvMode    = DISABLE;
    hadc2.Init.DiscontinuousConvMode = DISABLE;
    hadc2.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc2.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T2_TRGO;
    hadc2.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc2.Init.NbrOfConversion       = 1;
    hadc2.Init.DMAContinuousRequests = ENABLE;
    hadc2.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    hadc2.Init.LowPowerAutoWait      = DISABLE;
    hadc2.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
    if (HAL_ADC_Init(&hadc2) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure Regular Channel
     */
    sConfig.Channel      = ADC_CHANNEL_12;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SingleDiff   = ADC_SINGLE_ENDED;
    sConfig.SamplingTime = ADC_SAMPLETIME_61CYCLES_5;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset       = 0;
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
    hcan.Init.AutoBusOff           = DISABLE;
    hcan.Init.AutoWakeUp           = DISABLE;
    hcan.Init.AutoRetransmission   = DISABLE;
    hcan.Init.ReceiveFifoLocked    = DISABLE;
    hcan.Init.TransmitFifoPriority = DISABLE;
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
    hspi2.Init.CLKPolarity       = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase          = SPI_PHASE_1EDGE;
    hspi2.Init.NSS               = SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
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

    /* USER CODE BEGIN TIM2_Init 1 */

    /* USER CODE END TIM2_Init 1 */
    htim2.Instance               = TIM2;
    htim2.Init.Prescaler         = TIM2_PRESCALER - 1;
    htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim2.Init.Period            = (TIMx_FREQUENCY / TIM2_PRESCALER) / ADC_FREQUENCY - 1;
    htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM2_Init 2 */

    /* USER CODE END TIM2_Init 2 */
}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* DMA interrupt init */
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
    HAL_GPIO_WritePin(GPIOC, BSPD_LED_Pin | PDM_GREEN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(DIM_RED_GPIO_Port, DIM_RED_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(
        GPIOA,
        DCM_BLUE_Pin | PDM_RED_Pin | DCM_GREEN_Pin | PDM_BLUE_Pin | FSM_GREEN_Pin | BMS_BLUE_Pin |
            SEVENSEG_DIMMING_3V3_Pin,
        GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, DIM_GREEN_Pin | DIM_BLUE_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(
        GPIOB,
        DCM_RED_Pin | BMS_GREEN_Pin | FSM_RED_Pin | FSM_BLUE_Pin | BMS_RED_Pin | SEVENSEG_RCK_3V3_Pin | IMD_LED_Pin,
        GPIO_PIN_RESET);

    /*Configure GPIO pins : BSPD_LED_Pin DIM_RED_Pin PDM_GREEN_Pin */
    GPIO_InitStruct.Pin   = BSPD_LED_Pin | DIM_RED_Pin | PDM_GREEN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : DCM_BLUE_Pin DIM_GREEN_Pin DIM_BLUE_Pin PDM_RED_Pin
                             DCM_GREEN_Pin PDM_BLUE_Pin FSM_GREEN_Pin BMS_BLUE_Pin
                             SEVENSEG_DIMMING_3V3_Pin */
    GPIO_InitStruct.Pin = DCM_BLUE_Pin | DIM_GREEN_Pin | DIM_BLUE_Pin | PDM_RED_Pin | DCM_GREEN_Pin | PDM_BLUE_Pin |
                          FSM_GREEN_Pin | BMS_BLUE_Pin | SEVENSEG_DIMMING_3V3_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : DCM_RED_Pin BMS_GREEN_Pin FSM_RED_Pin FSM_BLUE_Pin
                             BMS_RED_Pin SEVENSEG_RCK_3V3_Pin IMD_LED_Pin */
    GPIO_InitStruct.Pin =
        DCM_RED_Pin | BMS_GREEN_Pin | FSM_RED_Pin | FSM_BLUE_Pin | BMS_RED_Pin | SEVENSEG_RCK_3V3_Pin | IMD_LED_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : DRIVE_MODE5_Pin DRIVE_MODE1_Pin TRAC_CTRL_Pin TORQ_VECT_Pin
                             IGNTN_Pin */
    GPIO_InitStruct.Pin  = DRIVE_MODE5_Pin | DRIVE_MODE1_Pin | TRAC_CTRL_Pin | TORQ_VECT_Pin | IGNTN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : DRIVE_MODE4_Pin DRIVE_MODE3_Pin DRIVE_MODE2_Pin */
    GPIO_InitStruct.Pin  = DRIVE_MODE4_Pin | DRIVE_MODE3_Pin | DRIVE_MODE2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
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
        Io_CanRx_UpdateRxTableWithMessage(App_DimWorld_GetCanRx(world), &message);
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
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, "TASK_1KHZ", period_ms);

    /* Infinite loop */
    for (;;)
    {
        // Check in for timeouts for all RTOS tasks
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
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, "TASK_1HZ", period_ms);

    /* Infinite loop */
    for (;;)
    {
        Io_StackWaterMark_Check();
        App_SharedStateMachine_Tick1Hz(state_machine);

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
