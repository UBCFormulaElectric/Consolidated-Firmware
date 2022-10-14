/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "App_SharedMacros.h"
#include "Io_SharedCan.h"
//#include "Io_SharedSoftwareWatchdog.h"
#include "Io_CanTx.h"
#include "Io_CanRx.h"
#include "App_JsoncantestWorld.h"
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

SPI_HandleTypeDef hspi2;

osThreadId          Task100HzHandle;
uint32_t            Task100HzBuffer[128];
osStaticThreadDef_t Task100HzControlBlock;
osThreadId          Task1HzHandle;
uint32_t            Task1HzBuffer[128];
osStaticThreadDef_t Task1HzControlBlock;
osThreadId          Task1kHzHandle;
uint32_t            Task1kHzBuffer[128];
osStaticThreadDef_t Task1kHzControlBlock;
osThreadId          TaskCanRxHandle;
uint32_t            TaskCanRxBuffer[128];
osStaticThreadDef_t TaskCanRxControlBlock;
osThreadId          TaskCanTxHandle;
uint32_t            TaskCanTxBuffer[128];
osStaticThreadDef_t TaskCanTxControlBlock;
/* USER CODE BEGIN PV */
// struct JsoncantestCanTxInterface *can_tx;
// struct JsoncantestCanRxInterface *can_rx;
struct Clock *clock;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void        SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI2_Init(void);
void        RunTask100Hz(void const *argument);
void        RunTask1Hz(void const *argument);
void        RunTask1kHz(void const *argument);
void        RunTaskCanRx(void const *argument);
void        RunTaskCanTx(void const *argument);

/* USER CODE BEGIN PFP */

static void CanRxQueueOverflowCallBack(size_t overflow_count);
static void CanTxQueueOverflowCallBack(size_t overflow_count);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void CanRxQueueOverflowCallBack(size_t overflow_count)
{
    // Not implemented
}

static void CanTxQueueOverflowCallBack(size_t overflow_count)
{
    // Not implemented
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
    MX_ADC1_Init();
    MX_SPI2_Init();
    /* USER CODE BEGIN 2 */

    App_CanTx_Init();
    App_CanRx_Init();

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
    osThreadStaticDef(Task100Hz, RunTask100Hz, osPriorityBelowNormal, 0, 128, Task100HzBuffer, &Task100HzControlBlock);
    Task100HzHandle = osThreadCreate(osThread(Task100Hz), NULL);

    /* definition and creation of Task1Hz */
    osThreadStaticDef(Task1Hz, RunTask1Hz, osPriorityLow, 0, 128, Task1HzBuffer, &Task1HzControlBlock);
    Task1HzHandle = osThreadCreate(osThread(Task1Hz), NULL);

    /* definition and creation of Task1kHz */
    osThreadStaticDef(Task1kHz, RunTask1kHz, osPriorityAboveNormal, 0, 128, Task1kHzBuffer, &Task1kHzControlBlock);
    Task1kHzHandle = osThreadCreate(osThread(Task1kHz), NULL);

    /* definition and creation of TaskCanRx */
    osThreadStaticDef(TaskCanRx, RunTaskCanRx, osPriorityIdle, 0, 128, TaskCanRxBuffer, &TaskCanRxControlBlock);
    TaskCanRxHandle = osThreadCreate(osThread(TaskCanRx), NULL);

    /* definition and creation of TaskCanTx */
    osThreadStaticDef(TaskCanTx, RunTaskCanTx, osPriorityIdle, 0, 128, TaskCanTxBuffer, &TaskCanTxControlBlock);
    TaskCanTxHandle = osThreadCreate(osThread(TaskCanTx), NULL);

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
    RCC_OscInitTypeDef       RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef       RCC_ClkInitStruct = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInit     = { 0 };

    /** Initializes the CPU, AHB and APB busses clocks
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
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
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC1;
    PeriphClkInit.Adc1ClockSelection   = RCC_ADC1PLLCLK_DIV1;

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

    ADC_ChannelConfTypeDef sConfig = { 0 };

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
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
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
    uint32_t                PreviousWakeTime = osKernelSysTick();
    static const TickType_t period_ms        = 10;
    //    SoftwareWatchdogHandle_t watchdog         = Io_SharedSoftwareWatchdog_AllocateWatchdog();
    //    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, "TASK_100HZ", period_ms);

    /* Infinite loop */
    for (;;)
    {
        //        App_SharedStateMachine_Tick100Hz(state_machine);

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        //        Io_SharedSoftwareWatchdog_CheckInWatchdog(watchdog);
        osDelayUntil(&PreviousWakeTime, period_ms);
    }
    /* USER CODE END 5 */
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
    uint32_t                PreviousWakeTime = osKernelSysTick();
    static const TickType_t period_ms        = 1000U;
    //    SoftwareWatchdogHandle_t watchdog         = Io_SharedSoftwareWatchdog_AllocateWatchdog();
    //    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, "TASK_1HZ", period_ms);

    for (;;)
    {
        //        Io_StackWaterMark_Check();
        //        App_SharedStateMachine_Tick1Hz(state_machine);

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        //        Io_SharedSoftwareWatchdog_CheckInWatchdog(watchdog);
        osDelayUntil(&PreviousWakeTime, period_ms);
    }
    /* USER CODE END RunTask1Hz */
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
    uint32_t                PreviousWakeTime = osKernelSysTick();
    static const TickType_t period_ms        = 10U;
    //    SoftwareWatchdogHandle_t watchdog         = Io_SharedSoftwareWatchdog_AllocateWatchdog();
    //    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, "TASK_1KHZ", period_ms);

    for (;;)
    {
        //        Io_SharedSoftwareWatchdog_CheckForTimeouts();
        const uint32_t task_start_ms = TICK_TO_MS(osKernelSysTick());
        //        App_SharedClock_SetCurrentTimeInMilliseconds(clock, task_start_ms); //BROKEN!

        App_CanTx_JSONCANTest_VITALS_HEARTBEAT_SetSignal(1);
        App_CanTx_JSONCANTest_STATUS_CONTACTORS_CLOSED_SetSignal(1);
        const float apps_pedal_travel = App_CanRx_FSM_APPS_Sapps_Mapped_Pedal_Percentage_GetValue();
        Io_CanTx_Enqueue1kHzMessages(task_start_ms);

        // Watchdog check-in must be the last function called before putting the
        // task to sleep. Prevent check in if the elapsed period is greater or
        // equal to the period ms
        //        if ((TICK_TO_MS(osKernelSysTick()) - task_start_ms) <= period_ms)
        //        {
        //            Io_SharedSoftwareWatchdog_CheckInWatchdog(watchdog);
        //        }

        osDelayUntil(&PreviousWakeTime, period_ms);
    }

    //    /* Infinite loop */
    //    for (;;)
    //    {
    //        osDelay(1);
    //    }
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
    /* Infinite loop */
    for (;;)
    {
        Io_SharedCan_TransmitEnqueuedCanTxMessagesFromTask();
    }
    /* USER CODE END RunTaskCanTx */
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
    /* User can add his own implementation to report the HAL error return state */

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
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
