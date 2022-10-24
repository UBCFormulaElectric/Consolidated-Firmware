#include <stdlib.h>
#include <assert.h>

#include "App_InRangeCheck.h"
#include "App_Brake.h"
#include "App_SharedSetPeriodicCanSignals.h"
#include "configs/App_BrakePressureThresholds.h"

#define BRAKE_PRESSURE_SENSOR_MAX_V (5.0f)
#define BRAKE_PRESSURE_SC_THRESHOLD_V (4.6f)
#define BRAKE_PRESSURE_OC_THRESHOLD_V (0.4f)
#define BRAKE_PRESSURE_SC_THRESHOLD \
    (MAX_BRAKE_PRESSURE_PSI * BRAKE_PRESSURE_SC_THRESHOLD_V / BRAKE_PRESSURE_SENSOR_MAX_V)
#define BRAKE_PRESSURE_OC_THRESHOLD \
    (MAX_BRAKE_PRESSURE_PSI * BRAKE_PRESSURE_OC_THRESHOLD_V / BRAKE_PRESSURE_SENSOR_MAX_V)

STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(FsmCanTxInterface)

struct Brake
{
    struct InRangeCheck *pressure_in_range_check;
    float (*get_primary_pressure_psi)(void);
    float (*get_secondary_pressure_psi)(void);
    bool (*is_primary_pressure_sensor_open_or_short_circuit)(void);
    bool (*is_secondary_pressure_sensor_open_or_short_circuit)(void);
    float (*pedal_angle)(void);
    bool (*is_pedal_angle_sensor_open_or_short_circuit)(void);
    bool (*is_brake_actuated)(void);
};

struct Brake *App_Brake_Create(
    float (*get_primary_pressure_psi)(void),
    float (*get_secondary_pressure_psi)(void),
    bool (*is_primary_pressure_sensor_open_or_short_circuit)(void),
    bool (*is_secondary_sensor_open_or_short_circuit)(void),
    float (*pedal_angle)(void),
    bool (*is_pedal_angle_sensor_open_or_short_circuit)(void),
    bool (*is_brake_actuated)(void))
{
    struct Brake *brake = malloc(sizeof(struct Brake));
    assert(brake != NULL);

    brake->pressure_in_range_check =
        App_InRangeCheck_Create(get_primary_pressure_psi, MIN_BRAKE_PRESSURE_PSI, MAX_BRAKE_PRESSURE_PSI);
    brake->get_primary_pressure_psi                           = get_primary_pressure_psi;
    brake->get_secondary_pressure_psi                         = get_secondary_pressure_psi;
    brake->is_primary_pressure_sensor_open_or_short_circuit   = is_primary_pressure_sensor_open_or_short_circuit;
    brake->is_secondary_pressure_sensor_open_or_short_circuit = is_secondary_sensor_open_or_short_circuit;
    brake->pedal_angle                                        = pedal_angle;
    brake->is_pedal_angle_sensor_open_or_short_circuit        = is_pedal_angle_sensor_open_or_short_circuit;
    brake->is_brake_actuated                                  = is_brake_actuated;

    return brake;
}
void App_Brake_Destroy(struct Brake *brake)
{
    App_InRangeCheck_Destroy(brake->pressure_in_range_check);
    free(brake);
}

struct InRangeCheck *App_Brake_GetPressureInRangeCheck(const struct Brake *const brake)
{
    return brake->pressure_in_range_check;
}
bool App_Brake_IsBrakeActuated(const struct Brake *brake)
{
    return brake->is_brake_actuated();
}
bool App_Brake_IsPressureSensorOpenOrShortCircuit(const struct Brake *brake)
{
    // TODO perhaps rethink how this is implemented
    return brake->is_primary_pressure_sensor_open_or_short_circuit() ||
           brake->is_secondary_pressure_sensor_open_or_short_circuit();
}
bool App_Brake_IsPressureSensorOpenCircuit(const struct Brake *brake)
{
    return brake->get_primary_pressure_psi() < BRAKE_PRESSURE_OC_THRESHOLD;
}
bool App_Brake_IsPressureSensorShortCircuited(const struct Brake *brake)
{
    return brake->get_primary_pressure_psi() > BRAKE_PRESSURE_SC_THRESHOLD;
}
// TODO Implement as signal??
bool App_Brake_IsPedalSensorOpenOrShortCircuit(const struct Brake *brake)
{
    return brake->is_pedal_angle_sensor_open_or_short_circuit();
}

void App_Brake_Broadcast(struct FsmCanTxInterface *can_tx, const struct Brake *brake)
{
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Brake_GetPressureInRangeCheck(brake), App_CanTx_SetPeriodicSignal_BRAKE_PRESSURE,
        App_CanTx_SetPeriodicSignal_BRAKE_PRESSURE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_PRESSURE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_PRESSURE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_PRESSURE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    uint8_t CANMSGS_FSM_BRAKE_BRAKE_IS_ACTUATED = App_Brake_IsBrakeActuated(brake)
                                                      ? CANMSGS_FSM_BRAKE_BRAKE_IS_ACTUATED_TRUE_CHOICE
                                                      : CANMSGS_FSM_BRAKE_BRAKE_IS_ACTUATED_FALSE_CHOICE;
    App_CanTx_SetPeriodicSignal_BRAKE_IS_ACTUATED(can_tx, CANMSGS_FSM_BRAKE_BRAKE_IS_ACTUATED);

    uint8_t CANMSGS_FSM_BRAKE_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT =
        App_Brake_IsPressureSensorOpenOrShortCircuit(brake)
            ? CANMSGS_FSM_BRAKE_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT_TRUE_CHOICE
            : CANMSGS_FSM_BRAKE_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT_FALSE_CHOICE;
    App_CanTx_SetPeriodicSignal_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT(
        can_tx, CANMSGS_FSM_BRAKE_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT);
}
