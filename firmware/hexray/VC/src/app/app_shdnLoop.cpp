#include "hw_gpios.hpp"
#include "app_canTx.hpp"
#include "io_vcShdn.hpp"

namespace app::shdnLoop
{
// broadcase the shutdown states
void broadcast(void)
{
    tsms_node::can_broadcast();
    inertia_stop_node::can_broadcast();
    rear_right_motor_interlock_node::can_broadcast();
    splitter_box_interlock_node::can_broadcast();
}
} // namespace app::shdnLoop