#include "test_VCBase.hpp"

#include "app_canAlerts.hpp"
#include "app_canRx.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "app_startSwitch.hpp"
#include "io_pcm.hpp"
#include "states/app_states.hpp"

using namespace app::can_utils;

static void SetStateWithEntry(const app::State *state)
{
    app::StateMachine::set_current_state(state);
    state->run_on_entry();
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_CLOSED);
}

static void SetAllSystemReady(const bool ready)
{
    app::can_rx::INVFL_bSystemReady_update(ready);
    app::can_rx::INVFR_bSystemReady_update(ready);
    app::can_rx::INVRL_bSystemReady_update(ready);
    app::can_rx::INVRR_bSystemReady_update(ready);
}

static void SetAllQuitDcOn(const bool quit)
{
    app::can_rx::INVFL_bQuitDcOn_update(quit);
    app::can_rx::INVFR_bQuitDcOn_update(quit);
    app::can_rx::INVRL_bQuitDcOn_update(quit);
    app::can_rx::INVRR_bQuitDcOn_update(quit);
}

static void SetAllQuitInverterOn(const bool quit)
{
    app::can_rx::INVFL_bQuitInverterOn_update(quit);
    app::can_rx::INVFR_bQuitInverterOn_update(quit);
    app::can_rx::INVRL_bQuitInverterOn_update(quit);
    app::can_rx::INVRR_bQuitInverterOn_update(quit);
}

static void SetAllInverterErrors(const bool faulted)
{
    app::can_rx::INVFL_bError_update(faulted);
    app::can_rx::INVFR_bError_update(faulted);
    app::can_rx::INVRL_bError_update(faulted);
    app::can_rx::INVRR_bError_update(faulted);
}

static void ResetCanMessages()
{
    app::can_rx::clear_board_rx_table(CanNode::BMS_NODE);
    app::can_rx::clear_board_rx_table(CanNode::RSM_NODE);
    app::can_rx::clear_board_rx_table(CanNode::FSM_NODE);
    app::can_rx::clear_board_rx_table(CanNode::CRIT_NODE);
    app::can_rx::clear_board_rx_table(CanNode::DAM_NODE);

    SetAllSystemReady(false);
    SetAllQuitDcOn(false);
    SetAllQuitInverterOn(false);
    SetAllInverterErrors(false);
    app::can_rx::INVFL_ErrorInfo_update(0u);

    app::can_tx::VC_ChannelOneVoltage_set(0.0f);
    app::can_tx::VC_PcmRetryCount_set(0u);
    app::can_tx::VC_Info_PcmUnderVoltage_set(false);
    app::can_tx::VC_Info_InverterRetry_set(false);
    app::can_tx::VC_Fault_InvLockoutFault_set(false);
    app::can_tx::VC_INVFLbDcOn_set(false);
    app::can_tx::VC_INVFRbDcOn_set(false);
    app::can_tx::VC_INVRLbDcOn_set(false);
    app::can_tx::VC_INVRRbDcOn_set(false);
    app::can_tx::VC_INVFLbEnable_set(false);
    app::can_tx::VC_INVFRbEnable_set(false);
    app::can_tx::VC_INVRLbEnable_set(false);
    app::can_tx::VC_INVRRbEnable_set(false);
    app::can_tx::VC_INVFLbInverterOn_set(false);
    app::can_tx::VC_INVFRbInverterOn_set(false);
    app::can_tx::VC_INVRLbInverterOn_set(false);
    app::can_tx::VC_INVRRbInverterOn_set(false);
    app::can_tx::VC_INVFLbErrorReset_set(false);
    app::can_tx::VC_INVFRbErrorReset_set(false);
    app::can_tx::VC_INVRLbErrorReset_set(false);
    app::can_tx::VC_INVRRbErrorReset_set(false);
    app::can_tx::VC_INVFLTorqueSetpoint_set(0);
    app::can_tx::VC_INVFRTorqueSetpoint_set(0);
    app::can_tx::VC_INVRLTorqueSetpoint_set(0);
    app::can_tx::VC_INVRRTorqueSetpoint_set(0);
    app::can_tx::VC_INVFLTorqueLimitNegative_set(0);
    app::can_tx::VC_INVFRTorqueLimitNegative_set(0);
    app::can_tx::VC_INVRLTorqueLimitNegative_set(0);
    app::can_tx::VC_INVRRTorqueLimitNegative_set(0);
    app::can_tx::VC_INVFLTorqueLimitPositive_set(0);
    app::can_tx::VC_INVFRTorqueLimitPositive_set(0);
    app::can_tx::VC_INVRLTorqueLimitPositive_set(0);
    app::can_tx::VC_INVRRTorqueLimitPositive_set(0);

    app::can_alerts::infos::InverterRetry_set(false);
    app::can_alerts::warnings::FrontLeftInverterFault_set(false);
}

