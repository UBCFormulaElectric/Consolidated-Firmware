/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os2.h"
#include "tasks.h"

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

FDCAN_HandleTypeDef hfdcan1;
FDCAN_HandleTypeDef hfdcan3;

I2C_HandleTypeDef hi2c4;
I2C_HandleTypeDef hi2c5;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart8;

PCD_HandleTypeDef hpcd_USB_OTG_HS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void        SystemClock_Config(void);
void        PeriphCommonClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USB_OTG_HS_PCD_Init(void);
static void MX_ADC2_Init(void);
static void MX_FDCAN1_Init(void);
static void MX_FDCAN3_Init(void);
static void MX_I2C4_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_I2C5_Init(void);
static void MX_UART8_Init(void);
static void MX_TIM3_Init(void);
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

    /* Configure the peripherals common clocks */
    PeriphCommonClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_USB_OTG_HS_PCD_Init();
    MX_ADC2_Init();
    MX_FDCAN1_Init();
    MX_FDCAN3_Init();
    MX_I2C4_Init();
    MX_SPI1_Init();
    MX_SPI2_Init();
    MX_I2C5_Init();
    MX_UART8_Init();
    MX_TIM3_Init();
    /* USER CODE BEGIN 2 */
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
    ;
    tasks_init();
    /* USER CODE END 2 */

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
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
    {
    }

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState            = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = 64;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM            = 32;
    RCC_OscInitStruct.PLL.PLLN            = 129;
    RCC_OscInitStruct.PLL.PLLP            = 2;
    RCC_OscInitStruct.PLL.PLLQ            = 2;
    RCC_OscInitStruct.PLL.PLLR            = 2;
    RCC_OscInitStruct.PLL.PLLRGE          = RCC_PLL1VCIRANGE_1;
    RCC_OscInitStruct.PLL.PLLVCOSEL       = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN        = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                  RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
void PeriphCommonClock_Config(void)
{
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };

    /** Initializes the peripherals clock
     */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInitStruct.PLL3.PLL3M           = 32;
    PeriphClkInitStruct.PLL3.PLL3N           = 129;
    PeriphClkInitStruct.PLL3.PLL3P           = 2;
    PeriphClkInitStruct.PLL3.PLL3Q           = 2;
    PeriphClkInitStruct.PLL3.PLL3R           = 2;
    PeriphClkInitStruct.PLL3.PLL3RGE         = RCC_PLL3VCIRANGE_1;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL      = RCC_PLL3VCOWIDE;
    PeriphClkInitStruct.PLL3.PLL3FRACN       = 0;
    PeriphClkInitStruct.AdcClockSelection    = RCC_ADCCLKSOURCE_PLL3;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
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
    hadc1.Instance                      = ADC1;
    hadc1.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV4;
    hadc1.Init.Resolution               = ADC_RESOLUTION_16B;
    hadc1.Init.ScanConvMode             = ADC_SCAN_ENABLE;
    hadc1.Init.EOCSelection             = ADC_EOC_SEQ_CONV;
    hadc1.Init.LowPowerAutoWait         = DISABLE;
    hadc1.Init.ContinuousConvMode       = DISABLE;
    hadc1.Init.NbrOfConversion          = 6;
    hadc1.Init.DiscontinuousConvMode    = DISABLE;
    hadc1.Init.ExternalTrigConv         = ADC_EXTERNALTRIG_T3_TRGO;
    hadc1.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
    hadc1.Init.Overrun                  = ADC_OVR_DATA_PRESERVED;
    hadc1.Init.LeftBitShift             = ADC_LEFTBITSHIFT_NONE;
    hadc1.Init.OversamplingMode         = DISABLE;
    hadc1.Init.Oversampling.Ratio       = 1;
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
    sConfig.Channel                = ADC_CHANNEL_8;
    sConfig.Rank                   = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime           = ADC_SAMPLETIME_1CYCLE_5;
    sConfig.SingleDiff             = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber           = ADC_OFFSET_NONE;
    sConfig.Offset                 = 0;
    sConfig.OffsetSignedSaturation = DISABLE;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_9;
    sConfig.Rank    = ADC_REGULAR_RANK_2;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_10;
    sConfig.Rank    = ADC_REGULAR_RANK_3;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_15;
    sConfig.Rank    = ADC_REGULAR_RANK_4;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_16;
    sConfig.Rank    = ADC_REGULAR_RANK_5;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_17;
    sConfig.Rank    = ADC_REGULAR_RANK_6;
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
    hadc2.Instance                      = ADC2;
    hadc2.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV4;
    hadc2.Init.Resolution               = ADC_RESOLUTION_16B;
    hadc2.Init.ScanConvMode             = ADC_SCAN_ENABLE;
    hadc2.Init.EOCSelection             = ADC_EOC_SEQ_CONV;
    hadc2.Init.LowPowerAutoWait         = DISABLE;
    hadc2.Init.ContinuousConvMode       = DISABLE;
    hadc2.Init.NbrOfConversion          = 5;
    hadc2.Init.DiscontinuousConvMode    = DISABLE;
    hadc2.Init.ExternalTrigConv         = ADC_EXTERNALTRIG_T3_TRGO;
    hadc2.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc2.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
    hadc2.Init.Overrun                  = ADC_OVR_DATA_PRESERVED;
    hadc2.Init.LeftBitShift             = ADC_LEFTBITSHIFT_NONE;
    hadc2.Init.OversamplingMode         = DISABLE;
    hadc2.Init.Oversampling.Ratio       = 1;
    if (HAL_ADC_Init(&hadc2) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel                = ADC_CHANNEL_4;
    sConfig.Rank                   = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime           = ADC_SAMPLETIME_1CYCLE_5;
    sConfig.SingleDiff             = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber           = ADC_OFFSET_NONE;
    sConfig.Offset                 = 0;
    sConfig.OffsetSignedSaturation = DISABLE;
    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_5;
    sConfig.Rank    = ADC_REGULAR_RANK_2;
    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_11;
    sConfig.Rank    = ADC_REGULAR_RANK_3;
    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_14;
    sConfig.Rank    = ADC_REGULAR_RANK_4;
    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_18;
    sConfig.Rank    = ADC_REGULAR_RANK_5;
    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC2_Init 2 */

    /* USER CODE END ADC2_Init 2 */
}

