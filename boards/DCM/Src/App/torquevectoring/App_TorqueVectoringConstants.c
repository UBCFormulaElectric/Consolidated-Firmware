#include "torquevectoring/App_TorqueVectoringConstants.h"
// TODO(akoen): Determine default PID parameters

const PID_Config PID_POWER_CORRECTION_CONFIG = { .Kp = 1.0f, .Ki = 0.0f, .Kd = 0.0f, .out_max = 2.0f, .out_min = 0.0f };

const PID_Config PID_TRACTION_CONTROL_CONFIG = { .Kp = 1.0f, .Ki = 0.0f, .Kd = 0.0f, .out_max = 1.0f, .out_min = 0.0f };
