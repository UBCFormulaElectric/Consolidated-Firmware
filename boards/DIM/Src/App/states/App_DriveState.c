#include <string.h>
#include "states/App_DriveState.h"
#include "App_CanAlerts.h"

#define SSEG_HB_NOT_RECEIVED_ERR (888U)

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    App_CanTx_DIM_Vitals_State_Set(DIM_STATE_DRIVE);
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct DimWorld *      world            = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence = App_DimWorld_GetRgbLedSequence(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct DimWorld *        world              = App_SharedStateMachine_GetWorld(state_machine);
    struct SevenSegDisplays *seven_seg_displays = App_DimWorld_GetSevenSegDisplays(world);
    struct HeartbeatMonitor *heartbeat_monitor  = App_DimWorld_GetHeartbeatMonitor(world);
    struct Led *             imd_led            = App_DimWorld_GetImdLed(world);
    struct Led *             bspd_led           = App_DimWorld_GetBspdLed(world);
    struct RgbLed *          bms_led            = App_DimWorld_GetBmsStatusLed(world);
    struct BinarySwitch *    start_switch       = App_DimWorld_GetStartSwitch(world);
    struct BinarySwitch *    aux_switch         = App_DimWorld_GetAuxSwitch(world);

    App_CanTx_DIM_Vitals_Heartbeat_Set(true);

    if (!App_CanRx_BMS_OkStatuses_ImdOk_Get())
    {
        App_Led_TurnOn(imd_led);
    }
    else
    {
        App_Led_TurnOff(imd_led);
    }

    if (!App_CanRx_BMS_OkStatuses_BspdOk_Get())
    {
        App_Led_TurnOn(bspd_led);
    }
    else
    {
        App_Led_TurnOff(bspd_led);
    }

    if (!App_CanRx_BMS_OkStatuses_BmsOk_Get())
    {
        App_SharedRgbLed_TurnRed(bms_led);
    }
    else
    {
        App_SharedRgbLed_TurnOff(bms_led);
    }

    const bool start_switch_on = App_BinarySwitch_IsTurnedOn(start_switch);
    const bool aux_switch_on   = App_BinarySwitch_IsTurnedOn(aux_switch);

    App_CanTx_DIM_Switches_StartSwitch_Set(start_switch_on ? SWITCH_ON : SWITCH_OFF);
    App_CanTx_DIM_Switches_AuxSwitch_Set(aux_switch_on ? SWITCH_ON : SWITCH_OFF);

    struct RgbLed *board_status_leds[NUM_BOARD_LEDS] = { [BMS_LED] = App_DimWorld_GetBmsStatusLed(world),
                                                         [DCM_LED] = App_DimWorld_GetDcmStatusLed(world),
                                                         [DIM_LED] = App_DimWorld_GetDimStatusLed(world),
                                                         [FSM_LED] = App_DimWorld_GetFsmStatusLed(world),
                                                         [PDM_LED] = App_DimWorld_GetPdmStatusLed(world) };

    CanAlertBoard alert_board_ids[NUM_BOARD_LEDS] = {
        [BMS_LED] = BMS_ALERT_BOARD, [DCM_LED] = DCM_ALERT_BOARD, [DIM_LED] = DIM_ALERT_BOARD,
        [FSM_LED] = FSM_ALERT_BOARD, [PDM_LED] = PDM_ALERT_BOARD,
    };

    for (size_t i = 0; i < NUM_BOARD_LEDS; i++)
    {
        struct RgbLed *board_status_led = board_status_leds[i];

        if (App_CanAlerts_BoardHasFault(alert_board_ids[i]))
        {
            App_SharedRgbLed_TurnRed(board_status_led);
        }
        else if (App_CanAlerts_BoardHasWarning(alert_board_ids[i]))
        {
            App_SharedRgbLed_TurnBlue(board_status_led);
        }
        else
        {
            App_SharedRgbLed_TurnGreen(board_status_led);
        }
    }

    const bool missing_hb = !App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
    // TODO: Show something on these LEDs now that error table is gone
    if (missing_hb)
    {
        App_CanAlerts_SetFault(DIM_FAULT_MISSING_HEARTBEAT, missing_hb);
        App_SevenSegDisplays_SetUnsignedBase10Value(seven_seg_displays, SSEG_HB_NOT_RECEIVED_ERR);
    }
    else
    {
        App_SevenSegDisplays_SetUnsignedBase10Value(seven_seg_displays, 0U);
    }
}

static void DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetDriveState(void)
{
    static struct State drive_state = {
        .name              = "DRIVE",
        .run_on_entry      = DriveStateRunOnEntry,
        .run_on_tick_1Hz   = DriveStateRunOnTick1Hz,
        .run_on_tick_100Hz = DriveStateRunOnTick100Hz,
        .run_on_exit       = DriveStateRunOnExit,
    };

    return &drive_state;
}
