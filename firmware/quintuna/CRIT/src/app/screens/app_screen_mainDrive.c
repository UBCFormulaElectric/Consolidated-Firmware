#include "app_screens.h"

static uint8_t main_drive_display_data[] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

static void main_screen_ccw(void) {}

static void main_screen_cw(void) {}

Screen screen0 = { .display_data = main_drive_display_data,
                   .ccw_callback = main_screen_ccw,
                   .cw_callback  = main_screen_cw };
