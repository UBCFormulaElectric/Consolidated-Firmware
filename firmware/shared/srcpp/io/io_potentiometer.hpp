#pragma once

#include <cstdint>
#include <io_potentiometer.h>

#ifdef TARGET_EMBEDDED

#include "hw_i2c.hpp"

namespace io
{
    class Potentiometer
    {
        private:
            const hw::i2c::I2CDevice handle;
        
            enum class POTENTIOMETER_WIPER
            {
                WIPER0 = 0,
                WIPER1,
            };

            enum class POTENTIOMETER_CMD
            {
                POTENTIOMETER_WRITE_CMD = 0x00,
                POTENTIOMETER_INC_CMD   = 0x01,
                POTENTIOMETER_DEC_CMD   = 0x02,
                POTENTIOMETER_READ_CMD  = 0x03,
            };

            struct __attribute__((packed)) pump_write_command
            {
                // byte 1
                uint8_t           _padding : 2;
                POTENTIOMETER_CMD cmd : 2;
                uint8_t           address : 4;
                // byte 2
                uint8_t data;
            }
        
        public:
            Potentiometer()
    }
}



