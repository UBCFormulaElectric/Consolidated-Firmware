#pragma once
#include "app_pid.h"
#include "app_yawRateController.h"
#include "app_units.h"

// Constants

#define CAR_MASS_AT_CG_KG (300.0) // checked with suspension -- weight with driver
#define GRAVITY (9.1)
#define CG_HEIGHT_FROM_GROUND_M (30.0 * CM_TO_M) // got 30cm from suspension team
#define SMALL_EPSILON .1                         // to avoid divide by zero error
#define SLIP_RATIO_IDEAL 0.5
#define WHEELBASE_mm 1550
#define WHEELBASE_m (WHEELBASE_mm * MM_TO_M)
#define TRACK_WIDTH_mm 1100
#define TRACK_WIDTH_m (TRACK_WIDTH_mm * MM_TO_M)
// 60 / (2 * pi) * 1000 to for T = P/w formula
#define POWER_TO_TORQUE_CONVERSION_FACTOR (9550)
// max power = 80.0 we can use by FSAE rules
#define POWER_LIMIT_CAR_kW 40.0
#define MOTOR_TEMP_CUTOFF_c 90.0
// amount to decrease max power by for every degree celsius over motor cutoff temp (we got this number by looking at the
// emrax 188 manual)
#define MOTOR_TEMP_POWER_DECREMENTING_RATIO (80.0 / 30.0)
#define PID_TIMEOUT_ms 1000 // Time after witch PID values
// TODO(akoen): Power factors limits may need to be adjusted
#define PID_POWER_FACTOR_MIN (-0.9)
#define PID_POWER_FACTOR_MAX 0.1
#define APPROX_STEERING_TO_WHEEL_ANGLE 0.3 // Underestimate for wheel angles > 40deg. See Confluence/Steering System
#define MAX_TORQUE_REQUEST_NM (20.5) // Max torque of AMK DD5-14-10-POW motor (actual max is 21, using this as safety)
#define NOMINAL_TORQUE_REQUEST_NM (9.8) // Nominal torque of AMK DD5-14-10-POW motor
#define RULES_BASED_POWER_LIMIT_KW (80.0)
#define MAX_REGEN_Nm (-15.0)
#define MAX_BATTERY_TEMP (45)
#define POWER_LIMIT_REGEN_kW (17.0) // 17.64kW ~ 30A charge for molicel cells

/**
 * Reference used: https://www.zotero.org/groups/5809911/vehicle_controls_2024/items/N4TQBR67/reader
 */

#define DIST_FRONT_AXLE_CG (0.7)                             // a in meters
#define DIST_REAR_AXLE_CG (WHEELBASE_m - DIST_FRONT_AXLE_CG) // b in meters
#define WEIGHT_ACROSS_BODY (CAR_MASS_AT_CG_KG * GRAVITY / WHEELBASE_m)

/************** Macros for finding vertical forces on wheels based on diagram on page 21 ****************/
#define REAR_WEIGHT_DISTRIBUTION (WEIGHT_ACROSS_BODY * DIST_REAR_AXLE_CG)
#define LONG_ACCEL_TERM_VERTICAL_FORCE(long_accel) \
    ((CAR_MASS_AT_CG_KG * (long_accel) * CG_HEIGHT_FROM_GROUND_M / WHEELBASE_m))

/************** Macros for finding Kmz based on diagram on page 57 ****************/
#define ACCELERATION_TERM_KMZ(long_accel) (DIST_FRONT_AXLE_CG + (long_accel) * CG_HEIGHT_FROM_GROUND_M / GRAVITY)

/************** Macros for moment scaling on page 58 ****************/

// Our motors no longer take in a torque command, instead they take in a percentage value. The percentage for the
// DD5-14-10-POW motor is relative to their nominal torque (9.8 Nm) where 100% torque is 9.8. The motors are able to
// output up to 21 Nm, this however cannot be a sustained behaviour. Note the message takes a int 16 that is essentially
// your percentage of nominal torque * 1000
#define NM_TO_INVERTER_TORQUE(torque) ((int16_t)((torque / NOMINAL_TORQUE_REQUEST_NM) * 1000.0))
#define TORQUE_TO_POWER(torque, rpm) ((torque) * ((rpm) / (GEAR_RATIO)) / (POWER_TO_TORQUE_CONVERSION_FACTOR))
#define POWER_TO_TORQUE(power, rpm)                  \
    (((power) * POWER_TO_TORQUE_CONVERSION_FACTOR) / \
     ((fmaxf(rpm, 0.1)) / (GEAR_RATIO))) // Doing this for no division by 0, and assuming rpm is always positive
// Tunable parameters
extern const PID_Config               PID_POWER_CORRECTION_CONFIG;
extern const PID_Config               PID_TRACTION_CONTROL_CONFIG;
extern const PID_Config               PID_YAW_RATE_CONTROLLER_CONFIG;
extern const YawRateController_Config YAW_RATE_CONTROLLER_CONFIG;
