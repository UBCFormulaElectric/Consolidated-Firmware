#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "io_rotary.hpp"

using namespace app::can_utils;

/**
 * Note:
 *
 * If you want additional drive modes, add them in shared_enum.json
 * and handle them in the VC
 */
namespace app::driveModes
{
static int drive_mode_num = 0;

static void cc_callback()
{
    drive_mode_num = (drive_mode_num + 1) % static_cast<int>(DriveMode::COUNT);
}

static void ccw_callback()
{
    drive_mode_num = (drive_mode_num - 1) % static_cast<int>(DriveMode::COUNT);
}

void init()
{
    io::rotary::setClockwiseCallback(cc_callback);
    io::rotary::setCounterClockwiseCallback(ccw_callback);
    io::rotary::init();
}

void broadcast()
{
    app::can_tx::CRIT_DriveMode_set(static_cast<DriveMode>(drive_mode_num));
}
} // namespace app::driveModesP