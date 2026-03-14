#include "app_brake.hpp"
#include "io_brake.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"
#include <cmath>

namespace app::brake
{
void broadcast()
{
    static constexpr float MIN_BRAKE_PRESSURE_PSI = 0.0f;
    static constexpr float MAX_BRAKE_PRESSURE_PSI = 1000.0f;

    const bool  brake_pressed             = io::brake::isActuated();
    const float front_pressure            = io::brake::getFrontPressurePsi();
    const bool  front_brake_pressure_ocsc = io::brake::OCSC();

    can_tx::FSM_BrakeActuated_set(brake_pressed);
    can_tx::FSM_FrontBrakePressure_set(static_cast<uint16_t>(roundf(front_pressure)));
    can_alerts::infos::FrontBrakePressureOCSC_set(front_brake_pressure_ocsc);
    can_tx::FSM_Info_FrontBrakePressureOutOfRange_set(
        front_pressure >= MAX_BRAKE_PRESSURE_PSI || front_pressure < MIN_BRAKE_PRESSURE_PSI);
}
} // namespace app::brake