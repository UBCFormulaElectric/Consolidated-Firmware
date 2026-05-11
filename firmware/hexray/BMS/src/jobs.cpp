#include "jobs.hpp"
#include "io/io_adbms.hpp"
#include "util_errorCodes.hpp"

// app
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canAlerts.hpp"
#include "app_states.hpp"
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
#include "app_canUtils.hpp"

// io
#include "io_canQueues.hpp"
#include "io_canMsg.hpp"
#include "io_irs.hpp"
#include "io_time.hpp"
#include "io_semaphore.hpp"
#include "io_bspdTest.hpp"
#include "io_charger.hpp"
#include "io_fans.hpp"
#include "io_faultLatch.hpp"
#include "io_canTx.hpp"
#include "io_adbms.hpp"

extern "C"
{
#include "app_commitInfo.h"
}

io::semaphore spi_bus_lock(true);
io::semaphore adbms_app_lock(true);

static void jsoncan_transmit_func(const JsonCanMsg &tx_msg)
{
    const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
    LOG_IF_ERR(can_tx_queue.push(msg));
}

static void charger_transmit_func(const JsonCanMsg &tx_msg)
{
    UNUSED(tx_msg);
}

void jobs_init()
{
    io::can_tx::init(jsoncan_transmit_func, charger_transmit_func);
    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEFAULT, true);
    io::can_tx::enableMode_charger(app::can_utils::chargerMode::CHARGER_MODE_DEFAULT, true);

    can_rx_queue.init();
    can_tx_queue.init();

    app::can_tx::BMS_Hash_set(GIT_COMMIT_HASH);
    app::can_tx::BMS_Clean_set(GIT_COMMIT_CLEAN);
    app::can_tx::BMS_Heartbeat_set(true);

    app::precharge::init();

#ifndef TARGET_HV_SUPPLY
    app::segments::setDefaultConfig();
    LOG_IF_ERR(io::adbms::wakeup());
    LOG_IF_ERR(io::adbms::clearCellVoltageReg());
    LOG_IF_ERR(io::adbms::clearFilteredCellVoltageReg());
    LOG_IF_ERR(io::adbms::clearCellAuxReg());
    LOG_IF_ERR(io::adbms::clearStatReg());
    LOG_IF_ERR(app::segments::configSync());
    LOG_INFO("Segment Initialization Done");
#endif
    // app::segments::initFaults();
    app::StateMachine::init(&app::states::init_state);
    app::can_tx::BMS_Heartbeat_set(true);
}

void jobs_run1Hz_tick()
{
    app::StateMachine::tick1Hz();
    io::can_tx::enqueue1HzMsgs();
}

void jobs_run100Hz_tick()
{
    app::StateMachine::tick100Hz();

    const bool debug_mode_enabled = app::can_rx::Debug_EnableDebugMode_get();
    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEBUG, debug_mode_enabled);

    app::ts::broadcast();
    // app::imd::broadcast();
    app::shdn::bms_shdnLoop.broadcast();
    app::plim::broadcast();
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
    // app::segments::checkWarnings();
    // const bool acc_fault = app::segments::checkFaults();
