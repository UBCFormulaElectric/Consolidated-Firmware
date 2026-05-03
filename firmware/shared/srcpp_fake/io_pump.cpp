#include "io_pump.hpp"

namespace io
{
std::expected<void, ErrorCode> pump::setPercentage(uint8_t percentage) const
{
    (void)percentage;
    return {};
}

std::expected<uint8_t, ErrorCode> pump::getPercentage() const
{
    return 0;
}

std::expected<void, ErrorCode> pump::enable(bool enable) const
{
    (void)enable;
    return {};
}

std::expected<bool, ErrorCode> pump::isEnabled() const
{
    return true;
}

std::expected<bool, ErrorCode> pump::ok() const
{
    return true;
}

std::expected<bool, ErrorCode> pump::isReady() const
{
    return true;
}
} // namespace io