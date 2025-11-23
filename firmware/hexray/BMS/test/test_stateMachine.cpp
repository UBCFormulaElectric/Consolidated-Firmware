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

// Precharge drive tests

TEST_F(BmsStateMachineTest, enter_precharge_drive) {
    ASSERT_STATE_EQ(init_state);

    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    app_canRx_VC_State_update(VC_BMS_ON_STATE);
    app_canRx_Debug_StartCharging_update(false);
    fakes::charger::setConnectionStatus(CHARGER_DISCONNECTED);
    LetTimePass(20);

    ASSERT_STATE_EQ(precharge_drive_state);
    ASSERT_EQ(io_irs_prechargeState(), CONTACTOR_STATE_CLOSED);
    ASSERT_EQ(io_irs_positiveState(), CONTACTOR_STATE_OPEN);
    ASSERT_EQ(io_irs_negativeState(), CONTACTOR_STATE_CLOSED);
}

TEST_F(BmsStateMachineTest, no_precharge_drive_if_negative_open)
{
    app_canRx_VC_State_update(VC_BMS_ON_STATE);
    fakes::irs::setNegativeState(CONTACTOR_STATE_OPEN);
    LetTimePass(20);

    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsStateMachineTest, no_precharge_drive_if_vc_state_not_bms_on)
{
    app_canRx_VC_State_update(VC_INIT_STATE);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    LetTimePass(20);

    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsStateMachineTest, no_precharge_drive_if_charging_requested)
{
    app_canRx_VC_State_update(VC_BMS_ON_STATE);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    app_canRx_Debug_StartCharging_update(true);
    LetTimePass(20);

    ASSERT_STATE_EQ(init_state);
}

static constexpr int precharge_too_fast_time = 20; // ms
static constexpr int precharge_just_good_time = 1220; // ms
static constexpr int precharge_too_slow_time = 7290; // ms

static constexpr int precharge_cooldown_time = 1000; // ms
