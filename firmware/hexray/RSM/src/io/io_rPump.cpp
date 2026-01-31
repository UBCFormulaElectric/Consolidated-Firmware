#include "io_rPump.hpp"
#include <util_errorCodes.hpp>
// #include "hw_i2cs.hpp" //Not yet created

// extern "C"
// {
// #include "io_potentiometer.h"
// }

// static const Potentiometer rsm_pot = { .i2c_handle = &r_pump_i2c };

#define RPUMP_WIPER 0
#define MAX_WIPER_VALUE 256.0f

namespace io::rPump
{
std::expected<void, ErrorCode> isPumpReady()
{
    // return hw_i2c_isTargetReady(&r_pump_i2c);
    return std::unexpected(ErrorCode::UNIMPLEMENTED);
}

std::expected<void, ErrorCode> write(const uint8_t data)
{
    // return io_potentiometer_writeWiper(&rsm_pot, RPUMP_WIPER, data);
    return std::unexpected(ErrorCode::UNIMPLEMENTED);
}

std::expected<void, ErrorCode> read(uint8_t *dest)
{
    // return io_potentiometer_readWiper(&rsm_pot, RPUMP_WIPER, dest);
    return std::unexpected(ErrorCode::UNIMPLEMENTED);
}

std::expected<void, ErrorCode> setPercentage(float percentage)
{
    // percentage = CLAMP(percentage, 0.0f, 100.0f);
    // return io::rPump::write((uint8_t)((1.0f - percentage / 100.0f) * MAX_WIPER_VALUE));
    return std::unexpected(ErrorCode::UNIMPLEMENTED);
}

std::expected<void, ErrorCode> readPercentage(float *dest)
{
    // uint8_t data;
    // RETURN_IF_ERR(io_potentiometer_readPercentage(&rsm_pot, RPUMP_WIPER, &data));
    // *dest = (1.0f - (float)data / MAX_WIPER_VALUE) * 100.0f;
    // return true;
    return std::unexpected(ErrorCode::UNIMPLEMENTED);
};
} // namespace io::rPump
