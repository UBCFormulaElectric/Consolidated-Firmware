#include "app_rsmShdnLoop.hpp"
#include "io_rsmShdn.hpp"

constexpr app::shdn::shdnLoop<1> rsm_shdnLoop{ { { &rl_int_3v3_sens } } };