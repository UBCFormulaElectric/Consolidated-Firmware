#pragma once
#include "App_PID.h"

// Conversions
#define IN_TO_M 0.0254f

// Constants
#define SMALL_EPSILON .000001f // to avoid divide by zero error
#define PLANETARY_GEAR_RATIO 4.241f
#define SLIP_RATIO_IDEAL 0.05f
#define WHEELBASE_mm 1550
#define TRACK_WIDTH_mm 1100
#define TIRE_DIAMETER_in 18.0f
#define POWER_TO_TORQUE_CONVERSION_FACTOR 9550 // as specified in the manual for the emrax 188 motors
#define POWER_LIMIT_CAR_kW 40.0f               // Actual Value: 80.0f   // max power we can use by FSAE rules
#define MOTOR_TEMP_CUTOFF_c 90.0f
#define MOTOR_TEMP_POWER_DECREMENTING_RATIO                                                                            \
    80.0f / 30.0f // amount to decrease max power by for every degree celsius over motor cutoff temp (we got this number
                  // by looking at the emrax 188 manual)
#define PID_TIMEOUT_ms 1000 // Time after witch PID values
// TODO(akoen): Power factors limits may need to be adjusted
#define PID_POWER_FACTOR_MIN (-0.9f)
#define PID_POWER_FACTOR_MAX 0.1f
#define APPROX_STEERING_TO_WHEEL_ANGLE 0.3f // Underestimate for wheel angles > 40deg. See Confluence/Steering System
#define MAX_REGEN_Nm -30.0f
#define MAX_BATTERY_TEMP 45
#define POWER_LIMIT_REGEN_kW 10.0f // 10.5kW ~ 35A tbd varying limits?

// Tunable parameters
extern const PID_Config PID_POWER_CORRECTION_CONFIG;
extern const PID_Config PID_TRACTION_CONTROL_CONFIG;
