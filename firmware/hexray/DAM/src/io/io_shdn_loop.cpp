#include "io_shdn_loop.hpp"
#include "hw_gpios.hpp"
#include "app_canTx.hpp"

const io::shdn::node r_estop(r_shdn_sense_pin, app::can_tx::DAM_REStopOKStatus_set);
const io::shdn::node l_estop(l_shdn_sense_pin, app::can_tx::DAM_LEStopOKStatus_set);
