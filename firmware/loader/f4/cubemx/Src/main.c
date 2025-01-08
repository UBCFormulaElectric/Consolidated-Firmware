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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_gcc.h"
#include "hw_crc.h"
#include "hw_hal.h"
#include "hw_hardFaultHandler.h"
#include "hw_utils.h"
#include "hw_can.h"

#include "io_can.h"

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
extern uint32_t __app_metadata_start__;
extern uint32_t __app_metadata_size__;

extern uint32_t __app_code_start__;
extern uint32_t __app_code_size__;

extern uint32_t __boot_code_start__;
extern uint32_t __boot_code_size__;

typedef enum
{
    LOADER_STATUS_APP_VALID,
    LOADER_STATUS_APP_INVALID,
    LOADER_STATUS_NO_APP
} LoaderStatus;

typedef struct
{
    uint32_t checksum;
    uint32_t size_bytes;
    uint32_t bootloader_status;
} Metadata;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

CRC_HandleTypeDef hcrc;

/* Definitions for defaultTask */
osThreadId_t         defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name       = "defaultTask",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};
/* Definitions for canTxTask */
osThreadId_t         canTxTaskHandle;
uint32_t             canTxTaskBuffer[512];
osStaticThreadDef_t  canTxTaskControlBlock;
const osThreadAttr_t canTxTask_attributes = {
    .name       = "canTxTask",
    .cb_mem     = &canTxTaskControlBlock,
    .cb_size    = sizeof(canTxTaskControlBlock),
    .stack_mem  = &canTxTaskBuffer[0],
    .stack_size = sizeof(canTxTaskBuffer),
    .priority   = (osPriority_t)osPriorityBelowNormal,
};
/* USER CODE BEGIN PV */
static void canRxOverflow(uint32_t unused)
{
    UNUSED(unused);
    // BREAK_IF_DEBUGGER_CONNECTED();
}

static void canTxOverflow(uint32_t unused)
{
    UNUSED(unused);
    // BREAK_IF_DEBUGGER_CONNECTED();
}

static const CanConfig can_config = {
    .rx_msg_filter        = NULL,
    .rx_overflow_callback = canRxOverflow,
    .tx_overflow_callback = canTxOverflow,
};

CanHandle can = {
    .can                       = &hcan1,
    .can_msg_received_callback = io_can_pushRxMsgToQueue,
};

static LoaderStatus verifyAppCodeChecksum(void)
{
    if (*&__app_code_start__ == 0xFFFFFFFF)
    {
        // If app initial stack pointer is all 0xFF, assume app is not present.
        return LOADER_STATUS_NO_APP;
    }

    Metadata *metadata = (Metadata *)&__app_metadata_start__;
    if (metadata->size_bytes > (uint32_t)&__app_code_size__)
    {
        // App binary size field is invalid.
        return LOADER_STATUS_APP_INVALID;
    }

    uint32_t *buffer              = &__app_code_start__;
    uint32_t  calculated_checksum = hw_crc_calculate(&__app_code_start__, metadata->size_bytes / 4);
    return calculated_checksum == metadata->checksum ? LOADER_STATUS_APP_VALID : LOADER_STATUS_APP_INVALID;
}

