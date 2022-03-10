
#include "Io_AdcDac.h"
#include "configs/App_ControlSystemConfig.h"
#include <stdlib.h>
#include "main.h"
#include "App_SharedErrorTable.h"

extern ADC_HandleTypeDef hadc1, hadc2;
extern DAC_HandleTypeDef hdac;

static uint16_t adc1_data[ADC1_NUM_CONVERSIONS];
static uint16_t adc2_data[ADC2_NUM_CONVERSIONS];
static uint16_t adc_offset_cal_data[ADC2_NUM_CONVERSIONS][ADC_OFFSET_CAL_CYCLES];
static uint32_t adc_offset_cal_sum[ADC2_NUM_CONVERSIONS] = {0,0,0,0};

struct PhaseValues ph_cur_adc_offset_v = {
        .a = 0,
        .b = 0,
        .c = 0,
};

struct PhaseValues ph_cur_adc_offset_amps = {
        .a = 0,
        .b = 0,
        .c = 0,
};


bool phase_cur_offset_complete = 0;

static float        powerstage_temp;
static float        motor_temperature;

// Sample all ADCs (1 and 2) in regular scanning conversion mode
void Io_AdcDac_AdcContModeInit(void)
{
    HAL_ADC_DeInit(&hadc1);
    HAL_ADC_DeInit(&hadc2);

    hadc1.Instance                   = ADC1;
    hadc1.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode          = ADC_SCAN_ENABLE;
    hadc1.Init.ContinuousConvMode    = ENABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion       = 3;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.EOCSelection          = ADC_EOC_SEQ_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    hadc2.Instance                   = ADC2;
    hadc2.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc2.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc2.Init.ScanConvMode          = ADC_SCAN_ENABLE;
    hadc2.Init.ContinuousConvMode    = ENABLE;
    hadc2.Init.DiscontinuousConvMode = DISABLE;
    hadc2.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc2.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    hadc2.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc2.Init.NbrOfConversion       = ADC2_NUM_CONVERSIONS;
    hadc2.Init.DMAContinuousRequests = ENABLE;
    hadc2.Init.EOCSelection          = ADC_EOC_SEQ_CONV;
    if (HAL_ADC_Init(&hadc2) != HAL_OK)
    {
        Error_Handler();
    }
}

void Io_AdcDac_AdcPwmSyncModeInit(void)
{
    HAL_ADC_DeInit(&hadc1);
    HAL_ADC_DeInit(&hadc2);

    hadc1.Instance                   = ADC1;
    hadc1.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode          = ADC_SCAN_ENABLE;
    hadc1.Init.ContinuousConvMode    = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc1.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T8_TRGO2;
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion       = ADC1_NUM_CONVERSIONS;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.EOCSelection          = ADC_EOC_SEQ_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        // TODO perhaps exitcode here
        // Error_Handler();
    }

    hadc2.Instance                   = ADC2;
    hadc2.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc2.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc2.Init.ScanConvMode          = ADC_SCAN_ENABLE;
    hadc2.Init.ContinuousConvMode    = DISABLE;
    hadc2.Init.DiscontinuousConvMode = DISABLE;
    hadc2.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc2.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T8_TRGO2;
    hadc2.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc2.Init.NbrOfConversion       = ADC2_NUM_CONVERSIONS;
    hadc2.Init.DMAContinuousRequests = ENABLE;
    hadc2.Init.EOCSelection          = ADC_EOC_SEQ_CONV;
    if (HAL_ADC_Init(&hadc2) != HAL_OK)
    {
        // TODO perhaps exitcode here
        // Error_Handler();
    }
}

void Io_AdcDac_AdcStart(void)
{
    if (HAL_ADC_Start_DMA(
            &hadc1, (uint32_t *)adc1_data, ADC1_NUM_CONVERSIONS) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_ADC_Start_DMA(
            &hadc2, (uint32_t *)adc2_data, ADC2_NUM_CONVERSIONS) != HAL_OK)
    {
        Error_Handler();
    }
}

void Io_AdcDac_AdcStop(void)
{
    HAL_ADC_Stop_DMA(&hadc1);
    HAL_ADC_Stop_DMA(&hadc2);
}

float Io_AdcDac_GetPowerstageTemp(void)
{
    float adc_voltage = 3.3f * (float)adc1_data[0] / 4096.0f;

    float therm_resistance;
    if (adc_voltage == 3.0f)
    {
        therm_resistance = -8660.0f * adc_voltage / (adc_voltage + 0.0001f - 3);
    }
    else
    {
        therm_resistance = -8660.0f * adc_voltage / (adc_voltage - 3);
    }
    powerstage_temp =
        1 / (logf(therm_resistance / 5000.0f) / 3380.0f + 1 / 298.15f) -
        273.15f;
    return powerstage_temp;
}

float Io_AdcDac_ReadPowerstageTemp(void)
{
    return powerstage_temp;
}

float Io_AdcDac_GetMotorTemp(void)
{
    float adc_voltage = 3.3f * (float)adc1_data[1] / 4096.0f;

    motor_temperature = -28.982f * (adc_voltage * adc_voltage) +
                        223.62f * adc_voltage - 123.67f;
    return motor_temperature;
}

float Io_AdcDac_ReadMotorTemp(void)
{
    return motor_temperature;
}

float Io_AdcDac_GetBusVoltage(void)
{
    float adc_voltage = 3.3f * (float)adc2_data[3] / 4096.0f;
    float bus_voltage = ((8.06f + 499 * 4) / 8.06f) * adc_voltage;
    return bus_voltage;
}

