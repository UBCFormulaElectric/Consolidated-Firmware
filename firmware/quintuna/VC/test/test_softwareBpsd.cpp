#include <gtest/gtest.h>

#include "fake_io_time.hpp"
#include "fake_io_canRx.hpp"

extern "C"
{
#include "app_faultCheck.h"
#include "app_timer.h"
#include "app_canAlerts.h"
#include "app_canTx.h"
#include "app_canRx.h"
}

class SoftwareBspdTest : public testing::Test
{
};

TEST_F(SoftwareBspdTest, test_bspd_active)
{
    app_faultCheck_init();

    app_canRx_FSM_BrakeActuated_update(1);

    ASSERT_TRUE(app_faultCheck_checkSoftwareBspd(0.3f, 0.3f));
}
