#include <stdlib.h>
#include <assert.h>

#include "App_Coolant.h"
#include "configs/App_CoolantSignalTimings.h"
#include "App_InRangeCheck.h"
#include "configs/App_FlowRateThresholds.h"

#include "App_SharedSetPeriodicCanSignals.h"
// STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(FsmCanTxInterface)

struct Coolant
{
    float (*get_flow_rate)(void);
    struct InRangeCheck *flow_rate_in_range_check;

    float (*get_temperature_A)(void);
    float (*get_temperature_B)(void);
    float (*get_pressure_A)(void);
    float (*get_pressure_B)(void);

    struct Signal *flow_in_range_signal;
};

struct Coolant *App_Coolant_Create(
    float (*get_flow_rate)(void),
    float (*get_temperature_A)(void),
    float (*get_temperature_B)(void),
    float (*get_pressure_A)(void),
    float (*get_pressure_B)(void))
{
    struct Coolant *coolant = malloc(sizeof(struct Coolant));
    assert(coolant != NULL);

    // Flow rate
    coolant->get_flow_rate = get_flow_rate;
    coolant->flow_rate_in_range_check =
        App_InRangeCheck_Create(get_flow_rate, MIN_FLOW_RATE_L_PER_MIN, MAX_FLOW_RATE_L_PER_MIN);
    // Temperature
    coolant->get_temperature_A = get_temperature_A;
    coolant->get_temperature_B = get_temperature_B;
    // Pressure
    coolant->get_pressure_A = get_pressure_A;
    coolant->get_pressure_B = get_pressure_B;

    coolant->flow_in_range_signal = App_SharedSignal_Create(FLOW_METER_TIME_TO_FAULT, FLOW_METER_TIME_TO_CLEAR);

    return coolant;
}
void App_Coolant_Destroy(struct Coolant *coolant)
{
    App_InRangeCheck_Destroy(coolant->flow_rate_in_range_check);
    App_SharedSignal_Destroy(coolant->flow_in_range_signal);
    free(coolant);
}

void App_Coolant_Broadcast(const struct FsmWorld *world)
{
    struct Coolant *coolant = App_FsmWorld_GetCoolant(world);

    // Value Broadcast TODO: JSONCAN ->
    // App_CanTx_SetPeriodicSignal_TEMPERATURE_A(can_tx, coolant->get_temperature_A());
    // App_CanTx_SetPeriodicSignal_TEMPERATURE_B(can_tx, coolant->get_temperature_B());
    // App_CanTx_SetPeriodicSignal_PRESSURE_A(can_tx, coolant->get_pressure_A());
    // App_CanTx_SetPeriodicSignal_PRESSURE_B(can_tx, coolant->get_pressure_B());

    // information in range check TODO: JSONCAN ->
    // App_SetPeriodicCanSignals_InRangeCheck(
    //    can_tx, coolant->flow_rate_in_range_check, App_CanTx_SetPeriodicSignal_FLOW_RATE,
    //    App_CanTx_SetPeriodicSignal_FLOW_RATE_OUT_OF_RANGE,
    //    CANMSGS_FSM_NON_CRITICAL_ERRORS_FLOW_RATE_OUT_OF_RANGE_OK_CHOICE,
    //    CANMSGS_FSM_NON_CRITICAL_ERRORS_FLOW_RATE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
    //    CANMSGS_FSM_NON_CRITICAL_ERRORS_FLOW_RATE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    // motor shutdown in flow rate check
    float                    flow_rate;
    enum InRangeCheck_Status flow_rate_inRangeCheck_status =
        App_InRangeCheck_GetValue(coolant->flow_rate_in_range_check, &flow_rate);
    SignalState flow_in_range_signal_state = App_SharedSignal_Update(
        coolant->flow_in_range_signal, flow_rate_inRangeCheck_status == VALUE_UNDERFLOW,
        flow_rate_inRangeCheck_status == VALUE_IN_RANGE);
    // TODO: JSONCAN ->
    // uint8_t CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_FLOW_METER_HAS_UNDERFLOW =
    //    flow_in_range_signal_state == SIGNAL_STATE_ACTIVE
    //        ? CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_FLOW_METER_HAS_UNDERFLOW_TRUE_CHOICE
    //        : CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_FLOW_METER_HAS_UNDERFLOW_FALSE_CHOICE;
    // App_CanTx_SetPeriodicSignal_FLOW_METER_HAS_UNDERFLOW(
    //    can_tx, CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_FLOW_METER_HAS_UNDERFLOW);
}
