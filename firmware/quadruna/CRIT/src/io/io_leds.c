#include "io_leds.h"
#include "app_canRx.h"

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

void io_led_shutdown_set(bool val)
{
    io_led_enable(leds->shdn_led, val);
}

void cringe(const RgbLed *led, const BoardLEDStatus status)
{
    switch (status)
    {
        case FAULT:
            io_rgbLed_enable(led, true, false, false);
            break;
        case OK:
            io_rgbLed_enable(led, false, true, false);
            break;
        case WARNING:
            io_rgbLed_enable(led, false, false, true);
            break;
        case WHITE:
            io_rgbLed_enable(led, true, true, true);
            break;
    }
}

void io_led_bms_status_set(const BoardLEDStatus status)
{
    cringe(leds->bms_status_led, status);
}
void io_led_fsm_status_set(const BoardLEDStatus status)
{
    cringe(leds->fsm_status_led, status);
}
void io_led_vc_status_set(const BoardLEDStatus status)
{
    cringe(leds->vc_status_led, status);
}
void io_led_aux_status_set(const BoardLEDStatus status)
{
    cringe(leds->aux_status_led, status);
}
void io_led_crit_status_set(const BoardLEDStatus status)
{
    cringe(leds->crit_status_led, status);
}
void io_led_rsm_status_set(const BoardLEDStatus status)
{
    cringe(leds->rsm_status_led, status);
}