_Noreturn static void modifyStackPointerAndJump(const uint32_t *address)
{
    // Disable interrupts before jumping.
    __disable_irq();

    // Disable system tick to stop FreeRTOS timebase.
    SysTick->CTRL = ~SysTick_CTRL_ENABLE_Msk;

    // Clear all pending interrupts by setting all ICPRs (Interrupt Clear Pending Register)
    // to 0xFFFFFFFF. This is done so no interrupts are queued up when we jump to the app.
    // (There are 8 registers on the Cortex-M4)
    for (uint32_t i = 0; i < 8; i++)
    {
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    // Update the vector table offset register. When an interrupt is fired,
    // the microcontroller looks for the corresponding interrupt service handler
    // at the memory address in the VTOR. We need to update it so the app ISRs
    // are used.
    SCB->VTOR = (uint32_t)address;

    // Flush processor pipeline.
    __ISB();

    // Tell MCU to use the main stack pointer rather than process stack pointer (PSP is used with RTOS)
    __set_CONTROL(__get_CONTROL() & ~CONTROL_SPSEL_Msk);

    // Modify stack pointer and jump to code.
    // In a binary built with our linker settings, the interrupt vector table is the first thing
    // placed in flash. The first word of the vector table contains the initial stack pointer
    // and the second word containers the address of the reset handler. Update stack pointer and
    // program counter accordingly.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
    uint32_t newStackPointer = address[0];
    uint32_t resetHandler    = address[1];
#pragma GCC diagnostic pop
    __set_MSP(newStackPointer);
    void (*app_reset_handler)(void) = (void (*)(void))resetHandler;
    app_reset_handler(); // Call app's Reset_Handler, starting the app.

    // Should never get here!
    BREAK_IF_DEBUGGER_CONNECTED();
    for (;;)
    {
    }
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void        SystemClock_Config(void);
static void MX_GPIO_Init(void);
// static void MX_CAN1_Init(void);
static void MX_CRC_Init(void);
void        StartDefaultTask(void *argument);
void        runCanTxTask(void *argument);

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

    /* USER CODE BEGIN SysInit */
    __HAL_RCC_CAN1_CLK_ENABLE();
    __HAL_RCC_CRC_CLK_ENABLE();
    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    // MX_CAN1_Init();
    MX_CRC_Init();
    /* USER CODE BEGIN 2 */
    hw_hardFaultHandler_init();
    hw_crc_init(&hcrc);
    // hw_can_init(&can);
    // io_can_init(&can_config);

    LoaderStatus status = verifyAppCodeChecksum();
    HAL_CRC_DeInit(&hcrc);
    if (status == LOADER_STATUS_APP_VALID)
    {
        modifyStackPointerAndJump(&__app_code_start__);
    }
    else
    {
        modifyStackPointerAndJump(&__boot_code_start__);
    }
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
    /* creation of defaultTask */
    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

    /* creation of canTxTask */
    canTxTaskHandle = osThreadNew(runCanTxTask, NULL, &canTxTask_attributes);

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
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
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
 * @brief CAN1 Initialization Function
 * @param None
 * @retval None
 */
// static void MX_CAN1_Init(void)
// {
//     /* USER CODE BEGIN CAN1_Init 0 */
//     /* USER CODE END CAN1_Init 0 */

//     /* USER CODE BEGIN CAN1_Init 1 */

//     /* USER CODE END CAN1_Init 1 */
//     hcan1.Instance                  = CAN1;
//     hcan1.Init.Prescaler            = 6;
//     hcan1.Init.Mode                 = CAN_MODE_NORMAL;
//     hcan1.Init.SyncJumpWidth        = CAN_SJW_4TQ;
//     hcan1.Init.TimeSeg1             = CAN_BS1_13TQ;
//     hcan1.Init.TimeSeg2             = CAN_BS2_2TQ;
//     hcan1.Init.TimeTriggeredMode    = DISABLE;
//     hcan1.Init.AutoBusOff           = ENABLE;
//     hcan1.Init.AutoWakeUp           = DISABLE;
//     hcan1.Init.AutoRetransmission   = ENABLE;
//     hcan1.Init.ReceiveFifoLocked    = ENABLE;
//     hcan1.Init.TransmitFifoPriority = ENABLE;
//     if (HAL_CAN_Init(&hcan1) != HAL_OK)
//     {
//         Error_Handler();
//     }
//     /* USER CODE BEGIN CAN1_Init 2 */

//     /* USER CODE END CAN1_Init 2 */
// }

/**
 * @brief CRC Initialization Function
 * @param None
 * @retval None
 */
static void MX_CRC_Init(void)
{
    /* USER CODE BEGIN CRC_Init 0 */

    /* USER CODE END CRC_Init 0 */

    /* USER CODE BEGIN CRC_Init 1 */

    /* USER CODE END CRC_Init 1 */
    hcrc.Instance = CRC;
    if (HAL_CRC_Init(&hcrc) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN CRC_Init 2 */

    /* USER CODE END CRC_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
    /* USER CODE BEGIN MX_GPIO_Init_1 */
    /* USER CODE END MX_GPIO_Init_1 */

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* USER CODE BEGIN MX_GPIO_Init_2 */
    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
    /* USER CODE BEGIN 5 */
    /* Infinite loop */
    for (;;)
    {
        osDelay(1);
    }
    /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_runCanTxTask */
/**
 * @brief Function implementing the canTxTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_runCanTxTask */
void runCanTxTask(void *argument)
{
    /* USER CODE BEGIN runCanTxTask */
    /* Infinite loop */
    for (;;)
    {
        osDelay(1);
    }
    /* USER CODE END runCanTxTask */
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
