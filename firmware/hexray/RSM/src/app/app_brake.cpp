#include "app_brake.hpp"
#include "io_brake.hpp"
#include <cmath>
extern "C"
{
#include "app_canTx.h"
}

constexpr float MIN_BRAKE_PRESSURE_PSI = 0.0f;
constexpr float MAX_BRAKE_PRESSURE_PSI = 1000.0f;
constexpr float BRAKE_ACTUATED_THRESHOLD_PSI = 200.0f;

namespace app::brake
{
bool isActuated()
{
    return io::brake::getRearPressurePsi() > BRAKE_ACTUATED_THRESHOLD_PSI;
}

void broadcast()
{
    const float rear_pressure = io::brake::getRearPressurePsi();
    const bool  brake_pressed = app::brake::isActuated();

    app_canTx_RSM_BrakeActuated_set(brake_pressed);
    app_canTx_RSM_RearBrakePressure_set((uint32_t)roundf(rear_pressure));

    app_canTx_RSM_Info_RearBrakePressureOutOfRange_set(
        (rear_pressure >= MAX_BRAKE_PRESSURE_PSI) || (rear_pressure < MIN_BRAKE_PRESSURE_PSI));
}
} // namespace app::brake