#pragma once
#include "app_shdnLoop.hpp"

extern const app::shdn::shdnLoop<4> fsm_shdnLoop;

namespace app::shdnLoop
{
// broadcast the shutdown states
inline void broadcast()
{
    fsm_shdnLoop.broadcast();
}
} // namespace app::shdnLoop
