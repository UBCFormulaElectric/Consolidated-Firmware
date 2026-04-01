#include "io_fsmShdn.hpp"

#include "hw_gpios.hpp"
#include "app_canTx.hpp"

const io::shdn::node cockpit_node{ cockpit_shdn_3v3, app::can_tx::FSM_COCKPITOKStatus_set };
const io::shdn::node bots_node{ bots_3v3, app::can_tx::FSM_BOTSOKStatus_set };
const io::shdn::node fl_shdn_ok_node{ fl_int_3v3, app::can_tx::FSM_FrontLeftILCKInertiaOKStatus_set };
const io::shdn::node fr_shdn_ok_node{ fr_int_3v3, app::can_tx::FSM_FrontRightILCKOKStatus_set };
