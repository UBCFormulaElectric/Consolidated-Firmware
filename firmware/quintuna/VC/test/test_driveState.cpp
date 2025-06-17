#include "app_canUtils.h"
#include "test_VCBase.hpp"

extern "C"
{
#include "app_stateMachine.h"
#include "states/app_states.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_pcm.h"
}

class DriveStateTest : public VCBaseTest
{
};

static void SetStateWithEntry(const State *s)
{
    app_stateMachine_setCurrentState(s);
    s->run_on_entry();
    // s->run_on_tick_100Hz();
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
}

TEST_F(DriveStateTest, SwitchToVanilla)
{
    app_canRx_FSM_PappsMappedPedalPercentage_update(80);

    SetStateWithEntry(&drive_state);
}