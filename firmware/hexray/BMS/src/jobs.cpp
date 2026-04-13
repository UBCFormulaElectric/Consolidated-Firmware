#include "jobs.hpp"
#include "app_canUtils.hpp"
#include "app_jsoncan.hpp"
#include "app_segments.hpp"
#include "io/io_adbms.hpp"
#include "io_canMsg.hpp"

// app
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canAlerts.hpp"

#include "states/app_states.hpp"
#include "app_precharge.hpp"
#include "app_segments.hpp"
#include "app_timer.hpp"
#include "app_imd.hpp"
#include "app_powerLimit.hpp"
#include "app_bmsShdnLoop.hpp"
#include "app_tractiveSystem.hpp"
#include "app_irs.hpp"
#include "app_heartbeatMonitors.hpp"
#include "app_heartbeatMonitor.hpp"
#include "app_jsoncan.hpp"
#include <app_canUtils.hpp>

// io
#include "io_canMsg.hpp"
#include "io_canQueues.hpp"
#include "io_canMsg.hpp"
#include "io_irs.hpp"
#include "io_time.hpp"
#include "io_semaphore.hpp"
#include "util_errorCodes.hpp"
#include "io_bspdTest.hpp"
#include "io_charger.hpp"
#include "io_fans.hpp"
#include "io_faultLatch.hpp"
#include <io_canTx.hpp>
#include "io_adbms.hpp"

extern "C" {
#include "app_commitInfo.h"
}

// static array<io::adbms::StatusGroups, io::NUM_SEGMENTS> stat_reg;
static array<expected<void, ErrorCode>, io::NUM_SEGMENTS> stat_regs_success;
static io::semaphore                                      spi_bus_lock(true);
static io::semaphore                                      adbms_app_lock(true);

// TODO: Uncomment when segments are added
// static Semaphore isospi_bus_access_lock;
// static Semaphore adbms_app_data_lock;

using namespace app;
void jobs_init()
{
    can_tx_queue.init();
    can_rx_queue.init();
    // TODO: Uncomment semaphores when segments are added (also if there's a semaphore.cpp update).
    // isospi_bus_access_lock guards access to the ISOSPI bus, to guarantee an ADuM SPI transaction doesn't get
    // interrupted by another task. It's just a regular semaphore (no priority inheritance) since it depends on
    // hardware.
    // io_semaphore_create(&isospi_bus_access_lock, false);

    // adbms_app_data_lock guards access to any ADuM app data that may be written to by the ADuM tasks and read from
    // other tick functions. It's a mutex (yes priority inheritance) since it's guarding shared data and doesn't depend
    // on hardware.
    // io_semaphore_create(&adbms_app_data_lock, true);

    io::can_tx::init(
        [](const JsonCanMsg &tx_msg)
        {
            const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
            (void)can_tx_queue.push(msg);
            // LOG_IF_ERR();
        },
        [](const JsonCanMsg &tx_msg)
        {
            UNUSED(tx_msg);
            // const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
            // LOG_IF_ERR(can_tx_queue.push(msg));
        });
    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEFAULT, true);
    io::can_tx::enableMode_charger(app::can_utils::chargerMode::CHARGER_MODE_DEFAULT, true);

    can_tx::BMS_Hash_set(GIT_COMMIT_HASH);
    can_tx::BMS_Clean_set(GIT_COMMIT_CLEAN);
    can_tx::BMS_Heartbeat_set(true);

    precharge::init();

#ifndef TARGET_HV_SUPPLY
    // TODO segments init and tests
#endif

    StateMachine::init(&states::init_state);
}

void jobs_run1Hz_tick()
{
    io::can_tx::enqueue1HzMsgs();
}

void jobs_run100Hz_tick()
{
    // TODO: Uncomment when segments are added
    // io_semaphore_take(&adbms_app_data_lock, MAX_TIMEOUT);

    StateMachine::tick100Hz();

    const bool debug_mode_enabled = app::can_rx::Debug_EnableDebugMode_get();
    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEBUG, debug_mode_enabled);

    ts::broadcast();
    imd::broadcast();
    shdn::bms_shdnLoop.broadcast();
    plim::broadcast();
    hb_monitor.checkIn();
    hb_monitor.broadcastFaults();

    // TODO: Enable fans for endurance when contactors are closed.
    // const bool hv_up = io_irs_isNegativeClosed() && io_irs_isPositiveClosed();
    // io::fans::tick(hv_up);
    io::fans::tick(false);

    io::bspdtest::enable(app::can_rx::Debug_EnableTestCurrent_get());
    app::can_tx::BMS_BSPDCurrentThresholdExceeded_set(io::bspdtest::isCurrentThresholdExceeded());

    // If charge state has not placed a lock on broadcasting
    // if the charger is charger is connected
    app::can_tx::BMS_ChargerConnectedType_set(io::charger::getConnectionStatus());