class VCStateMachineTest : public VCBaseTest
{
    void board_teardown() override
    {
        ResetCanMessages();
    }
};

TEST_F(VCStateMachineTest, InitToInverterOnTransition)
{
    SetStateWithEntry(&app::states::init_state);
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_OPEN);

    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::init_state);

    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverterOn_state);
}

TEST_F(VCStateMachineTest, InverterOnToBmsOnTransition)
{
    SetStateWithEntry(&app::states::inverterOn_state);

    app::can_rx::INVFL_bSystemReady_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverterOn_state);

    app::can_rx::INVFR_bSystemReady_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverterOn_state);

    app::can_rx::INVRL_bSystemReady_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverterOn_state);

    app::can_rx::INVRR_bSystemReady_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::bmsOn_state);
}

TEST_F(VCStateMachineTest, BmsOnTransitionsToPcmOnOnlyInDriveState)
{
    SetStateWithEntry(&app::states::bmsOn_state);

    app::can_rx::BMS_State_update(BmsState::BMS_PRECHARGE_DRIVE_STATE);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::bmsOn_state);

    app::can_rx::BMS_State_update(BmsState::BMS_DRIVE_STATE);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::pcmOn_state);
}

TEST_F(VCStateMachineTest, HvInitEntryInitializesStateAndOutputs)
{
    SetStateWithEntry(&app::states::hvInit_state);

    EXPECT_EQ(app::can_tx::VC_State_get(), VCState::VC_HV_INIT_STATE);
    EXPECT_EQ(app::can_tx::VC_INVFRTorqueSetpoint_get(), 0);
    EXPECT_EQ(app::can_tx::VC_INVRRTorqueSetpoint_get(), 0);
    EXPECT_EQ(app::can_tx::VC_INVFLTorqueSetpoint_get(), 0);
    EXPECT_EQ(app::can_tx::VC_INVRLTorqueSetpoint_get(), 0);
}

TEST_F(VCStateMachineTest, HvInitSystemReadyTransitionsToDcOn)
{
    SetStateWithEntry(&app::states::hvInit_state);
    SetAllSystemReady(true);

    LetTimePass(10);
    EXPECT_EQ(app::can_tx::VC_InverterState_get(), VCInverterState::INV_DC_ON);
    EXPECT_TRUE(app::can_tx::VC_INVFLbDcOn_get());
    EXPECT_TRUE(app::can_tx::VC_INVFRbDcOn_get());
    EXPECT_TRUE(app::can_tx::VC_INVRLbDcOn_get());
    EXPECT_TRUE(app::can_tx::VC_INVRRbDcOn_get());
}

TEST_F(VCStateMachineTest, HvInitDcOnQuitTransitionsToEnable)
{
    SetStateWithEntry(&app::states::hvInit_state);
    SetAllSystemReady(true);
    LetTimePass(10);

    SetAllQuitDcOn(true);
    LetTimePass(10);

    EXPECT_EQ(app::can_tx::VC_InverterState_get(), VCInverterState::INV_ENABLE);
    EXPECT_TRUE(app::can_tx::VC_INVFLbEnable_get());
    EXPECT_TRUE(app::can_tx::VC_INVFRbEnable_get());
    EXPECT_TRUE(app::can_tx::VC_INVRLbEnable_get());
    EXPECT_TRUE(app::can_tx::VC_INVRRbEnable_get());
}

