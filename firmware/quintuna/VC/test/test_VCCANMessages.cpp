#include "test_VCBase.hpp"

extern "C"
{
#include "states/app_states.h"
#include "app_canTx.h"
}

class VCCANMessagesTest : public VCBaseTest
{
};

#define ASSERT_STATE_EQ(expected)                           \
    ASSERT_EQ(app_stateMachine_getCurrentState(), expected) \
        << "Expected state: " << expected->name << ", but got: " << app_stateMachine_getCurrentState()->name

TEST_F(VCCANMessagesTest, states_broadcasted_over_can)
{
    for (const auto &state_metadata : state_metadatas)
    {
        // TODO: Fix PCM ON state.
        if (state_metadata.state == &pcmOn_state)
        {
            continue;
        }

        app_stateMachine_setCurrentState(state_metadata.state);
        app_canRx_BMS_IrNegative_update(
            state_metadata.state == &init_state ? CONTACTOR_STATE_OPEN : CONTACTOR_STATE_CLOSED);
        LetTimePass(10);
        ASSERT_STATE_EQ(state_metadata.state);
        ASSERT_EQ(app_canTx_VC_State_get(), state_metadata.can_state)
            << "for state: " << app_stateMachine_getCurrentState()->name
            << ", expected CAN state: " << state_metadata.can_state << ", but got: " << app_canTx_VC_State_get();
    }
}
