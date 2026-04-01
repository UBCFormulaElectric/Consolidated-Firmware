#include "app_fsmShdnLoop.hpp"
#include "io_fsmShdn.hpp"

constexpr app::shdn::shdnLoop<4> fsm_shdnLoop{ { { &bots_node, &cockpit_node, &fl_shdn_ok_node, &fr_shdn_ok_node } } };
