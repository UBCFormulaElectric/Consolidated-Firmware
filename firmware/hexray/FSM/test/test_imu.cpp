#include "fsmMocks.hpp"
#include "test_FSMBase.hpp"
#include "io_imuFake.h"

extern "C"{
    #include "app_canTx.h"
}

// Test NOT COMPLETED:
// LOG_IF_ERROR are not tested here

class ImuModulesTest : public FSMBaseTest{
};

TEST_F(ImuModulesTest, normal_imuConditions) {
    fakes::io::imu::set_LinearAccelerationX(1.0f);
    fakes::io::imu::set_LinearAccelerationY(2.0f);
    fakes::io::imu::set_LinearAccelerationZ(3.0f);
    fakes::io::imu::set_AngularVelocityRoll(0.1f);
    fakes::io::imu::set_AngularVelocityPitch(0.2f);
    fakes::io::imu::set_AngularVelocityYaw(0.3f);

    LetTimePass(100);

    EXPECT_EQ(app_canTx_FSM_LinearAccelerationX_get(), 1.0f);
    EXPECT_EQ(app_canTx_FSM_LinearAccelerationY_get(), 2.0f);  
    EXPECT_EQ(app_canTx_FSM_LinearAccelerationZ_get(), 3.0f);
    EXPECT_EQ(app_canTx_FSM_RollRate_get(), 0.1f);
    EXPECT_EQ(app_canTx_FSM_PitchRate_get(), 0.2f);
    EXPECT_EQ(app_canTx_FSM_YawRate_get(), 0.3f);
}