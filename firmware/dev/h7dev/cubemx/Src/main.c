/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attenti * Copyright (c) 2023 STMicroelectronics.
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
#include <stdio.h>
#include "../../nanopb/pb_encode.h"
#include "../../nanopb/pb_decode.h"
#include "sample.pb.h"
#include "string.h"
#include "hw_hardFaultHandler.h"
<<<<<<< HEAD
#include "hw_bootup.h"
<<<<<<< HEAD
#include "hw_uart.h"
=======
=======
#include "hw_can.h"
>>>>>>> 193708cf (main functin implementation)
#include "hw_sd.h"
#include "hw_bootup.h"
#include "io_can.h"
#include "io_canLogging.h"
#include "io_lfs_config.h"
#include "hw_gpio.h"
#include "io_log.h"
#include "hw_utils.h"
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

<<<<<<< HEAD
UART_HandleTypeDef huart9;
=======
FDCAN_HandleTypeDef hfdcan1;
FDCAN_HandleTypeDef hfdcan2;
>>>>>>> 2a5cb3af (10k with 3% loss)

SD_HandleTypeDef hsd1;

/* Definitions for defaultTask */
osThreadId_t         defaultTaskHandle;
uint32_t             defaultTaskBuffer[512];
osStaticThreadDef_t  defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
    .name       = "defaultTask",
    .cb_mem     = &defaultTaskControlBlock,
    .cb_size    = sizeof(defaultTaskControlBlock),
    .stack_mem  = &defaultTaskBuffer[0],
    .stack_size = sizeof(defaultTaskBuffer),
    .priority   = (osPriority_t)osPriorityNormal,
};
<<<<<<< HEAD
=======
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
/* Definitions for canRxTask */
osThreadId_t         canRxTaskHandle;
uint32_t             canRxTaskBuffer[512];
osStaticThreadDef_t  canRxTaskControlBlock;
const osThreadAttr_t canRxTask_attributes = {
    .name       = "canRxTask",
    .cb_mem     = &canRxTaskControlBlock,
    .cb_size    = sizeof(canRxTaskControlBlock),
    .stack_mem  = &canRxTaskBuffer[0],
    .stack_size = sizeof(canRxTaskBuffer),
    .priority   = (osPriority_t)osPriorityBelowNormal,
};
>>>>>>> 672ea0c3 (rm bootload for now)
/* USER CODE BEGIN PV */

int         write_num    = 0;
int         read_num     = 0;
int         overflow_num = 0;
static void callback(uint32_t i)
{
    overflow_num++;
    // BREAK_IF_DEBUGGER_CONNECTED();
}

>>>>>>> d9a69f1b (write message to sd card)
static CanConfig can_config = {
    .rx_msg_filter        = NULL,
    .tx_overflow_callback = callback,
    .rx_overflow_callback = callback,
};

/* Little fs config*/
struct lfs_config cfg;
>>>>>>> 193708cf (main functin implementation)
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void        SystemClock_Config(void);
static void MX_GPIO_Init(void);
<<<<<<< HEAD
static void MX_UART9_Init(void);
=======
static void MX_FDCAN2_Init(void);
static void MX_SDMMC1_SD_Init(void);
static void MX_FDCAN1_Init(void);
void        runDefaultTask(void *argument);
void        runCanTxTask(void *argument);
void        runCanRxTask(void *argument);
<<<<<<< HEAD
>>>>>>> 6102d31a (add canlogging queue)
void        runDefaultTask(void *argument);
=======
void runDefaultTask(void *argument);
void runCanTxTask(void *argument);
void runCanRxTask(void *argument);
>>>>>>> 672ea0c3 (rm bootload for now)

