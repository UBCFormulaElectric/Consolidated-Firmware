#include "jobs.h"

// app
#include "states/app_states.h"
#include "states/app_allStates.h"
#include "app_precharge.h"
#include "app_segments.h"
#include "app_shdnLoop.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_commitInfo.h"
#include "app_jsoncan.h"
#include "app_canAlerts.h"
#include "app_timer.h"
// io
#include "app_irs.h"
#include "io_canTx.h"
#include "io_canQueue.h"
#include "io_canMsg.h"
#include "io_irs.h"
#include "io_time.h"
#include "io_semaphore.h"

CanTxQueue can_tx_queue; // TODO there HAS to be a better location for this

static Semaphore isospi_bus_access_lock;
static Semaphore ltc_app_data_lock;

static TimerChannel air_n_debounce_timer;
#define AIR_N_DEBOUNCE_PERIOD (200) // ms

static void jsoncan_transmit_func(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = app_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&can_tx_queue, &msg);
}

static void charger_transmit_func(const JsonCanMsg *msg)
{
    // LOG_INFO("Send charger message: %d", msg->std_id);
    UNUSED(msg);
}

void jobs_init()
{
    // isospi_bus_access_lock guards access to the ISOSPI bus, to guarantee an LTC transaction doesn't get interrupted
    // by another task. It's just a regular semaphore (no priority inheritance) since it depends on hardware.
    io_semaphore_create(&isospi_bus_access_lock, false);

    // ltc_app_data_lock guards access to any LTC app data that may be written to by the LTC tasks and read from other
    // tick functions. It's a mutex (yes priority inheritance) since it's guarding shared data and doesn't depend on
    // hardware.
    io_semaphore_create(&ltc_app_data_lock, true);

    io_canTx_init(jsoncan_transmit_func, charger_transmit_func);
    io_canTx_enableMode_can1(CAN1_MODE_DEFAULT, true);
    io_canTx_enableMode_charger(CHARGER_MODE_DEFAULT, true);
    io_canQueue_initRx();
    io_canQueue_initTx(&can_tx_queue);

    app_canTx_init();
    app_canRx_init();

    app_canTx_BMS_Hash_set(GIT_COMMIT_HASH);
    app_canTx_BMS_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_BMS_Heartbeat_set(true);

    app_precharge_init();

    app_segments_setDefaultConfig();
    app_segments_initFaults();
    // Write LTC configs.
    app_segments_setDefaultConfig();
    app_segments_balancingInit();
    io_ltc6813_wakeup();
    LOG_IF_ERR(app_segments_configSync());

    // Run all self tests at init.
    LOG_IF_ERR(app_segments_runAdcAccuracyTest());
    LOG_IF_ERR(app_segments_runVoltageSelfTest());
    LOG_IF_ERR(app_segments_runAuxSelfTest());
    LOG_IF_ERR(app_segments_runStatusSelfTest());
    LOG_IF_ERR(app_segments_runOpenWireCheck());

    app_segments_broadcastAdcAccuracyTest();
    app_segments_broadcastVoltageSelfTest();
    app_segments_broadcastAuxSelfTest();
    app_segments_broadcastStatusSelfTest();
    app_segments_broadcastOpenWireCheck();
    app_segments_balancingInit();

    app_timer_init(&air_n_debounce_timer, AIR_N_DEBOUNCE_PERIOD);

    app_stateMachine_init(&init_state);
    app_allStates_init();
}

void jobs_run1Hz_tick(void)
{
    app_allStates_runOnTick1Hz();
    io_canTx_enqueue1HzMsgs();
}

void jobs_run100Hz_tick(void)
{
    io_semaphore_take(&ltc_app_data_lock, MAX_TIMEOUT);

    app_stateMachine_tick100Hz();

    app_allStates_runOnTick100Hz();

    const bool ir_negative_opened = io_irs_negativeState() == CONTACTOR_STATE_OPEN;
    const bool ir_negative_opened_debounced =
        app_timer_runIfCondition(&air_n_debounce_timer, ir_negative_opened) == TIMER_STATE_EXPIRED;
    if (ir_negative_opened_debounced)
    {
        app_stateMachine_setNextState(&init_state);
    }
    if (app_canAlerts_AnyBoardHasFault())
    {
        app_stateMachine_setNextState(&fault_state);
    }
    app_stateMachine_tickTransitionState();

    app_irs_broadcast();

    io_canTx_enqueue100HzMsgs();
    io_semaphore_give(&ltc_app_data_lock);
}

void jobs_run1kHz_tick(void)
{
    io_canTx_enqueueOtherPeriodicMsgs(io_time_getCurrentMs());
}

