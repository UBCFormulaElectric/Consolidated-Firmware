#pragma once

#include "io_potentiometer.hpp"

namespace io::rPump
{
/*
 * Checks rPump is communicating over I2C
 */
result<void> ready();

/*
 * Sends the percentage the wiper should be at
 */
result<void> setPercentage(uint8_t percentage);

/*
 * Reads the percentage the wiper is at
 */
result<uint8_t> readPercentage();
} // namespace io::rPump