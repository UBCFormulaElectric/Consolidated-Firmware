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

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/
#define INIT_EFUSE(index, \
                   efuse_pin, \
                   efuse_port, \
                   efuse_ampere_per_volt) \
        [index].pin_mapping.pin = efuse_pin, \
        [index].pin_mapping.port = efuse_port, \
        [index].current = 0, \
        [index].ampere_per_volt = efuse_ampere_per_volt,

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
// Internal variable to keep track of which SENSE output is currently selected
volatile SenseChannel_Enum sense_channel = SENSE_0;

const efuse_struct efuse_lut[NUM_EFUSES] =
{
    INIT_EFUSE(AUXILIARY_1, EFUSE_AUX_1_IN_Pin, EFUSE_AUX_1_IN_GPIO_Port, AMP_PER_VOLT_AUX)
    INIT_EFUSE(COOLING, EFUSE_COOLING_IN_Pin, EFUSE_COOLING_IN_GPIO_Port, AMP_PER_VOLT)
    INIT_EFUSE(AIR_SHDN, EFUSE_AIR_SHDN_IN_Pin, EFUSE_AIR_SHDN_IN_GPIO_Port, AMP_PER_VOLT)
    INIT_EFUSE(ACC_SEGMENT_FAN, EFUSE_ACC_SEG_FAN_IN_Pin, EFUSE_ACC_SEG_FAN_IN_GPIO_Port, AMP_PER_VOLT)
    INIT_EFUSE(LEFT_INVERTER, EFUSE_LEFT_INVERTER_IN_Pin, EFUSE_LEFT_INVERTER_IN_GPIO_Port, AMP_PER_VOLT)
    INIT_EFUSE(AUXILIARY_2, EFUSE_AUX_2_IN_Pin, EFUSE_AUX_2_IN_GPIO_Port, AMP_PER_VOLT_AUX)
    INIT_EFUSE(PDM_FAN, EFUSE_PDM_FAN_IN_Pin, EFUSE_PDM_FAN_IN_GPIO_Port, AMP_PER_VOLT)
    INIT_EFUSE(CAN_GLV, EFUSE_CAN_IN_Pin, EFUSE_CAN_IN_GPIO_Port, AMP_PER_VOLT)
    INIT_EFUSE(ACC_ENCLOSURE_FAN, EFUSE_ACC_ENC_FAN_IN_Pin, EFUSE_ACC_ENC_FAN_IN_GPIO_Port, AMP_PER_VOLT)
    INIT_EFUSE(RIGHT_INVERTER, EFUSE_RIGHT_INVERTER_IN_Pin, EFUSE_RIGHT_INVERTER_IN_GPIO_Port, AMP_PER_VOLT)
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
                  i < (start_index + NUM_PROFET2S);
                  i++, j++)
    {
        // Convert ADC readings to current values
       float32_t temp_current = (float32_t)(SharedAdc_GetAdcReadings()[j]) * efuse_lut[i].ampere_per_volt 
                                 * VDDA_VOLTAGE / (float32_t)(SharedAdc_GetAdcMaxValue());

        // Apply low pass filter to current values
        SharedFilter_LowPassFilter(&temp_current,
                                   &efuse_lut[i].current,
                                   CURRENT_IIR_LPF_SAMPLING_PERIOD,
                                   CURRENT_IIR_LPF_RC);
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
