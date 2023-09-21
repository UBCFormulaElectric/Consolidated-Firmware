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

#include "App_SharedMacros.h"
#include "App_SharedStateMachine.h"
#include "states/app_driveState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "app_globals.h"
#include "app_sevenSegDisplays.h"
#include "app_avgPower.h"

#include "Io_CanTx.h"
#include "Io_CanRx.h"
#include "Io_SharedCan.h"
#include "Io_SharedErrorHandlerOverride.h"
#include "Io_SharedHardFaultHandler.h"
#include "Io_SharedHeartbeatMonitor.h"

#include "io_time.h"
#include "io_led.h"
#include "io_switch.h"
#include "io_rgbLed.h"
#include "io_watchdogConfig.h"
#include "io_stackWaterMark.h"
#include "io_sevenSegDisplays.h"

#include "hw_gpio.h"
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
DMA_HandleTypeDef hdma_adc1;

CAN_HandleTypeDef hcan1;

IWDG_HandleTypeDef hiwdg;

osThreadId          Task100HzHandle;
uint32_t            Task100HzBuffer[512];
osStaticThreadDef_t Task100HzControlBlock;
osThreadId          TaskCanRxHandle;
uint32_t            TaskCanRxBuffer[512];
osStaticThreadDef_t TaskCanRxControlBlock;
osThreadId          TaskCanTxHandle;
uint32_t            TaskCanTxBuffer[512];
osStaticThreadDef_t TaskCanTxControlBlock;
osThreadId          Task1kHzHandle;
uint32_t            Task1kHzBuffer[512];
osStaticThreadDef_t Task1kHzControlBlock;
osThreadId          Task1HzHandle;
uint32_t            Task1HzBuffer[512];
osStaticThreadDef_t Task1HzControlBlock;
/* USER CODE BEGIN PV */
struct StateMachine *    state_machine;
struct HeartbeatMonitor *heartbeat_monitor;

static const BinaryLed imd_led   = { .gpio = {
                                       .port = IMD_LED_GPIO_Port,
                                       .pin  = IMD_LED_Pin,
                                   } };
static const BinaryLed bspd_led  = { .gpio = {
                                        .port = BSPD_LED_GPIO_Port,
                                        .pin  = BSPD_LED_Pin,
                                    } };
static const BinaryLed shdn_led  = { .gpio = {
                                        .port = SHDN_LED_GPIO_Port,
                                        .pin  = SHDN_LED_Pin,
                                    } };
static const BinaryLed drive_led = { .gpio = {
                                         .port = IGNTN_LED_GPIO_Port,
                                         .pin  = IGNTN_LED_Pin,
                                     } };

static const Switch start_switch = {
    .gpio = {
        .port = IGNTN_IN_GPIO_Port,
        .pin = IGNTN_IN_Pin,
    },
    .closed_state = true,
};
static const Switch aux_switch = {
    .gpio = {
        .port = AUX_IN_GPIO_Port,
        .pin = AUX_IN_Pin,
    },
    .closed_state = true,
};

static const RgbLed bms_status_led = {
    .red_gpio = {
        .port = BMS_RED_GPIO_Port,
        .pin = BMS_RED_Pin,
    },
    .green_gpio = {
        .port = BMS_GREEN_GPIO_Port,
        .pin = BMS_GREEN_Pin,
    },
    .blue_gpio = {
        .port = BMS_BLUE_GPIO_Port,
        .pin = BMS_BLUE_Pin,
    },
};
static const RgbLed dcm_status_led = {
    .red_gpio = {
        .port = DCM_RED_GPIO_Port,
        .pin = DCM_RED_Pin,
    },
    .green_gpio = {
        .port = DCM_GREEN_GPIO_Port,
        .pin = DCM_GREEN_Pin,
    },
    .blue_gpio = {
        .port = DCM_BLUE_GPIO_Port,
        .pin = DCM_BLUE_Pin,
    },
};
static const RgbLed fsm_status_led = {
    .red_gpio = {
        .port = FSM_RED_GPIO_Port,
        .pin = FSM_RED_Pin,
    },
    .green_gpio = {
        .port = FSM_GREEN_GPIO_Port,
        .pin = FSM_GREEN_Pin,
    },
    .blue_gpio = {
        .port = FSM_BLUE_GPIO_Port,
        .pin = FSM_BLUE_Pin,
    },
};
static const RgbLed pdm_status_led = {
    .red_gpio = {
        .port = PDM_RED_GPIO_Port,
        .pin = PDM_RED_Pin,
    },
    .green_gpio = {
        .port = PDM_GREEN_GPIO_Port,
        .pin = PDM_GREEN_Pin,
    },
    .blue_gpio = {
        .port = PDM_BLUE_GPIO_Port,
        .pin = PDM_BLUE_Pin,
    },
};
static const RgbLed dim_status_led = {
    .red_gpio = {
        .port = DIM_RED_GPIO_Port,
        .pin = DIM_RED_Pin,
    },
    .green_gpio = {
        .port = DIM_GREEN_GPIO_Port,
        .pin = DIM_GREEN_Pin,
    },
    .blue_gpio = {
        .port = DIM_BLUE_GPIO_Port,
        .pin = DIM_BLUE_Pin,
    },
};

