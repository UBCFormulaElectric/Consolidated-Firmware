#include <gtest/gtest.h>
#include "test/test_RSMBase.hpp"
#include "test_fakes.hpp"

extern "C"
{
    #include "app_canTx.h"
}

class RSMSuspensionTest : public RSMBaseTest
{};

TEST_F(RSMSuspensionTest, SuspensionTest)
{
    fakes::io::suspension::setRLTravel(15);
    fakes::io::suspension::setRRTravel(55);
    fakes::io::suspension::setRL_OCSC(true);
    fakes::io::suspension::setRR_OCSC(true);

    LetTimePass(1000);

    EXPECT_EQ(10, app_canTx_RSM_RearLeftSuspensionTravel_get());
    EXPECT_EQ(10, app_canTx_RSM_RearRightSuspensionTravel_get());
    EXPECT_TRUE(app_canTx_RSM_Info_RearLeftSuspension_OCSC_get());
    EXPECT_TRUE(app_canTx_RSM_Info_RearRightSuspension_OCSC_get());
}