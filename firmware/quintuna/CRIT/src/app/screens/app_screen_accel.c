#include "app_screens.h"

static uint8_t accel_display_data[] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

static void accel_ccw(void) {}

static void accel_cw(void) {}

static void accel_update(void) {}

Screen accel = { .display_data = accel_display_data,
                 .ccw_callback = accel_ccw,
                 .cw_callback  = accel_cw,
                 .update       = accel_update };
