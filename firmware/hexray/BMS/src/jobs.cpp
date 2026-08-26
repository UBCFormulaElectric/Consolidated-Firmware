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
#include "app_pack.hpp"
#include "app_powerLimit.hpp"
#include "app_precharge.hpp"
#include "app_states.hpp"
#include "app_tractiveSystem.hpp"
#include "app_charger.hpp"

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
#include "io_irs.hpp"

using io::adbms::Cells;
using io::adbms::OpenWireSwitch;
using io::adbms::Segments;
using io::adbms::ThermGpios;
using io::adbms::Therms;

static void vehicle_transmit_func(const JsonCanMsg &tx_msg)
{
    const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
    const auto       res = vehicle_can_tx_queue.push(msg);
    // LOG_IF_ERR(res);
    // if (not res)
    // {
    //     LOG_ERROR("failed on can id %d", tx_msg.std_id);
    // }
}

static void charger_transmit_func(const JsonCanMsg &tx_msg)
{
    const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
    const auto       res = charger_can_tx_queue.push(msg);
    // LOG_IF_ERR(res);
    // if (not res)
    // {
    //     LOG_ERROR("failed on can id %d", tx_msg.std_id);
    // }
}

void jobs_init()
{
    io::can_tx::init(vehicle_transmit_func, charger_transmit_func);
    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEFAULT, true);
    io::can_tx::enableMode_charger(app::can_utils::chargerMode::CHARGER_MODE_DEFAULT, true);

    can_rx_queue.init();
    vehicle_can_tx_queue.init();
    charger_can_tx_queue.init();

    app::can_tx::BMS_Hash_set(GIT_COMMIT_HASH);
    app::can_tx::BMS_Clean_set(GIT_COMMIT_CLEAN);
    app::can_tx::BMS_Heartbeat_set(true);
    io::can_tx::BMS_Bootup_sendAperiodic();

    app::precharge::init();

    app::pack::sequence::init();
    app::pack::alerts::init();

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
    io::can_tx::enableMode_charger(app::can_utils::chargerMode::CHARGER_MODE_DEBUG, debug_mode_enabled);

    app::ts::broadcast();
    app::shdn::bms_shdnLoop.broadcast();
    app::plim::broadcast();

    // heartbeat
    hb_monitor.checkIn();
    hb_monitor.broadcastFaults();
    // NOTE: Efuse is shorted — no software enable needed
    const bool vc_drive_state         = app::can_rx::VC_State_get() == app::can_utils::VCState::VC_DRIVE_STATE;
    const bool cell_balancing_request = app::can_rx::Debug_CellBalancing_Request_get();
    const bool charging_request       = app::can_rx::Debug_StartCharging_get();
    const bool fans_enabled           = vc_drive_state || cell_balancing_request || charging_request;
    io::fans::tick(fans_enabled);

    app::can_tx::BMS_FanStatus_set(true);
#ifdef TARGET_EMBEDDED
    // app::can_tx::BMS_FanPgood_set(fan_pgood.readPin());
#else
    app::can_tx::BMS_FanPgood_set(false);
#endif

    // Charger connection status
    app::can_tx::BMS_ChargerConnectedType_set(io::charger::getConnectionStatus());
    app::charger::broadcast();

    /*
    if (voltage data noti arrives) check voltFault
    if (temp data noti arrives) check tempFault
    if (owc data noti arrives) check owcFault
    if (ADBMS6830Diag data noti arrives) check check ADBMS6830Fault
    packFault = voltFault || tempFault || owcFault || ADBMS6830Fault
    bms_ok_latcch.setCurrentStatus(pack_fault ? io::FaultLatch::FaultLatchState::FAULT);
    */
    
    app::latches::broadcast();

    io::bspdtest::enable(app::can_rx::Debug_EnableTestCurrent_get());
    app::can_tx::BMS_BSPDBrakePressureThresholdExceeded_set(io::bspdtest::isBrakePressureThresholdExceeded());
    app::can_tx::BMS_BSPDAccelBrakeOk_set(io::bspdtest::isAccelBrakeOk());
    app::can_tx::BMS_BSPDCurrentThresholdExceeded_set(io::bspdtest::isCurrentThresholdExceeded());

    if (app::can_alerts::AnyBoardHasFault())
    {
        app::StateMachine::set_next_state(&app::states::fault_state);
    }

    app::irs::broadcast();
    io::can_tx::enqueue100HzMsgs();
}

void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}

void jobs_runAdbmsChain_tick()
{
    app::pack::sequence::tick();
    app::pack::view::publish(app::pack::sequence::snapshot());
}

void jobs_runAdbmsReport_tick()
{
    const app::pack::Snapshot local = app::pack::view::latest();
}



