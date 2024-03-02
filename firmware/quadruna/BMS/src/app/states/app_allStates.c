#include "states/app_allStates.h"
#include "states/app_faultState.h"
#include "app_utils.h"
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

uint32_t owcCounter = 0;

typedef enum
{
    RUN_CELL_MEASUREMENTS,
    RUN_CELL_OWC
} isoSpiTasks;

isoSpiTasks isoSpiTasksState = RUN_CELL_MEASUREMENTS;

void app_allStates_runOnTick1Hz(void)
{
    bool charger_is_connected = io_charger_isConnected();
    app_canTx_BMS_ChargerConnected_set(charger_is_connected);

    // const float    min_soc  = app_soc_getMinSocCoulombs();
    // const uint16_t soc_addr = app_soc_getSocAddress();

    // TODO: Update to SD Card Logic

    // Reset SOC from min cell voltage if soc corrupt and voltage readings settled
    // if (min_soc < 0)
    // {
    //     if (globals->cell_monitor_settle_count >= NUM_CYCLES_TO_SETTLE)
    //     {
    //          app_soc_resetSocFromVoltage();
    //     }
    // }
    // else
    // {
    //     app_eeprom_writeMinSoc(min_soc, soc_addr);
    // }
}

bool app_allStates_runOnTick100Hz(void)
{
    app_canTx_BMS_Heartbeat_set(true);

    app_heartbeatMonitor_checkIn();
    app_heartbeatMonitor_tick();
    app_heartbeatMonitor_broadcastFaults();

    switch (isoSpiTasksState)
    {
        case RUN_CELL_MEASUREMENTS:
        {
            app_accumulator_runCellMeasurements();
            owcCounter++;
            if (owcCounter >= 500)
            {
                isoSpiTasksState = RUN_CELL_OWC;
            }
            break;
        }
        case RUN_CELL_OWC:
        {
            if (app_accumulator_runOpenWireCheck())
            {
                owcCounter       = 0;
                isoSpiTasksState = RUN_CELL_MEASUREMENTS;
            }
            break;
        }
        default:
        {
            break;
        }
    }

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
    app_canTx_BMS_Soc_set(app_soc_getMinSocPercent());
    app_canTx_BMS_BmsOk_set(io_faultLatch_getCurrentStatus(globals->config->bms_ok_latch));
    app_canTx_BMS_ImdOk_set(io_faultLatch_getCurrentStatus(globals->config->imd_ok_latch));
    app_canTx_BMS_BspdOk_set(io_faultLatch_getCurrentStatus(globals->config->bspd_ok_latch));
    app_canTx_BMS_BmsLatchedFault_set(io_faultLatch_getLatchedStatus(globals->config->bms_ok_latch));
    app_canTx_BMS_ImdLatchedFault_set(io_faultLatch_getLatchedStatus(globals->config->imd_ok_latch));
    app_canTx_BMS_BspdLatchedFault_set(io_faultLatch_getLatchedStatus(globals->config->bspd_ok_latch));

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
        app_stateMachine_setNextState(app_faultState_get());
    }

    return status;
}
