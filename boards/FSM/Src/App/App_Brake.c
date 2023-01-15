#include <stdlib.h>
#include <assert.h>

#include "App_InRangeCheck.h"
#include "App_Brake.h"
#include "App_SharedSetPeriodicCanSignals.h"
#include "configs/App_BrakePressureThresholds.h"

// STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(FsmCanTxInterface)

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
    // TODO: JSONCAN -> App_CanTx_SetPeriodicSignal_BRAKE_PEDAL_PERCENTAGE(can_tx, brake->get_pedal_travel());

    // Brake Actuation Value TODO: JSONCAN ->
    // uint8_t CANMSGS_FSM_BRAKE_BRAKE_IS_ACTUATED = brake->is_brake_actuated()
    //                                                  ? CANMSGS_FSM_BRAKE_FLAGS_BRAKE_IS_ACTUATED_TRUE_CHOICE
    //                                                  : CANMSGS_FSM_BRAKE_FLAGS_BRAKE_IS_ACTUATED_FALSE_CHOICE;
    // App_CanTx_SetPeriodicSignal_BRAKE_IS_ACTUATED(can_tx, CANMSGS_FSM_BRAKE_BRAKE_IS_ACTUATED);

    // Pressure Values, with range check TODO: JSONCAN ->
    // App_SetPeriodicCanSignals_InRangeCheck(
    //    can_tx, brake->front_pressure_in_range_check, App_CanTx_SetPeriodicSignal_FRONT_BRAKE_PRESSURE,
    //    App_CanTx_SetPeriodicSignal_BRAKE_FRONT_PRESSURE_OUT_OF_RANGE,
    //    CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_FRONT_PRESSURE_OUT_OF_RANGE_OK_CHOICE,
    //    CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_FRONT_PRESSURE_OUT_OF_RANGE_UNDER_CHOICE,
    //    CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_FRONT_PRESSURE_OUT_OF_RANGE_OVER_CHOICE);
    // App_SetPeriodicCanSignals_InRangeCheck(
    //    can_tx, brake->rear_pressure_in_range_check, App_CanTx_SetPeriodicSignal_REAR_BRAKE_PRESSURE,
    //    App_CanTx_SetPeriodicSignal_BRAKE_REAR_PRESSURE_OUT_OF_RANGE,
    //    CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_REAR_PRESSURE_OUT_OF_RANGE_OK_CHOICE,
    //    CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_REAR_PRESSURE_OUT_OF_RANGE_UNDER_CHOICE,
    //    CANMSGS_FSM_NON_CRITICAL_ERRORS_BRAKE_REAR_PRESSURE_OUT_OF_RANGE_OVER_CHOICE);

    // Brake Pressure OCSC TODO: JSONCAN ->
    // uint8_t CANMSGS_FSM_BRAKE_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT =
    //    App_Brake_PressureElectricalFault(brake)
    //        ? CANMSGS_FSM_BRAKE_FLAGS_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT_TRUE_CHOICE
    //        : CANMSGS_FSM_BRAKE_FLAGS_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT_FALSE_CHOICE;
    // App_CanTx_SetPeriodicSignal_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT(
    //    can_tx, CANMSGS_FSM_BRAKE_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT);

    // Brake Pedal OCSC TODO: JSONCAN ->
    // uint8_t CANMSGS_FSM_BRAKE_PEDAL_IS_OPEN_OR_SHORT_CIRCUIT =
    //    brake->pedal_travel_sensor_ocsc() ? CANMSGS_FSM_BRAKE_FLAGS_PEDAL_IS_OPEN_OR_SHORT_CIRCUIT_TRUE_CHOICE
    //                                      : CANMSGS_FSM_BRAKE_FLAGS_PEDAL_IS_OPEN_OR_SHORT_CIRCUIT_FALSE_CHOICE;
    // App_CanTx_SetPeriodicSignal_PEDAL_IS_OPEN_OR_SHORT_CIRCUIT(
    //    can_tx, CANMSGS_FSM_BRAKE_PEDAL_IS_OPEN_OR_SHORT_CIRCUIT);
    // if (brake->pedal_travel_sensor_ocsc())
    //    App_CanTx_SetPeriodicSignal_BRAKE_PEDAL_PERCENTAGE(can_tx, 0);
}
