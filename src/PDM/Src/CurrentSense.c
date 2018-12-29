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
extern volatile GPIO_PinState dsel_state;
static float32_t
    filtered_adc_readings[NUM_ADC_CHANNELS * NUM_EFUSES_PER_PROFET2] = { 0 };

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
    if (dsel_state == DSEL_LOW)
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
