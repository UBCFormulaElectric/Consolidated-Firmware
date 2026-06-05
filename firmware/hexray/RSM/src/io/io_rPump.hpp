#pragma once

#include "io_potentiometer.hpp"
#include <cstdint>

namespace io::rPump
{
extern const Potentiometer rPump;

/*
 * Checks rPump is communicating over I2C
 */
result<void> isPumpReady();

/*
 * Sends the percentage the wiper should be at
 */
result<void> setPercentage(uint8_t percentage);

/*
 * Reads the percentage the wiper is at
 */
result<void> readPercentage(uint8_t &dest);
} // namespace io::rPump