/* USER CODE BEGIN PFP */
static void canMsgRecievecallback(CanMsg *rx_msg);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
CanHandle can = { .can = &hfdcan2, .canMsgRecievecallback = canMsgRecievecallback };
SdCard    sd;
Gpio      sd_present = {
         .pin  = GPIO_PIN_8,
         .port = GPIOA,
};
bool sd_inited;

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */
    // hw_bootup_enableInterruptsForApp();
    /* USER CODE END 1 */

    /* Enable I-Cache---------------------------------------------------------*/
    SCB_EnableICache();

    /* Enable D-Cache---------------------------------------------------------*/
    SCB_EnableDCache();

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
    MX_UART9_Init();
    /* USER CODE BEGIN 2 */
    // __HAL_DBGMCU_FREEZE_IWDG();

    // hw_hardFaultHandler_init();
    // hw_can_init(&hfdcan2);
    MX_FDCAN2_Init();
    MX_SDMMC1_SD_Init();
    MX_FDCAN1_Init();
    /* USER CODE BEGIN 2 */
    // __HAL_DBGMCU_FREEZE_IWDG();

    io_can_init(&can_config);
    hw_hardFaultHandler_init();
    hw_can_init(&can1_handle);

    // io_can_init(&can_config);

    sd.hsd = &hsd1;
    sd.timeout = osWaitForever;

    // Configure and initialize SEGGER SystemView.
    // static int   e = 0;
    // uint8_t      buff[4096];
    // SdCardStatus d = hw_sd_write_dma(&sd, buff, 0, 1);
    // while (1)
    //     ;
    // d              = hw_sd_write_dma(&sd, buff, 1, 1);
    // (void)d;
    // for (size_t i = 0; i < 1000; i++)
    // {
    //     uint32_t start = i * 4096 / 512;
    //     if (hw_sd_write_dma(&sd, buff, start, 1) != SD_CARD_OK)
    //     {
    //         e++;
    //     }
    // }
    SEGGER_SYSVIEW_Conf();
    // LOG_INFO("h7dev reset!");
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
    defaultTaskHandle = osThreadNew(runDefaultTask, NULL, &defaultTask_attributes);

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
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = 1;
    RCC_OscInitStruct.PLL.PLLN       = 64;
    RCC_OscInitStruct.PLL.PLLP       = 1;
    RCC_OscInitStruct.PLL.PLLQ       = 4;
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
<<<<<<< HEAD
 * @brief UART9 Initialization Function
=======
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
    hfdcan1.Init.FrameFormat          = FDCAN_FRAME_CLASSIC;
    hfdcan1.Init.Mode                 = FDCAN_MODE_NORMAL;
    hfdcan1.Init.AutoRetransmission   = DISABLE;
    hfdcan1.Init.TransmitPause        = DISABLE;
    hfdcan1.Init.ProtocolException    = DISABLE;
    hfdcan1.Init.NominalPrescaler     = 16;
    hfdcan1.Init.NominalSyncJumpWidth = 1;
    hfdcan1.Init.NominalTimeSeg1      = 2;
    hfdcan1.Init.NominalTimeSeg2      = 2;
    hfdcan1.Init.DataPrescaler        = 1;
    hfdcan1.Init.DataSyncJumpWidth    = 1;
    hfdcan1.Init.DataTimeSeg1         = 1;
    hfdcan1.Init.DataTimeSeg2         = 1;
    hfdcan1.Init.MessageRAMOffset     = 0;
    hfdcan1.Init.StdFiltersNbr        = 0;
    hfdcan1.Init.ExtFiltersNbr        = 0;
    hfdcan1.Init.RxFifo0ElmtsNbr      = 0;
    hfdcan1.Init.RxFifo0ElmtSize      = FDCAN_DATA_BYTES_8;
    hfdcan1.Init.RxFifo1ElmtsNbr      = 0;
    hfdcan1.Init.RxFifo1ElmtSize      = FDCAN_DATA_BYTES_8;
    hfdcan1.Init.RxBuffersNbr         = 0;
    hfdcan1.Init.RxBufferSize         = FDCAN_DATA_BYTES_8;
    hfdcan1.Init.TxEventsNbr          = 0;
    hfdcan1.Init.TxBuffersNbr         = 0;
    hfdcan1.Init.TxFifoQueueElmtsNbr  = 0;
    hfdcan1.Init.TxFifoQueueMode      = FDCAN_TX_FIFO_OPERATION;
    hfdcan1.Init.TxElmtSize           = FDCAN_DATA_BYTES_8;
    if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN FDCAN1_Init 2 */

    /* USER CODE END FDCAN1_Init 2 */
}

