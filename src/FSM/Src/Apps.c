/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Apps.h"
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
static uint32_t GetAcceleratorPedalPosition(uint32_t papps_value);

/**
 * @brief  Read PAPPS encoder counter value and adjust it as needed
 * @return PAPPS value
 */
static uint32_t GetPappsValue(void);

/**
 * @brief  Read SAPPS encoder counter value and adjust it as needed
 * @return SAPPS value
 */
static uint32_t GetSappsValue(void);

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/
static uint32_t GetAcceleratorPedalPosition(uint32_t papps_value)
{
    uint32_t accelerator_pedal_position;
    
    if (Gpio_IsBrakePressed() || papps_value < PAPPS_DEADZONE_THRESHOLD)
    {
        // The following conditions require zero torque requests:
        // 1. When the brake pedal is pressed
        // 2. When PAPPS reading is less than the pedal travel deadzone
        // threshold
        accelerator_pedal_position = 0;
    }
    else if (papps_value > PAPPS_SATURATION_THRESHOLD)
    {
        // Saturate pedal position to 100% if the pedal is almost fully pressed
        // to account deflection in pedal cluster
        accelerator_pedal_position = ACCELERATOR_PEDAL_POSITION_MAX_TORQUE;
    }
    else
    {
        // Map accelerator pedal position to a different bit range using
        // PAPPS reading
        accelerator_pedal_position =
            (papps_value * ACCELERATOR_PEDAL_POSITION_MAX_TORQUE) /
            PRIMARY_APPS_MAX_VALUE;
    }

    return accelerator_pedal_position;
}

static uint32_t GetPappsValue(void)
{
    // Get the PAPPS position value
    uint32_t papps_value = __HAL_TIM_GET_COUNTER(&htim2);

    // When the driver lets go of the pedal, it might deflect beyond the starting
    // position due to mechanical tolerance. This will cause the encoder value to
    // underflow. For example, when the driver lets go of the accelerator pedal,
    // the encoder may end up on tick (ENCODER_MAX_COUNT - 10) instead of tick 0
    if (papps_value > APPS_UNDERFLOW_THRESHOLD)
    {
        papps_value = 0;
    }

    return papps_value;
}

static uint32_t GetSappsValue(void)
{
    // Get the APPS position values (Note: PAPPS and SAPPS are oriented in
    // opposite orientation - when PAPPS timer (TIM2) is counting up, the SAPPS
    // timer (TIM3) is counting down. As a result, we need to adjust the SAPPS
    // timer count by subtracing it from the max count.)
    uint32_t sapps_value = ENCODER_MAX_COUNT - __HAL_TIM_GET_COUNTER(&htim3);

    // When the driver lets go of the pedal, it might deflect beyond the starting
    // position due to mechanical tolerance. This will cause the encoder value to
    // underflow. For example, when the driver lets go of the accelerator pedal,
    // the encoder may end up on tick (ENCODER_MAX_COUNT - 10) instead of tick 0
    if (sapps_value > APPS_UNDERFLOW_THRESHOLD)
    {
        sapps_value = 0;
    }

    return sapps_value;
}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void Apps_HandleAcceleratorPedalPosition(void)
{
    uint32_t papps_value = GetPappsValue();
    uint32_t sapps_value = GetSappsValue();
    uint32_t accelerator_pedal_position = GetAcceleratorPedalPosition(papps_value);

    // TODO (Issue #273): Transmit accelerator pedal position over CAN
}
