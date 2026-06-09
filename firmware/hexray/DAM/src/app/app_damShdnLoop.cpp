#include "app_damShdnLoop.hpp"
#include "io_shdn_loop.hpp"

const app::shdn::shdnLoop<2> dam_shdnLoop{ { { &r_estop, &l_estop } } };
