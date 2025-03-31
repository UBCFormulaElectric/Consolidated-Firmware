#include "app_leds.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "io_shift_register.h"

#define SHIFTREG_LED_RSM_R (1UL << 0)
#define SHIFTREG_LED_RSM_G (1UL << 1)
#define SHIFTREG_LED_RSM_B (1UL << 2)

#define SHIFTREG_LED_CRIT_R (1UL << 3)
#define SHIFTREG_LED_CRIT_G (1UL << 4)
#define SHIFTREG_LED_CRIT_B (1UL << 5)

#define SHIFTREG_LED_FSM_R (1UL << 6)
#define SHIFTREG_LED_FSM_G (1UL << 7)
#define SHIFTREG_LED_FSM_B (1UL << 8)

#define SHIFTREG_LED_LAUNCH_R (1UL << 9)
#define SHIFTREG_LED_LAUNCH_G (1UL << 10)
#define SHIFTREG_LED_LAUNCH_B (1UL << 11)

#define SHIFTREG_LED_PUSH_R (1UL << 12)
#define SHIFTREG_LED_PUSH_G (1UL << 13)
#define SHIFTREG_LED_PUSH_B (1UL << 14)

#define SHIFTREG_LED_VC_R (1UL << 15)
#define SHIFTREG_LED_VC_G (1UL << 16)
#define SHIFTREG_LED_VC_B (1UL << 17)

#define SHIFTREG_LED_BMS_R (1UL << 18)
#define SHIFTREG_LED_BMS_G (1UL << 19)
#define SHIFTREG_LED_BMS_B (1UL << 20)

#define SHIFTREG_LED_DAM_R (1UL << 21)
#define SHIFTREG_LED_DAM_G (1UL << 22)
#define SHIFTREG_LED_DAM_B (1UL << 23)

#define SHIFTREG_LED_SHDN_R (1UL << 24)
#define SHIFTREG_LED_SHDN_G (1UL << 25)
#define SHIFTREG_LED_SHDN_B (1UL << 26)

#define SHIFTREG_LED_IMD (1UL << 27)
#define SHIFTREG_LED_BSPD (1UL << 28)
#define SHIFTREG_LED_AMS (1UL << 29)
#define SHIFTREG_LED_REGEN (1UL << 30)
#define SHIFTREG_LED_TORQUE_VEC (1UL << 31)

static uint32_t boardStatusToRGBBits(BoardLEDStatus status, uint32_t redMask, uint32_t greenMask, uint32_t blueMask)
{
    switch (status)
    {
        case BOARD_LED_STATUS_OK:
            return greenMask;

        case BOARD_LED_STATUS_WARNING:
            // Yello = red + green
            return (redMask | greenMask);

        case BOARD_LED_STATUS_FAULT:
            return redMask;

        case BOARD_LED_STATUS_MISSING_HEARTBEAT:
            return blueMask;

        default:
            return 0;
    }
}

static BoardLEDStatus worstBoardStatus(CanAlertBoard board)
{
    bool is_missing_heartbeat = false;

    switch (board)
    {
        case BMS_ALERT_BOARD:
            is_missing_heartbeat = app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_get();
            break;
        case FSM_ALERT_BOARD:
            is_missing_heartbeat = app_canAlerts_CRIT_Fault_MissingFSMHeartbeat_get();
            break;
        case RSM_ALERT_BOARD:
            is_missing_heartbeat = app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_get();
            break;
        case VC_ALERT_BOARD:
            is_missing_heartbeat = app_canAlerts_CRIT_Fault_MissingVCHeartbeat_get();
            break;
        // TODO: add DAM
        default:
            is_missing_heartbeat = false;
            break;
    }

    if (is_missing_heartbeat)
    {
        return BOARD_LED_STATUS_MISSING_HEARTBEAT;
    }
    else if (app_canAlerts_BoardHasFault(board))
    {
        return BOARD_LED_STATUS_FAULT;
    }
    else if (app_canAlerts_BoardHasWarning(board))
    {
        return BOARD_LED_STATUS_WARNING;
    }

    return BOARD_LED_STATUS_OK;
}

