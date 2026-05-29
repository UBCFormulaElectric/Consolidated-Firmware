#include "io_pump.hpp"

namespace io
{
result<void> pump::setPercentage(uint8_t percentage) const
{
    (void)percentage;
    return {};
}

result<uint8_t> pump::getPercentage() const
{
    return 0;
}

result<void> pump::enable(bool enable) const
{
    (void)enable;
    return {};
}

result<bool> pump::isEnabled() const
{
    return true;
}

result<bool> pump::ok() const
{
    return true;
}

result<bool> pump::isReady() const
{
    return true;
}
} // namespace io