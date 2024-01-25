#include <gtest/gtest.h>
#include "Test_Utils.h"
#include "Test_BaseStateMachineTest.h"

#include "fake_io_time.hpp"
#include "fake_io_lowVoltageBattery.hpp"
#include "fake_io_efuse.hpp"

extern "C"
{
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedStateMachine.h"
#include "App_CanUtils.h"
#include "App_SharedMacros.h"
#include "states/app_initState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "app_globals.h"
}

// Test fixture definition for any test requiring the state machine. Can also be used for non-state machine related
// tests.

class PdmBaseStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        // config to forward can functions to shared heartbeat
        // PDM rellies on BMS
        bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = { [BMS_HEARTBEAT_BOARD] = true,
                                                                  [DCM_HEARTBEAT_BOARD] = false,
                                                                  [PDM_HEARTBEAT_BOARD] = false,
                                                                  [FSM_HEARTBEAT_BOARD] = false,
                                                                  [DIM_HEARTBEAT_BOARD] = false };

        // heartbeatGetters - get heartbeat signals from other boards
        bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = &App_CanRx_BMS_Heartbeat_Get,
                                                              [DCM_HEARTBEAT_BOARD] = NULL,
                                                              [PDM_HEARTBEAT_BOARD] = NULL,
                                                              [FSM_HEARTBEAT_BOARD] = NULL,
                                                              [DIM_HEARTBEAT_BOARD] = NULL };

        // heartbeatUpdaters - update local CAN table with heartbeat status
        void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD] =
                                                                       &App_CanRx_BMS_Heartbeat_Update,
                                                                   [DCM_HEARTBEAT_BOARD] = NULL,
                                                                   [PDM_HEARTBEAT_BOARD] = NULL,
                                                                   [FSM_HEARTBEAT_BOARD] = NULL,
                                                                   [DIM_HEARTBEAT_BOARD] = NULL };

        // heartbeatUpdaters - update local CAN table with heartbeat status
        void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(
            bool) = { [BMS_HEARTBEAT_BOARD] = &App_CanAlerts_PDM_Fault_MissingBMSHeartbeat_Set,
                      [DCM_HEARTBEAT_BOARD] = NULL,
                      [PDM_HEARTBEAT_BOARD] = NULL,
                      [FSM_HEARTBEAT_BOARD] = NULL,
                      [DIM_HEARTBEAT_BOARD] = NULL };

        // heartbeatFaultGetters - gets fault statuses over CAN
        bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] =
                                                                       &App_CanAlerts_PDM_Fault_MissingBMSHeartbeat_Get,
                                                                   [DCM_HEARTBEAT_BOARD] = NULL,
                                                                   [PDM_HEARTBEAT_BOARD] = NULL,
                                                                   [FSM_HEARTBEAT_BOARD] = NULL,
                                                                   [DIM_HEARTBEAT_BOARD] = NULL };

        BaseStateMachineTest::SetUp();

        App_CanTx_Init();
        App_CanRx_Init();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            io_time_getCurrentMs, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, heartbeatMonitorChecklist, heartbeatGetters,
            heartbeatUpdaters, &App_CanTx_PDM_Heartbeat_Set, heartbeatFaultSetters, heartbeatFaultGetters);

        state_machine = App_SharedStateMachine_Create(NULL, app_driveState_get());

        globals->heartbeat_monitor = heartbeat_monitor;
    }

    void TearDown() override
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);

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

    struct StateMachine *    state_machine;
    struct HeartbeatMonitor *heartbeat_monitor;
};
