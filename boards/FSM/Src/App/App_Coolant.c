#include <stdlib.h>
#include <assert.h>

#include "App_Coolant.h"
#include "App_InRangeCheck.h"
#include "configs/App_FlowRateThresholds.h"

#include "App_SharedSetPeriodicCanSignals.h"
STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(FsmCanTxInterface)

struct Coolant
{
    float (*get_flow_rate)(void);
    struct InRangeCheck *flow_rate_in_range_check;

    float (*get_temperature_A)(void);
    float (*get_temperature_B)(void);

    float (*get_pressure_A)(void);
    float (*get_pressure_B)(void);
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
    return coolant;
}

void App_Coolant_Destroy(struct Coolant *coolant)
{
    App_InRangeCheck_Destroy(coolant->flow_rate_in_range_check);
    free(coolant);
}

struct InRangeCheck *App_Coolant_GetFlowInRangeCheck(struct Coolant *coolant)
{
    return coolant->flow_rate_in_range_check;
}

void App_Coolant_Broadcast(struct FsmCanTxInterface *can_tx, struct Coolant *coolant)
{
    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Coolant_GetFlowInRangeCheck(coolant), App_CanTx_SetPeriodicSignal_FLOW_RATE,
        App_CanTx_SetPeriodicSignal_FLOW_RATE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_FLOW_RATE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_FLOW_RATE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_FLOW_RATE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}
