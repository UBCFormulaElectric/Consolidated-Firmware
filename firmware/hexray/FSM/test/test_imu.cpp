#include <gtest/gtest.h>
#include "test/test_FSMBase.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"
#include "io_imus.hpp"
#include "fsmMocks.hpp"

class FSMImuTest : public FSMBaseTest
{
};

TEST_F(FSMImuTest, Accel_Gyro_Test)
{
    io::imus::imu_front.set_AccelX(10.0f);
    io::imus::imu_front.set_AccelY(10.0f);
    io::imus::imu_front.set_AccelZ(10.0f);
    io::imus::imu_front.set_GyroRoll(10.0f);
    io::imus::imu_front.set_GyroPitch(10.0f);
    io::imus::imu_front.set_GyroYaw(10.0f);

    LetTimePass(100);

    ASSERT_FLOAT_EQ(10.0f, app::can_tx::FSM_AccelX_get());
    ASSERT_FLOAT_EQ(10.0f, app::can_tx::FSM_AccelY_get());
    ASSERT_FLOAT_EQ(10.0f, app::can_tx::FSM_AccelZ_get());
    ASSERT_FLOAT_EQ(10.0f, app::can_tx::FSM_GyroX_get());
    ASSERT_FLOAT_EQ(10.0f, app::can_tx::FSM_GyroY_get());
    ASSERT_FLOAT_EQ(10.0f, app::can_tx::FSM_GyroZ_get());
}