#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"
#include "states/app_states.hpp"
#include "app_startSwitch.hpp"
#include "app_canAlerts.hpp"
#include "vc_fakes.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canUtils.hpp"
#include "app_inverter.hpp"
#include "app_powerManager.hpp"

using namespace app::can_utils;

class VCStateMachineTest : public VCBaseTest
{
};

static void SetStateWithEntry(const app::State *s)
{
    app::StateMachine::set_current_state(s);
    s->run_on_entry();
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_CLOSED);
}

/* ------------------------- INIT STATE ------------------------------- */
TEST_F(VCStateMachineTest, InitToInverterOnTransition)
{
    app::StateMachine::set_current_state(&app::states::init_state);
    app::states::init_state.run_on_entry();
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_OPEN);

    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::init_state);

    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverterOn_state);
}

/* ------------------------ INVERTER ON STATE ------------------------------- */
TEST_F(VCStateMachineTest, InverterOnToBmsOnTransition)
{
    SetStateWithEntry(&app::states::inverterOn_state);
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_CLOSED);
    LetTimePass(10);

    app::can_rx::INVFL_bSystemReady_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverterOn_state);

    app::can_rx::INVFR_bSystemReady_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverterOn_state);

    app::can_rx::INVRL_bSystemReady_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverterOn_state);

    //all ready so transition to bms on 
    app::can_rx::INVRR_bSystemReady_update(true);
    LetTimePass(100);
    ASSERT_STATE_EQ(app::states::bmsOn_state);
}

/* ------------------------- BMS ON STATE ------------------------------- */
TEST_F(VCStateMachineTest, BmsOnTransitionToHvInit)
{
    SetStateWithEntry(&app::states::bmsOn_state);
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_CLOSED);
    LetTimePass(10);

    app::can_rx::BMS_State_update(BmsState::BMS_PRECHARGE_DRIVE_STATE);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::bmsOn_state);

    app::can_rx::BMS_State_update(BmsState::BMS_DRIVE_STATE);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::pcmOn_state);
}

/* ------------------------- PCM ON STATE ------------------------------- */
TEST_F(VCStateMachineTest, PcmOnStateTransitionsToHvInitWhenVoltageStable)
{
    SetStateWithEntry(&app::states::pcmOn_state);
    app::can_tx::VC_PcmChannelVoltage_set(18.0f);// mock earlier

    LetTimePass(40);
    ASSERT_STATE_EQ(app::states::hvInit_state);
}

TEST_F(VCStateMachineTest, PcmOnStateFailsToRecoverAfterMaxRetries)
{
    SetStateWithEntry(&app::states::pcmOn_state);
    app::can_tx::VC_PcmChannelVoltage_set(10.0f);

    LetTimePass(1200);

    ASSERT_STATE_EQ(app::states::fault_state);
    EXPECT_TRUE(app::can_tx::VC_Info_PcmUnderVoltage_get());
}

/* ------------------------- HV INIT STATE ------------------------------- */
TEST_F(VCStateMachineTest, EntryInitializesStateAndOutputs)
{
    SetStateWithEntry(&app::states::hvInit_state);
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_CLOSED);

    EXPECT_EQ(app::can_tx::VC_State_get(), VCState::VC_HV_INIT_STATE);
    EXPECT_EQ(app::can_tx::VC_INVFLTorqueSetpoint_get(), 0);
    EXPECT_EQ(app::can_tx::VC_INVRRTorqueSetpoint_get(), 0);
    EXPECT_EQ(app::can_tx::VC_INVFLTorqueSetpoint_get(), 0);
    EXPECT_EQ(app::can_tx::VC_INVRLTorqueSetpoint_get(), 0);
}

TEST_F(VCStateMachineTest, SystemReadyTransitionsToDcOn)
{
    SetStateWithEntry(&app::states::hvInit_state);
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_CLOSED);

    app::can_rx::INVFL_bSystemReady_update(true);
    app::can_rx::INVFR_bSystemReady_update(true);
    app::can_rx::INVRL_bSystemReady_update(true);
    app::can_rx::INVRR_bSystemReady_update(true);

    app::states::hvInit_state.run_on_tick_100Hz();
    app::states::hvInit_state.run_on_tick_100Hz();

    EXPECT_TRUE(app::can_tx::VC_INVFLbDcOn_get());
    EXPECT_TRUE(app::can_tx::VC_INVFRbDcOn_get());
    EXPECT_TRUE(app::can_tx::VC_INVRLbDcOn_get());
    EXPECT_TRUE(app::can_tx::VC_INVRRbDcOn_get());
}

