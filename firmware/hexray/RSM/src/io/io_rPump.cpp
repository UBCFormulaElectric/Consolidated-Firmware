#include "io_rPump.hpp"
#include "hw_i2cs.hpp"
#include "util_errorCodes.hpp"

namespace io::rPump
{
const Potentiometer rPump{ r_pump, io::POTENTIOMETER_WIPER::WIPER0 };

result<void> isPumpReady()
{
    return r_pump.isTargetReady();
}

result<void> setPercentage(uint8_t percentage)
{
    return rPump.writePercentage(percentage);
}

result<void> readPercentage(uint8_t &dest)
{
    return rPump.readPercentage(dest);
}
} // namespace io::rPump
