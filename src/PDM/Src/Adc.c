/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Adc.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
volatile uint32_t adc_readings[NUM_ADC_CHANNELS];

/******************************************************************************
 * Private Function Prototypes
 ******************************************************************************/

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/

/******************************************************************************
 * Function Definitions
 ******************************************************************************/

void Adc_StartAdcInDmaMode(void)
{
    // Start DMA - send ADC1 values, store in adc_readings
    // DMA writes into adc_readings in circular mode - every set of readings
    // continuously overwrites the oldest set of readings
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_readings, NUM_ADC_CHANNELS);
    HAL_ADC_Start(&hadc1);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    CurrentSense_LowPassFilterADCReadings(adc_readings);
    CurrentSense_ConvertFilteredADCToCurrentValues(converted_readings);

    // Check for overcurrent
    // Change e-fuse DSELs after conversion of all sets
    if (dsel_state == DSEL_LOW)
    {
        dsel_state = DSEL_HIGH;
    }
    else
    {
        // Only handle faults after all e-fuses are checked
        FaultHandling_Handler(e_fuse_fault_states, converted_readings);
        dsel_state = DSEL_LOW;
    }

    HAL_ADC_Start(&hadc1);
    GPIO_EFuseSelectDSEL(dsel_state);
}