TEST_F(VCStateMachineTest, DcOnQuitTransitionsToEnable)
{
    SetStateWithEntry(&app::states::hvInit_state);
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_CLOSED);

    app::can_rx::INVFL_bSystemReady_update(true);
    app::can_rx::INVFR_bSystemReady_update(true);
    app::can_rx::INVRL_bSystemReady_update(true);
    app::can_rx::INVRR_bSystemReady_update(true);

    app::states::hvInit_state.run_on_tick_100Hz();
    app::states::hvInit_state.run_on_tick_100Hz();

    EXPECT_TRUE(app::can_tx::VC_INVFLbDcOn_get());
    EXPECT_TRUE(app::can_tx::VC_INVFRbDcOn_get());
    EXPECT_TRUE(app::can_tx::VC_INVRLbDcOn_get());
    EXPECT_TRUE(app::can_tx::VC_INVRRbDcOn_get());

    app::can_rx::INVRR_bQuitDcOn_update(true);
    app::can_rx::INVRL_bQuitDcOn_update(true);
    app::can_rx::INVFR_bQuitDcOn_update(true);
    app::can_rx::INVFL_bQuitDcOn_update(true);

    app::states::hvInit_state.run_on_tick_100Hz();
    app::states::hvInit_state.run_on_tick_100Hz();

    EXPECT_TRUE(app::can_tx::VC_INVFLbEnable_get());
    EXPECT_TRUE(app::can_tx::VC_INVFRbEnable_get());
    EXPECT_TRUE(app::can_tx::VC_INVRLbEnable_get());
    EXPECT_TRUE(app::can_tx::VC_INVRRbEnable_get());
}

TEST_F(VCStateMachineTest, EnableQuitTransitionsToInverterOn)
{
    SetStateWithEntry(&app::states::hvInit_state);
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_CLOSED);

    app::can_rx::INVFL_bSystemReady_update(true);
    app::can_rx::INVFR_bSystemReady_update(true);
    app::can_rx::INVRL_bSystemReady_update(true);
    app::can_rx::INVRR_bSystemReady_update(true);

    LetTimePass(10);
    app::states::hvInit_state.run_on_tick_100Hz();
    EXPECT_TRUE(app::can_tx::VC_INVFLbDcOn_get()) << "DC_ON not asserted";

    app::can_rx::INVRR_bQuitDcOn_update(true);
    app::can_rx::INVRL_bQuitDcOn_update(true);
    app::can_rx::INVFR_bQuitDcOn_update(true);
    app::can_rx::INVFL_bQuitDcOn_update(true);

    LetTimePass(10);
    LetTimePass(10);
    EXPECT_TRUE(app::can_tx::VC_INVFLbEnable_get()) << "ENABLE not asserted";

    app::can_rx::INVRR_bQuitInverterOn_update(true);
    app::can_rx::INVRL_bQuitInverterOn_update(true);
    app::can_rx::INVFR_bQuitInverterOn_update(true);
    app::can_rx::INVFL_bQuitInverterOn_update(true);

    LetTimePass(10);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::hv_state);
}

TEST_F(VCStateMachineTest, ReadyForDriveWithRetryFlagGoesToDriveState)
{
    SetStateWithEntry(&app::states::hvInit_state);
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_CLOSED);

    app::can_rx::INVFL_bSystemReady_update(true);
    app::can_rx::INVFR_bSystemReady_update(true);
    app::can_rx::INVRL_bSystemReady_update(true);
    app::can_rx::INVRR_bSystemReady_update(true);
    LetTimePass(20);

    app::can_rx::INVFL_bQuitDcOn_update(true);
    app::can_rx::INVFR_bQuitDcOn_update(true);
    app::can_rx::INVRL_bQuitDcOn_update(true);
    app::can_rx::INVRR_bQuitDcOn_update(true);
    LetTimePass(20);

    app::can_rx::INVFL_bQuitInverterOn_update(true);
    app::can_rx::INVFR_bQuitInverterOn_update(true);
    app::can_rx::INVRL_bQuitInverterOn_update(true);
    app::can_rx::INVRR_bQuitInverterOn_update(true);
    app::can_alerts::infos::InverterRetry_set(true);

    LetTimePass(20);

    app::can_rx::FSM_BrakeActuated_update(true);
    app::can_rx::CRIT_StartButton_update(SwitchState::ON);
    LetTimePass(20);

    ASSERT_STATE_EQ(app::states::drive_state);
}