TEST_F(VCStateMachineTest, HvInitEnableQuitTransitionsToHv)
{
    SetStateWithEntry(&app::states::hvInit_state);
    SetAllSystemReady(true);
    LetTimePass(10);

    SetAllQuitDcOn(true);
    LetTimePass(10);
    LetTimePass(10);

    SetAllQuitInverterOn(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::hv_state);
}

TEST_F(VCStateMachineTest, HvEntryInitializesState)
{
    SetStateWithEntry(&app::states::hv_state);
    EXPECT_EQ(app::can_tx::VC_State_get(), VCState::VC_HV_ON_STATE);
}

TEST_F(VCStateMachineTest, HvDoesNotEnterDriveWithoutBrakeAndStart)
{
    SetStateWithEntry(&app::states::hv_state);

    app::can_rx::CRIT_StartButton_update(SwitchState::OFF);
    app::can_rx::FSM_BrakeActuated_update(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::hv_state);
}

TEST_F(VCStateMachineTest, HvBrakeAndStartRisingEdgeTransitionsToDrive)
{
    SetStateWithEntry(&app::states::hv_state);

    app::can_rx::CRIT_StartButton_update(SwitchState::OFF);
    (void)app::startSwitch::hasRisingEdge();
    app::can_rx::FSM_BrakeActuated_update(true);
    app::can_rx::CRIT_StartButton_update(SwitchState::ON);

    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::drive_state);
}

TEST_F(VCStateMachineTest, DriveStateEntryKeepsDriveState)
{
    SetStateWithEntry(&app::states::drive_state);

    LetTimePass(20);
    ASSERT_STATE_EQ(app::states::drive_state);
}

TEST_F(VCStateMachineTest, PcmOnEntryInitializesState)
{
    SetStateWithEntry(&app::states::pcmOn_state);

    EXPECT_EQ(app::can_tx::VC_State_get(), VCState::VC_PCM_ON_STATE);
    EXPECT_FALSE(io::pcm::enabled());
}

TEST_F(VCStateMachineTest, PcmGoodVoltageTransitionsToHvInit)
{
    SetStateWithEntry(&app::states::pcmOn_state);
    app::can_rx::BMS_IrNegative_update(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    app::can_tx::VC_ChannelOneVoltage_set(20.0f);

    LetTimePass(20);
    ASSERT_EQ(app::can_tx::VC_PcmRetryCount_get(), 0);
    ASSERT_STATE_EQ(app::states::hvInit_state);
}

TEST_F(VCStateMachineTest, PcmUnderVoltageRetriesThenFaults)
{
    SetStateWithEntry(&app::states::pcmOn_state);
    app::can_rx::BMS_IrNegative_update(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    app::can_tx::VC_ChannelOneVoltage_set(16.0f);
    ASSERT_EQ(app::can_tx::VC_PcmRetryCount_get(), 0);

    LetTimePass(20);
    EXPECT_TRUE(io::pcm::enabled());

    for (uint8_t retry = 1; retry < 5; retry++)
    {
        LetTimePass(110);
        EXPECT_FALSE(io::pcm::enabled());
        ASSERT_EQ(app::can_tx::VC_PcmRetryCount_get(), retry);

        LetTimePass(110);
        EXPECT_TRUE(io::pcm::enabled());
        ASSERT_STATE_EQ(app::states::pcmOn_state);
    }

    LetTimePass(110);
    EXPECT_FALSE(io::pcm::enabled());
    ASSERT_EQ(app::can_tx::VC_PcmRetryCount_get(), 5);
    LetTimePass(110);

    EXPECT_TRUE(app::can_tx::VC_Info_PcmUnderVoltage_get());
    ASSERT_STATE_EQ(app::states::fault_state);
}

TEST_F(VCStateMachineTest, InverterRetryOneFaultedInverterFromDrive)
{
    SetStateWithEntry(&app::states::drive_state);

    app::can_rx::INVFL_bError_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverter_fault_handling_state);

    app::can_rx::INVFL_bError_update(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::hvInit_state);
    ASSERT_FALSE(app::can_tx::VC_Info_InverterRetry_get());
}

TEST_F(VCStateMachineTest, InverterRetryOneFaultedInverterFromInit)
{
    SetStateWithEntry(&app::states::init_state);
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_OPEN);
    LetTimePass(10);
    app::can_rx::INVFL_bError_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverter_fault_handling_state);

    app::can_rx::INVFL_bError_update(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::init_state);
}

