#include "io_suspension.hpp"
#include "hw_adcs.hpp"
#include "hw_gpios.hpp"

constexpr float METERS_PER_VOLT = 100.0f;

static float travelFromVoltage(const float voltage)
{
    return voltage / METERS_PER_VOLT;
}

namespace io::suspension
{
float getRLTravel()
{
    return travelFromVoltage(susp_travel_rl_3v3.getVoltage());
}

float getRRTravel()
{
    return travelFromVoltage(susp_travel_rr_3v3.getVoltage());
}

bool RR_OCSC()
{
    return susp_travel_rr_ocsc.readPin();
}

bool RL_OCSC()
{
    return susp_travel_rl_ocsc.readPin();
}
} // namespace io::suspension
