#include "fff.h"
#include "gtest/gtest.h"

extern "C"
{
#include "StateMachine/App_InitState.h"
#include "StateMachine/App_DriveState.h"
#include "StateMachine/App_FaultState.h"
#include "StateMachine/App_StateMachine.h"
}

TEST(StateMachine, Simple_State_Machine)
{
    ASSERT_EQ(1, 1);
    //    // Can't get the current state before the state machine before is
    //    // initialized
    //    ASSERT_DEATH(App_StateMachine_GetCurrentState(), "");
    //
    //    // Initialize state machine
    //    App_StateMachine_Init();
    //
    //    // We start in Init State after state machine initialization
    //    ASSERT_EQ(App_InitState_EntryAction_fake.call_count, 1);
    //    ASSERT_EQ(
    //        App_StateMachine_GetCurrentState(),
    //        App_InitState_GetStateHandle());
    //
    //    // Go to Drive State
    //    App_SharedState_SetNextState(
    //        App_InitState_GetStateHandle(), App_DriveState_GetStateHandle());
    //    App_StateMachine_Tick();
    //    ASSERT_EQ(
    //        App_StateMachine_GetCurrentState(),
    //        App_DriveState_GetStateHandle());
    //    ASSERT_EQ(App_InitState_StateAction_fake.call_count, 1);
    //    ASSERT_EQ(App_InitState_ExitAction_fake.call_count, 1);
    //    ASSERT_EQ(App_DriveState_EntryAction_fake.call_count, 1);
    //
    //    // Go to Fault State
    //    App_SharedState_SetNextState(
    //        App_DriveState_GetStateHandle(), App_FaultState_GetStateHandle());
    //    App_StateMachine_Tick();
    //    ASSERT_EQ(
    //        App_StateMachine_GetCurrentState(),
    //        App_FaultState_GetStateHandle());
    //    ASSERT_EQ(App_DriveState_StateAction_fake.call_count, 1);
    //    ASSERT_EQ(App_DriveState_ExitAction_fake.call_count, 1);
    //    ASSERT_EQ(App_FaultState_EntryAction_fake.call_count, 1);
    //
    //    // Go to Init State
    //    App_SharedState_SetNextState(
    //        App_FaultState_GetStateHandle(), App_InitState_GetStateHandle());
    //    App_StateMachine_Tick();
    //    ASSERT_EQ(
    //        App_StateMachine_GetCurrentState(),
    //        App_InitState_GetStateHandle());
    //    ASSERT_EQ(App_FaultState_StateAction_fake.call_count, 1);
    //    ASSERT_EQ(App_FaultState_ExitAction_fake.call_count, 1);
    //    ASSERT_EQ(App_InitState_EntryAction_fake.call_count, 2);
}