TEST_F(VCStateMachineTest, InverterRetryOneFaultedInverterFromInverterOn)
{
    SetStateWithEntry(&app::states::inverterOn_state);

    app::can_rx::INVFL_bError_update(true);
    LetTimePass(50);
    ASSERT_STATE_EQ(app::states::inverter_fault_handling_state);

    app::can_rx::INVFL_bError_update(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverterOn_state);
}

TEST_F(VCStateMachineTest, InverterRetryOneFaultedInverterFromBmsOn)
{
    SetStateWithEntry(&app::states::bmsOn_state);

    app::can_rx::INVFL_bError_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverter_fault_handling_state);

    app::can_rx::INVFL_bError_update(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::bmsOn_state);
}

TEST_F(VCStateMachineTest, InverterRetryOneFaultedInverterFromHvInit)
{
    SetStateWithEntry(&app::states::hvInit_state);

    app::can_rx::INVFL_bError_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverter_fault_handling_state);

    app::can_rx::INVFL_bError_update(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::hvInit_state);
}

TEST_F(VCStateMachineTest, InverterRetryOneFaultedInverterFromHv)
{
    SetStateWithEntry(&app::states::hv_state);

    app::can_rx::INVFL_bError_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverter_fault_handling_state);

    app::can_rx::INVFL_bError_update(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::hvInit_state);
}

TEST_F(VCStateMachineTest, InverterRetryMoreThanOneFaultedInverter)
{
    SetStateWithEntry(&app::states::drive_state);

    SetAllInverterErrors(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverter_fault_handling_state);
}

TEST_F(VCStateMachineTest, InverterFaultLockoutTransitionsToFault)
{
    SetStateWithEntry(&app::states::drive_state);

    app::can_rx::INVFL_bError_update(true);
    app::can_rx::INVFL_ErrorInfo_update(259u);
    LetTimePass(20);

    ASSERT_STATE_EQ(app::states::fault_state);
    ASSERT_TRUE(app::can_tx::VC_Fault_InvLockoutFault_get());
}

TEST_F(VCStateMachineTest, InverterRetryTwoFaultsInARow)
{
    SetStateWithEntry(&app::states::bmsOn_state);
    app::can_rx::INVFL_ErrorInfo_update(0u);

    app::can_rx::INVFL_bError_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverter_fault_handling_state);
    app::can_rx::INVFL_bError_update(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::bmsOn_state);

    app::can_rx::BMS_State_update(BmsState::BMS_DRIVE_STATE);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::pcmOn_state);
    app::can_tx::VC_ChannelOneVoltage_set(20.0f);
    LetTimePass(20);
    ASSERT_STATE_EQ(app::states::hvInit_state);

    app::can_rx::INVFL_bError_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverter_fault_handling_state);
    app::can_rx::INVFL_bError_update(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::hvInit_state);
}

TEST_F(VCStateMachineTest, InitDoesNotExitUntilLatchClosed)
{
    SetStateWithEntry(&app::states::init_state);
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_OPEN);

    LetTimePass(1010);
    ASSERT_STATE_EQ(app::states::init_state);

    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverterOn_state);
}

TEST_F(VCStateMachineTest, AirMinusOpenDebounceReturnsInverterOnToInit)
{
    SetStateWithEntry(&app::states::inverterOn_state);
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_OPEN);

    LetTimePass(110);
    ASSERT_STATE_EQ(app::states::init_state);
}

