#include "io_pump.hpp"
#include "hw_i2cs.hpp"

namespace io
{
result<void> Pump::setPercentage(uint8_t percent) const
{
    const uint8_t hw_percent = logicalToHw(invert_, percent);
    return pot_.writePercentage(hw_percent);
}

result<uint8_t> Pump::getPercentage() const
{
    uint8_t hw_percent = 0u;
    if (auto status = pot_.readPercentage(hw_percent); !status)
    {
        return std::unexpected(status.error());
    }

    return hwToLogical(invert_, hw_percent);
}

result<void> Pump::enable(bool enable) const
{
    efuse_.setChannel(enable);
    return {};
}

result<bool> Pump::isEnabled() const
{
    return efuse_.isChannelEnabled();
}

result<bool> Pump::ok() const
{
    return efuse_.ok();
}

result<bool> Pump::isReady() const
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