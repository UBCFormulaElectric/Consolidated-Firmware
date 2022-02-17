#include "states/App_AllStates.h"
#include "App_GateDrive.h"
#include "App_PowerStage.h"
#include "App_Motor.h"
#include "control/App_ControlLoop.h"
#include "App_SharedMacros.h"

static struct ControllerValues * iq_controller;
static struct ControllerValues * id_controller;
static struct ControllerValues * speed_controller;

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    struct InvCanRxInterface *can_rx_interface = App_InvWorld_GetCanRx(world);

    App_CanTx_SetPeriodicSignal_
    App_CanRx_DCM_TORQUE_REQUEST_GetSignal_TORQUE_REQUEST(can_rx_interface);
    App_ControlLoop_GetIqControllerValues(iq_controller);
    App_ControlLoop_GetIdControllerValues(id_controller);
    App_ControlLoop_GetSpeedControllerValues(speed_controller);



}
