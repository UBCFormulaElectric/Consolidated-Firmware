#include "test_BMSBase.hpp"

extern "C"
{
#include "app_canTx.h"
#include "app_canAlerts.h"
}

class BMSCanTest : public BMSBaseTest
{
};

#define ASSERT_STATE_EQ(x)                           \
    ASSERT_EQ(app_stateMachine_getCurrentState(), x) \
        << "Expected state: " << x->name << ", but got: " << app_stateMachine_getCurrentState()->name

static void enforceStatePreconditions(const BMSBaseTest::StateMetadata &metadata)
{
    fakes::irs::setNegativeState(metadata.requires_irs_negative_closed ? IRS_CLOSED : IRS_OPEN);
    ASSERT_EQ(io_irs_negativeState(), metadata.requires_irs_negative_closed ? IRS_CLOSED : IRS_OPEN)
        << "Expected IRS negative state to be " << (metadata.requires_irs_negative_closed ? "CLOSED" : "OPEN")
        << " in state: " << metadata.state->name;
    app_canAlerts_BMS_Fault_TESTFAULT_set(metadata.requires_fault);
    ASSERT_EQ(app_canAlerts_AnyBoardHasFault(), metadata.requires_fault)
        << "Expected " << (metadata.requires_fault ? "some" : "no") << " fault to be "
        << (metadata.requires_fault ? "set" : "not set") << " in state: " << metadata.state->name;
}

TEST_F(BMSCanTest, check_states_is_broadcasted_over_can)
{
    for (const auto &metadata : state_metadata)
    {
        enforceStatePreconditions(metadata);
        app_stateMachine_setCurrentState(metadata.state);
        LetTimePass(10); // let the state settle
        ASSERT_STATE_EQ(metadata.state);
        ASSERT_EQ(app_canTx_BMS_State_get(), metadata.can_state)
            << "Expected " << metadata.can_state << ", but got: " << app_canTx_BMS_State_get()
            << " in state: " << metadata.state->name;
    }
}

TEST_F(BMSCanTest, check_latched_faults_broadcasted_over_can)
{
    //
    ASSERT_EQ(io_faultLatch_getCurrentStatus(&bms_ok_latch), FAULT_LATCH_OK) << "Expected BMS OK latch to be OK";
    ASSERT_EQ(io_faultLatch_getCurrentStatus(&imd_ok_latch), FAULT_LATCH_OK) << "Expected IMD OK latch to be OK";
    ASSERT_EQ(io_faultLatch_getCurrentStatus(&bspd_ok_latch), FAULT_LATCH_OK) << "Expected BSPD OK latch to be OK";

    LetTimePass(10);

    io_faultLatch_setCurrentStatus(&bms_ok_latch, FAULT_LATCH_FAULT);
    io_faultLatch_setCurrentStatus(&imd_ok_latch, FAULT_LATCH_FAULT);
    io_faultLatch_setCurrentStatus(&bspd_ok_latch, FAULT_LATCH_FAULT);

    ASSERT_EQ(io_faultLatch_getCurrentStatus(&bms_ok_latch), FAULT_LATCH_FAULT) << "Expected BMS OK latch to be FAULT";
    ASSERT_EQ(io_faultLatch_getLatchedStatus(&bms_ok_latch), FAULT_LATCH_FAULT)
        << "Expected BMS OK latch to be latched as FAULT";
    ASSERT_EQ(io_faultLatch_getCurrentStatus(&imd_ok_latch), FAULT_LATCH_FAULT) << "Expected IMD OK latch to be FAULT";
    ASSERT_EQ(io_faultLatch_getLatchedStatus(&imd_ok_latch), FAULT_LATCH_FAULT)
        << "Expected IMD OK latch to be latched as FAULT";
    ASSERT_EQ(io_faultLatch_getCurrentStatus(&bspd_ok_latch), FAULT_LATCH_FAULT)
        << "Expected BSPD OK latch to be FAULT";
    ASSERT_EQ(io_faultLatch_getLatchedStatus(&bspd_ok_latch), FAULT_LATCH_FAULT)
        << "Expected BSPD OK latch to be latched as FAULT";

    LetTimePass(10);

    ASSERT_FALSE(app_canTx_BMS_BmsCurrentlyOk_get());
    ASSERT_FALSE(app_canTx_BMS_BmsLatchOk_get());

    ASSERT_FALSE(app_canTx_BMS_ImdCurrentlyOk_get());
    ASSERT_FALSE(app_canTx_BMS_ImdLatchOk_get());

    ASSERT_FALSE(app_canTx_BMS_BspdCurrentlyOk_get());
    ASSERT_FALSE(app_canTx_BMS_BspdLatchOk_get());

    // LATCHES MUST PERSIST EVEN WHILE THE STATE IS RETURNED

    io_faultLatch_setCurrentStatus(&bms_ok_latch, FAULT_LATCH_OK);
    io_faultLatch_setCurrentStatus(&imd_ok_latch, FAULT_LATCH_OK);
    io_faultLatch_setCurrentStatus(&bspd_ok_latch, FAULT_LATCH_OK);

    ASSERT_EQ(io_faultLatch_getCurrentStatus(&bms_ok_latch), FAULT_LATCH_OK) << "Expected BMS OK latch to be OK";
    ASSERT_EQ(io_faultLatch_getLatchedStatus(&bms_ok_latch), FAULT_LATCH_FAULT)
        << "Expected BMS OK latch to be latched as FAULT";
    ASSERT_EQ(io_faultLatch_getCurrentStatus(&imd_ok_latch), FAULT_LATCH_OK) << "Expected IMD OK latch to be OK";
    ASSERT_EQ(io_faultLatch_getLatchedStatus(&imd_ok_latch), FAULT_LATCH_FAULT)
        << "Expected IMD OK latch to be latched as FAULT";
    ASSERT_EQ(io_faultLatch_getCurrentStatus(&bspd_ok_latch), FAULT_LATCH_OK) << "Expected BSPD OK latch to be OK";
    ASSERT_EQ(io_faultLatch_getLatchedStatus(&bspd_ok_latch), FAULT_LATCH_FAULT)
        << "Expected BSPD OK latch to be latched as FAULT";

    LetTimePass(10);

    ASSERT_TRUE(app_canTx_BMS_BmsCurrentlyOk_get());
    ASSERT_FALSE(app_canTx_BMS_BmsLatchOk_get());
    ASSERT_TRUE(app_canTx_BMS_ImdCurrentlyOk_get());
    ASSERT_FALSE(app_canTx_BMS_ImdLatchOk_get());
    ASSERT_TRUE(app_canTx_BMS_BspdCurrentlyOk_get());
    ASSERT_FALSE(app_canTx_BMS_BspdLatchOk_get());
}

