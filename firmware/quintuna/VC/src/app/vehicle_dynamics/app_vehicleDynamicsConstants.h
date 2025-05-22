#pragma once
<<<<<<< HEAD
#include "app_pid.h"
#include "app_yawRateController.h"
=======
// #include "app_pid.h"
// #include "app_yawRateController.h"
#include "app_velocityEstimator.h"
>>>>>>> 41a468488 (added constants)
#include "app_units.h"

// Constants

#define CAR_MASS_AT_CG_KG (300.0f) // checked with suspension -- weight with driver
#define GRAVITY (9.81f)
#define CG_HEIGHT_FROM_GROUND_m (30.0f * CM_TO_M) // got 30cm from suspension team
#define SMALL_EPSILON .000001f                    // to avoid divide by zero error
#define SLIP_RATIO_IDEAL 0.05f
#define WHEELBASE_mm 1550
#define WHEELBASE_m (WHEELBASE_mm * MM_TO_M)
#define TRACK_WIDTH_mm 1100
#define TRACK_WIDTH_m (TRACK_WIDTH_mm * MM_TO_M)
#define POWER_TO_TORQUE_CONVERSION_FACTOR 9550 // 60 / (2 * pi) * 1000 to for T = P/w formula
#define POWER_LIMIT_CAR_kW 40.0f               // max power = 80.0f we can use by FSAE rules
#define MOTOR_TEMP_CUTOFF_c 90.0f
#define MOTOR_TEMP_POWER_DECREMENTING_RATIO \
    80.0f / 30.0f // amount to decrease max power by for every degree celsius over motor cutoff temp (we got this number
                  // by looking at the emrax 188 manual)
#define PID_TIMEOUT_ms 1000 // Time after witch PID values
// TODO(akoen): Power factors limits may need to be adjusted
#define PID_POWER_FACTOR_MIN (-0.9f)
#define PID_POWER_FACTOR_MAX 0.1f
#define APPROX_STEERING_TO_WHEEL_ANGLE 0.3f // Underestimate for wheel angles > 40deg. See Confluence/Steering System
#define MAX_TORQUE_REQUEST_NM (20.5f) // Max torque of AMK DD5-14-10-POW motor (actual max is 21, using this as safety)
#define NOMINAL_TORQUE_REQUEST_NM (9.8f) // Nominal torque of AMK DD5-14-10-POW motor
#define RULES_BASED_POWER_LIMIT_KW (80.0f)
#define MAX_REGEN_Nm -15.0f
#define MAX_BATTERY_TEMP 45
#define POWER_LIMIT_REGEN_kW 17.0f // 17.64kW ~ 30A charge for molicel cells

/**
 * Reference used: https://www.zotero.org/groups/5809911/vehicle_controls_2024/items/N4TQBR67/reader
 */

#define DIST_FRONT_AXLE_CG (0.837f)                          // a in meters
#define DIST_REAR_AXLE_CG (WHEELBASE_m - DIST_FRONT_AXLE_CG) // b in meters
#define WEIGHT_ACROSS_BODY (CAR_MASS_AT_CG_KG * GRAVITY / WHEELBASE_m)

/************** Macros for finding vertical forces on wheels based on diagram on page 21 ****************/
#define REAR_WEIGHT_DISTRIBUTION (WEIGHT_ACROSS_BODY * DIST_REAR_AXLE_CG)
#define LONG_ACCEL_TERM_VERTICAL_FORCE(long_accel) \
    ((CAR_MASS_AT_CG_KG * (long_accel) * CG_HEIGHT_FROM_GROUND_m / WHEELBASE_m))
#define LAT_ACCEL_TERM_VERTICAL_FORCE(lat_accel) \
    ((CAR_MASS_AT_CG_KG * (lat_accel) * CG_HEIGHT_FROM_GROUND_m / (2.0f * (TRACK_WIDTH_m))))

/************** Macros for finding Kmz based on diagram on page 57 ****************/
#define ACCELERATION_TERM_KMZ(long_accel) (DIST_FRONT_AXLE_CG + (long_accel) * CG_HEIGHT_FROM_GROUND_m / GRAVITY)

/************** Macros for moment scaling on page 58 ****************/
#define F ((TRACK_WIDTH_m / (WHEEL_DIAMETER_IN / 2.0f * 2.54f)) * GEAR_RATIO)

// Our motors no longer take in a torque command, instead they take in a percentage value. The percentage for the
// DD5-14-10-POW motor is relative to their nominal torque (9.8 Nm) where 100% torque is 9.8. The motors are able to
// output up to 21 Nm, this however cannot be a sustained behaviour. Note the message takes a int 16 that is essentially
// your percentage of nominal torque * 1000
#define PEDAL_REMAPPING(torque) ((int16_t)((torque / NOMINAL_TORQUE_REQUEST_NM) * 1000.0f))
#define TORQUE_TO_POWER(torque, rpm) ((torque) * ((float)(rpm) / (GEAR_RATIO)) / (POWER_TO_TORQUE_CONVERSION_FACTOR))
#define POWER_TO_TORQUE(power, rpm)                  \
    (((power) * POWER_TO_TORQUE_CONVERSION_FACTOR) / \
     ((fmaxf(rpm, 0.00001f)) / (GEAR_RATIO))) // Doing this for no division by 0, and assuming rpm is always positive
// Tunable parameters
extern const PID_Config               PID_POWER_CORRECTION_CONFIG;
extern const PID_Config               PID_TRACTION_CONTROL_CONFIG;
extern const PID_Config               PID_YAW_RATE_CONTROLLER_CONFIG;
extern const YawRateController_Config YAW_RATE_CONTROLLER_CONFIG;