static const SevenSegsConfig seven_segs_config = {
    .srck_gpio = {
        .port = SEVENSEGS_SRCK_GPIO_Port,
        .pin = SEVENSEGS_SRCK_Pin,
    },
    .rck_gpio = {
        .port = SEVENSEGS_RCK_GPIO_Port,
        .pin = SEVENSEGS_RCK_Pin,
    },
    .ser_out_gpio = {
        .port = SEVENSEGS_SEROUT_GPIO_Port,
        .pin = SEVENSEGS_SEROUT_Pin,
    },
    .dimming_gpio = {
        .port = SEVENSEGS_DIMMING_GPIO_Port,
        .pin = SEVENSEGS_DIMMING_Pin,
    },
};

static const GlobalsConfig globals_config = {
    .imd_led        = &imd_led,
    .bspd_led       = &bspd_led,
    .shdn_led       = &shdn_led,
    .drive_led      = &drive_led,
    .start_switch   = &start_switch,
    .aux_switch     = &aux_switch,
    .bms_status_led = &bms_status_led,
    .dcm_status_led = &dcm_status_led,
    .fsm_status_led = &fsm_status_led,
    .pdm_status_led = &pdm_status_led,
    .dim_status_led = &dim_status_led,
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void        SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_CAN1_Init(void);
static void MX_IWDG_Init(void);
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
    App_CanTx_DIM_AlertsContext_RxOverflowCount_Set(overflow_count);
    App_CanAlerts_SetWarning(DIM_WARNING_RX_OVERFLOW, true);
}

