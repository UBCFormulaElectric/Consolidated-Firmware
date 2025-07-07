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
#include "hw_usb.h"
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
DMA_HandleTypeDef hdma_adc1;

CAN_HandleTypeDef hcan2;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c3;

IWDG_HandleTypeDef hiwdg;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim13;

/* Definitions for Task1KHz */
osThreadId_t         Task1KHzHandle;
uint32_t             Task1KHzBuffer[512];
osStaticThreadDef_t  Task1KHzControlBlock;
const osThreadAttr_t Task1KHz_attributes = {
    .name       = "Task1KHz",
    .cb_mem     = &Task1KHzControlBlock,
    .cb_size    = sizeof(Task1KHzControlBlock),
    .stack_mem  = &Task1KHzBuffer[0],
    .stack_size = sizeof(Task1KHzBuffer),
    .priority   = (osPriority_t)osPriorityRealtime,
};
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
/* Definitions for TaskCanTx */
osThreadId_t         TaskCanTxHandle;
uint32_t             TaskCanTxBuffer[512];
osStaticThreadDef_t  TaskCanTxControlBlock;
const osThreadAttr_t TaskCanTx_attributes = {
    .name       = "TaskCanTx",
    .cb_mem     = &TaskCanTxControlBlock,
    .cb_size    = sizeof(TaskCanTxControlBlock),
    .stack_mem  = &TaskCanTxBuffer[0],
    .stack_size = sizeof(TaskCanTxBuffer),
    .priority   = (osPriority_t)osPriorityNormal,
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
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void        SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_CAN2_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C3_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM2_Init(void);
static void MX_IWDG_Init(void);
static void MX_TIM13_Init(void);
void        RunTask1KHz(void *argument);
void        RunTask100Hz(void *argument);
void        RunTask1Hz(void *argument);
void        RunTaskCanTx(void *argument);
void        RunTaskCanRx(void *argument);
void        RunTaskChimera(void *argument);

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
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_CAN2_Init();
    MX_I2C1_Init();
    MX_I2C3_Init();
    MX_TIM4_Init();
    MX_TIM2_Init();
    MX_IWDG_Init();
    MX_TIM13_Init();
    /* USER CODE BEGIN 2 */
    tasks_init();
    /* USER CODE END 2 */

    /* Init scheduler */
    osKernelInitialize();

    /* USER CODE BEGIN RTOS_MUTEX */
    ASSERT_EXIT_OK(hw_usb_init());
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
    /* creation of Task1KHz */
    Task1KHzHandle = osThreadNew(RunTask1KHz, NULL, &Task1KHz_attributes);

    /* creation of Task100Hz */
    Task100HzHandle = osThreadNew(RunTask100Hz, NULL, &Task100Hz_attributes);

    /* creation of Task1Hz */
    Task1HzHandle = osThreadNew(RunTask1Hz, NULL, &Task1Hz_attributes);

    /* creation of TaskCanTx */
    TaskCanTxHandle = osThreadNew(RunTaskCanTx, NULL, &TaskCanTx_attributes);

    /* creation of TaskCanRx */
    TaskCanRxHandle = osThreadNew(RunTaskCanRx, NULL, &TaskCanRx_attributes);

    /* creation of TaskChimera */
    TaskChimeraHandle = osThreadNew(RunTaskChimera, NULL, &TaskChimera_attributes);

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

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.LSIState       = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = 4;
    RCC_OscInitStruct.PLL.PLLN       = 96;
    RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ       = 4;
    RCC_OscInitStruct.PLL.PLLR       = 2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
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
    hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc1.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T2_TRGO;
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion       = 4;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel      = ADC_CHANNEL_0;
    sConfig.Rank         = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_5;
    sConfig.Rank    = 2;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_6;
    sConfig.Rank    = 3;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_12;
    sConfig.Rank    = 4;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */
}

/**
 * @brief CAN2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_CAN2_Init(void)
{
    /* USER CODE BEGIN CAN2_Init 0 */

    /* USER CODE END CAN2_Init 0 */

    /* USER CODE BEGIN CAN2_Init 1 */

    /* USER CODE END CAN2_Init 1 */
    hcan2.Instance                  = CAN2;
    hcan2.Init.Prescaler            = 3;
    hcan2.Init.Mode                 = CAN_MODE_NORMAL;
    hcan2.Init.SyncJumpWidth        = CAN_SJW_3TQ;
    hcan2.Init.TimeSeg1             = CAN_BS1_12TQ;
    hcan2.Init.TimeSeg2             = CAN_BS2_3TQ;
    hcan2.Init.TimeTriggeredMode    = DISABLE;
    hcan2.Init.AutoBusOff           = ENABLE;
    hcan2.Init.AutoWakeUp           = DISABLE;
    hcan2.Init.AutoRetransmission   = ENABLE;
    hcan2.Init.ReceiveFifoLocked    = ENABLE;
    hcan2.Init.TransmitFifoPriority = ENABLE;
    if (HAL_CAN_Init(&hcan2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN CAN2_Init 2 */

    /* USER CODE END CAN2_Init 2 */
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
    hi2c3.Instance             = I2C3;
    hi2c3.Init.ClockSpeed      = 100000;
    hi2c3.Init.DutyCycle       = I2C_DUTYCYCLE_2;
    hi2c3.Init.OwnAddress1     = 0;
    hi2c3.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c3.Init.OwnAddress2     = 0;
    hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c3.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c3) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C3_Init 2 */

    /* USER CODE END I2C3_Init 2 */
}

