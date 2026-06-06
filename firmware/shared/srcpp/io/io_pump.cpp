#include "io_pump.hpp"
#include "hw_i2cs.hpp"

namespace io
{
result<void> pump::setPercentage(uint8_t percent) const
{
    const uint8_t hw_percent = logicalToHw(invert_, percent);
    return pot_.writePercentage(hw_percent);
}

result<uint8_t> pump::getPercentage() const
{
    const auto percentage_res = pot_.readPercentage();
    if (!percentage_res)
    {
        return std::unexpected(percentage_res.error());
    }

    return hwToLogical(invert_, percentage_res.value());
}

result<void> pump::enable(bool enable) const
{
    efuse_.setChannel(enable);
    return {};
}

result<bool> pump::isEnabled() const
{
    return efuse_.isChannelEnabled();
}

result<bool> pump::ok() const
{
    return efuse_.ok();
}

result<bool> pump::isReady() const
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