TEST_F(BMSCanTest, check_airs_can_signals_for_all_states)
{
    for (const auto metadata : state_metadata)
    {
        enforceStatePreconditions(metadata);
        app_stateMachine_setCurrentState(metadata.state);
        LetTimePass(10); // let the state settle
        ASSERT_STATE_EQ(metadata.state);

        io_irs_setPositive(IRS_CLOSED);
        io_irs_setPrecharge(IRS_CLOSED);
        LetTimePass(10);
        ASSERT_EQ(io_irs_positiveState(), IRS_CLOSED)
            << "Expected contactors closed in state: " << metadata.state->name;
        ASSERT_EQ(io_irs_prechargeState(), IRS_CLOSED)
            << "Expected precharge closed in state: " << metadata.state->name;

        io_irs_setPositive(IRS_OPEN);
        io_irs_setPrecharge(IRS_OPEN);
        LetTimePass(10);
        ASSERT_EQ(io_irs_positiveState(), IRS_OPEN) << "Expected contactors open in state: " << metadata.state->name;
        ASSERT_EQ(io_irs_prechargeState(), IRS_OPEN) << "Expected precharge open in state: " << metadata.state->name;
    }
}

TEST_F(BMSCanTest, check_imd_info_is_broadcasted_over_can_in_all_states)
{
    for (const auto &metadata : state_metadata)
    {
        enforceStatePreconditions(metadata);
        app_stateMachine_setCurrentState(metadata.state);
        LetTimePass(10); // let the state settle
        ASSERT_STATE_EQ(metadata.state);

        app_canTx_BMS_ImdOkHs_get();
        app_canTx_BMS_ImdCondition_get();

        // test 1
        fakes::imd::setFrequency(60.0f);
        fakes::imd::setDutyCycle(0.5);
        LetTimePass(10);

        ASSERT_EQ(app_canTx_BMS_ImdFrequency_get(), 60.0f)
            << "Expected IMD frequency to be 60.0Hz, but got: " << app_canTx_BMS_ImdFrequency_get();
        ASSERT_EQ(app_canTx_BMS_ImdDutyCycle_get(), 0.5f)
            << "Expected IMD duty cycle to be 0.5, but got: " << app_canTx_BMS_ImdDutyCycle_get();
        // app_canTx_BMS_ImdActiveFrequency_get();
        // app_canTx_BMS_ImdValidDutyCycle_get();

        // TODO idk what these are for...
        // insulation resistance 10hz, 20hz
        app_canTx_BMS_ImdInsulationMeasurementDcp10Hz_get();
        app_canTx_BMS_ImdInsulationMeasurementDcp20Hz_get();
        // speed_start_status_30hz
        app_canTx_BMS_ImdSpeedStartStatus30Hz_get();

        // seconds_since_power_on
        ASSERT_EQ(app_canTx_BMS_ImdTimeSincePowerOn_get(), io_time_getCurrentMs());
    }
}

TEST_F(BMSCanTest, check_shdn_broadcasted_in_all_states)
{
    for (const auto &metadata : state_metadata)
    {
        enforceStatePreconditions(metadata);
        app_stateMachine_setCurrentState(metadata.state);
        LetTimePass(10); // let the state settle
        ASSERT_STATE_EQ(metadata.state);
    }
}

TEST_F(BMSCanTest, charger_connection_status_in_all_states)
{
    // TODO
}
