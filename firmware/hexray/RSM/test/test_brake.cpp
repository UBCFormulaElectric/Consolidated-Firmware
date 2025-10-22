#include <gtest/gtest.h>
#include "test_fakes.hpp"
#include "app_brake.hpp"
extern "C"
{
    #include app_canTx.h
}

class RSMBrakeTest : public RSMBaseTest
{

}

TEST_F(RSMBrakeTest, BrakeIsActuated)
{
    fakes::io:brake::setRearPressurePsi(100);
   
    app::brake::broadcast(); 

    LetTimePass(20);
    EXPECT_EQ(100, app_canTx_RSM_rearBrakePressure_get());
    EXPECT_TRUE(app_canTx_RSM_BrakeActuated_get());
    EXPECT_FALSE(app_canTx_RSM_RearBrakePressureOutOfRange_get());   
}
