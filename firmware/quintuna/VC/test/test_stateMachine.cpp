#include "app_canUtils.h"
#include "test_VCBase.hpp"
#include "vcFakes.h"
#include "io_imuFake.h"

extern "C"
{
#include "app_stateMachine.h"
#include "states/app_states.h"
#include "app_startSwitch.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_pcm.h"
#include "app_vehicleDynamicsConstants.h"
}

class VCStateMachineTest : public VCBaseTest
{
};

// Helper to set state and invoke its entry action
static void SetStateWithEntry(const State *s)
{
    app_stateMachine_setCurrentState(s);
    s->run_on_entry();
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
}

/* ------------------------- INIT STATE ------------------------------- */
TEST_F(VCStateMachineTest, InitToInverterOnTransition)
{
    app_stateMachine_setCurrentState(&init_state);
    init_state.run_on_entry();
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_OPEN);

    // With contactor open, remain in INIT
    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);

    // Close contactor -> transition to INVERTER ON
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    ASSERT_STATE_EQ(inverterOn_state);
}

/* ------------------------ INVERTER ON STATE ------------------------------- */
TEST_F(VCStateMachineTest, InverterOnToBmsOnTransition)
{
    SetStateWithEntry(&inverterOn_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    LetTimePass(10);

    // Transition only after all inverters ready
    app_canRx_INVFL_bSystemReady_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(inverterOn_state);

    app_canRx_INVFR_bSystemReady_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(inverterOn_state);

    app_canRx_INVRL_bSystemReady_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(inverterOn_state);

    app_canRx_INVRR_bSystemReady_update(true);
    LetTimePass(100);
    ASSERT_STATE_EQ(bmsOn_state);
}

/* ------------------------- BMS ON STATE ------------------------------- */
TEST_F(VCStateMachineTest, BmsOnTransitionnToHvInit)
{
    SetStateWithEntry(&bmsOn_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    LetTimePass(10);

    // Stay in BMS ON for non-drive states
    app_canRx_BMS_State_update(BMS_PRECHARGE_DRIVE_STATE);
    LetTimePass(10);
    ASSERT_STATE_EQ(bmsOn_state);

    // Drive state -> transition to HV INIT
    app_canRx_BMS_State_update(BMS_DRIVE_STATE);
    LetTimePass(10);
    ASSERT_STATE_EQ(pcmOn_state);
}

/* ------------------------- HV INIT STATE ------------------------------- */
TEST_F(VCStateMachineTest, EntryInitializesStateAndOutputs)
{
    // Enter hvInit state
    SetStateWithEntry(&hvInit_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);

    // VC State should be HV_INIT
    ASSERT_EQ(app_canTx_VC_State_get(), VC_HV_INIT_STATE);

    // All torque setpoints should be zero
    EXPECT_EQ(app_canTx_VC_INVFRTorqueSetpoint_get(), 0);
    EXPECT_EQ(app_canTx_VC_INVRRTorqueSetpoint_get(), 0);
    EXPECT_EQ(app_canTx_VC_INVFLTorqueSetpoint_get(), 0);
    EXPECT_EQ(app_canTx_VC_INVRLTorqueSetpoint_get(), 0);
}

TEST_F(VCStateMachineTest, SystemReadyTransitionsToDcOn)
{
    SetStateWithEntry(&hvInit_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);

    // Bring all systemReady true
    app_canRx_INVFL_bSystemReady_update(true);
    app_canRx_INVFR_bSystemReady_update(true);
    app_canRx_INVRL_bSystemReady_update(true);
    app_canRx_INVRR_bSystemReady_update(true);

    // should move to DC_ON stage
    hvInit_state.run_on_tick_100Hz();

    // DC_ON outputs should asserrt
    hvInit_state.run_on_tick_100Hz();
    EXPECT_TRUE(app_canTx_VC_INVFLbDcOn_get());
    EXPECT_TRUE(app_canTx_VC_INVFRbDcOn_get());
    EXPECT_TRUE(app_canTx_VC_INVRLbDcOn_get());
    EXPECT_TRUE(app_canTx_VC_INVRRbDcOn_get());
}

TEST_F(VCStateMachineTest, DcOnQuitTransitionsToEnable)
{
    SetStateWithEntry(&hvInit_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);

    // Transition to DC_ON: assert all system ready signals
    app_canRx_INVFL_bSystemReady_update(true);
    app_canRx_INVFR_bSystemReady_update(true);
    app_canRx_INVRL_bSystemReady_update(true);
    app_canRx_INVRR_bSystemReady_update(true);

    // First to detect system ready, second to assert DC_ON outputs
    hvInit_state.run_on_tick_100Hz();
    hvInit_state.run_on_tick_100Hz();
    EXPECT_TRUE(app_canTx_VC_INVFLbDcOn_get());
    EXPECT_TRUE(app_canTx_VC_INVFRbDcOn_get());
    EXPECT_TRUE(app_canTx_VC_INVRLbDcOn_get());
    EXPECT_TRUE(app_canTx_VC_INVRRbDcOn_get());

    // Provide all quit-dc signals
    app_canRx_INVRR_bQuitDcOn_update(true);
    app_canRx_INVRL_bQuitDcOn_update(true);
    app_canRx_INVFR_bQuitDcOn_update(true);
    app_canRx_INVFL_bQuitDcOn_update(true);

    // Transition to INV_ENABLE
    hvInit_state.run_on_tick_100Hz();

    // INV_ENABLE outputs should assert and move to INV_ON
    hvInit_state.run_on_tick_100Hz();
    EXPECT_TRUE(app_canTx_VC_INVFLbEnable_get());
    EXPECT_TRUE(app_canTx_VC_INVFRbEnable_get());
    EXPECT_TRUE(app_canTx_VC_INVRLbEnable_get());
    EXPECT_TRUE(app_canTx_VC_INVRRbEnable_get());
}

TEST_F(VCStateMachineTest, EnableQuitTransitionsToInverterOn)
{
    SetStateWithEntry(&hvInit_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);

    // Assert all systemReady flags to transition to DC_ON
    app_canRx_INVFL_bSystemReady_update(true);
    app_canRx_INVFR_bSystemReady_update(true);
    app_canRx_INVRL_bSystemReady_update(true);
    app_canRx_INVRR_bSystemReady_update(true);

    // Two ticks to go through SYSTEM_READY -> DC_ON outputs
    LetTimePass(10);
    hvInit_state.run_on_tick_100Hz();
    EXPECT_TRUE(app_canTx_VC_INVFLbDcOn_get()) << "DC_ON not asserted";

    // Provide all quit-dc signals to move to ENABLE
    app_canRx_INVRR_bQuitDcOn_update(true);
    app_canRx_INVRL_bQuitDcOn_update(true);
    app_canRx_INVFR_bQuitDcOn_update(true);
    app_canRx_INVFL_bQuitDcOn_update(true);

    // First transitions to ENABLE, second asserts ENABLE outputs
    LetTimePass(10);
    LetTimePass(10);
    EXPECT_TRUE(app_canTx_VC_INVFLbEnable_get()) << "ENABLE not asserted";

    // Provide all quit-inverterOn signals to move to READY_FOR_DRIVE
    app_canRx_INVRR_bQuitInverterOn_update(true);
    app_canRx_INVRL_bQuitInverterOn_update(true);
    app_canRx_INVFR_bQuitInverterOn_update(true);
    app_canRx_INVFL_bQuitInverterOn_update(true);

    // First moves to READY_FOR_DRIVE, second commits hv_state
    LetTimePass(10);
    LetTimePass(10);
    ASSERT_STATE_EQ(hv_state);
}

TEST_F(VCStateMachineTest, ReadyForDriveWithRetryFlagGoesToDriveState)
{
    SetStateWithEntry(&hvInit_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);

    // run through to READY_FOR_DRIVE
    app_canRx_INVFL_bSystemReady_update(true);
    app_canRx_INVFR_bSystemReady_update(true);
    app_canRx_INVRL_bSystemReady_update(true);
    app_canRx_INVRR_bSystemReady_update(true);
    LetTimePass(20);

    app_canRx_INVFL_bQuitDcOn_update(true);
    app_canRx_INVFR_bQuitDcOn_update(true);
    app_canRx_INVRL_bQuitDcOn_update(true);
    app_canRx_INVRR_bQuitDcOn_update(true);
    LetTimePass(20);

    // Set retry flag: first tick should transition to DRIVE
    app_canRx_INVFL_bQuitInverterOn_update(true);
    app_canRx_INVFR_bQuitInverterOn_update(true);
    app_canRx_INVRL_bQuitInverterOn_update(true);
    app_canRx_INVRR_bQuitInverterOn_update(true);
    app_canAlerts_VC_Info_InverterRetry_set(true);

    // Go to drive state.
    LetTimePass(20);

    app_canRx_FSM_BrakeActuated_update(true);
    app_canRx_CRIT_StartSwitch_update(SWITCH_ON);
    LetTimePass(20);

    ASSERT_STATE_EQ(drive_state);
}

// I think this test is also not applicable becas
TEST_F(VCStateMachineTest, DriveStateRetrytoHvInit)
{
    // HACK FIX: Start switch is static and not reset from test to test. Do this to force the previous state to be
    // SWITCH_ON and prevent transition out of drive. This shouldn't be an issue on the car since
    // app_startSwitch_hasRisingEdge() will always be called before driving, because it is a condition to enter driving.
    // Ensure previous start state is ON so we can synthesize a rising edge later
    app_canRx_CRIT_StartSwitch_update(SWITCH_ON);
    (void)app_startSwitch_hasRisingEdge();

    // fault inverters
    SetStateWithEntry(&drive_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    LetTimePass(20);

    app_canRx_INVFL_bError_update(true);
    app_canRx_INVFR_bError_update(true);
    app_canRx_INVRL_bError_update(true);
    app_canRx_INVRR_bError_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(inverter_fault_handling_state);

    app_canRx_INVFL_bError_update(false);
    app_canRx_INVFR_bError_update(false);
    app_canRx_INVRL_bError_update(false);
    app_canRx_INVRR_bError_update(false);
    LetTimePass(10);

    // SYSTEM_READY
    app_canRx_INVFL_bSystemReady_update(true);
    app_canRx_INVFR_bSystemReady_update(true);
    app_canRx_INVRL_bSystemReady_update(true);
    app_canRx_INVRR_bSystemReady_update(true);
    hvInit_state.run_on_tick_100Hz();

    // DC_ON quits
    app_canRx_INVFL_bQuitDcOn_update(true);
    app_canRx_INVFR_bQuitDcOn_update(true);
    app_canRx_INVRL_bQuitDcOn_update(true);
    app_canRx_INVRR_bQuitDcOn_update(true);
    hvInit_state.run_on_tick_100Hz();
    hvInit_state.run_on_tick_100Hz();

    // INVERTER_ON quits
    app_canRx_INVFL_bQuitInverterOn_update(true);
    app_canRx_INVFR_bQuitInverterOn_update(true);
    app_canRx_INVRL_bQuitInverterOn_update(true);
    app_canRx_INVRR_bQuitInverterOn_update(true);
    hvInit_state.run_on_tick_100Hz();

    // brake + start rising edge
    app_canRx_FSM_BrakeActuated_update(true);
    app_canRx_CRIT_StartSwitch_update(SWITCH_OFF);
    (void)app_startSwitch_hasRisingEdge();
    app_canRx_CRIT_StartSwitch_update(SWITCH_ON);
    (void)app_startSwitch_hasRisingEdge();

    LetTimePass(10);
    ASSERT_STATE_EQ(drive_state);

    LetTimePass(10);
    // retry flag should be off if recovered
    ASSERT_FALSE(app_canAlerts_VC_Info_InverterRetry_get());
}

/* ------------------------- HV STATE -------------------------------*/
TEST_F(VCStateMachineTest, EntryInitializesState)
{
    SetStateWithEntry(&hv_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    EXPECT_EQ(app_canTx_VC_State_get(), VC_HV_ON_STATE);
}

TEST_F(VCStateMachineTest, NoTransitionWithoutBrakeAndStart)
{
    SetStateWithEntry(&hv_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);

    app_canRx_CRIT_StartSwitch_update(SWITCH_OFF);
    app_canRx_FSM_BrakeActuated_update(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(hv_state);
}

TEST_F(VCStateMachineTest, fault_and_open_irs_gives_fault_state)
{
    SetStateWithEntry(&hv_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);

    // Initial prev_start_switch_pos=true, now simulate off->on
    app_canRx_CRIT_StartSwitch_update(SWITCH_OFF);
    app_canRx_FSM_BrakeActuated_update(true);
    LetTimePass(10);

    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_OPEN);
    app_canAlerts_VC_Warning_FrontLeftInverterFault_set(true);
    LetTimePass(1010); /// the time out timer is 1000ms then to wait for next tick we add 10 ms
    ASSERT_STATE_EQ(init_state);

    app_canAlerts_VC_Warning_FrontLeftInverterFault_set(false); // cleanup
}

TEST_F(VCStateMachineTest, NoTransitionWithoutBrakeEvenIfStart)
{
    SetStateWithEntry(&hv_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);

    app_canRx_CRIT_StartSwitch_update(SWITCH_OFF);
    app_canRx_FSM_BrakeActuated_update(false);
    LetTimePass(10);
    app_canRx_CRIT_StartSwitch_update(SWITCH_ON);
    app_canRx_FSM_BrakeActuated_update(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(hv_state);
}

/* ------------------------- DRIVE STATE ------------------------------- */
// This test is not applicable right now because now we are going into
// handling state and then once recovered back to hvInit which is covered elsewhere
//  TEST_F(VCStateMachineTest, PreCheckInverterFaultTransitionsToHvInit)
//  {
//      SetStateWithEntry(&drive_state);
//      app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);

//     app_canRx_INVFL_bError_update(true);
//     LetTimePass(10);
//     ASSERT_STATE_EQ(inverter_fault_handling_state);
// }

TEST_F(VCStateMachineTest, StartSwitchOffTransitionsToHv)
{
    app_canRx_CRIT_StartSwitch_update(SWITCH_OFF);
    SetStateWithEntry(&drive_state);

    // Simulate start switch off
    app_canRx_CRIT_StartSwitch_update(SWITCH_ON);
    LetTimePass(10);
    ASSERT_STATE_EQ(hv_state);
}

TEST_F(VCStateMachineTest, RunAlgorithmSetsTorque)
{
    suppress_heartbeat = true;
    SetStateWithEntry(&drive_state);
    app_canRx_CRIT_StartSwitch_update(SWITCH_ON);

    // Simulate 50% pedal
    app_canRx_FSM_PappsMappedPedalPercentage_update(50);
    app_canRx_FSM_SappsMappedPedalPercentage_update(50);
    LetTimePass(20);

    ASSERT_STATE_EQ(drive_state);

    // Expect torque = 0.5 * MAX_TORQUE_REQUEST_NM
    int16_t expected = PEDAL_REMAPPING(0.5f * MAX_TORQUE_REQUEST_NM);
    ASSERT_EQ(app_canTx_VC_INVFRTorqueSetpoint_get(), expected);
    ASSERT_EQ(app_canTx_VC_INVFLTorqueSetpoint_get(), expected);
    ASSERT_EQ(app_canTx_VC_INVRRTorqueSetpoint_get(), expected);
    ASSERT_EQ(app_canTx_VC_INVRLTorqueSetpoint_get(), expected);
}

TEST_F(VCStateMachineTest, DisableVanillaEnterPower)
{
    SetStateWithEntry(&drive_state);
    app_canRx_CRIT_VanillaOverrideSwitch_update(SWITCH_OFF);
    app_canRx_CRIT_DriveMode_update(DRIVE_MODE_POWER);

    LetTimePass(10);

    EXPECT_TRUE(app_canTx_VC_VcDriveMode_get() == DRIVE_MODE_POWER);
}

TEST_F(VCStateMachineTest, DisableVanillaEnterPowerActiveDiff)
{
    app_canAlerts_VC_Info_ImuInitFailed_set(false);

    SetStateWithEntry(&drive_state);
    app_canRx_CRIT_VanillaOverrideSwitch_update(SWITCH_OFF);
    app_canRx_CRIT_DriveMode_update(DRIVE_MODE_POWER);

    LetTimePass(10);

    EXPECT_TRUE(app_canTx_VC_VcDriveMode_get() == DRIVE_MODE_POWER);
}

// TODO: Fix this test
// TEST_F(VCStateMachineTest, DisableVanillaEnterTorqueVectoring)
// {
//     // set gps to be ok
//     // app_canTx_VC_Info_SbgInitFailed_set(false);
//     // fake::io_sbgEllipse::setEkfSolutionMode(static_cast<uint32_t>(POSITION));

//     // set imu to be ok
//     // app_canTx_VC_Info_ImuInitFailed_set(false);

//     // set steering to be ok
//     app_canRx_FSM_Info_SteeringAngleOCSC_update(false);
//     app_canRx_FSM_Info_SteeringAngleOutOfRange_update(false);

//     SetStateWithEntry(&drive_state);
//     app_canRx_CRIT_VanillaOverrideSwitch_update(SWITCH_OFF);
//     app_canRx_CRIT_DriveMode_update(DRIVE_MODE_TV);

//     LetTimePass(10);

//     EXPECT_TRUE(app_canTx_VC_VcDriveMode_get() == DRIVE_MODE_TV);
// }

TEST_F(VCStateMachineTest, RegenSwitchOffSetsNotAvailable)
{
    suppress_heartbeat = true;
    SetStateWithEntry(&drive_state);
    app_canRx_CRIT_RegenSwitch_update(SWITCH_OFF);
    LetTimePass(20);
    EXPECT_FALSE(app_canTx_VC_RegenEnabled_get());
    EXPECT_TRUE(app_canAlerts_VC_Info_RegenNotAvailable_get());
}

TEST_F(VCStateMachineTest, EntryInitializesPcmOn)
{
    SetStateWithEntry(&pcmOn_state);
    EXPECT_EQ(app_canTx_VC_State_get(), VC_PCM_ON_STATE);
    LetTimePass(10);
    // TODO: Re-enable PCM_ON state.
    EXPECT_FALSE(io_pcm_enabled());
}

/* ------------------------- INVERTER FAULT HANDLING STATE ------------------------------- */
// Drive state to retry when only one inverter is faulted (interate through all 4)
TEST_F(VCStateMachineTest, InverterRetryOneFaultedInverter)
{
    SetStateWithEntry(&drive_state);
    LetTimePass(10);
    app_canRx_INVFL_bError_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(inverter_fault_handling_state);
}

// Drive state to retry when more than 1 inverter faulted
TEST_F(VCStateMachineTest, InverterRetryMoreThanOneFaultedInverter)
{
    SetStateWithEntry(&drive_state);
    LetTimePass(10);
    app_canRx_INVFL_bError_update(true);
    app_canRx_INVFR_bError_update(true);
    app_canRx_INVRL_bError_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(inverter_fault_handling_state);
}

// Retry lockout when error codes given (iterate through all cases)
TEST_F(VCStateMachineTest, InverterFaultLockout)
{
    SetStateWithEntry(&inverter_fault_handling_state);
    app_canTx_VC_Warning_FrontLeftInverterFault_set(true);
    ASSERT_STATE_EQ(inverter_fault_handling_state);
}

// State changing to HV_init when fault is cleared
TEST_F(VCStateMachineTest, InverterRetryRecovered)
{
    SetStateWithEntry(&drive_state);
    app_canRx_INVFL_bError_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(inverter_fault_handling_state);

    // Recover the fault
    app_canRx_INVFL_bError_update(false);

    // Re-enter HV_INIT path: set all systemReady
    app_canRx_INVFL_bSystemReady_update(true);
    app_canRx_INVFR_bSystemReady_update(true);
    app_canRx_INVRL_bSystemReady_update(true);
    app_canRx_INVRR_bSystemReady_update(true);
    hvInit_state.run_on_tick_100Hz();

    // DC_ON quits
    app_canRx_INVFL_bQuitDcOn_update(true);
    app_canRx_INVFR_bQuitDcOn_update(true);
    app_canRx_INVRL_bQuitDcOn_update(true);
    app_canRx_INVRR_bQuitDcOn_update(true);
    hvInit_state.run_on_tick_100Hz();
    hvInit_state.run_on_tick_100Hz();

    // INVERTER_ON quits
    app_canRx_INVFL_bQuitInverterOn_update(true);
    app_canRx_INVFR_bQuitInverterOn_update(true);
    app_canRx_INVRL_bQuitInverterOn_update(true);
    app_canRx_INVRR_bQuitInverterOn_update(true);
    hvInit_state.run_on_tick_100Hz();

    // Qualify DRIVE (rising edge + brake)
    app_canRx_FSM_BrakeActuated_update(true);
    app_canRx_CRIT_StartSwitch_update(SWITCH_OFF);
    (void)app_startSwitch_hasRisingEdge();
    app_canRx_CRIT_StartSwitch_update(SWITCH_ON);
    (void)app_startSwitch_hasRisingEdge();

    LetTimePass(20);
    ASSERT_STATE_EQ(drive_state);
    LetTimePass(10);
    ASSERT_FALSE(app_canAlerts_VC_Info_InverterRetry_get());
}

// Returning to Retry state when fault has not recovered yet
TEST_F(VCStateMachineTest, InverterRetryNotRecovered)
{
    SetStateWithEntry(&drive_state);
    app_canRx_INVFL_bError_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(inverter_fault_handling_state);
    LetTimePass(20);
    ASSERT_STATE_EQ(inverter_fault_handling_state);
}

/* ------------------------- PCM ON STATE ------------------------------- */
TEST_F(VCStateMachineTest, GoodVoltageTransitionsToHvInit)
{
    SetStateWithEntry(&pcmOn_state);
    app_canTx_VC_ChannelOneVoltage_set(20.0f);
    LetTimePass(20);
    ASSERT_EQ(app_canTx_VC_PcmRetryCount_get(), 0);
    ASSERT_STATE_EQ(hvInit_state);
}

// TODO: PCM tests!
// TEST_F(VCStateMachineTest, UnderVoltageRetryThenFault)
// {
//     // Override voltage read function to return below threshold
//     SetStateWithEntry(&pcmOn_state);
//     app_canTx_VC_ChannelOneVoltage_set(16.0f);
//     ASSERT_EQ(app_canTx_VC_PcmRetryCount_get(), 0);
//     LetTimePass(20); // PCM is not turned on in entry so wait 2 ticks then check
//     EXPECT_TRUE(io_pcm_enabled());

//     LetTimePass(100);
//     LetTimePass(10);
//     EXPECT_FALSE(io_pcm_enabled());
//     ASSERT_EQ(app_canTx_VC_PcmRetryCount_get(), 1);
//     LetTimePass(100);
//     LetTimePass(10);
//     EXPECT_TRUE(io_pcm_enabled()); // retry toggle complete
//     ASSERT_STATE_EQ(pcmOn_state);

//     LetTimePass(100);
//     LetTimePass(10);
//     EXPECT_FALSE(io_pcm_enabled());
//     ASSERT_EQ(app_canTx_VC_PcmRetryCount_get(), 2);
//     LetTimePass(100);
//     LetTimePass(10);
//     EXPECT_TRUE(io_pcm_enabled()); // retry toggle complete
//     ASSERT_STATE_EQ(pcmOn_state);

//     LetTimePass(100);
//     LetTimePass(10);
//     EXPECT_FALSE(io_pcm_enabled());
//     ASSERT_EQ(app_canTx_VC_PcmRetryCount_get(), 3);
//     LetTimePass(100);
//     LetTimePass(10);
//     EXPECT_TRUE(io_pcm_enabled()); // retry toggle complete
//     ASSERT_STATE_EQ(pcmOn_state);

//     LetTimePass(100);
//     LetTimePass(10);
//     EXPECT_FALSE(io_pcm_enabled());
//     ASSERT_EQ(app_canTx_VC_PcmRetryCount_get(), 4);
//     LetTimePass(100);
//     LetTimePass(10);
//     EXPECT_TRUE(io_pcm_enabled()); // retry toggle complete
//     ASSERT_STATE_EQ(pcmOn_state);

//     LetTimePass(100);
//     LetTimePass(10);
//     EXPECT_FALSE(io_pcm_enabled());
//     ASSERT_EQ(app_canTx_VC_PcmRetryCount_get(), 5);
//     LetTimePass(100);
//     LetTimePass(10);

//     EXPECT_TRUE(app_canAlerts_VC_Info_PcmUnderVoltage_get());
//     ASSERT_STATE_EQ(hvInit_state);
// }

// TODO: Test PCM!
// TEST_F(VCStateMachineTest, UnderVoltageRetryThenRecover)
// {
//     // Override voltage read function to return below threshold
//     SetStateWithEntry(&pcmOn_state);
//     app_canTx_VC_ChannelOneVoltage_set(16.0f);
//     ASSERT_EQ(app_canTx_VC_PcmRetryCount_get(), 0);
//     LetTimePass(20); // PCM is not turned on in entry so wait 2 ticks then check
//     EXPECT_TRUE(io_pcm_enabled());

//     LetTimePass(100);
//     LetTimePass(10);
//     EXPECT_FALSE(io_pcm_enabled());
//     ASSERT_EQ(app_canTx_VC_PcmRetryCount_get(), 1);
//     LetTimePass(100);
//     LetTimePass(10);
//     EXPECT_TRUE(io_pcm_enabled()); // retry toggle complete
//     ASSERT_STATE_EQ(pcmOn_state);

//     LetTimePass(100);
//     LetTimePass(10);
//     EXPECT_FALSE(io_pcm_enabled());
//     ASSERT_EQ(app_canTx_VC_PcmRetryCount_get(), 2);
//     LetTimePass(100);
//     LetTimePass(10);
//     EXPECT_TRUE(io_pcm_enabled()); // retry toggle complete
//     ASSERT_STATE_EQ(pcmOn_state);

//     LetTimePass(100);
//     LetTimePass(10);
//     EXPECT_FALSE(io_pcm_enabled());
//     ASSERT_EQ(app_canTx_VC_PcmRetryCount_get(), 3);
//     LetTimePass(100);
//     LetTimePass(10);
//     EXPECT_TRUE(io_pcm_enabled()); // retry toggle complete
//     ASSERT_STATE_EQ(pcmOn_state);
//     app_canTx_VC_ChannelOneVoltage_set(18.0f);

//     LetTimePass(10); // first tick
//     LetTimePass(10); // debounce tick
//     EXPECT_FALSE(app_canAlerts_VC_Info_PcmUnderVoltage_get());
//     ASSERT_STATE_EQ(hvInit_state);
// }

/******************************* LATCHED FAULT CHECKING ***********************/

TEST_F(VCStateMachineTest, initDoesNotExitUnitlLatchClosed)
{
    SetStateWithEntry(&init_state);
    LetTimePass(10);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_OPEN); // somewhere in the middle contactors open
    LetTimePass(1010);
    ASSERT_STATE_EQ(init_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    ASSERT_STATE_EQ(inverterOn_state);
}

TEST_F(VCStateMachineTest, inverterOnStateLatchedFault)
{
    SetStateWithEntry(&inverterOn_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    app_canRx_INVFL_bSystemReady_update(true);
    app_canRx_INVFL_bSystemReady_update(true);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_OPEN); // somewhere in the middle contactors open
    app_canRx_INVFL_bSystemReady_update(true);
    app_canRx_INVFL_bSystemReady_update(true);
    LetTimePass(1020);
    EXPECT_FALSE(app_canRx_BMS_IrNegative_get());
    ASSERT_STATE_EQ(init_state);
}

TEST_F(VCStateMachineTest, bmsOnLatchedFault)
{
    SetStateWithEntry(&bmsOn_state);
    app_canRx_BMS_State_update(BMS_DRIVE_STATE);
    LetTimePass(10);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_OPEN);
    LetTimePass(1010);
    ASSERT_STATE_EQ(init_state);
    LetTimePass(100);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    LetTimePass(1010);
    ASSERT_STATE_EQ(inverterOn_state);
}

TEST_F(VCStateMachineTest, pcmOnStateLatchedFault)
{
    SetStateWithEntry(&pcmOn_state);
    LetTimePass(10);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    app_canTx_VC_ChannelOneVoltage_set(28.0f);
    LetTimePass(10);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_OPEN);
    LetTimePass(1010);
    ASSERT_STATE_EQ(init_state);
}
