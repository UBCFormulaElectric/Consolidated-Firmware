// app_leds.c

#include <stdint.h>
#include <stdbool.h>
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

/**
 * 32‑bit register laid out exactly like the four
 * cascaded TPIC6C596 bytes (LSB first on the wire).
 */
typedef union
{
    uint32_t value;
    struct __attribute__((packed, aligned(1)))
    {
        /* byte 0 = chip #1 */
        unsigned shdn_r : 1; // bit  0
        unsigned shdn_g : 1; // bit  1
        unsigned shdn_b : 1; // bit  2
        unsigned dam_r : 1;  // bit  3
        unsigned vc_r : 1;   // bit  4
        unsigned vc_g : 1;   // bit  5
        unsigned vc_b : 1;   // bit  6
        unsigned dam_g : 1;  // bit  7

        /* byte 1 = chip #2 */
        unsigned crit_r : 1; // bit  8
        unsigned crit_g : 1; // bit  9
        unsigned crit_b : 1; // bit 10
        unsigned imd : 1;    // bit 11
        unsigned fsm_r : 1;  // bit 12
        unsigned fsm_g : 1;  // bit 13
        unsigned fsm_b : 1;  // bit 14
        unsigned dam_b : 1;  // bit 15

        /* byte 2 = chip #3 */
        unsigned bms_r : 1; // bit 16
        unsigned bms_g : 1; // bit 17
        unsigned bms_b : 1; // bit 18
        unsigned ams : 1;   // bit 19
        unsigned regen : 1; // bit 20
        unsigned rsm_r : 1; // bit 21
        unsigned rsm_g : 1; // bit 22
        unsigned rsm_b : 1; // bit 23

        /* byte 3 = chip #4 */
        unsigned bspd : 1;     // bit 24
        unsigned push_r : 1;   // bit 25
        unsigned push_g : 1;   // bit 26
        unsigned push_b : 1;   // bit 27
        unsigned launch_r : 1; // bit 28
        unsigned launch_g : 1; // bit 29
        unsigned launch_b : 1; // bit 30
        unsigned torque : 1;   // bit 31
    } bits;
} LedReg_t;

/**
 * Choose the worst status: MISSING_HEARTBEAT > FAULT > WARNING > OK
 */
static BoardLEDStatus worstBoardStatus(CanAlertBoard board)
{
    bool missing = false;
    switch (board)
    {
        case BMS_ALERT_BOARD:
            missing = app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_get();
            break;
        case FSM_ALERT_BOARD:
            missing = app_canAlerts_CRIT_Fault_MissingFSMHeartbeat_get();
            break;
        case RSM_ALERT_BOARD:
            missing = app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_get();
            break;
        case VC_ALERT_BOARD:
            missing = app_canAlerts_CRIT_Fault_MissingVCHeartbeat_get();
            break;
        case DAM_ALERT_BOARD:
            missing = app_canAlerts_CRIT_Fault_MissingDAMHeartbeat_get();
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
 * Macro to clear and then set an (r,g,b) triplet for any one of
 * our six “RGB” boards based on a BoardLEDStatus.
 */
#define SET_BOARD_RGB(name, status)                  \
    do                                               \
    {                                                \
        leds.bits.name##_r = 0;                      \
        leds.bits.name##_g = 0;                      \
        leds.bits.name##_b = 0;                      \
        switch (status)                              \
        {                                            \
            case BOARD_LED_STATUS_OK:                \
                leds.bits.name##_g = 1;              \
                break;                               \
            case BOARD_LED_STATUS_WARNING:           \
                leds.bits.name##_r = 1;              \
                leds.bits.name##_g = 1;              \
                break;                               \
            case BOARD_LED_STATUS_FAULT:             \
                leds.bits.name##_r = 1;              \
                break;                               \
            case BOARD_LED_STATUS_MISSING_HEARTBEAT: \
                leds.bits.name##_b = 1;              \
                break;                               \
            default:                                 \
                break;                               \
        }                                            \
    } while (0)

void led_self_test(void)
{
    io_shift_register_led_init();

    for (uint32_t bit = 0; bit < 32; ++bit)
    {
        LedReg_t r = { .value = (1u << bit) };
        io_shift_register_updateLedRegisters((uint8_t *)&r.value);
        HAL_Delay(1000);
    }

    io_shift_register_updateLedRegisters((uint8_t *)&(uint32_t){ 0 });
}

void app_leds_update(void)
{
    /* Single‑LED flags from CAN */
    const bool imd_fault  = app_canRx_BMS_ImdLatchedFault_get();
    const bool bspd_fault = app_canRx_BMS_BspdLatchedFault_get();
    const bool ams_fault  = app_canRx_BMS_BmsLatchedFault_get();
    const bool push_drive = (app_canRx_VC_State_get() == VC_DRIVE_STATE);
    const bool regen_on   = app_canRx_VC_RegenEnabled_get();
    const bool torque_on  = app_canRx_VC_TorqueVectoringEnabled_get();
    const bool shdn_ok    = (app_canRx_VC_FirstFaultNode_get() == SHDN_OK);

    /* Worst‑status of each RGB board */
    BoardLEDStatus rsm_st  = worstBoardStatus(RSM_ALERT_BOARD);
    BoardLEDStatus crit_st = worstBoardStatus(CRIT_ALERT_BOARD);
    BoardLEDStatus fsm_st  = worstBoardStatus(FSM_ALERT_BOARD);
    BoardLEDStatus bms_st  = worstBoardStatus(BMS_ALERT_BOARD);
    BoardLEDStatus vc_st   = worstBoardStatus(VC_ALERT_BOARD);
    BoardLEDStatus dam_st  = worstBoardStatus(DAM_ALERT_BOARD);

    /* Build the 32‑bit LED word */
    LedReg_t leds = { .value = 0 };

    /* Set each RGB board’s LEDs in place */
    SET_BOARD_RGB(rsm, rsm_st);
    SET_BOARD_RGB(crit, crit_st);
    SET_BOARD_RGB(fsm, fsm_st);
    SET_BOARD_RGB(bms, bms_st);
    SET_BOARD_RGB(vc, vc_st);
    SET_BOARD_RGB(dam, dam_st);

    /* Now the single‑bit flags */
    if (push_drive)
        leds.bits.push_r = 1;
    if (imd_fault)
        leds.bits.imd = 1;
    if (bspd_fault)
        leds.bits.bspd = 1;
    if (ams_fault)
        leds.bits.ams = 1;
    if (regen_on)
        leds.bits.regen = 1;
    if (torque_on)
        leds.bits.torque = 1;
    if (shdn_ok)
        leds.bits.shdn_r = 1;

    /* Shift out all 4 bytes (LSB first) */
    io_shift_register_updateLedRegisters((uint8_t *)&leds.value);
}
