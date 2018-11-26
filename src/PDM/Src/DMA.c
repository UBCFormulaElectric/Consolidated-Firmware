#include "DMA.h"

void DMA_Init(void)
{
    // Start DMA - send ADC1 values, store in adc_readings
    // DMA writes into adc_readings in circular mode - every set of readings
    // continuously overwrites the oldest set of readings
    HAL_ADC_Start_DMA(
        &hadc1, (uint32_t *)adc_readings, ADC_CHANNEL_COUNT * NUM_READINGS);
    HAL_ADC_Start(&hadc1);

    //	HAL_DMA_RegisterCallback(&hdma_adc1, HAL_DMA_XFER_CPLT_CB_ID,
    // DMA_TransferCompleteCallback);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    CurrentSense_LowPassFilterADCReadings(adc_readings);
    CurrentSense_ConvertFilteredADCToCurrentValues(converted_readings);

    // Check for overcurrent
    // Change e-fuse DSELs after conversion of all sets
    if (DSEL_State == DSEL_LOW)
    {
        DSEL_State = DSEL_HIGH;
    }
    else
    {
        FaultHandling_Handler(
            e_fuse_fault_states,
            converted_readings); // Only handle faults after all e-fuses are
                                 // checked
        DSEL_State = DSEL_LOW;
    }

    HAL_ADC_Start(&hadc1);
    GPIO_EFuseSelectDSEL(DSEL_State);
}
