#include "io_rPump.hpp"
#include "hw_i2cs.hpp"
#include "util_errorCodes.hpp"

namespace io::rPump
{
const Potentiometer rPump{ r_pump, POTENTIOMETER_WIPER::WIPER0 };

std::expected<void, ErrorCode> isPumpReady()
{
    return r_pump.isTargetReady();
}

std::expected<void, ErrorCode> setPercentage(const uint8_t percentage)
{
    return rPump.writePercentage(percentage);
}

std::expected<uint8_t, ErrorCode> readPercentage()
{
    return rPump.readPercentage();
}
} // namespace io::rPump
