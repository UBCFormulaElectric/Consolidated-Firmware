/******************************************************************************
 * Includes
 ******************************************************************************/
#include "VoltageSense.h"
#include "SharedFilters.h"
#include "Gpio.h"
#include "Adc.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/
// clang-format off

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/
#define INIT_VOLTAGE_SENSE(index, conversion_factor) \
    [index].voltage = 0, [index].adc_conversion_factor = conversion_factor

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/
// clang-format on
typedef struct
{
    float32_t voltage;
    float32_t adc_conversion_factor;
} VoltageSense_Struct;

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
static VoltageSense_Struct voltage_sense[NUM_VOLTAGE_SENSE_PINS] = {
    INIT_VOLTAGE_SENSE(_12V_SUPPLY, GLV_VOLTAGE),
    INIT_VOLTAGE_SENSE(VBAT_SUPPLY, VBAT_VOLTAGE),
    INIT_VOLTAGE_SENSE(FLYWIRE, ADC1_IN10_TO_12V_ACC_RATIO),
};

/******************************************************************************
 * Private Function Prototypes
 *******************************************************************************/

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void VoltageSense_ConvertVoltageAdcReadings(void)
{
    for (VoltageSenseIndex_Enum i = 0, j = ADC_READINGS_VOLTAGE_START_INDEX;
         i < NUM_VOLTAGE_SENSE_PINS; i++, j++)
    {
        // Convert ADC readings to voltage values
        float32_t temp_voltage = (float32_t)(SharedAdc_GetAdcValues()[j]) *
                                 voltage_sense[i].adc_conversion_factor /
                                 (float32_t)(SharedAdc_GetAdcMaxValue());

        // Apply low pass filter to current values
        SharedFilters_LowPassFilter(
            &temp_voltage, &voltage_sense[i].voltage,
            VOLTAGE_IIR_LPF_SAMPLING_PERIOD, VOLTAGE_IIR_LPF_RC);
    }
}
