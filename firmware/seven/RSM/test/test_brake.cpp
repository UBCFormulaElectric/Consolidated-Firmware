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

TEST_F(RSMBrakeTest, BrakeUnactuated)
{
    fakes::io::brake::setRearPressurePsi(5);
    LetTimePass(100);

    EXPECT_EQ(5, app::can_tx::RSM_RearBrakePressure_get());
    EXPECT_FALSE(app::can_tx::RSM_BrakeActuated_get());
    EXPECT_FALSE(app::can_tx::RSM_Info_RearBrakePressureOutOfRange_get());
}

TEST_F(RSMBrakeTest, actuatedToUnactuated)
{
    fakes::io::brake::setRearPressurePsi(300);
    LetTimePass(100);
    
    fakes::io::brake::setRearPressurePsi(10);
    LetTimePass(100);

    EXPECT_EQ(10, app::can_tx::RSM_RearBrakePressure_get());
    EXPECT_FALSE(app::can_tx::RSM_BrakeActuated_get());
    EXPECT_FALSE(app::can_tx::RSM_Info_RearBrakePressureOutOfRange_get());
}

TEST_F(RSMBrakeTest, upperActuationOutOfRange)
{
    fakes::io::brake::setRearPressurePsi(1500);
    LetTimePass(100);

    EXPECT_EQ(1500, app::can_tx::RSM_RearBrakePressure_get());
    EXPECT_TRUE(app::can_tx::RSM_BrakeActuated_get());
    EXPECT_TRUE(app::can_tx::RSM_Info_RearBrakePressureOutOfRange_get());
}

TEST_F(RSMBrakeTest, upperOutOfRange)
{
    fakes::io::brake::setRearPressurePsi(3000);
    LetTimePass(100);

    //CAN should clamp to 2500
    EXPECT_EQ(2500, app::can_tx::RSM_RearBrakePressure_get());
    EXPECT_TRUE(app::can_tx::RSM_BrakeActuated_get());
    EXPECT_TRUE(app::can_tx::RSM_Info_RearBrakePressureOutOfRange_get());
}


TEST_F(RSMBrakeTest, lowerOutOfRange)
{
    fakes::io::brake::setRearPressurePsi(-1);
    LetTimePass(100);

    //CAN should clamp to 0
    EXPECT_EQ(0, app::can_tx::RSM_RearBrakePressure_get());
    EXPECT_FALSE(app::can_tx::RSM_BrakeActuated_get());
    EXPECT_TRUE(app::can_tx::RSM_Info_RearBrakePressureOutOfRange_get());
}

// setting brakepoints
// pinpointing every step
// use debug
