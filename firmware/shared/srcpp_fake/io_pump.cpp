#include "io_pump.hpp"

namespace io
{
std::expected<void, ErrorCode> Pump::setPercentage(uint8_t percentage) const
{
    (void)percentage;
    return {};
}

std::expected<uint8_t, ErrorCode> Pump::getPercentage() const
{
    return 0;
}

std::expected<void, ErrorCode> Pump::enable(bool enable) const
{
    (void)enable;
    return {};
}

std::expected<bool, ErrorCode> Pump::isEnabled() const
{
    return true;
}

std::expected<bool, ErrorCode> Pump::ok() const
{
    return true;
}

std::expected<bool, ErrorCode> Pump::isReady() const
{
    return true;
}
} // namespace io