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

    EXPECT_EQ(80, app::can_tx::RSM_RPumpPercentage_get());
}
