/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Apps.h"
#include "Timers.h"
#include "main.h"
#include "Constants.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/
// Maximum PAPPS value that should not be exceeded or else the pedal box may
// physically break. This should be manually calibrated every once in a while.
#define PRIMARY_APPS_MAX_VALUE   (uint32_t)(1400)

// Maximum SAPPS value that should not be exceeded or else the pedal box may
// physically break. This should be manually calibrated every once in a while.
#define SECONDARY_APPS_MAX_VALUE (uint32_t)(1950)

// The driver should not have to push the pedal to its maximum travel
// to request maximum torque or else the pedal box will wear out more
// easily
#define APPS_SATURATION_THRESHOLD (float32_t)(0.80f)

// The initial pedal travel is treated as a "deadzone" - as if the pedal was
// not pressed at all
#define PAPPS_DEADZONE_THRESHOLD (float32_t)(0.04f)

// If the encoder reading is larger than this threshold, then the pedal has
// likely deflected beyond its original position
#define APPS_UNDERFLOW_THRESHOLD (float32_t)(0.95f)


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
 * @brief  Calculate the appropriate accelerator pedal position as a percentage
 *         [0.0 to 1.0]
 * @return The accelerator pedal position
 */
static float32_t GetPercentAcceleratorPedalPosition(void);

/**
 * @brief  Read PAPPS encoder counter value as a percentage [0.0 to 1.0] and
 *         adjust it as needed
 * @return PAPPS value
 */
static float32_t GetPercentPapps(void);

/**
 * @brief  Read SAPPS encoder counter value as a percentage [0.0 to 1.0] and
 *         adjust it as needed
 * @return SAPPS value
 */
static float32_t GetPercentSapps(void);

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/
static float32_t GetPercentAcceleratorPedalPosition(void)
{
    float32_t percent_papps = GetPercentPapps();
    float32_t percent_sapps = GetPercentSapps();
 
    float32_t percent_accelerator_pedal_position;

    if (percent_papps < PAPPS_DEADZONE_THRESHOLD)
    {
       percent_accelerator_pedal_position = 0.0f;
    }
    else
    {
        percent_accelerator_pedal_position = percent_papps;
    }

    return percent_accelerator_pedal_position;
}

static float32_t GetPercentPapps(void)
{
    // Get the PAPPS position value as a percentage value
    float32_t percent_papps =
        __HAL_TIM_GET_COUNTER(&PAPPS_TIMER) /
        ((float32_t)(PRIMARY_APPS_MAX_VALUE) * APPS_SATURATION_THRESHOLD);

    // When the driver lets go of the pedal, it might deflect beyond the
    // starting position due to mechanical tolerance. This will cause the
    // encoder value to underflow. For example, when the driver lets go of the
    // accelerator pedal, the encoder may end up on tick (ENCODER_MAX_COUNT -
    // 10) instead of tick 0
    if (percent_papps > APPS_UNDERFLOW_THRESHOLD)
    {
        percent_papps = 0.0f;
    }
    else if (
        (percent_papps > 1.0f) &&
        (percent_papps < (1.0f / APPS_SATURATION_THRESHOLD)))
    {
        // Set PAPPS to 100% in the saturation region
        percent_papps = 1.0f;
    }

    return percent_papps;
}

static float32_t GetPercentSapps(void)
{
    // Get the APPS position values (Note: PAPPS and SAPPS are oriented in
    // opposite orientation - when PAPPS timer (TIM2) is counting up, the SAPPS
    // timer (TIM3) is counting down. As a result, we need to adjust the SAPPS
    // timer count by subtracing it from the max count.)
    float32_t percent_sapps =
        (ENCODER_MAX_COUNT - __HAL_TIM_GET_COUNTER(&SAPPS_TIMER)) /
        ((float32_t)(SECONDARY_APPS_MAX_VALUE) * APPS_SATURATION_THRESHOLD);

    // When the driver lets go of the pedal, it might deflect beyond the
    // starting position due to mechanical tolerance. This will cause the
    // encoder value to underflow. For example, when the driver lets go of the
    // accelerator pedal, the encoder may end up on tick (ENCODER_MAX_COUNT -
    // 10) instead of tick 0
    if (percent_sapps > APPS_UNDERFLOW_THRESHOLD)
    {
        percent_sapps = 0;
    }
    else if (
        (percent_sapps > 1.0f) &&
        (percent_sapps < (1.0f / APPS_SATURATION_THRESHOLD)))
    {
        // Set SAPPS to 100% in the saturation region
        percent_sapps = 1.0f;
    }

    return percent_sapps;
}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void Apps_HandleAcceleratorPedalPosition(void)
{
   float32_t percent_accelerator_pedal_position =
        GetPercentAcceleratorPedalPosition();

    // TODO (Issue #273): Transmit accelerator pedal position over CAN. Make
    // sure to add "is brake pressed" check here.
}
