#include "torquevectoring/App_TorqueVectoringConstants.h"

const PID_Config PID_POWER_CORRECTION_CONFIG = { .Kp      = 0.01f,
                                                 .Ki      = 0.0f,
                                                 .Kd      = 0.0f,
                                                 .out_max = 1.0f,
                                                 .out_min = -1.0f };

// TODO(akoen): Determine default PID parameters for traction control
const PID_Config PID_TRACTION_CONTROL_CONFIG = { .Kp      = 0.001f,
                                                 .Ki      = 0.0f,
                                                 .Kd      = 0.0f,
                                                 .out_max = 0.0f,
                                                 .out_min = -1.0f };
