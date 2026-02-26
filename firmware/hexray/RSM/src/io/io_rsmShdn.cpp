#include "io_rsmShdn.hpp"
#include <app_canTx.hpp>

const io::shdn::node rl_int_3v3_sens{hw::gpio::rl_int, app::can_tx::RSM_RearLeftMotorInterlock_set};