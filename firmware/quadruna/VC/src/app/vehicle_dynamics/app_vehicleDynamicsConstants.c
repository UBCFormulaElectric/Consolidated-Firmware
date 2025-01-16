#include "app_vehicleDynamicsConstants.h"


const PID_Config PID_POWER_CORRECTION_CONFIG = { .Kp      = 0.1f,
                                                 .Ki      = 0.0f,
                                                 .Kd      = 0.0f,
                                                 .out_max = 1.0f,
                                                 .out_min = -1.0f };

const PID_Config PID_TRACTION_CONTROL_CONFIG = { .Kp      = 30.3f,  // old: 82.11
                                                 .Ki      = 3.0f,   // old: 16.08
                                                 .Kd      = 1.5f,   // old : 1.95
                                                 .out_max = 1.0f,
                                                 .out_min = -1.0f };