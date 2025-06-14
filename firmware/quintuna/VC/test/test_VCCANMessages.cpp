#include "test_VCBase.hpp"

extern "C"
{
#include "states/app_states.h"
#include "app_canTx.h"
}

class VCCANMessagesTest : public VCBaseTest
{
};

// TEST_F(VCCANMessagesTest, states_broadcasted_over_can)
// {
//     for (const auto &state_metadata : state_metadatas)
//     {
//         app_stateMachine_setCurrentState(state_metadata.state);
//         LetTimePass(10);
//         ASSERT_EQ(app_canTx_VC_State_get(), state_metadata.can_state)
//             << "for state: " << app_stateMachine_getCurrentState()->name
//             << ", expected CAN state: " << state_metadata.can_state << ", but got: " << app_canTx_VC_State_get();
//     }
// }
