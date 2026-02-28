#include <gtest/gtest.h>
#include "test_fakes.hpp"
#include "test_RSMBase.hpp"
#include "app_canTx.hpp"

class RSMBrakeTest : public RSMBaseTest
{
};

TEST_F(RSMBrakeTest, BrakeIsActuated)
{
    fakes::io::brake::setRearPressurePsi(300);

    LetTimePass(100);

    EXPECT_EQ(300, app::can_tx::RSM_RearBrakePressure_get());
    EXPECT_TRUE(app::can_tx::RSM_BrakeActuated_get());
    EXPECT_FALSE(app::can_tx::RSM_Info_RearBrakePressureOutOfRange_get());
}

// setting brakepoints
// pinpointing every step
// use debug
