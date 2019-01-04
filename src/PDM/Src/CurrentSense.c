/******************************************************************************
 * Includes
 ******************************************************************************/
#include "CurrentSense.h"
#include "Adc.h"
#include "Gpio.h"
#include "SharedAdc.h"
#include "SharedFilters.h"
#include "SharedGpio.h"

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
// Internal variable to keep track of which SENSE output is currently selected
volatile SenseChannel_Enum sense_channel = SENSE_0;
float32_t efuse_currents[NUM_EFUSES];

// TODO (Issue #191): Perhaps VOLTAGE_TO_CURRENT and MAX_FAULTS can be combined
// into a struct?
// TODO (Issue #191): Can this not be a static const? Or can it be in .c file
// instead at least
static const float32_t VOLTAGE_TO_CURRENT[NUM_EFUSES] = {
    CURRENT_SCALING_AUX / SENSE_RESISTANCE,
    CURRENT_SCALING / SENSE_RESISTANCE,
    CURRENT_SCALING / SENSE_RESISTANCE,
    CURRENT_SCALING / SENSE_RESISTANCE,
    CURRENT_SCALING / SENSE_RESISTANCE,
    CURRENT_SCALING_AUX / SENSE_RESISTANCE,
    CURRENT_SCALING / SENSE_RESISTANCE,
    CURRENT_SCALING / SENSE_RESISTANCE,
    CURRENT_SCALING / SENSE_RESISTANCE,
    CURRENT_SCALING / SENSE_RESISTANCE
};

/******************************************************************************
 * Private Function Prototypes
 ******************************************************************************/

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void CurrentSense_ConvertCurrentAdcReadings(void)
{
    EfuseCurrentIndex_Enum start_index;

    if (CurrentSense_GetCurrentSenseChannel() == SENSE_0)
    {
        start_index = AUXILIARY_1;
    }
    else
    {
        start_index = AUXILIARY_2;
    }

    for (EfuseCurrentIndex_Enum i = start_index, j = ADC_READINGS_CURRENT_START_INDEX;
                  i < (start_index + NUM_PROFET2S) && j < NUM_EFUSES;
                  i++, j++)
    {
        // Convert ADC readings to current values
        float32_t temp_current = adc_readings[j] * VOLTAGE_TO_CURRENT[i] * VDDA_VOLTAGE / adc_max_value;

        // Apply low pass filter to current values
        SharedFilter_LowPassFilter(&temp_current,
                                   &efuse_currents[i],
                                   IIR_LPF_SAMPLING_PERIOD,
                                   IIR_LPF_RC);
    }
}

SenseChannel_Enum CurrentSense_GetCurrentSenseChannel(void)
{
    return sense_channel;
}

void CurrentSense_ToggleCurrentSenseChannel(void)
{
    if (CurrentSense_GetCurrentSenseChannel() == SENSE_0)
    {
        CurrentSense_SelectCurrentSenseChannel(SENSE_1);
    }
    else
    {
        CurrentSense_SelectCurrentSenseChannel(SENSE_0);
    }
}

void CurrentSense_SelectCurrentSenseChannel(SenseChannel_Enum sense_channel)
{
    // This is the diagnostic truth table for BTS7008-2EPA:
    //
    // DEN  | DSEL           | IS
    // =====|================|================
    // low  | not relevant   | Z
    // high | low            | SENSE output 0
    // high | high           | SENSE output 1

    // Select the correct DSEL value based on what SENSE channel is requested
    if (sense_channel == SENSE_0)
    {
        Gpio_ConfigureAllDsels(DSEL_LOW);
    }
    else
    {
        Gpio_ConfigureAllDsels(DSEL_HIGH);
    }

}
