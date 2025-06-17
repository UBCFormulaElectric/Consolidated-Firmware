#include "app_canUtils.h"
#include "test_VCBase.hpp"

extern "C"
{
#include "app_stateMachine.h"
#include "states/app_states.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_pcm.h"
}

class DriveHandlingTest : public VCBaseTest
{
};

TEST_F(DriveHandlingTest, DriveModeVanilla)
{
    app_canRx_BMS_AvailablePower_update(80.0f);
}