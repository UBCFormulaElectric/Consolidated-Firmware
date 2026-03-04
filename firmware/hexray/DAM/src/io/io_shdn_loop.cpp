#include "io_shdnLoopNode.hpp"
#include "hw_gpios.hpp"
#include "app_canTx.hpp"

namespace io::shdn
{
io::shdn::node r_estop(r_shdn_sense_pin, app::can_tx::DAM_REStopOKStatus_set);
io::shdn::node l_estop(l_shdn_sense_pin, app::can_tx::DAM_LEStopOKStatus_set);
} // namespace io::shdn
