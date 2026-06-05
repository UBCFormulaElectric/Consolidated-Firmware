#pragma once
#include "io_vcShdn.hpp"

namespace app::shdnLoop
{
// broadcase the shutdown states
inline void broadcast()
{
    tsms_node.broadcast();
    inertia_stop_node.broadcast();
    rear_right_motor_interlock_node.broadcast();
    splitter_box_interlock_node.broadcast();
}
} // namespace app::shdnLoop