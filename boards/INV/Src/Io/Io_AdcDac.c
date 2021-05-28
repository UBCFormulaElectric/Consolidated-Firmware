
#include "Io_AdcDac.h"
#include "configs/App_ControlSystemConfig.h"
#include <stdlib.h>

uint32_t *adc_data;

ADC_HandleTypeDef *adc1_handle, *adc2_handle;
DAC_HandleTypeDef *dac_handle;

void Io_AdcDac_Init(
    ADC_HandleTypeDef *const adc1,
    ADC_HandleTypeDef *const adc2,
    DAC_HandleTypeDef *const dac)
{
    adc1_handle = adc1;
    adc2_handle = adc2;
    dac_handle  = dac;

    // allocate adc data buffer
    adc_data = malloc(
        sizeof(uint32_t) * (size_t)(
                               adc1_handle->Init.NbrOfConversion +
                               adc2_handle->Init.NbrOfConversion));
}

// Sample all ADCs (1 and 2) in regular scanning conversion mode
void Io_AdcDac_AdcContModeInit(void)
{
    if(adc1_handle == NULL || adc2_handle == NULL)
    {
        //todo exitcode here
    }
    HAL_ADC_DeInit(adc1_handle);
    HAL_ADC_DeInit(adc2_handle);

    adc1_handle->Instance                   = ADC1;
    adc1_handle->Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    adc1_handle->Init.Resolution            = ADC_RESOLUTION_12B;
    adc1_handle->Init.ScanConvMode          = ADC_SCAN_ENABLE;
    adc1_handle->Init.ContinuousConvMode    = ENABLE;
    adc1_handle->Init.DiscontinuousConvMode = DISABLE;
    adc1_handle->Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    adc1_handle->Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    adc1_handle->Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    adc1_handle->Init.NbrOfConversion       = 3;
    adc1_handle->Init.DMAContinuousRequests = ENABLE;
    adc1_handle->Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(adc1_handle) != HAL_OK)
    {
        Error_Handler();
    }

    adc2_handle->Instance                   = ADC2;
    adc2_handle->Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    adc2_handle->Init.Resolution            = ADC_RESOLUTION_12B;
    adc2_handle->Init.ScanConvMode          = ADC_SCAN_ENABLE;
    adc2_handle->Init.ContinuousConvMode    = ENABLE;
    adc2_handle->Init.DiscontinuousConvMode = DISABLE;
    adc2_handle->Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    adc2_handle->Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    adc2_handle->Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    adc2_handle->Init.NbrOfConversion       = 4;
    adc2_handle->Init.DMAContinuousRequests = ENABLE;
    adc2_handle->Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(adc2_handle) != HAL_OK)
    {
        Error_Handler();
    }
}

void Io_AdcDac_AdcPwmSyncModeInit(void)
{
    if(adc1_handle == NULL || adc2_handle == NULL)
    {
        //todo exitcode here
    }

    HAL_ADC_DeInit(adc1_handle);
    HAL_ADC_DeInit(adc2_handle);

    adc1_handle->Instance                   = ADC1;
    adc1_handle->Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    adc1_handle->Init.Resolution            = ADC_RESOLUTION_12B;
    adc1_handle->Init.ScanConvMode          = ADC_SCAN_ENABLE;
    adc1_handle->Init.ContinuousConvMode    = DISABLE;
    adc1_handle->Init.DiscontinuousConvMode = DISABLE;
    adc1_handle->Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
    adc1_handle->Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T8_TRGO2;
    adc1_handle->Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    adc1_handle->Init.NbrOfConversion       = 3;
    adc1_handle->Init.DMAContinuousRequests = ENABLE;
    adc1_handle->Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(adc1_handle) != HAL_OK)
    {
        // TODO perhaps exitcode here
        // Error_Handler();
    }

    adc2_handle->Instance                   = ADC2;
    adc2_handle->Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    adc2_handle->Init.Resolution            = ADC_RESOLUTION_12B;
    adc2_handle->Init.ScanConvMode          = ADC_SCAN_ENABLE;
    adc2_handle->Init.ContinuousConvMode    = DISABLE;
    adc2_handle->Init.DiscontinuousConvMode = DISABLE;
    adc2_handle->Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
    adc2_handle->Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T8_TRGO2;
    adc2_handle->Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    adc2_handle->Init.NbrOfConversion       = 4;
    adc2_handle->Init.DMAContinuousRequests = ENABLE;
    adc2_handle->Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(adc2_handle) != HAL_OK)
    {
        // TODO perhaps exitcode here
        // Error_Handler();
    }
}