static void CanTxQueueOverflowCallBack(size_t overflow_count)
{
    App_CanTx_DIM_AlertsContext_TxOverflowCount_Set(overflow_count);
    App_CanAlerts_SetWarning(DIM_WARNING_TX_OVERFLOW, true);
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
    MX_ADC1_Init();
    MX_CAN1_Init();
    MX_IWDG_Init();
    /* USER CODE BEGIN 2 */
    __HAL_DBGMCU_FREEZE_IWDG();

    SEGGER_SYSVIEW_Conf();

    Io_SharedHardFaultHandler_Init();
    Io_SharedSoftwareWatchdog_Init(io_watchdogConfig_refresh, io_watchdogConfig_timeoutCallback);
    Io_SharedCan_Init(&hcan1, CanTxQueueOverflowCallBack, CanRxQueueOverflowCallBack);
    Io_CanTx_EnableMode(CAN_MODE_DEFAULT, true);

    io_sevenSegDisplays_init(&seven_segs_config);

    App_CanTx_Init();
    App_CanRx_Init();

    heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
        io_time_getCurrentMs, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);

    state_machine = App_SharedStateMachine_Create(NULL, app_driveState_get());

    app_sevenSegDisplays_init();
    app_avgPower_init();
    app_globals_init(&globals_config);
    globals->heartbeat_monitor = heartbeat_monitor;
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
    osThreadStaticDef(Task100Hz, RunTask100Hz, osPriorityBelowNormal, 0, 512, Task100HzBuffer, &Task100HzControlBlock);
    Task100HzHandle = osThreadCreate(osThread(Task100Hz), NULL);

    /* definition and creation of TaskCanRx */
    osThreadStaticDef(TaskCanRx, RunTaskCanRx, osPriorityIdle, 0, 512, TaskCanRxBuffer, &TaskCanRxControlBlock);
    TaskCanRxHandle = osThreadCreate(osThread(TaskCanRx), NULL);

    /* definition and creation of TaskCanTx */
    osThreadStaticDef(TaskCanTx, RunTaskCanTx, osPriorityIdle, 0, 512, TaskCanTxBuffer, &TaskCanTxControlBlock);
    TaskCanTxHandle = osThreadCreate(osThread(TaskCanTx), NULL);

    /* definition and creation of Task1kHz */
    osThreadStaticDef(Task1kHz, RunTask1kHz, osPriorityNormal, 0, 512, Task1kHzBuffer, &Task1kHzControlBlock);
    Task1kHzHandle = osThreadCreate(osThread(Task1kHz), NULL);

    /* definition and creation of Task1Hz */
    osThreadStaticDef(Task1Hz, RunTask1Hz, osPriorityLow, 0, 512, Task1HzBuffer, &Task1HzControlBlock);
    Task1HzHandle = osThreadCreate(osThread(Task1Hz), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
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
    hadc1.Init.ScanConvMode          = DISABLE;
    hadc1.Init.ContinuousConvMode    = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion       = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel      = ADC_CHANNEL_5;
    sConfig.Rank         = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
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
    HAL_GPIO_WritePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(
        GPIOC,
        BMS_BLUE_Pin | BMS_GREEN_Pin | BMS_RED_Pin | DCM_BLUE_Pin | DCM_GREEN_Pin | DCM_RED_Pin | DIM_BLUE_Pin |
            DIM_GREEN_Pin | DIM_RED_Pin | PDM_BLUE_Pin | PDM_GREEN_Pin | PDM_RED_Pin,
        GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(
        GPIOA,
        SEVENSEGS_SEROUT_Pin | SEVENSEGS_SRCK_Pin | SEVENSEGS_RCK_Pin | SEVENSEGS_DIMMING_Pin | LED_Pin | FSM_BLUE_Pin |
            FSM_GREEN_Pin | FSM_RED_Pin,
        GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, BSPD_LED_Pin | IMD_LED_Pin | SHDN_LED_Pin | AUX_LED_Pin | IGNTN_LED_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : TEST_PIN_Pin BMS_BLUE_Pin BMS_GREEN_Pin BMS_RED_Pin
                             DCM_BLUE_Pin DCM_GREEN_Pin DCM_RED_Pin DIM_BLUE_Pin
                             DIM_GREEN_Pin DIM_RED_Pin PDM_BLUE_Pin PDM_GREEN_Pin
                             PDM_RED_Pin */
    GPIO_InitStruct.Pin = TEST_PIN_Pin | BMS_BLUE_Pin | BMS_GREEN_Pin | BMS_RED_Pin | DCM_BLUE_Pin | DCM_GREEN_Pin |
                          DCM_RED_Pin | DIM_BLUE_Pin | DIM_GREEN_Pin | DIM_RED_Pin | PDM_BLUE_Pin | PDM_GREEN_Pin |
                          PDM_RED_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : SEVENSEGS_SEROUT_Pin SEVENSEGS_SRCK_Pin SEVENSEGS_RCK_Pin */
    GPIO_InitStruct.Pin   = SEVENSEGS_SEROUT_Pin | SEVENSEGS_SRCK_Pin | SEVENSEGS_RCK_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : SEVENSEGS_DIMMING_Pin LED_Pin FSM_BLUE_Pin FSM_GREEN_Pin
                             FSM_RED_Pin */
    GPIO_InitStruct.Pin   = SEVENSEGS_DIMMING_Pin | LED_Pin | FSM_BLUE_Pin | FSM_GREEN_Pin | FSM_RED_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : REGEN_Pin */
    GPIO_InitStruct.Pin  = REGEN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(REGEN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : BSPD_LED_Pin IMD_LED_Pin SHDN_LED_Pin AUX_LED_Pin
                             IGNTN_LED_Pin */
    GPIO_InitStruct.Pin   = BSPD_LED_Pin | IMD_LED_Pin | SHDN_LED_Pin | AUX_LED_Pin | IGNTN_LED_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : AUX_IN_Pin IGNTN_IN_Pin */
    GPIO_InitStruct.Pin  = AUX_IN_Pin | IGNTN_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : DRIVE_MODE_0_Pin DRIVE_MODE_1_Pin DRIVE_MODE_2_Pin DRIVE_MODE_3_Pin */
    GPIO_InitStruct.Pin  = DRIVE_MODE_0_Pin | DRIVE_MODE_1_Pin | DRIVE_MODE_2_Pin | DRIVE_MODE_3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
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
        // Check in for timeouts for all RTOS tasks
        Io_SharedSoftwareWatchdog_CheckForTimeouts();
        const uint32_t task_start_ms = TICK_TO_MS(osKernelSysTick());

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
        io_stackWaterMark_check();
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
