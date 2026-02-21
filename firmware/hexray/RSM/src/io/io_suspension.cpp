#include "io_suspension.hpp"

constexpr float METERS_PER_VOLT = 100.0f;

static float travelFromVoltage(const float voltage)
{
    return voltage / METERS_PER_VOLT;
}

namespace io::suspension
{
float getRLTravel()
{
    return travelFromVoltage(hw::adcs::susp_travel_rl_3v3.getVoltage());
}

float getRRTravel()
{
    return travelFromVoltage(hw::adcs::susp_travel_rr_3v3.getVoltage());
}

bool RR_OCSC()
{
    return hw::gpio::susp_travel_rr_ocsc.readPin();
}

bool RL_OCSC()
{
    return hw::gpio::susp_travel_rl_ocsc.readPin();
}
} // namespace io::suspension
