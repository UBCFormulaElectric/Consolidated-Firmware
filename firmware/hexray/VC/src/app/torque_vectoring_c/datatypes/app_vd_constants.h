#pragma once
#include "app_pid.h"
#include "torque_vectoring_c/controllers/yaw_rate_control/app_yawRateController.h"
#include "app_units.h"
#include "math.h"

// Include generated CAN utils first to ensure enums are defined before macros
// This prevents macro/enum conflicts (SWITCH_OFF, DRIVE_MODE_POWER, etc.)
// The generated header only includes standard headers, so it's safe to include here
#include "app_canUtils.h"

// Constants

#define CAR_MASS_AT_CG_KG (300.0) // checked with suspension -- weight with driver
#define GRAVITY (9.81)
#define CG_HEIGHT_FROM_GROUND_M (30.0 * CM_TO_M) // got 30cm from suspension team
#define SMALL_EPSILON .1                         // to avoid divide by zero error
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

#define DIST_FRONT_AXLE_CG 0.7                               // a in meters
#define DIST_REAR_AXLE_CG (WHEELBASE_m - DIST_FRONT_AXLE_CG) // b in meters
#define WEIGHT_ACROSS_BODY (CAR_MASS_AT_CG_KG * GRAVITY / WHEELBASE_m)

/************** Macros for finding normal forces on wheels based on diagram on page 21 ****************/
#define REAR_CONST_LOAD (WEIGHT_ACROSS_BODY * DIST_FRONT_AXLE_CG / 2.0) // per wheel on axle
#define FRONT_CONST_LOAD (WEIGHT_ACROSS_BODY * DIST_REAR_AXLE_CG / 2.0) // per wheel on axle
#define LONG_LOAD_TRANSFER(long_accel) ((CAR_MASS_AT_CG_KG * (long_accel) * CG_HEIGHT_FROM_GROUND_M / WHEELBASE_m))
#define LAT_LOAD_TRANSFER(lat_accel) (CAR_MASS_AT_CG_KG * lat_accel * CG_HEIGHT_FROM_GROUND_M / (2.0 * (TRACK_WIDTH_m)))

/************** Macros for finding Kmz based on diagram on page 57 ****************/
#define ACCELERATION_TERM_KMZ(long_accel) (DIST_FRONT_AXLE_CG + (long_accel) * CG_HEIGHT_FROM_GROUND_M / GRAVITY)

/************** Macros for moment scaling on page 58 ****************/

// Our motors no longer take in a torque command, instead they take in a percentage value. The percentage for the
// DD5-14-10-POW motor is relative to their nominal torque (9.8 Nm) where 100% torque is 9.8. The motors are able to
// output up to 21 Nm, this however cannot be a sustained behaviour. Note the message takes a int 16 that is essentially
// your percentage of nominal torque * 1000
#define NM_TO_INVERTER_TORQUE(torque) ((int16_t)(((torque) / NOMINAL_TORQUE_REQUEST_NM) * 1000.0))
#define TORQUE_TO_POWER(torque, rpm) ((torque) * ((rpm) / (GEAR_RATIO)) / (POWER_TO_TORQUE_CONVERSION_FACTOR))
#define POWER_TO_TORQUE(power, rpm)                  \
    (((power) * POWER_TO_TORQUE_CONVERSION_FACTOR) / \
     ((fmaxf(rpm, 0.1)) / (GEAR_RATIO))) // Doing this for no division by 0, and assuming rpm is always positive

// Eqn 14.11. from RCVD
#define SLIP_RATIO(wheel_speed, vehicle_speed, slip_angle) \
    (((wheel_speed) - (vehicle_speed * cos(slip_angle))) / ((vehicle_speed * cos(slip_angle))))

#define SLIP_RATIO_IDEAL 0.05

// Switch state enum values (from CAN enum SwitchState)
// Only define if not already defined (e.g., by generated enum)
#ifndef SWITCH_OFF
#define SWITCH_OFF 0
#endif
#ifndef SWITCH_ON
#define SWITCH_ON 1
#endif

// Drive mode enum values (from CAN enum DriveMode)
// Only define if not already defined (e.g., by generated enum)
#ifndef DRIVE_MODE_POWER
#define DRIVE_MODE_POWER 0
#endif
#ifndef DRIVE_MODE_POWER_AND_ACTIVE
#define DRIVE_MODE_POWER_AND_ACTIVE 1
#endif
#ifndef DRIVE_MODE_TV
#define DRIVE_MODE_TV 2
#endif
#ifndef DRIVE_MODE_COUNT
#define DRIVE_MODE_COUNT 3
#endif
