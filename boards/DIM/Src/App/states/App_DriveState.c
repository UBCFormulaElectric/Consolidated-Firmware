#include "states/App_DriveState.h"
#include "App_SharedMacros.h"
#include "App_SevenSegDisplays.h"
#include "App_SharedExitCode.h"

#define SSEG_HB_NOT_RECEIVED_ERR (888U)

typedef enum
{
    BMS_LED,
    DCM_LED,
    DIM_LED,
    FSM_LED,
    PDM_LED,
    NUM_BOARD_LEDS,
} BoardLeds;

static void App_SetPeriodicCanSignals_BinarySwitch(
    struct DimCanTxInterface *can_tx,
    struct BinarySwitch *     binary_switch,
    void (*can_signal_setter)(struct DimCanTxInterface *, uint8_t value),
    uint8_t on_choice,
    uint8_t off_choice)
{
    if (App_BinarySwitch_IsTurnedOn(binary_switch))
    {
        can_signal_setter(can_tx, on_choice);
    }
    else
    {
        can_signal_setter(can_tx, off_choice);
    }
}

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct DimWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct DimCanTxInterface *can_tx_interface = App_DimWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_DIM_STATE_MACHINE_STATE_DRIVE_CHOICE);
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct DimWorld *      world            = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence = App_DimWorld_GetRgbLedSequence(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct DimWorld *         world                   = App_SharedStateMachine_GetWorld(state_machine);
    struct DimCanTxInterface *can_tx                  = App_DimWorld_GetCanTx(world);
    struct DimCanRxInterface *can_rx                  = App_DimWorld_GetCanRx(world);
    struct SevenSegDisplays * seven_seg_displays      = App_DimWorld_GetSevenSegDisplays(world);
    struct HeartbeatMonitor * heartbeat_monitor       = App_DimWorld_GetHeartbeatMonitor(world);
    struct Led *              imd_led                 = App_DimWorld_GetImdLed(world);
    struct Led *              bspd_led                = App_DimWorld_GetBspdLed(world);
    struct RgbLed *           bms_led                 = App_DimWorld_GetBmsStatusLed(world);
    struct BinarySwitch *     start_switch            = App_DimWorld_GetStartSwitch(world);
    struct BinarySwitch *     traction_control_switch = App_DimWorld_GetTractionControlSwitch(world);
    struct BinarySwitch *     torque_vectoring_switch = App_DimWorld_GetTorqueVectoringSwitch(world);
    struct ErrorTable *       error_table             = App_DimWorld_GetErrorTable(world);
    struct Clock *            clock                   = App_DimWorld_GetClock(world);

    App_CanTx_SetPeriodicSignal_HEARTBEAT(can_tx, true);

    if (!App_CanRx_BMS_OK_STATUSES_GetSignal_IMD_OK(can_rx))
    {
        App_Led_TurnOn(imd_led);
    }
    else
    {
        App_Led_TurnOff(imd_led);
    }

    if (!App_CanRx_BMS_OK_STATUSES_GetSignal_BSPD_OK(can_rx))
    {
        App_Led_TurnOn(bspd_led);
    }
    else
    {
        App_Led_TurnOff(bspd_led);
    }

    if (!App_CanRx_BMS_OK_STATUSES_GetSignal_BMS_OK(can_rx))
    {
        App_SharedRgbLed_TurnRed(bms_led);
    }
    else
    {
        App_SharedRgbLed_TurnOff(bms_led);
    }

    App_SetPeriodicCanSignals_BinarySwitch(
        can_tx, start_switch, App_CanTx_SetPeriodicSignal_START_SWITCH, CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE,
        CANMSGS_DIM_SWITCHES_START_SWITCH_OFF_CHOICE);

    App_SetPeriodicCanSignals_BinarySwitch(
        can_tx, traction_control_switch, App_CanTx_SetPeriodicSignal_TRACTION_CONTROL_SWITCH,
        CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE, CANMSGS_DIM_SWITCHES_START_SWITCH_OFF_CHOICE);

    App_SetPeriodicCanSignals_BinarySwitch(
        can_tx, torque_vectoring_switch, App_CanTx_SetPeriodicSignal_TORQUE_VECTORING_SWITCH,
        CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE, CANMSGS_DIM_SWITCHES_START_SWITCH_OFF_CHOICE);

    struct RgbLed *board_status_leds[5] = {
        [BMS_LED] = App_DimWorld_GetBmsStatusLed(world), [DCM_LED] = App_DimWorld_GetDcmStatusLed(world),
        [DIM_LED] = App_DimWorld_GetDimStatusLed(world), [FSM_LED] = App_DimWorld_GetFsmStatusLed(world),
        [PDM_LED] = App_DimWorld_GetPdmStatusLed(world),
    };

    for (size_t i = 0; i < NUM_BOARD_LEDS; i++)
    {
        if (i == BMS_LED)
        {
            continue;
        }

        struct ErrorBoardList boards_with_critical_errors;
        struct ErrorBoardList boards_with_warnings;

        App_SharedErrorTable_GetBoardsWithCriticalErrors(error_table, &boards_with_critical_errors);

        App_SharedErrorTable_GetBoardsWithWarnings(error_table, &boards_with_warnings);

        struct RgbLed *board_status_led = board_status_leds[i];

        if (App_SharedError_IsBoardInList(&boards_with_critical_errors, i))
        {
            App_SharedRgbLed_TurnRed(board_status_led);
        }
        else if (App_SharedError_IsBoardInList(&boards_with_warnings, i))
        {
            App_SharedRgbLed_TurnBlue(board_status_led);
        }
        else
        {
            App_SharedRgbLed_TurnGreen(board_status_led);
        }
    }

    struct ErrorList all_errors;
    App_SharedErrorTable_GetAllErrors(error_table, &all_errors);

    if (!App_SharedHeartbeatMonitor_Tick(heartbeat_monitor))
    {
        App_SevenSegDisplays_SetUnsignedBase10Value(seven_seg_displays, SSEG_HB_NOT_RECEIVED_ERR);
    }
    else if (all_errors.num_errors == 0)
    {
        App_SevenSegDisplays_SetUnsignedBase10Value(
            seven_seg_displays, (uint32_t)App_CanRx_BMS_STATE_OF_CHARGE_GetSignal_STATE_OF_CHARGE(can_rx));
    }
    else
    {
        // We want to display each error for one second, so we assume that the
        // error set will not change much and just associate each second to
        // a particular error. Errors that are set for less than N seconds,
        // where N is the number of errors set, may not be displayed.
        size_t        error_index = App_SharedClock_GetCurrentTimeInSeconds(clock) % all_errors.num_errors;
        struct Error *error       = all_errors.errors[error_index];

        // To avoid confusion between SoC and error IDs, the 7-segment
        // displays will show the error IDs with an offset of 500. For
        // example, if an error ID is 67, it will show up as 567 on the
        // 7-segment displays.
        const uint32_t error_id             = App_SharedError_GetId(error);
        const uint32_t ERROR_ID_OFFSET      = 500;
        const uint32_t error_id_with_offset = error_id + ERROR_ID_OFFSET;

        App_SevenSegDisplays_SetUnsignedBase10Value(seven_seg_displays, error_id_with_offset);
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
