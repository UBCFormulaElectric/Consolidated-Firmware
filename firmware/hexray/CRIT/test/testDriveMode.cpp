#include "critTestBase.hpp"
#include "app_canTx.hpp"
#include "screens/app_screens.hpp"
#include "app_rotary.hpp"

using namespace app::can_utils;

class CritDriveModeTest : public CRITBaseTest
{
};

TEST_F(CritDriveModeTest, ClockwiseStepGoesToNextMode)
{
    const auto start    = app::screens::drive_mode_get();
    const auto expected = static_cast<DriveMode>(
        (static_cast<uint8_t>(start) + 1) % static_cast<uint8_t>(DriveMode::COUNT));

    app::screens::drive_modes_screen.cw_callback();

    app::rotary::broadcast();

    EXPECT_EQ(app::can_tx::CRIT_DriveMode_get(), expected);
}


TEST_F(CritDriveModeTest, CounterClockwiseStepFromFirstModeWrapsToLastMode)
{
    while (app::screens::drive_mode_get() != DriveMode::VANILLA)
    {
        app::screens::drive_modes_screen.cw_callback();
    }
    const auto expected = static_cast<DriveMode>(static_cast<uint8_t>(DriveMode::COUNT) - 1);

    app::screens::drive_modes_screen.ccw_callback();

    app::rotary::broadcast();

    EXPECT_EQ(app::can_tx::CRIT_DriveMode_get(), expected);
}
