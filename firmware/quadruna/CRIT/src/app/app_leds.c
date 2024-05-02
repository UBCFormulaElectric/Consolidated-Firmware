#include "app_leds.h"
#include "app_canRx.h"
#include "app_canAlerts.h"

static const Leds *leds = NULL;

void app_led_init(const Leds *in_leds)
{
    leds = in_leds;
}

void app_leds_update(void)
{
    if (leds == NULL)
    {
        return;
    }

    const bool imd_fault_latched = app_canRx_BMS_ImdLatchedFault_get();
    io_led_enable(leds->imd_led, imd_fault_latched);

    const bool bspd_fault_latched = app_canRx_BMS_BspdLatchedFault_get();
    io_led_enable(leds->bspd_led, bspd_fault_latched);

    const bool ams_fault_latched = app_canRx_BMS_BmsLatchedFault_get();
    io_led_enable(leds->ams_led, ams_fault_latched);

    const bool shutdown_sensor = false; // TODO implement when SigmaWrath's changes are in
    io_led_enable(leds->shdn_led, shutdown_sensor);

    const bool start_led_on = app_canRx_VC_State_get() == VC_DRIVE_STATE;
    io_led_enable(leds->start_led, start_led_on);

    const bool regen_light_on = app_canRx_VC_RegenEnabled_get();
    io_led_enable(leds->regen_led, regen_light_on);

    const bool torquevec_light_on = app_canRx_VC_TorqueVectoringEnabled_get();
    io_led_enable(leds->torquevec_led, torquevec_light_on);

    typedef struct
    {
        const RgbLed *led;
        CanAlertBoard jsoncan_board_enum;
    } LedCAN;

    const LedCAN boards[NUM_BOARD_LEDS] = {
        { .led = leds->bms_status_led, .jsoncan_board_enum = BMS_ALERT_BOARD },
        { .led = leds->fsm_status_led, .jsoncan_board_enum = FSM_ALERT_BOARD },
        { .led = leds->vc_status_led, .jsoncan_board_enum = VC_ALERT_BOARD },
        { .led = leds->aux_status_led },
        { .led = leds->crit_status_led, .jsoncan_board_enum = CRIT_ALERT_BOARD },
        { .led = leds->rsm_status_led },
    };

    for (size_t i = 0; i < NUM_BOARD_LEDS; i++)
    {
        const RgbLed *board_status_led = boards[i].led;
        CanAlertBoard alert_board_ids  = boards[i].jsoncan_board_enum;

        if (app_canAlerts_BoardHasFault(alert_board_ids))
        {
            // Turn red.
            io_rgbLed_enable(board_status_led, true, false, false);
        }
        else if (app_canAlerts_BoardHasWarning(alert_board_ids))
        {
            // Turn blue.
            io_rgbLed_enable(board_status_led, false, false, true);
        }
        else
        {
            // Turn green.
            io_rgbLed_enable(board_status_led, false, true, false);
        }
    }
}
