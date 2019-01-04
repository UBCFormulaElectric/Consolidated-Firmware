/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Adc.h"
#include "Constants.h"
#include "SharedAdc.h"
#include "Gpio.h"
#include "CurrentSense.h"
#include "VoltageSense.h"
#include "SharedFilters.h"

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
    // Note: This callback is used for "ADC conversion by interruption" as well 
    //       as "ADC conversion with transfer by DMA"

    CurrentSense_ConvertCurrentAdcReadings();

    VoltageSense_ConvertVoltageReadings();

    // Toggle the SENSE channel between DMA tranfers to read both SENSE channnels
    CurrentSense_ToggleCurrentSenseChannel();

    // Only handle faults after all e-fuses are checked
    if (CurrentSense_GetCurrentSenseChannel() == SENSE_1)
    {
        FaultHandling_Handler(e_fuse_fault_states, converted_readings);
    }

}
