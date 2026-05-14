#pragma once

#include "io_potentiometer.hpp"

namespace io::rPump
{
/*
 * Checks rPump is communicating over I2C
 */
std::expected<void, ErrorCode> ready();

/*
 * Sends the percentage the wiper should be at
 */
std::expected<void, ErrorCode> setPercentage(uint8_t percentage);

/*
 * Reads the percentage the wiper is at
 */
std::expected<uint8_t, ErrorCode> readPercentage();
} // namespace io::rPump