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
ExitCode isPumpReady();

/*
 * data to write to the pump controller
 * return success of operation
 */
ExitCode write(uint8_t data);

/*
 * dest (destination) to write the data to
 * return success of operation
 */
ExitCode read(uint8_t *dest);

ExitCode setPercentage(float percentage);
ExitCode readPercentage(float *dest);
} // namespace io::rPump