#include "test_VCBase.hpp"

extern "C"
{
#include "app_pumpControl.h"
#include "io_loadswitches.h"
#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_canRx.h"
}

static Efuse *pumpEfuse()
{
    return const_cast<Efuse *>(efuse_channels[EFUSE_CHANNEL_RL_PUMP]);
}

class VCPumpControlTest : public VCBaseTest
{
};

static void SetStateWithEntry(const State *s)
{
    app_stateMachine_setCurrentState(s);
    s->run_on_entry();
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
}

TEST_F(VCPumpControlTest, RampUpFirstStep)
{
    SetStateWithEntry(&hvInit_state);
    rl_pump_loadswitch.pgood = true;
    pumpEfuse()->enabled     = true;

    LetTimePass(50u);

    EXPECT_EQ(app_canTx_VC_PumpRampUpSetPoint_get(), 25u);
    EXPECT_TRUE(app_canTx_VC_RsmTurnOnPump_get());
    EXPECT_FALSE(app_canTx_VC_PumpFailure_get());
}

TEST_F(VCPumpControlTest, RampUpTo100AndHold)
{
    // enable both inputs to the ramp logic
    rl_pump_loadswitch.pgood = true;
    pumpEfuse()->enabled     = true;

    // 20 cycles of 10ms each => 200ms => 0.5f * 200 = 100%
    LetTimePass(350u);
    EXPECT_EQ(app_canTx_VC_PumpRampUpSetPoint_get(), 100u);

    // RSM output remains true
    EXPECT_TRUE(app_canTx_VC_RsmTurnOnPump_get());
}

TEST_F(VCPumpControlTest, StopWhenPowerGoodFalse)
{
    // first tick: pumps_ok=false, enabled=true → stopFlow()
    rl_pump_loadswitch.pgood = false;
    pumpEfuse()->enabled     = true;
    LetTimePass(10u);

    EXPECT_TRUE(app_canTx_VC_PumpFailure_get());
    EXPECT_FALSE(app_canTx_VC_RsmTurnOnPump_get());

    // next tick: restore pumps_ok=true → fresh ramp to 5%
    rl_pump_loadswitch.pgood = true;
    // PumpFailure cleared manually so we can observe ramp
    app_canTx_VC_PumpFailure_set(false);

    LetTimePass(10u);
    EXPECT_EQ(app_canTx_VC_PumpRampUpSetPoint_get(), 5u);
    EXPECT_TRUE(app_canTx_VC_RsmTurnOnPump_get());
}

TEST_F(VCPumpControlTest, StopWhenChannelDisabled)
{
    GTEST_SKIP() << "This test does not make any sense because whether or not the pump is enabled/disabled here, the "
                    "power manager will override that setting";
    // pumps_ok=true, enabled=false → stopFlow()
    rl_pump_loadswitch.pgood = true;
    pumpEfuse()->enabled     = false;
    LetTimePass(100u);

    EXPECT_TRUE(app_canTx_VC_PumpFailure_get());
    EXPECT_FALSE(app_canTx_VC_RsmTurnOnPump_get());
}
