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

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
uint32_t voltage_sense_readings[NUM_VOLTAGE_SENSE_PINS] = { 0 };

/******************************************************************************
* Private Function Prototypes
*******************************************************************************/

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void VoltageSense_ConvertVoltageReadings(void)
{
    for (uint32_t i = 0; i < NUM_VOLTAGE_SENSE_PINS; i ++)
    {
        voltage_sense_readings[];
    }

    converted_readings[_12V_SUPPLY] =
        filtered_adc_readings[adc_channel] * GLV_VOLTAGE / adc_max_value;
    adc_channel++;

    converted_readings[VBAT_SUPPLY] =

        filtered_adc_readings[adc_channel] * VBAT_VOLTAGE / adc_max_value;
    adc_channel++;

    converted_readings[FLYWIRE] = filtered_adc_readings[adc_channel] *
                                  ADC1_IN10_TO_12V_ACC_RATIO * VDDA_VOLTAGE /
                                  adc_max_value;

}
