#include "io_leds.h"
#include "app_canRx.h"
#include <assert.h>

static const Leds *leds = NULL;

void io_led_init(const Leds *in_leds)
{
    leds = in_leds;
}

void io_led_imd_set(bool val)
{
    io_led_enable(leds->imd_led, val);
}

void io_led_bspd_set(bool val)
{
    io_led_enable(leds->bspd_led, val);
}

void io_led_ams_set(bool val)
{
    io_led_enable(leds->ams_led, val);
}

void io_led_start_set(bool val)
{
    io_led_enable(leds->start_led, val);
}

void io_led_regen_set(bool val)
{
    io_led_enable(leds->regen_led, val);
}

void io_led_torquevec_set(bool val)
{
    io_led_enable(leds->torquevec_led, val);
}

void set_led_from_board_status(const RgbLed *led, const BoardLEDStatus status)
{
    switch (status)
    {
        case FAULT:
            hw_rgbLed_enable(led, RGB_RED);
            break;
        case OK:
            hw_rgbLed_enable(led, RGB_GREEN);
            break;
        case WARNING:
            hw_rgbLed_enable(led, RGB_YELLOW);
            break;
        case WHITE:
            hw_rgbLed_enable(led, RGB_WHITE);
            break;
    }
}

void io_led_shutdown_set(const BoardLEDStatus status)
{
    assert(status != WHITE && status != WARNING);
    set_led_from_board_status(leds->shdn_led, status);
}

void io_led_bms_status_set(const BoardLEDStatus status)
{
    set_led_from_board_status(leds->bms_status_led, status);
}
void io_led_fsm_status_set(const BoardLEDStatus status)
{
    set_led_from_board_status(leds->fsm_status_led, status);
}
void io_led_vc_status_set(const BoardLEDStatus status)
{
    set_led_from_board_status(leds->vc_status_led, status);
}
void io_led_aux_status_set(const BoardLEDStatus status)
{
    set_led_from_board_status(leds->aux_status_led, status);
}
void io_led_crit_status_set(const BoardLEDStatus status)
{
    set_led_from_board_status(leds->crit_status_led, status);
}
void io_led_rsm_status_set(const BoardLEDStatus status)
{
    set_led_from_board_status(leds->rsm_status_led, status);
}
