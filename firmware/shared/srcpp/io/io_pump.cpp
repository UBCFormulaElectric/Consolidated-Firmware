#include "io_pump.hpp"

namespace io
{
result<void> pump::setPercentage(const uint8_t percentage) const
{
    const uint8_t hw_percent = logicalToHw(invert_, percentage);
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
} // namespace io