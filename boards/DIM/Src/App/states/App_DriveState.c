#include <string.h>
#include "states/App_DriveState.h"

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
    App_CanTx_DIM_Switches_AuxSwitch_Set(aux_switch ? SWITCH_ON : SWITCH_OFF);

<<<<<<< HEAD
    struct RgbLed *board_status_leds[NUM_BOARD_LEDS] = { [BMS_LED] = App_DimWorld_GetBmsStatusLed(world),
                                                         [DCM_LED] = App_DimWorld_GetDcmStatusLed(world),
                                                         [DIM_LED] = App_DimWorld_GetDimStatusLed(world),
                                                         [FSM_LED] = App_DimWorld_GetFsmStatusLed(world),
                                                         [PDM_LED] = App_DimWorld_GetPdmStatusLed(world) };
=======
    App_SetPeriodicCanSignals_BinarySwitch(
        can_tx, torque_vectoring_switch, App_CanTx_SetPeriodicSignal_TORQUE_VECTORING_SWITCH,
        CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE, CANMSGS_DIM_SWITCHES_START_SWITCH_OFF_CHOICE);

    struct RgbLed *board_status_leds[NUM_BOARD_LEDS] = { [DCM_LED] = App_DimWorld_GetDcmStatusLed(world),
                                                         [DIM_LED] = App_DimWorld_GetDimStatusLed(world),
                                                         [FSM_LED] = App_DimWorld_GetFsmStatusLed(world),
                                                         [PDM_LED] = App_DimWorld_GetPdmStatusLed(world),
                                                         [BMS_LED] = App_DimWorld_GetBmsStatusLed(world) };
>>>>>>> 972a52cc (ELEC-279 Remove BMS Faults and Warnings from Error Table (#854))

    // TODO: Update these LEDs to reflect board status, now that error table is gone
    for (size_t i = 0; i < NUM_BOARD_LEDS; i++)
    {
<<<<<<< HEAD
        struct RgbLed *board_status_led = board_status_leds[i];

        if (i == BMS_LED)
=======
        struct ErrorBoardList boards_with_critical_errors;
        struct ErrorBoardList boards_with_warnings;

        App_SharedErrorTable_GetBoardsWithCriticalErrors(error_table, &boards_with_critical_errors);

        App_SharedErrorTable_GetBoardsWithWarnings(error_table, &boards_with_warnings);

        struct RgbLed *board_status_led = board_status_leds[i];

        // BMS does not use error table, must use state broadcast on CAN to determine fault condition
        if (i == BMS_LED)
        {
            if (App_DriveState_HasBmsFault(can_rx))
            {
                App_SharedRgbLed_TurnRed(board_status_led);
            }

            else if (App_DriveState_HasBmsWarning(can_rx))
            {
                App_SharedRgbLed_TurnBlue(board_status_led);
            }
            else
            {
                App_SharedRgbLed_TurnGreen(board_status_led);
            }
        }
        else if (App_SharedError_IsBoardInList(&boards_with_critical_errors, i))
>>>>>>> 972a52cc (ELEC-279 Remove BMS Faults and Warnings from Error Table (#854))
        {
            if (App_DriveState_HasBmsFault())
            {
                App_SharedRgbLed_TurnRed(board_status_led);
            }
            else if (App_DriveState_HasBmsWarning())
            {
                App_SharedRgbLed_TurnBlue(board_status_led);
            }
            else
            {
                App_SharedRgbLed_TurnGreen(board_status_led);
            }
        }
        else
        {
            App_SharedRgbLed_TurnGreen(board_status_led);
        }
    }

    // TODO: Show something on these LEDs now that error table is gone
    if (!App_SharedHeartbeatMonitor_Tick(heartbeat_monitor))
    {
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

<<<<<<< HEAD
bool App_DriveState_HasBmsFault()
{
    // return App_CanRx_BMS_STATE_MACHINE_GetSignal_STATE(can_rx) == CANMSGS_BMS_STATE_MACHINE_STATE_FAULT_CHOICE;
    return false;
}

bool App_DriveState_HasBmsWarning()
{
    // TODO: Confirm whether or not these should be aperiodic
    // return App_CanRx_BMS_WARNINGS_GetSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ(can_rx) ||
    //        App_CanRx_BMS_WARNINGS_GetSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_KHZ(can_rx) ||
    //        App_CanRx_BMS_WARNINGS_GetSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX(can_rx) ||
    //        App_CanRx_BMS_WARNINGS_GetSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX(can_rx) ||
    //        App_CanRx_BMS_WARNINGS_GetSignal_WATCHDOG_TIMEOUT(can_rx);

    return false;
=======
bool App_DriveState_HasBmsFault(const struct DimCanRxInterface *can_rx)
{
    return App_CanRx_BMS_STATE_MACHINE_GetSignal_STATE(can_rx) == CANMSGS_BMS_STATE_MACHINE_STATE_FAULT_CHOICE;
}

bool App_DriveState_HasBmsWarning(const struct DimCanRxInterface *can_rx)
{
    return App_CanRx_BMS_WARNINGS_GetSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ(can_rx) ||
           App_CanRx_BMS_WARNINGS_GetSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_KHZ(can_rx) ||
           App_CanRx_BMS_WARNINGS_GetSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX(can_rx) ||
           App_CanRx_BMS_WARNINGS_GetSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX(can_rx) ||
           App_CanRx_BMS_WARNINGS_GetSignal_WATCHDOG_TIMEOUT(can_rx);
>>>>>>> 972a52cc (ELEC-279 Remove BMS Faults and Warnings from Error Table (#854))
}
