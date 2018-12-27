/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Apps.h"
#include "CanDefinitions.h"
#include "stdbool.h"
#include "Constants.h"
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
/* @brief Should be a value of FsmError_Enum with the prefix "FSM_APPS" */
volatile uint32_t apps_fault_state = FSM_APPS_NORMAL_OPERATION;

/******************************************************************************
 * Private Function Prototypes
 ******************************************************************************/
/**
 * @brief  Check for pedal travel implausibility as per 2017-18 EV2.3.6
 * @param  papps_value Encoder value for PAPPS
 *         sapps_value Encoder value for SAPPS (After being subtracted from
 *                     maximum encoder tick count)
 * @return SUCCESS No implausibility
 *         ERROR Active implausibility
 */
static ErrorStatus Apps_CheckPedalTravelImplausibility(uint32_t papps_value, uint32_t sapps_value);

/**
 * @brief  Check for any failures in APPS or APPS wiring as per 2017-18 EV2.3.9
 * @return SUCCESS No implausibility
 *         ERROR Active implausibility
 */
static ErrorStatus Apps_CheckAppsImplausibility(void);
/**
 * @brief  Perform APPS/brake pedal plausibility check as per 2017-18 EV2.5
 * @param  papps_value Encoder value for PAPPS
 * @return SUCCESS Plausibility check passed
 *         ERROR Plausibility check failed
 */
static ErrorStatus Apps_CheckPlausibility(uint32_t papps_value);

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/
static ErrorStatus Apps_CheckPedalTravelImplausibility(uint32_t papps_value, uint32_t sapps_value)
{
    float32_t percent_papps_value = papps_value / PRIMARY_APPS_MAX_VALUE;
    float32_t percent_sapps_value = sapps_value / SECONDARY_APPS_MAX_VALUE;

    // Check if the difference in pedal travel between PAPPS and SAPPS are
    // within the implausibility threshold defined in 2017-18 EV.2.3.6
    if ((percent_papps_value - percent_sapps_value > APPS_DEVIATION_THRESHOLD) ||
        (percent_sapps_value - percent_papps_value > APPS_DEVIATION_THRESHOLD))
    {
        return ERROR;
    }
    else
    {
        return SUCCESS;
    }
}

static ErrorStatus Apps_CheckAppsImplausibility(void)
{        
    // Note: Disconnecting APPS encoder will trigger a fault in MAX3097E ALARMD pin
    if (Gpio_IsPappsAlarmdActive() || Gpio_IsSappsAlarmdActive())
    {
        return ERROR;
    }
    else
    {
        return SUCCESS;
    }
}

static ErrorStatus Apps_CheckPlausibility(uint32_t papps_value)
{
    // Check if the mechanical brakes are actuvated and the APPS signals more 
    // than 25% pedal travel at the same time as per 2017-18 EV2.5
    if (papps_value > APPS_PLAUSIBILITY_THRESHOLD && Gpio_IsBrakePressed())
    {
        return ERROR;
    }
    else
    {
        return SUCCESS;
    }
}


/******************************************************************************
 * Function Definitions
 ******************************************************************************/
