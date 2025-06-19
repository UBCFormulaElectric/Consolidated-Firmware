#include <stdint.h>
#include <stdbool.h>
#include "app_leds.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "io_shift_register.h"

#define SHDN_R_BIT 0
#define SHDN_G_BIT 1
#define SHDN_B_BIT 2
#define DAM_R_BIT 3
#define VC_R_BIT 4
#define VC_G_BIT 5
#define VC_B_BIT 6
#define DAM_G_BIT 7

#define CRIT_R_BIT 8
#define CRIT_G_BIT 9
#define CRIT_B_BIT 10
#define IMD_BIT 11
#define FSM_R_BIT 12
#define FSM_G_BIT 13
#define FSM_B_BIT 14
#define DAM_B_BIT 15

#define BMS_R_BIT 16
#define BMS_G_BIT 17
#define BMS_B_BIT 18
#define AMS_BIT 19
#define REGEN_BIT 20
#define RSM_R_BIT 21
#define RSM_G_BIT 22
#define RSM_B_BIT 23

#define BSPD_BIT 24
#define PUSH_R_BIT 25
#define PUSH_G_BIT 26
#define PUSH_B_BIT 27
#define LAUNCH_R_BIT 28
#define LAUNCH_G_BIT 29
#define LAUNCH_B_BIT 30
#define TORQUE_BIT 31

// Helper to build a 3-bit mask for an RGB group
#define RGB_MASK(prefix) ((1u << prefix##_R_BIT) | (1u << prefix##_G_BIT) | (1u << prefix##_B_BIT))

// Macro to set an RGB LED using your *_BIT defines
#define SET_BOARD_RGB(prefix, status)                                                     \
    do                                                                                    \
    {                                                                                     \
        /* clear R,G,B */                                                                 \
        led_value &= ~RGB_MASK(prefix);                                                   \
        /* set new bits */                                                                \
        switch ((status))                                                                 \
        {                                                                                 \
            case BOARD_LED_STATUS_OK:                                                     \
                led_value |= 1u << prefix##_G_BIT; /* green */                            \
                break;                                                                    \
            case BOARD_LED_STATUS_WARNING:                                                \
                led_value |= (1u << prefix##_R_BIT) | (1u << prefix##_G_BIT); /* yellow*/ \
                break;                                                                    \
            case BOARD_LED_STATUS_FAULT:                                                  \
                led_value |= 1u << prefix##_R_BIT; /* red */                              \
                break;                                                                    \
            case BOARD_LED_STATUS_MISSING_HEARTBEAT:                                      \
                led_value |= 1u << prefix##_B_BIT; /* blue */                             \
                break;                                                                    \
            default:                                                                      \
                break;                                                                    \
        }                                                                                 \
    } while (0)

typedef enum
{
    BOARD_LED_STATUS_OK = 0,
    BOARD_LED_STATUS_WARNING,
    BOARD_LED_STATUS_FAULT,
    BOARD_LED_STATUS_NOT_IMPLEMENTED,
    BOARD_LED_STATUS_MISSING_HEARTBEAT
} BoardLEDStatus;

/**
 * @brief Choose the worst status: MISSING_HEARTBEAT > FAULT > WARNING > OK.
 * @param board (VC, BMS, RSM, FSM, DAM, CRIT)
 */
static BoardLEDStatus worstBoardStatus(CanNode board)
{
    bool missing = false;
    switch (board)
    {
        case BMS_NODE:
            missing = app_canAlerts_CRIT_Info_MissingBMSHeartbeat_get();
            break;
        case FSM_NODE:
            missing = app_canAlerts_CRIT_Info_MissingFSMHeartbeat_get();
            break;
        case RSM_NODE:
            missing = app_canAlerts_CRIT_Info_MissingRSMHeartbeat_get();
            break;
        case VC_NODE:
            missing = app_canAlerts_CRIT_Info_MissingVCHeartbeat_get();
            break;
        case DAM_NODE:
            missing = app_canAlerts_CRIT_Info_MissingDAMHeartbeat_get();
            break;
        default:
            missing = false;
            break;
    }

    if (missing)
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
    else
    {
        return BOARD_LED_STATUS_OK;
    }
}

/**
 * @brief Ran every 100Hz updates all LEDs.
 */
void app_leds_update(void)
{
    // Single‑LED flags from CAN
    const bool imd_fault       = !app_canRx_BMS_ImdLatchOk_get();
    const bool bspd_fault      = !app_canRx_BMS_BspdLatchOk_get();
    const bool ams_fault       = !app_canRx_BMS_BmsLatchOk_get();
    const bool push_drive      = (app_canRx_VC_State_get() == VC_DRIVE_STATE);
    const bool regen_on        = app_canRx_VC_RegenEnabled_get();
    const bool torque_on       = app_canRx_VC_TorqueVectoringEnabled_get();
    const bool shdn_ok         = (app_canRx_VC_FirstFaultNode_get() == SHDN_OK);
    const bool open_wire_check = app_canRx_BMS_OpenWireCheckStatus_get();

    // Worst‑status of each RGB board
    BoardLEDStatus rsm_st  = worstBoardStatus(RSM_NODE);
    BoardLEDStatus crit_st = worstBoardStatus(CRIT_NODE);
    BoardLEDStatus fsm_st  = worstBoardStatus(FSM_NODE);
    BoardLEDStatus bms_st  = worstBoardStatus(BMS_NODE);
    BoardLEDStatus vc_st   = worstBoardStatus(VC_NODE);
    BoardLEDStatus dam_st  = worstBoardStatus(DAM_NODE);

    uint32_t led_value = 0;

    // Set each RGB board’s LEDs in place
    SET_BOARD_RGB(RSM, rsm_st);
    SET_BOARD_RGB(CRIT, crit_st);
    SET_BOARD_RGB(FSM, fsm_st);
    SET_BOARD_RGB(BMS, bms_st);
    SET_BOARD_RGB(VC, vc_st);
    SET_BOARD_RGB(DAM, dam_st);

    // Now the single-bit flags
    if (push_drive)
    {
        led_value |= 1u << PUSH_R_BIT;
    }
    if (imd_fault)
    {
        led_value |= 1u << IMD_BIT;
    }
    if (bspd_fault)
    {
        led_value |= 1u << BSPD_BIT;
    }
    if (ams_fault)
    {
        led_value |= 1u << AMS_BIT;
    }
    if (regen_on)
    {
        led_value |= 1u << REGEN_BIT;
    }
    if (torque_on)
    {
        led_value |= 1u << TORQUE_BIT;
    }
    if (shdn_ok)
    {
        led_value |= 1u << SHDN_R_BIT;
    }
    if (open_wire_check)
    {
        // Reset led
        led_value |= 0u << BMS_R_BIT;
        led_value |= 0u << BMS_G_BIT;
        led_value |= 0u << BMS_B_BIT;

        // Set led to magenta
        led_value |= 1u << BMS_R_BIT;
        led_value |= 1u << BMS_B_BIT;
    }

    // Shift out all 4 bytes (LSB first)
    io_shift_register_updateLedRegisters((uint8_t *)&led_value);
}
