/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Timers.h"
#include "Apps.h"
#include "Gpio.h"
#include "CanDefinitions.h"

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
 *******************************************************************************/
/**
 * @brief Main control loop
 *          1. Transmits CAN messages containing the accelerator pedal position,
 *          front wheel speeds, and steering angle.
 */
static void Timers_ControlLoop(void);

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/
static void Timers_ControlLoop(void)
{
    static MotorShutdownStatus_Enum motor_state;
    uint16_t accelerator_pedal_position = 0;

    // Get sensor data
    // Only call this function in APPS_CONTROL_LOOP_MODE once in this control
    // loop function
    accelerator_pedal_position =
        getAcceleratorPedalPosition(APPS_CONTROL_LOOP_MODE);

    // ERROR HANDLING
    // APPS fault when motors are on
    if (apps_fault_state != FSM_APPS_NORMAL_OPERATION && motor_state == ON)
    {
        // TransmitCANError(Motor_Shutdown_Error_StandardID,
        // Front_Sensor_Module, apps_fault_state, CANBrakeAPPS);
        motor_state = OFF;
    }

    if (apps_fault_state != FSM_APPS_NORMAL_OPERATION)
    {
        Gpio_TurnOnRedLed();
    }

    // No APPS fault when motors are off
    if (apps_fault_state == FSM_APPS_NORMAL_OPERATION && motor_state == OFF)
    {
        // TransmitDataCAN(Motor_ReEnable_StandardID, Motor_ReEnable_ExtendedID,
        // Motor_ReEnable_DLC, Front_Sensor_Module);
        motor_state = ON;
    }
}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim14)
    {
        Timers_ControlLoop();
    }
}

void Timers_StartTimers()
{
    // Start control loop timer
    HAL_TIM_Base_Start_IT(&htim14);

    // Start primary APPS encoder
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

    // Start secondary APPS encoder
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}
