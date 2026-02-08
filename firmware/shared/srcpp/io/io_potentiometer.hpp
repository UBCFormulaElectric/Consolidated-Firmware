#pragma once

#include <cstdint>
#include <io_potentiometer.h>
#include "util_errorCodes.hpp"

constexpr float MAX_WIPER_VALUE = 256.0f;
constexpr float MIN_WIPER_VALUE = 0.0f;
constexpr uint8_t POT_WIPER0_REGISTER = 0x0;
constexpr uint8_t POT_WIPER1_REGISTER = 0x1;

#ifdef TARGET_EMBEDDED

#include "hw_i2c.hpp"

namespace io
{
    class Potentiometer
    {
        private:
            const hw::i2c::I2CDevice device;
        
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
            consteval explicit Potentiometer(
                hw::i2c::I2CDevice device,
                POTENTIOMETER_WIPER wiper
            )

            std::expected<void, ErrorCode> readWiper(std::array<uint8_t, 2> &dest) const
            {
                std::array<uint8_t, 1> read_cmd {static_cast<uint8_t>((((wiper == WIPER0) ? POT_WIPER0_REGISTER : POT_WIPER1_REGISTER) << 4) |
                                       (static_cast<uint8_t>(POTENTIOMETER_READ_CMD) << 2))};
                RETURN_IF_ERR(device.transmit(read_cmd));
                RETURN_IF_ERR(device.receive(dest));
                return {};
            }

            std::expected<void, ErrorCode> readPercentage(uint8_t &dest) const
            {
                std::array<uint8_t, 2> data {0};
                RETURN_IF_ERR(readWiper(data));
                uint16_t read_data {static_cast<uint16_t>(data[0] << 8 | data [1])};
                dest = static_cast<uint8_t>((read_data * 100)/ MAX_WIPER_VALUE);
                return {};
            }
    }
}

//Rpump is connected to wiper0 not wiper1 this year

