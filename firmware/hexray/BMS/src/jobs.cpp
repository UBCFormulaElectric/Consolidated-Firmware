#include "jobs.hpp"
#include "util_errorCodes.hpp"

#include <algorithm>
#include <ranges>

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
#include "io_notify.hpp"

using io::adbms::Cells;
using io::adbms::OpenWireSwitch;
using io::adbms::Segments;
using io::adbms::ThermGpios;
using io::adbms::Therms;

static void jsoncan_transmit_func(const JsonCanMsg &tx_msg)
{
    const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
    const auto       res = can_tx_queue.push(msg);
    LOG_IF_ERR(res);
    if (not res)
    {
        LOG_ERROR("failed on can id %d", tx_msg.std_id);
    }
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
    io::can_tx::BMS_Bootup_sendAperiodic();

    app::precharge::init();
    app::segments::alerts::init();

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

#ifdef TARGET_HV_SUPPLY
    const bool acc_fault = false;
#else
    bool acc_fault = false;
    {
        io::unique_semaphore h{ health_lock };
        io::unique_semaphore s{ shared_lock };
        acc_fault = app::segments::alerts::tick();
    }
#endif
    using FaultLatchState = io::FaultLatch::FaultLatchState;
    bms_ok_latch.setCurrentStatus(acc_fault ? FaultLatchState::FAULT : FaultLatchState::OK);
    bms_ok_latch.setCurrentStatus(FaultLatchState::OK);
    app::latches::broadcast();

    // Update CAN signals for BMS latch statuses.
    const bool imd_latched_ok  = imd_ok_latch.getLatchedStatus() == FaultLatchState::OK;
    const bool bspd_latched_ok = bspd_ok_latch.getLatchedStatus() == FaultLatchState::OK;
    const bool bms_latched_ok  = bms_ok_latch.getLatchedStatus() == FaultLatchState::OK;

    app::can_alerts::faults::ImdLatched_set(not imd_latched_ok);
    app::can_alerts::faults::HardwareBspdLatched_set(not bspd_latched_ok);
    app::can_alerts::faults::BmsLatched_set(not bms_latched_ok);

    app::can_tx::BMS_BmsCurrentlyOk_set(bms_ok_latch.getCurrentStatus() == FaultLatchState::OK);
    app::can_tx::BMS_ImdCurrentlyOk_set(imd_ok_latch.getCurrentStatus() == FaultLatchState::OK);
    app::can_tx::BMS_BspdCurrentlyOk_set(bspd_ok_latch.getCurrentStatus() == FaultLatchState::OK);
    app::can_tx::BMS_BmsLatchOk_set(bms_latched_ok);
    app::can_tx::BMS_ImdLatchOk_set(imd_latched_ok);
    app::can_tx::BMS_BspdLatchOk_set(bspd_latched_ok);

    app::can_tx::BMS_BSPDBrakePressureThresholdExceeded_set(io::bspdtest::isBrakePressureThresholdExceeded());
    app::can_tx::BMS_BSPDAccelBrakeOk_set(io::bspdtest::isAccelBrakeOk());

    // STATE TRANSITIONS
    if (app::can_rx::Debug_CellBalancing_Request_get())
    {
        app::StateMachine::set_next_state(&app::states::balancing_state);
    }
    else if (app::can_alerts::AnyBoardHasFault())
    {
        app::StateMachine::set_next_state(&app::states::fault_state);
    }
    else if (app::irs::negativeOpenedDebounced())
    {
        app::StateMachine::set_next_state(&app::states::init_state);
    }

    app::irs::broadcast();
    io::can_tx::enqueue100HzMsgs();
}

void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}

static io::notify::Notifier sync_done;

