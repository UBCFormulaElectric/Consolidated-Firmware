#include "io/brake.h"
#include "app/brake.h"
#include "app_canTx.h"

#include <math.h>

void app_brake_broadcast(void)
{
    const bool  brake_pressed = io_brake_isActuated();
    const float rear_pressure = io_brake_getRearPressurePsi();

    app_canTx_RSM_BrakeActuated_set(brake_pressed);
    app_canTx_RSM_RearBrakePressure_set((uint32_t)roundf(rear_pressure));

    app_canTx_RSM_Info_RearBrakePressureOutOfRange_set(
        (rear_pressure >= MAX_BRAKE_PRESSURE_PSI) || (rear_pressure < MIN_BRAKE_PRESSURE_PSI));
}
