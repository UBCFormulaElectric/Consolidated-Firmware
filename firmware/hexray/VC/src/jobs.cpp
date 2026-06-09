#include "jobs.hpp"

#include "app/app_inverter.hpp"
#include "app/app_powerManager.hpp"
#include "app_canUtils.hpp"
#include "app_jsoncan.hpp"
#include "app_heartbeatMonitors.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_lowVoltageBattery.hpp"
#include "app_powerMonitoring.hpp"
#include "app/states/app_states.hpp"
#include "app_stateMachine.hpp"
#include "app_timer.hpp"
#include "app_commitInfo.hpp"
#include "app_vcShdnLoop.hpp"
#include "app_shdnLast.hpp"

#include "io_canMsg.hpp"
#include "io_canTx.hpp"
#include "io_canQueues.hpp"
#include "io_time.hpp"
#include "io_batteryMonitoring.hpp"
#include "io_canReroute.hpp"
#include "io_pcm.hpp"

#include <util_errorCodes.hpp>

static void fdcan_tx(const JsonCanMsg &tx_msg)
{
    const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
    LOG_IF_ERR(fdcan_tx_queue.push(msg));
}

static void invcan_tx(const JsonCanMsg &tx_msg)
{
    const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
    LOG_IF_ERR(invcan_tx_queue.push(msg));
}

static constexpr uint32_t AIR_MINUS_OPEN_DEBOUNCE_MS = 100U;
static app::Timer         air_minus_open_debounce_timer{ AIR_MINUS_OPEN_DEBOUNCE_MS };

void jobs_init()
{
    fdcan_tx_queue.init();
    invcan_tx_queue.init();
    can_rx_queue.init();

    io::can_tx::init(fdcan_tx, invcan_tx);
    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEFAULT, true);
    io::can_tx::enableMode_InvCAN(app::can_utils::InvCANMode::INVCAN_MODE_DEFAULT, true);

    io::can_reroute::init(fdcan_tx, invcan_tx);

    app::can_tx::VC_Hash_set(GIT_COMMIT_HASH);
    app::can_tx::VC_Clean_set(GIT_COMMIT_CLEAN);
    app::can_tx::VC_Heartbeat_set(true);

    io::can_tx::VC_Bootup_sendAperiodic();

    app::StateMachine::init(&app::states::init_state);
}
void jobs_run1Hz_tick() {}
void jobs_run100Hz_tick()
{
    if (app::can_alerts::AnyBoardHasFault())
    {
        app::StateMachine::set_next_state(&app::states::fault_state);
    }

    const app::Timer::TimerState air_minus_open_debounced = air_minus_open_debounce_timer.runIfCondition(
        app::can_rx::BMS_IrNegative_get() == app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
    switch (air_minus_open_debounced)
    {
        case app::Timer::TimerState::IDLE:
            break;
        case app::Timer::TimerState::RUNNING:
            break;
        case app::Timer::TimerState::EXPIRED:
            app::StateMachine::set_next_state(&app::states::init_state);
            break;
        default:
            break;
    }
    app::inverter::FaultCheck();
    app::StateMachine::tick100Hz();
    app::can_tx::VC_PcmEnable_set(io::pcm::enabled());
    app::powerManager::efuseProtocolTick_100Hz();
    app::powerManager::broadcastRetryCounts();
    hb_monitor.checkIn();
    hb_monitor.broadcastFaults();
    app::shdnLoop::broadcast();
    app::shdnLast::broadcast();
    io::can_tx::enqueue100HzMsgs();
}
void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}
void jobs_runBatteryMonitoring_tick()
{
    LOG_IF_ERR(app::batteryMonitoring::update());
}
void jobs_runPowerMonitoring_tick()
{
    LOG_IF_ERR(app::powerMonitoring::update());
}