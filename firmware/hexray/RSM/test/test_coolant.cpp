#include <gtest/gtest.h>
#include "test/test_RSMBase.hpp"
#include "test_fakes.hpp"

extern "C"
{
#include "app_canTx.h"
}

class RSMCoolantTest : public RSMBaseTest
{
};

TEST_F(RSMCoolantTest, getFlowRate)
{
    fakes::io::coolant::setFlowRate(15);

    LetTimePass(100);

    EXPECT_EQ(15, app_canTx_RSM_CoolantFlowRate_get());
    EXPECT_FALSE(app_canTx_RSM_Info_FlowRateOutOfRange_get());
}