/**
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
    hfdcan1.Init.NominalTimeSeg1      = 1;
    hfdcan1.Init.NominalTimeSeg2      = 1;
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
 * @brief FDCAN3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_FDCAN3_Init(void)
{
    /* USER CODE BEGIN FDCAN3_Init 0 */

    /* USER CODE END FDCAN3_Init 0 */

    /* USER CODE BEGIN FDCAN3_Init 1 */

    /* USER CODE END FDCAN3_Init 1 */
    hfdcan3.Instance                  = FDCAN3;
    hfdcan3.Init.FrameFormat          = FDCAN_FRAME_CLASSIC;
    hfdcan3.Init.Mode                 = FDCAN_MODE_NORMAL;
    hfdcan3.Init.AutoRetransmission   = DISABLE;
    hfdcan3.Init.TransmitPause        = DISABLE;
    hfdcan3.Init.ProtocolException    = DISABLE;
    hfdcan3.Init.NominalPrescaler     = 16;
    hfdcan3.Init.NominalSyncJumpWidth = 1;
    hfdcan3.Init.NominalTimeSeg1      = 1;
    hfdcan3.Init.NominalTimeSeg2      = 1;
    hfdcan3.Init.DataPrescaler        = 1;
    hfdcan3.Init.DataSyncJumpWidth    = 1;
    hfdcan3.Init.DataTimeSeg1         = 1;
    hfdcan3.Init.DataTimeSeg2         = 1;
    hfdcan3.Init.MessageRAMOffset     = 0;
    hfdcan3.Init.StdFiltersNbr        = 0;
    hfdcan3.Init.ExtFiltersNbr        = 0;
    hfdcan3.Init.RxFifo0ElmtsNbr      = 0;
    hfdcan3.Init.RxFifo0ElmtSize      = FDCAN_DATA_BYTES_8;
    hfdcan3.Init.RxFifo1ElmtsNbr      = 0;
    hfdcan3.Init.RxFifo1ElmtSize      = FDCAN_DATA_BYTES_8;
    hfdcan3.Init.RxBuffersNbr         = 0;
    hfdcan3.Init.RxBufferSize         = FDCAN_DATA_BYTES_8;
    hfdcan3.Init.TxEventsNbr          = 0;
    hfdcan3.Init.TxBuffersNbr         = 0;
    hfdcan3.Init.TxFifoQueueElmtsNbr  = 0;
    hfdcan3.Init.TxFifoQueueMode      = FDCAN_TX_FIFO_OPERATION;
    hfdcan3.Init.TxElmtSize           = FDCAN_DATA_BYTES_8;
    if (HAL_FDCAN_Init(&hfdcan3) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN FDCAN3_Init 2 */

    /* USER CODE END FDCAN3_Init 2 */
}