// -----------------------------------------------------------------------------
// 4) app_leds_update
//    The main function that assembles all LED bits and shifts them out.
// -----------------------------------------------------------------------------
void app_leds_update(void)
{
    const bool imd_fault_latched  = app_canRx_BMS_ImdLatchedFault_get();
    const bool bspd_fault_latched = app_canRx_BMS_BspdLatchedFault_get();
    const bool ams_fault_latched  = app_canRx_BMS_BmsLatchedFault_get();
    const bool push_drive         = (app_canRx_VC_State_get() == VC_DRIVE_STATE);
    const bool regen_light_on     = app_canRx_VC_RegenEnabled_get();
    const bool torquevec_light_on = app_canRx_VC_TorqueVectoringEnabled_get();
    const bool is_shdn_ok         = (app_canRx_VC_FirstFaultNode_get() == SHDN_OK);

    // Determine board status.
    const BoardLEDStatus bms_status  = worstBoardStatus(BMS_ALERT_BOARD);
    const BoardLEDStatus fsm_status  = worstBoardStatus(FSM_ALERT_BOARD);
    const BoardLEDStatus rsm_status  = worstBoardStatus(RSM_ALERT_BOARD);
    const BoardLEDStatus vc_status   = worstBoardStatus(VC_ALERT_BOARD);
    const BoardLEDStatus crit_status = worstBoardStatus(CRIT_ALERT_BOARD);
    // TODO: add DAM

    uint32_t ledMask = 0;
    ledMask |= boardStatusToRGBBits(bms_status, SHIFTREG_LED_BMS_R, SHIFTREG_LED_BMS_G, SHIFTREG_LED_BMS_B);
    ledMask |= boardStatusToRGBBits(fsm_status, SHIFTREG_LED_FSM_R, SHIFTREG_LED_FSM_G, SHIFTREG_LED_FSM_B);
    ledMask |= boardStatusToRGBBits(rsm_status, SHIFTREG_LED_RSM_R, SHIFTREG_LED_RSM_G, SHIFTREG_LED_RSM_B);
    ledMask |= boardStatusToRGBBits(vc_status, SHIFTREG_LED_VC_R, SHIFTREG_LED_VC_G, SHIFTREG_LED_VC_B);
    ledMask |= boardStatusToRGBBits(crit_status, SHIFTREG_LED_CRIT_R, SHIFTREG_LED_CRIT_G, SHIFTREG_LED_CRIT_B);

    if (imd_fault_latched)
    {
        ledMask |= SHIFTREG_LED_IMD;
    }
    if (bspd_fault_latched)
    {
        ledMask |= SHIFTREG_LED_BSPD;
    }
    if (ams_fault_latched)
    {
        ledMask |= SHIFTREG_LED_AMS;
    }
    if (push_drive)
    {
        ledMask |= SHIFTREG_LED_PUSH_R;
    }
    if (regen_light_on)
    {
        ledMask |= SHIFTREG_LED_REGEN;
    }
    if (torquevec_light_on)
    {
        ledMask |= SHIFTREG_LED_TORQUE_VEC;
    }
    if (is_shdn_ok)
    {
        // TODO: may need more logic for shutdown LED.
        ledMask |= SHIFTREG_LED_SHDN_R;
    }

    // Convert 32-bit mask into bytes for the shift registers.
    uint8_t shiftBytes[4];
    shiftBytes[0] = (uint8_t)((ledMask >> 0) & 0xFF);
    shiftBytes[1] = (uint8_t)((ledMask >> 8) & 0xFF);
    shiftBytes[2] = (uint8_t)((ledMask >> 16) & 0xFF);
    shiftBytes[3] = (uint8_t)((ledMask >> 24) & 0xFF);

    io_shift_register_updateLedRegisters(shiftBytes, 4);
}
