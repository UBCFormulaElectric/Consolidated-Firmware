/******************************************************************************
 * Includes
 ******************************************************************************/
#include "ADC.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/
// clang-format off

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/
// clang-format on

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
	
/******************************************************************************
* Private Function Prototypes
*******************************************************************************/

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void FilterADCReadings(uint32_t ChannelCount, uint32_t FilterSize, volatile uint16_t *ADCReadings, float *FilteredADCReadings)
{
	int i;
	int j;

	// Reset filtered ADC readings array to 0
	for(i = 0; i < ChannelCount; i++)
	{
		FilteredADCReadings[i] = 0;
	}

	// Calculate sum of previous ADC_FILTER_SIZE readings
	for(i = 0; i < FilterSize; i++)
	{
		for(j = 0; j < ChannelCount; j++)
		{
			FilteredADCReadings[j] += ADCReadings[(ChannelCount * i) + j];
		}
	}

	// Calculate average of previous ADC_FILTER_SIZE readings
	for(i = 0; i < ChannelCount; i++)
	{
		FilteredADCReadings[i] = FilteredADCReadings[i] / FilterSize;
	}
}