#include "app/vehicle_dynamics/vehicleDynamicsConstants.h"

const PID_Config PID_POWER_CORRECTION_CONFIG = { .Kp      = 0.1f,
                                                 .Ki      = 0.0f,
                                                 .Kd      = 0.0f,
                                                 .out_max = 1.0f,
                                                 .out_min = -1.0f };

const PID_Config PID_TRACTION_CONTROL_CONFIG = { .Kp      = 0.0001f,
                                                 .Ki      = 0.01f,
                                                 .Kd      = 0.0f,
                                                 .out_max = 1.0f,
                                                 .out_min = -1.0f };

const PID_Config PID_YAW_RATE_CONTROLLER_CONFIG = { .Kp      = 0.25f,
                                                    .Ki      = 0.0f,
                                                    .Kd      = 0.0f,
                                                    .out_max = 360.0f,
                                                    .out_min = -360.0f };

const YawRateController_Config YAW_RATE_CONTROLLER_CONFIG = { .pid_config = &PID_YAW_RATE_CONTROLLER_CONFIG,
                                                              .ku         = 0.0f };