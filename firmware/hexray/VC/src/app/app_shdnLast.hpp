#include "app_shdnLoopNode.hpp"
extern "C"
{
#include "app_shdnLoop.h"
#include "io_vcShdn.h"
#include "app_canTx.h"
}

static const BoardShdnNode tsms_node         = { io_vcShdn_TsmsFault_get, &app_canTx_VC_TSMSOKStatus_set };
static const BoardShdnNode inertia_stop_node = { io_vcShdn_InertiaSwitch_get, &app_canTx_VC_InertiaSwitch_set };
static const BoardShdnNode rear_right_motor_interlock_node = { io_vcShdn_RearRightMotorInterlock_get,
                                                               &app_canTx_VC_RearRightMotorInterlock_set };
static const BoardShdnNode splitter_box_interlock_node     = { io_vcShdn_MSDOrEMeterFault_get,
                                                               &app_canTx_VC_MSDOrEMeterOKStatus_set };

void app_shdnLoop_broadcast(void)
{
    app_shdnLoopNode_broadcast(&tsms_node);
    app_shdnLoopNode_broadcast(&inertia_stop_node);
    app_shdnLoopNode_broadcast(&rear_right_motor_interlock_node);
    app_shdnLoopNode_broadcast(&splitter_box_interlock_node);
}
