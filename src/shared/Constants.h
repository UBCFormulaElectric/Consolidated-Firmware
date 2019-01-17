/**
 * @file  Constants.h
 * @brief A list of commonly used constants that may be shared between projects
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

/******************************************************************************
 * Includes
 ******************************************************************************/

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
#define MAX_4_BITS_VALUE  (uint32_t)(15)
#define MAX_8_BITS_VALUE  (uint32_t)(255)
#define MAX_10_BITS_VALUE (uint32_t)(1023)
#define MAX_16_BITS_VALUE (uint32_t)(65535)
#define MAX_32_BITS_VALUE (uint32_t)(4294967295)

/**
 * @brief The pedal position is normalized to a maximum value that is
 *        more-or-less arbitrary and can be adjusted according to the driver's
 *        preference
 */
#define ACCELERATOR_PEDAL_POSITION_MAX_TORQUE 1000

// clang-format on


// Diameter of the tire (m)
static const float TIRE_DIAMETER_IN_METERS = 0.52;				
// Wheel speed limits (km/h)
static const float MAX_WHEEL_SPEED = 200.0;

#endif /* CONSTANTS_H */