TEST_F(VCStateMachineTest, AirMinusOpenDebounceReturnsBmsOnToInit)
{
    SetStateWithEntry(&app::states::bmsOn_state);
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_OPEN);

    LetTimePass(110);
    ASSERT_STATE_EQ(app::states::init_state);
}

TEST_F(VCStateMachineTest, AirMinusOpenDebounceReturnsPcmOnToInit)
{
    SetStateWithEntry(&app::states::pcmOn_state);
    app::can_tx::VC_ChannelOneVoltage_set(28.0f);
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_OPEN);

    LetTimePass(110);
    ASSERT_STATE_EQ(app::states::init_state);
}

/* Quintuna VC state-machine test-name parity. */

TEST_F(VCStateMachineTest, BmsOnTransitionnToHvInit)
{
    SetStateWithEntry(&app::states::bmsOn_state);

    app::can_rx::BMS_State_update(BmsState::BMS_PRECHARGE_DRIVE_STATE);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::bmsOn_state);

    app::can_rx::BMS_State_update(BmsState::BMS_DRIVE_STATE);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::pcmOn_state);
}

TEST_F(VCStateMachineTest, EntryInitializesStateAndOutputs)
{
    SetStateWithEntry(&app::states::hvInit_state);

    ASSERT_EQ(app::can_tx::VC_State_get(), VCState::VC_HV_INIT_STATE);
    EXPECT_EQ(app::can_tx::VC_INVFRTorqueSetpoint_get(), 0);
    EXPECT_EQ(app::can_tx::VC_INVRRTorqueSetpoint_get(), 0);
    EXPECT_EQ(app::can_tx::VC_INVFLTorqueSetpoint_get(), 0);
    EXPECT_EQ(app::can_tx::VC_INVRLTorqueSetpoint_get(), 0);
}

TEST_F(VCStateMachineTest, SystemReadyTransitionsToDcOn)
{
    SetStateWithEntry(&app::states::hvInit_state);
    SetAllSystemReady(true);

    LetTimePass(10);
    EXPECT_TRUE(app::can_tx::VC_INVFLbDcOn_get());
    EXPECT_TRUE(app::can_tx::VC_INVFRbDcOn_get());
    EXPECT_TRUE(app::can_tx::VC_INVRLbDcOn_get());
    EXPECT_TRUE(app::can_tx::VC_INVRRbDcOn_get());
}

TEST_F(VCStateMachineTest, DcOnQuitTransitionsToEnable)
{
    SetStateWithEntry(&app::states::hvInit_state);
    SetAllSystemReady(true);
    LetTimePass(10);

    SetAllQuitDcOn(true);
    LetTimePass(10);

    EXPECT_TRUE(app::can_tx::VC_INVFLbEnable_get());
    EXPECT_TRUE(app::can_tx::VC_INVFRbEnable_get());
    EXPECT_TRUE(app::can_tx::VC_INVRLbEnable_get());
    EXPECT_TRUE(app::can_tx::VC_INVRRbEnable_get());
}

TEST_F(VCStateMachineTest, EnableQuitTransitionsToInverterOn)
{
    SetStateWithEntry(&app::states::hvInit_state);
    SetAllSystemReady(true);
    LetTimePass(10);

    SetAllQuitDcOn(true);
    LetTimePass(20);
    EXPECT_TRUE(app::can_tx::VC_INVFLbEnable_get()) << "ENABLE not asserted";

    SetAllQuitInverterOn(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::hv_state);
}

TEST_F(VCStateMachineTest, ReadyForDriveWithRetryFlagGoesToDriveState)
{
    SetStateWithEntry(&app::states::hvInit_state);
    SetAllSystemReady(true);
    LetTimePass(10);

    SetAllQuitDcOn(true);
    LetTimePass(20);

    SetAllQuitInverterOn(true);
    app::can_alerts::infos::InverterRetry_set(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::hv_state);

    app::can_rx::CRIT_StartButton_update(SwitchState::OFF);
    (void)app::startSwitch::hasRisingEdge();
    app::can_rx::FSM_BrakeActuated_update(true);
    app::can_rx::CRIT_StartButton_update(SwitchState::ON);
    LetTimePass(20);

    ASSERT_STATE_EQ(app::states::drive_state);
}

