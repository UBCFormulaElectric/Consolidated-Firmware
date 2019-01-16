/**
 * @file  ADC.h
 * @brief FSM_2017 - Function library header for the Analog-Digital Converter (ADC)
 */

#ifndef ADC_H
#define ADC_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "stdint.h"
/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
// ADC1 data register address
#define ADC1_DR				 	(uint32_t)(0x40012440)		  	
// VREFINT factory calibration value
#define VREFINT_CAL			 	*(uint16_t *)(0x01FFFF7BA)	  	
// Number of ADC channels used
#define ADC_CHANNEL_COUNT	   	2							   	
// Trigger (reading) frequency from TIM1 to ADC (Hz)
#define ADC_TRIGGER_FREQUENCY   1600			
// Oversampling filter size (number of readings)
#define ADC_FILTER_SIZE		 	16							  	


/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format on

/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief  Filters ADC readings by computing the mean of the previous ADC_FILTER_SIZE readings (oversampling filter)
 * @param  ChannelCount Number of ADC channels used
 * @param  FilterSize Oversampling filter size (number of readings)
	* @param  ADCReadings Pointer to array containing the unfiltered ADC readings
	* @param  FilteredADCReadings Pointer to array in which the filtered ADC readings will be stored
 */
void FilterADCReadings(uint32_t ChannelCount, uint32_t FilterSize, volatile uint16_t *ADCReadings, float *FilteredADCReadings);

#endif /* ADC_H */
