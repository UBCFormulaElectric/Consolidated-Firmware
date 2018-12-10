/******************************************************************************
* Includes
*******************************************************************************/
#include "CurrentSense.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
extern volatile GPIO_PinState dsel_state;
static float filtered_adc_readings[NUM_ADC_CHANNELS * NUM_EFUSES_PER_PROFET2] = {0};

// LPF constants (calculated using this article:
// https://en.wikipedia.org/wiki/Low-pass_filter#Discrete-time_realization)
static const float ADC_TRIGGER_FREQUENCY =
5000.0f; // ADC sampling frequency - 72MHz/14400 (TIM2 prescaler value)
static const float DELTA            = 1.0f / ADC_TRIGGER_FREQUENCY;
static const float CUTOFF_FREQUENCY = 10.0f; // 10Hz cutoff to account for false
                                             // tripping from inrush - see
                                             // SoftwareTools for data
static const float RC        = 1.0f / (2.0f * 3.14159265f * CUTOFF_FREQUENCY);
static const float LPF_ALPHA = DELTA / (RC + DELTA);


/******************************************************************************
* Private Function Prototypes
*******************************************************************************/
/**
 * @brief  Helper function to shift starting index in any ADC/Converted Readings 
 *         array, depending on dsel_state.
 *         DSEL_LOW corresponds to the first half of the array
 *         DSEL_HIGH corresponds to the second half of the array
 */
static uint8_t CurrentSense_DSELShiftIndex(void);

/******************************************************************************
* Private Function Definitions
*******************************************************************************/
static uint8_t CurrentSense_DSELShiftIndex(void) {
    if (dsel_state == DSEL_LOW) {
        return 0;
    } else {
        return NUM_ADC_CHANNELS;
    }
}

/******************************************************************************
* Function Definitions
*******************************************************************************/
void CurrentSense_LowPassFilterADCReadings(volatile uint32_t* adc_readings) {
    uint8_t adc_channel          = CurrentSense_DSELShiftIndex();
    uint8_t final_index = adc_channel + NUM_ADC_CHANNELS;
    uint8_t adc_index   = NUM_ADC_CHANNELS;
    for (; adc_channel < final_index; adc_channel++) {
        filtered_adc_readings[adc_channel] = filtered_adc_readings[adc_channel] + (LPF_ALPHA * (adc_readings[adc_index] - filtered_adc_readings[adc_channel]));
        adc_index++;
    }
}

void CurrentSense_ConvertFilteredADCToCurrentValues(volatile float* converted_readings) {
    uint8_t adc_channel           = CurrentSense_DSELShiftIndex(); //Shift index depending on DSEL state
    uint8_t final_index = adc_channel + NUM_PROFET2S;
    for (; adc_channel < final_index; adc_channel++) {
        converted_readings[adc_channel] = filtered_adc_readings[adc_channel] * VOLTAGE_TO_CURRENT[adc_channel] * VDDA_VOLTAGE / ADC_12_BIT_POINTS;
    }

    converted_readings[_12V_SUPPLY] =
    filtered_adc_readings[adc_channel] * GLV_VOLTAGE / ADC_12_BIT_POINTS;
    adc_channel++;

    converted_readings[VBAT_SUPPLY] =
    filtered_adc_readings[adc_channel] * VBAT_VOLTAGE / ADC_12_BIT_POINTS;
    adc_channel++;

    converted_readings[FLYWIRE] =
    filtered_adc_readings[adc_channel] * EN2_TO_12VACC * VDDA_VOLTAGE /
    ADC_12_BIT_POINTS;
}

