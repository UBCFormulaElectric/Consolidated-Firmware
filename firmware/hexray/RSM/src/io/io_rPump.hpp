#pragma once

#include "util_errorCodes.hpp"

#ifdef TARGET_EMBEDDED
// #include "hw_i2cs.hpp" //Not created yet
#endif

namespace io::rPump
{
/*
 * Checks rPump is communicating over I2C
 * returns True if the I2C is ready
 */
std::expected<void, ErrorCode> isPumpReady();

/*
 * data to write to the pump controller
 * return success of operation
 */
std::expected<void, ErrorCode> write(uint8_t data);

/*
 * dest (destination) to write the data to
 * return success of operation
 */
std::expected<void, ErrorCode> read(uint8_t *dest);

std::expected<void, ErrorCode> setPercentage(float percentage);
std::expected<void, ErrorCode> readPercentage(float *dest);
} // namespace io::rPump