TEST_F(VCStateMachineTest, DriveStateRetrytoHvInit)
{
    SetStateWithEntry(&app::states::drive_state);
    SetAllInverterErrors(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverter_fault_handling_state);

    SetAllInverterErrors(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::hvInit_state);

    SetAllSystemReady(true);
    SetAllQuitDcOn(true);
    SetAllQuitInverterOn(true);
    LetTimePass(40);
    ASSERT_STATE_EQ(app::states::hv_state);

    app::can_rx::CRIT_StartButton_update(SwitchState::OFF);
    (void)app::startSwitch::hasRisingEdge();
    app::can_rx::FSM_BrakeActuated_update(true);
    app::can_rx::CRIT_StartButton_update(SwitchState::ON);
    LetTimePass(20);

    ASSERT_STATE_EQ(app::states::drive_state);
    ASSERT_FALSE(app::can_tx::VC_Info_InverterRetry_get());
}

TEST_F(VCStateMachineTest, EntryInitializesState)
{
    SetStateWithEntry(&app::states::hv_state);
    EXPECT_EQ(app::can_tx::VC_State_get(), VCState::VC_HV_ON_STATE);
}

TEST_F(VCStateMachineTest, NoTransitionWithoutBrakeAndStart)
{
    SetStateWithEntry(&app::states::hv_state);

    app::can_rx::CRIT_StartButton_update(SwitchState::OFF);
    app::can_rx::FSM_BrakeActuated_update(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::hv_state);
}

TEST_F(VCStateMachineTest, fault_and_open_irs_gives_fault_state)
{
    SetStateWithEntry(&app::states::hv_state);

    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_OPEN);
    app::can_alerts::warnings::FrontLeftInverterFault_set(true);
    LetTimePass(110);

    ASSERT_STATE_EQ(app::states::init_state);
}

TEST_F(VCStateMachineTest, NoTransitionWithoutBrakeEvenIfStart)
{
    SetStateWithEntry(&app::states::hv_state);

    app::can_rx::CRIT_StartButton_update(SwitchState::OFF);
    app::can_rx::FSM_BrakeActuated_update(false);
    LetTimePass(10);
    app::can_rx::CRIT_StartButton_update(SwitchState::ON);
    app::can_rx::FSM_BrakeActuated_update(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::hv_state);
}

TEST_F(VCStateMachineTest, StartSwitchOffTransitionsToHv)
{
    GTEST_SKIP() << "Hexray drive_state currently has no start-button-off transition back to HV.";
}

TEST_F(VCStateMachineTest, RunAlgorithmSetsTorque)
{
    GTEST_SKIP() << "Hexray drive_state does not currently run the Quintuna pedal-to-torque algorithm.";
}

TEST_F(VCStateMachineTest, DisableVanillaEnterPower)
{
    GTEST_SKIP() << "Hexray VC does not currently expose Quintuna VanillaOverride/VC_VcDriveMode behavior.";
}

TEST_F(VCStateMachineTest, DisableVanillaEnterPowerActiveDiff)
{
    GTEST_SKIP() << "Hexray VC does not currently expose Quintuna VanillaOverride/active-diff drive-mode behavior.";
}

TEST_F(VCStateMachineTest, RegenSwitchOffSetsNotAvailable)
{
    GTEST_SKIP() << "Hexray drive_state does not currently run Quintuna regen availability handling.";
}

TEST_F(VCStateMachineTest, EntryInitializesPcmOn)
{
    SetStateWithEntry(&app::states::pcmOn_state);

    EXPECT_EQ(app::can_tx::VC_State_get(), VCState::VC_PCM_ON_STATE);
    LetTimePass(10);
    EXPECT_TRUE(io::pcm::enabled());
}

TEST_F(VCStateMachineTest, InverterRetryOneFaultedInverterFromInvOn)
{
    SetStateWithEntry(&app::states::inverterOn_state);

    app::can_rx::INVFL_bError_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverter_fault_handling_state);

    app::can_rx::INVFL_bError_update(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverterOn_state);
}

