/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Adc.h"
#include "Constants.h"
#include "SharedAdc.h"
#include "Gpio.h"

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

/******************************************************************************
 * Private Function Prototypes
 ******************************************************************************/

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
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

    GPIO_EFuseSelectDSEL(dsel_state);
}
