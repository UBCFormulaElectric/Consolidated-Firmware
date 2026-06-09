#include "io_rPump.hpp"
#include "hw_i2cs.hpp"
#include "util_errorCodes.hpp"

namespace io::rPump
{
const Potentiometer rPump{ r_pump, POTENTIOMETER_WIPER::WIPER0 };

result<void> ready()
{
    return r_pump.isTargetReady();
}

result<void> setPercentage(const uint8_t percentage)
{
    return rPump.writePercentage(percentage);
}

result<uint8_t> readPercentage()
{
    return rPump.readPercentage();
}
} // namespace io::rPump
