#include "app_brake.h"
#include <stdlib.h>
#include <assert.h>
#include "io_brake.h"

#define BRAKE_PRESSED_PRESSURE_THRESHOLD_PSI (40.0f)
#define MIN_BRAKE_PRESSURE_PSI (0.0f)
#define MAX_BRAKE_PRESSURE_PSI (1000.0f)

static struct InRangeCheck front_pressure_in_range_check;
static struct InRangeCheck rear_pressure_in_range_check;

void app_brake_init()
{
    front_pressure_in_range_check = App_InRangeCheck_Create(io_brake_getFrontPressurePsi, MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI);
    rear_pressure_in_range_check = App_InRangeCheck_Create(io_brake_getRearPressurePsi, MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI);
}

void app_brake_broadcast()
{
    const float brake_pedal_percentage = io_brake_getPedalPercentTravel();
    App_CanTx_FSM_Brake_BrakePedalPercentage_Set(brake_pedal_percentage);

    const bool brake_pressed = io_brake_isActuated();
    App_CanTx_FSM_Brake_IsActuated_Set(brake_pressed);

    float                    front_pressure;
    enum InRangeCheck_Status front_pressure_status =
        App_InRangeCheck_GetValue(front_pressure_in_range_check, &front_pressure);
    App_CanTx_FSM_Brake_FrontBrakePressure_Set((uint32_t)front_pressure);
    App_CanAlerts_SetWarning(FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE, front_pressure_status != VALUE_IN_RANGE);

    float                    rear_pressure;
    enum InRangeCheck_Status rear_pressure_status =
        App_InRangeCheck_GetValue(rear_pressure_in_range_check, &rear_pressure);
    App_CanTx_FSM_Brake_RearBrakePressure_Set((uint32_t)rear_pressure);
    App_CanAlerts_SetWarning(FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE, rear_pressure_status != VALUE_IN_RANGE);

    const bool brake_pressure_ocsc = io_brake_frontPressureSensorOCSC() || io_brake_getRearPressurePsi();
    App_CanTx_FSM_Brake_PressureSensorOpenShortCircuit_Set(App_Brake_PressureElectricalFault(brake));
    App_CanTx_FSM_Brake_PedalOpenShortCircuit_Set(brake->pedal_travel_sensor_ocsc());

    if (brake->pedal_travel_sensor_ocsc())
    {
        App_CanTx_FSM_Brake_BrakePedalPercentage_Set(0);
    }
}
