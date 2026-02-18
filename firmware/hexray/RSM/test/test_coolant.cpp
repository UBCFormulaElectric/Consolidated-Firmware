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
    GTEST_SKIP() << "io_coolant needs to be implemented and set up with fakes";
    
    fakes::io::coolant::setFlowRate(15);

    LetTimePass(100);

    EXPECT_EQ(15, app::can_tx::RSM_CoolantFlowRate_get());
    EXPECT_FALSE(app::can_tx::RSM_Info_FlowRateOutOfRange_get());
}