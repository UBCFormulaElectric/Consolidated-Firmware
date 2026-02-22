#include <gtest/gtest.h>
#include "test/test_RSMBase.hpp"
#include "test_fakes.hpp"
#include "app_canTx.hpp"

class RSMSuspensionTest : public RSMBaseTest
{
};

TEST_F(RSMSuspensionTest, SuspensionTest)
{
    fakes::io::suspension::setRLTravel(5);
    fakes::io::suspension::setRRTravel(5);
    fakes::io::suspension::setRL_OCSC(true);
    fakes::io::suspension::setRR_OCSC(true);

    LetTimePass(1000);

    EXPECT_EQ(5, app::can_tx::RSM_RearLeftSuspensionTravel_get());
    EXPECT_EQ(5, app::can_tx::RSM_RearRightSuspensionTravel_get());
    EXPECT_TRUE(app::can_tx::RSM_Info_RearLeftSuspension_OCSC_get());
    EXPECT_TRUE(app::can_tx::RSM_Info_RearRightSuspension_OCSC_get());
}