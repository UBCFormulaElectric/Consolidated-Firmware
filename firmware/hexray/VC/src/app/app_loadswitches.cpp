#include "app_loadswitches.hpp"
/*
Setting CAN signals for the efuse status aka turning efuse on or off and setting the
current we want to send over CAN for each efuse channel.
*/

Efuse a{ app::can_tx::VC_FrontInvertersStatus_set };
Efuse b{};

namespace app::loadswitches
{

} // namespace app::loadswitches