float Io_AdcDac_GetGpioVal(void)
{
    float adc_voltage = 3.3f * (float)adc1_data[2] / 4096.0f;
    return adc_voltage;
}

void Io_AdcDac_CorrectPhaseCurOffset(const uint32_t adc_startup_cycles)
{
    if(adc_startup_cycles < ADC_OFFSET_CAL_CYCLES)
    {
        adc_offset_cal_data[0][adc_startup_cycles] = adc2_data[0];
        adc_offset_cal_data[1][adc_startup_cycles] = adc2_data[1];
        adc_offset_cal_data[2][adc_startup_cycles] = adc2_data[2];

        adc_offset_cal_sum[0] += adc2_data[0];
        adc_offset_cal_sum[1] += adc2_data[1];
        adc_offset_cal_sum[2] += adc2_data[2];
    }
    else
    {

        ph_cur_adc_offset_v.a = 3.3f*((float)adc_offset_cal_sum[0]/ADC_OFFSET_CAL_CYCLES)/4096.0f - 1.65f;
        ph_cur_adc_offset_v.b = 3.3f*((float)adc_offset_cal_sum[1]/ADC_OFFSET_CAL_CYCLES)/4096.0f - 1.65f;
        ph_cur_adc_offset_v.c = 3.3f*((float)adc_offset_cal_sum[2]/ADC_OFFSET_CAL_CYCLES)/4096.0f - 1.65f;

        phase_cur_offset_complete = true;

        if (fabsf(ph_cur_adc_offset_v.a) > MAX_CUR_ADC_OFFSET ||
            fabsf(ph_cur_adc_offset_v.b) > MAX_CUR_ADC_OFFSET ||
            fabsf(ph_cur_adc_offset_v.c) > MAX_CUR_ADC_OFFSET)
        {
            // TODO set error table error here, measured offset is too large!
        }
    }
}

struct PhaseValues Io_AdcDac_GetPhaseCurOffsets(void)
{
    ph_cur_adc_offset_amps.a = ph_cur_adc_offset_v.a * CUR_SNS_GAIN;
    ph_cur_adc_offset_amps.b = ph_cur_adc_offset_v.b * CUR_SNS_GAIN;
    ph_cur_adc_offset_amps.c = ph_cur_adc_offset_v.c * CUR_SNS_GAIN;

    return ph_cur_adc_offset_amps;
}

bool Io_AdcDac_PhaseCurOffsetComplete(void)
{
    if (phase_cur_offset_complete)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Io_AdcDac_GetPhaseCurrents(struct PhaseValues *const phase_currents)
{
    float pha_cur_adcvoltage = 3.3f * (float)adc2_data[0] / 4096.0f;
    float phb_cur_adcvoltage = 3.3f * (float)adc2_data[1] / 4096.0f;
    float phc_cur_adcvoltage = 3.3f * (float)adc2_data[2] / 4096.0f;

    phase_currents->a =
        (pha_cur_adcvoltage - 1.65f - ph_cur_adc_offset_v.a) * CUR_SNS_GAIN;
    phase_currents->b =
        (phb_cur_adcvoltage - 1.65f - ph_cur_adc_offset_v.b) * CUR_SNS_GAIN;
    phase_currents->c =
        (phc_cur_adcvoltage - 1.65f - ph_cur_adc_offset_v.c) * CUR_SNS_GAIN;
}

void Io_AdcDac_DacStart(void)
{
    if (HAL_DAC_Start(&hdac, DAC_CHANNEL_1) != HAL_OK)
    {
        // TODO exitcode here
    }
    if (HAL_DAC_Start(&hdac, DAC_CHANNEL_2) != HAL_OK)
    {
        // TODO exitcode here
    }

    // Initialize DAC values to 3.3V and 0V to avoid fault latching.
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0xFFF);
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0x000);
}

void Io_AdcDac_DacSetCurrentLim(const float current)
{
    if (current > MAX_INVERTER_CURRENT || current < 0)
    {
        // TODO exitcode error here
    }

    // Current sensors are ratiometric with 10mV/A with 1v65 offset
    float    dac_high_output_voltage = (current * 0.01f) + 1.65f;
    float    dac_low_output_voltage  = 3.3f - dac_high_output_voltage;
    uint32_t high_dac_value =
        (uint32_t)((dac_high_output_voltage * (0xFFF + 1)) / 3.3f);
    uint32_t low_dac_value =
        (uint32_t)((dac_low_output_voltage * (0xFFF + 1)) / 3.3f);

    if (HAL_DAC_SetValue(
            &hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, high_dac_value) != HAL_OK)
    {
        // TODO exitcode here
    }
    if (HAL_DAC_SetValue(
            &hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, low_dac_value) != HAL_OK)
    {
        // TODO exitcode here
    }
}

float Io_AdcDac_Dac1GetVoltage(void)
{
    uint32_t dac_raw_value = HAL_DAC_GetValue(&hdac, DAC_CHANNEL_1);
    return 3.3f * (float)dac_raw_value / (float)0xFFF;
}

float Io_AdcDac_Dac2GetVoltage(void)
{
    uint32_t dac_raw_value = HAL_DAC_GetValue(&hdac, DAC_CHANNEL_2);
    return 3.3f * (float)dac_raw_value / (float)0xFFF;
}

float Io_AdcDac_Dac1GetCurrentLim(void)
{
    float dac_voltage = Io_AdcDac_Dac1GetVoltage();
    return (dac_voltage - 1.65f) * 100.0f;
}

float Io_AdcDac_Dac2GetCurrentLim(void)
{
    float dac_voltage = Io_AdcDac_Dac2GetVoltage();
    return (1.65f - dac_voltage) * -100.0f;
}