void jobs_runAdbmsConfigs_tick()
{
    bool all_segments_ok = true;

    const Segments<result<bool>> check_res = app::segments::reach::check();
    {
        const io::unique_semaphore h{ health_lock };
        for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
        {
            const auto &seg_res = check_res[seg];
            app::segments::health::setOrReset(seg, app::segments::health::ErrorBit::UNREACHABLE, not seg_res);
            if (!seg_res)
            {
                all_segments_ok = false;
                LOG_ERROR(
                    "Failed to run reachable check on segment %d: %s", (int)seg, error_code_to_string(seg_res.error()));
                continue;
            }
            if (!seg_res.value())
            {
                all_segments_ok = false;
                LOG_ERROR("Failed to reach segment %d: SPI link break detected", (int)seg);
            }
        }
    }

    const Segments<result<bool>> sync_res = app::segments::config::sync();
    {
        const io::unique_semaphore h{ health_lock };
        for (size_t seg_num = 0; seg_num < NUM_SEGMENTS; seg_num++)
        {
            const auto &seg_res = sync_res[seg_num];
            app::segments::health::setOrReset(seg_num, app::segments::health::ErrorBit::CONFIG, not seg_res);
            if (!seg_res)
            {
                all_segments_ok = false;
                LOG_ERROR(
                    "Failed to sync config on segment %d: %s", (int)seg_num, error_code_to_string(seg_res.error()));
                continue;
            }
            if (!seg_res.value())
            {
                all_segments_ok = false;
                LOG_ERROR(
                    "Failed to sync config on segment %d: ADBMS config did not match in-memory config", (int)seg_num);
            }
        }
    }

    // Segments<uint8_t>      mismatches;
    // io::adbms::SpiBusReach spi_reach;
    // {
    //     const io::unique_semaphore c{ internal_lock };
    //     mismatches = io::adbms::misc::getCmdCountMismatches();
    //     spi_reach  = io::adbms::misc::getSpiBusReach();
    // }

    std::array<std::bitset<app::segments::health::NUM_HEALTH_BITS>, MAX_NUM_SEGMENTS> health;
    {
        const io::unique_semaphore h{ health_lock };
        health = app::segments::health::getAll();
    }

    app::segments::broadcast::segmentHealthError(health);
    // app::segments::broadcast::cmdCountMismatch(mismatches);
    // app::segments::broadcast::spiLinkStats(spi_reach);

    if (all_segments_ok)
    {
        LOG_INFO("All segments reachable and synced! Notifying...");
        sync_done.notify();
    }
}

void jobs_runAdbmsVoltages_tick()
{
    sync_done.wait();
    LOG_INFO("Starting voltage readings");
    LOG_IF_ERR(io::adbms::clear::cell());

    result<void> cell_voltages_start_ok = io::adbms::command::startCellsAdc();
    {
        const io::unique_semaphore h{ health_lock };
        app::segments::health::setOrResetAll(
            app::segments::health::ErrorBit::CELL_ADC_START, not cell_voltages_start_ok);
    }

    if (not cell_voltages_start_ok)
    {
        app::segments::broadcast::debug::cellVoltages(Cells<result<float>>{}, cell_voltages_start_ok);
        return;
    }

    Cells<result<float>> cell_voltages = app::segments::conversion::cellVoltage();
    {
        const io::unique_semaphore h{ health_lock };
        for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
        {
            const bool seg_err = std::ranges::any_of(cell_voltages[seg], [](const result<float> &r) { return not r; });
            app::segments::health::setOrReset(seg, app::segments::health::ErrorBit::CELL_VOLTAGE, seg_err);
        }
    }

    app::segments::CellParam<float> max_voltage;
    app::segments::CellParam<float> min_voltage;
    {
        const io::unique_semaphore s{ shared_lock };
        app::segments::shared::setVoltageStats(cell_voltages);
        max_voltage = app::segments::shared::getMaxCellVoltage();
        min_voltage = app::segments::shared::getMinCellVoltage();
    }

    app::segments::health::Snapshot health;
    {
        const io::unique_semaphore h{ health_lock };
        health = app::segments::health::getAll();
    }

    app::segments::broadcast::segmentHealthError(health);
    app::segments::broadcast::cellVoltageStats(min_voltage, max_voltage);
    app::segments::broadcast::debug::cellVoltages(cell_voltages, result<void>{});
}

void jobs_runAdbmsCellOwc_tick()
{
    sync_done.wait();
    LOG_INFO("Starting cell open wire check");

    std::array<Cells<result<float>>, static_cast<size_t>(OpenWireSwitch::CHANNEL_COUNT)> owc_voltages;

    for (const OpenWireSwitch channel : { OpenWireSwitch::ODD_CHANNELS, OpenWireSwitch::EVEN_CHANNELS })
    {
        LOG_IF_ERR(io::adbms::clear::secondaryCell());
        const auto owc_start_ok = io::adbms::command::owcCells(channel);
        {
            const io::unique_semaphore h{ health_lock };
            app::segments::health::setOrResetAll(app::segments::health::ErrorBit::OWC_ADC_START, not owc_start_ok);
        }

        if (not owc_start_ok)
        {
            app::segments::broadcast::debug::cellOwc(Cells<result<bool>>{}, owc_start_ok);
            return;
        }

        owc_voltages[static_cast<size_t>(channel)] = app::segments::conversion::cellOwcVoltages();
    }

    {
        const io::unique_semaphore h{ health_lock };
        for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
        {
            bool seg_err = false;
            for (const OpenWireSwitch channel : { OpenWireSwitch::ODD_CHANNELS, OpenWireSwitch::EVEN_CHANNELS })
            {
                seg_err = seg_err || std::ranges::any_of(
                                         owc_voltages[static_cast<size_t>(channel)][seg],
                                         [](const result<float> &r) { return not r; });
            }
            app::segments::health::setOrReset(seg, app::segments::health::ErrorBit::CELL_OWC_VOLTAGE, seg_err);
        }
    }

    const Cells<result<bool>> cell_owc = app::segments::calculate::cellOwc(owc_voltages);

    {
        io::unique_semaphore s{ shared_lock };
        app::segments::shared::setCellOwc(cell_owc);
    }

    app::segments::health::Snapshot health;
    {
        const io::unique_semaphore h{ health_lock };
        health = app::segments::health::getAll();
    }

    app::segments::broadcast::segmentHealthError(health);
    app::segments::broadcast::debug::cellOwc(cell_owc, result<void>{});
}

