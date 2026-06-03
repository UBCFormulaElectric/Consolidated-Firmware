#include "jobs.hpp"

#include "app_canUtils.hpp"
#include "app_jsoncan.hpp"
#include "app_heartbeatMonitors.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_lowVoltageBattery.hpp"
#include "app_powerMonitoring.hpp"
#include "app_commitInfo.hpp"
#include "app_vcShdnLoop.hpp"
#include "app_shdnLast.hpp"

#include "io_canMsg.hpp"
#include "io_canTx.hpp"
#include "io_canQueues.hpp"
#include "io_time.hpp"
#include "io_batteryMonitoring.hpp"
#include "io_canReroute.hpp"

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
}
void jobs_run1Hz_tick() {}
void jobs_run100Hz_tick()
{
    const uint32_t k = app::can_rx::BMS_ChargePowerLimit_get();
    LOG_INFO("%d", k);
    hb_monitor.checkIn();
    hb_monitor.broadcastFaults();

    io::can_tx::enqueue100HzMsgs();

    app::shdnLoop::broadcast();
    app::shdnLast::broadcast();
}
void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}
void jobs_runBatteryMonitoring_tick()
{
    app::batteryMonitoring::update();
}
void jobs_runPowerMonitoring_tick()
{
    LOG_IF_ERR(app::powerMonitoring::update());
}