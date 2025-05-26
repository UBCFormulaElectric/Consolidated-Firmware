#include "app_allStates.h"
#include "app_heartbeatMonitors.h"
#include "quintuna/BMS/src/io/io_faultLatch.h"
#include "stdbool.h"

/**
 * Num of cycles for voltage and cell temperature values to settle
 */
#define NUM_CYCLES_TO_SETTLE 30U
#define NUM_CYCLES_TO_BALANCE 1000U
#define NUM_CYCLES_TO_MEASURE_BALANCING 100U
#define NUM_CYCLES_TO_MEASURE_NOMINAL 1000U

void app_allStates_runOnTick1Hz(void)
{
    /**
     * add charger connection status CAN tx once charging implementation branch is merged
     */

     /**
      * add reset SOC from min cell voltage if soc corrupt and voltage readings settled after soc app is finished
      */
}

bool app_allStates_runOnTick100Hz(void)
{
    app_canTx_BMS_Heartbeat_set(true);
    app_heartbeatMonitor_checkIn(&hb_monitor);
    app_heartbeatMonitor_broadcastFaults(&hb_monitor);

     /**
     * Update CAN signals for BMS latch statuses
     */
    app_canTx_BMS_BmsOk_set(io_faultLatch_getCurrentStatus(&bms_ok_latch));
    app_canTx_BMS_ImdOk_set(io_faultLatch_getCurrentStatus(&imd_ok_latch));
    app_canTx_BMS_BspdOk_set(io_faultLatch_getCurrentStatus(&bspd_ok_latch));
    app_canTx_BMS_BmsLatchedFault_set(io_faultLatch_getLatchedStatus(&bms_ok_latch));
    app_canTx_BMS_ImdLatchedFault_set(io_faultLatch_getLatchedStatus(&imd_ok_latch));
    app_canTx_BMS_BspdLatchedFault_set(io_faultLatch_getLatchedStatus(&bspd_ok_latch));
    /**
     * app_canTx_BMS_Soc_set(app_soc_getMinSocPercent()); add once soc app is finished
     */

    /**
     * add cell balancing check once cell balancing is enabled
     */

    /**
     * add accumulator tasks once accumulator app is finished
     */

    /**
     * add soc stat update once app soc is finished
     */

    /**
     * add waiting for cell voltage and temperature measurements to settle once accumulator app is finished
     */
}