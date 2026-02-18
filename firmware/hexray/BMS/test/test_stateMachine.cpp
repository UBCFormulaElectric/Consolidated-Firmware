#include "test_fakes.hpp"
#include "test_BMSBase.hpp"
#include "states/app_states.hpp"
#include "app_precharge.hpp"
#include "io_irs.hpp"
#include "io_time.hpp"
#include "app.segments.hpp"

extern "C"
{
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
}

class BmsStateMachineTest : public BMSBaseTest
{
};

static constexpr int target_voltage          = (float)(NUM_SEGMENTS * CELLS_PER_SEGMENT) * 4.2f; // V
static constexpr int undervoltage            = 200.0f;                                           // V
static constexpr int tolerance_time          = 500;                                              // ms
static constexpr int PRECHARGE_COOLDOWN_TIME = 1000;                                             // ms
static constexpr int precharge_completion_ms = static_cast<int>(PRECHARGE_COMPLETION_MS_F);
static constexpr int latch_timeout           = PRECHARGE_LATCH_TIMEOUT_MS + 100; // ms
namespace fakes
{
}

// Init-PrechargeDrive tests

TEST_F(BmsStateMachineTest, enter_precharge_drive)
{
    ASSERT_STATE_EQ(init_state);

    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    app_canRx_VC_State_update(VC_BMS_ON_STATE);
    app_canRx_Debug_StartCharging_update(false);
    fakes::charger::setConnectionStatus(CHARGER_DISCONNECTED);
    LetTimePass(10);

    ASSERT_STATE_EQ(precharge_drive_state);
    ASSERT_EQ(io_irs_prechargeState(), CONTACTOR_STATE_CLOSED);
    ASSERT_EQ(io_irs_positiveState(), CONTACTOR_STATE_OPEN);
    ASSERT_EQ(io_irs_negativeState(), CONTACTOR_STATE_CLOSED);
}

TEST_F(BmsStateMachineTest, no_precharge_drive_if_negative_open)
{
    app_canRx_VC_State_update(VC_BMS_ON_STATE);
    fakes::irs::setNegativeState(CONTACTOR_STATE_OPEN);
    LetTimePass(10);

    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsStateMachineTest, no_precharge_drive_if_vc_state_not_bms_on)
{
    app_canRx_VC_State_update(VC_INIT_STATE);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    LetTimePass(10);

    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsStateMachineTest, no_precharge_drive_if_charging_requested)
{
    app_canRx_VC_State_update(VC_BMS_ON_STATE);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    app_canRx_Debug_StartCharging_update(true);
    LetTimePass(10);

    ASSERT_STATE_EQ(init_state);
}

// PrechargeDrive-Drive tests

TEST_F(BmsStateMachineTest, precharge_drive_success)
{
    fakes::segments::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    fakes::tractiveSystem::setVoltage(0.0f);
    app_stateMachine_setCurrentState(&precharge_drive_state);
    LetTimePass(10);

    for (int i = 0; i < precharge_completion_ms; i += 10)
    {
        ASSERT_STATE_EQ(precharge_drive_state);
        ASSERT_EQ(io_irs_prechargeState(), CONTACTOR_STATE_CLOSED);
        ASSERT_EQ(app_canTx_BMS_PrechargeRelay_get(), CONTACTOR_STATE_CLOSED);
        LetTimePass(10);
    }

    fakes::tractiveSystem::setVoltage(target_voltage);
    LetTimePass(10);
    ASSERT_STATE_EQ(drive_state);
    ASSERT_EQ(io_irs_prechargeState(), CONTACTOR_STATE_OPEN);
    ASSERT_EQ(app_canTx_BMS_PrechargeRelay_get(), CONTACTOR_STATE_OPEN);
}

