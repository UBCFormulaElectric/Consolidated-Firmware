#pragma once
#include "io_potentiometer.hpp"
#include "hw_i2cs.hpp"
#include "util_errorCodes.hpp"
#include <cstdint>

#ifdef TARGET_EMBEDDED
// #include "hw_i2cs.hpp" //Not created yet
#endif

namespace io::rPump
{
extern const Potentiometer rPump;

/*
 * Checks rPump is communicating over I2C
 */
std::expected<void, ErrorCode> isPumpReady();

/*
 * Sends the percentage the wiper should be at
 */
std::expected<void, ErrorCode> setPercentage(uint8_t percentage);

/*
 * Reads the percentage the wiper is at
 */
std::expected<void, ErrorCode> readPercentage(uint8_t &dest);
} // namespace io::rPump