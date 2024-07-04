#include "io_driveMode.h"
extern "C"
{
#include "app_canTx.h"
}

namespace app::driveMode
{
void broadcast()
{
    uint16_t driveMode = io::driveMode::readPins();
    app_canTx_CRIT_DriveMode_set(driveMode);
}
} // namespace app::driveMode
