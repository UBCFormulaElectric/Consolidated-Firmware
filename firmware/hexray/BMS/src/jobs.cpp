#include "jobs.hpp"
#include "app_canUtils.hpp"
#include "app_jsoncan.hpp"
#include "app_segments.hpp"
#include "io/io_adbms.hpp"
#include "io_canMsg.hpp"
#include "io_canTx.hpp"
#include "io_canQueues.hpp"
#include "io_time.hpp"
#include "io_semaphore.hpp"
#include "util_errorCodes.hpp"

//temp
#include "io_adbms.hpp" 

//static array<io::adbms::StatusGroups, io::NUM_SEGMENTS> stat_reg; 
static array<expected<void, ErrorCode>, io::NUM_SEGMENTS> stat_regs_success;
static io::semaphore spi_bus_lock(true);
static io::semaphore adbms_app_lock(true);

void jobs_init()
{
    can_tx_queue.init();
    can_rx_queue.init();

    // io::can_tx::init(
    //     [](const JsonCanMsg &tx_msg)
    //     {
    //         const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
    //         (void)can_tx_queue.push(msg);
    //         // LOG_IF_ERR();
    //     },
    //     [](const JsonCanMsg &tx_msg)
    //     {
    //         UNUSED(tx_msg);
    //         // const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
    //         // LOG_IF_ERR(can_tx_queue.push(msg));
    //     });
    // io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEFAULT, true);
    // io::can_tx::enableMode_charger(app::can_utils::chargerMode::CHARGER_MODE_DEFAULT, true);
}
void jobs_run1Hz_tick()
{
    io::can_tx::enqueue1HzMsgs();
}
void jobs_run100Hz_tick()
{
    io::can_tx::enqueue100HzMsgs();
}
void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}

void jobs_adbms_init() {
    app::segments::setDefaultConfig();
    LOG_IF_ERR(io::adbms::wakeup());
    LOG_IF_ERR(app::segments::configSync());
}

void jobs_runAdbmsVoltages_tick()
{
    const bool balancing_enabled = false;
    
    spi_bus_lock.take(io::MAX_TIMEOUT);

    app::segments::setDefaultConfig();
    LOG_IF_ERR(io::adbms::wakeup());
    LOG_IF_ERR(app::segments::configSync());

    if (balancing_enabled) {
        LOG_IF_ERR(io::adbms::sendBalanceCmd());
    } else {
        LOG_IF_ERR(io::adbms::sendStopBalanceCmd());
    }
 
    LOG_IF_ERR(app::segments::runVoltageConversion());  

    spi_bus_lock.give();

    adbms_app_lock.take(io::MAX_TIMEOUT);

    app::segments::broadcastCellVoltages();
    //app::segments::broadcastCellVoltageStats();
    app::segments::balancingTick(balancing_enabled);

    adbms_app_lock.give();
}

void jobs_runAdbmsFilteredVoltages_tick() {
    spi_bus_lock.take(io::MAX_TIMEOUT);

    LOG_IF_ERR(io::adbms::wakeup());
    LOG_IF_ERR(app::segments::configSync());
    LOG_IF_ERR(app::segments::runFilteredVoltageConversion());  

    spi_bus_lock.give();

    adbms_app_lock.take(io::MAX_TIMEOUT);

    app::segments::broadcastFilteredCellVoltages();

    adbms_app_lock.give();
}

void jobs_runAdbmsTemperatures_tick() {
    spi_bus_lock.take(io::MAX_TIMEOUT);

    LOG_IF_ERR(io::adbms::wakeup());
    LOG_IF_ERR(app::segments::runAuxConversion());  

    spi_bus_lock.give();

    adbms_app_lock.take(io::MAX_TIMEOUT);

    app::segments::broadcastCellTemps();
    //app::segments::broadcastCellTempsStats();


    adbms_app_lock.give();
}

void jobs_runAdbmsDiagnostics_tick() {

    spi_bus_lock.take(io::MAX_TIMEOUT);

    LOG_IF_ERR(io::adbms::wakeup());
    LOG_IF_ERR(app::segments::configSync());

    spi_bus_lock.give();

    adbms_app_lock.take(io::MAX_TIMEOUT);



    adbms_app_lock.give();
}