TEST_F(BmsStateMachineTest, precharge_rises_too_slowly_then_latches_after_max_retries)
{
    fakes::segments::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    fakes::tractiveSystem::setVoltage(undervoltage);
    app_stateMachine_setCurrentState(&precharge_drive_state);
    LetTimePass(10);

    for (int retry = 0; retry < MAX_PRECHARGE_ATTEMPTS; retry++)
    {
        int closed_time = 0;
        while (io_irs_prechargeState() == CONTACTOR_STATE_CLOSED &&
               closed_time < PRECHARGE_COMPLETION_UPPER_BOUND + tolerance_time)
        {
            ASSERT_STATE_EQ(precharge_drive_state);
            ASSERT_EQ(app_canTx_BMS_PrechargeRelay_get(), CONTACTOR_STATE_CLOSED);
            LetTimePass(10);
            closed_time += 10;
        }
        ASSERT_NEAR(closed_time, PRECHARGE_COMPLETION_UPPER_BOUND, 100)
            << "Precharge relay stayed closed for " << closed_time << "ms (expected ~"
            << PRECHARGE_COMPLETION_UPPER_BOUND << "ms)"
            << ", time=" << io_time_getCurrentMs();

        if (retry == MAX_PRECHARGE_ATTEMPTS - 1)
            break;

        int open_time = 0;
        while (io_irs_prechargeState() == CONTACTOR_STATE_OPEN && open_time < PRECHARGE_COOLDOWN_TIME + tolerance_time)
        {
            ASSERT_EQ(app_canTx_BMS_PrechargeRelay_get(), CONTACTOR_STATE_OPEN);
            LetTimePass(10);
            open_time += 10;
        }
        ASSERT_NEAR(open_time, PRECHARGE_COOLDOWN_TIME, 100)
            << "Precharge relay stayed open for " << open_time << "ms (expected ~" << PRECHARGE_COOLDOWN_TIME << "ms)"
            << ", time=" << io_time_getCurrentMs();
    }
    ASSERT_STATE_EQ(precharge_latch_state);
}

TEST_F(BmsStateMachineTest, precharge_rises_too_quickly_then_latches_after_max_retries)
{
    fakes::segments::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    fakes::tractiveSystem::setVoltage(undervoltage);
    app_stateMachine_setCurrentState(&precharge_drive_state);
    LetTimePass(10);

    for (int retry = 0; retry < MAX_PRECHARGE_ATTEMPTS; retry++)
    {
        int closed_time = 0;
        while (closed_time < PRECHARGE_COMPLETION_LOWER_BOUND - tolerance_time)
        {
            ASSERT_STATE_EQ(precharge_drive_state);
            ASSERT_EQ(io_irs_prechargeState(), CONTACTOR_STATE_CLOSED);
            LetTimePass(10);
            closed_time += 10;
        }
        ASSERT_LT(closed_time, PRECHARGE_COMPLETION_LOWER_BOUND)
            << "Precharge relay stayed closed for " << closed_time << "ms (expected ~"
            << PRECHARGE_COMPLETION_LOWER_BOUND - tolerance_time << "ms)"
            << ", time=" << io_time_getCurrentMs();

        fakes::tractiveSystem::setVoltage(target_voltage);
        LetTimePass(10);
        ASSERT_NE(io_irs_prechargeState(), CONTACTOR_STATE_CLOSED);

        if (retry == MAX_PRECHARGE_ATTEMPTS - 1)
            break;

        int open_time = 0;
        while (open_time < PRECHARGE_COOLDOWN_TIME)
        {
            ASSERT_EQ(io_irs_prechargeState(), CONTACTOR_STATE_OPEN);
            LetTimePass(10);
            open_time += 10;
        }
        ASSERT_NEAR(open_time, PRECHARGE_COOLDOWN_TIME, 100)
            << "Precharge relay stayed open for " << open_time << "ms (expected ~" << PRECHARGE_COOLDOWN_TIME << "ms)"
            << ", time=" << io_time_getCurrentMs();
    }
    ASSERT_STATE_EQ(precharge_latch_state);
}

// PrechargeLatch-Init test

TEST_F(BmsStateMachineTest, precharge_latch_timouts_to_init)
{
    fakes::segments::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    fakes::tractiveSystem::setVoltage(undervoltage);
    app_stateMachine_setCurrentState(&precharge_latch_state);
    LetTimePass(latch_timeout);

    ASSERT_STATE_EQ(init_state);
}

// Init-Balance tests

TEST_F(BmsStateMachineTest, enter_balance)
{
    ASSERT_STATE_EQ(init_state);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    app_canRx_Debug_CellBalancingRequest_update(true);
    LetTimePass(10);

    ASSERT_STATE_EQ(balancing_state);
    EXPECT_TRUE(app_canRx_Debug_CellBalancingRequest_get());
}

