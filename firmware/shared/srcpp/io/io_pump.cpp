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

bool pump::isReady() const
{
    if (!efuse_.ok())
    {
        return false;
    }
    if (!efuse_.isChannelEnabled())
    {
        return false;
    }
    return true;
}

} // namespace io