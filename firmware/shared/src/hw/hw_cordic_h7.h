#include <stm32h7xx_hal_cordic.h>
#include <stdint.h>
#include "app_utils.h"

/**
 * Single argument CORDIC operations
 *
 * These functions are designed to be used similarly to standard library math functions for ease of use
 */

// result = m*cos(theta)
float c_cos(const float theta, const float m);

// result = m*sin(theta)
float c_sin(const float theta, const float m);

// result = angle of 2D vector
float c_phase(const float x, const float y);

// result = magnitude of 2D vector
float c_mag(const float x, const float y);

// result of the functions below should be same as the standard lib
float c_atan(const float x);
float c_cosh(const float x);
float c_sinh(const float x);
float c_atanh(const float x);
float c_ln(const float x);
float c_sqrt(const float x);

/**
 * Multiple argument CORDIC operations
 *
 * These functions take in buffers of arguments and performs the operation on all arguments
 */