TEST_F(BmsStateMachineTest, exit_balance)
{
    SetInitialState(&balancing_state);
    app_canRx_Debug_CellBalancingRequest_update(false);
    LetTimePass(10);

    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsStateMachineTest, exits_balance_when_negative_opens_and_clears_request_on_exit)
{
    SetInitialState(&balancing_state);
    app_canRx_Debug_CellBalancingRequest_update(true);
    fakes::irs::setNegativeState(CONTACTOR_STATE_OPEN);
    LetTimePass(10);

    ASSERT_STATE_EQ(init_state);
    EXPECT_FALSE(app_canRx_Debug_CellBalancingRequest_get());
}

// Init-PrechargeCharge tests

TEST_F(BmsStateMachineTest, enter_precharge_charge)
{
    ASSERT_STATE_EQ(init_state);

    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    fakes::charger::setConnectionStatus(CHARGER_CONNECTED_EVSE);
    app_canRx_Debug_StartCharging_update(true);
    LetTimePass(10);

    ASSERT_STATE_EQ(precharge_charge_state);
    ASSERT_EQ(app_canTx_BMS_State_get(), BMS_PRECHARGE_CHARGE_STATE);
}

TEST_F(BmsStateMachineTest, precharge_charge_success_to_charge_init)
{
    fakes::segments::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    fakes::tractiveSystem::setVoltage(target_voltage);

    app_stateMachine_setCurrentState(&precharge_charge_state);
    LetTimePass(10);

    ASSERT_STATE_EQ(charge_init_state);
    ASSERT_EQ(io_irs_positiveState(), CONTACTOR_STATE_CLOSED);
    ASSERT_EQ(io_irs_prechargeState(), CONTACTOR_STATE_OPEN);
}

TEST_F(BmsStateMachineTest, precharge_charge_latches_after_retries)
{
    fakes::segments::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    fakes::tractiveSystem::setVoltage(undervoltage);
    app_canRx_Debug_StartCharging_update(true);

    app_stateMachine_setCurrentState(&precharge_charge_state);
    LetTimePass(10);

    for (int retry = 0; retry < MAX_PRECHARGE_ATTEMPTS; retry++)
    {
        LetTimePass(PRECHARGE_COMPLETION_UPPER_BOUND + tolerance_time);

        if (retry == MAX_PRECHARGE_ATTEMPTS - 1)
            break;

        LetTimePass(PRECHARGE_COOLDOWN_TIME + tolerance_time);
        ASSERT_STATE_EQ(precharge_charge_state);
    }

    ASSERT_STATE_EQ(precharge_latch_state);
    EXPECT_FALSE(app_canRx_Debug_StartCharging_get());
    ASSERT_EQ(io_irs_prechargeState(), CONTACTOR_STATE_OPEN);
}

// Drive tests

TEST_F(BmsStateMachineTest, drive_to_init_on_negative_open)
{
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    SetInitialState(&drive_state);
    fakes::irs::setNegativeState(CONTACTOR_STATE_OPEN);
    LetTimePass(10);

    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsStateMachineTest, drive_to_fault_on_positive_open)
{
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    SetInitialState(&drive_state);
    io::irs::setPositiveState(CONTACTOR_STATE_OPEN);
    LetTimePass(10);

    ASSERT_STATE_EQ(fault_state);
}

TEST_F(BmsStateMachineTest, drive_to_fault_from_board_faults)
{
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    SetInitialState(&drive_state);
    app_canAlerts_AnyBoardHasFault_set(true);
    LetTimePass(10);

    ASSERT_STATE_EQ(fault_state);
}

TEST_F(BmsStateMachineTest, drive_to_fault_on_overtemp)
{
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    SetInitialState(&drive_state);

    fakes::segments::setPackVoltageEvenly(MAX_CELL_VOLTAGE_FAULT_V - 0.1f);
    fakes::segments::setCellTemperatures(const int &temperatures)

        LetTimePass(10);

    ASSERT_STATE_EQ(fault_state);
}

// imd
// bspd
// overtemp
// overvoltage
// undervotlge
// other board faulting
