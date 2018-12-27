/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Apps.h"
#include "CanDefinitions.h"
#include "stdbool.h"
#include "Gpio.h"
#include "main.h"

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
/**
 * @brief  Calculate the appropriate accelerator pedal position based on PAPPS
 *         position value and brake pedal status
 * @return The accelerator pedal position
 */
static uint32_t prvSetAcceleratorPedalPosition(
    uint32_t *papps_value,
    uint32_t *accelerator_pedal_position);

/**
 * @brief  Read encoder counter values for PAPPS and SAPPS value and adjust
 *         them as appropriate
 */
static void
    prvSetPappsSappsValues(uint32_t *papps_value, uint32_t *sapps_value);

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/
static uint32_t prvSetAcceleratorPedalPosition(
    uint32_t *papps_value,
    uint32_t *accelerator_pedal_position)
{
    if (Gpio_IsBrakePressed() || *papps_value < PAPPS_DEADZONE_THRESHOLD)
    {
        // The following conditions require zero torque requests:
        // 1. When the brake pedal is pressed
        // 2. When PAPPS reading is less than the pedal travel deadzone
        // threshold
        *accelerator_pedal_position = 0;
    }
    else if (*papps_value > PAPPS_SATURATION_THRESHOLD)
    {
        // Saturate pedal position to 100% if the pedal is almost fully pressed
        // to account deflection in pedal cluster
        *accelerator_pedal_position = ACCELERATOR_PEDAL_POSITION_MAX_TORQUE;
    }
    else
    {
        // Map accelerator pedal position to a different bit range using
        // PAPPS reading
        *accelerator_pedal_position =
            (*papps_value * ACCELERATOR_PEDAL_POSITION_MAX_TORQUE) /
            PRIMARY_APPS_MAX_VALUE;
    }
}

static void prvSetPappsSappsValues(uint32_t *papps_value, uint32_t *sapps_value)
{
    // Get the APPS position values (Note: PAPPS and SAPPS are oriented in
    // opposite orientation - when PAPPS timer (TIM2) is counting up, the SAPPS
    // timer (TIM3) is counting down. As a result, we need to adjust the SAPPS
    // timer count by subtracing it from the max count.)
    *papps_value = __HAL_TIM_GET_COUNTER(&htim2);
    *sapps_value = ENCODER_MAX_COUNT - __HAL_TIM_GET_COUNTER(&htim3);
}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void Apps_HandleAcceleratorPedalPosition(void)
{
    uint32_t papps_value;
    uint32_t sapps_value;

    prvSetPappsSappsValues(&papps_value, &sapps_value);

    uint32_t accelerator_pedal_position;

    prvSetAcceleratorPedalPosition(&papps_value, &accelerator_pedal_position);

    // TODO (Issue #273): Transmit accelerator pedal position over CAN
}
