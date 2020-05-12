#include <stdint.h>
#include <assert.h>
#include "Io_SharedAdc.h"
#include "App_SharedConstants.h"

struct AdcInput
{
    ADC_HandleTypeDef *hadc;
    size_t             vrefint_index;
    uint32_t           num_active_channels;
    uint16_t           adc_max_value;
    uint16_t *         adc_readings;
};

// Pointer to the VREFINT measured by the ADC
static uint16_t *measured_vrefint_address;

/**
 * Gets the ADC resolution for the given ADC handle
 * @param hadc The ADC handle for the ADC we are initializing
 * @return adc_max_resolution The maximum ADC resolution for the given ADC
 * handle
 */
static ErrorStatus
    Io_GetAdcResolution(ADC_HandleTypeDef *hadc, uint16_t *adc_max_resolution);

static ErrorStatus
    Io_GetAdcResolution(ADC_HandleTypeDef *hadc, uint16_t *adc_max_resolution)
{
    switch (hadc->Init.Resolution)
    {
        case ADC_RESOLUTION_6B:
            *adc_max_resolution = MAX_6_BITS_VALUE;
            break;
        case ADC_RESOLUTION_8B:
            *adc_max_resolution = MAX_8_BITS_VALUE;
            break;
        case ADC_RESOLUTION_10B:
            *adc_max_resolution = MAX_10_BITS_VALUE;
            break;
        case ADC_RESOLUTION_12B:
            *adc_max_resolution = MAX_12_BITS_VALUE;
            break;
        default:
            return ERROR;
    }

    return SUCCESS;
}

struct AdcInput *
    Io_SharedAdc_Create(ADC_HandleTypeDef *hadc, size_t vrefint_index)
{
    assert(hadc != NULL);

    static size_t          alloc_index = 0;
    static struct AdcInput adc_inputs[MAX_NUM_OF_ADC];
    static uint32_t adc_readings[MAX_NUM_OF_ADC][MAX_NUM_OF_ADC_CHANNELS];

    struct AdcInput *const adc_input = &adc_inputs[alloc_index];

    assert(alloc_index < MAX_NUM_OF_ADC);
    assert(Io_GetAdcResolution(hadc, &adc_input->adc_max_value) == SUCCESS);

    adc_input->hadc                = hadc;
    adc_input->num_active_channels = hadc->Init.NbrOfConversion;
    adc_input->adc_readings        = (uint16_t *)adc_readings[alloc_index];

    HAL_ADCEx_Calibration_Start(hadc, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(
        hadc, (uint32_t *)adc_input->adc_readings,
        adc_input->num_active_channels);

    if (alloc_index == 0U)
    {
        // Initialize ADC instance containing VREFINT before initializing other
        // ADCs
        assert(vrefint_index <= adc_input->num_active_channels);

        adc_inputs->vrefint_index = vrefint_index - 1;
        // Store the pointer pointing to the measured VREFINT value
        measured_vrefint_address =
            &adc_input->adc_readings[adc_input->vrefint_index];
    }
    else
    {
        UNUSED(vrefint_index);
    }

    alloc_index++;
    return adc_input;
}

float Io_SharedAdc_GetChannelVoltage(
    const struct AdcInput *const adc_input,
    size_t                       adc_rank)
{
    assert(adc_rank > 0 && adc_rank <= adc_input->num_active_channels);

    // This function performs the following:
    // (1) Calculate the microcontroller's actual VDDA value using the following
    // formula
    //
    //                    3.3V x VREFINT_CAL
    //  ACTUAL_VDDA = ---------------------------
    //                      VREFINT_MEASURED
    //
    // VREFINT_CAL is the VREFINT calibration value (stored at 0x1FFF7BA)
    // VREFINT_MEASURED is the actual VREFINT output value converted by an ADC
    //
    // (2) Calculate the voltage of the given ADC Channel
    //
    //                 ACTUAL_VDDA x ADCx_READINGS
    //  V_CHANNELx = ----------------------------
    //                       ADC_MAX_VALUE
    //
    // ACTUAL_VDDA is the vdda value computed in step (1)
    // ADCx_DATA is the value of the channel measured by the ADC
    // ADC_MAX_VALUE is the maximum ADC value

    const uint16_t *const VREFINT_CALIBRATION_ADDRESS =
        (uint16_t *)(0x1FFFF7BA);
    const float ACTUAL_VDDA =
        (float)(3.3f * (uint16_t)(*VREFINT_CALIBRATION_ADDRESS) / *measured_vrefint_address);

    return (
        float)(ACTUAL_VDDA * adc_input->adc_readings[adc_rank - 1] / adc_input->adc_max_value);
}

ADC_HandleTypeDef *
    Io_SharedAdc_GetAdcHandle(const struct AdcInput *const adc_input)
{
    return adc_input->hadc;
}

uint32_t
    Io_SharedAdc_GetNumActiveChannel(const struct AdcInput *const adc_input)
{
    return adc_input->num_active_channels;
}
