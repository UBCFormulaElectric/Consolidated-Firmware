#include "app_screens.h"

static uint8_t main_drive_display_data[] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

static void main_drive_ccw(void) {}

static void main_drive_cw(void) {}

static void main_drive_update(void) {}

Screen main_drive = { .display_data = main_drive_display_data,
                      .ccw_callback = main_drive_ccw,
                      .cw_callback  = main_drive_cw,
                      .update       = main_drive_update };
