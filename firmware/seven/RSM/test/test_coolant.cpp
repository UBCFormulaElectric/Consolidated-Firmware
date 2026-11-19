#include <gtest/gtest.h>
#include "test/test_RSMBase.hpp"
#include "test_fakes.hpp"
#include "app_canTx.hpp"

class RSMCoolantTest : public RSMBaseTest
{
};

// TODO: Set up io_coolant and add fake functionality with #ifdefs
TEST_F(RSMCoolantTest, getFlowRate)
{
    fakes::io::coolant::setFlowRate(15);
    LetTimePass(100);

    EXPECT_EQ(15, app::can_tx::RSM_CoolantFlowRate_get());
    EXPECT_FALSE(app::can_tx::RSM_Info_FlowRateOutOfRange_get());
}

TEST_F(RSMCoolantTest, upperOutOfRange)
{
    fakes::io::coolant::setFlowRate(35);
    LetTimePass(100);

    //APP layer clamps to 30
    EXPECT_EQ(30, app::can_tx::RSM_CoolantFlowRate_get());
    EXPECT_TRUE(app::can_tx::RSM_Info_FlowRateOutOfRange_get());
}

TEST_F(RSMCoolantTest, lowerOutOfRange)
{
    fakes::io::coolant::setFlowRate(-5);
    LetTimePass(100);

    //APP layer clamps to 1
    EXPECT_EQ(1, app::can_tx::RSM_CoolantFlowRate_get());
    EXPECT_TRUE(app::can_tx::RSM_Info_FlowRateOutOfRange_get());
}

TEST_F(RSMCoolantTest, out2inRange)
{
    fakes::io::coolant::setFlowRate(-5);
    LetTimePass(100);
    fakes::io::coolant::setFlowRate(15);
    LetTimePass(100);

    EXPECT_EQ(15, app::can_tx::RSM_CoolantFlowRate_get());
    EXPECT_FALSE(app::can_tx::RSM_Info_FlowRateOutOfRange_get());
}


