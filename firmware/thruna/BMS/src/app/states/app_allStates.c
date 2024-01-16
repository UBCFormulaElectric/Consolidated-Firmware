#include "states/app_allStates.h"
#include "states/app_faultState.h"
#include "App_SharedMacros.h"
#include "app_thermistors.h"
#include "app_accumulator.h"
#include "app_tractiveSystem.h"
#include "app_imd.h"
#include "app_airs.h"
#include "app_soc.h"
#include "io_faultLatch.h"
#include "io_airs.h"

// Num of cycles for voltage and cell temperature values to settle
#define NUM_CYCLES_TO_SETTLE (30U)

static bool sendAndReceiveHeartbeat(struct HeartbeatMonitor *hb_monitor)
{
    App_CanTx_BMS_Heartbeat_Set(true);

    if (App_CanRx_FSM_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, FSM_HEARTBEAT_ONE_HOT);
        App_CanRx_FSM_Heartbeat_Update(false);
    }

    if (App_CanRx_DCM_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, DCM_HEARTBEAT_ONE_HOT);
        App_CanRx_DCM_Heartbeat_Update(false);
    }

    if (App_CanRx_PDM_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, PDM_HEARTBEAT_ONE_HOT);
        App_CanRx_PDM_Heartbeat_Update(false);
    }

    const bool missing_hb = !App_SharedHeartbeatMonitor_Tick(hb_monitor);
    return missing_hb;
}

void app_allStates_runOnTick1Hz(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
    bool charger_is_connected = io_charger_isConnected();
    App_CanTx_BMS_ChargerConnected_Set(charger_is_connected);

    const float    min_soc  = app_soc_getMinSocCoulombs();
    const uint16_t soc_addr = app_soc_getSocAddress();

    // Reset SOC from min cell voltage if soc corrupt and voltage readings settled
    if (min_soc < 0)
    {
        if (globals->cell_monitor_settle_count >= NUM_CYCLES_TO_SETTLE)
        {
            app_soc_resetSocFromVoltage();
        }
    }
    else
    {
        app_eeprom_writeMinSoc(min_soc, soc_addr);
    }
}

bool app_allStates_runOnTick100Hz(struct StateMachine *const state_machine)
{
    const bool missing_hb = sendAndReceiveHeartbeat(globals->hb_monitor);
    App_CanAlerts_BMS_Fault_MissingHeartbeat_Set(missing_hb);

    app_accumulator_runOnTick100Hz();
    app_thermistors_updateAuxThermistorTemps();

    app_accumulator_broadcast();
    app_tractiveSystem_broadcast();
    app_imd_broadcast();
    app_airs_broadcast();
    app_thermistors_broadcast();

    if (io_airs_isNegativeClosed() && io_airs_isPositiveClosed())
    {
        app_soc_updateSocStats();
    }

    const bool acc_fault = app_accumulator_checkFaults();
    const bool ts_fault  = app_tractveSystem_checkFaults();

    // Update CAN signals for BMS latch statuses.
    App_CanTx_BMS_Soc_Set(app_soc_getMinSocPercent());
    App_CanTx_BMS_BmsOk_Set(io_faultLatch_getCurrentStatus(globals->config->bms_ok_latch));
    App_CanTx_BMS_ImdOk_Set(io_faultLatch_getCurrentStatus(globals->config->imd_ok_latch));
    App_CanTx_BMS_BspdOk_Set(io_faultLatch_getCurrentStatus(globals->config->bspd_ok_latch));
    App_CanTx_BMS_BmsLatchedFault_Set(io_faultLatch_getLatchedStatus(globals->config->bms_ok_latch));
    App_CanTx_BMS_ImdLatchedFault_Set(io_faultLatch_getLatchedStatus(globals->config->imd_ok_latch));
    App_CanTx_BMS_BspdLatchedFault_Set(io_faultLatch_getLatchedStatus(globals->config->bspd_ok_latch));

    // Wait for cell voltage and temperature measurements to settle. We expect to read back valid values from the
    // monitoring chips within 3 cycles
    bool status = true;
    if (globals->cell_monitor_settle_count < NUM_CYCLES_TO_SETTLE)
    {
        globals->cell_monitor_settle_count++;
    }
    else if (acc_fault || ts_fault)
    {
        status = false;
        App_SharedStateMachine_SetNextState(state_machine, app_faultState_get());
    }

    return status;
}
