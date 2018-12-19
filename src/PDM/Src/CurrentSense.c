/******************************************************************************
 * Includes
 ******************************************************************************/
#include "CurrentSense.h"

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
    filtered_adc_readings[NUM_ADC_CHANNELS * NUM_EFUSES_PER_PROFET2] = {0};

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
        return ADC_CHANNEL_COUNT;
    }
}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void CurrentSense_LowPassFilterADCReadings(volatile uint32_t *adc_readings)
{
    uint8_t adc_channel = CurrentSense_DSELShiftIndex();
    uint8_t final_index = adc_channel + ADC_CHANNEL_COUNT;
    // adc_index is set to ADC_CHANNEL_COUNT because of the bug described in
    // NUM_READINGS_PER_ADC_DMA_TRANSFER - that is, we only use the second half
    // of adc_readings[]
    uint8_t adc_index = ADC_CHANNEL_COUNT;
    for (; adc_channel < final_index; adc_channel++)
    {
        filtered_adc_readings[adc_channel] =
            filtered_adc_readings[adc_channel] +
            (LPF_ALPHA *
             (adc_readings[adc_index] - filtered_adc_readings[adc_channel]));
        adc_index++;
    }
}

void CurrentSense_ConvertFilteredADCToCurrentValues(
    volatile float32_t *converted_readings)
{
    uint8_t adc_channel =
        CurrentSense_DSELShiftIndex(); // Shift index depending on DSEL state
    uint8_t final_index = adc_channel + ADC_EFUSE_READINGS;
    for (; adc_channel < final_index; adc_channel++)
    {
        converted_readings[adc_channel] = filtered_adc_readings[adc_channel] *
                                          VOLTAGE_TO_CURRENT[adc_channel] *
                                          VDDA_VOLTAGE / ADC_12_BIT_POINTS;
    }

    converted_readings[_12V_SUPPLY_INDEX] =
        filtered_adc_readings[adc_channel] * GLV_VOLTAGE / ADC_12_BIT_POINTS;
    adc_channel++;

    converted_readings[VBAT_SUPPLY_INDEX] =
        filtered_adc_readings[adc_channel] * VBAT_VOLTAGE / ADC_12_BIT_POINTS;
    adc_channel++;

    converted_readings[FLYWIRE] = filtered_adc_readings[adc_channel] *
                                  ADC1_IN10_TO_12V_ACC_RATIO * VDDA_VOLTAGE /
                                  ADC_12_BIT_POINTS;
}