uint32_t Apps_GetAcceleratorPedalPosition(void)
{
    static uint32_t apps_fault_counter         = 0;
    uint32_t        papps_value                = 0;
    uint32_t        sapps_value                = 0;
    bool            fault_flag                 = false;
    uint32_t        accelerator_pedal_position = 0;

    // Read the APPS position values from TIM2 and TIM3 (Note: PAPPS and
    // SAPPS are oriented in opposite orientation. This means when PAPPS
    // timer is counting up, the SAPPS timer is counting down.)
    papps_value = __HAL_TIM_GET_COUNTER(&htim2);
    // TODO: Change MAX_16_BITS_VALUE to cube user constant
    sapps_value = MAX_16_BITS_VALUE - __HAL_TIM_GET_COUNTER(&htim3);

    if (Gpio_IsBrakePressed() || papps_value > PRIMARY_APPS_MAX_VALUE ||
        sapps_value > SECONDARY_APPS_MAX_VALUE ||
        papps_value < PEDAL_TRAVEL_DEADZONE_THRESHOLD)
    {
        // The following conditions require zero torque requests:
        // 1. When the brake pedal is pressed
        // 2. When the PAPPS/SAPPS reading is out-of-bound (e.g. When pedal is
        //    pressed beyond the calibrated maximum value)
        // 3. When PAPPS reading is less than the pedal travel deadzone
        // threshold
        accelerator_pedal_position = 0;
    }
    else if (papps_value > PEDAL_SATURATION_POINT)
    {
        // Saturate pedal position to 100% if the pedal is almost fully pressed
        // to account deflection in pedal cluster
        accelerator_pedal_position = ACCELERATOR_PEDAL_BIT_RESOLUTION;
    }
    else
    {
        // Map accelerator pedal position to a different bit resolution using
        // PAPPS reading
        accelerator_pedal_position =
            (papps_value * ACCELERATOR_PEDAL_BIT_RESOLUTION) /
            PRIMARY_APPS_MAX_VALUE;
    }

    // Prevent FSM_APPS_OPEN_CIRCUIT_SHORT_CIRCUIT_ERROR and
    // FSM_APPS_PRIMARY_SECONDARY_DIFFERENCE_ERROR from changing the
    // apps_fault_state when apps_fault_state is in
    // FSM_APPS_BRAKE_PEDAL_PLAUSIBILITY_ERROR or FSM_APPS_MAX_TORQUE_ERROR
    // Reason: Rules state that pedal must be released back to less than 5%
    // before car can be operational (EV2.5.1)
    if (apps_fault_state != FSM_APPS_BRAKE_PEDAL_PLAUSIBILITY_ERROR &&
        apps_fault_state != FSM_APPS_MAX_TORQUE_ERROR)
    {
        if (Apps_CheckAppsImplausibility() == ERROR)
        {
            // Check if implausibility occurs after 100msec
            if (apps_fault_counter > MAX_APPS_FAULTS)
            {
                apps_fault_state = FSM_APPS_OPEN_CIRCUIT_SHORT_CIRCUIT_ERROR;
            }
            apps_fault_counter++;
            fault_flag = true;
        }

        if (Apps_CheckPedalTravelImplausibility(papps_value, sapps_value) == ERROR)
        {
            // Check if implausibility occurs after 100msec
            if (apps_fault_counter > MAX_APPS_FAULTS)
            {
                apps_fault_state = FSM_APPS_PRIMARY_SECONDARY_DIFFERENCE_ERROR;
            }
            apps_fault_counter++;
            fault_flag = true;
        }
    }

    if (Apps_CheckPlausibility(papps_value) == ERROR)
    {
        // Check if plausibility occurs after 100msec
        if (apps_fault_counter > MAX_APPS_FAULTS)
        {
            apps_fault_state = FSM_APPS_BRAKE_PEDAL_PLAUSIBILITY_ERROR;
        }
        apps_fault_counter++;
        fault_flag = true;
    }

    // Check if pedal is at max. torque request
    if (accelerator_pedal_position == MAX_10_BITS_VALUE)
    {
        // Check if pedal is stuck at max. torque after 10 secs
        if (apps_fault_counter > MAX_SATURATION_FAULTS)
        {
            apps_fault_state = FSM_APPS_MAX_TORQUE_ERROR;
        }
        apps_fault_counter++;
        fault_flag = true;
    }

    if (apps_fault_state == FSM_APPS_PRIMARY_SECONDARY_DIFFERENCE_ERROR)
    {
        accelerator_pedal_position = 0;
    }
    // Check for "APPS / Brake Pedal Plausibility Check" fault state and
    // ensure APPS returns to 5% pedal travel (EV2.5.1)
    else if (
        (apps_fault_state == FSM_APPS_BRAKE_PEDAL_PLAUSIBILITY_ERROR) ||
        (apps_fault_state == FSM_APPS_MAX_TORQUE_ERROR))
    {
        accelerator_pedal_position = 0;
        if (papps_value < 0.05)
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

    return accelerator_pedal_position;
}