#ifdef TARGET_HV_SUPPLY
    const bool acc_fault = false;
#else
    segments::checkWarnings();
    const bool acc_fault = segments::checkFaults();
#endif
    using namespace io::faultLatch;

    setCurrentStatus(&bms_ok_latch, acc_fault ? FaultLatchState::FAULT : FaultLatchState::OK);

    // Update CAN signals for BMS latch statuses.
    app::can_tx::BMS_BmsCurrentlyOk_set(getCurrentStatus(&bms_ok_latch) == FaultLatchState::OK);
    app::can_tx::BMS_ImdCurrentlyOk_set(getCurrentStatus(&imd_ok_latch) == FaultLatchState::OK);
    app::can_tx::BMS_BspdCurrentlyOk_set(getCurrentStatus(&bspd_ok_latch) == FaultLatchState::OK);
    app::can_tx::BMS_BmsLatchOk_set(getLatchedStatus(&bms_ok_latch) == FaultLatchState::OK);
    app::can_tx::BMS_ImdLatchOk_set(getLatchedStatus(&imd_ok_latch) == FaultLatchState::OK);
    app::can_tx::BMS_BspdLatchOk_set(getLatchedStatus(&bspd_ok_latch) == FaultLatchState::OK);

    app::can_tx::BMS_BSPDBrakePressureThresholdExceeded_set(io::bspdtest::isBrakePressureThresholdExceeded());
    app::can_tx::BMS_BSPDAccelBrakeOk_set(io::bspdtest::isAccelBrakeOk());

    using namespace app;

    const bool ir_negative_opened_debounced = irs::negativeOpenedDebounced();
    if (ir_negative_opened_debounced)
    {
        StateMachine::set_next_state(&states::init_state);
    }
    if (can_alerts::AnyBoardHasFault())
    {
        StateMachine::set_next_state(&states::fault_state);
    }

    irs::broadcast();

    io::can_tx::enqueue100HzMsgs();
    // TODO: Uncomment when segments are added
    // io_semaphore_give(&adbms_app_data_lock);
}

void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}

void jobs_adbms_init()
{
    app::segments::setDefaultConfig();
    LOG_IF_ERR(io::adbms::wakeup());
    LOG_IF_ERR(io::adbms::clearCellVoltageReg());
    LOG_IF_ERR(io::adbms::clearFilteredCellVoltageReg());
    LOG_IF_ERR(io::adbms::clearCellTempReg());
    LOG_IF_ERR(app::segments::configSync());
}

void jobs_runAdbmsVoltages_tick()
{
    const bool balancing_enabled = false;

    spi_bus_lock.take(io::MAX_TIMEOUT);

    LOG_IF_ERR(io::adbms::wakeup());
    LOG_IF_ERR(app::segments::configSync());
    LOG_IF_ERR(app::segments::runVoltageConversion());
    app::segments::balancingTick(balancing_enabled);

    spi_bus_lock.give();

    adbms_app_lock.take(io::MAX_TIMEOUT);

    app::segments::broadcastCellVoltages();

    adbms_app_lock.give();
}

void jobs_runAdbmsFilteredVoltages_tick()
{
    spi_bus_lock.take(io::MAX_TIMEOUT);

    LOG_IF_ERR(io::adbms::wakeup());
    LOG_IF_ERR(app::segments::configSync());
    LOG_IF_ERR(app::segments::runFilteredVoltageConversion());

    spi_bus_lock.give();

    adbms_app_lock.take(io::MAX_TIMEOUT);

    app::segments::broadcastFilteredCellVoltages();

    adbms_app_lock.give();
}

void jobs_runAdbmsTemperatures_tick()
{
    spi_bus_lock.take(io::MAX_TIMEOUT);

    LOG_IF_ERR(io::adbms::wakeup());
    LOG_IF_ERR(app::segments::configSync());
    LOG_IF_ERR(app::segments::runAuxConversion());

    spi_bus_lock.give();

    adbms_app_lock.take(io::MAX_TIMEOUT);

    app::segments::broadcastCellTemps();

    adbms_app_lock.give();
}

void jobs_runAdbmsDiagnostics_tick()
{
    spi_bus_lock.take(io::MAX_TIMEOUT);

    LOG_IF_ERR(io::adbms::wakeup());
    LOG_IF_ERR(app::segments::configSync());
    LOG_IF_ERR(app::segments::runStatusConversion());
    LOG_IF_ERR(app::segments::runCellOpenWireCheck());

    spi_bus_lock.give();

    adbms_app_lock.take(io::MAX_TIMEOUT);

    app::segments::broadcastStatus();
    app::segments::broadcastCellOpenWireCheck();

    adbms_app_lock.give();
}
