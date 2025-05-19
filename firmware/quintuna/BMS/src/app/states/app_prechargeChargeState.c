#include "app_stateMachine.h"
#include "app_tractiveSystem.h"
#include "io_charger.h"
#include "io_irs.h"
#include "io_time.h"
#include "app_timer.h"

#define PRECHARGE_ACC_VOLTAGE_THRESHOLD 0.9f
#define NUM_OF_INVERTERS 4U
#define INVERTER_CAPACITANCE_F 75e-6f
#define NUM_OF_RESISTORS 1U
#define PRECHARGE_RESISTANCE_OHMS 3e3f
#define PRECHARGE_RC_MS S_TO_MS(PRECHARGE_RESISTANCE_OHMS * NUM_OF_RESISTORS * INVERTER_CAPACITANCE_F * NUM_OF_INVERTERS)
#define MAX_PRECHARGE_ATTEMPTS 3U
#define PRECHARGE_COMPLETION_MS (float)PRECHARGE_RC_MS * 2.7f // 2.7RC corresponds to time to reach ~93% charged
#define PRECHARGE_COMPLETION_UPPERBOUND_MS (uint32_t)(PRECHARGE_COMPLETION_MS * 3.0f)
#define PRECHARGE_COMPLETION_LOWERBOUND_MS (uint32_t)(PRECHARGE_COMPLETION_MS * 0.5f)

static uint32_t precharge_start_time;
static uint32_t precharge_failures;

static void app_prechargeChargeStateRunOnEntry()
{
    io_irs_closePrecharge();
    precharge_start_time = io_time_getCurrentMs();
}

static void app_prechargeChargeStateRunOnTick1Hz()
{
    
}

static void app_prechargeChargeStateRunOnTick100Hz()
{
    float ts_voltage = app_tractiveSystem_getVoltage();
    float thresh_voltage = app_accumulator_getPackVoltage() * PRECHARGE_ACC_VOLTAGE_THRESHOLD;

    uint32_t precharge_time = io_time_getCurrentMs() - precharge_start_time;

    const bool is_charger_connected = (io_charger_getConnectionStatus() == EVSE_CONNECTED || WALL_CONNECTED);
    
    if(!is_charger_connected)
        app_stateMachine_setNextState(app_initState_get());
    if(ts_voltage >= thresh_voltage)
    {
        bool is_raising_slowly = precharge_time >= PRECHARGE_COMPLETION_UPPERBOUND_MS;
        bool is_raising_quickly = precharge_time >= PRECHARGE_COMPLETION_LOWERBOUND_MS;
        bool is_irs_negative_open = !io_irs_isNegativeClosed();

        bool has_precharge_fault =
            is_raising_slowly        // always a fault
            || is_irs_negative_open; // fault if AIR- is open

        /**
         * If there is a pre-charge fault and there were no more than three previous pre-charge faults
         * Go back to Init State, add one to the pre-charge failed counter
         * Else go to Precharge Latch State, reset the pre-charge failed counter
         */
        if (has_precharge_fault)
        {
            if (precharge_failures > MAX_PRECHARGE_ATTEMPTS)
            {
                precharge_failures = 0;
                app_stateMachine_setNextState(app_prechargeLatchState_get());
            }
            else
            {
                precharge_failures++;
                app_stateMachine_setNextState(app_initState_get());
            }
        }
        else
        {
            precharge_failures = 0;
            io_irs_closePositive();
            app_stateMachine_setNextState(app_chargeInitState_get());
        }
    }
}

static void app_prechargeChargeStateRunOnExit()
{
    io_irs_openPrecharge();
}

const State *app_prechargeChargeState_get(void)
{
    static State precharge_charge_state = {
        .name               = "PRECHARGE CHARGE",
        .run_on_entry       = app_prechargeChargeStateRunOnEntry,
        .run_on_tick_1Hz    = app_prechargeChargeStateRunOnTick1Hz,
        .run_on_tick_100Hz  = app_prechargeChargeStateRunOnTick100Hz,
        .run_on_exit        = app_prechargeChargeStateRunOnExit,
    };

    return &precharge_charge_state;
}