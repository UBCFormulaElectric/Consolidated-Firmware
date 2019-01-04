/******************************************************************************
 * Includes
 ******************************************************************************/
#include "CurrentSense.h"
#include "SharedAdc.h"
#include "Gpio.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
volatile SenseChannel_Enum sense_channel = SENSE_0;

/******************************************************************************
 * Private Function Prototypes
 ******************************************************************************/
/**
 * @brief  Helper function to shift starting index in any ADC/Converted Readings
 *         array, depending on dsel_state.
 *         DSEL_LOW corresponds to the first half of the array
 *         DSEL_HIGH corresponds to the second half of the array
 */
static uint8_t CurrentSense_DSELShiftIndex(void);

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/
static uint8_t CurrentSense_DSELShiftIndex(void)
{
    if (CurrentSense_GetCurrentSenseChannel() == SENSE_0)
    {
        return 0;
    }
    else
    {
        return NUM_ADC_CHANNELS;
    }
}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void CurrentSense_LowPassFilterADCReadings(volatile uint32_t *adc_readings)
{
    uint8_t adc_channel = CurrentSense_DSELShiftIndex();
    uint8_t final_index = adc_channel + NUM_ADC_CHANNELS;
    // adc_index is set to ADC_CHANNEL_COUNT because of the bug described in
    // NUM_READINGS_PER_ADC_DMA_TRANSFER - that is, we only use the second half
    // of adc_readings[]
    uint8_t adc_index = NUM_ADC_CHANNELS;
    for (; adc_channel < final_index; adc_channel++)
    {
        // The pseudo-code for this LPF implementation is as follows:
        // y[i] = y[i-1] + SmoothingFactor * ( x[i] - y[i-1] ), where y = 
        // filter output, x = input. That is, the change from one filter output
        // to the next is proportional to the difference between the previous 
        // output and the next input.
        filtered_adc_readings[adc_channel] =
            filtered_adc_readings[adc_channel] +
            (IIR_LPF_SMOOTHING_FACTOR *
             (adc_readings[adc_index] - filtered_adc_readings[adc_channel]));
        adc_index++;
    }
}

void CurrentSense_ConvertFilteredADCToCurrentValues(
    volatile float32_t *converted_readings)
{
    // Shift index depending on DSEL state
    uint8_t adc_channel = CurrentSense_DSELShiftIndex();
    uint8_t final_index = adc_channel + NUM_PROFET2S;
    for (; adc_channel < final_index; adc_channel++)
    {
        converted_readings[adc_channel] = filtered_adc_readings[adc_channel] *
                                          VOLTAGE_TO_CURRENT[adc_channel] *
                                          VDDA_VOLTAGE / adc_max_value;
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
SenseChannel_Enum CurrentSense_GetCurrentSenseChannel(void)
{
    return sense_channel;
}

void CurrentSense_SelectCurrentSenseChannel(SenseChannel_Enum channel)
{
    DselState_Enum dsel_value;

    // This is the diagnostic truth table for BTS7008-2EPA:
    // 
    // DEN  | DSEL           | IS
    // =====|================|================
    // low  | not relevant   | Z
    // high | low            | SENSE output 0 
    // high | high           | SENSE output 1

    if (channel == SENSE_0)
    {
        dsel_value = DSEL_LOW;
    }
    else
    {
        dsel_value = DSEL_HIGH;
    }

    SharedGpio_GPIO_WritePin(
        EFUSE_DSEL_1_GPIO_Port, EFUSE_DSEL_1_Pin, dsel_value);
    SharedGpio_GPIO_WritePin(
        EFUSE_DSEL_2_GPIO_Port, EFUSE_DSEL_2_Pin, dsel_value);
    SharedGpio_GPIO_WritePin(
        EFUSE_DSEL_3_GPIO_Port, EFUSE_DSEL_3_Pin, dsel_value);
    SharedGpio_GPIO_WritePin(
        EFUSE_DSEL_4_GPIO_Port, EFUSE_DSEL_4_Pin, dsel_value);
    SharedGpio_GPIO_WritePin(
        EFUSE_DSEL_5_GPIO_Port, EFUSE_DSEL_5_Pin, dsel_value);
}