/**
 * @brief FDCAN2 Initialization Function
>>>>>>> 2a5cb3af (10k with 3% loss)
 * @param None
 * @retval None
 */
static void MX_UART9_Init(void)
{
    /* USER CODE BEGIN UART9_Init 0 */

    /* USER CODE END UART9_Init 0 */

    /* USER CODE BEGIN UART9_Init 1 */

<<<<<<< HEAD
    /* USER CODE END UART9_Init 1 */
    huart9.Instance                    = UART9;
    huart9.Init.BaudRate               = 57600;
    huart9.Init.WordLength             = UART_WORDLENGTH_8B;
    huart9.Init.StopBits               = UART_STOPBITS_1;
    huart9.Init.Parity                 = UART_PARITY_NONE;
    huart9.Init.Mode                   = UART_MODE_TX_RX;
    huart9.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart9.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart9.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart9.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    huart9.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart9) != HAL_OK)
=======
    /* USER CODE END FDCAN2_Init 1 */
    hfdcan2.Instance                  = FDCAN2;
    hfdcan2.Init.FrameFormat          = FDCAN_FRAME_CLASSIC;
    hfdcan2.Init.Mode                 = FDCAN_MODE_NORMAL;
    hfdcan2.Init.AutoRetransmission   = ENABLE;
    hfdcan2.Init.TransmitPause        = DISABLE;
    hfdcan2.Init.ProtocolException    = DISABLE;
    hfdcan2.Init.NominalPrescaler     = 16;
    hfdcan2.Init.NominalSyncJumpWidth = 4;
    hfdcan2.Init.NominalTimeSeg1      = 13;
    hfdcan2.Init.NominalTimeSeg2      = 2;
    hfdcan2.Init.DataPrescaler        = 1;
    hfdcan2.Init.DataSyncJumpWidth    = 1;
    hfdcan2.Init.DataTimeSeg1         = 1;
    hfdcan2.Init.DataTimeSeg2         = 1;
    hfdcan2.Init.MessageRAMOffset     = 0;
    hfdcan2.Init.StdFiltersNbr        = 1;
    hfdcan2.Init.ExtFiltersNbr        = 0;
    hfdcan2.Init.RxFifo0ElmtsNbr      = 1;
    hfdcan2.Init.RxFifo0ElmtSize      = FDCAN_DATA_BYTES_8;
    hfdcan2.Init.RxFifo1ElmtsNbr      = 0;
    hfdcan2.Init.RxFifo1ElmtSize      = FDCAN_DATA_BYTES_8;
    hfdcan2.Init.RxBuffersNbr         = 0;
    hfdcan2.Init.RxBufferSize         = FDCAN_DATA_BYTES_8;
    hfdcan2.Init.TxEventsNbr          = 0;
    hfdcan2.Init.TxBuffersNbr         = 0;
    hfdcan2.Init.TxFifoQueueElmtsNbr  = 1;
    hfdcan2.Init.TxFifoQueueMode      = FDCAN_TX_FIFO_OPERATION;
    hfdcan2.Init.TxElmtSize           = FDCAN_DATA_BYTES_8;
    if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK)
>>>>>>> 672ea0c3 (rm bootload for now)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart9, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart9, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&huart9) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN UART9_Init 2 */

    /* USER CODE END UART9_Init 2 */
}

