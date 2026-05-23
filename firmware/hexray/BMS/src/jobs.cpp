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
    LOG_IF_ERR(io::adbms::command::wakeup());
    LOG_IF_ERR(io::adbms::clear::cell());
    LOG_IF_ERR(io::adbms::clear::aux());
    LOG_IF_ERR(io::adbms::clear::stat());
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
    // app::segments::faults::checkWarnings();
    // const bool acc_fault = app::segments::faults::checkFaults();
    const bool acc_fault = false;
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
    } else if (app::can_alerts::AnyBoardHasFault())
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

void jobs_runAdbmsVoltages_tick()
{
    app::segments::config::waitForSync();

    Cells<result<float>> voltages;
    Cells<result<bool>> owc;

    {  
        const io::unique_semaphore s{ spi_bus_lock };
        voltages = app::segments::conversion::cellVoltage();
        owc = app::segments::conversion::cellOwc();
    }
    
    app::segments::broadcast::cellVoltages(voltages);
    app::segments::broadcast::cellOwc(owc);
    
}

void jobs_runAdbmsConfigs_tick()
{
    const io::unique_semaphore s{ spi_bus_lock };
    app::segments::config::configSync();
    LOG_INFO("CONFIG RAN");
}

void jobs_runAdbmsAux_tick()
{
    LOG_INFO("WAITING");
    app::segments::config::waitForSync();
    LOG_INFO("RESUME");

    // std::pair<Therms<result<float>>, Therms<result<bool>>> therm;
    // Segments<io::adbms::StatusGroups>  status;
    // Segments<result<float>> seg_voltage;
    
    // {
    //     const io::unique_semaphore s{ spi_bus_lock };
    //     LOG_IF_ERR(io::adbms::clear::stat()); //not sure how to handle error for this
    //     therm = app::segments::conversion::thermTempOwc();
    //     status =  app::segments::conversion::status();
    //     seg_voltage = app::segments::conversion::segVoltage();
    // }

    // app::segments::broadcast::thermTemps(therm.first);
    // app::segments::broadcast::thermOwc(therm.second);
    // app::segments::broadcast::status(status);
    // app::segments::broadcast::segVoltages(seg_voltage);
}
