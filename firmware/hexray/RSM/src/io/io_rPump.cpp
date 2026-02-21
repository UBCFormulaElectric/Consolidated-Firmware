#include "io_rPump.hpp"

namespace io::rPump
{
const Potentiometer rPump{ hw::i2c::r_pump, io::POTENTIOMETER_WIPER::WIPER0 };

std::expected<void, ErrorCode> isPumpReady()
{
    return hw::i2c::r_pump.isTargetReady();
}

std::expected<void, ErrorCode> setPercentage(uint8_t percentage)
{
    return rPump.writePercentage(percentage);
}

std::expected<void, ErrorCode> readPercentage(uint8_t &dest)
{
    return rPump.readPercentage(dest);
};
} // namespace io::rPump
