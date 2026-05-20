#include "jobs.hpp"
#include "io/io_adbms.hpp"
#include "util_errorCodes.hpp"

// app
#include "app_bmsShdnLoop.hpp"
#include "app_canAlerts.hpp"
#include "app_canRx.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "app_commitInfo.hpp"
#include "app_heartbeatMonitor.hpp"
#include "app_heartbeatMonitors.hpp"
#include "app_irs.hpp"
#include "app_jsoncan.hpp"
#include "app_powerLimit.hpp"
#include "app_precharge.hpp"
#include "app_segments.hpp"
#include "app_states.hpp"
#include "app_tractiveSystem.hpp"

// io
#include "app_latches.hpp"
#include "io_bspdTest.hpp"
#include "io_canMsg.hpp"
#include "io_canQueues.hpp"
#include "io_canTx.hpp"
#include "io_charger.hpp"
#include "io_fans.hpp"
#include "io_faultLatch.hpp"
#include "io_semaphore.hpp"
#include "io_time.hpp"

using io::adbms::Cells;
using io::adbms::Segments;
using io::adbms::Status;
using io::adbms::Therms;

io::semaphore spi_bus_lock(true);

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
    LOG_IF_ERR(io::adbms::wakeup());
    LOG_IF_ERR(io::adbms::clear::CellVoltageReg());
    LOG_IF_ERR(io::adbms::clear::FilteredCellVoltageReg());
    LOG_IF_ERR(io::adbms::clear::CellAuxReg());
    LOG_IF_ERR(io::adbms::clear::StatReg());
    LOG_INFO("Segment Initialization Done");
#endif
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
    app::shdn::bms_shdnLoop.broadcast();
    app::plim::broadcast();

    // heartbeat
    hb_monitor.checkIn();
    hb_monitor.broadcastFaults();

    // TODO: Enable fans for endurance when contactors are closed.
    io::fans::tick(false);

    // Charger connection status
    app::can_tx::BMS_ChargerConnectedType_set(io::charger::getConnectionStatus());

    // Fault Latches
    using FaultLatchState = io::FaultLatch::FaultLatchState;
#ifdef TARGET_HV_SUPPLY
    const bool acc_fault = false;
#else
    app::segments::faults::checkWarnings();
    const bool acc_fault = app::segments::faults::checkFaults();
#endif
    bms_ok_latch.setCurrentStatus(acc_fault ? FaultLatchState::FAULT : FaultLatchState::OK);
    app::latches::broadcast();

    // BSPD
    io::bspdtest::enable(app::can_rx::Debug_EnableTestCurrent_get());
    app::can_tx::BMS_BSPDCurrentThresholdExceeded_set(io::bspdtest::isCurrentThresholdExceeded());
    app::can_tx::BMS_BSPDBrakePressureThresholdExceeded_set(io::bspdtest::isBrakePressureThresholdExceeded());
    app::can_tx::BMS_BSPDAccelBrakeOk_set(io::bspdtest::isAccelBrakeOk());

    // STATE TRANSITIONS
    if (app::can_alerts::AnyBoardHasFault())
    {
        app::StateMachine::set_next_state(&app::states::fault_state);
    }
    else if (app::irs::negativeOpenedDebounced())
    {
        app::StateMachine::set_next_state(&app::states::init_state);
    }
    else if (app::can_rx::Debug_CellBalancing_Request_get())
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
    app::segments::state::resetAll(app::segments::state::Bit::Voltage);

    result<Cells<result<float>>> volt_r;
    {
        const io::unique_semaphore s{ spi_bus_lock };
        volt_r = app::segments::runVoltageConversion();
    }

    if (volt_r)
    {
        app::segments::state::setAll(app::segments::state::Bit::Voltage);
        app::segments::broadcast::cellVoltages(volt_r.value());
    }

    result<Cells<result<bool>>> owc_r;
    {
        const io::unique_semaphore s{ spi_bus_lock };
        owc_r = app::segments::runCellOpenWireCheck();
    }
    if (owc_r)
    {
        app::segments::broadcast::owc(owc_r.value());
    }
}

void jobs_runAdbmsConfigs_tick()
{
    app::segments::state::resetAll(app::segments::state::Bit::Config);

    result<void> sync_result;
    {
        const io::unique_semaphore s{ spi_bus_lock };
        sync_result = app::segments::config::configSync();
    }
    if (sync_result)
        app::segments::state::setAll(app::segments::state::Bit::Config);
}

void jobs_runAdbmsTemperatures_tick()
{
    app::segments::state::resetAll(app::segments::state::Bit::Temp);

    result<std::pair<Therms<result<float>>, Therms<result<bool>>>> temp_r;
    {
        const io::unique_semaphore s{ spi_bus_lock };
        temp_r = app::segments::runAuxConversion();
    }
    if (temp_r)
    {
        // Therms<result<float>> &temp_results      = std::get<0>(temp_r.value());
        // Therms<result<bool>>  &therm_owc_results = std::get<1>(temp_r.value());
        app::segments::state::setAll(app::segments::state::Bit::Temp);
    }
    else
    {
        temp_r = {};
        for (io::adbms::SegmentTherms<result<float>> &seg : std::get<0>(temp_r.value()))
            seg.fill(std::unexpected(temp_r.error()));
        for (io::adbms::SegmentTherms<result<bool>> &seg : std::get<1>(temp_r.value()))
            seg.fill(std::unexpected(temp_r.error()));
        // std::get<2>(temp_r.value()).fill(std::unexpected(temp_r.error()));
    }
    app::segments::broadcast::temps(std::get<0>(temp_r.value()), std::get<1>(temp_r.value()));
    // app::segments::broadcast::segVoltages(std::get<2>(temp_r.value()));
}

void jobs_runAdbmsDiagnostics_tick()
{
    app::segments::state::resetAll(app::segments::state::Bit::Status);

    result<Segments<io::adbms::StatusGroups>> stat_r;
    {
        const io::unique_semaphore s{ spi_bus_lock };
        stat_r = app::segments::runStatusConversion();
    }

    if (stat_r)
    {
        app::segments::state::setAll(app::segments::state::Bit::Status);
    }
    else
    {
        stat_r = {};
        for (auto &sg : stat_r.value())
            sg = io::adbms::StatusGroups::makeError(stat_r.error());
    }
    app::segments::broadcast::status(stat_r.value());
}