/**
 * @brief I2C4 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C4_Init(void)
{
    /* USER CODE BEGIN I2C4_Init 0 */

    /* USER CODE END I2C4_Init 0 */

    /* USER CODE BEGIN I2C4_Init 1 */

    /* USER CODE END I2C4_Init 1 */
    hi2c4.Instance              = I2C4;
    hi2c4.Init.Timing           = 0x10707DBC;
    hi2c4.Init.OwnAddress1      = 0;
    hi2c4.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    hi2c4.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    hi2c4.Init.OwnAddress2      = 0;
    hi2c4.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c4.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    hi2c4.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c4) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Analogue filter
     */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c4, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Digital filter
     */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c4, 0) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C4_Init 2 */

    /* USER CODE END I2C4_Init 2 */
}

/**
 * @brief I2C5 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C5_Init(void)
{
    /* USER CODE BEGIN I2C5_Init 0 */

    /* USER CODE END I2C5_Init 0 */

    /* USER CODE BEGIN I2C5_Init 1 */

    /* USER CODE END I2C5_Init 1 */
    hi2c5.Instance              = I2C5;
    hi2c5.Init.Timing           = 0x10707DBC;
    hi2c5.Init.OwnAddress1      = 0;
    hi2c5.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    hi2c5.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    hi2c5.Init.OwnAddress2      = 0;
    hi2c5.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c5.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    hi2c5.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c5) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Analogue filter
     */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c5, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Digital filter
     */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c5, 0) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C5_Init 2 */

    /* USER CODE END I2C5_Init 2 */
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
    hspi1.Instance                        = SPI1;
    hspi1.Init.Mode                       = SPI_MODE_MASTER;
    hspi1.Init.Direction                  = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize                   = SPI_DATASIZE_4BIT;
    hspi1.Init.CLKPolarity                = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase                   = SPI_PHASE_1EDGE;
    hspi1.Init.NSS                        = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler          = SPI_BAUDRATEPRESCALER_2;
    hspi1.Init.FirstBit                   = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode                     = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation             = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial              = 0x0;
    hspi1.Init.NSSPMode                   = SPI_NSS_PULSE_ENABLE;
    hspi1.Init.NSSPolarity                = SPI_NSS_POLARITY_LOW;
    hspi1.Init.FifoThreshold              = SPI_FIFO_THRESHOLD_01DATA;
    hspi1.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi1.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi1.Init.MasterSSIdleness           = SPI_MASTER_SS_IDLENESS_00CYCLE;
    hspi1.Init.MasterInterDataIdleness    = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    hspi1.Init.MasterReceiverAutoSusp     = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    hspi1.Init.MasterKeepIOState          = SPI_MASTER_KEEP_IO_STATE_DISABLE;
    hspi1.Init.IOSwap                     = SPI_IO_SWAP_DISABLE;
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI1_Init 2 */

    /* USER CODE END SPI1_Init 2 */
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
    hspi2.Instance                        = SPI2;
    hspi2.Init.Mode                       = SPI_MODE_SLAVE;
    hspi2.Init.Direction                  = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize                   = SPI_DATASIZE_4BIT;
    hspi2.Init.CLKPolarity                = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase                   = SPI_PHASE_1EDGE;
    hspi2.Init.NSS                        = SPI_NSS_SOFT;
    hspi2.Init.FirstBit                   = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode                     = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation             = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial              = 0x0;
    hspi2.Init.NSSPMode                   = SPI_NSS_PULSE_DISABLE;
    hspi2.Init.NSSPolarity                = SPI_NSS_POLARITY_LOW;
    hspi2.Init.FifoThreshold              = SPI_FIFO_THRESHOLD_01DATA;
    hspi2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi2.Init.MasterSSIdleness           = SPI_MASTER_SS_IDLENESS_00CYCLE;
    hspi2.Init.MasterInterDataIdleness    = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    hspi2.Init.MasterReceiverAutoSusp     = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    hspi2.Init.MasterKeepIOState          = SPI_MASTER_KEEP_IO_STATE_DISABLE;
    hspi2.Init.IOSwap                     = SPI_IO_SWAP_DISABLE;
    if (HAL_SPI_Init(&hspi2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI2_Init 2 */

    /* USER CODE END SPI2_Init 2 */
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

    TIM_MasterConfigTypeDef sMasterConfig = { 0 };
    TIM_OC_InitTypeDef      sConfigOC     = { 0 };

    /* USER CODE BEGIN TIM3_Init 1 */

    /* USER CODE END TIM3_Init 1 */
    htim3.Instance               = TIM3;
    htim3.Init.Prescaler         = 0;
    htim3.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim3.Init.Period            = 65535;
    htim3.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_OC_Init(&htim3) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode     = TIM_OCMODE_TIMING;
    sConfigOC.Pulse      = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM3_Init 2 */

    /* USER CODE END TIM3_Init 2 */
}

/**
 * @brief UART8 Initialization Function
 * @param None
 * @retval None
 */
static void MX_UART8_Init(void)
{
    /* USER CODE BEGIN UART8_Init 0 */

    /* USER CODE END UART8_Init 0 */

    /* USER CODE BEGIN UART8_Init 1 */

    /* USER CODE END UART8_Init 1 */
    huart8.Instance                    = UART8;
    huart8.Init.BaudRate               = 115200;
    huart8.Init.WordLength             = UART_WORDLENGTH_8B;
    huart8.Init.StopBits               = UART_STOPBITS_1;
    huart8.Init.Parity                 = UART_PARITY_NONE;
    huart8.Init.Mode                   = UART_MODE_TX_RX;
    huart8.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart8.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart8.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart8.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    huart8.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart8, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart8, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&huart8) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN UART8_Init 2 */

    /* USER CODE END UART8_Init 2 */
}