#endif
    using namespace io::faultLatch;

    // setCurrentStatus(&bms_ok_latch, acc_fault ? FaultLatchState::FAULT : FaultLatchState::OK);
    setCurrentStatus(&bms_ok_latch, FaultLatchState::OK);

    // Update CAN signals for BMS latch statuses.
    app::can_tx::BMS_BmsCurrentlyOk_set(getCurrentStatus(&bms_ok_latch) == FaultLatchState::OK);
    app::can_tx::BMS_ImdCurrentlyOk_set(getCurrentStatus(&imd_ok_latch) == FaultLatchState::OK);
    app::can_tx::BMS_BspdCurrentlyOk_set(getCurrentStatus(&bspd_ok_latch) == FaultLatchState::OK);
    app::can_tx::BMS_BmsLatchOk_set(getLatchedStatus(&bms_ok_latch) == FaultLatchState::OK);
    app::can_tx::BMS_ImdLatchOk_set(getLatchedStatus(&imd_ok_latch) == FaultLatchState::OK);
    app::can_tx::BMS_BspdLatchOk_set(getLatchedStatus(&bspd_ok_latch) == FaultLatchState::OK);

    app::can_tx::BMS_BSPDBrakePressureThresholdExceeded_set(io::bspdtest::isBrakePressureThresholdExceeded());
    app::can_tx::BMS_BSPDAccelBrakeOk_set(io::bspdtest::isAccelBrakeOk());

    // commnet back in
    const bool ir_negative_opened_debounced = app::irs::negativeOpenedDebounced();
    const bool balancing_enabled            = app::can_rx::Debug_CellBalancingRequest_get();

    if (app::can_alerts::AnyBoardHasFault())
    {
        app::StateMachine::set_next_state(&app::states::fault_state);
    }
    else if (ir_negative_opened_debounced)
    {
        app::StateMachine::set_next_state(&app::states::init_state);
    }
    else if (balancing_enabled)
    {
        app::StateMachine::set_next_state(&app::states::balancing_state);
    }

    app::irs::broadcast();

    io::can_tx::enqueue100HzMsgs();
}

void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}

void jobs_runAdbmsVoltages_tick()
{
    app::segments::Cell        voltages;
    app::segments::CellSuccess voltages_success;

    {
        const io::unique_semaphore s{ spi_bus_lock };
        LOG_IF_ERR(io::adbms::wakeup());
        LOG_IF_ERR(app::segments::configSync());
        LOG_IF_ERR(app::segments::runVoltageConversion(voltages, voltages_success));
    }
    {
        const io::unique_semaphore s{ adbms_app_lock };
        app::segments::broadcastCellVoltages(voltages, voltages_success);
    }
}

void jobs_runAdbmsFilteredVoltages_tick()
{
    app::segments::Cell        voltages;
    app::segments::CellSuccess voltages_success;

    {
        const io::unique_semaphore s{ spi_bus_lock };
        LOG_IF_ERR(io::adbms::wakeup());
        LOG_IF_ERR(app::segments::configSync());
        LOG_IF_ERR(app::segments::runFilteredVoltageConversion(voltages, voltages_success));
    }

    app::segments::broadcastFilteredCellVoltages(voltages, voltages_success);
}

void jobs_runAdbmsTemperatures_tick()
{
    app::segments::Therm          temps;
    app::segments::ThermSuccess   temp_success;
    app::segments::Segment        seg_voltages;
    app::segments::SegmentSuccess seg_voltages_success;

    {
        const io::unique_semaphore s{ spi_bus_lock };
        LOG_IF_ERR(io::adbms::wakeup());
        LOG_IF_ERR(app::segments::configSync());
        LOG_IF_ERR(app::segments::runTempConversion(temps, temp_success));
        LOG_IF_ERR(app::segments::runSegVoltageConversion(seg_voltages, seg_voltages_success));
    }
    {
        const io::unique_semaphore s{ adbms_app_lock };
        app::segments::broadcastTemps(temps, temp_success);
    }
    app::segments::broadcastSegVoltages(seg_voltages, seg_voltages_success);
}

void jobs_runAdbmsDiagnostics_tick()
{
    app::segments::Status         status;
    app::segments::SegmentSuccess status_success;
    app::segments::Owc            owc;
    app::segments::CellSuccess    owc_success;

    {
        const io::unique_semaphore s{ spi_bus_lock };
        LOG_IF_ERR(io::adbms::wakeup());
        LOG_IF_ERR(app::segments::configSync());
        LOG_IF_ERR(app::segments::runStatusConversion(status, status_success));
        LOG_IF_ERR(app::segments::runCellOpenWireCheck(owc, owc_success));
    }
    {
        const io::unique_semaphore s{ adbms_app_lock };
        app::segments::broadcastCellOpenWireCheck(owc, owc_success);
        app::segments::broadcastInfo();
    }
    app::segments::broadcastStatus(status, status_success);
}