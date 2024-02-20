#include "app_vehicleDynamicsConstants.h"

const PID_Config PID_POWER_CORRECTION_CONFIG = { .Kp      = 0.1f,
                                                 .Ki      = 0.0f,
                                                 .Kd      = 0.0f,
                                                 .out_max = 1.0f,
                                                 .out_min = -1.0f };

const PID_Config PID_TRACTION_CONTROL_CONFIG = { .Kp      = 0.1f,
                                                 .Ki      = 0.0f,
                                                 .Kd      = 0.0f,
                                                 .out_max = 1.0f,
                                                 .out_min = -1.0f };