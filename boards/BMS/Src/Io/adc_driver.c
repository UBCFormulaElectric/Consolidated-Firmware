#include <stm32f3xx.h>
#include <stm32f3xx_hal_adc.h>
#include "adc_driver.h"

#define ADC_RESOLUTION (4095.0f)
#define SINGLE_ENDED_ADC_V_SCALE (3.3f) // Single-ended ADC voltage has a range of 3.3V as we can measure [0, 3.3] V
#define DIFFERENTIAL_ADC_V_SCALE (6.6f) // Differential ADC voltage has a range of 6.6V as we can measure [-3.3, 3.3] V

// Macros used to convert digital ADC readings to an analog voltage
#define SINGLE_END_RAW_ADC_TO_V (SINGLE_ENDED_ADC_V_SCALE / ADC_RESOLUTION)
#define DIFFERENTIAL_RAW_ADC_TO_V (DIFFERENTIAL_ADC_V_SCALE / ADC_RESOLUTION)

/***************************************
 * ADC 1 SCALE AND OFFSET CONFIGURATIONS
 ***************************************/
// TS offset and scale cfgs
#define TS_VOLTAGE_R_ERROR_COMPENSATION (1.05f)
#define TS_VOLTAGE_AMPLIFIER_GAIN (8.0f)
#define TS_VOLTAGE_DIV (1.24e+3f / (499e+3f + 499e+3f + 499e+3f + 499e+3f + 1.24e+3f))
#define TS_VOLTAGE_SCALE \
    (DIFFERENTIAL_RAW_ADC_TO_V * TS_VOLTAGE_R_ERROR_COMPENSATION / (TS_VOLTAGE_DIV * TS_VOLTAGE_AMPLIFIER_GAIN))
#define TS_VOLTAGE_OFFSET_SCALED (0.0f)

/***************************************
 * ADC 2 SCALE AND OFFSET CONFIGURATIONS
 ***************************************/
// High res current sense offset and scale cfgs
#define OUTPUT_1_SENSITIVITY (1.0f / 40e-3f)      // Sensitivity of output 1: 40mV/A
#define OUTPUT_1_DIV ((1.1f + 2.155f) / (2.155f)) // Voltage divider from adc -> current sensor output
#define HIGH_RES_TS_CURRENT_OFFSET_V (2.45f)      // Offset voltage of output 1. Found to be 2.45V through testing
#define HIGH_RES_TS_CURRENT_OFFSET_SCALED (HIGH_RES_TS_CURRENT_OFFSET_V * OUTPUT_1_SENSITIVITY)
#define HIGH_RES_TS_CURRENT_SCALE (SINGLE_END_RAW_ADC_TO_V * OUTPUT_1_DIV * OUTPUT_1_SENSITIVITY)

// Low res current sense offset and scale cfgs
#define OUTPUT_2_SENSITIVITY (1.0f / 6.67e-3f)    // Sensitivity of output 2: 6.67mV/A
#define OUTPUT_2_DIV ((1.1f + 2.155f) / (2.155f)) // Voltage divider from adc --> current sensor output
#define OUTPUT_2_OFFSET_V (2.45f)                 // Offset voltage of output 2. Found to be 2.45V through testing
#define LOW_RES_TS_CURRENT_OFFSET_SCALED (OUTPUT_2_OFFSET_V * OUTPUT_2_SENSITIVITY)
#define LOW_RES_TS_CURRENT_SCALE (SINGLE_END_RAW_ADC_TO_V * OUTPUT_2_DIV * OUTPUT_2_SENSITIVITY)

// A macro that can be used to get the ADC result
#define GET_CONVERTED_ADC_VALUE(raw, scale, offset) ((raw) * (scale) + (offset))

typedef enum
{
    ADC_INSTANCE_1 = 0U,
    ADC_INSTANCE_2,
} AdcInstance_E;

