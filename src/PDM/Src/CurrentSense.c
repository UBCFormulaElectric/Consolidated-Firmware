/******************************************************************************
 * Includes
 ******************************************************************************/
#include "CurrentSense.h"
#include "Adc.h"
#include "SharedAdc.h"
#include "SharedFilters.h"
#include "SharedGpio.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/
// clang-format off

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/
// clang-format on

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
// Internal variable to keep track of which SENSE output is currently selected
volatile SenseChannel_Enum sense_channel = SENSE_0;

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
    for (uint32_t i = 0, j = ADC_READINGS_VOLTAGE_START_INDEX; i < NUM_PROFET2S;
         i++, j++)
    {
        Efuse_Struct *efuse =
            &(Profet2_GetProfet2s()[i]
                  .efuse[CurrentSense_GetCurrentSenseChannel()]);

        // Convert ADC values to current values
        float32_t temp_current =
            (float32_t)(SharedAdc_GetAdcValues()[j]) * efuse->ampere_per_volt *
            SharedAdc_GetActualVdda() / (float32_t)(SharedAdc_GetAdcMaxValue());

        // Apply low pass filter to current values
        SharedFilters_LowPassFilter(
            &temp_current, (float32_t *)(&efuse->current),
            CURRENT_IIR_LPF_SAMPLING_PERIOD, CURRENT_IIR_LPF_RC);
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

void CurrentSense_SelectCurrentSenseChannel(SenseChannel_Enum channel)
{
    // This is the diagnostic truth table for BTS7008-2EPA (Note that in our
    // implementation, every DEN pin is always high):
    //
    // DEN  | DSEL           | IS
    // =====|================|================
    // low  | not relevant   | Z
    // high | low            | SENSE output 0
    // high | high           | SENSE output 1

    // Select the correct DSEL value based on what SENSE channel is requested
    if (channel == SENSE_0)
    {
        sense_channel = SENSE_1;
        Profet2_ConfigureAllDsels(DSEL_LOW);
    }
    else
    {
        sense_channel = SENSE_0;
        Profet2_ConfigureAllDsels(DSEL_HIGH);
    }
}
