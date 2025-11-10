#include "app_brake.hpp"
#include "io_brake.hpp"
extern "C"
{
#include "app_canTx.h"
#include "app_canAlerts.h"
}

#include <cmath>
namespace app::brake{
    void broadcast(void)
    {
        const bool  brake_pressed             = io::brake::isActuated();
        const float front_pressure            = io::brake::getFrontPressurePsi();
        const bool  front_brake_pressure_ocsc = io::brake::OCSC();

        app_canTx_FSM_BrakeActuated_set(brake_pressed);
        app_canTx_FSM_FrontBrakePressure_set((uint32_t)roundf(front_pressure));
        app_canAlerts_FSM_Info_FrontBrakePressureOCSC_set(front_brake_pressure_ocsc);
        app_canTx_FSM_Info_FrontBrakePressureOutOfRange_set(
        (front_pressure >= MAX_BRAKE_PRESSURE_PSI) || (front_pressure < MIN_BRAKE_PRESSURE_PSI));
    }
}