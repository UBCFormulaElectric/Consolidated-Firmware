#include "io_pump.hpp"
#include "io_pumpControllerFake.hpp"

namespace fakes::io::pumpController
{
bool    pumps_ok_state      = true;
bool    pumps_enabled_state = true;
uint8_t pump_percentage     = 0;

void set_pumps_ok(bool ok)
{
    pumps_ok_state = ok;
}

void set_pumps_enable(bool enabled)
{
    pumps_enabled_state = enabled;
}
} // namespace fakes::io::pumpController
namespace io
{
std::expected<void, ErrorCode> pump::setPercentage(const uint8_t percentage) const
{
    fakes::io::pumpController::pump_percentage = percentage;
    return {};
}

std::expected<uint8_t, ErrorCode> pump::getPercentage() const
{
    return fakes::io::pumpController::pump_percentage;
}
} // namespace io