/**
 * @brief SDMMC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SDMMC1_SD_Init(void)
{
    /* USER CODE BEGIN SDMMC1_Init 0 */
    if (hw_gpio_readPin(&sd_present))
    {
        sd_inited = false;
        return;
    }

    /* USER CODE END SDMMC1_Init 0 */

    /* USER CODE BEGIN SDMMC1_Init 1 */

    /* USER CODE END SDMMC1_Init 1 */
    hsd1.Instance                 = SDMMC1;
    hsd1.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
    hsd1.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
    hsd1.Init.BusWide             = SDMMC_BUS_WIDE_4B;
    hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_ENABLE;
    hsd1.Init.ClockDiv            = 9;
    if (HAL_SD_Init(&hsd1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN SDMMC1_Init 2 */
    sd_inited = true;
    HAL_SD_ConfigWideBusOperation(&hsd1, SDMMC_BUS_WIDE_4B);
    /* USER CODE END SDMMC1_Init 2 */
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
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
<<<<<<< HEAD
=======
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
>>>>>>> 6102d31a (add canlogging queue)
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : LED_Pin */
    GPIO_InitStruct.Pin   = LED_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

<<<<<<< HEAD
    /*Configure GPIO pin : PD8 */
    GPIO_InitStruct.Pin       = GPIO_PIN_8;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
=======
    /*Configure GPIO pin : PA8 */
    GPIO_InitStruct.Pin  = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
>>>>>>> d9a69f1b (write message to sd card)

<<<<<<< HEAD
    /*Configure GPIO pin : PD0 */
    GPIO_InitStruct.Pin       = GPIO_PIN_0;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : PB5 PB6 */
    GPIO_InitStruct.Pin       = GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

=======
>>>>>>> 2a5cb3af (10k with 3% loss)
    /* USER CODE BEGIN MX_GPIO_Init_2 */
    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

static void canMsgRecievecallback(CanMsg *rx_msg)
{
    // TODO: check gpio present
    static uint32_t id = 0;
    rx_msg->std_id     = id;
    id++;
    io_can_msgReceivedCallback(rx_msg);
    io_canLogging_msgReceivedCallback(rx_msg);
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_runDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_runDefaultTask */
void runDefaultTask(void *argument)
{
    /* USER CODE BEGIN 5 */
    UART modem_uart = { .handle = &huart9 };
    /* Infinite loop */
    // uint8_t message[7] = { 66, 79, 79, 66, 83, 13, 10 };
    // uint8_t num; // use this if just want numbers
    // uint8_t predicData[3];
    // predicData[1] = 13;
    // predicData[2] = 10;
    uint8_t buffer[128];
    uint8_t message_length;
    bool    status;

    // SimpleMessage message = SimpleMessage_init_zero;

    for (;;)
    {
        /* Create a stream that will write to our buffer. */
        TelemMessage message = TelemMessage_init_zero;
        pb_ostream_t stream  = pb_ostream_from_buffer(buffer, sizeof(buffer));

<<<<<<< HEAD
        /* Fill in the lucky number */
        message.can_id     = 53;
        message.data       = 23;
        message.time_stamp = 9;

        /* Now we are ready to encode the message! */
        status         = pb_encode(&stream, TelemMessage_fields, &message);
        message_length = (uint8_t)stream.bytes_written;
        // message_length = stream.bytes_written;
        if (status)
        {
            hw_uart_transmitPoll(&modem_uart, &message_length, 1, 100);
            hw_uart_transmitPoll(&modem_uart, buffer, sizeof(buffer), 100); // fun string
        }
        osDelay(10);
=======
        CanMsg msg = {
            .std_id = 100,
            .dlc    = 0,
        };
        io_can_pushTxMsgToQueue(&msg);
>>>>>>> 672ea0c3 (rm bootload for now)
    }
    /* USER CODE END 5 */
}

<<<<<<< HEAD
=======
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

    for (unsigned int i = 0; i < 10000; i++)
    {
        CanMsg msg = { .std_id = i, .dlc = 8, .data = { 0, 1, 2, 3, 4, 5, 6, 7 } };
        for (int j = 0; j < 6; j++)
        {
            read_num++;
            io_canLogging_pushTxMsgToQueue(&msg);
        }
        osDelay(1);
    }

    osDelay(osWaitForever);
    /* USER CODE END runCanTxTask */
}

/* USER CODE BEGIN Header_runCanRxTask */
/**
 * @brief Function implementing the canRxTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_runCanRxTask */
void runCanRxTask(void *argument)
{
    /* USER CODE BEGIN runCanRxTask */
    /* Infinite loop */
    static uint32_t count = 0;
    for (;;)
    {
        // CanMsg msg;
        // io_can_popRxMsgFromQueue(&msg);
        io_canLogging_recordMsgFromQueue();
        write_num++;
        count++;

        if (count > 256)
        {
            io_canLogging_sync();
            count = 0;
        }
    }
    /* USER CODE END runCanRxTask */
}

>>>>>>> 672ea0c3 (rm bootload for now)
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
