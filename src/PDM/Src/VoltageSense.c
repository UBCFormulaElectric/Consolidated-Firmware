/******************************************************************************
 * Includes
 ******************************************************************************/
#include "VoltageSense.h"
#include "Gpio.h"
#include "Adc.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/
#define INIT_VOLTAGE_SENSE(index, scaling_factor) \
        [index].voltage = 0, \
        [index].scaling_factor = scaling_factor, \

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/
typedef struct
{
    float32_t voltage;
    float32_t scaling_factor;
} VoltageSense_Struct; 

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
static VoltageSense_Struct voltage_sense_lut[NUM_VOLTAGE_SENSE_PINS] =
{
    INIT_VOLTAGE_SENSE(_12V_SUPPLY, GLV_VOLTAGE)
    INIT_VOLTAGE_SENSE(VBAT_SUPPLY, VBAT_VOLTAGE)
    INIT_VOLTAGE_SENSE(FLYWIRE, ADC1_IN10_TO_12V_ACC_RATIO * VDDA_VOLTAGE)
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
                  i < NUM_VOLTAGE_SENSE_PINS;
                  i++, j++)
    {
        // Convert ADC readings to voltage values
       float32_t temp_voltage = (float32_t)(SharedAdc_GetAdcReadings()[j]) * voltage_sense_lut[i].scaling_factor / (float32_t)(SharedAdc_GetAdcMaxValue());

        // Apply low pass filter to current values
        SharedFilter_LowPassFilter(&temp_voltage,
                                   &voltage_sense_lut[i].voltage,
                                   VOLTAGE_IIR_LPF_SAMPLING_PERIOD,
                                   VOLTAGE_IIR_LPF_RC);
    }
}