// In STM32 terminology, each ADC pin corresponds to an ADC channel (See:
// ADCEx_channels). If there are multiple ADC channels being measured, the ADC
// measures them based on "rank" (See: ADCEx_regular_rank). The rank starts
// counting at 1, and the lower the rank, the higher the measurement priority.
//
// For example, suppose we are measuring ADC channel 2, 4, and 7, which have
// rank 3, 1, and 2 respectively. The ADC will measure the channel 4, then
// channel 7, and finally channel 2. This order is important because it
// determines the order in which the DMA writes data to our raw_adc2_values
// buffer.
//
// The following enum is used to index into raw_adc2_values, which means it must
// be ordered in ascending ranks. If we were writing an enum for the earlier
// example, it would look like:
//
// enum
// {
//     CHANNEL_4, // Rank 1
//     CHANNEL_7, // Rank 2
//     CHANNEL_2, // Rank 3
//     NUM_ADC_CHANNELS,
// };
typedef enum
{
    RANK_ADC1_CHANNEL_3 = 0U,
    NUM_ADC1_CHANNELS,
    RANK_ADC2_CHANNEL_1 = 0U,
    RANK_ADC2_CHANNEL_3,
    NUM_ADC2_CHANNELS,
} AdcRank_E;

typedef struct
{
    AdcInstance_E adc_instance; // Specify which ADC the channel corresponds to
    uint8_t       rank;         // Specifies the rank of the ADC channel
    float         scale;        // The scale used to convert the raw ADC result to a "real" result
    float         offset;       // An optional offset field used while converting a raw ADC result to a "real" result
} AdcConfig_S;

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim3;

// Buffers to store raw ADC values
static uint16_t raw_adc1_buffer[NUM_ADC1_CHANNELS] = { 0U };
static uint16_t raw_adc2_buffer[NUM_ADC2_CHANNELS] = { 0U };

// Buffers to store converted ADC values
static float    adc1_voltages[NUM_ADC1_CHANNELS]   = { 0U };
static float    adc2_voltages[NUM_ADC2_CHANNELS]   = { 0U };

static AdcConfig_S adc_cfg[NUM_ADC_CHANNELS] =
{
    // ADC instance 1 configuration
    [TS_VOLTAGE] =
    {
        .adc_instance = ADC_INSTANCE_1,
        .rank = RANK_ADC1_CHANNEL_3,
        .scale = TS_VOLTAGE_SCALE,
        .offset = TS_VOLTAGE_OFFSET_SCALED,
    },

    // ADC instance 2 configuration
    [HIGH_RES_TS_CURRENT_SENSE] =
    {
        .adc_instance = ADC_INSTANCE_2,
        .rank = RANK_ADC2_CHANNEL_1,
        .scale = HIGH_RES_TS_CURRENT_SCALE,
        .offset = HIGH_RES_TS_CURRENT_OFFSET_SCALED,
    },
    [LOW_RES_TS_CURRENT_SENSE] =
    {
        .adc_instance = ADC_INSTANCE_2,
        .rank = RANK_ADC2_CHANNEL_3,
        .scale = LOW_RES_TS_CURRENT_SCALE,
        .offset = LOW_RES_TS_CURRENT_OFFSET_SCALED,
    },
};

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        for (uint8_t channel = TS_VOLTAGE; channel < HIGH_RES_TS_CURRENT_SENSE; channel++)
        {
            const uint8_t adc_rank = adc_cfg[channel].rank;
            adc1_voltages[adc_rank] =
                GET_CONVERTED_ADC_VALUE(raw_adc1_buffer[channel], adc_cfg[channel].scale, adc_cfg[channel].offset);
        }
    }
    else if (hadc->Instance == ADC2)
    {
        for (uint8_t channel = HIGH_RES_TS_CURRENT_SENSE; channel < NUM_ADC_CHANNELS; channel++)
        {
            const uint8_t adc_rank = adc_cfg[channel].rank;
            adc2_voltages[adc_rank] =
                GET_CONVERTED_ADC_VALUE(raw_adc2_buffer[channel], adc_cfg[channel].scale, adc_cfg[channel].offset);
        }
    }
}

void adc_driver_init(void)
{
    // DMA will populate raw adc buffers every 1kHz (configured in timer 3)
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)raw_adc1_buffer, hadc1.Init.NbrOfConversion);
    HAL_ADC_Start_DMA(&hadc2, (uint32_t *)raw_adc2_buffer, hadc2.Init.NbrOfConversion);
    HAL_TIM_Base_Start(&htim3);
}

float adc_driver_getConvertedResult(AdcChannels_E adc_channel)
{
    float result = 0.0f;

    switch (adc_cfg[adc_channel].adc_instance)
    {
        case ADC_INSTANCE_1:
            result = adc1_voltages[adc_cfg[adc_channel].rank];
            break;

        case ADC_INSTANCE_2:
            result = adc2_voltages[adc_cfg[adc_channel].rank];
            break;

        default:
            break;
    }

    return result;
}
