
/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Io_AdcDriver.h"
#include "App_Constants.h"
#include "main.h"
#include "stdbool.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
/** @brief The maximum ADC value depends on the selected ADC resolution */
static uint32_t adc_max_value;

/** @brief Array of raw ADC values */
static uint32_t adc_values[NUM_ADC_CHANNELS] = { 0 };

/** @brief Keep track of which element in adc_values[] is VREFINT (The default
 *         value is a garbage value that doesn't correspond to any Regular Rank)
 */
static uint32_t vrefint_index = NUM_ADC_CHANNELS;

/** @brief Keep track of whether the ADC has been initialized yet */
static bool adc_initialized = false;

/******************************************************************************
 * Private Function Prototypes
 *******************************************************************************/
/**
 * @brief  Configure the maximum ADC value based on ADC resolution
 * @param  hadc ADC handle
 * @return ErrorStatus
 */
static ErrorStatus InitializeAdcMaxValue(ADC_HandleTypeDef *hadc);

/**
 * @brief  Return the VREFINT calibration value acquired by the ADC during the
 *         manufacturing process at VDDA = 3.3V (Note that this is a 16-bit
 *         value)
 * @return VDDA calibration data
 */
static uint16_t GetVrefintCalibrationValue(void);

/**
 * @brief Check if the ADC module has been initialized using
 *        Io_AdcDriver_StartAdcInDmaMode()
 * @note  This should be called at the beginning of functions that require ADC
 *        to be initialized
 */
static void CheckAdcInitialized(void);

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/
static ErrorStatus InitializeAdcMaxValue(ADC_HandleTypeDef *hadc)
{
    switch (hadc->Init.Resolution)
    {
        case ADC_RESOLUTION_6B:
            adc_max_value = MAX_6_BITS_VALUE;
            break;
        case ADC_RESOLUTION_8B:
            adc_max_value = MAX_8_BITS_VALUE;
            break;
        case ADC_RESOLUTION_10B:
            adc_max_value = MAX_10_BITS_VALUE;
            break;
        case ADC_RESOLUTION_12B:
            adc_max_value = MAX_12_BITS_VALUE;
            break;
        default:
            return ERROR;
    }
    return SUCCESS;
}

static uint16_t GetVrefintCalibrationValue(void)
{
    return (uint16_t)(*VREFINT_CALIBRATION_ADDRESS);
}

static void CheckAdcInitialized(void)
{
    if (adc_initialized != true)
    {
        Error_Handler();
    }
}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void Io_AdcDriver_StartAdcInDmaMode(
    ADC_HandleTypeDef *hadc,
    uint32_t           vrefint_regular_rank)
{
    // The index of each ADC channel is always its Regular Rank minus one
    vrefint_index = vrefint_regular_rank - 1;
    bool status   = true;

    if (HAL_ADCEx_Calibration_Start(hadc, ADC_SINGLE_ENDED) != HAL_OK)
    {
        status = false;
        Error_Handler();
    }

    if (HAL_ADC_Start_DMA(hadc, &adc_values[0], NUM_ADC_CHANNELS) != HAL_OK)
    {
        status = false;
        Error_Handler();
    }

    if (InitializeAdcMaxValue(hadc) != SUCCESS)
    {
        status = false;
        Error_Handler();
    }

    // Only set adc_initialized to true if all initialization steps are
    // error-free
    if (status == true)
    {
        adc_initialized = true;
    }
}

uint32_t Io_AdcDriver_GetAdcMaxValue(void)
{
    CheckAdcInitialized();

    return (const uint32_t)adc_max_value;
}

const uint32_t *Io_AdcDriver_GetAdcValues(void)
{
    CheckAdcInitialized();

    return (const uint32_t *)adc_values;
}

float32_t Io_AdcDriver_GetActualVdda(void)
{
    CheckAdcInitialized();

    // The actual VDDA voltage supplying the microcontroller can be calculated
    // using the following formula:
    //
    //         3.3V x VREFINT_CAL
    // VDDA = ---------------------
    //           VREFINT_DATA
    //
    // Where:
    // - VREFINT_CAL is the VREFINT calibration value
    // - VREFINT_DATA is the actual VREFINT output value converted by the ADC

    return (float32_t)(
        3.3f * (float32_t)(GetVrefintCalibrationValue()) /
        adc_values[vrefint_index]);
}

float32_t Io_AdcDriver_GetAdcVoltage(uint32_t regular_rank)
{
    CheckAdcInitialized();

    // The index of each ADC channel is always its Regular Rank minus one
    uint32_t adc_values_index = regular_rank - 1;

    //  The voltage at any ADC channel can be calculated using the following
    //  generic formula:
    //
    //                   ACTUAL_VDDA x ADCx_DATA
    //    V_CHANNELx = ----------------------------
    //                          FULL_SCALE
    //
    // Where:
    // - ADC_DATAx is the value measured by the ADC on channel x (right-aligned)
    // - FULL_SCALE is the maximum digital value of the ADC output. For example
    //   with 12-bit resolution, it will be 212 - 1 = 4095 or with 8-bit
    //   resolution, 28 - 1 = 255.

    return (float32_t)(Io_AdcDriver_GetActualVdda()) *
           (float32_t)(Io_AdcDriver_GetAdcValues()[adc_values_index]) /
           (float32_t)(Io_AdcDriver_GetAdcMaxValue());
}
