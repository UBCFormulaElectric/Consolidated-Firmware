/******************************************************************************
 * Includes
 ******************************************************************************/
#include "FaultHandling.h"
#include "Gpio.h"
#include "SharedGpio.h"

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
volatile uint8_t num_faults[NUM_ADC_CHANNELS * NUM_EFUSES_PER_PROFET2] = {0};

/******************************************************************************
 * Private Function Prototypes
 ******************************************************************************/
/**
 * @brief  Helper function to turn e-fuse on or off
 * @param  index Index of e-fuse
 * @param  state Turn e-fuse on or off
 */
static void FaultHandling_ConfigureEfuseOnOff(
    uint8_t                  index,
    EfuseOnOff_GPIO_PinState state);

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/
static void FaultHandling_ConfigureEfuseOnOff(
    uint8_t                  index,
    EfuseOnOff_GPIO_PinState state)
{
    if (index < NUM_ADC_CHANNELS)
    {
        SharedGpio_GPIO_WritePin(
            PROFET2_IN0.port[index], PROFET2_IN0.pin[index], state);
    }
    else
    {
        index = index - NUM_ADC_CHANNELS; // adjust index for pinout array
        SharedGpio_GPIO_WritePin(
            PROFET2_IN1.port[index], PROFET2_IN1.pin[index], state);
    }
}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void FaultHandling_Handler(
    volatile uint8_t *  fault_states,
    volatile float32_t *converted_readings)
{
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

                    TransmitCANError(
                        MOTOR_SHUTDOWN_ERROR, Power_Distribution_Module, 0, 0);
                }
                else if (adc_channel == COOLING)
                {
                    TransmitCANError(
                        MOTOR_SHUTDOWN_ERROR, Power_Distribution_Module, 0, 0);
                }
                fault_states[adc_channel] = ERROR_STATE;

                // TODO  (Issue #191): CAN_GLV message implementation
                can_error_msg =
                    (adc_channel << 16) +
                    (uint16_t)(
                        converted_readings[adc_channel] * ADC_12_BIT_POINTS /
                        (VOLTAGE_TO_CURRENT[adc_channel] * VDDA_VOLTAGE));
                TransmitCANError(
                    PDM_ERROR, Power_Distribution_Module, EFUSE_FAULT,
                    can_error_msg);
            }
        }
    }

    if (converted_readings[_12V_SUPPLY] < UNDERVOLTAGE_GLV_THRES)
    {
        TransmitCANError(
            PDM_ERROR, Power_Distribution_Module, _12V_FAULT_UV, can_error_msg);
        num_faults[_12V_SUPPLY]++;
    }
    else if (converted_readings[_12V_SUPPLY] > OVERVOLTAGE_GLV_THRES)
    {
        TransmitCANError(
            PDM_ERROR, Power_Distribution_Module, _12V_FAULT_OV, can_error_msg);
        num_faults[_12V_SUPPLY]++;
    }

    if (converted_readings[VBAT_SUPPLY] > VBAT_OVERVOLTAGE)
    {
        TransmitCANError(
            PDM_ERROR, Power_Distribution_Module, VBAT_FAULT, can_error_msg);
        num_faults[VBAT_SUPPLY]++;
    }

    if (converted_readings[FLYWIRE] > UNDERVOLTAGE_VICOR_THRES)
    {
        GPIO_ConfigurePreChargeComplete(fault_states);
    }
    else
    {
        GPIO_ConfigurePowerUp(fault_states);
    }
}

void FaultHandling_RetryEFuse(volatile uint8_t *fault_states)
{
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
}