TEST_F(VCStateMachineTest, DriveStateRetryToHvInit)
{
    app::can_rx::CRIT_StartButton_update(SwitchState::ON);
    LetTimePass(10);
    ASSERT_TRUE(app::startSwitch::hasRisingEdge());

    SetStateWithEntry(&app::states::drive_state);
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_CLOSED);
    LetTimePass(20);

    app::can_rx::INVFL_bError_update(true);
    app::can_rx::INVFR_bError_update(true);
    app::can_rx::INVRL_bError_update(true);
    app::can_rx::INVRR_bError_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverter_fault_handling_state);

    app::can_rx::INVFL_bError_update(false);
    app::can_rx::INVFR_bError_update(false);
    app::can_rx::INVRL_bError_update(false);
    app::can_rx::INVRR_bError_update(false);
    LetTimePass(10);

    app::can_rx::INVFL_bSystemReady_update(true);
    app::can_rx::INVFR_bSystemReady_update(true);
    app::can_rx::INVRL_bSystemReady_update(true);
    app::can_rx::INVRR_bSystemReady_update(true);
    app::states::hvInit_state.run_on_tick_100Hz();

    app::can_rx::INVFL_bQuitDcOn_update(true);
    app::can_rx::INVFR_bQuitDcOn_update(true);
    app::can_rx::INVRL_bQuitDcOn_update(true);
    app::can_rx::INVRR_bQuitDcOn_update(true);

    app::can_rx::INVFL_bQuitInverterOn_update(true);
    app::can_rx::INVFR_bQuitInverterOn_update(true);
    app::can_rx::INVRL_bQuitInverterOn_update(true);
    app::can_rx::INVRR_bQuitInverterOn_update(true);

    app::can_rx::CRIT_StartButton_update(SwitchState::OFF);
    app::can_rx::FSM_BrakeActuated_update(true);

    LetTimePass(200);

    app::can_rx::CRIT_StartButton_update(SwitchState::ON);
    app::can_rx::FSM_BrakeActuated_update(true);

    LetTimePass(100);
    ASSERT_STATE_EQ(app::states::drive_state);
}

/* ------------------------- HV STATE ------------------------------- */
TEST_F(VCStateMachineTest, HvStateEntrySetsHvOnAndPowerManager)
{
    SetStateWithEntry(&app::states::hv_state);

    EXPECT_EQ(app::can_tx::VC_State_get(), VCState::VC_HV_ON_STATE);
    EXPECT_TRUE(app::powerManager::getEfuse(0));
    EXPECT_TRUE(app::powerManager::getEfuse(1));
    EXPECT_TRUE(app::powerManager::getEfuse(4));
    EXPECT_TRUE(app::powerManager::getEfuse(5));
}

TEST_F(VCStateMachineTest, HvStateTransitionsToDriveOnBrakeAndStart)
{
    app::can_rx::CRIT_StartButton_update(SwitchState::ON);
    ASSERT_TRUE(app::startSwitch::hasRisingEdge());

    SetStateWithEntry(&app::states::hv_state);
    app::can_rx::FSM_BrakeActuated_update(true);

    LetTimePass(20);
    ASSERT_STATE_EQ(app::states::drive_state);
}

/* ------------------------- DRIVE STATE ------------------------------- */
TEST_F(VCStateMachineTest, DriveStateEntryConfiguresPowerManager)
{
    SetStateWithEntry(&app::states::drive_state);
    LetTimePass(10);
    EXPECT_TRUE(app::powerManager::getEfuse(0));
    EXPECT_TRUE(app::powerManager::getEfuse(1));
    EXPECT_TRUE(app::powerManager::getEfuse(4));
    EXPECT_TRUE(app::powerManager::getEfuse(5));
}

/* ------------------------- INVERTER FAULT HANDLING STATE ------------------------------- */
TEST_F(VCStateMachineTest, InverterFaultHandlerEntrySetsRetryFlag)
{
    SetStateWithEntry(&app::states::inverter_fault_handling_state);

    EXPECT_EQ(app::can_tx::VC_State_get(), VCState::VC_INVERTER_FAULT_HANDELER);
    EXPECT_TRUE(app::can_tx::VC_Info_InverterRetry_get());
}

TEST_F(VCStateMachineTest, InverterFaultHandlerTransitionsToHvInitOnRecovery)
{
    app::can_rx::INVFL_bError_update(true);
    app::can_rx::INVFR_bError_update(true);
    app::can_rx::INVRL_bError_update(true);
    app::can_rx::INVRR_bError_update(true);

    app::StateMachine::set_current_state(&app::states::drive_state);
    app::inverter::FaultCheck();
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverter_fault_handling_state);

    app::can_rx::INVFL_bError_update(false);
    app::can_rx::INVFR_bError_update(false);
    app::can_rx::INVRL_bError_update(false);
    app::can_rx::INVRR_bError_update(false);

    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::hvInit_state);
}

TEST_F(VCStateMachineTest, InverterFaultHandlerLockoutTransitionsToFaultState)
{
    app::can_rx::INVFL_ErrorInfo_update(259u);
    app::can_rx::INVFR_ErrorInfo_update(259u);
    app::can_rx::INVRL_ErrorInfo_update(259u);
    app::can_rx::INVRR_ErrorInfo_update(259u);

    SetStateWithEntry(&app::states::inverter_fault_handling_state);
    LetTimePass(10);

    ASSERT_STATE_EQ(app::states::fault_state);
    EXPECT_TRUE(app::can_tx::VC_Fault_InvLockoutFault_get());
}
