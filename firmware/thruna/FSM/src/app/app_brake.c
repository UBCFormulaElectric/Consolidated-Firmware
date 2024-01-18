#include "app_brake.h"
#include <stdlib.h>
#include <assert.h>
#include "app_rangeCheck.h"
#include "App_CanTx.h"
#include "App_CanAlerts.h"
#include "io_brake.h"

static const RangeCheck front_pressure_in_range_check = { .get_value = io_brake_getFrontPressurePsi,
                                                          .min_value = MIN_BRAKE_PRESSURE_PSI,
                                                          .max_value = MAX_BRAKE_PRESSURE_PSI };
static const RangeCheck rear_pressure_in_range_check  = { .get_value = io_brake_getRearPressurePsi,
                                                         .min_value = MIN_BRAKE_PRESSURE_PSI,
                                                         .max_value = MAX_BRAKE_PRESSURE_PSI };

void app_brake_broadcast()
{
    const float brake_pedal_percentage = io_brake_getPedalPercentTravel();
    App_CanTx_FSM_BrakePedalPercentage_Set(brake_pedal_percentage);

    const bool brake_pressed = io_brake_isActuated();
    App_CanTx_FSM_BrakeActuated_Set(brake_pressed);

    float            front_pressure;
    RangeCheckStatus front_pressure_status = app_rangeCheck_getValue(&front_pressure_in_range_check, &front_pressure);
    App_CanTx_FSM_FrontBrakePressure_Set((uint32_t)front_pressure);
    App_CanAlerts_FSM_Warning_FrontBrakePressureOutOfRange_Set(front_pressure_status != VALUE_IN_RANGE);

    float            rear_pressure;
    RangeCheckStatus rear_pressure_status = app_rangeCheck_getValue(&rear_pressure_in_range_check, &rear_pressure);
    App_CanTx_FSM_RearBrakePressure_Set((uint32_t)rear_pressure);
    App_CanAlerts_FSM_Warning_RearBrakePressureOutOfRange_Set(rear_pressure_status != VALUE_IN_RANGE);

    const bool brake_pressure_ocsc = io_brake_frontPressureSensorOCSC() || io_brake_rearPressureSensorOCSC();
    App_CanTx_FSM_BrakePressureSensorOCSC_Set(brake_pressure_ocsc);
    App_CanTx_FSM_BrakePedalSensorOCSC_Set(io_brake_pedalSensorOCSC());

    if (io_brake_pedalSensorOCSC())
    {
        App_CanTx_FSM_BrakePedalPercentage_Set(0);
    }
}