/**
 * @brief USB_OTG_HS Initialization Function
 * @param None
 * @retval None
 */
static void MX_USB_OTG_HS_PCD_Init(void)
{
    /* USER CODE BEGIN USB_OTG_HS_Init 0 */

    /* USER CODE END USB_OTG_HS_Init 0 */

    /* USER CODE BEGIN USB_OTG_HS_Init 1 */

    /* USER CODE END USB_OTG_HS_Init 1 */
    hpcd_USB_OTG_HS.Instance                 = USB_OTG_HS;
    hpcd_USB_OTG_HS.Init.dev_endpoints       = 9;
    hpcd_USB_OTG_HS.Init.speed               = PCD_SPEED_FULL;
    hpcd_USB_OTG_HS.Init.dma_enable          = DISABLE;
    hpcd_USB_OTG_HS.Init.phy_itface          = USB_OTG_EMBEDDED_PHY;
    hpcd_USB_OTG_HS.Init.Sof_enable          = DISABLE;
    hpcd_USB_OTG_HS.Init.low_power_enable    = DISABLE;
    hpcd_USB_OTG_HS.Init.lpm_enable          = DISABLE;
    hpcd_USB_OTG_HS.Init.vbus_sensing_enable = DISABLE;
    hpcd_USB_OTG_HS.Init.use_dedicated_ep1   = DISABLE;
    hpcd_USB_OTG_HS.Init.use_external_vbus   = DISABLE;
    if (HAL_PCD_Init(&hpcd_USB_OTG_HS) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USB_OTG_HS_Init 2 */

    /* USER CODE END USB_OTG_HS_Init 2 */
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
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(
        GPIOE, IMU_CS1_Pin | IMU_CS2_Pin | IMU_CS3_Pin | LED_Pin | BOOT_Pin | INERTIA_3V3_OUT_Pin | IMU_FSYNC_Pin,
        GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(
        GPIOB, TSMS_3V3_OUT_Pin | BAT_CHRG_nSHDN_Pin | BMS_EN_Pin | FRONT_EN_Pin | RL_PUMP_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(
        GPIOD, F_INV_EN_Pin | SB_SHDN_3V3_OUT_Pin | RSM_EN_Pin | R_INV_EN_Pin | R_RAD_FAN_EN_Pin | L_RAD_FAN_EN_Pin,
        GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, DAM_EN_Pin | PCM_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(RR_PUMP_EN_GPIO_Port, RR_PUMP_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : IMU_CS1_Pin IMU_CS2_Pin IMU_CS3_Pin LED_Pin
                             BOOT_Pin INERTIA_3V3_OUT_Pin IMU_FSYNC_Pin */
    GPIO_InitStruct.Pin =
        IMU_CS1_Pin | IMU_CS2_Pin | IMU_CS3_Pin | LED_Pin | BOOT_Pin | INERTIA_3V3_OUT_Pin | IMU_FSYNC_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : IMU_INT1_Pin IMU_INT2_Pin */
    GPIO_InitStruct.Pin  = IMU_INT1_Pin | IMU_INT2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pin : IMU_INT3_Pin */
    GPIO_InitStruct.Pin  = IMU_INT3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(IMU_INT3_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : TSMS_3V3_OUT_Pin BAT_CHRG_nSHDN_Pin BMS_EN_Pin FRONT_EN_Pin
                             RL_PUMP_EN_Pin */
    GPIO_InitStruct.Pin   = TSMS_3V3_OUT_Pin | BAT_CHRG_nSHDN_Pin | BMS_EN_Pin | FRONT_EN_Pin | RL_PUMP_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : RR_ILCK_3V3_OUT_Pin BMS_PG_Pin PWR_MTR_nALERT_Pin BOOST_PG_Pin
                             BAT_CHRG_MODE_Pin */
    GPIO_InitStruct.Pin  = RR_ILCK_3V3_OUT_Pin | BMS_PG_Pin | PWR_MTR_nALERT_Pin | BOOST_PG_Pin | BAT_CHRG_MODE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : BAT_MTR_nALERT_Pin FRONT_PG_Pin RL_PUMP_PGOOD_Pin */
    GPIO_InitStruct.Pin  = BAT_MTR_nALERT_Pin | FRONT_PG_Pin | RL_PUMP_PGOOD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : R_INV_PG_Pin F_INV_PG_Pin L_RAD_FAN_PG_Pin R_RAD_FAN_PG_Pin */
    GPIO_InitStruct.Pin  = R_INV_PG_Pin | F_INV_PG_Pin | L_RAD_FAN_PG_Pin | R_RAD_FAN_PG_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : F_INV_EN_Pin SB_SHDN_3V3_OUT_Pin RSM_EN_Pin R_INV_EN_Pin
                             R_RAD_FAN_EN_Pin L_RAD_FAN_EN_Pin */
    GPIO_InitStruct.Pin =
        F_INV_EN_Pin | SB_SHDN_3V3_OUT_Pin | RSM_EN_Pin | R_INV_EN_Pin | R_RAD_FAN_EN_Pin | L_RAD_FAN_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : RSM_PG_Pin DAM_PG_Pin */
    GPIO_InitStruct.Pin  = RSM_PG_Pin | DAM_PG_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : DAM_EN_Pin PCM_EN_Pin */
    GPIO_InitStruct.Pin   = DAM_EN_Pin | PCM_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : RR_PUMP_EN_Pin */
    GPIO_InitStruct.Pin   = RR_PUMP_EN_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RR_PUMP_EN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : RR_PUMP_PGOOD_Pin */
    GPIO_InitStruct.Pin  = RR_PUMP_PGOOD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(RR_PUMP_PGOOD_GPIO_Port, &GPIO_InitStruct);

    /* USER CODE BEGIN MX_GPIO_Init_2 */

    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
