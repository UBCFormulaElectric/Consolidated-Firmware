#include "io_pump.hpp"

namespace io::pumpController
{
extern bool    pumps_ok_state;
extern bool    pumps_enabled_state;
extern uint8_t pump_percentage;
} // namespace io::pumpController

namespace io
{
std::expected<void, ErrorCode> Pump::setPercentage(uint8_t percentage) const
{
    pumpController::pump_percentage = percentage;
    return {};
}

std::expected<uint8_t, ErrorCode> Pump::getPercentage() const
{
    return pumpController::pump_percentage;
}

std::expected<void, ErrorCode> Pump::enable(bool enable) const
{
    pumpController::pumps_enabled_state = enable;
    return {};
}

std::expected<bool, ErrorCode> Pump::isEnabled() const
{
    return pumpController::pumps_enabled_state;
}

std::expected<bool, ErrorCode> Pump::ok() const
{
    return pumpController::pumps_ok_state;
}

std::expected<bool, ErrorCode> Pump::isReady() const
{
    return pumpController::pumps_ok_state && pumpController::pumps_enabled_state;
}
} // namespace io