void Io_AdcDac_AdcStart(void)
{
    if (HAL_ADC_Start_DMA(
            adc1_handle, adc_data,
            adc1_handle->Init.NbrOfConversion) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_ADC_Start_DMA(
            adc2_handle,
            adc_data + adc1_handle->Init.NbrOfConversion,
            adc2_handle->Init.NbrOfConversion) != HAL_OK)
    {
        Error_Handler();
    }
}

void Io_AdcDac_AdcStop(void)
{
    if (HAL_ADC_Stop_DMA(adc1_handle) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_ADC_Stop_DMA(adc2_handle) != HAL_OK) {
        Error_Handler();
    }
}

double Io_AdcDac_GetPowerstageTemp(void)
{
    double adc_voltage = 3.3*(double)*(adc_data + 4)/4096.0;

    //TODO this has to return temperature instead of voltage
    return adc_voltage;
}

double Io_AdcDac_GetMotorTemp(void)
{
    double adc_voltage = 3.3*(double)*(adc_data + 5)/4096.0;

    //TODO this has to return temperature instead of voltage
    return adc_voltage;
}

double Io_AdcDac_GetBusVoltage(void)
{
    double adc_voltage = 3.3*(double)*(adc_data + 3)/4096.0;
    double bus_voltage = ((8.06+499*4)/8.06)*adc_voltage;
    return bus_voltage;
}

double Io_AdcDac_GetGpioVal(void)
{
    double adc_voltage = 3.3*(double)*(adc_data + 6)/4096.0;
    return adc_voltage;
}

void Io_AdcDac_GetPhaseCurrents(PhaseValues *const phase_currents)
{
    double pha_voltage = 3.3*(double)*(adc_data + 0)/4096.0;
    double phb_voltage = 3.3*(double)*(adc_data + 1)/4096.0;
    double phc_voltage = 3.3*(double)*(adc_data + 2)/4096.0;

    phase_currents->a = (pha_voltage-1.65)*100;
    phase_currents->b = (phb_voltage-1.65)*100;
    phase_currents->c = (phc_voltage-1.65)*100;
}

void Io_AdcDac_DacStart(void)
{
    HAL_DAC_Start(dac_handle, DAC_CHANNEL_1);
    HAL_DAC_Start(dac_handle, DAC_CHANNEL_2);

    // Initialize DAC values to 3.3V and 0V to avoid fault latching.
    HAL_DAC_SetValue(dac_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0xFFF);
    HAL_DAC_SetValue(dac_handle, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0x000);
}

void Io_AdcDac_DacSetCurrent(const double current)
{
    if (current > MAX_INVERTER_CURRENT || current < 0)
    {
        // TODO exitcode error here
    }

    // Current sensors are ratiometric with 10mV/A with 1v65 offset
    double   dac_high_output_voltage = (current * 0.01) + 1.65;
    double   dac_low_output_voltage  = 3.3 - dac_high_output_voltage;
    uint32_t high_dac_value =
        (uint32_t)((dac_high_output_voltage * (0xFFF + 1)) / 3.3);
    uint32_t low_dac_value =
        (uint32_t)((dac_low_output_voltage * (0xFFF + 1)) / 3.3);

    HAL_DAC_SetValue(
        dac_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, high_dac_value);
    HAL_DAC_SetValue(dac_handle, DAC_CHANNEL_2, DAC_ALIGN_12B_R, low_dac_value);
}
