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
    fakes::io::suspension::setRLTravel(10);
    fakes::io::suspension::setRRTravel(10);
    fakes::io::suspension::setRL_OCSC(false);
    fakes::io::suspension::setRR_OCSC(false);

    LetTimePass(100);

    EXPECT_EQ(10, app_canTx_RSM_RearLeftSuspensionTravel_get());
    EXPECT_EQ(10, app_canTx_RSM_RearRightSuspensionTravel_get());
    EXPECT_FALSE(app_canTx_RSM_Info_RearLeftSuspension_OCSC_get());
    EXPECT_FALSE(app_canTx_RSM_Info_RearRightSuspension_OCSC_get());
}