#include "app_brake.h"
#include "app_rangeCheck.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_brake.h"
#include <math.h>

static const RangeCheck front_pressure_in_range_check = { .min_value = MIN_BRAKE_PRESSURE_PSI,
                                                          .max_value = MAX_BRAKE_PRESSURE_PSI };
static const RangeCheck rear_pressure_in_range_check  = { .min_value = MIN_BRAKE_PRESSURE_PSI,
                                                          .max_value = MAX_BRAKE_PRESSURE_PSI };

void app_brake_broadcast(void)
{
    const bool brake_pressed = io_brake_isActuated();
    app_canTx_FSM_BrakeActuated_set(brake_pressed);

    float                    front_pressure = io_brake_getFrontPressurePsi();
    RangeCheckStatusMetaData front_pressure_status =
        app_rangeCheck_getValue(&front_pressure_in_range_check, front_pressure);
    app_canTx_FSM_FrontBrakePressure_set((uint32_t)roundf(front_pressure));
    app_canAlerts_FSM_Warning_FrontBrakePressureOutOfRange_set(front_pressure_status.status != VALUE_IN_RANGE);

    float                    rear_pressure = io_brake_getRearPressurePsi();
    RangeCheckStatusMetaData rear_pressure_status =
        app_rangeCheck_getValue(&rear_pressure_in_range_check, rear_pressure);
    app_canTx_FSM_RearBrakePressure_set((uint32_t)roundf(rear_pressure));
    app_canAlerts_FSM_Warning_RearBrakePressureOutOfRange_set(rear_pressure_status.status != VALUE_IN_RANGE);

    app_canAlerts_FSM_Warning_FrontBrakePressureOcSc_set(io_brake_frontPressureSensorOCSC());
    app_canAlerts_FSM_Warning_RearBrakePressureOcSc_set(io_brake_rearPressureSensorOCSC());
    app_canAlerts_FSM_Warning_BrakeOcScNotOk_set(io_brake_hwOCSC());
}
