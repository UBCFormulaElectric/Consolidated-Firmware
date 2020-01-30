/**
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
#define MAX_6_BITS_VALUE  (uint32_t)(63)
#define MAX_8_BITS_VALUE  (uint32_t)(255)
#define MAX_10_BITS_VALUE (uint32_t)(1023)
#define MAX_12_BITS_VALUE (uint32_t)(4095)
#define MAX_16_BITS_VALUE (uint32_t)(65535)
#define MAX_32_BITS_VALUE (uint32_t)(4294967295)

/**
 * @brief The pedal position is normalized to a maximum value that is
 *        more-or-less arbitrary and can be adjusted according to the driver's
 *        preference
 */
#define ACCELERATOR_PEDAL_POSITION_MAX_TORQUE 1000

// clang-format on

#endif /* CONSTANTS_H */
