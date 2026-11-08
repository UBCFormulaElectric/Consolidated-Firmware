#include <app_canTx.hpp>
#include <gtest/gtest.h>
#include "test_fakes.hpp"
#include "test_RSMBase.hpp"
#include "util_errorCodes.hpp"

class RSMPumpTest : public RSMBaseTest
{
};

TEST_F(RSMPumpTest, PumpPercentageTest)
{
    fakes::io::rPump::set_readPercentage(80);

    LetTimePass(100);

    EXPECT_EQ(80, app::can_tx::RSM_RearLeftPumpPercentage_get());
    EXPECT_TRUE(app::can_tx::RSM_RearLeftPumpPercentageValid_get());
}

TEST_F(RSMPumpTest, OutOfRangePercentageTest)
{
    fakes::io::rPump::set_readPercentage(101);

    LetTimePass(100);

    //The RSM_tx.json has 100 as the max of RSM_RearLeftPumpPercentage_get(), so any value above 100 is clamped
    EXPECT_EQ(100, app::can_tx::RSM_RearLeftPumpPercentage_get());
    EXPECT_FALSE(app::can_tx::RSM_RearLeftPumpPercentageValid_get());
}

TEST_F(RSMPumpTest, invalidToValidTest)
{
    fakes::io::rPump::set_readPercentage(101);
    LetTimePass(100);
    fakes::io::rPump::set_readPercentage(0);
    LetTimePass(100);

    EXPECT_EQ(0, app::can_tx::RSM_RearLeftPumpPercentage_get());
    EXPECT_TRUE(app::can_tx::RSM_RearLeftPumpPercentageValid_get());
}