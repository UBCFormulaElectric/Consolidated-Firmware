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
}
namespace io
{
std::expected<void, ErrorCode> pump::setPercentage(uint8_t percentage) const
{
    fakes::io::pumpController::pump_percentage = percentage;
    return {};
}

std::expected<uint8_t, ErrorCode> pump::getPercentage() const
{
    return fakes::io::pumpController::pump_percentage;
}

std::expected<void, ErrorCode> pump::enable(bool enable) const
{
    fakes::io::pumpController::pumps_enabled_state = enable;
    return {};
}

std::expected<bool, ErrorCode> pump::isEnabled() const
{
    return fakes::io::pumpController::pumps_enabled_state;
}

std::expected<bool, ErrorCode> pump::ok() const
{
    return fakes::io::pumpController::pumps_ok_state;
}

std::expected<bool, ErrorCode> pump::isReady() const
{
    return fakes::io::pumpController::pumps_ok_state && fakes::io::pumpController::pumps_enabled_state;
}
} // namespace io
