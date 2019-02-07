/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Adc.h"
#include "Constants.h"
#include "SharedAdc.h"
#include "Gpio.h"
#include "CurrentSense.h"
#include "VoltageSense.h"

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

    // Save a copy of the most recent current sense channel
    SenseChannel_Enum last_sense_channel = CurrentSense_GetCurrentSenseChannel();

    // Toggle the SENSE channel between DMA tranfers to read both SENSE
    // channnels. Do this at right after the most recent current sense channel
    // is saved in memeory.
    CurrentSense_ToggleCurrentSenseChannel();

    CurrentSense_ConvertCurrentAdcReadings(last_sense_channel);

    VoltageSense_ConvertVoltageAdcReadings();

}
