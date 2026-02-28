#include "jobs.hpp"

// app
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canAlerts.hpp"

#include "states/app_states.hpp"
#include "app_precharge.hpp"
// #include "app_segments.hpp"
// #include "app_commitInfo.hpp"
#include "app_timer.hpp"
#include "app_imd.hpp"
#include "app_powerLimit.hpp"
#include "app_shdnLoop.hpp"
#include "app_tractiveSystem.hpp"
#include "app_irs.hpp"
#include "app_jsoncan.hpp"
#include <app_canUtils.hpp>

// io
extern "C"
{
#include "io_semaphore.h"
}

#include "io_canMsg.hpp"
#include "io_canQueues.hpp"
#include "io_canMsg.hpp"
#include "io_irs.hpp"
#include "io_time.hpp"
#include "io_bspdTest.hpp"
#include "io_charger.hpp"
#include "io_fans.hpp"
#include "io_faultLatch.hpp"
#include <io_canTx.hpp>

#include <util_errorCodes.hpp>

CanTxQueue can_tx_queue; // TODO find a better location for this

static Semaphore isospi_bus_access_lock;
static Semaphore adbms_app_data_lock;

static void jsoncan_transmit_func(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = app_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&can_tx_queue, &msg);
}

static void charger_transmit_func(const JsonCanMsg *msg)
{
    // LOG_INFO("Send charger message: %d", msg->std_id);
    UNUSED(msg);
}

using namespace app;
void jobs_init()
{
    // isospi_bus_access_lock guards access to the ISOSPI bus, to guarantee an ADuM SPI transaction doesn't get
    // interrupted by another task. It's just a regular semaphore (no priority inheritance) since it depends on
    // hardware.
    io_semaphore_create(&iso_spi_bus_access_lock, false);

    // adbms_app_data_lock guards access to any ADuM app data that may be written to by the ADuM tasks and read from
    // other tick functions. It's a mutex (yes priority inheritance) since it's guarding shared data and doesn't depend
    // on hardware.
    io::semaphore::create(&adbms_app_data_lock, true);
    io::can_tx::init(
        [](const JsonCanMsg &tx_msg)
        {
            const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
            LOG_IF_ERR(can_tx_queue.push(msg));
        },
        [](const JsonCanMsg &tx_msg)
        {
            UNUSED(tx_msg);
            // const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
            // LOG_IF_ERR(can_tx_queue.push(msg));
        });
    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEFAULT, true);
    io::can_tx::enableMode_charger(app::can_utils::chargerMode::CHARGER_MODE_DEFAULT, true);

    io_canQueue_initRx();
    io_canQueue_initTx(&can_tx_queue);

    app_canTx_init();
    app_canRx_init();

    app_canTx_BMS_Hash_set(GIT_COMMIT_HASH);
    app_canTx_BMS_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_BMS_Heartbeat_set(true);

    ts::init();
    precharge::init();

#ifndef TARGET_HV_SUPPLY
    // TODO segments init and tests
#endif

    StateMachine::init(&init_state);
}

void jobs_run1Hz_tick()
{
    io::can_tx::enqueue1HzMsgs();
}

void jobs_run100Hz_tick()
{
    io::semaphore::take(&adbms_app_data_lock, MAX_TIMEOUT);

    StateMachine::tick100Hz();

    const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
    io_canTx_enableMode_can1(CAN1_MODE_DEBUG, debug_mode_enabled);

    ts::broadcast();
    imd::broadcast();
    shdnLoop::broadcast();
    plim::broadcast();

    // TODO: Enable fans for endurance when contactors are closed.
    // const bool hv_up = io_irs_isNegativeClosed() && io_irs_isPositiveClosed();
    // io::fans::tick(hv_up);
    io::fans::tick(false);

    io::bspdtest::enable(app_canRx_Debug_EnableTestCurrent_get());
    app_canTx_BMS_BSPDCurrentThresholdExceeded_set(io::bspdtest::isCurrentThresholdExceeded());

    // If charge state has not placed a lock on broadcasting
    // if the charger is charger is connected
    app_canTx_BMS_ChargerConnectedType_set(io::charger::getConnectionStatus());

#ifdef TARGET_HV_SUPPLY
    const bool acc_fault = false;
#else
    // segments::checkWarnings();
    // const bool acc_fault = segments::checkFaults();
#endif

    io::faultLatch::setCurrentStatus(&bms_ok_latch, acc_fault ? FAULT_LATCH_FAULT : FAULT_LATCH_OK);

    // Update CAN signals for BMS latch statuses.
    app_canTx_BMS_BmsCurrentlyOk_set(io::faultLatch::getCurrentStatus(&bms_ok_latch) == FAULT_LATCH_OK);
    app_canTx_BMS_ImdCurrentlyOk_set(io::faultLatch::getCurrentStatus(&imd_ok_latch) == FAULT_LATCH_OK);
    app_canTx_BMS_BspdCurrentlyOk_set(io::faultLatch::getCurrentStatus(&bspd_ok_latch) == FAULT_LATCH_OK);
    app_canTx_BMS_BmsLatchOk_set(io::faultLatch::getLatchedStatus(&bms_ok_latch) == FAULT_LATCH_OK);
    app_canTx_BMS_ImdLatchOk_set(io::faultLatch::getLatchedStatus(&imd_ok_latch) == FAULT_LATCH_OK);
    app_canTx_BMS_BspdLatchOk_set(io::faultLatch::getLatchedStatus(&bspd_ok_latch) == FAULT_LATCH_OK);

    app_canTx_BMS_BSPDBrakePressureThresholdExceeded_set(io::bspdtest::isBrakePressureThresholdExceeded());
    app_canTx_BMS_BSPDAccelBrakeOk_set(io::bspdtest::isAccelBrakeOk());

    const bool ir_negative_opened_debounced = irs::negativeOpenedDebounced();
    if (ir_negative_opened_debounced)
    {
        StateMachine::setNextState(&init_state);
    }
    if (app_canAlerts_AnyBoardHasFault())
    {
        StateMachine::setNextState(&fault_state);
    }
    StateMachine::tickTransitionState();

    irs::broadcast();

    io::can_tx::enqueue100HzMsgs();
    io::semaphore::give(&adbms_app_data_lock);
}

void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}
