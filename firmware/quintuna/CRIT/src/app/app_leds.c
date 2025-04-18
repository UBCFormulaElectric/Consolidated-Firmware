#include <stdint.h>
#include "app_leds.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "io_shift_register.h"

/**
 * Enumerates the overall LED statuses a board can have.
 */
typedef enum
{
    BOARD_LED_STATUS_OK = 0,
    BOARD_LED_STATUS_WARNING,
    BOARD_LED_STATUS_FAULT,
    BOARD_LED_STATUS_NOT_IMPLEMENTED,
    BOARD_LED_STATUS_MISSING_HEARTBEAT
} BoardLEDStatus;

typedef struct __attribute__((packed, aligned(1)))
{
    unsigned int r : 1;
    unsigned int g : 1;
    unsigned int b : 1;
} LedRgb_t;
typedef struct
{
    LedRgb_t rsm;           // bits 0-2
    LedRgb_t crit;          // bits 3-5
    LedRgb_t fsm;           // bits 6-8
    LedRgb_t launch;        // bits 9-11
    LedRgb_t push;          // bits 12-14
    LedRgb_t vc;            // bits 15-17
    LedRgb_t bms;           // bits 18-20
    LedRgb_t dam;           // bits 21-23
    LedRgb_t shdn;          // bits 24-26
    uint8_t  imd : 1;       // bit 27
    uint8_t  bspd : 1;      // bit 28
    uint8_t  ams : 1;       // bit 29
    uint8_t  regen : 1;     // bit 30
    uint8_t  torqueVec : 1; // bit 31
} LedRegister_t;

typedef union
{
    LedRegister_t bits;
    uint32_t      value;
} LedReg_t;

static void set_rgb_led(LedRgb_t *led, BoardLEDStatus status)
{
    led->r = 0;
    led->g = 0;
    led->b = 0;
    switch (status)
    {
        case BOARD_LED_STATUS_OK:
            led->g = 1;
            break;
        case BOARD_LED_STATUS_WARNING:
            led->r = 1;
            led->g = 1;
            break;
        case BOARD_LED_STATUS_FAULT:
            led->r = 1;
            break;
        case BOARD_LED_STATUS_MISSING_HEARTBEAT:
            led->b = 1;
            break;
        default:
            break;
    }
}

static BoardLEDStatus worstBoardStatus(CanAlertBoard board)
{
    bool missingHeartbeat = false;

    switch (board)
    {
        case BMS_ALERT_BOARD:
            missingHeartbeat = app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_get();
            break;
        case FSM_ALERT_BOARD:
            missingHeartbeat = app_canAlerts_CRIT_Fault_MissingFSMHeartbeat_get();
            break;
        case RSM_ALERT_BOARD:
            missingHeartbeat = app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_get();
            break;
        case VC_ALERT_BOARD:
            missingHeartbeat = app_canAlerts_CRIT_Fault_MissingVCHeartbeat_get();
            break;
        case DAM_ALERT_BOARD:
            missingHeartbeat = app_canAlerts_CRIT_Fault_MissingDAMHeartbeat_get();
            break;
        default:
            missingHeartbeat = false;
            break;
    }

    if (missingHeartbeat)
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

void app_leds_update(void)
{
    const bool imd_fault_latched  = app_canRx_BMS_ImdLatchedFault_get();
    const bool bspd_fault_latched = app_canRx_BMS_BspdLatchedFault_get();
    const bool ams_fault_latched  = app_canRx_BMS_BmsLatchedFault_get();
    const bool push_drive         = (app_canRx_VC_State_get() == VC_DRIVE_STATE);
    const bool regen_light_on     = app_canRx_VC_RegenEnabled_get();
    const bool torquevec_light_on = app_canRx_VC_TorqueVectoringEnabled_get();
    const bool is_shdn_ok         = (app_canRx_VC_FirstFaultNode_get() == SHDN_OK);

    const BoardLEDStatus rsm_status  = worstBoardStatus(RSM_ALERT_BOARD);
    const BoardLEDStatus crit_status = worstBoardStatus(CRIT_ALERT_BOARD);
    const BoardLEDStatus fsm_status  = worstBoardStatus(FSM_ALERT_BOARD);
    const BoardLEDStatus bms_status  = worstBoardStatus(BMS_ALERT_BOARD);
    const BoardLEDStatus vc_status   = worstBoardStatus(VC_ALERT_BOARD);
    const BoardLEDStatus dam_status  = worstBoardStatus(DAM_ALERT_BOARD);

    LedReg_t leds = { 0 };

    set_rgb_led(&leds.bits.rsm, rsm_status);
    set_rgb_led(&leds.bits.crit, crit_status);
    set_rgb_led(&leds.bits.fsm, fsm_status);
    set_rgb_led(&leds.bits.bms, bms_status);
    set_rgb_led(&leds.bits.vc, vc_status);
    set_rgb_led(&leds.bits.dam, dam_status);

    if (push_drive)
    {
        leds.bits.push.r = 1;
    }
    if (imd_fault_latched)
    {
        leds.bits.imd = 1;
    }
    if (bspd_fault_latched)
    {
        leds.bits.bspd = 1;
    }
    if (ams_fault_latched)
    {
        leds.bits.ams = 1;
    }
    if (regen_light_on)
    {
        leds.bits.regen = 1;
    }
    if (torquevec_light_on)
    {
        leds.bits.torqueVec = 1;
    }
    if (is_shdn_ok)
    {
        leds.bits.shdn.r = 1;
    }

    io_shift_register_updateLedRegisters((uint8_t *)leds.value);
}
