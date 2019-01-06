/******************************************************************************
 * Includes
 ******************************************************************************/
#include "FaultHandling.h"
#include "Gpio.h"
#include "SharedGpio.h"
#include "SharedCan.h"
#include "SharedAdc.h"

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
volatile uint8_t num_faults[NUM_UNIQUE_ADC_READINGS] = { 0 };
volatile uint8_t e_fuse_fault_states
    [NUM_ADC_CHANNELS * NUM_READINGS_PER_ADC_DMA_TRANSFER] = { NORMAL_STATE };

/******************************************************************************
 * Private Function Prototypes
 ******************************************************************************/


/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/



/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void FaultHandling_Handler(
    volatile uint8_t *  fault_states,
    volatile float32_t *converted_readings)
{
    // TODO: Fix this after currentsense and voltage sense are solid
    /*
    uint64_t can_error_msg;

    for (ADC_Index_Enum adc_channel = 0; adc_channel < NUM_UNIQUE_ADC_READINGS;
         adc_channel++)
    {
        // This function only handles efuse errors, not voltage sense errors
        if (adc_channel == _12V_SUPPLY || adc_channel == VBAT_SUPPLY ||
            adc_channel == FLYWIRE)
        {
            continue;
        }

        // If the efuse is not in RETRY or ERROR mode and the current reading is
        // over the limit, disable efuse
        if (fault_states[adc_channel] == NORMAL_STATE &&
            converted_readings[adc_channel] >= EFUSE_CURRENT_LIMIT)
        {
            num_faults[adc_channel]++;
            FaultHandling_ConfigureEfuseOnOff(adc_channel, EFUSE_OFF);

            // Has faulted fewer than max number of times
            if (num_faults[adc_channel] <= MAX_FAULTS[adc_channel])
            {
                fault_states[adc_channel] = RETRY_STATE;
            }
            else
            {
                // Special handling for specific outputs
                if (adc_channel == RIGHT_INVERTER ||
                    adc_channel == LEFT_INVERTER || adc_channel == CAN_GLV ||
                    adc_channel == AIR_SHDN)
                {
                    fault_states[RIGHT_INVERTER] = ERROR_STATE;
                    fault_states[LEFT_INVERTER]  = ERROR_STATE;
                    FaultHandling_ConfigureEfuseOnOff(LEFT_INVERTER, EFUSE_OFF);
                    FaultHandling_ConfigureEfuseOnOff(
                        RIGHT_INVERTER, EFUSE_OFF);

                    Can_BroadcastMotorShutdownError();
                }
                else if (adc_channel == COOLING)
                {
                    Can_BroadcastMotorShutdownError();
                }
                fault_states[adc_channel] = ERROR_STATE;

                // TODO  (Issue #191): CAN_GLV message implementation
                can_error_msg =
                    (adc_channel << 16) +
                    (uint16_t)(
                        converted_readings[adc_channel] * adc_max_value /
                        (VOLTAGE_TO_CURRENT[adc_channel] * VDDA_VOLTAGE));
                SharedCan_BroadcastPcbErrors(EFUSE_FAULT);
            }
        }
    }

    if (converted_readings[_12V_SUPPLY] < UNDERVOLTAGE_GLV_THRES)
    {
        SharedCan_BroadcastPcbErrors(_12V_FAULT_UV);
        num_faults[_12V_SUPPLY]++;
    }
    else if (converted_readings[_12V_SUPPLY] > OVERVOLTAGE_GLV_THRES)
    {
        SharedCan_BroadcastPcbErrors(_12V_FAULT_OV);
        num_faults[_12V_SUPPLY]++;
    }

    if (converted_readings[VBAT_SUPPLY] > VBAT_OVERVOLTAGE)
    {
        SharedCan_BroadcastPcbErrors(VBAT_FAULT);
        num_faults[VBAT_SUPPLY]++;
    }

    if (converted_readings[FLYWIRE] > UNDERVOLTAGE_VICOR_THRES)
    {
        GPIO_ConfigureFor12VAcc(fault_states);
    }
    else
    {
        GPIO_ConfigureFor12VAux(fault_states);
    }
    */
}

void FaultHandling_RetryEFuse(volatile uint8_t *fault_states)
{
    // TODO: Fix this after currentsense and voltage sense are solid
    /*
    for (uint8_t adc_channel = 0; adc_channel < NUM_UNIQUE_ADC_READINGS;
         adc_channel++)
    {
        if (adc_channel == _12V_SUPPLY || adc_channel == VBAT_SUPPLY ||
            adc_channel == FLYWIRE)
            continue;

        if (fault_states[adc_channel] == RETRY_STATE)
        {
            FaultHandling_ConfigureEfuseOnOff(adc_channel, EFUSE_ON);
            fault_states[adc_channel] = NORMAL_STATE;
        }
        else if (fault_states[adc_channel] == ERROR_STATE)
        {
            FaultHandling_ConfigureEfuseOnOff(adc_channel, EFUSE_OFF);
        }
    }
    */
}
