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
#include "app_segments_internal.hpp"
#include "app_powerLimit.hpp"
#include "app_bmsShdnLoop.hpp"
#include "app_tractiveSystem.hpp"
#include "app_irs.hpp"
#include "app_heartbeatMonitors.hpp"
#include "app_heartbeatMonitor.hpp"
#include "app_jsoncan.hpp"
#include "app_canUtils.hpp"
#include "app_commitInfo.hpp"

#include "io_canQueues.hpp"
#include "io_canMsg.hpp"
#include "io_time.hpp"
#include "io_semaphore.hpp"
#include "io_bspdTest.hpp"
#include "io_charger.hpp"
#include "io_fans.hpp"
#include "io_faultLatch.hpp"
#include "io_canTx.hpp"

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
    const bool balancing_enabled            = app::can_rx::Debug_CellBalancing_Request_get();

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
    app::segments::health::resetAll(app::segments::health::Bit::Voltage);

    std::expected<Cells<std::expected<float, ErrorCode>>, ErrorCode> volt_r;
    std::expected<Cells<std::expected<bool, ErrorCode>>, ErrorCode>  owc_r;
    {
        const io::unique_semaphore s{ spi_bus_lock };
        volt_r = app::segments::runVoltageConversion();
        owc_r  = app::segments::runCellOpenWireCheck();
    }

    Cells<std::expected<float, ErrorCode>> voltages{};
    if (volt_r)
    {
        voltages = volt_r.value();
        app::segments::health::setAll(app::segments::health::Bit::Voltage);
    }

    Cells<std::expected<bool, ErrorCode>> owc{};
    if (owc_r)
        owc = owc_r.value();

    app::segments::broadcast::cellVoltages(voltages);
    app::segments::broadcast::owc(owc);
}

void jobs_runAdbmsConfigs_tick()
{
    app::segments::health::resetAll(app::segments::health::Bit::Config);

    std::expected<void, ErrorCode> sync_result;
    {
        const io::unique_semaphore s{ spi_bus_lock };
        sync_result = app::segments::config::configSync();
    }
    if (sync_result)
        app::segments::health::setAll(app::segments::health::Bit::Config);
}

void jobs_runAdbmsTemperatures_tick()
{
    app::segments::health::resetAll(app::segments::health::Bit::Temp);

    std::expected<Therms<std::expected<float, ErrorCode>>, ErrorCode>   temp_r;
    std::expected<Segments<std::expected<float, ErrorCode>>, ErrorCode> seg_r;
    {
        const io::unique_semaphore s{ spi_bus_lock };
        temp_r = app::segments::runTempConversion();
        seg_r  = app::segments::runSegVoltageConversion();
    }

    Therms<std::expected<float, ErrorCode>> temp_results{};
    if (temp_r)
    {
        temp_results = temp_r.value();
        app::segments::health::setAll(app::segments::health::Bit::Temp);
    }
    else
    {
        for (auto &seg : temp_results)
            seg.fill(std::unexpected(temp_r.error()));
    }

    Segments<std::expected<float, ErrorCode>> seg_voltage_results{};
    if (seg_r)
        seg_voltage_results = seg_r.value();
    else
        seg_voltage_results.fill(std::unexpected(seg_r.error()));

    app::segments::broadcast::temps(temp_results);
    app::segments::broadcast::segVoltages(seg_voltage_results);
}

void jobs_runAdbmsDiagnostics_tick()
{
    app::segments::health::resetAll(app::segments::health::Bit::Status);

    std::expected<Segments<io::adbms::StatusGroups>, ErrorCode> stat_r;
    {
        const io::unique_semaphore s{ spi_bus_lock };
        stat_r = app::segments::runStatusConversion();
    }

    Status status{};
    if (stat_r)
    {
        status = stat_r.value();
        app::segments::health::setAll(app::segments::health::Bit::Status);
    }
    else
    {
        for (auto &sg : status)
            sg = io::adbms::StatusGroups::makeError(stat_r.error());
    }

    app::segments::broadcast::status(status);
}