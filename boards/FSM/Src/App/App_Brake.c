#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "App_InRangeCheck.h"
#include "App_Brake.h"
#include "App_SharedSetPeriodicCanSignals.h"
#include "configs/App_BrakePressureThresholds.h"

STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(FsmCanTxInterface)

struct Brake
{
    struct InRangeCheck *pressure_in_range_check;
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

    brake->pressure_in_range_check = App_InRangeCheck_Create(get_front_pressure_psi,
        MIN_BRAKE_PRESSURE_PSI,
        MAX_BRAKE_PRESSURE_PSI
    );
    brake->get_front_pressure_psi = get_front_pressure_psi;
    brake->get_rear_pressure_psi  = get_rear_pressure_psi;
    brake->get_pedal_travel       = get_pedal_travel;
    brake->is_brake_actuated      = is_brake_actuated;

    brake->front_pressure_sensor_ocsc = front_pressure_sensor_ocsc;
    brake->rear_pressure_sensor_ocsc = rear_pressure_sensor_ocsc;
    brake->pedal_travel_sensor_ocsc = pedal_travel_sensor_ocsc;

    return brake;
}
void App_Brake_Destroy(struct Brake *brake)
{
    App_InRangeCheck_Destroy(brake->pressure_in_range_check);
    free(brake);
}

//act
bool App_Brake_IsBrakeActuated(const struct Brake *brake)
{
    return brake->is_brake_actuated();
}

//pressures
float App_Brake_GetFrontPSI(const struct Brake *brake)
{
    return brake->get_front_pressure_psi();
}
float App_Brake_GetRearPSI(const struct Brake *brake)
{
    return brake->get_rear_pressure_psi();
}
bool App_Brake_AllPressureElectricalFault(const struct Brake *brake){
    return brake->front_pressure_sensor_ocsc() || brake->rear_pressure_sensor_ocsc();
}
struct InRangeCheck *App_Brake_GetPressureInRangeCheck(const struct Brake *const brake)
{
    return brake->pressure_in_range_check;
}

//pedal
float App_Brake_GetPedalTravel(const struct Brake * brake){
    return brake->get_pedal_travel();
}
bool App_Brake_PedalSensorAlarm(const struct Brake *brake)
{
    return brake->pedal_travel_sensor_ocsc;
}

//broadcast
void App_Brake_Broadcast(const struct FsmWorld * world)
{
    struct FsmCanTxInterface *can_tx = App_FsmWorld_GetCanTx(world);
    struct Brake *brake = App_FsmWorld_GetBrake(world);

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
        App_Brake_AllPressureElectricalFault(brake)
            ? CANMSGS_FSM_BRAKE_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT_TRUE_CHOICE
            : CANMSGS_FSM_BRAKE_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT_FALSE_CHOICE;
    App_CanTx_SetPeriodicSignal_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT(
        can_tx, CANMSGS_FSM_BRAKE_PRESSURE_SENSOR_IS_OPEN_OR_SHORT_CIRCUIT);

    // TODO CAN message for when travel sensor is short or open circuit
    App_Brake_PedalSensorAlarm(brake);
}
