#include <stdlib.h>
#include <assert.h>

#include "App_InRangeCheck.h"
#include "App_Brake.h"
#include "App_SharedSetPeriodicCanSignals.h"
#include "App_SharedSetPeriodicCanSignals.h"
#include "configs/App_BrakePressureThresholds.h"

struct Brake
{
    struct InRangeCheck *front_pressure_in_range_check;
    struct InRangeCheck *rear_pressure_in_range_check;
    float (*get_front_pressure_psi)(void);
    float (*get_rear_pressure_psi)(void);

    float (*get_pedal_travel)(void);
    bool (*is_brake_actuated)(void);

    bool (*front_pressure_sensor_ocsc)(void);
    bool (*rear_pressure_sensor_ocsc)(void);
    bool (*pedal_travel_sensor_ocsc)(void);
};

struct Brake *App_Brake_Create(
    float (*get_front_pressure_psi)(void),
    bool (*front_pressure_sensor_ocsc)(void),
    float (*get_rear_pressure_psi)(void),
    bool (*rear_pressure_sensor_ocsc)(void),
    float (*get_pedal_travel)(void),
    bool (*pedal_travel_sensor_ocsc)(void),
    bool (*is_brake_actuated)(void))
{
    struct Brake *brake = malloc(sizeof(struct Brake));
    assert(brake != NULL);

    brake->front_pressure_in_range_check =
        App_InRangeCheck_Create(get_front_pressure_psi, MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI);
    brake->rear_pressure_in_range_check =
        App_InRangeCheck_Create(get_rear_pressure_psi, MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI);
    brake->get_front_pressure_psi = get_front_pressure_psi;
    brake->get_rear_pressure_psi  = get_rear_pressure_psi;
    brake->get_pedal_travel       = get_pedal_travel;
    brake->is_brake_actuated      = is_brake_actuated;

    brake->front_pressure_sensor_ocsc = front_pressure_sensor_ocsc;
    brake->rear_pressure_sensor_ocsc  = rear_pressure_sensor_ocsc;
    brake->pedal_travel_sensor_ocsc   = pedal_travel_sensor_ocsc;

    return brake;
}
void App_Brake_Destroy(struct Brake *brake)
{
    App_InRangeCheck_Destroy(brake->front_pressure_in_range_check);
    App_InRangeCheck_Destroy(brake->rear_pressure_in_range_check);
    free(brake);
}

// act
bool App_Brake_IsBrakeActuated(const struct Brake *brake)
{
    return brake->is_brake_actuated();
}

// pressures
float App_Brake_GetFrontPSI(const struct Brake *brake)
{
    return brake->get_front_pressure_psi();
}
float App_Brake_GetRearPSI(const struct Brake *brake)
{
    return brake->get_rear_pressure_psi();
}
bool App_Brake_PressureElectricalFault(const struct Brake *brake)
{
    return brake->front_pressure_sensor_ocsc() || brake->rear_pressure_sensor_ocsc();
}
struct InRangeCheck *App_Brake_GetPressureInRangeCheck(const struct Brake *const brake)
{
    return brake->front_pressure_in_range_check;
}

// pedal
float App_Brake_GetPedalTravel(const struct Brake *brake)
{
    return brake->get_pedal_travel();
}
bool App_Brake_PedalSensorAlarm(const struct Brake *brake)
{
    return brake->pedal_travel_sensor_ocsc();
}

// broadcast
void App_Brake_Broadcast(const struct FsmWorld *world)
{
    struct Brake *brake = App_FsmWorld_GetBrake(world);

    // Brake Pedal Value
    App_CanTx_FSM_Brake_BrakePedalPercentage_Set(brake->get_pedal_travel());

    App_CanTx_FSM_Brake_IsActuated_Set(brake->is_brake_actuated());

    App_SetPeriodicCanSignals_InRangeCheck_long(
        brake->front_pressure_in_range_check, App_CanTx_FSM_Brake_FrontBrakePressure_Set,
        (void (*)(uint8_t))App_CanTx_FSM_Warning_FrontBrakePressureOutOfRange_Set);
    App_SetPeriodicCanSignals_InRangeCheck_long(
        brake->rear_pressure_in_range_check, App_CanTx_FSM_Brake_RearBrakePressure_Set,
        (void (*)(uint8_t))App_CanTx_FSM_Warning_RearBrakePressureOutOfRange_Set);

    App_CanTx_FSM_Brake_PressureSensorOpenShortCircuit_Set(App_Brake_PressureElectricalFault(brake));

    App_CanTx_FSM_Brake_PedalOpenShortCircuit_Set(brake->pedal_travel_sensor_ocsc());
    if (brake->pedal_travel_sensor_ocsc())
    {
        App_CanTx_FSM_Brake_BrakePedalPercentage_Set(0);
    }
}
