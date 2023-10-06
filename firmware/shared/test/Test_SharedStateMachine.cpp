#include <gtest/gtest.h>
#include <fff.h>

#include "Test_Utils.h"
#include "App_TestWorld.h"

extern "C"
{
#include "App_SharedStateMachine.h"
}

FAKE_VOID_FUNC(state_A_entry, struct StateMachine *);
FAKE_VOID_FUNC(state_A_tick_1kHz, struct StateMachine *);
FAKE_VOID_FUNC(state_A_exit, struct StateMachine *);
FAKE_VOID_FUNC(state_B_entry, struct StateMachine *);
FAKE_VOID_FUNC(state_B_tick_1Hz, struct StateMachine *);
FAKE_VOID_FUNC(state_B_tick_1kHz, struct StateMachine *);
FAKE_VOID_FUNC(state_B_exit, struct StateMachine *);
FAKE_VOID_FUNC(state_C_entry, struct StateMachine *);
FAKE_VOID_FUNC(state_C_exit, struct StateMachine *);

static struct State state_A;
static struct State state_B;
static struct State state_C;

class SharedStateMachineTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        world = App_TestWorld_Create();

        state_A.run_on_entry      = state_A_entry;
        state_A.run_on_tick_1Hz   = state_A_tick_1Hz;
        state_A.run_on_tick_100Hz = state_A_tick_1kHz;
        state_A.run_on_exit       = state_A_exit;

        state_B.run_on_entry      = state_B_entry;
        state_B.run_on_tick_1Hz   = state_B_tick_1Hz;
        state_B.run_on_tick_100Hz = state_B_tick_1kHz;
        state_B.run_on_exit       = state_B_exit;

        state_C.run_on_entry      = state_C_entry;
        state_C.run_on_tick_1Hz   = NULL;
        state_C.run_on_tick_100Hz = NULL;
        state_C.run_on_exit       = state_C_exit;

        state_machine = App_SharedStateMachine_Create(world, &state_A);

        RESET_FAKE(state_A_entry);
        RESET_FAKE(state_A_tick_1kHz);
        RESET_FAKE(state_A_exit);
        RESET_FAKE(state_B_entry);
        RESET_FAKE(state_B_tick_1Hz);
        RESET_FAKE(state_B_tick_1kHz);
        RESET_FAKE(state_B_exit);
    }

    void TearDown() override
    {
        TearDownObject(world, App_TestWorld_Destroy);
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
    }

    void SetInitialState(const struct State *const initial_state)
    {
        App_SharedStateMachine_Destroy(state_machine);
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        ASSERT_TRUE(state_machine != NULL);
        ASSERT_EQ(initial_state, App_SharedStateMachine_GetCurrentState(state_machine));
    }

    // We provide our own implementation of the 1hz tick for state_A
    // here so that we can simulate a state transition in a tick
    static void state_A_tick_1Hz(struct StateMachine *state_machine)
    {
        App_SharedStateMachine_SetNextState(state_machine, &state_B);
    }

    struct World *       world;
    struct StateMachine *state_machine;
};

TEST_F(SharedStateMachineTest, check_that_switching_states_in_tick_switches_states_for_all_ticks)
{
    // Check that when we transition states in a tick function at one frequency,
    // that all the other frequencies also transition state
    SetInitialState(&state_A);

    App_SharedStateMachine_Tick1Hz(state_machine);

    App_SharedStateMachine_Tick100Hz(state_machine);

    EXPECT_EQ(state_B_tick_1kHz_fake.call_count, 1);
}

TEST_F(SharedStateMachineTest, check_that_null_tick_functions_dont_deadlock)
{
    // This test was created to reproduce a bug whereby we would take the mutex
    // with a null tick function and then would return without releasing it.
    // Then the next time we called `tick` we would be stuck forever waiting
    // for a lock on the mutex.
    // The failure case for this test is it timing out. As long as it finishes
    // we know we didn't deadlock.

    SetInitialState(&state_C);
    App_SharedStateMachine_Tick1Hz(state_machine);
    App_SharedStateMachine_Tick1Hz(state_machine);
}
