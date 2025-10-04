#pragma once
extern "C"
{
    #include "app_utils.h"
}

#ifdef TARGET_EMBEDDED
#include "hw_i2cs.hpp" //Not created yet
/*
* Checks rPump is communicating over I2C
* returns True if the I2C is ready
*/
ExitCode io_rPump_isPumpReady(); 

/*
* data to write to the pump controller
* return success of operation
*/
ExitCode io_rPump_write(uint8_t data); 


/*dest (destination) to write the data to
* return success of operation
*/
ExitCode io_rPump_read(uint8_t *dest); 

#endif

ExitCode io_rPump_setPercentage(float percentage); 

ExitCode io_rPump_readPercentage(float *dest);