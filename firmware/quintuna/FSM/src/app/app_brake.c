#include "app_brake.h"

#include "io_brake.h"
#include "app_canTx.h"
#include "app_canAlerts.h"

#include <math.h>

void app_brake_broadcast(void)
{
    const bool  brake_pressed             = io_brake_isActuated();
    const float front_pressure            = io_brake_getFrontPressurePsi();
    const bool  front_brake_pressure_ocsc = io_brake_OCSC();

    app_canTx_FSM_BrakeActuated_set(brake_pressed);
    app_canTx_FSM_FrontBrakePressure_set((uint32_t)roundf(front_pressure));
    app_canAlerts_FSM_Info_FrontBrakePressureOCSC_set(front_brake_pressure_ocsc);
}
