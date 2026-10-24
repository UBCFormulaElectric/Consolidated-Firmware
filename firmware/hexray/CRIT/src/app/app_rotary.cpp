#include "app_rotary.hpp"
#include "app_canTx.hpp"
#include "screens/app_screens.hpp"

namespace app::rotary
{
void broadcast()
{
    app::can_tx::CRIT_DriveMode_set(app::screens::drive_mode_get());
}
} // namespace app::rotary