void jobs_runAdbmsAux_tick()
{
    sync_done.wait();
    LOG_INFO("Starting AUX readings");
    LOG_IF_ERR(io::adbms::clear::flags());

    std::array<ThermGpios<result<float>>, static_cast<size_t>(app::segments::ThermistorMux::THERMISTOR_MUX_COUNT)>
        therm_voltages;

    for (const app::segments::ThermistorMux mux :
         { app::segments::ThermistorMux::THERMISTOR_MUX_0_7, app::segments::ThermistorMux::THERMISTOR_MUX_8_13 })
    {
        app::segments::config::setThermistorConfig(mux);
        sync_done.notify();
        io::time::delay(100); // todo: expirement with this
        LOG_IF_ERR(io::adbms::clear::aux());

        const auto therm_voltage_start_ok = io::adbms::command::startAuxAdc();
        {
            const io::unique_semaphore h{ health_lock };
            app::segments::health::setOrResetAll(
                app::segments::health::ErrorBit::AUX_ADC_START, not therm_voltage_start_ok);
        }
        if (not therm_voltage_start_ok)
        {
            app::segments::broadcast::debug::thermTemps(Therms<result<float>>{}, therm_voltage_start_ok);
            app::segments::broadcast::debug::thermOwc(Therms<result<bool>>{}, therm_voltage_start_ok);
            return;
        }

        const auto therm_voltage_poll_ok = io::adbms::command::pollAuxAdc();
        {
            const io::unique_semaphore h{ health_lock };
            app::segments::health::setOrResetAll(
                app::segments::health::ErrorBit::AUX_ADC_POLL, not therm_voltage_poll_ok);
        }
        if (not therm_voltage_poll_ok)
        {
            app::segments::broadcast::debug::thermTemps(Therms<result<float>>{}, therm_voltage_poll_ok);
            app::segments::broadcast::debug::thermOwc(Therms<result<bool>>{}, therm_voltage_poll_ok);
            return;
        }

        therm_voltages[static_cast<size_t>(mux)] = app::segments::conversion::thermVoltage();
    }

    const Segments<result<float>> seg_voltages = app::segments::conversion::segVoltage();
    const Segments<io::adbms::StatusGroupsRes> status       = io::adbms::read::status();

    const Therms<result<float>> therm_temps = app::segments::calculate::thermTemps(therm_voltages);
    const Therms<result<bool>>  therm_owc   = app::segments::calculate::thermOwc(therm_voltages);

    result<float>                      pack_voltage;
    app::segments::CellParam<float>    max_temp;
    app::segments::CellParam<float>    min_temp;
    app::segments::SegmentParam<float> max_voltage;
    app::segments::SegmentParam<float> min_voltage;
    {
        io::unique_semaphore s{ shared_lock };
        app::segments::shared::setThermistorOwc(therm_owc);
        app::segments::shared::setTemperatureStats(therm_temps);
        app::segments::shared::setSegmentVoltageStats(seg_voltages);
        pack_voltage = app::segments::shared::getPackVoltage();
        max_temp     = app::segments::shared::getMaxCellTemperature();
        min_temp     = app::segments::shared::getMinCellTemperature();
        max_voltage  = app::segments::shared::getMaxSegmentVoltage();
        min_voltage  = app::segments::shared::getMinSegmentVoltage();
    }

    app::segments::health::Snapshot health;
    {
        const io::unique_semaphore h{ health_lock };
        health = app::segments::health::getAll();
    }

    app::segments::broadcast::segmentHealthError(health);
    app::segments::broadcast::packVoltage(pack_voltage);
    app::segments::broadcast::cellTempStats(min_temp, max_temp);
    app::segments::broadcast::segmentVoltageStats(min_voltage, max_voltage);

    app::segments::broadcast::debug::thermTemps(therm_temps, result<void>{});
    app::segments::broadcast::debug::thermOwc(therm_owc, result<void>{});
    app::segments::broadcast::debug::segVoltages(seg_voltages);
    app::segments::broadcast::debug::status(status);
}
