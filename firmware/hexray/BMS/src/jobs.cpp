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

using io::adbms::Cells;
using io::adbms::OpenWireSwitch;
using io::adbms::Segments;
using io::adbms::ThermGpios;
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

    using FaultLatchState = io::FaultLatch::FaultLatchState;
    const bool acc_fault  = app::segments::alerts::tick();
    bms_ok_latch.setCurrentStatus(acc_fault ? FaultLatchState::FAULT : FaultLatchState::OK);
    app::latches::broadcast();

    // BSPD
    io::bspdtest::enable(app::can_rx::Debug_EnableTestCurrent_get());
    app::can_tx::BMS_BSPDCurrentThresholdExceeded_set(io::bspdtest::isCurrentThresholdExceeded());
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

static hw::notify::Notifier sync_done;

void jobs_runAdbmsConfigs_tick()
{
    const io::unique_semaphore s{ spi_bus_lock };

    const auto res             = app::segments::config::sync();
    bool       all_segments_ok = true;
    for (size_t seg_num = 0; seg_num < NUM_SEGMENTS; seg_num++)
    {
        const auto &seg_res = res[seg_num];
        app::segments::health::setOrReset(seg_num, app::segments::health::ErrorBit::CONFIG, not seg_res);
        if (!seg_res)
        {
            all_segments_ok = false;
            LOG_ERROR("Failed to sync config on segment %d: %s", seg_num, error_code_to_string(seg_res.error()));
            continue;
        }
        if (!seg_res.value())
        {
            all_segments_ok = false;
            LOG_ERROR("Failed to sync config on segment %d: ADBMS config did not match in-memory config", seg_num);
        }
    }
    // TODO handle this properly
    LOG_IF_ERR(io::adbms::clear::cell());
    LOG_IF_ERR(io::adbms::clear::aux());
    LOG_IF_ERR(io::adbms::clear::stat());
    if (all_segments_ok)
    {
        LOG_INFO("All configs ok! Notifying...");
        sync_done.notify();
    }
}

void jobs_runAdbmsVoltages_tick()
{
    sync_done.wait();
    LOG_INFO("Starting voltage poll and conversion");

    result<void>         voltages_poll_ok;
    Cells<result<float>> cell_voltages;

    result<void>                                                                         owc_voltages_poll_ok;
    std::array<Cells<result<float>>, static_cast<size_t>(OpenWireSwitch::CHANNEL_COUNT)> owc_voltages;

    {
        const io::unique_semaphore s{ spi_bus_lock };

        // Cell Voltages
        voltages_poll_ok = app::segments::startPoll::cellAdc();
        if (voltages_poll_ok)
            cell_voltages = app::segments::conversion::cellVoltage();

        // Cell Open Wire Check
        for (const OpenWireSwitch channel : { OpenWireSwitch::ODD_CHANNELS, OpenWireSwitch::EVEN_CHANNELS })
        {
            const auto poll1 = app::segments::startPoll::secondaryCellAdc(channel);
            const auto poll2 = app::segments::startPoll::secondaryCellAdc(channel);
            if (poll1 && poll2)
            {
                owc_voltages[static_cast<size_t>(channel)] = app::segments::conversion::cellOwcVoltages();
            }
            else if (owc_voltages_poll_ok)
            {
                owc_voltages_poll_ok = std::unexpected((!poll1 ? poll1 : poll2).error());
            }
        }
    }

    const Cells<result<bool>> cell_owc = app::segments::calculate::cellOwc(owc_voltages);

    app::segments::shared::setVoltageStats(cell_voltages, cell_owc);

    app::segments::broadcast::voltageStats();
    app::segments::broadcast::segmentHealthError();
    app::segments::broadcast::debug::cellVoltages(cell_voltages, voltages_poll_ok);
    app::segments::broadcast::debug::cellOwc(cell_owc, owc_voltages_poll_ok);
}

void jobs_runAdbmsAux_tick()
{
    sync_done.wait();
    LOG_INFO("Starting AUX poll and conversion");

    result<void> therm_voltages_poll_ok;
    std::array<ThermGpios<result<float>>, static_cast<size_t>(app::segments::ThermistorMux::THERMISTOR_MUX_COUNT)>
                                         therm_voltages;
    Segments<result<float>>              seg_voltage;
    Segments<io::adbms::StatusGroupsRes> status;

    {
        const io::unique_semaphore s{ spi_bus_lock };

        // Thermistor Aux
        for (const app::segments::ThermistorMux mux :
             { app::segments::ThermistorMux::THERMISTOR_MUX_0_7, app::segments::ThermistorMux::THERMISTOR_MUX_8_13 })
        {
            const auto poll = app::segments::startPoll::auxAdc(mux);
            if (poll)
            {
                therm_voltages[static_cast<size_t>(mux)] = app::segments::conversion::thermVoltage(mux);
            }
            else if (therm_voltages_poll_ok)
            {
                therm_voltages_poll_ok = std::unexpected(poll.error());
            }
        }

        seg_voltage = app::segments::conversion::segVoltage();
        status      = app::segments::conversion::status();
    }

    const Therms<result<float>> therm_temps = app::segments::calculate::thermTemps(therm_voltages);
    const Therms<result<bool>>  therm_owc   = app::segments::calculate::thermOwc(therm_voltages);

    app::segments::shared::setTemperatureStats(therm_temps, therm_owc);

    app::segments::broadcast::temperatureStats();
    app::segments::broadcast::segmentHealthError();
    app::segments::broadcast::debug::thermTemps(therm_temps, therm_voltages_poll_ok);
    app::segments::broadcast::debug::thermOwc(therm_owc, therm_voltages_poll_ok);
    app::segments::broadcast::debug::segVoltages(seg_voltage);
    app::segments::broadcast::debug::status(status);
}
