#include "states/App_AllStates.h"

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    struct RgbLedSequence *   rgb_led_sequence =
        App_BmsWorld_GetRgbLedSequence(world);
    struct Charger *charger = App_BmsWorld_GetCharger(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);

    bool charger_is_connected = App_Charger_IsConnected(charger);
    App_CanTx_SetPeriodicSignal_IS_CONNECTED(can_tx, charger_is_connected);
}
