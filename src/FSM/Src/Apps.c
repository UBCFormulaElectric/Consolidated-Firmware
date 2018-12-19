/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Apps.h"
#include "CanDefinitions.h"
#include "stdbool.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/
#define MAX_10_BIT_POINTS 1023

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
// Used to send error CAN messages and track faults
volatile uint32_t apps_fault_state = FSM_APPS_NORMAL_OPERATION;

/******************************************************************************
* Private Function Prototypes
*******************************************************************************/

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
uint16_t getAcceleratorPedalPosition(APPS_Mode_Enum Mode)
{
    static uint32_t apps_fault_counter = 0; 
    float32_t raw_primary_apps_value       = 0.0;
    float32_t raw_secondary_apps_value     = 0.0;
    float32_t percent_primary_apps_value   = 0.0;
    float32_t percent_secdonary_apps_value = 0.0;

    uint32_t temporary_apps_fault_counter = 0;
    bool fault_flag                       = false;
    uint16_t accelerator_pedal_position   = 0;

    // Save apps_fault_counter in Mode = APPS_NORMAL_MODE
    if (Mode == APPS_NORMAL_MODE)
    {
        temporary_apps_fault_counter = apps_fault_counter;
    }

    if (apps_fault_state == FSM_APPS_PRIMARY_SECONDARY_DIFFERENCE_ERROR)
    {
        accelerator_pedal_position = 0;
    }
    else
    {
        // Read latest APPS position values, from timer counter.
        raw_primary_apps_value   = (float32_t)TIM2->CNT;
        raw_secondary_apps_value = 65535 - (float32_t)TIM3->CNT;

        // Set a deadzone to handle underflow
        if (((65535 - raw_primary_apps_value) < PRIMARY_APPS_DEADZONE) ||
            (raw_primary_apps_value < PRIMARY_APPS_DEADZONE))
        {
            raw_primary_apps_value = 0.0;
        }

        if (((65535 - raw_primary_apps_value) < SECONDARY_APPS_DEADZONE) ||
            (raw_primary_apps_value < SECONDARY_APPS_DEADZONE))
        {
            raw_secondary_apps_value = 0.0;
        }

        // Calculate ratio between APPS position readings and calibrated maximum
        // values
        percent_primary_apps_value = raw_primary_apps_value / PRIMARY_APPS_MAX_VALUE;
        percent_secdonary_apps_value =
            raw_secondary_apps_value / SECONDARY_APPS_MAX_VALUE;

        // Set torque request to zero whenever brake pedal is pushed
        // This satisfies shutting off the motors whenever the brake pedal
        // harness is disconnected (EV2.4.4) Pins are defined as User Labels in
        // cube.
        if (HAL_GPIO_ReadPin(BSPD_BRAKE_THRES_GPIO_Port, BSPD_BRAKE_THRES_Pin))
        {
            accelerator_pedal_position = 0;
        }
        // Check for position readings outside bounds (when pedal is released OR
        // when pedal is pushed passed max rotation beyond calibrated pedal box)
        else if (
            percent_primary_apps_value > 1.0 || percent_secdonary_apps_value > 1.0)
        {
            // Set pedal position to zero when encoder underflows OR when pedal
            // pushes passed max. pedal box rotation
            accelerator_pedal_position = 0;
        }
        else if (
            percent_primary_apps_value > PEDAL_SATURATION_POINT &&
            percent_primary_apps_value <= 1.0)
        {
            // Saturate pedal position to 100% if close to being fully pressed
            // (accounts for deflection in pedal cluster)
            accelerator_pedal_position = MAX_10_BIT_POINTS;
        }
        else if (percent_primary_apps_value < PEDAL_RELEASE_POINT)
        {
            // Map to 0 when pedal is close to the release point
            accelerator_pedal_position = 0;
        }
        else
        {
            // Map accelerator pedal position to 10-bit value using primary APPS
            // reading
            accelerator_pedal_position = percent_primary_apps_value * MAX_10_BIT_POINTS;
        }

        // Prevent FSM_APPS_Fault_State_1 and FSM_APPS_Fault_State_2 from
        // changing the apps_fault_state when apps_fault_state is in
        // FSM_APPS_Fault_State_3 or FSM_APPS_Fault_State_4 Reason: Rules state
        // that pedal must be released back to less than 5% before car can be
        // operational (EV2.5.1)
        if (apps_fault_state != FSM_APPS_BRAKE_PEDAL_PLAUSIBILITY_ERROR &&
            apps_fault_state != FSM_APPS_MAX_TORQUE_ERROR)
        {
            // Check for improperly connected APPS encoders (open/short circuit
            // of APPS lines)
            if ((HAL_GPIO_ReadPin(
                     PRIMARY_APPS_ALARM_GPIO_Port, PRIMARY_APPS_ALARM_Pin) ==
                 GPIO_PIN_SET) ||
                (HAL_GPIO_ReadPin(
                     SECONDARY_APPS_ALARM_GPIO_Port,
                     SECONDARY_APPS_ALARM_Pin) == GPIO_PIN_SET))
            {
                // Check if implausibility occurs after 100msec
                if (apps_fault_counter > MAX_APPS_FAULTS)
                {
                    apps_fault_state =
                        FSM_APPS_PRIMARY_SECONDARY_DIFFERENCE_ERROR;
                }
                apps_fault_counter++;
                fault_flag = true;
            }

            // Check for implausibility between the primary and secondary APPS
            // position readings (>10% difference)
            if (((percent_primary_apps_value - percent_secdonary_apps_value) > 0.1) ||
                ((percent_secdonary_apps_value - percent_primary_apps_value) > 0.1))
            {
                // Check if implausibility occurs after 100msec
                if (apps_fault_counter > MAX_APPS_FAULTS)
                {
                    apps_fault_state = FSM_APPS_BRAKE_PEDAL_PLAUSIBILITY_ERROR;
                }
                apps_fault_counter++;
                fault_flag = true;
            }
        }

        // Check for APPS/Brake Pedal Plausibility Check, where accelerator
        // pedal is pushed >25% while brake is pushed (EV2.5)
        if ((percent_primary_apps_value > APPS_BPPC_THRESHOLD) &&
            HAL_GPIO_ReadPin(BSPD_BRAKE_THRES_GPIO_Port, BSPD_BRAKE_THRES_Pin))
        {
            // Check if implausibility occurs after 100msec
            if (apps_fault_counter > MAX_APPS_FAULTS)
            {
                apps_fault_state = FSM_APPS_BRAKE_PEDAL_PLAUSIBILITY_ERROR;
            }
            apps_fault_counter++;
            fault_flag = true;
        }

        // Check if pedal is at max. torque request
        if (accelerator_pedal_position ==  MAX_10_BIT_POINTS)
        {
            // Check if pedal is stuck at max. torque after 10 secs
            if (apps_fault_counter > MAX_SATURATION_FAULTS)
            {
                apps_fault_state = FSM_APPS_MAX_TORQUE_ERROR;
            }
            apps_fault_counter++;
            fault_flag = true;
        }

        // Check for "APPS / Brake Pedal Plausibility Check" fault state and
        // ensure APPS returns to 5% pedal travel (EV2.5.1)
        if ((apps_fault_state == FSM_APPS_BRAKE_PEDAL_PLAUSIBILITY_ERROR) ||
            (apps_fault_state == FSM_APPS_MAX_TORQUE_ERROR))
        {
            accelerator_pedal_position = 0;
            if (percent_primary_apps_value < 0.05)
            {
                // Reset fault variables to normal operational state
                apps_fault_state   = FSM_APPS_NORMAL_OPERATION;
                apps_fault_counter = 0;
            }
        }
        else if (
            (apps_fault_state == FSM_APPS_OPEN_CIRCUIT_SHORT_CIRCUIT_ERROR ||
             apps_fault_state == FSM_APPS_PRIMARY_SECONDARY_DIFFERENCE_ERROR) &&
            (fault_flag == true))
        {
            // Set pedal position to zero if in error state AND fault flag is
            // triggered Need the fault flag check since FSM may latch onto
            // these error states
            accelerator_pedal_position = 0;
        }
        else if (fault_flag)
        {
            // Keep fault state at normal operation
            // BUT allow apps_fault_counter to increase
            apps_fault_state = FSM_APPS_NORMAL_OPERATION;
        }
        else
        {
            // Reset fault variables to normal operating state
            apps_fault_state   = FSM_APPS_NORMAL_OPERATION;
            apps_fault_counter = 0;
        }
    }

    // Reload apps_fault_counter in Mode = 0
    if (Mode == 0)
    {
        apps_fault_counter = temporary_apps_fault_counter;
    }

    return accelerator_pedal_position;
}