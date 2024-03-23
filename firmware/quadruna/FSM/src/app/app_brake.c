#include "app_brake.h"
#include <stdlib.h>
#include <assert.h>
#include "app_rangeCheck.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_brake.h"

static const RangeCheck front_pressure_in_range_check = { .get_value = io_brake_getFrontPressurePsi,
                                                          .min_value = MIN_BRAKE_PRESSURE_PSI,
                                                          .max_value = MAX_BRAKE_PRESSURE_PSI };
static const RangeCheck rear_pressure_in_range_check  = { .get_value = io_brake_getRearPressurePsi,
                                                          .min_value = MIN_BRAKE_PRESSURE_PSI,
                                                          .max_value = MAX_BRAKE_PRESSURE_PSI };

void app_brake_broadcast()
{
    const bool brake_pressed = io_brake_isActuated();
    app_canTx_FSM_BrakeActuated_set(brake_pressed);

    float            front_pressure;
    RangeCheckStatus front_pressure_status = app_rangeCheck_getValue(&front_pressure_in_range_check, &front_pressure);
    app_canTx_FSM_FrontBrakePressure_set((uint32_t)front_pressure);
    app_canAlerts_FSM_Warning_FrontBrakePressureOutOfRange_set(front_pressure_status != VALUE_IN_RANGE);

    float            rear_pressure;
    RangeCheckStatus rear_pressure_status = app_rangeCheck_getValue(&rear_pressure_in_range_check, &rear_pressure);
    app_canTx_FSM_RearBrakePressure_set((uint32_t)rear_pressure);
    app_canAlerts_FSM_Warning_RearBrakePressureOutOfRange_set(rear_pressure_status != VALUE_IN_RANGE);

    const bool brake_pressure_ocsc = io_brake_frontPressureSensorOCSC() || io_brake_rearPressureSensorOCSC();
    app_canAlerts_FSM_Warning_BrakePressureSensorOCSC_set(brake_pressure_ocsc);
}
