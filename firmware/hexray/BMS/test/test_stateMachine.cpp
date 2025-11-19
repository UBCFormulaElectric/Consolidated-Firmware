#include "test_fakes.hpp"
#include "test_BMSBase.hpp"
#include "states/app_states.hpp"
#include "io_irs.hpp"
#include "io_time.hpp"

extern "C"
{
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "app_segments.h" //switch to hpp
}

class BmsStateMachineTest : public BMSBaseTest
{
};

// Precharge tests

TEST_F(BmsStateMachineTest, enter_precharge_drive_when_vc_bms_on_and_negative_closed)
{
    ASSERT_STATE_EQ(init_state);

    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    app_canRx_VC_State_update(VC_BMS_ON_STATE);
    app_canRx_Debug_StartCharging_update(false);
    fakes::charger::setConnectionStatus(CHARGER_DISCONNECTED);
    LetTimePass(20);

    ASSERT_STATE_EQ(precharge_drive_state);
}

TEST_F(BmsStateMachineTest, no_precharge_drive_if_negative_open)
{
    ASSERT_STATE_EQ(init_state);

    app_canRx_VC_State_update(VC_BMS_ON_STATE);
    fakes::irs::setNegativeState(CONTACTOR_STATE_OPEN);
    LetTimePass(20);

    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsStateMachineTest, no_precharge_drive_if_vc_state_not_bms_on)
{
    ASSERT_STATE_EQ(init_state);

    app_canRx_VC_State_update(VC_INIT_STATE);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    LetTimePass(20);

    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsStateMachineTest, no_precharge_drive_if_charging_requested)
{
    ASSERT_STATE_EQ(init_state);

    app_canRx_VC_State_update(VC_BMS_ON_STATE);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    app_canRx_Debug_StartCharging_update(true);
    LetTimePass(20);

    ASSERT_STATE_EQ(init_state);
}