/**
 * @brief IWDG Initialization Function
 * @param None
 * @retval None
 */
static void MX_IWDG_Init(void)
{
    /* USER CODE BEGIN IWDG_Init 0 */
#ifndef WATCHDOG_DISABLED
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
#endif
    /* USER CODE END IWDG_Init 2 */
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
    htim2.Init.Prescaler         = TIM2_PRESCALER - 1;
    htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim2.Init.Period            = TIM2_ARR - 1;
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
    if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode     = TIM_OCMODE_TIMING;
    sConfigOC.Pulse      = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM2_Init 2 */

    /* USER CODE END TIM2_Init 2 */
}

/**
 * @brief TIM4 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM4_Init(void)
{
    /* USER CODE BEGIN TIM4_Init 0 */

    /* USER CODE END TIM4_Init 0 */

    TIM_ClockConfigTypeDef  sClockSourceConfig = { 0 };
    TIM_MasterConfigTypeDef sMasterConfig      = { 0 };
    TIM_IC_InitTypeDef      sConfigIC          = { 0 };

    /* USER CODE BEGIN TIM4_Init 1 */

    /* USER CODE END TIM4_Init 1 */
    htim4.Instance               = TIM4;
    htim4.Init.Prescaler         = TIM4_PRESCALER - 1;
    htim4.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim4.Init.Period            = TIM4_ARR - 1;
    htim4.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_IC_Init(&htim4) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter    = 0;
    if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM4_Init 2 */

    /* USER CODE END TIM4_Init 2 */
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

    /* USER CODE BEGIN TIM13_Init 1 */

    /* USER CODE END TIM13_Init 1 */
    htim13.Instance               = TIM13;
    htim13.Init.Prescaler         = 10 - 1;
    htim13.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim13.Init.Period            = 960 - 1;
    htim13.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim13.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim13) != HAL_OK)
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
    /* USER CODE BEGIN MX_GPIO_Init_1 */
    /* USER CODE END MX_GPIO_Init_1 */

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, BOOT_Pin | LED_Pin | BRAKE_LIGHT_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : RL_INT_3V3_SENS_Pin SUSP_TRAVEL_RL_OCSC_Pin */
    GPIO_InitStruct.Pin  = RL_INT_3V3_SENS_Pin | SUSP_TRAVEL_RL_OCSC_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : SUSP_TRAVEL_RR_OCSC_Pin */
    GPIO_InitStruct.Pin  = SUSP_TRAVEL_RR_OCSC_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(SUSP_TRAVEL_RR_OCSC_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : BOOT_Pin LED_Pin BRAKE_LIGHT_EN_Pin */
    GPIO_InitStruct.Pin   = BOOT_Pin | LED_Pin | BRAKE_LIGHT_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : IMU_INT1_Pin IMU_INT2_Pin */
    GPIO_InitStruct.Pin  = IMU_INT1_Pin | IMU_INT2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    /* USER CODE BEGIN MX_GPIO_Init_2 */
    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_RunTask1KHz */
/**
 * @brief  Function implementing the Task1KHz thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTask1KHz */
void RunTask1KHz(void *argument)
{
    /* init code for USB_DEVICE */
    MX_USB_DEVICE_Init();
    /* USER CODE BEGIN 5 */
    tasks_run1kHz();
    /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_RunTask100Hz */
/**
 * @brief Function implementing the Task100Hz thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTask100Hz */
void RunTask100Hz(void *argument)
{
    /* USER CODE BEGIN RunTask100Hz */
    tasks_run100Hz();
    /* USER CODE END RunTask100Hz */
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

/* USER CODE BEGIN Header_RunTaskCanTx */
/**
 * @brief Function implementing the TaskCanTx thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTaskCanTx */
void RunTaskCanTx(void *argument)
{
    /* USER CODE BEGIN RunTaskCanTx */
    tasks_runCanTx();
    /* USER CODE END RunTaskCanTx */
}

/* USER CODE BEGIN Header_RunTaskCanRx */
/**
 * @brief Function implementing the TaskCanRx thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RunTaskCanRx */
void RunTaskCanRx(void *argument)
{
    /* USER CODE BEGIN RunTaskCanRx */
    tasks_runCanRx();
    /* USER CODE END RunTaskCanRx */
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
    /* Infinite loop */
    tasks_runChimera();
    /* USER CODE END RunTaskChimera */
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
