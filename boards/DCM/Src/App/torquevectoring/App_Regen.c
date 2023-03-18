#include "torquevectoring/App_Regen.h"
#include "torquevectoring/App_TorqueVectoringConstants.h"
#include "math.h"

void App_Regen_ComputeTorque(struct Regen_Inputs *inputs, struct Regen_Outputs *outputs)
{
    float vehicle_speed_kph = (inputs->wheel_speed_front_left_kph + inputs->wheel_speed_front_left_kph) / 2.0f;

    if (vehicle_speed_kph > 5.0f)
    {
        // TODO: Implement regen with power limiting (CAN messages not yet defined)
    }
    outputs->regen_torque_left_Nm  = 0;
    outputs->regen_torque_right_Nm = 0;
}
