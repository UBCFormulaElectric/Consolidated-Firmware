#pragma once
extern "C"
{
    #include "app_utils.h"
}

#ifdef TARGET_EMBEDDED
#include "hw_i2cs.hpp" //Not created yet

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
    
    #endif 

    ExitCode setPercentage(float percentage);
    ExitCode readPercentage(float *dest);
}