void jobs_initLTCVoltages(void)
{
    io_semaphore_take(&isospi_bus_access_lock, MAX_TIMEOUT);
    {
        // Write LTC configs.
        io_ltc6813_wakeup();
        LOG_IF_ERR(app_segments_configSync());
    }
    io_semaphore_give(&isospi_bus_access_lock);
}

void jobs_runLTCVoltages(void)
{
    const bool balancing_enabled = app_stateMachine_getCurrentState() == &balancing_state;
    io_semaphore_take(&isospi_bus_access_lock, MAX_TIMEOUT);
    {
        io_ltc6813_wakeup();
        LOG_IF_ERR(app_segments_configSync());

        // Mute/unmute balancing.
        if (balancing_enabled)
        {
            LOG_IF_ERR(io_ltc6813_sendBalanceCommand());
        }
        else
        {
            io_ltc6813_sendStopBalanceCommand();
        }

        LOG_IF_ERR(app_segments_runVoltageConversion());
    }
    io_semaphore_give(&isospi_bus_access_lock);

    io_semaphore_take(&ltc_app_data_lock, MAX_TIMEOUT);
    {
        app_segments_broadcastCellVoltages();
        app_segments_broadcastVoltageStats();
        app_segments_balancingTick(balancing_enabled);
    }
    io_semaphore_give(&ltc_app_data_lock);
}

void jobs_initLTCTemps(void)
{
    io_semaphore_take(&isospi_bus_access_lock, MAX_TIMEOUT);
    {
        // Write LTC configs.
        io_ltc6813_wakeup();
        LOG_IF_ERR(app_segments_configSync());
    }
    io_semaphore_give(&isospi_bus_access_lock);
}

void jobs_runLTCTemperatures(void)
{
    io_semaphore_take(&isospi_bus_access_lock, MAX_TIMEOUT);
    {
        io_ltc6813_wakeup();
        LOG_IF_ERR(app_segments_runAuxConversion());
    }
    io_semaphore_give(&isospi_bus_access_lock);

    io_semaphore_take(&ltc_app_data_lock, MAX_TIMEOUT);
    {
        app_segments_broadcastTempsVRef();
        app_segments_broadcastTempStats();
    }
    io_semaphore_give(&ltc_app_data_lock);
}

void jobs_initLTCDiagnostics(void)
{
    // Run all self tests at init.
    io_semaphore_take(&isospi_bus_access_lock, MAX_TIMEOUT);
    {
        // Write LTC configs.
        io_ltc6813_wakeup();
        LOG_IF_ERR(app_segments_configSync());

        LOG_IF_ERR(app_segments_runAdcAccuracyTest());
        LOG_IF_ERR(app_segments_runVoltageSelfTest());
        LOG_IF_ERR(app_segments_runAuxSelfTest());
        LOG_IF_ERR(app_segments_runStatusSelfTest());
        LOG_IF_ERR(app_segments_runOpenWireCheck());
    }
    io_semaphore_give(&isospi_bus_access_lock);

    io_semaphore_take(&ltc_app_data_lock, MAX_TIMEOUT);
    {
        app_segments_broadcastAdcAccuracyTest();
        app_segments_broadcastVoltageSelfTest();
        app_segments_broadcastAuxSelfTest();
        app_segments_broadcastStatusSelfTest();
        app_segments_broadcastOpenWireCheck();
    }
    io_semaphore_give(&ltc_app_data_lock);
}

void jobs_runLTCDiagnostics(void)
{
    io_semaphore_take(&isospi_bus_access_lock, MAX_TIMEOUT);
    {
        io_ltc6813_wakeup();

        // Open wire check is the only one we need to perform as per rules.
        LOG_IF_ERR(app_segments_runStatusConversion());
        LOG_IF_ERR(app_segments_runOpenWireCheck());

        if (app_canRx_Debug_EnableDebugMode_get())
        {
            LOG_IF_ERR(app_segments_runAdcAccuracyTest());
            LOG_IF_ERR(app_segments_runVoltageSelfTest());
            LOG_IF_ERR(app_segments_runAuxSelfTest());
            LOG_IF_ERR(app_segments_runStatusSelfTest());
        }
    }
    io_semaphore_give(&isospi_bus_access_lock);

    io_semaphore_take(&ltc_app_data_lock, MAX_TIMEOUT);
    {
        app_segments_broadcastStatus();
        app_segments_broadcastOpenWireCheck();

        if (app_canRx_Debug_EnableDebugMode_get())
        {
            app_segments_broadcastAdcAccuracyTest();
            app_segments_broadcastVoltageSelfTest();
            app_segments_broadcastAuxSelfTest();
            app_segments_broadcastStatusSelfTest();
        }
    }
    io_semaphore_give(&ltc_app_data_lock);
}
