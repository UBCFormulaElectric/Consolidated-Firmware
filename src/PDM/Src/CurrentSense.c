#include "CurrentSense.h"


/**
 *    @brief      Helper function to shift starting index in any ADC/Converted Readings array, depending on DSEL_State
 *					DSEL_LOW corresponds to the first half of the array
 *					DSEL_HIGH corresponds to the second half of the array
 *	  @param 		None
 * 	  @return 		None
 * 
*/
static uint8_t CurrentSense_DSELShiftIndex(void);

extern volatile GPIO_PinState DSEL_State;
float FilteredADCReadings[ADC_CHANNEL_COUNT * NUM_CHANNELS] = {0};

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


void CurrentSense_LowPassFilterADCReadings(volatile uint32_t* ADCReadings) {
    uint8_t ADC_channel          = CurrentSense_DSELShiftIndex();
    uint8_t final_index = ADC_channel + ADC_CHANNEL_COUNT;
    uint8_t ADC_index   = ADC_CHANNEL_COUNT;
    for (; ADC_channel < final_index; ADC_channel++) {
        FilteredADCReadings[ADC_channel] = FilteredADCReadings[ADC_channel] + (LPF_ALPHA * (ADCReadings[ADC_index] - FilteredADCReadings[ADC_channel]));
        ADC_index++;
    }
}

void CurrentSense_ConvertFilteredADCToCurrentValues(volatile float* converted_readings) {
    uint8_t ADC_channel           = CurrentSense_DSELShiftIndex(); //Shift index depending on DSEL state
    uint8_t final_index = ADC_channel + ADC_EFUSE_READINGS;
    for (; ADC_channel < final_index; ADC_channel++) {
        converted_readings[ADC_channel] = FilteredADCReadings[ADC_channel] * VOLTAGE_TO_CURRENT[ADC_channel] * VDDA_VOLTAGE / ADC_12_BIT_POINTS;
    }

    converted_readings[_12V_SUPPLY_INDEX] =
    FilteredADCReadings[ADC_channel] * GLV_VOLTAGE / ADC_12_BIT_POINTS;
    ADC_channel++;
	
    converted_readings[VBAT_SUPPLY_INDEX] =
    FilteredADCReadings[ADC_channel] * VBAT_VOLTAGE / ADC_12_BIT_POINTS;
    ADC_channel++;
	
    converted_readings[VICOR_SUPPLY_INDEX] =
    FilteredADCReadings[ADC_channel] * EN2_TO_12VACC * VDDA_VOLTAGE /
    ADC_12_BIT_POINTS;
}

uint8_t CurrentSense_DSELShiftIndex(void) {
    if (DSEL_State == DSEL_LOW) {
        return 0;
    } else {
        return ADC_CHANNEL_COUNT;
    }
}