TEST_F(VCStateMachineTest, InverterFaultLockout)
{
    SetStateWithEntry(&app::states::drive_state);

    app::can_rx::INVFL_bError_update(true);
    app::can_rx::INVFL_ErrorInfo_update(259u);
    LetTimePass(20);

    ASSERT_STATE_EQ(app::states::fault_state);
    ASSERT_TRUE(app::can_tx::VC_Fault_InvLockoutFault_get());
}

TEST_F(VCStateMachineTest, InverterRetryRecovered)
{
    SetStateWithEntry(&app::states::drive_state);

    app::can_rx::INVFL_bError_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverter_fault_handling_state);

    app::can_rx::INVFL_bError_update(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::hvInit_state);

    SetAllSystemReady(true);
    SetAllQuitDcOn(true);
    SetAllQuitInverterOn(true);
    LetTimePass(40);
    ASSERT_STATE_EQ(app::states::hv_state);

    app::can_rx::CRIT_StartButton_update(SwitchState::OFF);
    (void)app::startSwitch::hasRisingEdge();
    app::can_rx::FSM_BrakeActuated_update(true);
    app::can_rx::CRIT_StartButton_update(SwitchState::ON);

    LetTimePass(100);
    ASSERT_STATE_EQ(app::states::drive_state);
    ASSERT_FALSE(app::can_tx::VC_Info_InverterRetry_get());
}

TEST_F(VCStateMachineTest, InverterRetryTwoFaultsInaRow)
{
    SetStateWithEntry(&app::states::bmsOn_state);

    app::can_rx::INVFL_bError_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverter_fault_handling_state);
    app::can_rx::INVFL_bError_update(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::bmsOn_state);

    app::can_rx::BMS_State_update(BmsState::BMS_DRIVE_STATE);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::pcmOn_state);
    app::can_tx::VC_ChannelOneVoltage_set(20.0f);
    LetTimePass(20);
    ASSERT_STATE_EQ(app::states::hvInit_state);

    app::can_rx::INVFL_bError_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverter_fault_handling_state);
    app::can_rx::INVFL_bError_update(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::hvInit_state);
}

TEST_F(VCStateMachineTest, GoodVoltageTransitionsToHvInit)
{
    SetStateWithEntry(&app::states::pcmOn_state);
    app::can_tx::VC_ChannelOneVoltage_set(20.0f);

    LetTimePass(20);
    ASSERT_EQ(app::can_tx::VC_PcmRetryCount_get(), 0);
    ASSERT_STATE_EQ(app::states::hvInit_state);
}

TEST_F(VCStateMachineTest, initDoesNotExitUnitlLatchClosed)
{
    SetStateWithEntry(&app::states::init_state);
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_OPEN);

    LetTimePass(1010);
    ASSERT_STATE_EQ(app::states::init_state);

    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverterOn_state);
}

TEST_F(VCStateMachineTest, inverterOnStateLatchedFault)
{
    SetStateWithEntry(&app::states::inverterOn_state);

    app::can_rx::INVFL_bSystemReady_update(true);
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_OPEN);

    LetTimePass(110);
    EXPECT_EQ(app::can_rx::BMS_IrNegative_get(), ContactorState::CONTACTOR_STATE_OPEN);
    ASSERT_STATE_EQ(app::states::init_state);
}

TEST_F(VCStateMachineTest, bmsOnLatchedFault)
{
    SetStateWithEntry(&app::states::bmsOn_state);

    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_OPEN);
    LetTimePass(110);
    ASSERT_STATE_EQ(app::states::init_state);

    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::inverterOn_state);
}

TEST_F(VCStateMachineTest, pcmOnStateLatchedFault)
{
    SetStateWithEntry(&app::states::pcmOn_state);
    app::can_tx::VC_ChannelOneVoltage_set(28.0f);
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_OPEN);

    LetTimePass(110);
    ASSERT_STATE_EQ(app::states::init_state);
}
