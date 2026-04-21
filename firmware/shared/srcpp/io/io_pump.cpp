#include "io_pump.hpp"
#include "hw_i2cs.hpp"

namespace io
{
std::expected<void, ErrorCode> Pump::setPercentage(uint8_t percent) const
{
    const uint8_t hw_percent = logicalToHw(invert_, percent);
    return pot_.writePercentage(hw_percent);
}

std::expected<uint8_t, ErrorCode> Pump::getPercentage() const
{
    uint8_t hw_percent = 0u;
    if (auto status = pot_.readPercentage(hw_percent); !status)
    {
        return std::unexpected(status.error());
    }

    return hwToLogical(invert_, hw_percent);
}

std::expected<void, ErrorCode> Pump::enable(bool enable) const
{
    efuse_.setChannel(enable);
    return {};
}

std::expected<bool, ErrorCode> Pump::isEnabled() const
{
    return efuse_.isChannelEnabled();
}

std::expected<bool, ErrorCode> Pump::ok() const
{
    return efuse_.ok();
}

std::expected<bool, ErrorCode> Pump::isReady() const
{
    auto healthy = ok();
    if (!healthy)
    {
        return std::unexpected(healthy.error());
    }
    auto enabled = isEnabled();
    if (!enabled)
    {
        return std::unexpected(enabled.error());
    }
    return *healthy && *enabled;
}

} // namespace io