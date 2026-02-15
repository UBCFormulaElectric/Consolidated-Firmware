#include "io_shdnLoopNode.hpp"
#include "hw_gpios.hpp"
#include "app_canTx.hpp"

namespace app::shdnLoop
{
// Initialize the shutdown nodes
io::shdn::node tsms_node                       = { tsms_3v3_out, &app::can_tx::VC_TSMSOKStatus_set };
io::shdn::node inertia_stop_node               = { inertia_3v3_out, &app::can_tx::VC_InertiaSwitch_set };
io::shdn::node rear_right_motor_interlock_node = { rr_ilck_3v3_out, &app::can_tx::VC_RearRightMotorInterlock_set };
io::shdn::node splitter_box_interlock_node     = { sb_shdn_3v3_out, &app::can_tx::VC_MSDOrEMeterOKStatus_set };

// broadcase the shutdown states
void broadcast(void)
{
    tsms_node::can_broadcast();
    inertia_stop_node::can_broadcast();
    rear_right_motor_interlock_node::can_broadcast();
    splitter_box_interlock_node::can_broadcast();
}

} // namespace app::shdnLoop