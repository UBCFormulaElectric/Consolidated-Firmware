#include <gtest/gtest.h>
#include "test_baseStateMachineTest.h"

#include "fake_io_time.hpp"
#include "fake_io_lowVoltageBattery.hpp"
#include "fake_io_efuse.hpp"

extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_heartbeatMonitor.h"
#include "app_stateMachine.h"
#include "app_canUtils.h"
#include "app_utils.h"
#include "states/app_initState.h"
#include "states/app_driveState.h"
#include "app_powerManager.h"
}

// Test fixture definition for any test requiring the state machine. Can also be used for non-state machine related
// tests.

class PdmBaseStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        app_canTx_init();
        app_canRx_init();

        app_heartbeatMonitor_init(
            heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters, &app_canTx_PDM_Heartbeat_set,
            heartbeatFaultSetters, heartbeatFaultGetters);

        fake_io_time_getCurrentMs_returns(250);

        app_stateMachine_init(app_initState_get());
        LetTimePass(100);

        // Disable heartbeat monitor in the nominal case. To use representative heartbeat behavior,
        // re-enable the heartbeat monitor.
        app_heartbeatMonitor_blockFaults(true);
    }

    void TearDown() override
    {
        // Reset fakes.
        fake_io_time_getCurrentMs_reset();
        fake_io_lowVoltageBattery_hasChargeFault_reset();
        fake_io_lowVoltageBattery_hasChargeFault_reset();
        fake_io_lowVoltageBattery_hasChargeFault_reset();
        fake_io_lowVoltageBattery_hasBoostControllerFault_reset();
        fake_io_lowVoltageBattery_getBatVoltage_reset();
        fake_io_lowVoltageBattery_getBoostVoltage_reset();
        fake_io_lowVoltageBattery_getAccVoltage_reset();
        fake_io_efuse_setChannel_reset();
        fake_io_efuse_isChannelEnabled_reset();
        fake_io_efuse_getChannelCurrent_reset();
        fake_io_efuse_standbyReset_reset();
    }

    // config to forward can functions to shared heartbeat
    // PDM rellies on BMS
    bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = { [BMS_HEARTBEAT_BOARD] = true,
                                                              [DCM_HEARTBEAT_BOARD] = false,
                                                              [PDM_HEARTBEAT_BOARD] = false,
                                                              [FSM_HEARTBEAT_BOARD] = false,
                                                              [DIM_HEARTBEAT_BOARD] = false };
    void SetInitialState(const State *const initial_state)
    {
        app_stateMachine_init(initial_state);
        ASSERT_EQ(initial_state, app_stateMachine_getCurrentState());
    }

    // heartbeatGetters - get heartbeat signals from other boards
    bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = &app_canRx_BMS_Heartbeat_get,
                                                          [DCM_HEARTBEAT_BOARD] = NULL,
                                                          [PDM_HEARTBEAT_BOARD] = NULL,
                                                          [FSM_HEARTBEAT_BOARD] = NULL,
                                                          [DIM_HEARTBEAT_BOARD] = NULL };

    // heartbeatUpdaters - update local CAN table with heartbeat status
    void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD] = &app_canRx_BMS_Heartbeat_update,
                                                               [DCM_HEARTBEAT_BOARD] = NULL,
                                                               [PDM_HEARTBEAT_BOARD] = NULL,
                                                               [FSM_HEARTBEAT_BOARD] = NULL,
                                                               [DIM_HEARTBEAT_BOARD] = NULL };

    // heartbeatUpdaters - update local CAN table with heartbeat status
    void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
        [BMS_HEARTBEAT_BOARD] = &app_canAlerts_PDM_Fault_MissingBMSHeartbeat_set,
        [DCM_HEARTBEAT_BOARD] = NULL,
        [PDM_HEARTBEAT_BOARD] = NULL,
        [FSM_HEARTBEAT_BOARD] = NULL,
        [DIM_HEARTBEAT_BOARD] = NULL
    };

    // heartbeatFaultGetters - gets fault statuses over CAN
    bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
        [BMS_HEARTBEAT_BOARD] = &app_canAlerts_PDM_Fault_MissingBMSHeartbeat_get,
        [DCM_HEARTBEAT_BOARD] = NULL,
        [PDM_HEARTBEAT_BOARD] = NULL,
        [FSM_HEARTBEAT_BOARD] = NULL,
        [DIM_HEARTBEAT_BOARD] = NULL
    };
};
