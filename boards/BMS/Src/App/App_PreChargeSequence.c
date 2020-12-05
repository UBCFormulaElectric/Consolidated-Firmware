#include <assert.h>
#include <stdlib.h>
#include "App_PreChargeSequence.h"
#include "App_PrechargeStateMachine.h"

#define NUM_OF_RC_TIME_CONSTANTS 5U

struct PreChargeSequence
{
    void (*enable_pre_charge_sequence)(void);
    void (*disable_pre_charge_sequence)(void);
    float (*get_ts_adc_voltage)(void);
    float (*get_ts_voltage)(float);

    float                  accumulator_voltage;
    uint32_t               rc_time_constant_ms;
    size_t                 rc_time_constant_index;
    uint32_t               prev_time_ms;
    enum PreChargingStatus pre_charging_status;

    struct PreChargeStateMachine *state_machine;
};

struct PreChargeSequence *App_PreChargeSequence_Create(
    void (*enable_pre_charge_sequence)(void),
    void (*disable_pre_charge_sequence)(void),
    float (*get_ts_adc_voltage)(void),
    float (*get_ts_voltage)(float),
    float    accumulator_voltage,
    uint32_t rc_time_constant)
{
    struct PreChargeSequence *pre_charge_sequence =
        malloc(sizeof(struct PreChargeSequence));
    assert(pre_charge_sequence != NULL);

    pre_charge_sequence->enable_pre_charge_sequence =
        enable_pre_charge_sequence;
    pre_charge_sequence->disable_pre_charge_sequence =
        disable_pre_charge_sequence;
    pre_charge_sequence->get_ts_adc_voltage     = get_ts_adc_voltage;
    pre_charge_sequence->get_ts_voltage         = get_ts_voltage;
    pre_charge_sequence->accumulator_voltage    = accumulator_voltage;
    pre_charge_sequence->rc_time_constant_ms    = rc_time_constant;
    pre_charge_sequence->rc_time_constant_index = 0U;
    pre_charge_sequence->prev_time_ms           = 0U;
    pre_charge_sequence->pre_charging_status    = PRE_CHARGING_VOLTAGE_IN_RANGE;

    pre_charge_sequence->state_machine = App_PreChargeStateMachine_Create();

    return pre_charge_sequence;
}

void App_PreChargeSequence_Destroy(
    struct PreChargeSequence *pre_charge_sequence)
{
    free(pre_charge_sequence->state_machine);
    free(pre_charge_sequence);
}

struct PreChargeStateMachine *App_PreChargeSequence_GetStateMachine(
    const struct PreChargeSequence *const pre_charge_sequence)
{
    return pre_charge_sequence->state_machine;
}

void App_PreChargeSequence_Enable(
    const struct PreChargeSequence *const pre_charge_sequence)
{
    pre_charge_sequence->enable_pre_charge_sequence();
}

void App_PreChargeSequence_Disable(
    const struct PreChargeSequence *const pre_charge_sequence)
{
    pre_charge_sequence->disable_pre_charge_sequence();
}

void App_PreChargeSequence_SetPrevTimeMs(
    struct PreChargeSequence *const pre_charge_sequence,
    uint32_t                        current_time_ms)
{
    pre_charge_sequence->prev_time_ms = current_time_ms;
}

enum PreChargingStatus App_PreChargeSequence_GetPreChargingStatus(
    struct PreChargeSequence *pre_charge_sequence)
{
    return pre_charge_sequence->pre_charging_status;
}

enum PreChargingStatus App_PreChargeSequence_CheckPreChargeBusVoltage(
    struct PreChargeSequence *pre_charge_sequence,
    uint32_t                  current_ms)
{
    if (pre_charge_sequence->rc_time_constant_index < NUM_OF_RC_TIME_CONSTANTS)
    {
        if (current_ms - pre_charge_sequence->prev_time_ms >=
            pre_charge_sequence->rc_time_constant_ms)
        {
            pre_charge_sequence->prev_time_ms = current_ms;
            size_t index = pre_charge_sequence->rc_time_constant_index++;

            // The lower bound for the voltage expected at RC, 2RC, ... , 5RC
            // expressed as a percentage of the expected voltage
            const float min_cap_voltage_pct[NUM_OF_RC_TIME_CONSTANTS] = {
                0.5689f, 0.8414f, 0.9417f, 0.9785f, 0.9800f
            };

            // The upper bound for the voltage expected at RC, 2RC, ... , 5RC
            // expressed as a percentage of the expected voltage
            const float max_cap_voltage_pct[NUM_OF_RC_TIME_CONSTANTS] = {
                0.6953f, 0.8879f, 0.9587f, 0.9800f, 1.0000f
            };
            const float accumulator_voltage =
                pre_charge_sequence->accumulator_voltage;
            const float min_ts_voltage =
                accumulator_voltage * min_cap_voltage_pct[index];
            const float max_ts_voltage =
                accumulator_voltage * max_cap_voltage_pct[index];
            const float ts_voltage = pre_charge_sequence->get_ts_voltage(
                pre_charge_sequence->get_ts_adc_voltage());

            if (ts_voltage > min_ts_voltage && ts_voltage < max_ts_voltage)
            {
                if (index == NUM_OF_RC_TIME_CONSTANTS - 1U)
                {
                    pre_charge_sequence->pre_charging_status =
                        PRE_CHARGING_SUCCESS;
                }
                else
                {
                    pre_charge_sequence->pre_charging_status =
                        PRE_CHARGING_VOLTAGE_IN_RANGE;
                }
            }
            else if (ts_voltage > max_ts_voltage)
            {
                pre_charge_sequence->pre_charging_status =
                    PRE_CHARGING_OVERVOLTAGE_ERROR;
            }
            else if (ts_voltage < min_ts_voltage)
            {
                pre_charge_sequence->pre_charging_status =
                    PRE_CHARGING_UNDERVOLTAGE_ERROR;
            }
        }
    }

    return pre_charge_sequence->pre_charging_status;
}
