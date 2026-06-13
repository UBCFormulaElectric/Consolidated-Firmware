#include "io_pump.hpp"
#include "io_pumpFake.hpp"

namespace fakes::io::rPump
{
uint8_t percentage = 0;

void set_readPercentage(uint8_t value)
{
    percentage = value;
}

uint8_t get_readPercentage()
{
    return percentage;
}
} // namespace fakes::io::rPump

namespace io
{
result<void> pump::setPercentage(uint8_t percentage) const
{
    (void)percentage;
    return {};
}

result<uint8_t> pump::getPercentage() const
{
    return fakes::io::rPump::get_readPercentage();
}
} // namespace io