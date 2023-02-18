//
// Created by formulae on 11/02/23.
//

#ifndef FORMULA_ELECTRIC_FIRMWARE_TORQUEVECTORING_CONSTANTS_H
#define FORMULA_ELECTRIC_FIRMWARE_TORQUEVECTORING_CONSTANTS_H

#define WHEEL_RADIUS_m
#define FRONT_AXLE_LENGTH_m
#define POWER_TO_TORQUE_CONVERSION_FACTOR  9550 //as specified in the manual for the emrax 188 motors
#define HARD_POWER_LIMIT_kW 80
#define MOTOR_TEMP_CUTOFF_c 90 //in degrees celsius
#define MOTOR_TEMP_POWER_DECREMENTING_RATIO 80/30 //amount to decrease max power
                                                // by for every degree celsius over motor cutoff temp (we got this number by looking at the emrax 188 manual)
#define SMALL_EPSILON .000001 //to avoid divide by zero error

#endif //FORMULA_ELECTRIC_FIRMWARE_TORQUEVECTORING_CONSTANTS_H
