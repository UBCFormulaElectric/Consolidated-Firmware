#include "states/app_allStates.h"
#include "states/app_faultState.h"
#include "app_utils.h"
#include "app_thermistors.h"
#include "app_accumulator.h"
#include "app_tractiveSystem.h"
#include "app_imd.h"
#include "app_airs.h"
#include "app_soc.h"
#include "app_shdnLoop.h"
#include "io_faultLatch.h"
#include "io_airs.h"

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
    bool charger_is_connected = io_charger_isConnected();
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
    else
    {
        app_soc_writeSocToSd(min_soc);
    }
}

bool app_allStates_runOnTick100Hz(void)
{
    app_canTx_BMS_Heartbeat_set(true);

    app_heartbeatMonitor_checkIn();
    app_heartbeatMonitor_broadcastFaults();

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

            const uint32_t cycles_to_measure =
                balancing_enabled ? NUM_CYCLES_TO_MEASURE_BALANCING : NUM_CYCLES_TO_MEASURE_NOMINAL;
            if (iso_spi_state_counter == cycles_to_measure)
            {
                iso_spi_state_counter = 0;
                iso_spi_task_state    = RUN_OPEN_WIRE_CHECK;
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
                    app_accumulator_calculateCellsToBalance();
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
                io_ltc6813Shared_disableBalance();
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

    app_accumulator_broadcast();
    app_tractiveSystem_broadcast();
    app_imd_broadcast();
    app_airs_broadcast();
    app_shdn_loop_broadcast();

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
