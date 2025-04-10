#include "states/app_allStates.h"
#include "states/app_faultState.h"
#include "app_accumulator.h"
#include "app_tractiveSystem.h"
#include "app_imd.h"
#include "app_airs.h"
#include "app_soc.h"
#include "app_shdnLoop.h"
#include "app_diagnosticsMode.h"
#include "io_faultLatch.h"
#include "io_airs.h"
#include "io_bspdTest.h"
#include "app_heartbeatMonitors.h"
#include "io_ltc6813.h"

// Num of cycles for voltage and cell temperature values to settle
#define NUM_CYCLES_TO_SETTLE (30U)
#define NUM_CYCLES_TO_BALANCE (1000U)
#define NUM_CYCLES_TO_MEASURE_BALANCING (100U)
#define NUM_CYCLES_TO_MEASURE_NOMINAL (1000U)

typedef enum
{
    RUN_CELL_MEASUREMENTS,
    RUN_OPEN_WIRE_CHECK,
    RUN_CELL_BALANCING
} isoSpiTasks;

isoSpiTasks iso_spi_task_state    = RUN_CELL_MEASUREMENTS;
uint32_t    iso_spi_state_counter = 0;

void app_allStates_runOnTick1Hz(void)
{
    // If charge state has not placed a lock on broadcasting
    // if the charger is charger is connected
    bool charger_is_connected = app_canRx_BRUSA_IsConnected_get();
    app_canTx_BMS_ChargerConnected_set(charger_is_connected);

    const float min_soc = app_soc_getMinSocCoulombs();

    // Reset SOC from min cell voltage if soc corrupt and voltage readings settled
    if (min_soc < 0)
    {
        if (globals->cell_monitor_settle_count >= NUM_CYCLES_TO_SETTLE)
        {
            app_soc_resetSocFromVoltage();
        }
    }
}

bool app_allStates_runOnTick100Hz(void)
{
    app_canTx_BMS_Heartbeat_set(true);

    app_heartbeatMonitor_checkIn(&hb_monitor);
    app_heartbeatMonitor_broadcastFaults(&hb_monitor);

    const bool balancing_enabled = app_canRx_Debug_CellBalancingRequest_get();

    switch (iso_spi_task_state)
    {
        case RUN_CELL_MEASUREMENTS:
        {
            app_accumulator_runCellMeasurements();

            if (globals->cell_monitor_settle_count < NUM_CYCLES_TO_SETTLE)
            {
                // Do not start timeout until voltage readings have settled
                iso_spi_state_counter = 0;
            }

            // Open wire check is mysteriously causing communication errors with the LTCs, so disabling it for now.
            // TODO: Find out why!
            const uint32_t cycles_to_measure =
                balancing_enabled ? NUM_CYCLES_TO_MEASURE_BALANCING : NUM_CYCLES_TO_MEASURE_NOMINAL;

            if (iso_spi_state_counter >= cycles_to_measure)
            {
                iso_spi_state_counter = 0;
                if (balancing_enabled)
                {
                    iso_spi_task_state = RUN_CELL_BALANCING;
                }
            }

            iso_spi_state_counter++;
            break;
        }
        case RUN_OPEN_WIRE_CHECK:
        {
            if (app_accumulator_runOpenWireCheck())
            {
                iso_spi_state_counter = 0;
                if (balancing_enabled)
                {
                    iso_spi_task_state = RUN_CELL_BALANCING;
                }
                else
                {
                    globals->cell_monitor_settle_count = 0;
                    iso_spi_task_state                 = RUN_CELL_MEASUREMENTS;
                }
            }
            break;
        }
        case RUN_CELL_BALANCING:
        {
            app_accumulator_balanceCells();
            iso_spi_state_counter++;

            if (iso_spi_state_counter >= NUM_CYCLES_TO_BALANCE)
            {
                io_ltc6813_sendStopBalanceCommand();
                iso_spi_task_state    = RUN_CELL_MEASUREMENTS;
                iso_spi_state_counter = 0;
            }

            globals->cell_monitor_settle_count = 0;
            break;
        }
        default:
        {
            break;
        }
    }

    // Re-enable if auxiliary thermistors installed
    // app_thermistors_updateAuxThermistorTemps();
    // app_thermistors_broadcast();

    const bool bspd_test_current_enable = app_canRx_Debug_EnableTestCurrent_get();
    io_bspdTest_enable(bspd_test_current_enable);
    const bool bspd_current_threshold_exceeded = io_bspdTest_isCurrentThresholdExceeded();
    app_canTx_BMS_BSPDCurrentThresholdExceeded_set(bspd_current_threshold_exceeded);

    app_accumulator_broadcast();
    app_tractiveSystem_broadcast();
    app_imd_broadcast();
    app_airs_broadcast();
    app_shdnLoop_broadcast();

    app_diagnosticsMode_broadcast();

    if (io_airs_isNegativeClosed() && io_airs_isPositiveClosed())
    {
        app_soc_updateSocStats();
    }

    const bool acc_fault = app_accumulator_checkFaults();
    const bool ts_fault  = app_tractiveSystem_checkFaults();

    // Update CAN signals for BMS latch statuses.
    app_canTx_BMS_Soc_set(app_soc_getMinSocPercent());
    app_canTx_BMS_BmsOk_set(io_faultLatch_getCurrentStatus(&bms_ok_latch));
    app_canTx_BMS_ImdOk_set(io_faultLatch_getCurrentStatus(&imd_ok_latch));
    app_canTx_BMS_BspdOk_set(io_faultLatch_getCurrentStatus(&bspd_ok_latch));
    app_canTx_BMS_BmsLatchedFault_set(io_faultLatch_getLatchedStatus(&bms_ok_latch));
    app_canTx_BMS_ImdLatchedFault_set(io_faultLatch_getLatchedStatus(&imd_ok_latch));
    app_canTx_BMS_BspdLatchedFault_set(io_faultLatch_getLatchedStatus(&bspd_ok_latch));

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
