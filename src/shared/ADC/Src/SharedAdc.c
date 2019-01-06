
/******************************************************************************
 * Includes
 ******************************************************************************/
#include "SharedAdc.h"
#include "Constants.h"
#include "main.h"

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
static uint32_t adc_readings[NUM_ADC_CHANNELS] = { 0 };

/******************************************************************************
 * Private Function Prototypes
 *******************************************************************************/
/**
 * @brief  Configure the maximum ADC value based on ADC resolution
 * @param  hadc ADC handle
 * @return ErrorStatus
 */
static ErrorStatus InitializeAdcMaxValue(ADC_HandleTypeDef *hadc);

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

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void SharedAdc_StartAdcInDmaMode(
    ADC_HandleTypeDef *hadc,
    uint32_t *         data,
    uint32_t           length)
{
    if (HAL_ADCEx_Calibration_Start(hadc, ADC_SINGLE_ENDED) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_ADC_Start_DMA(hadc, data, length) != HAL_OK)
    {
        Error_Handler();
    }

    if (InitializeAdcMaxValue(hadc) != SUCCESS)
    {
        Error_Handler();
    }
}

const uint32_t SharedAdc_GetAdcMaxValue(void)
{
    return (const uint32_t)adc_max_value;
}

const uint32_t *SharedAdc_GetAdcReadings(void)
{
    return (const uint32_t *)adc_readings;
}
