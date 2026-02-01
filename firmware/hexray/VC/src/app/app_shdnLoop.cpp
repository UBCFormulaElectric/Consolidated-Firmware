#include "io_shdnLoopNode.hpp"
extern "C"
{
#include "io_vcShdn.h"
#include "app_canTx.h"
}

// I need to define these in io layer?
hw::Gpio tsms_pin{};
hw::Gpio inertia_switch_pin{};
hw::Gpio rear_right_motor_interlock_pin{};
hw::Gpio splitter_box_interlock_pin{};

namespace vc::app::shdnLoop
{
// Initialize the shutdown nodes
io::shdn::node tsms_node                       = { tsms_pin, &app_canTx_VC_TSMSOKStatus_set };
io::shdn::node inertia_stop_node               = { inertia_switch_pin, &app_canTx_VC_InertiaSwitch_set };
io::shdn::node rear_right_motor_interlock_node = { rear_right_motor_interlock_pin,
                                                   &app_canTx_VC_RearRightMotorInterlock_set };
io::shdn::node splitter_box_interlock_node     = { splitter_box_interlock_pin, &app_canTx_VC_MSDOrEMeterOKStatus_set };

// broadcase the shutdown states
void app_shdnLoop_broadcast(void)
{
    tsms_node::can_broadcast();
    inertia_stop_node::can_broadcast();
    rear_right_motor_interlock_node::can_broadcast();
    splitter_box_interlock_node::can_broadcast();
}

} // namespace vc::app::shdnLoop

// static const BoardShdnNode tsms_node         = { io_vcShdn_TsmsFault_get, &app_canTx_VC_TSMSOKStatus_set };
// static const BoardShdnNode inertia_stop_node = { io_vcShdn_InertiaSwitch_get, &app_canTx_VC_InertiaSwitch_set };
// static const BoardShdnNode rear_right_motor_interlock_node = { io_vcShdn_RearRightMotorInterlock_get,
//                                                                &app_canTx_VC_RearRightMotorInterlock_set };
// static const BoardShdnNode splitter_box_interlock_node     = { io_vcShdn_MSDOrEMeterFault_get,
//                                                                &app_canTx_VC_MSDOrEMeterOKStatus_set };

// app_shdnLoopNode_broadcast(&tsms_node);
// app_shdnLoopNode_broadcast(&inertia_stop_node);
// app_shdnLoopNode_broadcast(&rear_right_motor_interlock_node);
// app_shdnLoopNode_broadcast(&splitter_box_interlock_node);
